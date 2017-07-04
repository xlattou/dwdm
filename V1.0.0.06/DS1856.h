/****************************************************************************
 *
 * File:                DS1856.h
 *
 * Author:              Superxon(Roger Li)
 *
 * Description:         DS1856基本功能代码
 *
 * Time:                2007-11-28
 *
 * version:				v1.0.0.2
 * version note			1.DS1856_SET_PassWord()函数中设置PW1密码为0xFF,0xFF,0xFF,0xFF
****************************************************************************/

#ifndef _DS1856_H_
#define _DS1856_H_

#include <windows.h>

//根据DS1856的datasheet定义EEPROM结构 
#pragma pack (1) /* set maximum alignment to 1 按一个字节对齐*/  

#pragma pack ()  /*取消自定义字节对齐方式*/

extern BOOL	 DS1856_SET_TEN(int USB_Handle, unsigned char DS1856_Slaadd, BYTE TEN);
extern BOOL  DS1856_SET_AEN(int USB_Handle, unsigned char DS1856_Slaadd, BYTE AEN);
extern BOOL  DS1856_GET_Mode(int USB_Handle, unsigned char DS1856_Slaadd, BYTE *TEN, BYTE *AEN); 
extern BOOL	 DS1856_SET_ADEN(int USB_Handle, unsigned char DS1856_Slaadd, BYTE ADEN); 
extern BOOL	 DS1856_SET_ADFIX(int USB_Handle, unsigned char DS1856_Slaadd, BYTE ADFIX);
extern BOOL  DS1856_SET_INV1(int USB_Handle, unsigned char DS1856_Slaadd, BYTE INV1); 
extern BOOL  DS1856_SET_INV2(int USB_Handle, unsigned char DS1856_Slaadd, BYTE INV2);
extern BOOL  DS1856_GET_Config(int USB_Handle, unsigned char DS1856_Slaadd, BYTE *ADEN, BYTE *ADFIX, BYTE *INV1, BYTE *INV2); 
extern BOOL	 DS1856_SET_RHIZ(int USB_Handle, unsigned char DS1856_Slaadd, BYTE RHIZ);
extern BOOL  DS1856_SET_Table(int USB_Handle, unsigned char DS1856_Slaadd, BYTE Table);
extern BOOL  DS1856_SET_PassWord(int USB_Handle, unsigned char DS1856_Slaadd, BYTE PW0, BYTE PW1, BYTE PW2, BYTE PW3);
extern BOOL  DS1856_ENTRY_PassWord(int USB_Handle, unsigned char DS1856_Slaadd, BYTE PW0, BYTE PW1, BYTE PW2, BYTE PW3);
extern BOOL	 DS1856_GET_Temperatrue(int USB_Handle, unsigned char DS1856_Slaadd, double *Temperatrue);
extern BOOL	 DS1856_GET_Vcc(int USB_Handle, unsigned char DS1856_Slaadd, double *Vcc);
extern BOOL  DS1856_GET_TxPower(int USB_Handle, unsigned char DS1856_SlaAdd, double *TxPower);
extern BOOL  DS1856_SET_LUT(int USB_Handle, unsigned char DS1856_Slaadd, BYTE Table, BYTE *LUT_arr);
extern BOOL	 DS1856_GET_ResFromRpos(int USB_Handle, unsigned char DS1856_Slaadd, BYTE DS1856_TYPE, BYTE H0, BYTE H1, double *Res0, double *Res1);  //Res 单位kohm
extern BOOL	 DS1856_GET_RposFromRes(int USB_Handle, unsigned char DS1856_Slaadd, BYTE DS1856_TYPE, double Res0, double Res1, BYTE *H0, BYTE *H1);  //Res 单位kohm
extern BOOL	 DS1856_GET_Status(int USB_Handle, unsigned char DS1856_Slaadd, BYTE *Status, BYTE *RHIZ, BYTE *TxF, BYTE *RxL, BYTE *Rdyb);
extern BOOL  DS1856_Chang_Addr(int USB_Handle, unsigned char DS1856_SlaAdd_OLD, unsigned char DS1856_SlaAdd_NEW);
//2009-06-11添加Ibais电流读取功能
extern BOOL	 DS1856_GET_Ibias(int USB_I2CHandle, unsigned char DS1856_Slaadd, double *Ibias); 

//2009-09-01 添加DS1856校准功能函数
extern int	DS1856_DDMI_GET_ADC (int USB_Handle, unsigned char DS1856_Slaadd, BYTE rom_add, unsigned short *ADC); 
extern int	DS1856_DDMI_SET_Gain (int USB_Handle, unsigned char DS1856_Slaadd, BYTE rom_add, unsigned short gain); 
extern int	DS1856_DDMI_SET_Offset (int USB_Handle, unsigned char DS1856_Slaadd, BYTE rom_add, unsigned short Offset);
extern int  DS1856_DDMI_GET_Gain(int USB_Handle, unsigned char DS1856_Slaadd, int ADC_target, BYTE ADC_add, BYTE Gain_add, unsigned short *Gain);
extern int	DS1856_DDMI_GET_RightShift (int USB_Handle, unsigned char DS1856_Slaadd, BYTE rom_add, BYTE *RightShift); 
extern int	DS1856_DDMI_SET_RightShift (int USB_Handle, unsigned char DS1856_Slaadd, BYTE rom_add, BYTE RightShift); 

