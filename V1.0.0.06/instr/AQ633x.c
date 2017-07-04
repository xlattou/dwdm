#include "global.h" 
#include <formatio.h>
#include <utility.h>
#include "aq633x.h"


int AQ633X_Init(ViSession *instHandle, ViRsrc Viname)
{
	ViSession 	defaultRM;
	ViStatus  	status;
	int			rcount;
	char 		buffer[128] = ""; 
	
    viOpenDefaultRM (&defaultRM);
    
    status = viOpen (defaultRM, Viname, VI_NULL, VI_NULL, instHandle);
    if (status < VI_SUCCESS)                                                 
    {                                                                       
    	MessagePopup ("��ʾ", "�������Ự����������û��GPIB����������ֹ");
        return -1;
    }

	viClear (*instHandle);
	
	status = viSetAttribute(*instHandle, VI_ATTR_TMO_VALUE, 10000);  
    status = viWrite (*instHandle, "*IDN?\n",StringLength("*IDN?\n"), &rcount);
    Delay(0.1); 
    status = viRead(*instHandle, buffer, 128, &rcount); 
    if (status < VI_SUCCESS)
    {  
    	MessagePopup ("��ʾ", "error");
    	return -1;
    } 
	
	//sets the header to the talker data
	status = Visa_Write(*instHandle, "HD1");
	if (!status) return -1;
	
	return 0;
}

int AQ633X_Config(ViSession instHandle, int LaserType, ViReal64 ctrwl, ViReal64 span, ViReal64 resolution)
{
	ViStatus  	status;
	char  buf[256];
	
	 //��ֹͣɨ��
	status = Visa_Write(instHandle, "STP");
	if (!status) return -1;
	
	if (LaserType==0) // for FP
	{
		//�������Ĳ���
		sprintf (buf, "CTRWL%.2f", ctrwl);
		status = Visa_Write(instHandle, buf);
		if (!status) return -1;
		
		//����SPAN
		sprintf (buf, "SPAN%.2f", span); 
		status = Visa_Write(instHandle, buf);
		if (!status) return -1;
			
		//����RMSģʽ
		status = Visa_Write(instHandle, "SWRMS");
		if (!status) return -1;
		
		//����RMSģʽ����
		status = Visa_Write(instHandle, "PMSTH10.00");
		if (!status) return -1;
	}
	else	//for DFB 
	{
		//�������Ĳ���
		sprintf (buf, "CTRWL%.2f", ctrwl);
		status = Visa_Write(instHandle, buf);
		if (!status) return -1;
		
		//����SPAN
		sprintf (buf, "SPAN%.2f", span); 
		status = Visa_Write(instHandle, buf);
		if (!status) return -1;
		
		//����bandwidthģʽ
		status = Visa_Write(instHandle, "SWTHR");
		if (!status) return -1;
		
		//���ô������ģʽ  bandwidth MODFT2=ON MODFT1=off 
		status = Visa_Write(instHandle, "MODFT1");
		if (!status) return -1;
		
		//����20dB����������� Sets the threshold value of spectral width
		status = Visa_Write(instHandle, "THRTH20.00");
		if (!status) return -1;
	}
	
	//����Y����Ref LEVEL
	status = Visa_Write(instHandle, "REFL-15.0");
	if (!status) return -1; 
	
	//����Y����SCALEģʽ
	status = Visa_Write(instHandle, "LSCL6.0");
	if (!status) return -1; 
	
	//�����������Զ�ģʽ
	status = Visa_Write(instHandle, "SNAT");
	if (!status) return -1; 
	
	//�����ظ�ɨ��ģʽ
	status = Visa_Write(instHandle, "RPT");
	if (!status) return -1; 
	
	//�����Զ����²��Խ��ģʽ
	status = Visa_Write(instHandle, "ATANA1");
	if (!status) return -1; 
	
	//���÷ֱ���
	sprintf (buf, "RESLN%.2f", resolution);   
	status = Visa_Write(instHandle, buf);
	if (!status) return -1; 
	
	status = Visa_OPC(instHandle, 5);
	if (!status) return -1; 
	
	//Ϊ�˱�֤�����ܹ���Ӧ��������ã���Ҫ����ʱ
	Delay (5);
	
	return 0;
}

