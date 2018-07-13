
//-----------------------------Khai bao thu vien---------------------//

#include "hal/micro/cortexm3/compiler/iar.h"
#include "Thonv/Midware/HalDelay/halDelay.h"
#include "rail.h"
#include "system-timer.h"
#include "InitUart.h"

#include "Uart_Transmitter.h"
//-----------------------------Khai bao bien-------------------------//

int8u UART_PACKET_KEY[2] = { 0x4C, 0x4D };

int8u UartTxCommandBuff[512];
#define MAX_RX_BUFFER  512
int16u UartTxCommandStartWaitTimer = 0;
int16u UartTxCommandCurrentTimeOut = 0;

//---------------------------------UART Send Command--------------------------//



//------------------------------Ban tin yeu cau thong ke so luong thiet bi----------------//
/*
 Mau ban tin:

 CommandID          
 1byte              
 0x81
 */

void UartSendGetListTotalDevice(void) {
    int16u StartIndexOfTxBuff;
    int8u PacketDataSend[4];    
    int8u CheckXor = 0;
    int8u us_GetListTotalDevice_CommandId =0x81;
    int8u us_GetListTotalDevice_Length = 2;
    int8u us_GetListTotalDevice_TimeOut = NormalTimeOut;
    
    
    CheckXor = us_GetListTotalDevice_CommandId;
    PacketDataSend[0] = us_GetListTotalDevice_Length;
    PacketDataSend[1] = us_GetListTotalDevice_CommandId;
    PacketDataSend[2] = CheckXor;
    PacketDataSend[3] = us_GetListTotalDevice_TimeOut;
    StartIndexOfTxBuff = GetLastTxUartCmd();
    if(StartIndexOfTxBuff != OverRxBuffer){
        MEMCOPY(&UartTxCommandBuff[GetLastTxUartCmd()], PacketDataSend,us_GetListTotalDevice_Length +2);
    }
}

//------------------------------Ban tin yeu cau cap nhat trang thai thiet bi----------------//
/*
 Mau ban tin:

 CommandID           
 1byte              
 0x82
 */

void UartSendGetStatusOfDevice(int16u DeviceMask) {
    int16u StartIndexOfTxBuff;
    int8u PacketDataSend[6];
    int8u us_GetStatusOfDevice_CommandId = 0x82;
    int8u us_GetStatusOfDevice_Length = 4;    
    int8u us_GetStatusOfDevice_TimeOut = NormalTimeOut;
    int8u CheckXor =0;

    CheckXor = us_GetStatusOfDevice_CommandId ^ LOW_BYTE(DeviceMask) ^ HIGH_BYTE(DeviceMask);

    PacketDataSend[0] = us_GetStatusOfDevice_Length;
    PacketDataSend[1] = us_GetStatusOfDevice_CommandId;
    PacketDataSend[2] = HIGH_BYTE(DeviceMask);
    PacketDataSend[3] = LOW_BYTE(DeviceMask);
    PacketDataSend[4] = CheckXor;
    PacketDataSend[5] = us_GetStatusOfDevice_TimeOut;
    StartIndexOfTxBuff = GetLastTxUartCmd();
    if(StartIndexOfTxBuff != OverRxBuffer){
        MEMCOPY(&UartTxCommandBuff[GetLastTxUartCmd()], PacketDataSend,us_GetStatusOfDevice_Length +2);
    }
}
//------------------------------Ban tin dieu khien thiet bi--------------------------------//
/*
 Mau ban tin:

 CommandID           
 1byte              
 0x83
 */

