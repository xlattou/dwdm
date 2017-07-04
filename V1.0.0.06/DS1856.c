#include <cvirte.h>	
#include "CH341A_DLL.h"
#include "supermaster.h" 
#include "DS1856.h"
#include <windows.h>  
#include <ansi_c.h>
#include <userint.h>
#include "global.h"
#include <utility.h>

/***********for I2C***********************/
static float 	T_wait=0.015;//delay time 
static BYTE 	rom_addr, rom_value_arr[256], rom_value; 
static int 	error; 
char strInpt[256], strOupt[256]; 
/***********end for I2C***********************/ 

BOOL DS1856_GET_Temperatrue(int USB_Handle, unsigned char DS1856_SlaAdd, double *Temperatrue)
{
unsigned short 	temp;
BYTE	MSB, LSB;

	error = I2C_2BYTEs_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x60, &MSB, &LSB);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	temp = (MSB <<8) | LSB;
	if ((temp & 0x8000) == 0) 
	{*Temperatrue = temp/256.0;}
	else 
	{*Temperatrue = temp/256.0 - 256.0;}

return TRUE; 
}

BOOL DS1856_GET_Vcc(int USB_Handle, unsigned char DS1856_SlaAdd, double *Vcc)
{
unsigned short 	temp;
BYTE	MSB, LSB;

	error = I2C_2BYTEs_READ_DLL  (USB_Handle, DS1856_SlaAdd, 0x62, &MSB, &LSB);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	temp = (MSB <<8) | LSB;
	*Vcc = temp/10000.0;

return TRUE; 
}

BOOL DS1856_GET_TxPower(int USB_Handle, unsigned char DS1856_SlaAdd, double *TxPower)
{
unsigned short 	temp;
BYTE	MSB, LSB;

	error = I2C_2BYTEs_READ_DLL  (USB_Handle, DS1856_SlaAdd, 0x66, &MSB, &LSB);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	temp = (MSB <<8) | LSB;
	*TxPower = 10 * log10(temp * 0.1e-3);

return TRUE; 
}

BOOL DS1856_SET_TEN(int USB_Handle, unsigned char DS1856_SlaAdd, BYTE TEN)
{
unsigned char temp_r, temp_w; 

	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, 0x03); 			//set table3
	
	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x80, &temp_r);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}
	
	temp_w = (0xFD & temp_r) | (TEN<<1);  				//TEN=1:LUT;TEN=0:Manul
	
	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x80, temp_w, T_wait); 
	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}

	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x80, &temp_r);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}

	if(temp_r != temp_w) return FALSE; 
	
	return TRUE;	
}

BOOL DS1856_SET_AEN(int USB_Handle, unsigned char DS1856_SlaAdd, BYTE AEN)
{
unsigned char temp_r, temp_w; 

	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, 0x03); 			//set table3
	
	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x80, &temp_r);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}
	
	temp_w = (0xFE & temp_r) | (AEN<<0);  				//TEN=1:LUT;TEN=0:Manul
	
	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x80, temp_w, T_wait); 
	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}

	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x80, &temp_r);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}

	if(temp_r != temp_w) return FALSE; 
	
	return TRUE;	
}

BOOL DS1856_SET_Table(int USB_Handle, unsigned char DS1856_SlaAdd, BYTE Table)
{

	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x7F, Table, T_wait); 
	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	
	return TRUE;
}

BOOL DS1856_SET_LUT(int USB_Handle, unsigned char DS1856_SlaAdd, BYTE Table, BYTE *LUT_arr) 
{
int 	i;
BYTE 	rom_StartAddress, rom_value_arr_w[256], rom_value_arr_r[256];

	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, Table);
	
	memset (rom_value_arr_w, 255, 256);
	memset (rom_value_arr_r, 255, 256); 
	for (i=0; i<72; i++) {rom_value_arr_w[i+0x80]=LUT_arr[i];}
	
	for (i=0; i<9; i++)
	{
		rom_StartAddress=i*8+0x80;
		error = I2C_BYTEs_WRITE_DLL(USB_Handle, DS1856_SlaAdd, rom_StartAddress, 8, rom_value_arr_w, T_wait);
		if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	}
	
	error = I2C_BYTEs_READ_DLL(USB_Handle, DS1856_SlaAdd, 0x80, 72, rom_value_arr_r);
	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	
	for (i=0; i<256; i++) 
	{
		if (!(rom_value_arr_w[i]==rom_value_arr_r[i])) return FALSE;
	}

	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, 0x03);
	
	return TRUE;
}

BOOL DS1856_SET_ADEN(int USB_Handle, unsigned char DS1856_SlaAdd, BYTE ADEN)
{
unsigned char temp_r, temp_w; 

	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, 0x03); 			//set table3
	
	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x89, &temp_r);  //read 0x89 vlaue      
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	temp_w = (0xDF & temp_r) | (ADEN<<5);  	//ADEN=1 the memory is located in or as table 01h
													//ADEN=0 the memory is addressed by using a Device address of A0h and 
													//  the locations in memory are 00h to 7Fh.
	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x89, temp_w, T_wait); 
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x89, &temp_r);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}

	if(temp_r != temp_w) return FALSE;
	
	return TRUE;
}

BOOL DS1856_SET_ADFIX(int USB_Handle, unsigned char DS1856_SlaAdd, BYTE ADFIX)
{
unsigned char temp_r, temp_w; 

	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, 0x03); 			//set table3

	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x89, &temp_r);  //read 0x89 vlaue      
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	temp_w = (0xEF & temp_r) | (ADFIX<<4);  	//ADFIX=1 the main memory of the DS1856 is a Device Address equal to the value found in byte chip_address .
													//ADFIX=0 the main memory of the DS1856 is a Device Address of A2h
	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x89, temp_w, T_wait); 
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 

	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x89, &temp_r);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}

	if(temp_r != temp_w) return FALSE;
	
	return TRUE; 
}

BOOL DS1856_SET_INV1(int USB_Handle, unsigned char DS1856_SlaAdd, BYTE INV1)
{
unsigned char temp_r, temp_w; 

	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, 0x03); 			//set table3

	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x89, &temp_r);  //read 0x89 vlaue      
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	temp_w = (0xFD & temp_r) | (INV1<<1);  	
													
	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x89, temp_w, T_wait); 
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x89, &temp_r);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}

	if(temp_r != temp_w) return FALSE;
	
	return TRUE; 
}

BOOL DS1856_SET_INV2(int USB_Handle, unsigned char DS1856_SlaAdd, BYTE INV2)
{
unsigned char temp_r, temp_w; 

	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, 0x03); 			//set table3

	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x89, &temp_r);  //read 0x89 vlaue      
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	temp_w = (0xFE & temp_r) | (INV2<<0);  	
													
	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x89, temp_w, T_wait); 
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 

	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x89, &temp_r);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}

	if(temp_r != temp_w) return FALSE;
	
	return TRUE; 
}

BOOL DS1856_SET_RHIZ(int USB_Handle, unsigned char DS1856_SlaAdd, BYTE RHIZ)
{
unsigned char temp_r, temp_w; 

	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, 0x03); 			//set table3
	
	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x6E, &temp_r);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}
	
	temp_w = (0xBF & temp_r) | (RHIZ<<6);  				//TEN=1:LUT;TEN=0:Manul
	
	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x6E, temp_w, T_wait); 
	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}

	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x6E, &temp_r);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}

	temp_r = (0x40 & temp_r) >>6; 
	if(temp_r != RHIZ) return FALSE; 
	
	return TRUE;	
}

BOOL DS1856_ENTRY_PassWord(int USB_Handle, unsigned char DS1856_SlaAdd, BYTE PW0, BYTE PW1, BYTE PW2, BYTE PW3)
{
unsigned char temp_r, temp_w; 
unsigned char temp_arr[256];

	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, 0x03);

	error = I2C_4BYTEs_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x7B, PW0, PW1, PW2, PW3, T_wait);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, 0x03);
	
	error = I2C_BYTEs_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x80, 0x80, temp_arr);     
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	if( (temp_arr[128] == 0xFF) && (temp_arr[129] == 0xFF)) return FALSE;
	
	return TRUE;					  
}

BOOL DS1856_SET_PassWord(int USB_Handle, unsigned char DS1856_SlaAdd, BYTE PW0, BYTE PW1, BYTE PW2, BYTE PW3)
{
unsigned char temp_r, temp_w;

	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, 0x03);
	
	error = I2C_4BYTEs_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0xB0, 0xFF, 0xFF, 0xFF, 0xFF, T_wait);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	error = I2C_4BYTEs_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0xB4, PW0, PW1, PW2, PW3, T_wait);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	error = I2C_4BYTEs_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x7B, PW0, PW1, PW2, PW3, T_wait);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, 0x05);

	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x80, &temp_r);     
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 

	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x80, temp_r+1, T_wait); 
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x80, &temp_w);     
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	if(temp_r == temp_w) return FALSE;
	
	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x80, temp_w-1, T_wait); 
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, 0x03);
	
	return TRUE;
}

