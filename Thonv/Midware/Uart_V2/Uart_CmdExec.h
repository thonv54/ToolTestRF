#ifndef __UART_CMDEXEC_H__
#define __UART_CMDEXEC_H__

extern unsigned char IdicatorUartControl;

//------------------------------Enum----------------------//
typedef enum {
    RelayOff = 0, RelayOn = 1,
} RelayStateCmd_enum;

typedef enum {
    ButtonUnpress = 0,
    ButtonShortHold = 1,
    ButtonLongHold = 2,
    ButtonResetHold = 3,
} ButtonHoldCmd_enum;

//------------------------------Protocol------------------------//

void MC_TouchButtonHold(int8u *Data);

void MC_ListTotalDevice(int8u *data);
void MC_McUartError(int8u *data);
void MC_UpdateStatusOfDevice(int8u *data, int8u length);


#endif

