/*
 * InitUart.h
 *
 *  Created on: Nov 8, 2017
 *      Author: RocketBoy
 */

#ifndef THONV_DRIVER_INITUART_H_
#define THONV_DRIVER_INITUART_H_


#include "InitUartConfig.h"

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup UART1_RETARGETIo
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__CROSSWORKS_ARM)
int __putchar(int ch);
int __getchar(void);
#endif

int  UART1_RETARGET_ReadChar(void);
int  UART1_RETARGET_WriteChar(char c);

void UART1_RETARGET_SerialCrLf(int on);
void UART1_RETARGET_SerialInit(void);
void emberSerialWriteData(unsigned char* Data, unsigned char Length);

#ifdef __cplusplus
}
#endif

/** @} (end group UART1_RETARGETIo) */
/** @} (end group Drivers) */


#endif /* THONV_DRIVER_INITUART_H_ */
