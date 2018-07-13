/*
 * InitUartConfig.h
 *
 *  Created on: Nov 8, 2017
 *      Author: RocketBoy
 */

#ifndef THONV_DRIVER_INITUARTCONFIG_H_
#define THONV_DRIVER_INITUARTCONFIG_H_

#include "bsp.h"

/***************************************************************************//**
 *
 * When UART1_RETARGETing serial output the user can choose which peripheral
 * to use as the serial output device. This choice is made by configuring
 * one or more of the following defines: UART1_RETARGET_USART1, UART1_RETARGET_LEUART0,
 * UART1_RETARGET_VCOM.
 *
 * This table shows the supported configurations and the resulting serial
 * output device.
 *
 * +----------------------------------------------------------------------+
 * | Defines                            | Serial Output (Locations)       |
 * |----------------------------------------------------------------------+
 * | None                               | USART1 (Rx #0, Tx #0)           |
 * | UART1_RETARGET_USART1                    | USART1 (Rx #0, Tx #0)           |
 * | UART1_RETARGET_VCOM                      | VCOM using USART1               |
 * | UART1_RETARGET_LEUART0                   | LEUART0 (Rx #0, Tx #0)          |
 * | UART1_RETARGET_LEUART0 and UART1_RETARGET_VCOM | VCOM using LEUART0              |
 * +----------------------------------------------------------------------+
 *
 * Note that the default configuration is the same as UART1_RETARGET_USART1.
 *
 ******************************************************************************/

#if !defined(UART1_RETARGET_USART1) \
  && !defined(UART1_RETARGET_LEUART0)
#define UART1_RETARGET_USART1    /* Use USART1 by default. */
#endif

#if defined(UART1_RETARGET_USART1)
  #define UART1_RETARGET_IRQ_NAME    USART1_RX_IRQHandler         /* UART IRQ Handler */
  #define UART1_RETARGET_CLK         cmuClock_USART1              /* HFPER Clock */
  #define UART1_RETARGET_UART        USART1                       /* UART instance */
  #define UART1_RETARGET_IRQn        USART1_RX_IRQn               /* IRQ number */
  #define UART1_RETARGET_TX          USART_Tx                     /* Set TX to USART_Tx */
  #define UART1_RETARGET_RX          USART_Rx                     /* Set RX to USART_Rx */
  #define UART1_RETARGET_TX_LOCATION _USART_ROUTELOC0_TXLOC_LOC0  /* Location of of USART TX pin */
  #define UART1_RETARGET_RX_LOCATION _USART_ROUTELOC0_RXLOC_LOC0  /* Location of of USART RX pin */
  #define UART1_RETARGET_TXPORT      gpioPortA                    /* UART transmission port */
  #define UART1_RETARGET_TXPIN       0                            /* UART transmission pin */
  #define UART1_RETARGET_RXPORT      gpioPortA                    /* UART reception port */
  #define UART1_RETARGET_RXPIN       1                            /* UART reception pin */
  #define UART1_RETARGET_USART       1                            /* Includes em_usart.h */
  #define UART1_RETARGET_CTS_LOCATION _USART_ROUTELOC1_CTSLOC_LOC30
  #define UART1_RETARGET_RTS_LOCATION _USART_ROUTELOC1_RTSLOC_LOC30
  #define UART1_RETARGET_CTSPORT      gpioPortA
  #define UART1_RETARGET_CTSPIN       2
  #define UART1_RETARGET_RTSPORT      gpioPortA
  #define UART1_RETARGET_RTSPIN       3

#elif defined(UART1_RETARGET_LEUART0)
  #define UART1_RETARGET_IRQ_NAME    LEUART0_IRQHandler           /* LEUART IRQ Handler */
  #define UART1_RETARGET_CLK         cmuClock_LEUART0             /* HFPER Clock */
  #define UART1_RETARGET_IRQn        LEUART0_IRQn                 /* IRQ number */
  #define UART1_RETARGET_UART        LEUART0                      /* LEUART instance */
  #define UART1_RETARGET_TX          LEUART_Tx                    /* Set TX to LEUART_Tx */
  #define UART1_RETARGET_RX          LEUART_Rx                    /* Set RX to LEUART_Rx */
  #define UART1_RETARGET_TX_LOCATION _LEUART_ROUTELOC0_TXLOC_LOC0 /* Location of of LEUART TX pin */
  #define UART1_RETARGET_RX_LOCATION _LEUART_ROUTELOC0_RXLOC_LOC0 /* Location of of LEUART RX pin */
  #define UART1_RETARGET_TXPORT      gpioPortC                    /* LEUART transmission port */
  #define UART1_RETARGET_TXPIN       10                            /* LEUART transmission pin */
  #define UART1_RETARGET_RXPORT      gpioPortC                    /* LEUART reception port */
  #define UART1_RETARGET_RXPIN       9                            /* LEUART reception pin */
  #define UART1_RETARGET_LEUART      1                            /* Includes em_leuart.h */

#else
#error "Illegal USART selection."
#endif

#if defined(UART1_RETARGET_VCOM)
  #define UART1_RETARGET_PERIPHERAL_ENABLE() \
  GPIO_PinModeSet(BSP_BCC_ENABLE_PORT, \
                  BSP_BCC_ENABLE_PIN,  \
                  gpioModePushPull,    \
                  1);
#else
  #define UART1_RETARGET_PERIPHERAL_ENABLE()
#endif

#endif /* THONV_DRIVER_INITUARTCONFIG_H_ */
