#include <formatio.h>
#include <utility.h>
#include <toolbox.h>  
#include <visa.h>  

static unsigned char	Temp_str[1024]; 
static BOOL   			Status; 

BOOL OLA55M_Init(ViSession *instHandle, int COM, int WaveLength, int ATT_MODE)
{
ViSession 	defaultRM;
int 		rcount;

	//Open instruments resource
    viOpenDefaultRM (&defaultRM);
						
    sprintf (Temp_str, "ASRL%d::INSTR", COM);
    Status = viOpen (defaultRM, Temp_str, VI_NULL, VI_NULL, instHandle);  
    if (Status != VI_SUCCESS) return FALSE; 
    
    //设置串口通讯的参数		   
	Status = viSetAttribute(*instHandle, VI_ATTR_TMO_VALUE, 5000);  
    if(Status !=VI_SUCCESS) return FALSE;
	Status = viSetAttribute (*instHandle, VI_ATTR_ASRL_BAUD, 115200); 
    if (Status != VI_SUCCESS) return FALSE;
    Status = viSetAttribute (*instHandle, VI_ATTR_ASRL_DATA_BITS, 8); 
    if (Status != VI_SUCCESS) return FALSE;
    Status = viSetAttribute (*instHandle, VI_ATTR_ASRL_PARITY, VI_ASRL_PAR_NONE); 
    if (Status != VI_SUCCESS) return FALSE;
    Status = viSetAttribute (*instHandle, VI_ATTR_ASRL_STOP_BITS, VI_ASRL_STOP_ONE); 
    if (Status != VI_SUCCESS) return FALSE;
	Status = viSetAttribute (*instHandle, VI_ATTR_ASRL_FLOW_CNTRL, VI_ASRL_FLOW_NONE);
    if (Status != VI_SUCCESS) return FALSE;
	Status = viSetAttribute (*instHandle, VI_ATTR_ASRL_END_IN, VI_ASRL_END_TERMCHAR);
    if (Status != VI_SUCCESS) return FALSE;
	Status = viSetAttribute (*instHandle, VI_ATTR_ASRL_END_OUT, VI_ASRL_END_NONE);
    if (Status != VI_SUCCESS) return FALSE;
	Status = viSetAttribute (*instHandle, VI_ATTR_TERMCHAR_EN, VI_TRUE);  //Specify that the read operation should terminate when a termination character is received.
 	if (Status != VI_SUCCESS) return FALSE;
	Status = viSetAttribute (*instHandle, VI_ATTR_TERMCHAR, 0x0A);   //十进制的10 来做 结束符Set the termination character to 0xA
	if (Status != VI_SUCCESS) return FALSE;

	Status = viWrite (*instHandle, "*IDN?\n",StringLength("*IDN?\n"), &rcount); 
	if (Status != VI_SUCCESS) return FALSE; 
	Delay(1); 
    Status = viRead(*instHandle, Temp_str, 1024, &rcount); 
    if (Status != VI_SUCCESS) return FALSE;  
    
	if(ATT_MODE==1)	//set ALC mode 
		Status= viWrite(*instHandle,":DEV:MODE ALC\n",StringLength(":DEV:MODE ALC\n"), &rcount); 
	else	 		//set ATT mode 
 		Status= viWrite(*instHandle,":DEV:MODE ATT\n",StringLength(":DEV:MODE ATT\n"), &rcount); 
	
    if(Status < VI_SUCCESS)                                                 
    {                                                                       
            MessagePopup("error", "设置OLA-55M衰减模式出错！");
            return FALSE;
    }

	sprintf (Temp_str, ":SYST:CAL:WAV:TAB %d\n", WaveLength);
	Status = viWrite (*instHandle, Temp_str,StringLength(Temp_str), &rcount);
	if (Status != VI_SUCCESS) return FALSE;
	
	return TRUE;   
}

BOOL OLA55M_Set_ATTValue(ViSession instHandle, float Value)
{
	char 	buffer[200];
	double	ATT_value_temp;
	int		rcount;
	
	sprintf (Temp_str, ":ATT:POW:ATT:ABS %f\n", Value);
	Status = viWrite (instHandle, Temp_str,StringLength(Temp_str), &rcount);
	if(Status < VI_SUCCESS)                                                 
    {                                                                       
        MessagePopup("error", "设置OLA-55M衰减值出错！");
        return FALSE;
    }
   
    Delay(0.2);
	
	strcpy (Temp_str, ":ATT:POW:ATT:ABS?\n");
	Status = viWrite (instHandle, Temp_str,StringLength(Temp_str), &rcount);
	
	Status = viRead(instHandle, buffer, 128, &rcount); 
	if(Status < VI_SUCCESS)                                                 
    {                                                                       
        MessagePopup("error", "设置OLA-55M衰减值回读出错！");
        return FALSE;
    }
    Scan (buffer, "%s>%f", &ATT_value_temp);

	if(fabs(ATT_value_temp-Value)>0.2) MessagePopup ("error", "设置OLA-55M衰减值与回读值不一致！   ");

	return TRUE;
}

BOOL OLA55M_Set_ALCValue(ViSession instHandle, float Value)
{
	char 	buffer[200];
	double	ATT_value_temp;
	int		rcount;
	
	sprintf (Temp_str, ":ALC:NOM:VAL %f\n", Value);
	Status = viWrite (instHandle, Temp_str,StringLength(Temp_str), &rcount);
	if(Status < VI_SUCCESS)                                                 
    {                                                                       
        MessagePopup("error", "设置OLA-55M输出值出错！");
        return FALSE;
    }
   
    Delay(0.2);
	
	strcpy (Temp_str, ":ALC:NOM:VAL?\n");
	Status = viWrite (instHandle, Temp_str,StringLength(Temp_str), &rcount);
	
	Status = viRead(instHandle, buffer, 128, &rcount); 
	if(Status < VI_SUCCESS)                                                 
    {                                                                       
        MessagePopup("error", "设置OLA-55M输出值回读出错！");
        return FALSE;
    }
    Scan (buffer, "%s>%f", &ATT_value_temp);
    if(fabs(ATT_value_temp-Value)>0.2) MessagePopup ("error", "设置OLA-55M输出值与回读值不一致！   ");

	return TRUE;
}

BOOL OLA55M_Set_WaveLength(ViSession instHandle, int WaveLength)
{
int rcount;
	sprintf (Temp_str, ":SYST:CAL:WAV:TAB %d\n", WaveLength);
	Status = viWrite (instHandle, Temp_str,StringLength(Temp_str), &rcount);
	if (Status != VI_SUCCESS) return FALSE;
	
	return TRUE;
}

BOOL OLA55M_Get_Power(ViSession instHandle, float *Power)
{
   	char    command_buffer[200]="";
	int		rcount;

   	Status = viWrite(instHandle,":FETC:POW:INP?\n",StringLength(":FETC:POW:INP?\n"), &rcount); 
	if (Status != VI_SUCCESS) return FALSE; 
   	Status = viRead(instHandle, command_buffer, 200, &rcount);
	if (Status != VI_SUCCESS) return FALSE; 
   	Fmt (Power, "%f<%s", command_buffer);  
   
	return TRUE;
}


