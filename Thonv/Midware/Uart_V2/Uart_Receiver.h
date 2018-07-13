#ifndef __UART_RECEIVER_H__
#define __UART_RECEIVER_H__






typedef enum {
    ListTotalDevice = 0x01,
    UpdateStatusOfDevice = 0x02,
    TouchButtonHold = 0x03,
    McUartError = 0x05,
} UartRxCommand_ID;



//---------------------Protocol------------------//

void UartProcess(unsigned char *UartPacketCommand ,unsigned char  Length);

void UartGetCommand(void);


//----------------------END-----------------------//






#endif




