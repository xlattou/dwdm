/****************************************************************************
 *
 * File:                B2BERT.h
 *
 * Author:              SUPERXON INC.(Roger Li)
 *
 * Description:         B2BERT.c ͷ�ļ�
 *
 * Time:                2010-03-29
 *
 * version:				V2.0.0.1
 * 
 * Update Description:  new document
V2.0.0.1   
1. ɾ������B2BERT_GET_BURST_SDChk_Status_noClear ()
2. ɾ������B2BERT_SET_BURST_ClearSD ()
 
V2.0.0.0
B�汾B2�豸����
****************************************************************************/

#ifndef _B2BERT_H_
#define _B2BERT_H_

#include "ch341a_dll.h"

//B2BERT�Ĵ������� 
#pragma pack (1) /* set maximum alignment to 1 ��һ���ֽڶ���*/  

struct sB2BERT_GLOBAL_Rst
{
INT8U bit0:1;				//Ϊ1��λ���Ĵ������ȫ����·��Ϊ0����������ͬʱҪ��λNorm��Burst���ֵĸ�λ�Ĵ���
INT8U reserved:7;			//reserved
};

struct sB2BERT_GLOBAL_REG06
{
INT8U Sel_Rx_ONU:1;			//Norm Burst����ѡ��Ϊ0ѡ���0��ONU�Ľ��գ�Ϊ1ѡ���1��ONU�Ľ���
INT8U isGPONorEPON:1;		//Ϊ1��ʾ����EPON,���������ʽ�Ϊ1.25Ghz��Ϊ0��ʾ����GPON������Ϊ1.244Ghz������Ϊ2.488Ghz
INT8U reserved:6;			//reserved
};

struct sB2BERT_GLOBAL_REG07
{
INT8U Period_Trig_Sel:1;	//���ͷ�ʽѡ��Ϊ0��ʾ���ڷ��ͣ�Ϊ1��ʾ���η��ͣ��ɱ��Ĵ�����bit1����
INT8U Period_Trig:1;		//д��bitΪ0--1--0�ʹ���һ��Burst���ͣ�ֻ�е����Ĵ�����bit0Ϊ1ʱ����Ч
INT8U reserved:6;			//reserved
};

struct sB2BERT_GLOBAL_REG08 //1��ʵ�ʷ�������Ҫ��1��2��ʱ���Է���Ƶ�ʵ�8��ƵΪ1�����ڡ�3���������ڱ����������Burst Bert�ĸ��������Ĵ���֮��
{
INT8U Burst_Period7_0;		//20λBurst Bert�ķ������ڼĴ����ĵ�8λ
INT8U Burst_Period15_8;		//20λBurst Bert�ķ������ڼĴ�������8λ
INT8U Burst_Period19_16:4;	//20λBurst Bert�ķ������ڼĴ����ĸ�4λ
INT8U reserved:4;			//reserved 
};

struct sB2BERT_GLOBAL_OLT_Rst  
{
INT8U OLT_Rst_Level:1;		//OLT�ĸ�λ�ź���Ч��ƽѡ��Ϊ0��ʾ�͵�ƽ��λOLT��Ϊ1��ʾ�ߵ�ƽ��λOLT 
INT8U OLT_Rst_Out:1;		//Ϊ1��ʾ�ر�OLT_Rst�������Ϊ0��ʾ��OLT_Rst�������ע��CPUд��OLT�ĸ�λ���ܴ�λ�Ŀ��� 
INT8U reserved:6;			//reserved 
};

struct sB2BERT_GLOBAL_REG0C
{
INT8U bit0:1;				//����Burst Bert���ֹ��õ�������ʱ���Ƶĸ�λ�źţ���0��1�����䴥����λ
INT8U reserved:6;			//reserved 
};				

struct sB2BERT_GLOBAL_REG0D
{
INT8U reserved0:1;
INT8U Burst_Ctrl_OK:1;		//Ϊ1��ʾBurst Bert�����нӿ���ʱ���Ʋ�����λ��ϣ�Ϊ0���ڸ�λ
INT8U Burst_Ctrl_Err:1;		//Ϊ1��ʾBurst Bert��������ʱ���Ʋ�����������Ϊ0����
INT8U reserved7_3:5;		//reserved 
};

struct sB2BERT_GLOBAL_REG10
{
INT8U Rst_Delay_Control:5;  //���Ƹ�λOLT��Rst�źŵ���ʱֵ��һ����Ϊ0x05��Ҫ����ʵ�������������
INT8U reserved7_5:3;		//reserved 
};

struct sB2BERT_GLOBAL_REG11
{
INT8U En_Delay_Control:5;  	//����Burst En��Trig��Gate��Period���������ʱֵ��һ����Ϊ0x0E
INT8U reserved7_5:3;		//reserved 
};

struct sB2BERT_GLOBAL_REG12
{
INT8U BTx_LOS:1;  			//Ϊ1��ʾADN2812��LOS�澯��Ϊ0��ʾ��������
INT8U BTx_LOL:1;  			//Ϊ1��ʾADN2812��LOL�澯��Ϊ0��ʾ��������
INT8U reserved7_2:6;		//reserved 
};

struct sB2BERT_GLOBAL_REG31
{
INT8U reserved3_0:4;		//reserved 
INT8U TxDis:1;				//Tx_DisableEn����ӦBit4
INT8U reserved7_5:3;		//reserved 
};

struct sB2BERT_GLOBAL_REG40
{
INT8U Trig_Sel:1;			//�����ź���Ч��ƽѡ��Ϊ0��ʾ�͵�ƽ��Ч��Ϊ1��ʾ�ߵ�ƽ��Ч
INT8U Trig_Off:1;			//Ϊ1��ʾ�رղ����źŵ������Ϊ1��ʾ�򿪵������Ϊ0��ʾ�ر�
INT8U reserved:6;			//reserved 
};

struct sB2BERT_GLOBAL_REG41 //1��ʵ��λ��Ҫ��1��2��ʱ���Է���Ƶ�ʵ�8��ƵΪ1��λ
{
INT8U Trig_Pos7_0;			//20λ����λ���źżĴ�����8λ
INT8U Trig_Pos15_8;			//20λ����λ���źżĴ�����8λ
INT8U Trig_Pos19_16:4;		//20λ����λ���źżĴ����ĸ�4λ
INT8U reserved:4;			//reserved 
};

struct sB2BERT_GLOBAL_REG44  //1��ʵ��λ��Ҫ��1��2��ʱ���Է���Ƶ�ʵ�8��ƵΪ1��λ
{
INT8U Trig_Width7_0;		//20λ��������źżĴ�����8λ
INT8U Trig_Width15_8;		//20λ��������źżĴ�����8λ
INT8U Trig_Width19_16:4;	//20λ��������źżĴ����ĸ�4λ
INT8U reserved:4;			//reserved 
};

struct sB2BERT_GLOBAL_REG63  
{
INT8U Burst_RST_Dly:1;		//����AD2855��FPGA֮��ӿ���ʱ��������д0--1--0ʱ������ʱֵ��Ϊ��ʼֵ
INT8U Burst_INC:1;			//����AD2855��FPGA֮��ӿ���ʱ������һ����Ϊ0
INT8U reserved7_2:6;		//reserved 
};