extern int  DS1856_Calibrate_Ibias (int USB_Handle, unsigned char DS1856_Slaadd, const float Ibias_amp, const float Ibias_resister, float *Ibias);
extern int  DS1856_Calibrate_TxPower (int USB_Handle, unsigned char DS1856_Slaadd, const float TxP_in, float *TxP_out);
extern int  DS1856_Calibrate_RxPower (int USB_Handle, unsigned char DS1856_Slaadd, const float RxP_in, float *RxP_out);
extern int  DS1856_Calibrate_RightShift(const unsigned short ADC_in, BYTE *number_right_shift);
extern int  DS1856_SET_IntEnable(int USB_Handle, unsigned char DS1856_Slaadd, BYTE Temp, BYTE Vcc, BYTE Mon1, BYTE Mon2, BYTE Mon3); 
extern BOOL DS1856_GET_LUT(int USB_Handle, unsigned char DS1856_SlaAdd, BYTE Table, BYTE LUT_arr[72]);

/////////////////////////////////////////
extern BOOL DS1856_GET_Temperatrue_SM(unsigned char DS1856_SlaAdd, double *Temperatrue);
extern BOOL DS1856_GET_Vcc_SM(unsigned char DS1856_SlaAdd, double *Vcc);
extern BOOL DS1856_SET_Table_SM(unsigned char DS1856_SlaAdd, BYTE Table);
extern int	DS1856_DDMI_GET_RightShift_SM ( unsigned char DS1856_Slaadd, BYTE rom_add, BYTE *RightShift);
extern int	DS1856_DDMI_SET_RightShift_SM ( unsigned char DS1856_Slaadd, BYTE rom_add, BYTE RightShift);
extern int  DS1856_SET_IntEnable_SM (unsigned char DS1856_Slaadd, BYTE Temp, BYTE Vcc, BYTE Mon1, BYTE Mon2, BYTE Mon3);
extern int  DS1856_Calibrate_RxPower_SM (unsigned char DS1856_Slaadd, const float RxP_in, float *RxP_out);
extern int  DS1856_Calibrate_TxPower_SM ( unsigned char DS1856_Slaadd, const float TxP_in, float *TxP_out);
extern int  DS1856_Calibrate_Ibias_SM ( unsigned char DS1856_Slaadd, const float Ibias_amp, const float Ibias_resister, float *Ibias);
extern int  DS1856_DDMI_GET_Gain_SM(unsigned char DS1856_Slaadd, int ADC_target, BYTE ADC_add, BYTE Gain_add, unsigned short *Gain);
extern int  DS1856_DDMI_SET_Offset_SM (unsigned char DS1856_Slaadd, BYTE rom_add, unsigned short Offset);
extern int  DS1856_DDMI_SET_Gain_SM (unsigned char DS1856_Slaadd, BYTE rom_add, unsigned short gain);
extern int	DS1856_DDMI_GET_ADC_SM (unsigned char DS1856_SlaAdd, BYTE rom_add, unsigned short *ADC);
extern BOOL DS1856_GET_Ibias_SM (unsigned char DS1856_SlaAdd, double *Ibias);
extern BOOL DS1856_Chang_Addr_SM (unsigned char DS1856_SlaAdd_OLD, unsigned char DS1856_SlaAdd_NEW);
extern BOOL DS1856_GET_Config_SM (unsigned char DS1856_SlaAdd, BYTE *ADEN, BYTE *ADFIX, BYTE *INV1, BYTE *INV2);
extern BOOL DS1856_GET_Mode_SM (unsigned char DS1856_SlaAdd, BYTE *TEN, BYTE *AEN);
extern BOOL DS1856_GET_Status_SM( unsigned char DS1856_SlaAdd, BYTE *Status, BYTE *RHIZ, BYTE *TxF, BYTE *RxL, BYTE *Rdyb);
extern BOOL DS1856_GET_RposFromRes_SM( unsigned char DS1856_SlaAdd, BYTE DS1856_TYPE, double Res0, double Res1, BYTE *H0, BYTE *H1);
extern BOOL DS1856_GET_ResFromRpos_SM(unsigned char DS1856_SlaAdd, BYTE DS1856_TYPE, BYTE H0, BYTE H1, double *Res0, double *Res1);
extern BOOL DS1856_SET_PassWord_SM(unsigned char DS1856_SlaAdd, BYTE PW0, BYTE PW1, BYTE PW2, BYTE PW3);
extern BOOL DS1856_ENTRY_PassWord_SM(unsigned char DS1856_SlaAdd, BYTE PW0, BYTE PW1, BYTE PW2, BYTE PW3);
extern BOOL DS1856_SET_RHIZ_SM( unsigned char DS1856_SlaAdd, BYTE RHIZ);
extern BOOL DS1856_SET_INV2_SM( unsigned char DS1856_SlaAdd, BYTE INV2);
extern BOOL DS1856_SET_INV1_SM( unsigned char DS1856_SlaAdd, BYTE INV1);
extern BOOL DS1856_SET_ADFIX_SM( unsigned char DS1856_SlaAdd, BYTE ADFIX);
extern BOOL DS1856_SET_ADEN_SM( unsigned char DS1856_SlaAdd, BYTE ADEN);
extern BOOL DS1856_SET_LUT_SM(unsigned char DS1856_SlaAdd, BYTE Table, BYTE *LUT_arr);
extern BOOL DS1856_SET_AEN_SM(unsigned char DS1856_SlaAdd, BYTE AEN);
extern BOOL DS1856_SET_TEN_SM(unsigned char DS1856_SlaAdd, BYTE TEN);
extern BOOL DS1856_GET_LUT_SM( unsigned char DS1856_SlaAdd, BYTE Table, BYTE LUT_arr[72]);

#endif  
