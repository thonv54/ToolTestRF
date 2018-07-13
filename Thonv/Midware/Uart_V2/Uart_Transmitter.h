



#ifndef __UART_TRANSMITTER_H__
#define __UART_TRANSMITTER_H__

#include "hal/micro/cortexm3/compiler/iar.h"

//------------------------------enum--------------------------//


//---------------------Define------------------//
#define NORMAL_TIMEOUT  0               // 0 delay
#define NormalTimeOut   0xFF            // timeout off UART message send is NormalTimeOut*MinimumTimeOut
#define MinimumTimeOut  100              //MinimumTimeOut for lag value 50ms
#define BlinkLedTimeOut 30

#define OverRxBuffer    0xFFFF

typedef struct{
    int8u DeviceNo;
    int8u DeviceType;
    int8u DeviceData[2];
}us_DeviceControl_Str;

#define CurtainControlId_Run  0x02
#define CurtainControlId_Stop 0x01

typedef enum{
    SwitchTypeDevice = 0x01,
    OutletTypeDevice = 0x02,
    CurtainTypeDevice = 0x03,
    DimmerTypeDevice = 0x04,
    FanTypeDevice     = 0x05,
}DeviceType_enum;
typedef enum {
    OffRelay = 0, OnRelay = 1, ToggleRelay = 2,
} SwitchState_enum;

typedef enum {
	_84_cmd_RefreshLed	= 0,
	_84_cmd_OnBlue		  = 1,
	_84_cmd_OnRed		    = 2,
	_84_cmd_OnPink		  = 3,
	_84_cmd_OffAll		  = 4,
	_84_cmd_BlinkBlue	  = 5,
	_84_cmd_BlinkRed	  = 6,
	_84_cmd_BlinkPink	  = 7,
} SpecialLedCmdEnum;

//-------------------------------Protocol----------------------//
int16u GetLastTxUartCmd(void);

//-------------------------------Protocol----------------------//
void UartSendSpecialLedMark(int16u LedNumberMark, int8u State);
void UartSendSpecialLedNumber(int8u LedNumber, int8u State);
void UartSendPinkLed(int16u LedNumberMark);
void UartSendOffLed(int16u LedNumberMark);
void UartSendBlinkLed(int16u LedNumberMark, int8u BlinkTime);
void UartSendSpecialLedControl(int16u LedMask, int8u State, int8u BlinkTimes, int8u LastState) ;
void UartSendToggleLed(int8u LedNumberMark);
void UartSendRefreshAllLed(void);
void UartSendRefreshLedNumber(int8u LedNumber);

void UartSendSwitchControlNumber(int8u SwitchNumber, int8u State);
void UartSendLevel(int8u DeviceNo, int8u DeviceData);
void UartSendCurtainStop(int8u DeviceNo);
void UartSendCheckMcReady(void);

void UartSendGetListTotalDevice(void);

void UartSendCommand(void);


void MC_UpdateStatusOfDevice(int8u *data, int8u length);
void MC_ListTotalDevice(int8u *data);
void MC_McUartError(int8u *data);

#endif