BOOL DS1856_GET_ResFromRpos(int USB_Handle, unsigned char DS1856_SlaAdd, BYTE DS1856_TYPE, BYTE H0, BYTE H1, double *Res0, double *Res1)
{
double 	c, u, v, w, x, y, z, alfa;
BYTE 	rom_StartAddress, rom_value_arr[256];

	error = DS1856_GET_Temperatrue(USB_Handle, DS1856_SlaAdd, &c);
	if (!error) return FALSE;
	
	//get resistor for H0
	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x7F, 4, T_wait); 
	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	
	rom_StartAddress=0xF8;
	error = I2C_BYTEs_READ_DLL(USB_Handle, DS1856_SlaAdd, rom_StartAddress, 8, rom_value_arr);
	u = rom_value_arr[rom_StartAddress];
	v = rom_value_arr[rom_StartAddress+1]*20E-6;
	w = rom_value_arr[rom_StartAddress+2]*100E-9;
	x = rom_value_arr[rom_StartAddress+3]*2+rom_value_arr[rom_StartAddress+4]*pow(2,-7);
	y = rom_value_arr[rom_StartAddress+5]*2E-6;  //DS1856_TYPE 保留
	z = rom_value_arr[rom_StartAddress+6]*10E-9;
	alfa = rom_value_arr[rom_StartAddress+7]*pow(2,-2);
	*Res0 = ((H0+alfa)*x*(1+y*(c-25)+z*(c-25)*(c-25))+u*(1+v*(c-25)+w*(c-25)*(c-25)))/1000;
	
	//get resistor for H1
	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x7F, 5, T_wait); 
	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	
	rom_StartAddress=0xF8;
	error = I2C_BYTEs_READ_DLL(USB_Handle, DS1856_SlaAdd, rom_StartAddress, 8, rom_value_arr);
	u = rom_value_arr[rom_StartAddress];
	v = rom_value_arr[rom_StartAddress+1]*20E-6;
	w = rom_value_arr[rom_StartAddress+2]*100E-9;
	x = rom_value_arr[rom_StartAddress+3]*2+rom_value_arr[rom_StartAddress+4]*pow(2,-7);
	y = rom_value_arr[rom_StartAddress+5]*2E-6;  //DS1856_TYPE 保留 
	z = rom_value_arr[rom_StartAddress+6]*10E-9;
	alfa = rom_value_arr[rom_StartAddress+7]*pow(2,-2);
	*Res1 = ((H1+alfa)*x*(1+y*(c-25)+z*(c-25)*(c-25))+u*(1+v*(c-25)+w*(c-25)*(c-25)))/1000;
	
	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x7F, 3, T_wait); 
	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	
	return TRUE;
}

BOOL DS1856_GET_RposFromRes(int USB_Handle, unsigned char DS1856_SlaAdd, BYTE DS1856_TYPE, double Res0, double Res1, BYTE *H0, BYTE *H1)
{
double 	c, u, v, w, x, y, z, alfa;
BYTE 	rom_StartAddress, rom_value_arr[256];

	error = DS1856_GET_Temperatrue(USB_Handle, DS1856_SlaAdd, &c);
	if (!error) return FALSE;
	
	//get H0 for resistor
	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x7F, 4, T_wait); 
	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	
	rom_StartAddress=0xF8;
	error = I2C_BYTEs_READ_DLL(USB_Handle, DS1856_SlaAdd, rom_StartAddress, 8, rom_value_arr);
	u = rom_value_arr[rom_StartAddress];
	v = rom_value_arr[rom_StartAddress+1]*20E-6;
	w = rom_value_arr[rom_StartAddress+2]*100E-9;
	x = rom_value_arr[rom_StartAddress+3]*2+rom_value_arr[rom_StartAddress+4]*pow(2,-7);
	y = rom_value_arr[rom_StartAddress+5]*2E-6;  //DS1856_TYPE 保留
	z = rom_value_arr[rom_StartAddress+6]*10E-9;
	alfa = rom_value_arr[rom_StartAddress+7]*pow(2,-2);
	*H0=(BYTE)((Res0*1000.0-u*(1+v*(c-25.0)+w*(c-25.0)*(c-25.0)))/(x*(1+y*(c-25.0)+z*(c-25.0)*(c-25.0)))-alfa);
	
	//get H1 for resistor
	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x7F, 5, T_wait); 
	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	
	rom_StartAddress=0xF8;
	error = I2C_BYTEs_READ_DLL(USB_Handle, DS1856_SlaAdd, rom_StartAddress, 8, rom_value_arr);
	u = rom_value_arr[rom_StartAddress];
	v = rom_value_arr[rom_StartAddress+1]*20E-6;
	w = rom_value_arr[rom_StartAddress+2]*100E-9;
	x = rom_value_arr[rom_StartAddress+3]*2+rom_value_arr[rom_StartAddress+4]*pow(2,-7);
	y = rom_value_arr[rom_StartAddress+5]*2E-6;  //DS1856_TYPE 保留 
	z = rom_value_arr[rom_StartAddress+6]*10E-9;
	alfa = rom_value_arr[rom_StartAddress+7]*pow(2,-2);
	*H1=(BYTE)((Res1*1000.0-u*(1+v*(c-25.0)+w*(c-25.0)*(c-25.0)))/(x*(1+y*(c-25.0)+z*(c-25.0)*(c-25.0)))-alfa); 
	
	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x7F, 3, T_wait); 
	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}

	return TRUE;
}

BOOL DS1856_GET_Status(int USB_Handle, unsigned char DS1856_SlaAdd, BYTE *Status, BYTE *RHIZ, BYTE *TxF, BYTE *RxL, BYTE *Rdyb)
{
BYTE temp;

	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x6E, &temp);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	*Status=temp;
	*RHIZ=(temp & 0x80) >> 7;
	*TxF =(temp & 0x4) >> 2; 
	*RxL =(temp & 0x2) >> 1; 
	*Rdyb=(temp & 0x1) >> 0; 
	
	return TRUE;
}

BOOL DS1856_GET_Mode(int USB_Handle, unsigned char DS1856_SlaAdd, BYTE *TEN, BYTE *AEN)
{
BYTE temp;

	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, 0x03);
	
	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x80, &temp);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}
	
	*TEN = (temp & 0x2) >> 1;
	*AEN = (temp & 0x1) >> 0; 
	
	return TRUE;
}

BOOL DS1856_GET_Config(int USB_Handle, unsigned char DS1856_SlaAdd, BYTE *ADEN, BYTE *ADFIX, BYTE *INV1, BYTE *INV2)
{
BYTE temp;

	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, 0x03);
	
	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x89, &temp);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}
	
	*ADEN = (temp & 0x20) >> 5;
	*ADFIX= (temp & 0x10) >> 4; 
	*INV1 = (temp & 0x02) >> 1;
	*INV2 = (temp & 0x01) >> 0; 
	
	return TRUE;
}

BOOL DS1856_Chang_Addr(int USB_Handle, unsigned char DS1856_SlaAdd_OLD, unsigned char DS1856_SlaAdd_NEW)
{
BYTE temp;

	if (!DS1856_SET_Table(USB_Handle, DS1856_SlaAdd_OLD, 3)) return FALSE;
	
	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd_OLD, 0x8C, DS1856_SlaAdd_NEW, T_wait);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}
	
	if (!DS1856_SET_ADEN(USB_Handle, DS1856_SlaAdd_OLD, 1)) return FALSE; 
	
	if (!DS1856_SET_ADFIX(USB_Handle, DS1856_SlaAdd_OLD, 1)) return FALSE;
	
	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd_NEW, 0x8C, &temp);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}
	
	if (DS1856_SlaAdd_NEW !=temp) return FALSE;
	
	return TRUE; 
}

BOOL DS1856_GET_Ibias(int USB_I2CHandle, unsigned char DS1856_SlaAdd, double *Ibias)
{
unsigned short 	temp;
BYTE	MSB, LSB;

	error = I2C_2BYTEs_READ_DLL  (USB_I2CHandle, DS1856_SlaAdd, 0x64, &MSB, &LSB);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	temp = (MSB <<8) | LSB;
	*Ibias = temp/5000.0;

return TRUE; 
}


int	DS1856_DDMI_GET_ADC (int USB_Handle, unsigned char DS1856_Slaadd, BYTE rom_add, unsigned short *ADC)
{
	int 	error, i,sum=0;
	BYTE	MSB, LSB;
	
	for (i=0; i<10; i++)
	{
		error = I2C_2BYTEs_READ_DLL  (USB_Handle, DS1856_Slaadd, rom_add, &MSB, &LSB);
		if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;} 
		sum += MSB*256+LSB;
	}
	*ADC = sum/10;
	return 0;
}

int DS1856_DDMI_SET_Gain (int USB_Handle, unsigned char DS1856_Slaadd, BYTE rom_add, unsigned short gain)
{
	int  error;
	BYTE MSB, LSB;
	
	MSB = gain >> 8;  
	LSB = gain & 0xFF;  	
	error = I2C_2BYTEs_WRITE_DLL (USB_Handle, DS1856_Slaadd, rom_add, MSB, LSB, 0.015);  
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;} 
	
	return 0;
}