struct strB2BERT_GLOBAL		//����ȫ�ֿ��ƼĴ���
{
INT8U	FPGA_Ver; 									//[0x00h]
INT8U   FPGA_TYPE1;									//[0x01h]
INT8U   FPGA_TYPE2;									//[0x02h]
INT8U   FPGA_TYPE3;									//[0x03h]
INT8U   CPU_Haddr;									//[0x04h]
struct  sB2BERT_GLOBAL_Rst 	 		GLOBALRst;		//[0x05h]
struct	sB2BERT_GLOBAL_REG06 		REG06;			//[0x06h]
struct	sB2BERT_GLOBAL_REG07 		Trigger;		//[0x07h] 
struct  sB2BERT_GLOBAL_REG08 		Burst_Period;  	//[0x08h~0x0Ah], 3byte
struct	sB2BERT_GLOBAL_OLT_Rst 		OLTRst;			//[0x0Bh]
struct	sB2BERT_GLOBAL_REG0C 		BurstCtrlRst;	//[0x0Ch]
struct	sB2BERT_GLOBAL_REG0D 		BurstCtrlStatus;//[0x0Dh]
INT8U	Reserved14to15[2];							//[0x0Eh~0x0Fh], 2byte
struct	sB2BERT_GLOBAL_REG10		OLTRst_Dly;		//[0x10h] ���Ƹ�λOLT��Rst�źŵ���ʱֵ��һ����Ϊ0x05��Ҫ����ʵ�������������
struct	sB2BERT_GLOBAL_REG11		GLOBAL_Dly;		//[0x11h] ����Burst En��Trig��Gate��Period���������ʱֵ��һ����Ϊ0x0E 
struct	sB2BERT_GLOBAL_REG12		ADN2817Status;  //[0x12h] ADN2812״̬
INT8U	Reserved19to48[30];							//[0x13h~0x30h], 30byte 
struct 	sB2BERT_GLOBAL_REG31	    OLT_TxDis;		//[0x31h]
INT8U	Reserved50to63[14];							//[0x32h~0x3Fh], 14byte 
struct	sB2BERT_GLOBAL_REG40 		Trig_Ctrl;		//[0x40h]
struct	sB2BERT_GLOBAL_REG41 		Trig_Pos;		//[0x41h~0x43h], 3byte
struct	sB2BERT_GLOBAL_REG44 		Trig_Width;		//[0x44h~0x46h], 3byte
INT8U	Reserved71to95[25];							//[0x47h~0x5Fh], 25byte  
INT8U   REG60;										//[0x60h] ����Burst���ֵ�����ΪTx Train(�Ĵ���0x30Ϊ2ʱ)ΪTX_TRAIN_BITʱ���͵�Byte����Ҫ����Ӳ�����ԣ�һ�㲻��
INT8U	Reserved61;									//[0x61h]
INT8U   REG62; 										//[0x62h] AD2855��FPGA�Ľӿ�����ֱ�Ӷ���ֵ����Ҫ����Ӳ�����ԣ�һ�㲻��
struct  sB2BERT_GLOBAL_REG63		AD2855_Dly;		//[0x63h]
INT8U	Reserved100to127[28];						//[0x64h~0x7Fh], 28byte  
};

union uB2BERT_GLOBAL										  
{ 
struct strB2BERT_GLOBAL  sStrB2BERT_GLOBAL;
INT8U  pStrB2BERT_GLOBAL[128]; 
};

struct sB2BERT_NORM_Rst
{
INT8U bit0:1;			//Ϊ1��λ���Ĵ������ȫ��Norm Bert���ֵ�·��Ϊ0������������ȫ�ּĴ�����ResetΪ1ʱ��λΪ1
INT8U reserved:7;		//reserved
};

struct sB2BERT_NORM_REG06 //dynamic
{
INT8U Rx_LOS:1;			//Ϊ1��ʾLOS�澯��Ϊ0��ʾ��������̬�澯
INT8U RX_Lock_Err:1;	//Ϊ1��ʾ����PLL�澯��Ϊ0��ʾ��������̬�澯
INT8U Tx_FIFO_Err:1;	//Ϊ1��ʾ����FIFO�澯��Ϊ0��ʾ��������̬�澯
INT8U Tx_Lock_Err:1;  	//Ϊ1��ʾ����PLL�澯��Ϊ0��ʾ��������̬�澯
INT8U reserved:4;		//reserved
};

struct sB2BERT_NORM_REG07 //static
{
INT8U Rx_LOS:1;			//Ϊ1��ʾLOS�澯��Ϊ0��ʾ��������̬̬�澯        
INT8U RX_Lock_Err:1;	//Ϊ1��ʾ����PLL�澯��Ϊ0��ʾ��������̬�澯    
INT8U Tx_FIFO_Err:1;	//Ϊ1��ʾ����FIFO�澯��Ϊ0��ʾ��������̬�澯   
INT8U Tx_Lock_Err:1;	//Ϊ1��ʾ����PLL�澯��Ϊ0��ʾ��������̬�澯    
INT8U reserved:4;		//reserved
};

struct sB2BERT_NORM_Loop 
{
INT8U Rx2Tx_Loop:1;		//Ϊ1����Norm Bert��FPGA�ڲ��ӽ������뻷�ص����������Ϊ0��������       
INT8U Tx2Rx_Loop:1;		//Ϊ1����Norm Bert��FPGA�ڲ��ӷ���������ص��������룬Ϊ0��������  
INT8U VSC_Rx2Tx_Loop:1;	//Ϊ1����Norm Bert��VSC8145�ڲ��ӽ������뻷�ص����������Ϊ0�������� 
INT8U VSC_Tx2Rx_Loop:1;	//Ϊ1����Norm Bert��VSC8145�ڲ��ӷ���������ص��������룬Ϊ0��������   
INT8U reserved:4;		//reserved
};

struct sB2BERT_NORM_Align
{
INT8U Align_ReStr:1;	//��0��1����������SFI41�Ľӿڵ�����·
INT8U reserved:7;		//reserved
};

struct sB2BERT_NORM_AlignStatus
{
INT8U Align_OK:1;	//Ϊ1��ʾSFI41�ӿ�����ʱ����״̬��������ϣ�Ϊ0���ڵ���
INT8U Ctrl_OK:1;	//Ϊ1��ʾSFI41�ӿ�����ʱ���Ʋ�����λ��ϣ�Ϊ0���ڸ�λ
INT8U Ctrl_Err:1;	//Ϊ1��ʾSFI41�ӿ�����ʱ���Ʋ�����������Ϊ0����
INT8U reserved:5;	//reserved
};

struct sB2BERT_NORM_Pattern
{
INT8U isUserorPRBS:1;	//Ϊ1��ʾ��������Ϊ�û��Զ������У�Ϊ0��ʾ��m����
INT8U reserved:7;		//reserved
};

struct sB2BERT_NORM_UserLen  //��ѡ���û���������ʱ����Ч���512bit�����4��bit��ʵ�ʸ���Ҫ��1
{
INT8U UserLen7_0;		//�û��������г��ȵĵ�8λ
INT8U UserLen8:1;		//�û��������г��ȵ����λ
INT8U reserved:7;		//reserved  
};

struct sB2BERT_NORM_UserStatus  
{	 
INT8U User_Start:1;		//����������Ϊ�û��Զ�������ʱ��Ϊ1�������ԣ�Ϊ0��λ���ԡ�����������Ϊm����ʱ��Ϊ0
INT8U User_Rdy:1;		//����������Ϊ�û��Զ�������ʱ����дUser_StartΪ1��������ʱ��Ϊ1��ʾ���ͻ�û׼���ã�Ϊ0��ʾ׼����
INT8U reserved:6;		//reserved  
};

struct sB2BERT_NORM_REG17
{
INT8U InsertErrBit:1;	//д0 -- 1 -- 0�ڷ��������в���һ��bit���룬�˶������ܼĴ���0x19��Bert_InsErr_En��Ӱ��
INT8U reserved:7;		//reserved
};

struct sB2BERT_NORM_REG18
{
INT8U Out_Inv:1;		//Ϊ1���Ʒ���������࣬Ϊ0������
INT8U In_Inv:1;			//Ϊ1���ƽ������뷴�࣬Ϊ0������
INT8U reserved:6;		//reserved  
};

