#include <ansi_c.h>
#include <userint.h>
#include "cvidll.h"
#include "SEVB0027_4001.h"
#include "toolbox.h"

int SEVB0027_4001_Init (int *Inst, char *SN)
{
	int error;
	int lpdwNumDevices;
	int iIndex;
	char myUSBSN[30];
	int I2Cslave_found;
	
	error = GetF320Numbers(&lpdwNumDevices); 
	if(error==0 && lpdwNumDevices>0)
	{
		for (iIndex=0x800; iIndex<lpdwNumDevices+0x800; iIndex++) 
		{
			*Inst = iIndex;
						
			//read F320 SN
			memset (myUSBSN, 0, sizeof(myUSBSN));
			error = GetF320SerialNumber(iIndex-0x800, myUSBSN);
			
			MyDLLCheckSN(myUSBSN);
			
			//compare SN
			error = strcmp (myUSBSN, SN);
			if (error && (iIndex-0x800)>=(lpdwNumDevices-1)) 
			{
				MessagePopup("��ʾ","ѡ�õ�SEVB0027_4001���кź�ʵ�����кŲ�һ��");
				return -1;
			}
			
			if (error==0) 
			{
				I2Cslave_found=1;
				break;
			}
		}
	}
	
	if (I2Cslave_found==0) 
	{
		return -1;
	}
	
	return 0;
}

int SEVB0027_4001_SetChannel(int Inst, int channel)
{
	int error;
	int count;
	BYTE chan;
	
	if (0>channel || 7<channel)
	{
		MessagePopup("��ʾ","SEVB0027_4001ͨ�����ô���    ");
		return -1;
	}
	
	count=0;
	do
	{
		errChk(SEL_CLK_F320_DLL(Inst-0x800, channel));
	
		errChk(CLK_Get_F320_DLL(Inst-0x800, &chan));
	
		if (channel==chan) break;
	 
		count++;
		
	} while (count<10);
	
	if (count>=10)
	{
		MessagePopup("��ʾ","SEVB0027_4001ͨ�����ô���    ");
		return -1;
	}
	
Error:

	return error;
}