int DS1856_DDMI_SET_Offset (int USB_Handle, unsigned char DS1856_Slaadd, BYTE rom_add, unsigned short Offset)
{
	int  error;
	BYTE MSB, LSB;
	
	MSB = Offset >> 8;  
	LSB = Offset & 0xFF;  	
	error = I2C_2BYTEs_WRITE_DLL (USB_Handle, DS1856_Slaadd, rom_add, MSB, LSB, 0.015);  
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;} 
	
	return 0;
}

int DS1856_DDMI_GET_Gain(int USB_Handle, unsigned char DS1856_Slaadd, int ADC_target, BYTE ADC_add, BYTE Gain_add, unsigned short *Gain)
{
	unsigned short mygain;
	unsigned char  msb, lsb;
	int i, myadc;
	
	mygain=0x8000;
	for (i=15; i>=0; i--)
	{
		msb = mygain >> 8;  
		lsb = mygain & 0xFF;  	
	
		error = I2C_2BYTEs_WRITE_DLL (USB_Handle, DS1856_Slaadd, Gain_add, msb, lsb, 0.015);  
		if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;} 
		
		Delay (0.05);
		
		error = I2C_2BYTEs_READ_DLL  (USB_Handle, DS1856_Slaadd, ADC_add, &msb, &lsb);
		if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;} 
		
		myadc = msb*256+lsb;
		
		if (i>0)
		{
			if (myadc>ADC_target)
			{
				mygain=mygain-pow(2, i-1);	
			}
			else
			{
				mygain=mygain+pow(2, i-1);
			}
		}
		else
		{
			if (myadc>ADC_target)
			{
				mygain=mygain-1;	
			}
			else
			{
				mygain=mygain+1;
			}
		}
	}
	*Gain = mygain;
	
    return 0;
}

int  DS1856_Calibrate_Ibias (int USB_Handle, unsigned char DS1856_Slaadd, const float Ibias_amp, const float Ibias_resister, float *Ibias)
{
	unsigned short ADC, ADC_target, gain;
	unsigned char  right_shift_num, right_shift_w, right_shift_r, right_shift; 
	float    Ibias0, Ibias1;
		
	if (!DS1856_SET_Table(USB_Handle, DS1856_Slaadd, 3)) return -1; 
	
	//set gain=0xBE9 offset=0 对于大多数DS1856来说gain=0xBE9对应了ADC=FFF8h => 2.5V，实际的出厂设置可能有偏差，可忽略 
	error = DS1856_DDMI_SET_Gain (USB_Handle, DS1856_Slaadd, 0x94, 0xBE9);
	if (error<0) return -1;
	
	error = DS1856_DDMI_SET_Offset (USB_Handle, DS1856_Slaadd, 0xA4, 0);
	if (error<0) return -1;
	
	//set right shift regediter 8Eh (bit6-bit4 => 0)  
	error = DS1856_DDMI_GET_RightShift (USB_Handle, DS1856_Slaadd, 0x8E, &right_shift_r);
	if (error<0) return -1;
	right_shift_w = right_shift_r & 0x8F;
	error = DS1856_DDMI_SET_RightShift (USB_Handle, DS1856_Slaadd, 0x8E, right_shift_w);
	if (error<0) return -1;
	
	Delay (0.05);
	
	error = DS1856_DDMI_GET_ADC (USB_Handle, DS1856_Slaadd, 0x64, &ADC);  
	if (error<0) return -1; 
	
	Ibias0 = ((ADC*2.5/65535.)*Ibias_amp/Ibias_resister)*1000.; 
	if(Ibias0<5.0 || Ibias0>45.0)
	{
		MessagePopup ("Error", "Ibias out of range 0.0~45.0mA");  
		return -1;	
	}
	
	ADC_target = Ibias0/0.002;
	
	//get adc with 0 right shifts
	error = DS1856_Calibrate_RightShift (ADC_target, &right_shift_num);
	if (error) return -1; 
	ADC_target = ADC_target << right_shift_num;
	
	error = DS1856_DDMI_GET_Gain (USB_Handle, DS1856_Slaadd, ADC_target, 0x64, 0x94, &gain);
	if (error<0) return -1; 
	
	error = DS1856_DDMI_SET_Gain (USB_Handle, DS1856_Slaadd, 0x94, gain);
	if (error<0) return -1;
	
	//set right shifts
	right_shift =  right_shift_num << 4;
	right_shift_w = right_shift_w | right_shift;
	error = DS1856_DDMI_SET_RightShift (USB_Handle, DS1856_Slaadd, 0x8E, right_shift_w);
	if (error<0) return -1;
	
	Delay (0.05);
	
	error = DS1856_DDMI_GET_ADC (USB_Handle, DS1856_Slaadd, 0x64, &ADC);  
	if (error<0) return -1; 
			  
	Ibias1 =  ADC*0.002;
	
	if (fabs(Ibias0- Ibias1)>4)
	{
		MessagePopup ("Error", "Ibias calibration fail");  
		return -1;	
	}
	
	*Ibias = Ibias1;
	
	return 0;
}

int  DS1856_Calibrate_TxPower (int USB_Handle, unsigned char DS1856_Slaadd, const float TxP_in, float *TxP_out)
{
	unsigned short ADC, ADC_target, gain; 
	unsigned char  right_shift_num, right_shift_w, right_shift_r, right_shift; 
	float  tx_power;
	
	if (!DS1856_SET_Table(USB_Handle, DS1856_Slaadd, 3)) return -1; 
	
	//set right shift regediter 8Eh (bit2-bit0 => 0)  
	error = DS1856_DDMI_GET_RightShift (USB_Handle, DS1856_Slaadd, 0x8E, &right_shift_r);
	if (error<0) return -1;
	right_shift_w = right_shift_r & 0xF8;
	error = DS1856_DDMI_SET_RightShift (USB_Handle, DS1856_Slaadd, 0x8E, right_shift_w);
	if (error<0) return -1;
	Delay (0.05);
	
	//get adc with 0 right shifts
	ADC_target= pow(10, TxP_in/10)/0.0001; 
//	error = DS1856_Calibrate_RightShift (ADC_target, &right_shift_num);
//	if (error) return -1; 
//	ADC_target = ADC_target << right_shift_num;
	
	error = DS1856_DDMI_GET_Gain (USB_Handle, DS1856_Slaadd, ADC_target, 0x66, 0x96, &gain);
	if (error<0) return -1; 
	
	error = DS1856_DDMI_SET_Gain (USB_Handle, DS1856_Slaadd, 0x96, gain);
	if (error<0) return -1;
	
	//set right shifts
//	right_shift =  right_shift_num;
//	right_shift_w = right_shift_w | right_shift;
//	error = DS1856_DDMI_SET_RightShift (USB_Handle, DS1856_Slaadd, 0x8E, right_shift_w);
//	if (error<0) return -1;
	
	Delay (0.05);
	
	error = DS1856_DDMI_GET_ADC (USB_Handle, DS1856_Slaadd, 0x66, &ADC);  
	if (error<0) return -1; 
    tx_power= 10*log10(ADC*0.0001);
    
	if (fabs(tx_power - TxP_in)>0.1)
    {
		MessagePopup ("Error", "Tx_Power calibration fail");  
		return -1;
    }

	*TxP_out =  tx_power;
	
	return 0;	
}

int  DS1856_Calibrate_RightShift(const unsigned short ADC_in, BYTE *number_right_shift)
{
	if (ADC_in>0 && ADC_in<0x0FFF)
	{
		*number_right_shift=4;
	}
	else if (ADC_in<0x1FFF) 
	{
		*number_right_shift=3;
	}
	else if (ADC_in<0x3FFF) 
	{
		*number_right_shift=2;
	}
	else if (ADC_in<0x7FFF) 
	{
		*number_right_shift=1;
	}
	else if (ADC_in<0xFFFF) 
	{
		*number_right_shift=0;
	}
	else
	{
		MessagePopup ("Error", "get right shift fail"); 
		return -1;
	}
	return 0;
}

int	DS1856_DDMI_GET_RightShift (int USB_Handle, unsigned char DS1856_Slaadd, BYTE rom_add, BYTE *RightShift)
{
	int error;
	
	error = I2C_BYTE_READ_DLL  (USB_Handle, DS1856_Slaadd, rom_add, RightShift);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;} 

	return 0;
}

int	DS1856_DDMI_SET_RightShift (int USB_Handle, unsigned char DS1856_Slaadd, BYTE rom_add, BYTE RightShift)
{
	int  error;
	
	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_Slaadd, rom_add, RightShift, 0.015);  
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;} 
	
	return 0;
}

