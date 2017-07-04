#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <visa.h>

extern BOOL HP8156_Init(ViSession *instHandle, ViRsrc Viname, int Wavelength, float AttValue);  
extern BOOL HP8156_SET_ATT(ViSession instHandle, float AttValue);  
extern BOOL HP8156_SET_WaveLength(ViSession instHandle, int Wavelength) ;
