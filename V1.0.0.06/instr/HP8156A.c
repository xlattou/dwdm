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
    	MessagePopup ("��ʾ", "�������Ự����������û��GPIB����������ֹ!       ");
        return FALSE;
    }
 
	status = viSetAttribute(*instHandle, VI_ATTR_TMO_VALUE, 5000);  
	if (status < VI_SUCCESS) return FALSE;
	
    status = viWrite (*instHandle, "*IDN?\n",StringLength("*IDN?\n"), &rcount);
    if (status < VI_SUCCESS) 
    {
    	MessagePopup ("��ʾ", "�����豸��ַ�Ƿ�������ȷ  !");
    	return FALSE; 
    }
    Delay(0.1); 
    status = viRead(*instHandle, buffer, 128, &rcount); 
    if (status < VI_SUCCESS)
    {  
    	MessagePopup ("��ʾ", "�޷���HP8156ͨѶ����ȷ�������Ƿ�����\n�Ƿ���������,�Լ�GPIB���뿪���Ƿ�����Ϊ6!     \n ������ֹ��                ");
    	return FALSE;
    } 
    
    //ͨѶ�ɹ��󣬽�����������
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
	
	//��Ҫȷ�ϻض�˥�����ﵽ����ֵʱ,�⹦���Ƿ�Ҳ�ﵽ��Ҫ���ֵ
	
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