int  DS1856_Calibrate_RxPower (int USB_Handle, unsigned char DS1856_Slaadd, const float RxP_in, float *RxP_out)
{
	unsigned short ADC, ADC_target, gain; 
	unsigned char  right_shift_num, right_shift_w, right_shift_r, right_shift;
	float  rx_power;
	
	//get adc with 0 right shifts
	ADC_target= pow(10, RxP_in/10)/0.0001; 
	error = DS1856_Calibrate_RightShift (ADC_target, &right_shift_num);
	if (error) return -1; 
	ADC_target = ADC_target << right_shift_num;
	
	if (!DS1856_SET_Table(USB_Handle, DS1856_Slaadd, 3)) return -1; 
	
	//set right shift regediter 8Fh (bit6-bit4 => 0)  
	error = DS1856_DDMI_GET_RightShift (USB_Handle, DS1856_Slaadd, 0x8F, &right_shift_r);
	if (error<0) return -1;
	right_shift_w = right_shift_r & 0x8F;
	error = DS1856_DDMI_SET_RightShift (USB_Handle, DS1856_Slaadd, 0x8F, right_shift_w);
	if (error<0) return -1;
	
	error = DS1856_DDMI_GET_Gain (USB_Handle, DS1856_Slaadd, ADC_target, 0x68, 0x98, &gain);
	if (error<0) return -1; 
	
	error = DS1856_DDMI_SET_Gain (USB_Handle, DS1856_Slaadd, 0x98, gain);
	if (error<0) return -1;
	
	//set right shifts
	right_shift =  right_shift_num << 4;
	right_shift_w = right_shift_w | right_shift;
	error = DS1856_DDMI_SET_RightShift (USB_Handle, DS1856_Slaadd, 0x8F, right_shift_w);
	if (error<0) return -1;
	
	Delay (0.05);
	
	//检查当前的收端光功率是否读取正确
	error = DS1856_DDMI_GET_ADC (USB_Handle, DS1856_Slaadd, 0x68, &ADC);  
	if (error<0) return -1; 
    rx_power= 10*log10(ADC*0.0001);
    
	if (fabs(rx_power - RxP_in)>1)
    {
		MessagePopup ("Error", "Rx_Power calibration fail");  
		return -1;
    }

	*RxP_out =  rx_power;
	
	return 0;	
}

int DS1856_SET_IntEnable(int USB_Handle, unsigned char DS1856_Slaadd, BYTE Temp, BYTE Vcc, BYTE Mon1, BYTE Mon2, BYTE Mon3)
{
	int  error;
	BYTE temp_w, temp_r;
	
	DS1856_SET_Table(USB_Handle, DS1856_Slaadd, 0x03); 
	
	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_Slaadd, 0x88, &temp_r);  //read 0x88 vlaue      
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;} 
	
	temp_w = (0x7F & temp_r) | (Temp<<7);  
	temp_w = (0xBF & temp_w) | (Vcc <<6);
	temp_w = (0xDF & temp_w) | (Mon1<<5); 
	temp_w = (0xEF & temp_w) | (Mon2<<4);
	temp_w = (0xF7 & temp_w) | (Mon2<<3); 
													
	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_Slaadd, 0x88, temp_w, 0.015); 
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;} 
	
	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_Slaadd, 0x88, &temp_r);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;}

	if(temp_r != temp_w) return -1;
	
	return 0;
}

BOOL DS1856_GET_LUT(int USB_Handle, unsigned char DS1856_SlaAdd, BYTE Table, BYTE LUT_arr[72]) 
{
int 	i;
BYTE 	rom_value_arr_r[256];

	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, Table);
	
	memset (rom_value_arr_r, 255, 256); 
	
	error = I2C_BYTEs_READ_DLL(USB_Handle, DS1856_SlaAdd, 0x80, 72, rom_value_arr_r);
	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	
	for (i=0; i<72; i++)
	{
		LUT_arr[i]=rom_value_arr_r[i+128];
	}	 
	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, 0x03);
	
	return TRUE;
}

////////////////supermaster command ///////////////////////////////////////////////////////////////////////////

BOOL DS1856_GET_Temperatrue_SM(unsigned char DS1856_SlaAdd, double *Temperatrue)
{
unsigned short 	temp;
BYTE	MSB, LSB;
int value_M, value_L;
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	sprintf(strInpt, "MCU_I2C_READ(0x%X,96,2)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x,0x%x", &value_M, &value_L); 
	
	MSB = value_M;
	LSB = value_L;

//	error = I2C_2BYTEs_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x60, &MSB, &LSB);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	temp = (MSB <<8) | LSB;
	if ((temp & 0x8000) == 0) 
	{*Temperatrue = temp/256.0;}
	else 
	{*Temperatrue = temp/256.0 - 256.0;}

return TRUE; 
}

BOOL DS1856_GET_Vcc_SM(unsigned char DS1856_SlaAdd, double *Vcc)
{
unsigned short 	temp;
BYTE	MSB, LSB;
int value_M, value_L;
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	sprintf(strInpt, "MCU_I2C_READ(0x%X,98,2)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x,0x%x", &value_M, &value_L); 
	
	MSB = value_M;
	LSB = value_L;

//	error = I2C_2BYTEs_READ_DLL  (USB_Handle, DS1856_SlaAdd, 0x62, &MSB, &LSB);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	temp = (MSB <<8) | LSB;
	*Vcc = temp/10000.0;

return TRUE; 
}

BOOL DS1856_SET_TEN_SM(unsigned char DS1856_SlaAdd, BYTE TEN)
{
unsigned char temp_r, temp_w; 
int value;

	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x03); 			//set table3
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	
	sprintf(strInpt, "MCU_I2C_READ(0x%X,128,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp_r = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x80, &temp_r);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}
	
	temp_w = (0xFD & temp_r) | (TEN<<1);  				//TEN=1:LUT;TEN=0:Manul
	
	value = temp_w;
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,128,0x%X)", DS1856_SlaAdd, value);  
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "设置TEN失败!     "); 
		return FALSE;
	} 
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x80, temp_w, T_wait); 
//	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	sprintf(strInpt, "MCU_I2C_READ(0x%X,128,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp_r = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x80, &temp_r);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}

	if(temp_r != temp_w) return FALSE; 
	
	return TRUE;	
}

BOOL DS1856_SET_AEN_SM(unsigned char DS1856_SlaAdd, BYTE AEN)
{
unsigned char temp_r, temp_w; 
int value;

	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x03); 			//set table3

	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	
	sprintf(strInpt, "MCU_I2C_READ(0x%X,128,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp_r = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x80, &temp_r);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}
	
	temp_w = (0xFE & temp_r) | (AEN<<0);  				//TEN=1:LUT;TEN=0:Manul

	value = temp_w;
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,128,0x%X)", DS1856_SlaAdd, value);  
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "设置AEN失败!     "); 
		return FALSE;
	} 
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x80, temp_w, T_wait); 
//	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}

	sprintf(strInpt, "MCU_I2C_READ(0x%X,128,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp_r = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x80, &temp_r);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}

	if(temp_r != temp_w) return FALSE; 
	
	return TRUE;	
}

BOOL DS1856_SET_Table_SM(unsigned char DS1856_SlaAdd, BYTE Table)
{
	int value;
	
	value = Table;
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,127,0x%X)", DS1856_SlaAdd, value);  
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "选表失败!     "); 
		return FALSE;
	} 
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x7F, Table, T_wait); 
//	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	
	return TRUE;
}

BOOL DS1856_SET_LUT_SM(unsigned char DS1856_SlaAdd, BYTE Table, BYTE *LUT_arr) 
{
int 	i;
BYTE 	rom_StartAddress, rom_value_arr_w[256], rom_value_arr_r[256];
int 	value;

	DS1856_SET_Table_SM(DS1856_SlaAdd, Table);
	
	memset (rom_value_arr_w, 255, 256);
	memset (rom_value_arr_r, 255, 256); 
	for (i=0; i<72; i++) {rom_value_arr_w[i+0x80]=LUT_arr[i];}
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	
	for (i=0; i<9; i++)
	{
		rom_StartAddress=i*8+0x80;
	//	error = I2C_BYTEs_WRITE_DLL(USB_Handle, DS1856_SlaAdd, rom_StartAddress, 8, rom_value_arr_w, T_wait);
	//	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
		sprintf(strInpt, "MCU_I2C_WRITE(0x%X,%d,0x%X,0x%X,0x%X,0x%X,0x%X,0x%X,0x%X,0x%X)", DS1856_SlaAdd, rom_StartAddress, 
				rom_value_arr_w[rom_StartAddress+0], rom_value_arr_w[rom_StartAddress+1], rom_value_arr_w[rom_StartAddress+2], rom_value_arr_w[rom_StartAddress+3], 
				rom_value_arr_w[rom_StartAddress+4], rom_value_arr_w[rom_StartAddress+5], rom_value_arr_w[rom_StartAddress+6], rom_value_arr_w[rom_StartAddress+7]);  
		SetCommand(strInpt, strOupt);    
		if(0 != strcmp(strOupt, "OK"))
		{
			MessagePopup ("ERROR", "写表失败!     "); 
			return FALSE;
		} 
	}
	
//	error = I2C_BYTEs_READ_DLL(USB_Handle, DS1856_SlaAdd, 0x80, 72, rom_value_arr_r);
//	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	for (i=0; i<9; i++) 
	{
		rom_StartAddress=i*8+0x80; 	
	
		sprintf(strInpt, "MCU_I2C_READ(0x%X,%d,8)", DS1856_SlaAdd, rom_StartAddress);  
		SetCommand(strInpt, strOupt);    
		sscanf(strOupt,"0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x", &rom_value_arr_r[rom_StartAddress+0], &rom_value_arr_r[rom_StartAddress+1], &rom_value_arr_r[rom_StartAddress+2],
			   &rom_value_arr_r[rom_StartAddress+3], &rom_value_arr_r[rom_StartAddress+4], &rom_value_arr_r[rom_StartAddress+5], &rom_value_arr_r[rom_StartAddress+6], &rom_value_arr_r[rom_StartAddress+7]); 
	}
	
	for (i=0; i<256; i++) 
	{
		if (!(rom_value_arr_w[i]==rom_value_arr_r[i])) return FALSE;
	}

	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x03);
	
	return TRUE;
}

