/***************************************************************************//**
 * @file
 * @brief Provide stdio UART1_RETARGETing to USART/UART or LEUART.
 * @version 5.2.1
 *******************************************************************************
 * # License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include <stdio.h>
#include "em_device.h"
#include "em_cmu.h"
#include "em_core.h"
#include "em_gpio.h"
#include "InitUart.h"
#include "InitUartConfig.h"


 /******************************************************************************/

#if defined(UART1_RETARGET_USART)
#include "em_usart.h"
#endif

#if defined(UART1_RETARGET_LEUART)
#include "em_leuart.h"
#endif

/* Receive buffer */
#ifndef RXBUFSIZE
#define RXBUFSIZE    8                          /**< Buffer size for RX */
#endif
static volatile int     rxReadIndex  = 0;       /**< Index in buffer to be read */
static volatile int     rxWriteIndex = 0;       /**< Index in buffer to be written to */
static volatile int     rxCount      = 0;       /**< Keeps track of how much data which are stored in the buffer */
static volatile uint8_t rxBuffer[RXBUFSIZE];    /**< Buffer to store data */
static uint8_t          LFtoCRLF    = 0;        /**< LF to CRLF conversion disabled */
static bool             initialized = false;    /**< Initialize UART/LEUART */

/**************************************************************************//**
 * @brief UART/LEUART IRQ Handler
 *****************************************************************************/
void UART1_RETARGET_IRQ_NAME(void)
{
#if defined(UART1_RETARGET_USART)
  if (UART1_RETARGET_UART->STATUS & USART_STATUS_RXDATAV) {
#else
  if (UART1_RETARGET_UART->IF & LEUART_IF_RXDATAV) {
#endif

    /* Store Data */
    rxBuffer[rxWriteIndex] = UART1_RETARGET_RX(UART1_RETARGET_UART);
    rxWriteIndex++;
    rxCount++;
    if (rxWriteIndex == RXBUFSIZE) {
      rxWriteIndex = 0;
    }
    /* Check for overflow - flush buffer */
    if (rxCount > RXBUFSIZE) {
      rxWriteIndex = 0;
      rxCount      = 0;
      rxReadIndex  = 0;
    }
  }
}

/** @} (end group UART1_RETARGETIo) */

/**************************************************************************//**
 * @brief UART/LEUART toggle LF to CRLF conversion
 * @param on If non-zero, automatic LF to CRLF conversion will be enabled
 *****************************************************************************/
void UART1_RETARGET_SerialCrLf(int on)
{
  if (on) {
    LFtoCRLF = 1;
  } else {
    LFtoCRLF = 0;
  }
}

/**************************************************************************//**
 * @brief Intializes UART/LEUART
 *****************************************************************************/
void UART1_RETARGET_SerialInit(void)
{
  /* Enable peripheral clocks */
  CMU_ClockEnable(cmuClock_HFPER, true);
  /* Configure GPIO pins */
  CMU_ClockEnable(cmuClock_GPIO, true);
  /* To avoid false start, configure output as high */
  GPIO_PinModeSet(UART1_RETARGET_TXPORT, UART1_RETARGET_TXPIN, gpioModePushPull, 1);
  GPIO_PinModeSet(UART1_RETARGET_RXPORT, UART1_RETARGET_RXPIN, gpioModeInput, 0);

#if defined(UART1_RETARGET_USART)
  USART_TypeDef           *usart = UART1_RETARGET_UART;
  USART_InitAsync_TypeDef init   = USART_INITASYNC_DEFAULT;

  /* Enable DK RS232/UART switch */
  UART1_RETARGET_PERIPHERAL_ENABLE();

  CMU_ClockEnable(UART1_RETARGET_CLK, true);

  /* Configure USART for basic async operation */
  init.enable = usartDisable;
  init.baudrate = 19200;
  USART_InitAsync(usart, &init);

  /* Enable pins at correct UART/USART location. */
  #if defined(USART_ROUTEPEN_RXPEN)
  usart->ROUTEPEN = USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;
  usart->ROUTELOC0 = (usart->ROUTELOC0
                      & ~(_USART_ROUTELOC0_TXLOC_MASK
                          | _USART_ROUTELOC0_RXLOC_MASK) )
                     | (UART1_RETARGET_TX_LOCATION << _USART_ROUTELOC0_TXLOC_SHIFT)
                     | (UART1_RETARGET_RX_LOCATION << _USART_ROUTELOC0_RXLOC_SHIFT);
  #else
  usart->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | UART1_RETARGET_LOCATION;
  #endif

  /* Clear previous RX interrupts */
  USART_IntClear(UART1_RETARGET_UART, USART_IF_RXDATAV);
  NVIC_ClearPendingIRQ(UART1_RETARGET_IRQn);

  /* Enable RX interrupts */
  USART_IntEnable(UART1_RETARGET_UART, USART_IF_RXDATAV);
  NVIC_EnableIRQ(UART1_RETARGET_IRQn);

  /* Finally enable it */
  USART_Enable(usart, usartEnable);

#else
  LEUART_TypeDef      *leuart = UART1_RETARGET_UART;
  LEUART_Init_TypeDef init    = LEUART_INIT_DEFAULT;

  /* Enable DK LEUART/RS232 switch */
  UART1_RETARGET_PERIPHERAL_ENABLE();

  /* Enable CORE LE clock in order to access LE modules */
  CMU_ClockEnable(cmuClock_CORELE, true);

#if defined(UART1_RETARGET_VCOM)
  /* Select HFXO/2 for LEUARTs (and wait for it to stabilize) */
#if defined(_CMU_LFCLKSEL_LFB_HFCORECLKLEDIV2)
  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_CORELEDIV2);
#else
  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_HFCLKLE);