int AQ633X_Read(ViSession instHandle, int LaserType, ViReal64 span, double *PeakWavelength, double *Sigma, double *BandWidth, double *SMSR)
{
	float 		peakWavelength, sigma, bandwidth, smsr;
	float 		temp1, temp2, temp3, temp4, temp5; 
	ViStatus  	status; 
	char        buf[500];
	int			rcount, count;
	
	if (LaserType==0) // for FP 
	{
		//�������Ĳ�������ֵ
		status = Visa_Write(instHandle, "CTR=P");
		if (!status) return -1;
		
		//����SPAN
		sprintf (buf, "SPAN%.2f", span); 
		status = Visa_Write(instHandle, buf);
		if (!status) return -1;
		
		//���һ��������ɨ��ʱ��10s
		Delay (10); 
			
		//for smsr
		status = Visa_Write(instHandle, "SWRMS");
		if (!status) return -1;
		
		Delay (1);
		
	    status = Visa_Write(instHandle, "ANA?");
		if (!status ) return FALSE;
    
	    status = viRead(instHandle, buf, 200, &rcount); 
	    if (status < VI_SUCCESS) return FALSE;
	
		if (strcmp (strstr (buf, "SWRM"), "") == 0)
		{
			MessagePopup ("Error", "AQ633X read SMSR results error");
			return -1;
		}
		//���ڷ��ص��ַ��������д����Ƚ���ˢѡ
		strcpy (buf, strstr (buf, "SWRM"));
		sscanf (buf, "SWRM %f,%f", &peakWavelength, &sigma);
	}
	else //for DFB 
	{
		//���һ��������ɨ��ʱ��5s
		Delay (5); 
		
		//for bandwith
		status = Visa_Write(instHandle, "SWTHR");
		if (!status) return -1;
		
		Delay (1);
		
	    status = Visa_Write(instHandle, "ANA?");
		if (!status ) return FALSE;
    
	    status = viRead(instHandle, buf, 200, &rcount); 
	    if (status < VI_SUCCESS) return FALSE;
	
		if (strcmp (strstr (buf, "SWTH"), "") == 0)
		{
			MessagePopup ("Error", "AQ633X read RMS results error");
			return -1;
		}
		//���ڷ��ص��ַ��������д����Ƚ���ˢѡ
		strcpy (buf, strstr (buf, "SWTH"));
		sscanf (buf, "SWTH %f,%f", &peakWavelength, &bandwidth); 
		
		//for smsr
		status = Visa_Write(instHandle, "SMSR1");
		if (!status) return -1;
		
		Delay (1);
		
	    status = Visa_Write(instHandle, "ANA?");
		if (!status ) return FALSE;
    
	    status = viRead(instHandle, buf, 200, &rcount); 
	    if (status < VI_SUCCESS) return FALSE;
	
		if (strcmp (strstr (buf, "SMSR"), "") == 0)
		{
			MessagePopup ("Error", "AQ633X read SMSR results error");
			return -1;
		}
		//���ڷ��ص��ַ��������д����Ƚ���ˢѡ
		strcpy (buf, strstr (buf, "SMSR"));
		sscanf (buf, "SMSR %f,%f,%f,%f,%f,%f", &temp1, &temp2, &temp3, &temp4, &temp5, &smsr);
		
		//��temp5������peak֮����׿�С��0.1nm����ʱ�Ĳ��Խ���Ǵ���ģ���Ҫ����Ӧ�Ĵ���
		//SMSR 1490.500,  -9.39,1477.600, -56.91,  12.900,  47.52 
		count=0;
		while (fabs(temp5)<0.1 && count<4)
		{
			//�������Ĳ�������ֵ
			status = Visa_Write(instHandle, "CTR=P");
			if (!status) return -1;

			//����SPAN
			sprintf (buf, "SPAN%.2f", span+10.); 
			status = Visa_Write(instHandle, buf);
			if (!status) return -1;
			
			//���һ��������ɨ��ʱ��5s
			Delay (5); 
			
			//for smsr
			status = Visa_Write(instHandle, "SMSR1");
			if (!status) return -1;
		
			Delay (1);
		
		    status = Visa_Write(instHandle, "ANA?");
			if (!status ) return FALSE;
    
		    status = viRead(instHandle, buf, 200, &rcount); 
		    if (status < VI_SUCCESS) return FALSE;
	
			if (strcmp (strstr (buf, "SMSR"), "") == 0)
			{
				MessagePopup ("Error", "AQ633X read SMSR results error");
				return -1;
			}
			//���ڷ��ص��ַ��������д����Ƚ���ˢѡ
			strcpy (buf, strstr (buf, "SMSR"));
			sscanf (buf, "SMSR %f,%f,%f,%f,%f,%f", &temp1, &temp2, &temp3, &temp4, &temp5, &smsr);
			
			count++;
		}
		//����SPAN
		sprintf (buf, "SPAN%.2f", span); 
		status = Visa_Write(instHandle, buf);
		if (!status) return -1;
	}
	
	*PeakWavelength=peakWavelength;
	
	if (LaserType==0)
	{
		*BandWidth=0;
		*SMSR=0;
		*Sigma=sigma; 
	}
	else
	{
		*BandWidth=bandwidth;
		*SMSR=smsr;
		*Sigma=0;
	}
	
	return 0;
}