BOOL DS1856_SET_ADEN_SM( unsigned char DS1856_SlaAdd, BYTE ADEN)
{
unsigned char temp_r, temp_w; 
int value;

	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x03); 			//set table3

	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256);  
	
	sprintf(strInpt, "MCU_I2C_READ(0x%X,137,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp_r = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x89, &temp_r);  //read 0x89 vlaue      
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	temp_w = (0xDF & temp_r) | (ADEN<<5);  	//ADEN=1 the memory is located in or as table 01h
													//ADEN=0 the memory is addressed by using a Device address of A0h and 
													//  the locations in memory are 00h to 7Fh.
	value = temp_w;
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,137,0x%X)", DS1856_SlaAdd, value);  
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "设置ADEN失败!     "); 
		return FALSE;
	} 
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x89, temp_w, T_wait); 
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 

	sprintf(strInpt, "MCU_I2C_READ(0x%X,137,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp_r = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x89, &temp_r);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}

	if(temp_r != temp_w) return FALSE;
	
	return TRUE;
}

BOOL DS1856_SET_ADFIX_SM( unsigned char DS1856_SlaAdd, BYTE ADFIX)
{
unsigned char temp_r, temp_w; 
int value;

	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x03); 			//set table3

	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256);  
	
	sprintf(strInpt, "MCU_I2C_READ(0x%X,137,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp_r = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x89, &temp_r);  //read 0x89 vlaue      
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	temp_w = (0xEF & temp_r) | (ADFIX<<4);  	//ADFIX=1 the main memory of the DS1856 is a Device Address equal to the value found in byte chip_address .
													//ADFIX=0 the main memory of the DS1856 is a Device Address of A2h
	value = temp_w;
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,137,0x%X)", DS1856_SlaAdd, value);  
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "设置ADFIX失败!     "); 
		return FALSE;
	} 
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x89, temp_w, T_wait); 
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 

	sprintf(strInpt, "MCU_I2C_READ(0x%X,137,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp_r = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x89, &temp_r);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}

	if(temp_r != temp_w) return FALSE;
	
	return TRUE; 
}

BOOL DS1856_SET_INV1_SM( unsigned char DS1856_SlaAdd, BYTE INV1)
{
unsigned char temp_r, temp_w; 
int value;

	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x03); 			//set table3

	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256);  
	
	sprintf(strInpt, "MCU_I2C_READ(0x%X,137,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp_r = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x89, &temp_r);  //read 0x89 vlaue      
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	temp_w = (0xFD & temp_r) | (INV1<<1);  	
	
	value = temp_w;
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,137,0x%X)", DS1856_SlaAdd, value);  
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "设置INV1失败!     "); 
		return FALSE;
	} 
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x89, temp_w, T_wait); 
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	sprintf(strInpt, "MCU_I2C_READ(0x%X,137,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp_r = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x89, &temp_r);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}

	if(temp_r != temp_w) return FALSE;
	
	return TRUE; 
}

BOOL DS1856_SET_INV2_SM( unsigned char DS1856_SlaAdd, BYTE INV2)
{
unsigned char temp_r, temp_w; 
int value;

	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x03); 			//set table3

	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256);  
	
	sprintf(strInpt, "MCU_I2C_READ(0x%X,137,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp_r = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x89, &temp_r);  //read 0x89 vlaue      
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	temp_w = (0xFE & temp_r) | (INV2<<0);  	
	value = temp_w;
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,137,0x%X)", DS1856_SlaAdd, value);  
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "设置INV2失败!     "); 
		return FALSE;
	} 												
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x89, temp_w, T_wait); 
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	sprintf(strInpt, "MCU_I2C_READ(0x%X,137,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp_r = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x89, &temp_r);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}

	if(temp_r != temp_w) return FALSE;
	
	return TRUE; 
}

BOOL DS1856_SET_RHIZ_SM( unsigned char DS1856_SlaAdd, BYTE RHIZ)
{
unsigned char temp_r, temp_w; 
int value;

	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x03); 			//set table3

	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256);  
	
	sprintf(strInpt, "MCU_I2C_READ(0x%X,110,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp_r = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x6E, &temp_r);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}
	
	temp_w = (0xBF & temp_r) | (RHIZ<<6);  				//TEN=1:LUT;TEN=0:Manul
	
	value = temp_w;
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,110,0x%X)", DS1856_SlaAdd, value);  
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "设置RHIZ失败!     "); 
		return FALSE;
	} 		
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x6E, temp_w, T_wait); 
//	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}

	sprintf(strInpt, "MCU_I2C_READ(0x%X,110,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp_r = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x6E, &temp_r);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}

	temp_r = (0x40 & temp_r) >>6; 
	if(temp_r != RHIZ) return FALSE; 
	
	return TRUE;	
}

BOOL DS1856_ENTRY_PassWord_SM(unsigned char DS1856_SlaAdd, BYTE PW0, BYTE PW1, BYTE PW2, BYTE PW3)
{
unsigned char temp_r, temp_w; 
unsigned char temp_arr[256];
int  		  value1, value2;

//	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x03);

	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,123,0x%X,0x%X,0x%X,0x%X)", DS1856_SlaAdd, PW0, PW1, PW2, PW3);  
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "写PW2密码失败!     "); 
		return FALSE;
	} 		
//	error = I2C_4BYTEs_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x7B, PW0, PW1, PW2, PW3, T_wait);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x03);
	
	sprintf(strInpt, "MCU_I2C_READ(0x%X,128,2)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x,0x%x", &value1, &value2); 
//	error = I2C_BYTEs_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x80, 0x80, temp_arr);     
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
//	if( (temp_arr[128] == 0xFF) && (temp_arr[129] == 0xFF)) return FALSE;
	if( (value1 == 0xFF) && (value2 == 0xFF)) return FALSE;   
	
	return TRUE;					  
}

BOOL DS1856_SET_PassWord_SM(unsigned char DS1856_SlaAdd, BYTE PW0, BYTE PW1, BYTE PW2, BYTE PW3)
{
unsigned char temp_r, temp_w;	  
int 		  value;
							  
	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x03);
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,176,0xFF,0xFF,0xFF,0xFF)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "设置密码PW1失败!     "); 
		return FALSE;
	} 	
//	error = I2C_4BYTEs_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0xB0, 0xFF, 0xFF, 0xFF, 0xFF, T_wait);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,180,0x%X,0x%X,0x%X,0x%X)", DS1856_SlaAdd, PW0, PW1, PW2, PW3);    
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "设置密码PW2失败!     "); 
		return FALSE;
	} 	
//	error = I2C_4BYTEs_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0xB4, PW0, PW1, PW2, PW3, T_wait);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,123,0x%X,0x%X,0x%X,0x%X)", DS1856_SlaAdd, PW0, PW1, PW2, PW3);  
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "写PW2密码失败!     "); 
		return FALSE;
	} 
//	error = I2C_4BYTEs_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x7B, PW0, PW1, PW2, PW3, T_wait);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x05);

	sprintf(strInpt, "MCU_I2C_READ(0x%X,128,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp_r = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x80, &temp_r);     
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	value = temp_r + 1;
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,128,0x%X)", DS1856_SlaAdd, value);  
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "设置128字节失败!     "); 
		return FALSE;
	} 		
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x80, temp_r+1, T_wait); 
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	sprintf(strInpt, "MCU_I2C_READ(0x%X,128,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp_w = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x80, &temp_w);     
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	if(temp_r == temp_w) return FALSE;
	
	value = temp_w - 1;
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,128,0x%X)", DS1856_SlaAdd, value);  
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "设置128字节失败!     "); 
		return FALSE;
	} 	
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x80, temp_w-1, T_wait); 
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x03);
	
	return TRUE;
}