struct sB2BERT_NORM_REG19
{
INT8U InsErrEnbit0:1;	//Ϊ1��ʾ�����ڷ���������������в������룬Ϊ0����������
INT8U reserved:7;		//reserved
};

struct sB2BERT_NORM_BertInsErrTime	//�Է���Ƶ�ʵ�16��ƵΪ��λ(����1��bit����)��ʵ��ʱ��Ҫ��1
{
INT8U Time7_0;			//��������ʱ��ʱ���������8λ
INT8U Time15_8;	
INT8U Time23_16;
INT8U Time31_24;  		//��������ʱ��ʱ���������8λ
};

struct sB2BERT_NORM_Sync_Err_Max	//ÿ���жϼ����14784��bit�����������С�����õ���������ʱ����Ϊͬ������ΪPRBS��ʱ��һ������Ϊ0x5C6������Ϊ�Զ�������ʱ��һ����Ϊ0x004
{
INT8U Err_Max7_0;		//��ͬ��ʱ�ж�ͬ�������������8λ
INT8U Err_Max13_8:6;	//��ͬ��ʱ�ж�ͬ�������������6λ
INT8U reserved:2;		//reserved  
};

struct sB2BERT_NORM_Sync_Status
{
INT8U Status:1;			//Ϊ1��ʾ���մ���ͬ��״̬��Ϊ0��ʾ��������״̬
INT8U reserved:7;		//reserved
};

struct sB2BERT_NORM_Sync_ReSync
{
INT8U ReSync:1;			//��0��1����������������������ͬ��
INT8U reserved:7;		//reserved
};

struct sB2BERT_NORM_ErrTime_Cnt //��дErrTime7_0�Ĵ�������ֵ����������ǰ���Ե������ۼӸ����Ͳ���ʱ�䣬ͬʱ�����´β��� �����ζ�д������ܳ���27s��
{
INT8U ErrTime7_0;		//������Ե�ʱ�����8λ���Է���Ƶ�ʵ�16��ƵΪ��λ��ʵ������Ҫ��1��������32λ���ʱ��ʱ������ֵΪȫ1��
INT8U ErrTime15_8;		
INT8U ErrTime23_16;		
INT8U ErrTime31_24;		//������Ե�ʱ������8λ
};

struct sB2BERT_NORM_ErrNum_Cnt 
{
INT8U ErrNum7_0;		//������Ե������ۼӸ��������8λ��������40λ������ʱ������ֵΪȫ1��
INT8U ErrNum15_8;		
INT8U ErrNum23_16;		
INT8U ErrNum31_24;		//������Ե������ۼӸ��������8λ
};

struct strB2BERT_NORM	//����NormBERT�Ĵ���
{
INT8U	FirstByte; 									//[0x00h] 
INT8U	Reserved1to3[3];							//[0x01h~0x03h], 3byte 
INT8U   CPU_Haddr;									//[0x04h] 
struct  sB2BERT_NORM_Rst 			NormRst;		//[0x05h]
struct	sB2BERT_NORM_REG06 			Status_Dynamic;	//[0x06h]
struct	sB2BERT_NORM_REG07 			Status_Static;	//[0x07h]
struct 	sB2BERT_NORM_Loop   		Loop;			//[0x08h]
struct 	sB2BERT_NORM_Align  		Align;			//[0x09h]
struct 	sB2BERT_NORM_AlignStatus 	AlignStatus;	//[0x0Ah] 
INT8U	Reserved11to15[5];							//[0x0Bh~0x0Fh], 5byte 
struct	sB2BERT_NORM_Pattern 		Pattern;		//[0x10h]
INT8U 	PRBS;										//[0x11h]
struct 	sB2BERT_NORM_UserLen		UserLen;		//[0x12h~0x13h], 2byte 
struct 	sB2BERT_NORM_UserStatus 	UserStatus;		//[0x14h]
INT8U	Reserved21to22[2];							//[0x15h~0x16h], 2byte  
struct	sB2BERT_NORM_REG17			REG17;			//[0x17h]
struct	sB2BERT_NORM_REG18			INV;			//[0x18h] 
struct	sB2BERT_NORM_REG19			InsErrEn;		//[0x19h] 
INT8U	Reserved26to27[2];							//[0x1Ah~0x1Bh], 2byte  
struct	sB2BERT_NORM_BertInsErrTime InsErrTime; 	//[0x1Ch~0x1Fh], 4byte 
struct	sB2BERT_NORM_Sync_Err_Max	SyncErrMax;   	//[0x20h~0x21h], 2byte
struct	sB2BERT_NORM_Sync_Status	SyncStatus;		//[0x22h] 
struct	sB2BERT_NORM_Sync_ReSync	SyncReStar;		//[0x23h]
struct	sB2BERT_NORM_ErrTime_Cnt	ErrTime;		//[0x24h~0x27h], 4byte 
struct	sB2BERT_NORM_ErrNum_Cnt		ErrNum;			//[0x28h~0x2Bh], 4byte  
INT8U	Reserved44to63[20];							//[0x2Ch~0x3Fh], 20byte 
INT8U	UserPattern[64];							//[0x40h~0x7Fh], 64byte  ����û��Զ������еĴ洢�����64��byte(512bit)�����4��bit�����ݷ��ʹӵ�λ��ַ��ʼ��ÿһ��Byte�ǴӸ�λD7��ʼ��D0��˳����
};

union uB2BERT_NORM										  
{ 
struct strB2BERT_NORM  sStrB2BERT_NORM;
INT8U  pStrB2BERT_NORM[128]; 
};

struct sB2BERT_BURST_Rst
{
INT8U bit0:1;			//Ϊ1��λ���Ĵ��������Ӧ���ֵ�Burst Bert���ֵ�·��Ϊ0������������ȫ�ּĴ�����ResetΪ1ʱ��λΪ1
INT8U reserved:7;		//reserved
};

struct sB2BERT_BURST_OLTRst
{
INT8U bit0:1;			//Ϊ1��ʾ��λOLT��ģ�飬Ϊ0������������λ�����Ӧ��OLT Pin�Ĵ���λ�Ĺ��ܡ�����������ʱӦ�ô�λΪ0������Burst���ֵĸ�λ�ǻ�ع�ϵ��������һ����λ�Ĵ���Ϊ1���ͻḴλOLT����λ����ȫ�ֿ��ƼĴ���0xb��bit1λӰ�졣
INT8U reserved:7;		//reserved
};

struct sB2BERT_BURST_REG08
{
INT8U Framer_Not:1;		//Ϊ1��ʾBurstʱ�����й�û���ҵ�֡ͷ�������Ϊ0�����������Զ����
INT8U reserved7_1:7;	//reserved
};

struct sB2BERT_BURST_Pattern
{
INT8U isUserorPRBS:1;	//Ϊ1��ʾ��ӦBurst Bert�Ĳ�������Ϊ�û��Զ������У�Ϊ0��ʾ��m����
INT8U reserved:7;		//reserved
};

struct sB2BERT_BURST_UserLen  //��ѡ���û���������ʱ����Ч���4096bit�����4��bit��ʵ�ʸ���Ҫ��1
{
INT8U UserLen7_0;		//�û��������г��ȵĵ�8λ
INT8U UserLen11_8:4;	//�û��������г��ȵĸ�4λ
INT8U reserved:4;		//reserved  
};

struct sB2BERT_BURST_UserStatus  
{	 
INT8U User_Start:1;		//����������Ϊ�û��Զ�������ʱ��Ϊ1�������ԣ�Ϊ0��λ���ԡ�����������Ϊm����ʱ��Ϊ0
INT8U User_Rdy:1;		//����������Ϊ�û��Զ�������ʱ����дUser_StartΪ1��������ʱ��Ϊ1��ʾ���ͻ�û׼���ã�Ϊ0��ʾ׼����
INT8U reserved:6;		//reserved  
};