void UartSendDeviceControl(int8u NumberOfDevice, us_DeviceControl_Str* DeviceControlVal) {
    int16u StartIndexOfTxBuff;
    int8u PacketDataSend[68];
    int8u CheckXor = 0;
    int8u us_DeviceControl_CommandId = 0x83;
    int8u us_DeviceControl_Length = 0;
    int8u us_DeviceControl_TimeOut = NormalTimeOut;
    
    CheckXor = us_DeviceControl_CommandId;
    
    us_DeviceControl_Length = 2;
    
    for(int8u i =0;i< NumberOfDevice;i++){
        switch(DeviceControlVal[i].DeviceType){
            case SwitchTypeDevice:
            case DimmerTypeDevice:
            case FanTypeDevice:
                us_DeviceControl_Length += 3;
                CheckXor ^= DeviceControlVal[i].DeviceNo 
                    ^ DeviceControlVal[i].DeviceType
                    ^ DeviceControlVal[i].DeviceData[0];
                PacketDataSend[2+3*i] = DeviceControlVal[i].DeviceNo;
                PacketDataSend[3+3*i] = DeviceControlVal[i].DeviceType;
                PacketDataSend[4+3*i] = DeviceControlVal[i].DeviceData[0];
                break;
                
            case CurtainTypeDevice:
                us_DeviceControl_Length += 4;
                CheckXor ^= DeviceControlVal[i].DeviceNo 
                    ^ DeviceControlVal[i].DeviceType
                    ^ DeviceControlVal[i].DeviceData[0]
                    ^ DeviceControlVal[i].DeviceData[1];
                PacketDataSend[2+3*i] = DeviceControlVal[i].DeviceNo;
                PacketDataSend[3+3*i] = DeviceControlVal[i].DeviceType;
                PacketDataSend[4+3*i] = DeviceControlVal[i].DeviceData[0];
                PacketDataSend[5+3*i] = DeviceControlVal[i].DeviceData[1];
                break;
            
            break;
            default:
                break;
        }
    }
    PacketDataSend[0] = us_DeviceControl_Length;
    PacketDataSend[1] = us_DeviceControl_CommandId;
    
    PacketDataSend[us_DeviceControl_Length] = CheckXor;
    PacketDataSend[us_DeviceControl_Length+1] = us_DeviceControl_TimeOut;
    StartIndexOfTxBuff = GetLastTxUartCmd();
    if(StartIndexOfTxBuff != OverRxBuffer){
        MEMCOPY(&UartTxCommandBuff[GetLastTxUartCmd()], PacketDataSend,us_DeviceControl_Length +2);
    }
}

void UartSendSwitchControlNumber(int8u SwitchNumber, int8u State){
    int16u StartIndexOfTxBuff;
    us_DeviceControl_Str DeviceControlVal;
    DeviceControlVal.DeviceNo = SwitchNumber;
    DeviceControlVal.DeviceType = SwitchTypeDevice;
    DeviceControlVal.DeviceData[0] = State;
    StartIndexOfTxBuff = GetLastTxUartCmd();
    if(StartIndexOfTxBuff != OverRxBuffer){
        UartSendDeviceControl(1,(us_DeviceControl_Str*)&DeviceControlVal);
    }
}
void UartSendDimLevel(int8u DeviceNo, int8u DimLevel){
    int16u StartIndexOfTxBuff;
    us_DeviceControl_Str DeviceControlVal;
    DeviceControlVal.DeviceNo = DeviceNo;
    DeviceControlVal.DeviceType = DimmerTypeDevice;
    DeviceControlVal.DeviceData[0] = DimLevel;
    StartIndexOfTxBuff = GetLastTxUartCmd();
    if(StartIndexOfTxBuff != OverRxBuffer){
        UartSendDeviceControl(1,(us_DeviceControl_Str*)&DeviceControlVal);
    }    
}

