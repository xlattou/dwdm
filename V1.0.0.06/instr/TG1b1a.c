#include <ansi_c.h>
#include <visa.h> 
#include "global.h"
#include <formatio.h>
#include <utility.h>
#include <userint.h>   

BOOL TG1b1a_Init(ViSession *instHandle, ViRsrc Viname, int Freq, char *pattern)
{
int	rcount;
ViSession defaultRM; 
char	buffer[128]="", str[256]="";
BOOL   	Status; 

	//Open instruments resource
    viOpenDefaultRM (&defaultRM);
    
    Status = viOpen (defaultRM, Viname, VI_NULL, VI_NULL, instHandle);
    if (Status < VI_SUCCESS)                                                 
    {                                                                       
    	MessagePopup ("提示", "打开仪器会话出错，可能是没有GPIB卡，程序将终止!       ");
        return FALSE;
    }
	Status = viSetAttribute(*instHandle, VI_ATTR_TMO_VALUE, 50000);  
	
    Status = viWrite (*instHandle, "*IDN?\n",StringLength("*IDN?\n"), &rcount);
    Delay(0.1); 
    Status = viRead(*instHandle, buffer, 128, &rcount); 
    if ((Status < VI_SUCCESS) || (NULL == strstr (buffer, "TG1B1-A")))
    {  
    	MessagePopup ("Error", "Can not find TG1B1-A, Please check the address is right  !");
    	return FALSE;
    } 
	
    //通讯成功后，进行仪器设置
	Status = Visa_Write(*instHandle, ":TRIG:ABOR");
	if (!Status) return FALSE;
	
    Status = Visa_Write(*instHandle, "*RST");
	if (!Status) return FALSE;
	
	Status = Visa_OPC(*instHandle, 1); 
	if (!Status) return FALSE;
	
	//TxCLK Source
	Status = Visa_Write(*instHandle, ":SOUR:ROSC:SOUR INT");
	if (!Status) return FALSE;
	
	//mode 
	Status = Visa_Write(*instHandle, ":MODe NORM");
	if (!Status) return FALSE;
	
	//amplitude 
	Status = Visa_Write(*instHandle, ":SOUR:VOLT:AMPL 500");
	if (!Status) return FALSE;
	//phase
	Status = Visa_Write(*instHandle, ":SENS:ROSC:PHAS 90");
	if (!Status) return FALSE;
	
	//速率 
	memset(str, 0, 256);
	sprintf(str, ":SOUR:ROSC:INT:FREQ %d", Freq);
	Status = Visa_Write(*instHandle, str);
	if (!Status) return FALSE;
	
	//PATTern PRBS31
	memset(str, 0, 256);
	sprintf(str, ":SOUR:PATT %s", pattern);
	Status = Visa_Write(*instHandle, str);
	if (!Status) return FALSE;
	
	//Density
	Status = Visa_Write(*instHandle, ":SOUR:PRBS:MARK MS2");
	if (!Status) return FALSE;
	//
	Status = Visa_Write(*instHandle, ":TRIG:INIT");
	if (!Status) return FALSE;
	//
	Status = Visa_Write(*instHandle, ":INIT:BER");
	if (!Status) return FALSE;
	
	return TRUE;   
}

BOOL TG1b1a_Phase(ViSession instHandle, int inv_flag, double berclass, int *phase)
{
int	rcount, i, pha=0;
double temp_ber=0.0;
char buffer[256];
BOOL   	Status; 

	if (0 == inv_flag)
	{
		Status = Visa_Write(instHandle, ":SOUR:PRBS:INV OFF");
		if (!Status) return FALSE;	 
	}
	else
	{
		Status = Visa_Write(instHandle, ":SOUR:PRBS:INV ON");
		if (!Status) return FALSE;
	}
	i=0;
	do
	{
    	Status = Visa_Write(instHandle, ":TRIG:INIT");
		if (!Status) return FALSE;
		Delay(1);
		Status = Visa_Write(instHandle, ":TRIG:ABOR");
		if (!Status) return FALSE;
		Delay(0.2); 
		Status = Visa_Write(instHandle, ":SENS:ROSC:PHAS AUTO");
		if (!Status) return FALSE;
		Delay(0.2);
		Status = Visa_Write (instHandle, ":FETC:BER? BER");
		if (!Status) return FALSE;   
		Delay(0.2); 
		Status = viRead(instHandle, buffer, 256, &rcount);  
	//	if (!Status) //return FALSE;   
		Scan (buffer, "%s>%f", &temp_ber);
		i++;
		
	}while ((i<3)&&(temp_ber > berclass*10));
	
	if(i>=3)
		return FALSE; 
	memset(buffer, 0, sizeof(buffer));
	Status = Visa_Write(instHandle, ":SENS:ROSC:PHAS?");
	if (!Status) //return FALSE;
	Delay(0.2);
	Status = viRead(instHandle, buffer, 128, &rcount); 
    if (Status < VI_SUCCESS) return FALSE;
		Scan (buffer, "%s>%d", &pha);  
	*phase = pha;

	return TRUE;
}