struct sB2BERT_BURST_REG17
{
INT8U InsertErrBit:1;	//д0 -- 1 -- 0�ڷ��������в���һ��bit���룬�˶������ܼĴ���0x19��Bert_InsErr_En��Ӱ��
INT8U reserved:7;		//reserved
};

struct sB2BERT_BURST_REG18
{
INT8U Out_Inv:1;		//Ϊ1���Ʒ���������࣬Ϊ0������
INT8U In_Inv:1;			//Ϊ1���ƽ������뷴�࣬Ϊ0������
INT8U reserved:6;		//reserved  
};

struct sB2BERT_BURST_InsErrEn
{
INT8U InsErrEnbit0:1;	//Ϊ1��ʾ�����ڷ���������������в������룬Ϊ0����������
INT8U reserved:7;		//reserved
};

struct sB2BERT_BURST_InsErrTime	//�Է���Ƶ�ʵ�16��ƵΪ��λ(����1��bit����)��ʵ��ʱ��Ҫ��1
{
INT8U Time7_0;			//��������ʱ��ʱ���������8λ
INT8U Time15_8;	
INT8U Time23_16;
INT8U Time31_24;  		//��������ʱ��ʱ���������8λ
};

struct sB2BERT_BURST_Sync_Err_Max	//ÿ���жϼ����14784��bit�����������С�����õ���������ʱ����Ϊͬ������ΪPRBS��ʱ��һ������Ϊ0x5C6������Ϊ�Զ�������ʱ��һ����Ϊ0x004
{
INT8U Err_Max7_0;		//��ͬ��ʱ�ж�ͬ�������������8λ
INT8U Err_Max13_8:6;	//��ͬ��ʱ�ж�ͬ�������������6λ
INT8U reserved:2;		//reserved  
};

struct sB2BERT_BURST_Sync_Status
{
INT8U Status:1;			//Ϊ1��ʾ���մ���ͬ��״̬��Ϊ0��ʾ��������״̬
INT8U reserved:7;		//reserved
};

struct sB2BERT_BURST_Sync_ReSync
{
INT8U ReSync:1;			//��0��1����������������������ͬ��
INT8U reserved:7;		//reserved
};

struct sB2BERT_BURST_ErrTime_Cnt //��дErrTime7_0�Ĵ�������ֵ����������ǰ���Ե������ۼӸ����Ͳ���ʱ�䣬ͬʱ�����´β��� �����ζ�д������ܳ���27s��
{
INT8U ErrTime7_0;		//������Ե�ʱ�����8λ���Է���Ƶ�ʵ�16��ƵΪ��λ��ʵ������Ҫ��1��������32λ���ʱ��ʱ������ֵΪȫ1��
INT8U ErrTime15_8;		
INT8U ErrTime23_16;		
INT8U ErrTime31_24;		//������Ե�ʱ������8λ
};

struct sB2BERT_BURST_ErrNum_Cnt 
{
INT8U ErrNum7_0;		//������Ե������ۼӸ��������8λ��������40λ������ʱ������ֵΪȫ1��
INT8U ErrNum15_8;		
INT8U ErrNum23_16;		
INT8U ErrNum31_24;		//������Ե������ۼӸ��������8λ
};

struct sB2BERT_BURST_Preamble_Len 	//ǰ������λ���ȣ���λΪ�ֽ�(8������ʱ�ӿ��)��ʵ���ֽڸ���Ҫ��1��һ��Ϊ5��6���ֽ�
{
INT8U Len7_0;			//ǰ������λ���ȵĵ�8λ
INT8U Len11_8:4;		//ǰ������λ���ȵĸ�4λ
INT8U reserved:4;		//reserved  
};

struct sB2BERT_BURST_Data_Len 		//BURST�������ݿ�ȣ�1����λΪ�ֽ�(8������ʱ�ӿ��)��2��ʵ�ʷ����ֽ���Ҫ��1��
{
INT8U Len7_0;			//Burst Bert�������ݿ�ȵĵ�8λ
INT8U Len15_8;			//Burst Bert�������ݿ�ȵ���8λ  
INT8U Len18_16:3;		//Burst Bert�������ݿ�ȵĸ�3λ
INT8U reserved:5;		//reserved  
};

struct sB2BERT_BURST_TxDly_Len  	//Burst Bert��ͻ���������ݵ���ʱ��ȣ�1����λΪ�ֽ�(8������ʱ�ӿ��)��2��ʵ�ʷ����ֽ���Ҫ��1��
{
INT8U Len7_0;			//Burst Bert��ͻ���������ݵ���ʱ��ȼĴ����ĵ�8λ
INT8U Len15_8;			//Burst Bert��ͻ���������ݵ���ʱ��ȼĴ�������8λ  
INT8U Len18_16:3;		//Burst Bert��ͻ���������ݵ���ʱ��ȼĴ����ĸ�3λ
INT8U reserved:5;		//reserved  
};

struct sB2BERT_BURST_RxDly_Len  	//���Գ��ķ�����·��ʱ��1����λΪ�ֽ�(8������ʱ�ӿ��)��2��ʵ�ʷ����ֽ���Ҫ��1��
{
INT8U Len7_0;			//���Գ��ķ�����·��ʱ�ĵ�8λ
INT8U Len15_8;			//���Գ��ķ�����·��ʱ�ĸ�8λ 
};

struct sB2BERT_BURST_Train_Status  	
{
INT8U Bit_OK:1;			//Ϊ1��ʾ���յ���Bit��λ��ϣ�Ϊ0��ʾ���ڵ�������Burst_Train_StateΪ2ʱ����Ч
INT8U Dly_OK:1;			//Ϊ1��ʾ������ʱ��ϣ�Ϊ0��ʾ���ڲ��ԡ���Burst_Train_StateΪ3ʱ����Ч
INT8U reserved:6;		//reserved  
};

struct sB2BERT_BURST_OLTRst_Pos_Sel  	
{
INT8U bit0:1;			//Ϊ1��ʾ��ӦONU��OLT�ĸ�λ�ź����Զ���λ�ò����Ŀɱ�̿�ȵĸ�λ�źŴ��棬Ϊ0��ʾ���ڲ���·�Զ�������Ϊ��������Ƶ�ʵ�8��Ƶʱ�����ڵĸ�λ�ź�
INT8U reserved:7;		//reserved  
};

struct sB2BERT_BURST_REG41
{
INT8U Pos7_0;			//12λ��ӦONU��OLT��λ���Զ���λ���źżĴ����ĵ�8λ
INT8U Pos11_8:4;		//12λ��ӦONU��OLT��λ���Զ���λ���źżĴ����ĸ�4λ
INT8U reserved:4;		//reserved  
};

struct sB2BERT_BURST_SDChk_Width
{
INT8U width7_0;			//19λ���SDD��ȼĴ����ĵ�8λ
INT8U width15_8;			//20λ��ӦONU��OLT��λ���Զ���λ���źżĴ�������8λ  
INT8U width18_16:3;		//20λ��ӦONU��OLT��λ���Զ���λ���źżĴ����ĸ�4λ
INT8U reserved:5;		//reserved  
};

struct sB2BERT_BURST_SDChk_Status  	//��д�˼Ĵ�������ֵ�������SDA��SDD�ļ���� 
{									
INT8U SDD:1;			//Ϊ1��ʾ�ڶ�Ӧ��OLT_Rst�����غ���ʱ(SDA_ChkPos + 1)*8������ʱ�����ں���(SDDChk_Width + 1)*8������ʱ��������Ϊ���ó���SDΪ1�ĵط�������SDΪ1��Ϊ0��ʾ����
INT8U SDA:1;			//Ϊ1��ʾ�ڶ�Ӧ��OLT_Rst�����غ���ʱ(SDA_ChkPos + 1)*8������ʱ�����ں���(SDDChk_Width + 1)*8������ʱ��������Ϊ���ó���SDΪ0�ĵط�������SDΪ0��Ϊ0��ʾ����
INT8U reserved:6;		//reserved  
};

