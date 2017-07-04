#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <visa.h>

#ifndef _OLA55M_H_
#define _OLA55M_H_

//ATT mode 
enum emum_OLA55M_Mode
{  
	OLA55M_MODE_ATT,
	OLA55M_MODE_ALC
} OLA55M_MODE;

extern BOOL OLA55M_Init(ViSession *instHandle, int COM, int WaveLength, int ATT_MODE);  
extern BOOL OLA55M_Set_WaveLength(ViSession instHandle, int WaveLength);
extern BOOL OLA55M_Set_ATTValue(ViSession instHandle, float Value);  
extern BOOL OLA55M_Set_ALCValue(ViSession instHandle, float Value); 
extern BOOL OLA55M_Get_Power(ViSession instHandle, float *Power);  

#endif
