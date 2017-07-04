#include "global.h"
#include <formatio.h>
#include <utility.h>
#include <visa.h>
#include <userint.h>
#include <ansi_c.h>
#include <toolbox.h> 

static unsigned char Temp_str[256]; 

BOOL HP8156_Init(ViSession *instHandle, ViRsrc Viname, int Wavelength, float AttValue)

{
int			tmp;
int 		ccc;
int 		status;
int 		rcount;
char 		buffer[128] = "";
ViSession 	defaultRM; 

	//Open instruments resource
    viOpenDefaultRM (&defaultRM);
    
    status = viOpen (defaultRM, Viname, VI_NULL, VI_NULL, instHandle);
    if (status < VI_SUCCESS)                                                 
    {                                                                       
    	MessagePopup ("提示", "打开仪器会话出错，可能是没有GPIB卡，程序将终止!       ");
        return FALSE;
    }
 
	status = viSetAttribute(*instHandle, VI_ATTR_TMO_VALUE, 5000);  
	if (status < VI_SUCCESS) return FALSE;
	
    status = viWrite (*instHandle, "*IDN?\n",StringLength("*IDN?\n"), &rcount);
    if (status < VI_SUCCESS) 
    {
    	MessagePopup ("提示", "请检查设备地址是否设置正确  !");
    	return FALSE; 
    }
    Delay(0.1); 
    status = viRead(*instHandle, buffer, 128, &rcount); 
    if (status < VI_SUCCESS)
    {  
    	MessagePopup ("提示", "无法和HP8156通讯，请确认仪器是否开启，\n是否正常连接,以及GPIB拨码开关是否设置为6!     \n 程序将终止？                ");
    	return FALSE;
    } 
    
    //通讯成功后，进行仪器设置
    status = Visa_Write(*instHandle, "*CLS");
	if (!status) return FALSE;
	
	status = Visa_Write(*instHandle, ":DISPlay:ENABle ON");
	if (!status) return FALSE; 
	
	status = Visa_Write(*instHandle, ":INPut:LCMode OFF");
	if (!status) return FALSE;
	
	status = Visa_Write(*instHandle, ":INPut:OFFSet 0");
	if (!status) return FALSE;
	
	sprintf (Temp_str, ":INPut:ATTenuation %f", AttValue);     
	status = Visa_Write(*instHandle, Temp_str);
	if (!status) return FALSE; 
	
	sprintf (Temp_str, ":INPut:WAVelength %dnm", Wavelength);     
	status = Visa_Write(*instHandle, Temp_str);
	if (!status) return FALSE;

	status = Visa_Write(*instHandle, ":OUTP ON");
	if (!status) return FALSE;
								   	
	return TRUE;   
}

BOOL HP8156_SET_ATT(ViSession instHandle, float ATT)  
{
char Temp_str[128];
int  rcount, count;
BOOL Status;
double fAtt;

    sprintf (Temp_str, ":INPut:ATTenuation %f", ATT);
    
    Status = Visa_Write(instHandle, Temp_str);
	if (!Status ) return FALSE;
	Delay(0.1);
	count=0;
	do 
	{
		Status = Visa_Write(instHandle, ":INPut:ATTenuation?"); 
		
	    Status = viRead(instHandle, Temp_str, 128, &rcount); 
	    if (Status < VI_SUCCESS) return FALSE;
		Scan (Temp_str, "%s>%f", &fAtt);
	
		if (fabs(ATT-fAtt)<0.1) break;
		Delay(0.5);count++;
	}while (count<10);
	
	//需要确认回读衰减量达到设置值时,光功率是否也达到了要求的值
	
	if (count>10) return FALSE;
	
	return TRUE;
}

BOOL HP8156_SET_WaveLength(ViSession instHandle, int Wavelength) 
{
int status;
int rcount; 

    sprintf (Temp_str, ":INPut:WAVelength %snm\n", Wavelength);     
	status = viWrite (instHandle, Temp_str,StringLength(Temp_str), &rcount);
	if (!status) { MessagePopup ("error", "Initial HP8156A error!"); return FALSE;}
	
	return TRUE;
}