BOOL DS1856_GET_ResFromRpos_SM(unsigned char DS1856_SlaAdd, BYTE DS1856_TYPE, BYTE H0, BYTE H1, double *Res0, double *Res1)
{
double 	c, u, v, w, x, y, z, alfa;
BYTE 	rom_StartAddress, rom_value_arr[256];

	error = DS1856_GET_Temperatrue_SM(DS1856_SlaAdd, &c);
	if (!error) return FALSE;
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	//get resistor for H0
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x7F, 4, T_wait); 				   
//	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x04);  
	
	rom_StartAddress=0xF8;
	sprintf(strInpt, "MCU_I2C_READ(0x%X,%d,8)", DS1856_SlaAdd, rom_StartAddress);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x", &rom_value_arr[rom_StartAddress], &rom_value_arr[rom_StartAddress+1], &rom_value_arr[rom_StartAddress+2], &rom_value_arr[rom_StartAddress+3], 
			&rom_value_arr[rom_StartAddress+4], &rom_value_arr[rom_StartAddress+5], &rom_value_arr[rom_StartAddress+6], &rom_value_arr[rom_StartAddress+7]); 
//	error = I2C_BYTEs_READ_DLL(USB_Handle, DS1856_SlaAdd, rom_StartAddress, 8, rom_value_arr);		   
	u = rom_value_arr[rom_StartAddress];
	v = rom_value_arr[rom_StartAddress+1]*20E-6;
	w = rom_value_arr[rom_StartAddress+2]*100E-9;
	x = rom_value_arr[rom_StartAddress+3]*2+rom_value_arr[rom_StartAddress+4]*pow(2,-7);
	y = rom_value_arr[rom_StartAddress+5]*2E-6;  //DS1856_TYPE 保留
	z = rom_value_arr[rom_StartAddress+6]*10E-9;
	alfa = rom_value_arr[rom_StartAddress+7]*pow(2,-2);
	*Res0 = ((H0+alfa)*x*(1+y*(c-25)+z*(c-25)*(c-25))+u*(1+v*(c-25)+w*(c-25)*(c-25)))/1000;
	
	//get resistor for H1
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x7F, 5, T_wait); 
//	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x05);    
	
	rom_StartAddress=0xF8;
	sprintf(strInpt, "MCU_I2C_READ(0x%X,%d,8)", DS1856_SlaAdd, rom_StartAddress);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x", &rom_value_arr[rom_StartAddress], &rom_value_arr[rom_StartAddress+1], &rom_value_arr[rom_StartAddress+2], &rom_value_arr[rom_StartAddress+3], 
			&rom_value_arr[rom_StartAddress+4], &rom_value_arr[rom_StartAddress+5], &rom_value_arr[rom_StartAddress+6], &rom_value_arr[rom_StartAddress+7]); 
//	error = I2C_BYTEs_READ_DLL(USB_Handle, DS1856_SlaAdd, rom_StartAddress, 8, rom_value_arr);
	u = rom_value_arr[rom_StartAddress];
	v = rom_value_arr[rom_StartAddress+1]*20E-6;
	w = rom_value_arr[rom_StartAddress+2]*100E-9;
	x = rom_value_arr[rom_StartAddress+3]*2+rom_value_arr[rom_StartAddress+4]*pow(2,-7);
	y = rom_value_arr[rom_StartAddress+5]*2E-6;  //DS1856_TYPE 保留 
	z = rom_value_arr[rom_StartAddress+6]*10E-9;
	alfa = rom_value_arr[rom_StartAddress+7]*pow(2,-2);
	*Res1 = ((H1+alfa)*x*(1+y*(c-25)+z*(c-25)*(c-25))+u*(1+v*(c-25)+w*(c-25)*(c-25)))/1000;
	
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x7F, 3, T_wait); 
//	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x03);
	
	return TRUE;
}

BOOL DS1856_GET_RposFromRes_SM( unsigned char DS1856_SlaAdd, BYTE DS1856_TYPE, double Res0, double Res1, BYTE *H0, BYTE *H1)
{
double 	c, u, v, w, x, y, z, alfa;
BYTE 	rom_StartAddress, rom_value_arr[256];

	error = DS1856_GET_Temperatrue_SM(DS1856_SlaAdd, &c);
	if (!error) return FALSE;
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	//get H0 for resistor
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x7F, 4, T_wait); 
//	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x04);
	
	rom_StartAddress=0xF8;
//	error = I2C_BYTEs_READ_DLL(USB_Handle, DS1856_SlaAdd, rom_StartAddress, 8, rom_value_arr);
	sprintf(strInpt, "MCU_I2C_READ(0x%X,%d,8)", DS1856_SlaAdd, rom_StartAddress);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x", &rom_value_arr[rom_StartAddress], &rom_value_arr[rom_StartAddress+1], &rom_value_arr[rom_StartAddress+2], &rom_value_arr[rom_StartAddress+3], 
			&rom_value_arr[rom_StartAddress+4], &rom_value_arr[rom_StartAddress+5], &rom_value_arr[rom_StartAddress+6], &rom_value_arr[rom_StartAddress+7]); 
	
	u = rom_value_arr[rom_StartAddress];
	v = rom_value_arr[rom_StartAddress+1]*20E-6;
	w = rom_value_arr[rom_StartAddress+2]*100E-9;
	x = rom_value_arr[rom_StartAddress+3]*2+rom_value_arr[rom_StartAddress+4]*pow(2,-7);
	y = rom_value_arr[rom_StartAddress+5]*2E-6;  //DS1856_TYPE 保留
	z = rom_value_arr[rom_StartAddress+6]*10E-9;
	alfa = rom_value_arr[rom_StartAddress+7]*pow(2,-2);
	*H0=(BYTE)((Res0*1000.0-u*(1+v*(c-25.0)+w*(c-25.0)*(c-25.0)))/(x*(1+y*(c-25.0)+z*(c-25.0)*(c-25.0)))-alfa);
	
	//get H1 for resistor
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x7F, 5, T_wait); 
//	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x05);
	
	rom_StartAddress=0xF8;

	sprintf(strInpt, "MCU_I2C_READ(0x%X,%d,8)", DS1856_SlaAdd, rom_StartAddress);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x", &rom_value_arr[rom_StartAddress], &rom_value_arr[rom_StartAddress+1], &rom_value_arr[rom_StartAddress+2], &rom_value_arr[rom_StartAddress+3], 
			&rom_value_arr[rom_StartAddress+4], &rom_value_arr[rom_StartAddress+5], &rom_value_arr[rom_StartAddress+6], &rom_value_arr[rom_StartAddress+7]); 
//	error = I2C_BYTEs_READ_DLL(USB_Handle, DS1856_SlaAdd, rom_StartAddress, 8, rom_value_arr);
	u = rom_value_arr[rom_StartAddress];
	v = rom_value_arr[rom_StartAddress+1]*20E-6;
	w = rom_value_arr[rom_StartAddress+2]*100E-9;
	x = rom_value_arr[rom_StartAddress+3]*2+rom_value_arr[rom_StartAddress+4]*pow(2,-7);
	y = rom_value_arr[rom_StartAddress+5]*2E-6;  //DS1856_TYPE 保留 
	z = rom_value_arr[rom_StartAddress+6]*10E-9;
	alfa = rom_value_arr[rom_StartAddress+7]*pow(2,-2);
	*H1=(BYTE)((Res1*1000.0-u*(1+v*(c-25.0)+w*(c-25.0)*(c-25.0)))/(x*(1+y*(c-25.0)+z*(c-25.0)*(c-25.0)))-alfa); 
	
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd, 0x7F, 3, T_wait); 
//	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x03);

	return TRUE;
}

BOOL DS1856_GET_Status_SM( unsigned char DS1856_SlaAdd, BYTE *Status, BYTE *RHIZ, BYTE *TxF, BYTE *RxL, BYTE *Rdyb)
{
BYTE temp;
int  value;

	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	sprintf(strInpt, "MCU_I2C_READ(0x%X,110,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x6E, &temp);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	*Status=temp;
	*RHIZ=(temp & 0x80) >> 7;
	*TxF =(temp & 0x4) >> 2; 
	*RxL =(temp & 0x2) >> 1; 
	*Rdyb=(temp & 0x1) >> 0; 
	
	return TRUE;
}

BOOL DS1856_GET_Mode_SM (unsigned char DS1856_SlaAdd, BYTE *TEN, BYTE *AEN)
{
BYTE temp;
int value;

//	DS1856_SET_Table(USB_Handle, DS1856_SlaAdd, 0x03);
	DS1856_SET_Table_SM(DS1856_SlaAdd, 0x03);  
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	sprintf(strInpt, "MCU_I2C_READ(0x%X,128,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x80, &temp);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}
	
	*TEN = (temp & 0x2) >> 1;
	*AEN = (temp & 0x1) >> 0; 
	
	return TRUE;
}

BOOL DS1856_GET_Config_SM (unsigned char DS1856_SlaAdd, BYTE *ADEN, BYTE *ADFIX, BYTE *INV1, BYTE *INV2)
{
BYTE temp;
int  value;

	DS1856_SET_Table_SM( DS1856_SlaAdd, 0x03);
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	sprintf(strInpt, "MCU_I2C_READ(0x%X,137,1)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd, 0x89, &temp);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}
	
	*ADEN = (temp & 0x20) >> 5;
	*ADFIX= (temp & 0x10) >> 4; 
	*INV1 = (temp & 0x02) >> 1;
	*INV2 = (temp & 0x01) >> 0; 
	
	return TRUE;
}

BOOL DS1856_Chang_Addr_SM (unsigned char DS1856_SlaAdd_OLD, unsigned char DS1856_SlaAdd_NEW)
{
BYTE temp;   
int  valueold, valuenew, value;

	if (!DS1856_SET_Table_SM(DS1856_SlaAdd_OLD, 3)) return FALSE;
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	
	valueold = DS1856_SlaAdd_OLD;
	valuenew = DS1856_SlaAdd_NEW;
	
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,140,0x%X)", valueold, valuenew);  
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "设置140字节失败!     "); 
		return FALSE;
	} 	
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_SlaAdd_OLD, 0x8C, DS1856_SlaAdd_NEW, T_wait);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}
	
	if (!DS1856_SET_ADEN_SM(DS1856_SlaAdd_OLD, 1)) return FALSE; 
	
	if (!DS1856_SET_ADFIX_SM( DS1856_SlaAdd_OLD, 1)) return FALSE;
	
	sprintf(strInpt, "MCU_I2C_READ(0x%X,140,1)", valuenew);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp = value;
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_SlaAdd_NEW, 0x8C, &temp);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;}
	
	if (DS1856_SlaAdd_NEW !=temp) return FALSE;
	
	return TRUE; 
}

