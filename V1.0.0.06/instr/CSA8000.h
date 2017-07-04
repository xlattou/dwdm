/****************************************************************************
 *
 * File:                csa8000.h
 *
 * Author:              Roger Li
 *
 * Description:         csa8000基本功能代码
 *
 * Time:                2009-12-07
 *
 * version:				v1.0.0.0
 *
****************************************************************************/
#ifndef _CSA8000_H_
#define _CSA8000_H_ 

#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <visa.h>

struct struCSA8000
{
char    MainSN[50];
char    ModuleSN[50];
float	X_Scale;
float  	X_Position;
float	Y_Scale;
float  	Y_Position;
float	Y_Offset;
char	Filter[50];
int		Wavelength;
int		MaskMargin;
char    MaskName[50];
int		Channel_O;  
};

extern BOOL CSA8000_Init(ViSession *instHandle, ViRsrc Viname);  
extern BOOL CSA8000_Set_O(ViSession instHandle, int Channel_O, double X_Scale, double X_Position, double Y_Scale, double Y_Position, double Offset, int Wavelength, char *Filter, char *MaskName);  //光口部分设置
extern BOOL CSA8000_Set_E(ViSession instHandle); //电口部分设置
extern BOOL CSA8000_GET_Er(ViSession instHandle, double *Er);
extern BOOL CSA8000_Set_Clear(ViSession instHandle);
extern BOOL CSA8000_SET_AUTOSet_EXECute(ViSession instHandle); 
extern BOOL CSA8000_SET_AUTOSet_UNDo(ViSession instHandle); 
extern BOOL CSA8000_GET_O_PPj(ViSession instHandle, double *PPj); 
extern BOOL CSA8000_GET_O_RSj(ViSession instHandle, double *RSj);
extern BOOL CSA8000_SET_MaskMargin(ViSession instHandle, double MaskMargin, char *MaskName);
extern BOOL CSA8000_GET_MaskCount(ViSession instHandle, int WaveForms);
extern BOOL CSA8000_GET_O_Fall(ViSession instHandle, double *Fall); 
extern BOOL CSA8000_GET_O_Rise(ViSession instHandle, double *Rise); 
extern BOOL CSA8000_GET_O_PCTCROss(ViSession instHandle, double *PCTCROss);

//获取主机序列号
extern BOOL CSA8000_GET_MainSN(ViSession instHandle, char *MainSN); 
//获取模块序列号
extern BOOL CSA8000_GET_ModuleSN(ViSession instHandle, char *ModuleSN); 

#endif  
