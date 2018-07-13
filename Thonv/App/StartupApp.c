/*
 * StartupApp.c
 *
 *  Created on: Nov 8, 2017
 *      Author: RocketBoy
 */

#include "Thonv/App/AppCallback.h"
#include "Thonv/Driver/InitUart.h"


void StartupApp (void){

	  // Init App Uart
	  UART1_RETARGET_SerialInit();
	  UART1_RETARGET_SerialCrLf(0);
}
