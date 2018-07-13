/*
 * AppCallback.h
 *
 *  Created on: Nov 8, 2017
 *      Author: RocketBoy
 */

#ifndef THONV_APP_APPCALLBACK_H_
#define THONV_APP_APPCALLBACK_H_

#include "rail_types.h"
#include "stdint.h"


void StartupApp (void);
void AppMain(void);


void RFSendTestPacketEventCallback(void);
void StartupConfigRFEventCallback(void);
void RxCback(int8_t RSSI);
#endif /* THONV_APP_APPCALLBACK_H_ */