void UartSendFanLevel(int8u DeviceNo, int8u FanLevel){
    int16u StartIndexOfTxBuff;
    us_DeviceControl_Str DeviceControlVal;
    DeviceControlVal.DeviceNo = DeviceNo;
    DeviceControlVal.DeviceType = FanTypeDevice;
    DeviceControlVal.DeviceData[0] = FanLevel;
    StartIndexOfTxBuff = GetLastTxUartCmd();
    if(StartIndexOfTxBuff != OverRxBuffer){
        UartSendDeviceControl(1,(us_DeviceControl_Str*)&DeviceControlVal);
    }    
}
void UartSendCurtainLevel(int8u DeviceNo, int8u CurtainLevel){
    int16u StartIndexOfTxBuff;
    us_DeviceControl_Str DeviceControlVal;
    DeviceControlVal.DeviceNo = DeviceNo;
    DeviceControlVal.DeviceType = CurtainTypeDevice;
    DeviceControlVal.DeviceData[0] = CurtainControlId_Run ;
    DeviceControlVal.DeviceData[1] =  CurtainLevel;
    StartIndexOfTxBuff = GetLastTxUartCmd();
    if(StartIndexOfTxBuff != OverRxBuffer){
        UartSendDeviceControl(1,(us_DeviceControl_Str*)&DeviceControlVal);
    }    
}
void UartSendCurtainStop(int8u DeviceNo){
    int16u StartIndexOfTxBuff;
    us_DeviceControl_Str DeviceControlVal;
    DeviceControlVal.DeviceNo = DeviceNo;
    DeviceControlVal.DeviceType = CurtainTypeDevice;
    DeviceControlVal.DeviceData[0] = CurtainControlId_Stop ;
    DeviceControlVal.DeviceData[1] =  0;
    StartIndexOfTxBuff = GetLastTxUartCmd();
    if(StartIndexOfTxBuff != OverRxBuffer){
        UartSendDeviceControl(1,(us_DeviceControl_Str*)&DeviceControlVal);
    }    
}


void UartSendLevel(int8u DeviceNo, int8u DeviceData){
#ifdef DIMMER
    UartSendDimLevel(DeviceNo, DeviceData);
#endif
#ifdef DUAL_DIMMER
    UartSendDimLevel(DeviceNo, DeviceData);
#endif
#ifdef CURTAIN
    UartSendCurtainLevel(DeviceNo, DeviceData);
#endif
#ifdef DUAL_CURTAIN
    UartSendCurtainLevel(DeviceNo, DeviceData);
#endif
#ifdef FAN
    UartSendFanLevel(DeviceNo, DeviceData);
#endif

}
//------------------------------Ban tin dieu khien Led sang trang thai dac biet-----------//
/*
 Mau ban tin:

 CommandID           
 1byte              
 0x84
 */
void UartSendSpecialLedControl(int16u LedMask, int8u State, int8u BlinkTimes, int8u LastState) {
    int16u StartIndexOfTxBuff;
    int8u PacketDataSend[9];
    int8u CheckXor;
    int8u us_SpecialLedControl_CommandId = 0x84;
    int8u us_SpecialLedControl_Length = 7;
    int8u us_SpecialLedControl_TimeOut;
    
    CheckXor = us_SpecialLedControl_CommandId
            ^ HIGH_BYTE(LedMask)
            ^ LOW_BYTE(LedMask)
            ^ State
            ^ BlinkTimes
            ^ LastState;
    PacketDataSend[0] = us_SpecialLedControl_Length;
    PacketDataSend[1] = us_SpecialLedControl_CommandId;
    PacketDataSend[2] = HIGH_BYTE(LedMask);
    PacketDataSend[3] = LOW_BYTE(LedMask);
    PacketDataSend[4] = State;
    PacketDataSend[5] = BlinkTimes;
    PacketDataSend[6] = LastState;
    PacketDataSend[7] = CheckXor;
    if(State >=_84_cmd_BlinkBlue){
        us_SpecialLedControl_TimeOut = BlinkLedTimeOut;
    }
    else{
        us_SpecialLedControl_TimeOut = NormalTimeOut;
    }
    PacketDataSend[8] = us_SpecialLedControl_TimeOut;
    StartIndexOfTxBuff = GetLastTxUartCmd();
    if(StartIndexOfTxBuff != OverRxBuffer){
        MEMCOPY(&UartTxCommandBuff[GetLastTxUartCmd()], PacketDataSend,us_SpecialLedControl_Length +2);
    }
}
/**
 * @func   UartSendPinkLed
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
void UartSendPinkLed(int16u LedNumberMark){
    UartSendSpecialLedControl(LedNumberMark, _84_cmd_OnPink, 0 ,0);
}
void UartSendBlinkLed(int16u LedNumberMark, int8u BlinkTime){
    UartSendSpecialLedControl(LedNumberMark, _84_cmd_BlinkPink, BlinkTime , _84_cmd_RefreshLed);
}
void UartSendRefreshAllLed(void){
    UartSendSpecialLedControl(0xFFFF,_84_cmd_RefreshLed,0,0);
}
void UartSendSpecialLedNumber(int8u LedNumber, int8u State){
    UartSendSpecialLedControl((1<<(LedNumber - 1)), State,0,0);
}

//------------------------------Ban tin dieu khien Led sang trang thai dac biet-----------//
/*
 Mau ban tin:

 CommandID           
 1byte              
 0x84
 */