struct sB2BERT_BURST_UserPattern  
{									
INT8U Table:2;			//����û��Զ������еĴ洢���ĸ���λ��չ��ַ
INT8U reserved:6;		//reserved
INT8U Data[128]; //����û��Զ������еĴ洢���Ķ�д���ڣ�ÿ������Ϊ128byte���û��Զ��������512��byte(4096bit)�����4��bit�������źŹ��� Table0 byte0 D7......Table3 byte127 D0
};
	
struct strB2BERT_BURST		//����BurstBERT�Ĵ���
{
INT8U	FirstByte; 									//[0x00h] 
INT8U	Reserved1to3[3];							//[0x01h~0x03h], 3byte 
INT8U   CPU_Haddr;									//[0x04h] 
struct  sB2BERT_BURST_Rst 			BurstRst;		//[0x05h] 
struct  sB2BERT_BURST_OLTRst 		OLTRst;			//[0x06h]  	
INT8U	Reserved7;									//[0x07h]
struct  sB2BERT_BURST_REG08 		Framer;			//[0x08h]   
INT8U	Reserved9to15[7];							//[0x09h~0x0Fh], 7byte  
struct	sB2BERT_BURST_Pattern 		Pattern;		//[0x10h]
INT8U 	PRBS;										//[0x11h]
struct 	sB2BERT_BURST_UserLen		UserLen;		//[0x12h~0x13h], 2byte 
struct 	sB2BERT_BURST_UserStatus 	UserStatus;		//[0x14h] 
INT8U	Reserved21to22[2];							//[0x15h~0x16h], 2byte      
struct	sB2BERT_BURST_REG17			REG17;			//[0x17h]
struct	sB2BERT_BURST_REG18			INV;			//[0x18h] 
struct	sB2BERT_BURST_InsErrEn		InsErrEn;		//[0x19h] 
INT8U	Reserved26to27[2];							//[0x1Ah~0x1Bh], 2byte  
struct	sB2BERT_BURST_InsErrTime 	InsErrTime;		//[0x1Ch~0x1Fh], 4byte 
struct	sB2BERT_BURST_Sync_Err_Max	SyncErrMax;   	//[0x20h~0x21h], 2byte
struct	sB2BERT_BURST_Sync_Status	SyncStatus;		//[0x22h] 
struct	sB2BERT_BURST_Sync_ReSync	SyncReStar;		//[0x23h]
struct	sB2BERT_BURST_ErrTime_Cnt	ErrTime;		//[0x24h~0x27h], 4byte 
struct	sB2BERT_BURST_ErrNum_Cnt	ErrNum;			//[0x28h~0x2Bh], 4byte  
INT8U	Reserved44to47[4];							//[0x2Ch~0x2Fh], 4byte  
INT8U   Burst_State;								//[0x30h] 
INT8U   PreGuideLen;								//[0x31h] ǰ������λ���ȣ���λΪ�ֽ�(8������ʱ�ӿ��)��ʵ���ֽڸ���Ҫ��1��һ��Ϊ4���ֽڡ�
struct 	sB2BERT_BURST_Preamble_Len	PreambleLen;	//[0x32h~0x33h], 2byte  
struct 	sB2BERT_BURST_Data_Len		DataLen;		//[0x34h~0x36h], 3byte 
INT8U	Reserved55;									//[0x37h] 
struct 	sB2BERT_BURST_TxDly_Len		TxDelayLen;		//[0x38h~0x3Ah], 3byte 
INT8U	Reserved59;									//[0x3Bh] 
struct 	sB2BERT_BURST_RxDly_Len		RxDelayLen;		//[0x3Ch~0x3Dh], 2byte  
struct	sB2BERT_BURST_Train_Status	Train_Status;	//[0x3Eh]  
INT8U	Reserved63to64[2];							//[0x3Fh~0x40h], 2byte  
struct 	sB2BERT_BURST_REG41			OLTRst_Pos;		//[0x41h~0x42h], 2byte 
INT8U	Reserved67;									//[0x43h]
INT8U   OLTRst_Width;								//[0x44h] 
INT8U	Reserved69to71[3];							//[0x45h~0x47h], 3byte 
INT8U   SD_Level;									//[0x48h] OLT��SD�ź���Ч��ƽѡ��Ϊ0��ʾ�͵�ƽ��Ч��Ϊ1��ʾ�ߵ�ƽ��Ч  
INT8U   SDChk_Pos;									//[0x49h] 8λ���SDλ�ã���Ӧ��·OLT_Rst�½��ء�ע�⣺1��ʵ��λ��Ҫ��1��2��ʱ���Է���Ƶ�ʵ�8��ƵΪ1��λ��3��λ��������ڱ�·OLT��λ�źŵ�
struct 	sB2BERT_BURST_SDChk_Width	SDChk_Width;	//[0x4Ah~0x4Ch], 3byte 
struct	sB2BERT_BURST_SDChk_Status 	SDChk_Status;	//[0x4Dh] 
INT8U	Reserved78to126[49];						//[0x4Eh~0x7Eh], 49byte  
struct 	sB2BERT_BURST_UserPattern	UserPattern;	//[0x7Fh, 0x80h~0xFFh], 1byte + 128*4byte
};
										
union uB2BERT_BURST										  
{ 
struct strB2BERT_BURST  sStrB2BERT_BURST;
INT8U  pStrB2BERT_BURST[256]; 
};

