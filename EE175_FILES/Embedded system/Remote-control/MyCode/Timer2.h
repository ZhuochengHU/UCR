#ifndef __Timer2_H
#define __Timer2_H	

#include "sys.h"
/*初始化定时器2
例:TIM2_Int_Init(99,7199);10Khz的计数频率，计数到100为10ms 
想要定时为5ms则TIM2_Int_Init(49,7199);即可

TIM2_Int_Init(99,7199);

*/
void TIM2_Int_Init(u16 arr,u16 psc);

#endif