void UartSendMcUartError(int8u ErrorCode) {
    int16u StartIndexOfTxBuff;
    int8u PacketDataSend[5];
    int8u CheckXor;
    int8u us_McUartError_CommandId = 0x85;
    int8u us_McUartError_Length = 3;
    int8u us_McUartError_TimeOut = NormalTimeOut;
    
    CheckXor = us_McUartError_CommandId ^ ErrorCode;
    PacketDataSend[0] = us_McUartError_Length;
    PacketDataSend[1] = us_McUartError_CommandId;
    PacketDataSend[2] = ErrorCode;
    PacketDataSend[3] = CheckXor;
    PacketDataSend[4] = us_McUartError_TimeOut;
    StartIndexOfTxBuff = GetLastTxUartCmd();
    if(StartIndexOfTxBuff != OverRxBuffer){
        MEMCOPY(&UartTxCommandBuff[StartIndexOfTxBuff], PacketDataSend,us_McUartError_Length +2);
    }
}


//----------------------------UART Send Command Utility-----------------------//

int16u GetLastTxUartCmd(void) {
    int16u Head = 0;
    int16u Length = 0;
    while (UartTxCommandBuff[Head] != 0) {
        if (UartTxCommandBuff[Head] != 0) {
            Length = UartTxCommandBuff[Head];
            Head = Head + Length + 2;
        }
    }
    if(Head > 500){
        return OverRxBuffer;
    }
    return Head;
}




//---------------------------------Check Uart Status And Send Quere Data Buffer-------------------------------//
typedef enum {
    CmdReady, CmdBusy,
} uartSendCommandStatus_enum;

int8u CheckWaitTimeOut(void) {

    if (UartTxCommandStartWaitTimer != 0) {
        if (CommonGetDurationTime(UartTxCommandStartWaitTimer)
                >= UartTxCommandCurrentTimeOut) {
            //clear timer
            UartTxCommandStartWaitTimer = 0;
            return CmdReady;
        }
        else {
            return CmdBusy;
        }
    }
    else {
        return CmdReady;
    }
}
void UartSendCommand(void) {
    //  Check buff
    if (UartTxCommandBuff[0] != 0) {
        //  Check Timeout

        switch (CheckWaitTimeOut()) {
        case CmdBusy:
            break;
        case CmdReady:
            //  Send buff
            if (UartTxCommandBuff[UartTxCommandBuff[0] + 1] == NormalTimeOut) {
                UartTxCommandCurrentTimeOut = NORMAL_TIMEOUT*MinimumTimeOut;
            }
            else {
                UartTxCommandCurrentTimeOut =
                        UartTxCommandBuff[UartTxCommandBuff[0] + 1]
                                * MinimumTimeOut;
            }
            emberSerialWriteData( UART_PACKET_KEY, 2);
            emberSerialWriteData( UartTxCommandBuff,
                    (UartTxCommandBuff[0] + 1));
            MEMCOPY(UartTxCommandBuff,
                    &UartTxCommandBuff[UartTxCommandBuff[0] + 2],
                    (MAX_RX_BUFFER - (UartTxCommandBuff[0] + 2)));
            UartTxCommandStartWaitTimer =
					(int16u) RAIL_GetTime();
            break;
        default:
            break;
        }
    }
}
//----------------------------------------------END--------------------------------------//

