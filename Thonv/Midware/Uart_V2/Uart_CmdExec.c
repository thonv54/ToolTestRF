

#include "hal/micro/cortexm3/compiler/iar.h"
#include "Uart_Transmitter.h"
#include "Uart_CmdExec.h"




/**
 * @func   MC_ListTotalDevice
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
void MC_ListTotalDevice(int8u *data){

}

/**
 * @func   \MC_UpdateStatusOfDevice
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
void MC_UpdateStatusOfDevice(int8u *data, int8u length){
    us_DeviceControl_Str DeviceControlVal;
    int8u RelayState;
    int8u DimmerLevel;
    int8u CurtainLevel;
    int8u FanLevel;
    int8u OnOffValue;
    *data ++;
    int8u SourceEndpoint;

    DeviceControlVal.DeviceNo = *data++;
    DeviceControlVal.DeviceType = *data++;
    DeviceControlVal.DeviceData[0] = *data++;
    DeviceControlVal.DeviceData[1] = *data++;

}

/**
 * @func   MC_TouchButtonHold
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */

void MC_TouchButtonHold(int8u *data){
    int8u ButtonId;
    int8u ButtonHoldType;
    *data ++;
    ButtonId = *data++;
    ButtonHoldType = *data++;

    switch (ButtonHoldType) {
    case ButtonUnpress:
        break;
    case ButtonShortHold:
        break;
    case ButtonLongHold:
        break;
    case ButtonResetHold:
        break;
    default:
        break;    
    }
}

/**
 * @func   MC_McUartError
 *
 * @brief  
 *
 * @param  None
 *
 * @retval None
 */
void MC_McUartError(int8u *data){
    
}



//-----------------------------***************--------------------------//


