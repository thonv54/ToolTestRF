
//-----------------------------Khai bao thu vien---------------------//


#include "rail.h"
#include "hal/micro/cortexm3/compiler/iar.h"
#include "halDelay.h"

//-----------------------------Khai bao bien-------------------------//




int16u CommonGetDurationTime(int16u CommonBeforeTimer){
	int16u DurationTime;
	int16u CurrentTime;
	CurrentTime = (int16u)RAIL_GetTime();

	if( CurrentTime >= CommonBeforeTimer){
		DurationTime = CurrentTime - CommonBeforeTimer;
	}
	else{
		DurationTime = 0xFFFF - CommonBeforeTimer + CurrentTime;
	}
	return DurationTime;
}

void __delay_ms(int32u n){
	int32u StartDelayTimer;
	StartDelayTimer = RAIL_GetTime();
	while(CommonGetDurationTime(StartDelayTimer) < n){
		halResetWatchdog();
	}
}