BOOL DS1856_GET_Ibias_SM (unsigned char DS1856_SlaAdd, double *Ibias)
{
unsigned short 	temp;
BYTE	MSB, LSB;
int     value_M, value_L;

//	error = I2C_2BYTEs_READ_DLL  (USB_I2CHandle, DS1856_SlaAdd, 0x64, &MSB, &LSB);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 

	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	sprintf(strInpt, "MCU_I2C_READ(0x%X,100,2)", DS1856_SlaAdd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x,0x%x", &value_M, &value_L); 
	
	MSB = value_M;
	LSB = value_L;		
	temp = (MSB <<8) | LSB;
	*Ibias = temp/5000.0;

return TRUE; 
}


int	DS1856_DDMI_GET_ADC_SM (unsigned char DS1856_SlaAdd, BYTE rom_add, unsigned short *ADC)
{
	int 	error, i,sum=0;
	BYTE	MSB, LSB;
	int     value_M, value_L; 
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256);
	for (i=0; i<10; i++)
	{
	//	error = I2C_2BYTEs_READ_DLL  (USB_Handle, DS1856_Slaadd, rom_add, &MSB, &LSB);
	//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;} 
		sprintf(strInpt, "MCU_I2C_READ(0x%X,%d,2)", DS1856_SlaAdd, rom_add);  
		SetCommand(strInpt, strOupt);    
		sscanf(strOupt,"0x%x,0x%x", &value_M, &value_L); 
		MSB = value_M;
		LSB = value_L;
	
		sum += MSB*256+LSB;
	}
	*ADC = sum/10;
	return 0;
}

int DS1856_DDMI_SET_Gain_SM (unsigned char DS1856_Slaadd, BYTE rom_add, unsigned short gain)
{
	int  error;
	BYTE MSB, LSB;
	int  value_M, value_L; 
	
	MSB = gain >> 8;  
	LSB = gain & 0xFF;  	
	
	value_M = MSB;
	value_L = LSB;
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256);
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,%d,0x%X,0x%X)", DS1856_Slaadd, rom_add, value_M, value_L);  
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "设置gain失败!     "); 
		return FALSE;
	} 	
//	error = I2C_2BYTEs_WRITE_DLL (USB_Handle, DS1856_Slaadd, rom_add, MSB, LSB, 0.015);  
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;} 
	
	return 0;
}

int DS1856_DDMI_SET_Offset_SM (unsigned char DS1856_Slaadd, BYTE rom_add, unsigned short Offset)
{
	int  error;
	BYTE MSB, LSB;
	int  value_M, value_L;
	
	MSB = Offset >> 8;  
	LSB = Offset & 0xFF;  
	
	value_M = MSB;
	value_L = LSB;
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256);
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,%d,0x%X,0x%X)", DS1856_Slaadd, rom_add, value_M, value_L);  
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "设置offset失败!     "); 
		return FALSE;
	} 	
//	error = I2C_2BYTEs_WRITE_DLL (USB_Handle, DS1856_Slaadd, rom_add, MSB, LSB, 0.015);  
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;} 
	
	return 0;
}		//512 LINE

int DS1856_DDMI_GET_Gain_SM(unsigned char DS1856_Slaadd, int ADC_target, BYTE ADC_add, BYTE Gain_add, unsigned short *Gain)
{
	unsigned short mygain;
	unsigned char  msb, lsb;
	int i, myadc;
	int  value_M, value_L; 
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256);
	
	mygain=0x8000;
	for (i=15; i>=0; i--)
	{
		msb = mygain >> 8;  
		lsb = mygain & 0xFF;  	
	
		value_M = msb;
		value_L = lsb;
		sprintf(strInpt, "MCU_I2C_WRITE(0x%X,%d,0x%X,0x%X)", DS1856_Slaadd, Gain_add, value_M, value_L);  
		SetCommand(strInpt, strOupt);    
		if(0 != strcmp(strOupt, "OK"))
		{
			MessagePopup ("ERROR", "设置Gain失败!     "); 
			return FALSE;
		} 	
	//	error = I2C_2BYTEs_WRITE_DLL (USB_Handle, DS1856_Slaadd, Gain_add, msb, lsb, 0.015);  
	//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;} 
		
		Delay (0.05);
		
	//	error = I2C_2BYTEs_READ_DLL  (USB_Handle, DS1856_Slaadd, ADC_add, &msb, &lsb);
	//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;}
		sprintf(strInpt, "MCU_I2C_READ(0x%X,%d,2)", DS1856_Slaadd, ADC_add);  
		SetCommand(strInpt, strOupt);    
		sscanf(strOupt,"0x%x,0x%x", &value_M, &value_L); 
		msb = value_M;
		lsb = value_L;
		
		myadc = msb*256+lsb;
		
		if (i>0)
		{
			if (myadc>ADC_target)
			{
				mygain=mygain-pow(2, i-1);	
			}
			else
			{
				mygain=mygain+pow(2, i-1);
			}
		}
		else
		{
			if (myadc>ADC_target)
			{
				mygain=mygain-1;	
			}
			else
			{
				mygain=mygain+1;
			}
		}
	}
	*Gain = mygain;
	
    return 0;
}

int  DS1856_Calibrate_Ibias_SM ( unsigned char DS1856_Slaadd, const float Ibias_amp, const float Ibias_resister, float *Ibias)
{
	unsigned short ADC, ADC_target, gain;
	unsigned char  right_shift_num, right_shift_w, right_shift_r, right_shift; 
	float    Ibias0, Ibias1;
		
	if (!DS1856_SET_Table_SM (DS1856_Slaadd, 3)) return -1; 
	
	//set gain=0xBE9 offset=0 对于大多数DS1856来说gain=0xBE9对应了ADC=FFF8h => 2.5V，实际的出厂设置可能有偏差，可忽略 
	error = DS1856_DDMI_SET_Gain_SM (DS1856_Slaadd, 0x94, 0xBE9);
	if (error<0) return -1;
	
	error = DS1856_DDMI_SET_Offset_SM (DS1856_Slaadd, 0xA4, 0);
	if (error<0) return -1;
	
	//set right shift regediter 8Eh (bit6-bit4 => 0)  
	error = DS1856_DDMI_GET_RightShift_SM (DS1856_Slaadd, 0x8E, &right_shift_r);
	if (error<0) return -1;
	right_shift_w = right_shift_r & 0x8F;
	error = DS1856_DDMI_SET_RightShift_SM (DS1856_Slaadd, 0x8E, right_shift_w);
	if (error<0) return -1;
	
	Delay (0.05);
	
	error = DS1856_DDMI_GET_ADC_SM (DS1856_Slaadd, 0x64, &ADC);  
	if (error<0) return -1; 
	
	Ibias0 = ((ADC*2.5/65535.)*Ibias_amp/Ibias_resister)*1000.; 
	if(Ibias0<5.0 || Ibias0>45.0)
	{
		MessagePopup ("Error", "Ibias out of range 0.0~45.0mA");  
		return -1;	
	}
	
	ADC_target = Ibias0/0.002;
	
	//get adc with 0 right shifts
	error = DS1856_Calibrate_RightShift (ADC_target, &right_shift_num);
	if (error) return -1; 
	ADC_target = ADC_target << right_shift_num;
	
	error = DS1856_DDMI_GET_Gain_SM (DS1856_Slaadd, ADC_target, 0x64, 0x94, &gain);
	if (error<0) return -1; 
	
	error = DS1856_DDMI_SET_Gain_SM (DS1856_Slaadd, 0x94, gain);
	if (error<0) return -1;
	
	//set right shifts
	right_shift =  right_shift_num << 4;
	right_shift_w = right_shift_w | right_shift;
	error = DS1856_DDMI_SET_RightShift_SM ( DS1856_Slaadd, 0x8E, right_shift_w);
	if (error<0) return -1;
	
	Delay (0.05);
	
	error = DS1856_DDMI_GET_ADC_SM ( DS1856_Slaadd, 0x64, &ADC);  
	if (error<0) return -1; 
			  
	Ibias1 =  ADC*0.002;
	
	if (fabs(Ibias0- Ibias1)>4)
	{
		MessagePopup ("Error", "Ibias calibration fail");  
		return -1;	
	}
	
	*Ibias = Ibias1;
	
	return 0;
}

