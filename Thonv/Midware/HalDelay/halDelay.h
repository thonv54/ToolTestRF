

#ifndef __HALDELAY_H__
#define __HALDELAY_H__

#include "hal/micro/cortexm3/compiler/iar.h"


int16u CommonGetDurationTime(int16u CommonBeforeTimer);
void __delay_ms(int32u n);



#endif