BOOL TG1b1a_Align(ViSession instHandle, int *phase)
{
	int	rcount, i, pha=0;
	double temp_ber=0.0;
	char buffer[256];
	BOOL   	Status; 

	i=0;
	do
	{
    	Status = Visa_Write(instHandle, ":TRIG:INIT");
		if (!Status) return FALSE;
		Delay(0.5);
		Status = Visa_Write(instHandle, ":TRIG:ABOR");
		if (!Status) return FALSE;
		Delay(0.2); 
		Status = Visa_Write(instHandle, ":SENS:ROSC:PHAS AUTO");
		if (!Status) return FALSE;
		Delay(0.1);
	/*	Status = Visa_Write (instHandle, ":FETC:BER? BER");
		if (!Status) return FALSE;   
		Delay(0.2); 
		Status = viRead(instHandle, buffer, 256, &rcount);  
	//	if (!Status) //return FALSE;   
		Scan (buffer, "%s>%f", &temp_ber);	   */
		i++;		
	}while (i<2);
	
	memset(buffer, 0, sizeof(buffer));
	Status = Visa_Write(instHandle, ":SENS:ROSC:PHAS?");
	if (!Status)  return FALSE;
	Delay(0.2);
	Status = viRead(instHandle, buffer, 128, &rcount); 
//    if (Status < VI_SUCCESS) return FALSE;
	Scan (buffer, "%s>%d", &pha);  
	*phase = pha;

	return TRUE;
}

BOOL TG1b1a_Phase_Ber(ViSession instHandle, int *phase, double *ber)
{
	int	rcount, i, pha=0;
	double temp_ber=0.0;
	char buffer[256];
	BOOL   	Status; 
	
	Status = Visa_Write(instHandle, ":TRIG:INIT");
	if (!Status) return FALSE;
	Delay(1);
	Status = Visa_Write(instHandle, ":TRIG:ABOR");
	if (!Status) return FALSE;
//	Delay(0.2); 
	Status = Visa_Write(instHandle, ":SENS:ROSC:PHAS AUTO");
	if (!Status) return FALSE;
	Delay(0.2);
	Status = Visa_Write (instHandle, ":FETC:BER? BER");
	if (!Status) return FALSE;   
	Delay(0.2); 
	Status = viRead(instHandle, buffer, 256, &rcount);  
//	if (!Status) //return FALSE;   
	Scan (buffer, "%s>%f", &temp_ber);
	*ber = temp_ber; 
	
	memset(buffer, 0, sizeof(buffer));
	Status = Visa_Write(instHandle, ":SENS:ROSC:PHAS?");
	if (!Status) return FALSE;
	Delay(0.2);
	Status = viRead(instHandle, buffer, 256, &rcount); 
 //   if (Status < VI_SUCCESS) //return FALSE;
	Scan (buffer, "%s>%d", &pha);  
	*phase = pha;

	return TRUE;
}

BOOL TG1b1a_Start_Test(ViSession instHandle)
{
	BOOL   	Status; 
	Status = Visa_Write(instHandle, ":TRIG:INIT");
	if (!Status) return FALSE;
	
	return TRUE;
}

BOOL TG1b1a_End_Test(ViSession instHandle)
{
	BOOL   	Status; 
	Status = Visa_Write(instHandle, ":TRIG:ABOR");
	if (!Status) return FALSE;
	
	return TRUE;
}

BOOL TG1b1a_Set_Phase(ViSession instHandle, int phase)
{
	char    str[256];
	BOOL   	Status; 
	//phase
	memset(str, 0, 256);
	sprintf(str, ":SENS:ROSC:PHAS %d", phase);
	Status = Visa_Write(instHandle, str); 
	if (!Status) return FALSE;
	
	return TRUE;
}

