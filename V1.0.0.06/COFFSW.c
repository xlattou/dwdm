#include <ansi_c.h>
#include <rs232.h>
#include <userint.h>
#include <utility.h>
#include "COFFSW.h" 

int COFFSW_Init(int COMIndex)
{
int  Status; 

	Status = OpenComConfig (COMIndex, "", 9600, 0, 8, 1, 0, -1);  
	if(Status != 0) 
	{
		MessagePopup ("error", "�⿪�ش�������ʧ�ܣ����鴮�������Ƿ���ȷ�������Ƿ�ռ�ã�"); 
		return -1;
	} 
	
	return 0;
}

int COFFSW_Close(int COMIndex)
{
int Status; 

	Status = CloseCom (COMIndex);
	if(Status != 0) {MessagePopup ("error", "�⿪�ش��ڹر�ʧ��");return -1;} 
	
	return 0;
}

int COFFSW_SetChannel(int COMIndex, int channel)
{
	int count;
	char buf_w[50],buf_r[50];

	if ((8<channel) || (1>channel))
	{
		MessagePopup ("��ʾ", "�⿪��ͨ��ѡ�����    ");
		return -1;
	}
	
	sprintf(buf_w, "<AD01_S_%02d>", channel);
	count = ComWrt (COMIndex, buf_w, sizeof(buf_w));
	if(sizeof(buf_w) != count) {MessagePopup ("error", "FSW Send command fail"); return -1;}  
	
	Delay(0.1); 

	return 0;
}

int COFFSW02_SetChannel(int COMIndex, int channel)
{
	int count;
	char buf_w[6];

	if ((8<channel) || (1>channel))
	{
		MessagePopup ("��ʾ", "�⿪��ͨ��ѡ�����    ");
		return -1;
	}
	
	buf_w[0] = 0xEE;
	buf_w[1] = 0xAA;  
	buf_w[2] = 0x02;  
	buf_w[3] = 0x01;  
	buf_w[4] = 0x01;  
	buf_w[5] = channel;  
	
	count = ComWrt (COMIndex, buf_w, sizeof(buf_w));
	if(sizeof(buf_w) != count) {MessagePopup ("error", "FSW Send command fail"); return -1;}  
	
	Delay(0.1); 

	return 0;
}
