/*
 * MainApp.c
 *
 *  Created on: Nov 8, 2017
 *      Author: RocketBoy
 */

#include "rail.h"
#include "stdio.h"
#include "stdint.h"
#include "rail_types.h"
#include "Thonv/App/AppCallback.h"
#include "Thonv/Midware/Uart_V2/Uart_Receiver.h"
#include "Thonv/Midware/Uart_V2/Uart_Transmitter.h"
#include "app_ci.h"

//#define ChanelFreq "2404000000"
//#define ChanelFreq "2440000000"
#define ChanelFreq "2477000000"

//#define ToolTest

#ifdef ToolTest
#undef DeviceTest
#else
#define DeviceTest
#endif

typedef enum {
	StartUpMode = 0, RunMode = 1,
} RunFlag_enum;

char Buf_1[50];
char Buf_2[50];
static unsigned char StartUpconfigStep = 0;
static unsigned char RunFlag = StartUpMode;
static unsigned long TimeStartEvent = 0;
#define TimeOutEachEventStep  500000   //5000ms

#define TimeOutEachTxPacket  3000000   //5000ms

void StartupConfigRF(void);
void Run(void);

void AppMain(void) {
	UartSendCommand();
	UartGetCommand();
	if (RunFlag == StartUpMode) {
		StartupConfigRF();
	} else if (RunFlag == RunMode) {
		Run();
	}
}

void RxCback(int8_t RSSI) {

	printf("\n\r rssi:%d \n\r", RSSI);
	if (RSSI > -50) {
		UartSendSpecialLedControl(0xFFFF, _84_cmd_BlinkPink, 5, _84_cmd_OnPink);
	} else if ((-70 < RSSI) && (RSSI < -50)) {
		UartSendSpecialLedControl(0xFFFF, _84_cmd_BlinkRed, 5, _84_cmd_OnRed);
	} else if (RSSI < -70) {
		UartSendSpecialLedControl(0xFFFF, _84_cmd_BlinkBlue, 5, _84_cmd_OnBlue);
	}
#ifdef DeviceTest
	char *Argv[20];
	sprintf(Buf_1, "tx");
	sprintf(Buf_2, "1");
	Argv[0] = Buf_1;
	Argv[1] = Buf_2;
	tx(2, Argv);

#endif
}
void Run(void) {
#ifdef ToolTest
	char *Argv[20];
	if(RAIL_GetTime() >= TimeStartEvent) {
		if (RAIL_GetTime() >= TimeStartEvent + TimeOutEachTxPacket) {
			sprintf(Buf_1, "tx");
			sprintf(Buf_2, "1");
			Argv[0] = Buf_1;
			Argv[1] = Buf_2;
			tx(2, Argv);
			TimeStartEvent = RAIL_GetTime();
		}
	}
	else {
		TimeStartEvent = RAIL_GetTime(); // tran bo dem timer
	}
#endif

}

void StartupConfigRF(void) {
	char *Argv[20];
	switch (StartUpconfigStep) {
	case 0:
		TimeStartEvent = RAIL_GetTime();
		// chuyen sang rx disable
		sprintf(Buf_1, "rx");
		sprintf(Buf_2, "0");
		Argv[0] = Buf_1;
		Argv[1] = Buf_2;
		rx(2, Argv);
		StartUpconfigStep++;
		break;

	case 1:
		// doi kenh truyen
		if (RAIL_GetTime() >= TimeStartEvent + TimeOutEachEventStep) {
			sprintf(Buf_1, "setDebugMode");
			sprintf(Buf_2, "1");
			Argv[0] = Buf_1;
			Argv[1] = Buf_2;
			setDebugMode(2, Argv);
			TimeStartEvent = RAIL_GetTime();
			StartUpconfigStep++;
		}
		break;
	case 2:
		// doi kenh truyen
		if (RAIL_GetTime() >= TimeStartEvent + TimeOutEachEventStep) {
			sprintf(Buf_1, "freqOverride");
			sprintf(Buf_2, ChanelFreq);
			Argv[0] = Buf_1;
			Argv[1] = Buf_2;
			setFrequency(2, Argv);
			TimeStartEvent = RAIL_GetTime();
			StartUpconfigStep++;
		}
		break;
	case 3:
		// doi cong suat phat
		if (RAIL_GetTime() >= TimeStartEvent + TimeOutEachEventStep) {
			sprintf(Buf_1, "setpower");
			sprintf(Buf_2, "10");
			Argv[0] = Buf_1;
			Argv[1] = Buf_2;
			setPower(2, Argv);
			TimeStartEvent = RAIL_GetTime();
			StartUpconfigStep++;
		}
		break;
	case 4:
		// chuyen sang rx enable
		if (RAIL_GetTime() >= TimeStartEvent + TimeOutEachEventStep) {
			sprintf(Buf_1, "rx");
			sprintf(Buf_2, "1");
			Argv[0] = Buf_1;
			Argv[1] = Buf_2;
			rx(2, Argv);
			TimeStartEvent = RAIL_GetTime();
			StartUpconfigStep++;
		}
		break;
	case 5:
		RunFlag = RunMode;
		break;
	default:
		break;
	}

}

void RFSendTest(void) {

}

////thonv
//{
//	  char i;
//	  for(i=0;i<argc;i++){
//		  printf("\n\r");
//		  printf("argc: %u", i);
//		  printf("\n\r");
//		  printf("argv[%u] :  ",i);
//		  printf(argv[i]);
//		  printf("\n\r");
//	  }
//}