BOOL TG1b1a_GET_ErrorBit(ViSession instHandle, double *datacount, double *errorcount, double *elapsedtime, double *Ber)
{
int rcount;
double temp_ber, datac, errorc, etime;
char buf[1024], *strdata, *strerror, *strtime, *strber;
BOOL   	Status; 
	
/*	Status = Visa_Write(instHandle, ":TRIG:SAMPLE");  
	if (!Status ) return FALSE;
	
	Status = Visa_Write(instHandle, ":SENS:DATA? ALL");  //  ":SENS:DATA? ALL"
	if (!Status ) return FALSE;	
	Status = viRead(instHandle, buf, 1024, &rcount); 
    if (Status < VI_SUCCESS) return FALSE;		*/
    memset(buf, 0, 1024);
	Status = Visa_Write (instHandle, ":TRIGger:ABORt");
	if (!Status ) return FALSE;  
//	Status = Visa_Write (instHandle, ":FETCh:BER? BER");    
	Status = Visa_Write (instHandle, ":FETCh:BER? ALL");
	if (!Status ) return FALSE;    
 
    Status = viRead(instHandle, buf, 1024, &rcount); 
    if (Status < VI_SUCCESS) return FALSE;
	Scan (buf, "%s>%f[x]%f[x]%f[x]%f", &datac, &errorc, &etime, &temp_ber);  
	*datacount = datac;
	*errorcount = errorc;
	*elapsedtime = etime;
	*Ber = temp_ber;
	
	return TRUE;
}

BOOL TG1b1a_GET_BER(ViSession instHandle, double *Ber)
{
int rcount;
double temp_ber, datac, errorc, etime;
char buf[1024], *strdata, *strerror, *strtime, *strber;
BOOL   	Status; 
	
    memset(buf, 0, 1024);
	Status = Visa_Write (instHandle, ":TRIGger:ABORt");
	if (!Status ) return FALSE;  
	Status = Visa_Write (instHandle, ":FETCh:BER? BER");    
	if (!Status ) return FALSE;    
 
    Status = viRead(instHandle, buf, 1024, &rcount); 
    if (Status < VI_SUCCESS) return FALSE;
	Scan (buf, "%s>%f", &temp_ber);  
	
	*Ber = temp_ber;
	
	return TRUE;
}

BOOL TG1b1a_Config(ViSession instHandle, int Freq, int phase, int volamp, int inv_flag, char *pattern, char *maskspace, char *ierrrate)
{
	char	buffer[128]="", str[256]="";
	BOOL   	Status; 

	Status = Visa_Write(instHandle, ":TRIG:ABOR");
	if (!Status) return FALSE;
	//速率 
	memset(str, 0, 256);
	sprintf(str, ":SOUR:ROSC:INT:FREQ %d", Freq);
	Status = Visa_Write(instHandle, str);
	if (!Status) return FALSE;
	//PATTern PRBS31
	memset(str, 0, 256);
	sprintf(str, ":SOUR:PATT %s", pattern);
	Status = Visa_Write(instHandle, str);
	if (!Status) return FALSE;
	//amplitude 
	memset(str, 0, 256);
	sprintf(str, ":SOUR:VOLT:AMPL %d", volamp);
	Status = Visa_Write(instHandle, str); 
//	Status = Visa_Write(instHandle, ":SOUR:VOLT:AMPL 500");
	if (!Status) return FALSE;
	//phase
	memset(str, 0, 256);
	sprintf(str, ":SENS:ROSC:PHAS %d", phase);
	Status = Visa_Write(instHandle, str); 
//	Status = Visa_Write(instHandle, ":SENS:ROSC:PHAS 90");
	if (!Status) return FALSE;
	//invert
	if (0 == inv_flag)
	{
		Status = Visa_Write(instHandle, ":SOUR:PRBS:INV OFF");
		if (!Status) return FALSE;	 
	}
	else
	{
		Status = Visa_Write(instHandle, ":SOUR:PRBS:INV ON");
		if (!Status) return FALSE;
	}
	//Density
	memset(str, 0, 256);
	sprintf(str, ":SOUR:PRBS:MARK %s", maskspace);
	Status = Visa_Write(instHandle, str); 
//	Status = Visa_Write(instHandle, ":SOUR:PRBS:MARK MS2");
	if (!Status) return FALSE;
	
	//
	memset(str, 0, 256);
	sprintf(str, ":SOUR:PRBS:IERR:RAT %s", ierrrate);
	Status = Visa_Write(instHandle, str); 
//	Status = Visa_Write(instHandle, ":SOUR:PRBS:IERR:RAT ERR1E3");
	if (!Status) return FALSE;
	
	Status = Visa_Write(instHandle, ":TRIG:INIT");
	if (!Status) return FALSE;
	//
	Status = Visa_Write(instHandle, ":INIT:BER");
	if (!Status) return FALSE;
	
	return TRUE;   
}

BOOL TG1b1a_Get_Rate(ViSession instHandle, double *Rate)
{
int 	rcount;
double  temp_ber;
char    buf[1024];
BOOL   	Status; 
	
    memset(buf, 0, 1024);
	
	Status = Visa_Write (instHandle, ":SOUR:ROSC:FREQ?");    
	if (!Status ) return FALSE;    
 
    Status = viRead(instHandle, buf, 1024, &rcount); 
    if (Status < VI_SUCCESS) return FALSE;
	Scan (buf, "%s>%f", &temp_ber);  
	
	*Rate = temp_ber;
	
	return TRUE;
}