int  DS1856_Calibrate_TxPower_SM ( unsigned char DS1856_Slaadd, const float TxP_in, float *TxP_out)
{
	unsigned short ADC, ADC_target, gain; 
	unsigned char  right_shift_num, right_shift_w, right_shift_r, right_shift; 
	float  tx_power;
	
	if (!DS1856_SET_Table_SM( DS1856_Slaadd, 3)) return -1; 
	
	//set right shift regediter 8Eh (bit2-bit0 => 0)  
	error = DS1856_DDMI_GET_RightShift_SM (DS1856_Slaadd, 0x8E, &right_shift_r);
	if (error<0) return -1;
	right_shift_w = right_shift_r & 0xF8;
	error = DS1856_DDMI_SET_RightShift_SM (DS1856_Slaadd, 0x8E, right_shift_w);
	if (error<0) return -1;
	Delay (0.05);
	
	//get adc with 0 right shifts
	ADC_target= pow(10, TxP_in/10)/0.0001; 
//	error = DS1856_Calibrate_RightShift (ADC_target, &right_shift_num);
//	if (error) return -1; 
//	ADC_target = ADC_target << right_shift_num;
	
	error = DS1856_DDMI_GET_Gain_SM (DS1856_Slaadd, ADC_target, 0x66, 0x96, &gain);
	if (error<0) return -1; 
	
	error = DS1856_DDMI_SET_Gain_SM (DS1856_Slaadd, 0x96, gain);
	if (error<0) return -1;
	
	//set right shifts
//	right_shift =  right_shift_num;
//	right_shift_w = right_shift_w | right_shift;
//	error = DS1856_DDMI_SET_RightShift (USB_Handle, DS1856_Slaadd, 0x8E, right_shift_w);
//	if (error<0) return -1;
	
	Delay (0.05);
	
	error = DS1856_DDMI_GET_ADC_SM (DS1856_Slaadd, 0x66, &ADC);  
	if (error<0) return -1; 
    tx_power= 10*log10(ADC*0.0001);
    
	if (fabs(tx_power - TxP_in)>0.1)
    {
		MessagePopup ("Error", "Tx_Power calibration fail");  
		return -1;
    }

	*TxP_out =  tx_power;
	
	return 0;	
}

int	DS1856_DDMI_GET_RightShift_SM ( unsigned char DS1856_Slaadd, BYTE rom_add, BYTE *RightShift)
{
	int error, value;
	
//	error = I2C_BYTE_READ_DLL  (USB_Handle, DS1856_Slaadd, rom_add, RightShift);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;} 
	sprintf(strInpt, "MCU_I2C_READ(0x%X,%d,1)", DS1856_Slaadd, rom_add);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	*RightShift = value;

	return 0;
}

int	DS1856_DDMI_SET_RightShift_SM ( unsigned char DS1856_Slaadd, BYTE rom_add, BYTE RightShift)
{
	int  error, value;
	
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_Slaadd, rom_add, RightShift, 0.015);  
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;} 
	value = RightShift;
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,%d,0x%X)", DS1856_Slaadd, rom_add, value);  
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "设置RightShift失败!     "); 
		return FALSE;
	} 	
	
	return 0;
}

int  DS1856_Calibrate_RxPower_SM (unsigned char DS1856_Slaadd, const float RxP_in, float *RxP_out)
{
	unsigned short ADC, ADC_target, gain; 
	unsigned char  right_shift_num, right_shift_w, right_shift_r, right_shift;
	float  rx_power;
	
	//get adc with 0 right shifts
	ADC_target= pow(10, RxP_in/10)/0.0001; 
	error = DS1856_Calibrate_RightShift (ADC_target, &right_shift_num);
	if (error) return -1; 
	ADC_target = ADC_target << right_shift_num;
	
	if (!DS1856_SET_Table_SM( DS1856_Slaadd, 3)) return -1; 
	
	//set right shift regediter 8Fh (bit6-bit4 => 0)  
	error = DS1856_DDMI_GET_RightShift_SM (DS1856_Slaadd, 0x8F, &right_shift_r);
	if (error<0) return -1;
	right_shift_w = right_shift_r & 0x8F;
	error = DS1856_DDMI_SET_RightShift_SM (DS1856_Slaadd, 0x8F, right_shift_w);
	if (error<0) return -1;
	
	error = DS1856_DDMI_GET_Gain_SM (DS1856_Slaadd, ADC_target, 0x68, 0x98, &gain);
	if (error<0) return -1; 
	
	error = DS1856_DDMI_SET_Gain_SM (DS1856_Slaadd, 0x98, gain);
	if (error<0) return -1;
	
	//set right shifts
	right_shift =  right_shift_num << 4;
	right_shift_w = right_shift_w | right_shift;
	error = DS1856_DDMI_SET_RightShift_SM (DS1856_Slaadd, 0x8F, right_shift_w);
	if (error<0) return -1;
	
	Delay (0.05);
	
	//检查当前的收端光功率是否读取正确
	error = DS1856_DDMI_GET_ADC_SM ( DS1856_Slaadd, 0x68, &ADC);  
	if (error<0) return -1; 
    rx_power= 10*log10(ADC*0.0001);
    
	if (fabs(rx_power - RxP_in)>1)
    {
		MessagePopup ("Error", "Rx_Power calibration fail");  
		return -1;
    }

	*RxP_out =  rx_power;
	
	return 0;	
}

int DS1856_SET_IntEnable_SM (unsigned char DS1856_Slaadd, BYTE Temp, BYTE Vcc, BYTE Mon1, BYTE Mon2, BYTE Mon3)
{
	int  error, value;
	BYTE temp_w, temp_r;
	
	DS1856_SET_Table_SM( DS1856_Slaadd, 0x03); 
	
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_Slaadd, 0x88, &temp_r);  //read 0x88 vlaue      
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;} 
	sprintf(strInpt, "MCU_I2C_READ(0x%X,136,1)", DS1856_Slaadd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp_r = value;
	
	temp_w = (0x7F & temp_r) | (Temp<<7);  
	temp_w = (0xBF & temp_w) | (Vcc <<6);
	temp_w = (0xDF & temp_w) | (Mon1<<5); 
	temp_w = (0xEF & temp_w) | (Mon2<<4);
	temp_w = (0xF7 & temp_w) | (Mon2<<3); 
													
//	error = I2C_BYTE_WRITE_DLL (USB_Handle, DS1856_Slaadd, 0x88, temp_w, 0.015); 
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;} 
	value = temp_w;
	sprintf(strInpt, "MCU_I2C_WRITE(0x%X,136,0x%X)", DS1856_Slaadd, value);  
	SetCommand(strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "设置IntEnable失败!     "); 
		return FALSE;
	} 	
	
//	error = I2C_BYTE_READ_DLL (USB_Handle, DS1856_Slaadd, 0x88, &temp_r);
//	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return -1;}
	sprintf(strInpt, "MCU_I2C_READ(0x%X,136,1)", DS1856_Slaadd);  
	SetCommand(strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &value); 
	temp_r = value;

	if(temp_r != temp_w) return -1;
	
	return 0;
}//812

BOOL DS1856_GET_LUT_SM( unsigned char DS1856_SlaAdd, BYTE Table, BYTE LUT_arr[72]) 
{
int 	i, rom_StartAddress;
BYTE 	rom_value_arr_r[256];

	DS1856_SET_Table_SM (DS1856_SlaAdd, Table);
	
	memset (rom_value_arr_r, 255, 256); 
	
//	error = I2C_BYTEs_READ_DLL(USB_Handle, DS1856_SlaAdd, 0x80, 72, rom_value_arr_r);
//	if (error<0) { MessagePopup ("ERROR", "No Acknowledge from target !"); return FALSE;}
	for (i=0; i<9; i++) 
	{
		rom_StartAddress=i*8+0x80; 	
	
		sprintf(strInpt, "MCU_I2C_READ(0x%X,%d,8)", DS1856_SlaAdd, rom_StartAddress);  
		SetCommand(strInpt, strOupt);    
		sscanf(strOupt,"0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x", &rom_value_arr_r[rom_StartAddress+0], &rom_value_arr_r[rom_StartAddress+1], &rom_value_arr_r[rom_StartAddress+2],
			   &rom_value_arr_r[rom_StartAddress+3], &rom_value_arr_r[rom_StartAddress+4], &rom_value_arr_r[rom_StartAddress+5], &rom_value_arr_r[rom_StartAddress+6], &rom_value_arr_r[rom_StartAddress+7]); 
	}
	
	for (i=0; i<72; i++)
	{
		LUT_arr[i]=rom_value_arr_r[i+128];
	}	 
	
	DS1856_SET_Table_SM (DS1856_SlaAdd, 0x03);
	
	return TRUE;
}
