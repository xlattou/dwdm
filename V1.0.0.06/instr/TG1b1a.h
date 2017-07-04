#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <visa.h>

extern BOOL TG1b1a_Init(ViSession *instHandle, ViRsrc Viname, int Freq, char *pattern);
extern BOOL TG1b1a_Phase(ViSession instHandle, int inv_flag, double berclass, int *phase);
extern BOOL TG1b1a_Start_Test(ViSession instHandle);
extern BOOL TG1b1a_End_Test(ViSession instHandle);
extern BOOL TG1b1a_GET_ErrorBit(ViSession instHandle, double *datacount, double *errorcount, double *elapsedtime, double *Ber);
extern BOOL TG1b1a_Config(ViSession instHandle, int Freq, int phase, int volamp, int inv_flag, char *pattern, char *maskspace, char *ierrrate);  
extern BOOL TG1b1a_Sweep_Time(ViSession instHandle, double time); 
extern BOOL TG1b1a_GET_BER(ViSession instHandle, double *Ber);
extern BOOL TG1b1a_Phase_Ber(ViSession instHandle, int *phase, double *ber);
extern BOOL TG1b1a_Set_Phase(ViSession instHandle, int phase);
extern BOOL TG1b1a_Get_Rate(ViSession instHandle, double *Rate);
extern BOOL TG1b1a_Align(ViSession instHandle, int *phase);