struct strB2BERT
{                       
INT8U	FirstByte;				//[0], 1byte
INT8U	FirmwareVersionSuffix;	//[1], 1byte, 
INT8U	FirmwareVersion;		//[2], 1byte 										
INT8U	CPLDVersion;			//[3], 1byte,
INT16U	Core_Voltage;			//[4~5], 2byte 							
INT16S	Core_Temperature;		//[6~7], 2byte
INT8U   reserved8to9[2];		//[8~9], 2byte                                                  
INT16U  ADC0_V33_MON;			//[10~11], 2byte��
INT16U  ADC1_VRX_MON;			//[12~13], 2byte��
INT16U  ADC2_V25_MON;			//[14~15], 2byte��
INT16U  ADC3_V10_MON;			//[16~17], 2byte��
INT16U  ADC4_LD_MON;			//[18~19], 2byte��TOSA-LDǰ���ѹ
INT16U  DAC0_SET_VPD;			//[20~21], 2byte��3482��3571��boost-up��ѹ����
INT16U  DAC1_SET_VTR;			//[22~23], 2byte��VTR��ѹ����
INT16U  ADC14_PD_MON;			//[24~25], 2byte��PD�ܵ�����ѹ
INT16U  ADC15_MON;				//[26~27], 2byte��δ��
INT16U  ADC_V33_MON;			//[28~29], 2byte
INT16U  ADC_VTR_MON;			//[30~31], 2byte
INT16U  ADC_V25_MON;			//[32~33], 2byte
INT16U  ADC_V10_MON;			//[34~35], 2byte
INT16U  ADC_VPDM_MON;			//[36~37], 2byte
INT16U  ADC_VTRx_MON;			//[38~39], 2byte
INT16U  ADC_VBUS_MON;			//[40~41], 2byte��ʵ����+5V����
INT16U  ADC_LD_MON;				//[42~43], 2byte
INT16U  ADC_PD_MON;				//[44~45], 2byte
INT16U  ADC0_VPDM_MON;			//[46~47], 2byte
INT16U  DAC_ADN8810_SET;	    //[48~49], 2byte
INT16U  ADC15_SC_MON;			//[50~51], 2byte
INT16U  ADC_SC_MON;				//[52~53], 2byte
INT8U   reserved54;				//[54],    1byte
INT8U   Vapd_3571LT3482_sel;	//[55], 1byte; =0, 3571, Vapd������DAC
INT8U   reserved56to73[18];		//[56~73], 18byte
INT8U	BEN_mode;				//[74],    1byte 
INT8U   RSSI_mode;				//[75],    1byte 
INT16U  Tune_VPD_MON;			//[76~77], 2byte
INT8U   VapdAdj_loop;			//[78],    1byte 
INT8U   VapdAdj_en;				//[79],    1byte
float	Vapd_expect;			//[80~83], 4byte 
INT16U  Tune_IPD_MON;			//[84~85], 2byte
INT8U   IapdAdj_loop;			//[86],    1byte 
INT8U   IapdAdj_en;				//[87],    1byte
float	Iapd_expectMIN;			//[88~91], 4byte 
float	Iapd_expectMAX;			//[92~95], 4byte 
INT16U	Vapd20V_loop;			//[96~97], 2byte 
INT16U	Vapd20V_DAC;			//[98~99], 2byte 
INT8U   Vapd20V_en;				//[100],   1byte
INT8U	Vbr_3V4V_sel;			//[101],   1byte, =0 Vbr-3V; =1 Vbr-4V;
INT8U	Vbr_3V_en;				//[102],   1byte
INT8U	Vbr_3V_status;			//[103],   1byte
INT16U	ADC_Vbr_MON;			//[104~105],2byte
INT8U   Vapd_30V_en;			//[106],   1byte
INT8U   Vapd_30V_status;		//[107],   1byte
INT16U	ADC_V30V_MON;			//[108~109],2byte
INT16U	myPWMDAT0;				//[110~111],2byte
INT16U  Tune_VTR_MON;			//[112~113],2byte
INT8U   VTRAdj_loop;			//[114],    1byte 
INT8U   VTRAdj_en;				//[115],    1byte
float	VTR_expect;				//[116~119],4byte 
INT8U   delay1;					//[120],    1byte
INT8U   delay2;					//[121],    1byte
INT8U   delay3;					//[122],    1byte
INT32U  Password;  				//[123~126]   										
INT8U   LUTIndex;				//[127] 
INT8U	I2CSlaveAdd2;			//[128], 1byte
INT8U	I2CSlaveAdd0;			//[129], 1byte
INT8U	I2CSlaveAdd1;			//[130], 1byte
INT8U	CheckSumPNSN;			//[131], 1byte
INT8U   vendorPN[16];			//[132~147], 16byte
INT8U   vendorSN[16];			//[148~163], 16byte
INT8U	reserved164to227[64];	//[188~227], 40byte           
INT16U	RSSI_PW;				//[228~229], 2byte 
INT8U	RSSI_polarity;			//[230], 1byte 
INT8U	Reserved231;			//[231], 1byte 
float	Vapd_expectMIN;			//[232~235], 4byte 
float	Vapd_expectMAX;			//[236~239], 4byte 
float	Vapd_DeltaMAX;			//[240~243], 4byte,
INT16U	Vapd20V_loop_ini;		//[244~245], 2byte 
INT16U	Vapd20V_DAC_ini;		//[246~247], 2byte 
INT8U   Vapd20V_en_ini;			//[248], 1byte
INT8U	BEN_polarity;			//[249], 1byte 
INT16U	BEN_PW;					//[250~251], 2byte 
INT8U	Reserved252to255[4];	//[252~255], 4byte 
};

union uB2BERT										  
{ struct strB2BERT  sStrB2BERT;
  INT8U  pStrB2BERT[256];
};

#pragma pack ()  /*ȡ���Զ����ֽڶ��뷽ʽ*/

//BERT pattern
enum eB2BERT_PATTERN   //PRBS7��PRBS9��PRBS11��PRBS15��PRBS20��PRBS21��PRBS23��PRBS29��PRBS31
{   
	B2BERT_PATT_PRBS,
	B2BERT_PATT_USER
};

//BERT PRBS
enum eB2BERT_PRBS   //PRBS7��PRBS9��PRBS11��PRBS15��PRBS20��PRBS21��PRBS23��PRBS29��PRBS31
{   
	B2BERT_PATT_PRBS7=7,
	B2BERT_PATT_PRBS9=9,
	B2BERT_PATT_PRBS11=11,
	B2BERT_PATT_PRBS15=15,
	B2BERT_PATT_PRBS20=20,
	B2BERT_PATT_PRBS21=21,
	B2BERT_PATT_PRBS23=23,
	B2BERT_PATT_PRBS29=29,
	B2BERT_PATT_PRBS31=31
};	

//BERT rate
enum eB2BERT_RATE
{   
	B2BERT_RATE_GPON,
	B2BERT_RATE_EPON
};	

//BERT Norm����ģʽ��ѡ����յ�ONU
enum eB2BERT_NORM_ONU
{   
	B2BERT_NORM_OLTtoONU0,
	B2BERT_NORM_OLTtoONU1
};	

//BERT Norm����ģʽ��ѡ���Ƿ���Ҫȫ�ָ�λ
enum eB2BERT_NORM_GLOBALRst
{   
	B2BERT_NORM_neverGLOBALRst,
	B2BERT_NORM_needGLOBALRst
};	

//BERT mode
enum eB2BERT_MODE
{   
	B2BERT_MODE_OLTtoONU0,
	B2BERT_MODE_OLTtoONU1,
	B2BERT_MODE_ONU0toOLT,
	B2BERT_MODE_ONU1toOLT,
	B2BERT_MODE_ONU0andONU1toOLT,
	B2BERT_MODE_ONU0andONU1toOLTtoONU0,
	B2BERT_MODE_ONU0andONU1toOLTtoONU1,
	B2BERT_MODE_ONU0toOLTtoONU0,
	B2BERT_MODE_ONU1toOLTtoONU1
};	

//BERT Burst Trigger
enum eB2BERT_BURST_TRIGGER
{   
	B2BERT_BURST_TRIG_CYCLE,
	B2BERT_BURST_TRIG_PULSE
};

//BERT Burst Trigger out
enum eB2BERT_TRIGGER_OUT
{   
	B2BERT_TRIG_OUT_CLOSE,
	B2BERT_TRIG_OUT_OPEN
};

//BERT Burst Trigger level
enum eB2BERT_TRIGGER_LEVEL
{   
	B2BERT_TRIG_LEVEL_LOW,
	B2BERT_TRIG_LEVEL_HIGH
};

//BERT OLT Rst Out
enum eB2BERT_OLTRst_OUT
{   
	B2BERT_OLTRst_OUT_OPEN,
	B2BERT_OLTRst_OUT_CLOSE
};

//BERT OLT Rst level
enum eB2BERT_OLTRst_LEVEL
{   
	B2BERT_OLTRst_LEVEL_LOW,
	B2BERT_OLTRst_LEVEL_HIGH
};

//BERT OLT Rst Pos mode
enum eB2BERT_OLTRst_POS_MODE
{   
	B2BERT_OLTRst_POS_AUTO,
	B2BERT_OLTRst_POS_USER
} eB2BERT_OLTRst_POS_MODE;

//BERT SD Level
enum eB2BERT_SD_LEVEL
{   
	B2BERT_SD_LOW,
	B2BERT_SD_HIGH
};

//extern union uB2BERT_GLOBAL B2BERT_GLOBAL;
//extern union uB2BERT_NORM 	B2BERT_NORM; 
//extern union uB2BERT_BURST 	B2BERT_BURST; 
//extern union uB2BERT 		B2BERT; 