#endif
#else
  /* Select LFXO for LEUARTs (and wait for it to stabilize) */
  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
#endif

  CMU_ClockEnable(UART1_RETARGET_CLK, true);

  /* Do not prescale clock */
  CMU_ClockDivSet(UART1_RETARGET_CLK, cmuClkDiv_1);

  /* Configure LEUART */
  init.enable = leuartDisable;
#if defined(UART1_RETARGET_VCOM)
  init.baudrate = 115200;
#endif
  init.baudrate = 19200;
  LEUART_Init(leuart, &init);
  /* Enable pins at default location */
  #if defined(LEUART_ROUTEPEN_RXPEN)
  leuart->ROUTEPEN = USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;
  leuart->ROUTELOC0 = (leuart->ROUTELOC0
                       & ~(_LEUART_ROUTELOC0_TXLOC_MASK
                           | _LEUART_ROUTELOC0_RXLOC_MASK) )
                      | (UART1_RETARGET_TX_LOCATION << _LEUART_ROUTELOC0_TXLOC_SHIFT)
                      | (UART1_RETARGET_RX_LOCATION << _LEUART_ROUTELOC0_RXLOC_SHIFT);
  #else
  leuart->ROUTE = LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN | UART1_RETARGET_LOCATION;
  #endif

  /* Clear previous RX interrupts */
  LEUART_IntClear(UART1_RETARGET_UART, LEUART_IF_RXDATAV);
  NVIC_ClearPendingIRQ(UART1_RETARGET_IRQn);

  /* Enable RX interrupts */
  LEUART_IntEnable(UART1_RETARGET_UART, LEUART_IF_RXDATAV);
  NVIC_EnableIRQ(UART1_RETARGET_IRQn);

  /* Finally enable it */
  LEUART_Enable(leuart, leuartEnable);
#endif

#if !defined(__CROSSWORKS_ARM) && defined(__GNUC__)
  setvbuf(stdout, NULL, _IONBF, 0);   /*Set unbuffered mode for stdout (newlib)*/
#endif

  initialized = true;
}

/**************************************************************************//**
 * @brief Receive a byte from USART/LEUART and put into global buffer
 * @return -1 on failure, or positive character integer on sucesss
 *****************************************************************************/
int UART1_RETARGET_ReadChar(void)
{
  int c = -1;
  CORE_DECLARE_IRQ_STATE;

  if (initialized == false) {
    UART1_RETARGET_SerialInit();
  }

  CORE_ENTER_ATOMIC();
  if (rxCount > 0) {
    c = rxBuffer[rxReadIndex];
    rxReadIndex++;
    if (rxReadIndex == RXBUFSIZE) {
      rxReadIndex = 0;
    }
    rxCount--;
  }
  CORE_EXIT_ATOMIC();

  return c;
}

/**************************************************************************//**
 * @brief Transmit single byte to USART/LEUART
 * @param c Character to transmit
 * @return Transmitted character
 *****************************************************************************/
int UART1_RETARGET_WriteChar(char c)
{
  if (initialized == false) {
    UART1_RETARGET_SerialInit();
  }

  /* Add CR or LF to CRLF if enabled */
  if (LFtoCRLF && (c == '\n')) {
    UART1_RETARGET_TX(UART1_RETARGET_UART, '\r');
  }
  UART1_RETARGET_TX(UART1_RETARGET_UART, c);

  if (LFtoCRLF && (c == '\r')) {
    UART1_RETARGET_TX(UART1_RETARGET_UART, '\n');
  }

  return c;
}

void emberSerialWriteData(unsigned char* Data, unsigned char Length){
	unsigned char i;
	for(i=0; i<Length;i++){
		(void)UART1_RETARGET_WriteChar(Data[i]);
	}
}