int	B2BERT_Addr;  

#define B2_USERPATT_MAX_BYTENUMBER   512 
#define B2_USERPATT_MAX_ROWNUMBER    32 
#define B2_USERPATT_MAX_PATTFILESIZE 1024

char B2BERT_PATTFILE[MAX_PATHNAME_LEN];
unsigned char B2BERT_PATT[B2_USERPATT_MAX_BYTENUMBER]; 

extern int B2BERT_Init (int *USB_Handle, char *SN); 				//����sn��ȡ�豸�˿ں�
extern int B2BERT_SET_CPU_Haddr (int USB_Handle, int Haddr);		//�����λ��ַ��= 0ѡ��ȫ�ּĴ������֣�= 1ѡ��Norm�Ĵ������֣�= 2ѡ��Burst1�Ĵ������֣�= 2ѡ��Burst2�Ĵ�������
extern int B2BERT_SET_RATE (int USB_Handle, int rate);				//�������ʣ� 0=GPON�� 1=EPON
extern int B2BERT_SET_Sel_Rx_ONU (int USB_Handle, int sel_rx_onu);  //ѡ��Norm����״̬�µ�ONU����
extern int B2BERT_SET_GLOBAL_Rst (int USB_Handle, int Rst); 		//����ȫ�ָ�λ�Ĵ���
extern int B2BERT_SET_GLOBAL_OLT_TxDis (int USB_Handle, int TxDisEn); 
extern int B2BERT_SET_GLOBAL_PeriodTrigSel (int USB_Handle, int TrigSel);//Burst Bert�ķ��ͷ�ʽѡ��Ϊ0��ʾ���ڷ��ͣ�Ϊ1��ʾ���η��ͣ��ɱ��Ĵ�����bit1����
extern int B2BERT_SET_GLOBAL_PeriodTrig (int USB_Handle, int Trig); 	//��bitΪ0--1--0�ʹ���һ��Burst���ͣ�ֻ�е����Ĵ�����bit0Ϊ1ʱ����Ч
extern int B2BERT_SET_GLOBAL_BurstPeriod (int USB_Handle, unsigned int BurstPeriod);
extern int B2BERT_SET_GLOBAL_TrigOut (int USB_Handle, int TrigOut);		//Ϊ0��ʾ�رղ����źŵ������Ϊ1��ʾ�򿪵���� 
extern int B2BERT_SET_GLOBAL_TrigLevel (int USB_Handle, int TrigLevel);	//�����ź���Ч��ƽѡ��Ϊ0��ʾ�͵�ƽ��Ч��Ϊ1��ʾ�ߵ�ƽ��Ч
extern int B2BERT_SET_GLOBAL_TrigPos (int USB_Handle, unsigned int TrigPos);
extern int B2BERT_SET_GLOBAL_TrigWidth (int USB_Handle, unsigned int TrigWidth);
extern int B2BERT_SET_GLOBAL_OLTRst_Out (int USB_Handle, int OLTRstOut);//Ϊ1��ʾ�ر�OLTRst�źŵ������Ϊ0��ʾ�򿪵����
extern int B2BERT_SET_GLOBAL_OLTRst_Level (int USB_Handle, int OLTRstLevel);//OLTRst�ź���Ч��ƽѡ��Ϊ0��ʾ�͵�ƽ��Ч��Ϊ1��ʾ�ߵ�ƽ��Ч
extern int B2BERT_GET_GLOBAL_AD2817_Status (int inst_B2, int *LOS, int *LOL);
extern int B2BERT_SET_GLOBAL_FPGA_Dly (int USB_Handle, int Rst);
extern int B2BERT_SET_GLOBAL_OLTRst_Dly (int USB_Handle, unsigned char Dly);
extern int B2BERT_SET_GLOBAL_GLOBAL_Dly (int USB_Handle, unsigned char Dly);

extern int B2BERT_SET_NORM_Rst (int USB_Handle, int Rst);
extern int B2BERT_SET_NORM_Loop (int USB_Handle, int Rx2Tx_Loop, int Tx2Rx_Loop, int VSC_Rx2Tx_Loop, int VSC_Tx2Rx_Loop); 
extern int B2BERT_SET_NORM_Pattern (int USB_Handle, int Pattern);		//��������Ϊ�Զ����m���У�0=m���У�1=�Զ�������  
extern int B2BERT_SET_NORM_PRBS (int USB_Handle, int PRBS);				//����PRBS�������� 
extern int B2BERT_SET_NORM_User (int USB_Handle, int UserStart);		//�����Զ�������״̬ 
extern int B2BERT_SET_NORM_INV (int USB_Handle, int InINV, int OutINV);	//��������������๦��  
extern int B2BERT_SET_NORM_InsErrEn (int USB_Handle, int InsErrEn);		//�����Ƿ��������
extern int B2BERT_SET_NORM_InsErrTime (int USB_Handle, unsigned int InsErrTime);		//���ò�������ʱ��
extern int B2BERT_SET_NORM_SyncErrMax (int USB_Handle, unsigned short int SyncErrMax);	//�����Ƿ�������� 
extern int B2BERT_SET_NORM_AlignStar (int USB_Handle, int Align);
extern int B2BERT_GET_NORM_AlignStatus (int USB_Handle, int *AlignOK, int *Ctrl_OK, int *Ctrl_Err); 
/*
AlignOK��Ϊ1��ʾSFI41�ӿ�����ʱ����״̬��������ϣ�Ϊ0���ڵ���
Ctrl_OK��Ϊ1��ʾSFI41�ӿ�����ʱ���Ʋ�����λ��ϣ�Ϊ0���ڸ�λ
Ctrl_Err��Ϊ1��ʾSFI41�ӿ�����ʱ���Ʋ�����������Ϊ0����
*/
extern int B2BERT_SET_NORM_SyncReStar (int USB_Handle, int SyncReStar);
extern int B2BERT_GET_NORM_SyncStatus (int USB_Handle, int *Sync);		//Ϊ1��ʾ���մ���ͬ��״̬��Ϊ0��ʾ��������״̬ 

extern int B2BERT_SET_BURST_Rst (int USB_Handle, int Rst);
//extern int B2BERT_SET_BURST_Loop (int USB_Handle, int Loop); 
extern int B2BERT_SET_BURST_Pattern (int USB_Handle, int Pattern);		//��������Ϊ�Զ����m���У�0=m���У�1=�Զ�������  
extern int B2BERT_SET_BURST_PRBS (int USB_Handle, int PRBS);			//����PRBS��������
extern int B2BERT_SET_BURST_UserStart (int USB_Handle, int UserStart);	//�����Զ�������״̬
extern int B2BERT_GET_BURST_UserState (int USB_Handle, int *UserState); //��ȡ�Զ������ͷ���״̬, Ϊ1��ʾ���ͻ�û׼���ã�Ϊ0��ʾ׼����
extern int B2BERT_SET_BURST_INV (int USB_Handle, int InINV, int OutINV);//��������������๦��
extern int B2BERT_SET_BURST_InsErrEn (int USB_Handle, int InsErrEn);	//�����Ƿ��������
extern int B2BERT_SET_BURST_InsErrTime (int USB_Handle, unsigned int InsErrTime);		//���ò�������ʱ��
extern int B2BERT_SET_BURST_SyncErrMax (int USB_Handle, unsigned short int SyncErrMax);	//�����Ƿ�������� 
extern int B2BERT_SET_BURST_State (int USB_Handle, int state);							//����Burst����״̬
/*
0���ر���Ӧ��ONU����
1������������ʱ���źţ����ڽ��յ���������ʱ
2�����������������ַ������ڽ��յ���Bit��λ
3����������������֡�źţ����ڽ��ղ�����·��ʱ
4������������m���л��Զ������ݡ��ڴ���ģʽ�¾���һ�������Bert��
5�����У��رշ��͡�
6���������൱�ڹرշ��� 
7�����������ڷ���ͻ����m���л��Զ������ݣ�������ʽ��Burst Bert���ԡ�
*/
extern int B2BERT_SET_BurstCtrlRst (int USB_Handle);		//��������Burst Bert���ֹ��õ�������ʱ���Ƶĸ�λ�źţ���0��1�����䴥����λ
extern int B2BERT_SET_GLOBAL_BurstCtrlRst (int USB_Handle, int Rst);
extern int B2BERT_GET_GLOBAL_BurstCtrlStatus (int USB_Handle, int *BurstCtrlOK, int *BurstCtrlErr);//BurstCtrlOKΪ1��ʾBurst Bert�����нӿ���ʱ���Ʋ�����λ��ϣ�Ϊ0���ڸ�λ;BurstCtrlErrΪ1��ʾBurst Bert��������ʱ���Ʋ�����������Ϊ0����
extern int B2BERT_SET_BURST_PreGuideLen (int USB_Handle, unsigned char PreGuideLen);		
extern int B2BERT_SET_BURST_PreambleLen (int USB_Handle, unsigned short PreambleLen);
extern int B2BERT_SET_BURST_DataLen (int USB_Handle, unsigned int DataLen);
extern int B2BERT_SET_BURST_TxDelayLen (int USB_Handle, unsigned int TxDelayLen);
//extern int B2BERT_SET_BURST_OLTRst_Pos_Sel (int USB_Handle, int OLTRst_Pos_Sel);
extern int B2BERT_SET_BURST_OLTRst_Pos (int USB_Handle, unsigned short OLTRst_Pos);
extern int B2BERT_SET_BURST_OLTRst_Width (int USB_Handle, unsigned char OLTRst_Width);
extern int B2BERT_SET_BURST_OLTRst (int USB_Handle, int Rst);
//extern int B2BERT_SET_BURST_AlignStar (int USB_Handle, int Align);
//extern int B2BERT_GET_BURST_AlignStatus (int USB_Handle, int *AlignOK); 
extern int B2BERT_GET_BURST_TrainStatus (int USB_Handle, int *Bit_OK, int *Dly_OK);	//Bit_OK:Ϊ1��ʾ���յ���Bit��λ��ϣ�Ϊ0��ʾ���ڵ�������Burst_Train_StateΪ2ʱ����Ч��Dly_OK:Ϊ1��ʾ������ʱ��ϣ�Ϊ0��ʾ���ڲ��ԡ���Burst_Train_StateΪ3ʱ����Ч
extern int B2BERT_SET_BURST_SyncReStar (int USB_Handle, int SyncReStar);
extern int B2BERT_GET_BURST_SyncStatus (int USB_Handle, int *Sync);					//Ϊ1��ʾ���մ���ͬ��״̬��Ϊ0��ʾ��������״̬ 
extern int B2BERT_SET_BURST_SD_Level (int USB_Handle, int SD_Level);				//Ϊ0��ʾ�͵�ƽ��Ч��Ϊ1��ʾ�ߵ�ƽ��Ч
extern int B2BERT_SET_BURST_SDChk_Pos (int USB_Handle, unsigned char SDChk_Pos);	//����SDA/SDD���λ�ã������OLT_Rst�½���
extern int B2BERT_SET_BURST_SDChk_Width (int USB_Handle, unsigned int SDChk_Width);	//����SDA/SDD����ʵ�ʿ��Ҫ��1��ʱ���Է���Ƶ�ʵ�8��ƵΪ1��λ
extern int B2BERT_SET_BURST_UserPatternLen (int USB_Handle, unsigned short UserPatternLen);		//�����Զ������ͳ���
extern int B2BERT_SET_BURST_UserPatternData (int USB_Handle, unsigned char *UserPatternData);   //�����Զ�����������

//Ӧ�ú���
extern int B2BERT_SET_NORM (int USB_Handle, int RstSel, int rate, int SelONU, int InINV, int OutINV, int Pattern, int PRBS, unsigned short int SyncErrMax); 	//����NORM���� 
extern int B2BERT_GET_NORM_Align (int USB_Handle, int *align);						//ѵ��
extern int B2BERT_GET_NORM_Sync (int USB_Handle, int *sync_state);					//����NORMͬ��
extern int B2BERT_GET_NORM_ErrCount (int USB_Handle, int rate, unsigned int *ErrNum, double *ErrTime, double *ErrRate);
extern int B2BERT_SET_NORM_ClearErr (int USB_Handle);	//����������
extern int B2BERT_SET_NORM_Close (int USB_Handle);		//�ر�Norm����״̬ 

extern int B2BERT_SET_BURST_COMMON (int USB_Handle, int rate, int SelONU, int BurstPeriod_Sel, unsigned int BurstPeriod, 
	                                int Trig_Out, int Trig_Level, unsigned int Trig_Pos, unsigned int Trig_Width, int OLTRstOut, int OLTRstLevel);//����BURST GPON�������� 
extern int B2BERT_SET_BURST_ParaConfig (int USB_Handle, int Pattern, unsigned short UserPatternLen, int PRBS, int InINV, int OutINV, unsigned short int SyncErrMax);		//����Burst��������  				
extern int B2BERT_SET_BURST_TransConfig (int USB_Handle, int Pattern, unsigned char PreGuideLen, unsigned short int PreAmbleLen, unsigned int DataLen, unsigned int TxDelay, 
										 int OLTRst_Out, unsigned short OLTRst_Pos, unsigned char OLTRst_Width, int SD_Level, unsigned char SDChk_Pos, unsigned int SDChk_Width);	//����Burst��������
extern int B2BERT_SET_BURST_ClearErr (int USB_Handle);	//����������
extern int B2BERT_GET_BURST_Align (int USB_Handle, int *align);						//ѵ��  
extern int B2BERT_GET_BURST_Sync (int USB_Handle, int *sync_state);					//����BURSTͬ�� ����ֵ-2����ͬ��ʧ�ܣ�����<0�ķ���ֵ��ʾ����ִ���쳣
//B�汾��֧�ִ���ܣ���ȡSD״̬��B2�Զ����
//extern int B2BERT_SET_BURST_ClearSD (int USB_Handle);	//���SD��� 
extern int B2BERT_GET_BURST_SDChk_Status (int USB_Handle, int *SDA, int *SDD);		//��ȡSD���
//B�汾��֧�ִ���ܣ���ȡSD״̬��B2�Զ���� 
//extern int B2BERT_GET_BURST_SDChk_Status_noClear (int USB_Handle, int *SDA, int *SDD);//��ȡSD����������Զ����״̬����
extern int B2BERT_GET_BURST_ErrCount (int USB_Handle, int rate, unsigned int *ErrNum, double *ErrTime, double *ErrRate);
extern int B2BERT_SET_BURST_Start (int USB_Handle, int ONU0State, int ONU1State);  	//���ÿ�ʼ����״̬
extern int B2BERT_SET_BURST_UserPatternOpen (int USB_Handle);						//����BERTΪ�Զ������͹���״̬���Զ��������ɵ�ǰ�Ĵ��������þ���
extern int B2BERT_SET_BURST_UserPatternClose (int USB_Handle);						//����BERTΪα������͹���״̬��m���е�ȷ���ɵ�ǰ�Ĵ��������þ��� 

extern int B2BERT_CreatPatternFile (const int datalen, unsigned char *data, char *filename);
extern int B2BERT_ReadPatternFile (const char *filename, unsigned short *datalen, unsigned char *data); 
extern int B2BERT_SET_CDR (int USB_Handle);				//����CDR
extern int BERT_Align(int USB_Handle);
#endif
