//==============================================================================
//
// Title:       EVB16.h
// Purpose:     EVB16ͷ�ļ��������˻����Ĺ��ܺ�������ͼĴ�������
//
// Created on:  2010-12-16 at 22:35:57 by roger.
// Copyright:   superxon. All Rights Reserved.
//
//==============================================================================

#ifndef _EVB16_H_
#define _EVB16_H_

#include "ch341a_dll.h"

static int AddrEVB16		= 0x84;
static int AddrEVB16_8479	= 0x86;
static int AddrEVB16_TX2012	= 0x48;
static int AddrEVB16_RX2012 = 0x50;
//static int reg_add, error;
static double T_wait=0.001;
//double global_Rate;
//unsigned char EVB16_F320_Firm_Ver;
//unsigned char EVB16_FPGA_Firm_Ver;

//static int Timer_Status_Flag = 0, Bert_Run_Flag = 0;

//10GBERT�Ĵ�������
#pragma pack (1) /* set maximum alignment to 1 ��һ���ֽڶ���*/

struct strEVB16_FPGA_REG05
{
INT8U Rst:1;			//Ϊ1Ϊ1��λ���Ĵ������ȫ��Norm Bert���ֵ�·��Ϊ0������������ȫ�ּĴ�����ResetΪ1ʱ��λΪ1
INT8U Reserved:7;		//reserved
};

struct strEVB16_FPGA_REG06
{
INT8U Rx2Tx_Loop:1;			//Ϊ1����Norm Bert��FPGA�ڲ��ӽ��ջ��ص������Ϊ0��������
INT8U Tx2Rx_Loop:1;			//Ϊ1����Norm Bert��FPGA�ڲ��ӷ���������ص��������룬Ϊ0��������
INT8U Reserved2_3:2;		//reserved
INT8U XFP_LOS:1;  			//���շ���XFPģ��LOS����澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯
INT8U XFP_NR:1;  			//���շ���XFPģ��NR����澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯
INT8U XFP_ABS:1;  			//���շ���XFPģ��ABS����澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯
INT8U Reserved7:1;
};

struct strEVB16_FPGA_REG07
{
INT8U Tx8479_LOL:1;			//���ͷ���VSC8479��ʱ��ʧ������澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯
INT8U Tx_FIFO_Err:1;		//���ͷ���VSC8479��FIFO����澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯
INT8U Rx8479_LOL:1;			//���շ���VSC8479��ʱ��ʧ������澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯
INT8U Rx8479_LOS:1;			//���շ���VSC8479��LOS����澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯
INT8U TxGenum_Fault:1;		//���ͷ���Genum�Ĵ���澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯
INT8U TxGenum_LOS:1;		//���ͷ���Genum��LOS����澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯
INT8U RxGenum_Fault:1;		//���շ���Genum�Ĵ���澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯
INT8U RxGenum_LOS:1;		//���շ���Genum��LOS����澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯
};

struct strEVB16_FPGA_REG08
{
INT8U Tx8479_LOL:1;			//���ͷ���VSC8479��ʱ��ʧ������澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯�������Զ���0
INT8U Tx_FIFO_Err:1;		//���ͷ���VSC8479��FIFO����澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯�������Զ���0
INT8U Rx8479_LOL:1;			//���շ���VSC8479��ʱ��ʧ������澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯�������Զ���0
INT8U Rx8479_LOS:1;			//���շ���VSC8479��LOS����澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯�������Զ���0
INT8U TxGenum_Fault:1;		//���ͷ���Genum�Ĵ���澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯�������Զ���0
INT8U TxGenum_LOS:1;		//���ͷ���Genum��LOS����澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯�������Զ���0
INT8U RxGenum_Fault:1;		//���շ���Genum�Ĵ���澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯�������Զ���0
INT8U RxGenum_LOS:1;		//���շ���Genum��LOS����澯��Ϊ1��ʾ�澯��Ϊ0��ʾ��������̬�澯�������Զ���0
};

struct strEVB16_FPGA_ReAlign
{
INT8U start:1; 				//��0��1����������SFI41�Ľӿڵ�����·
INT8U Reserved:7;			//reserved
};

struct strEVB16_FPGA_REG0A
{
INT8U Rx_Align_OK:1;		//Ϊ1��ʾSFI41�ӿ�����ʱ����״̬��������ϣ�Ϊ0���ڵ���
INT8U Rx_Ctrl_OK:1;			//Ϊ1��ʾSFI41�ӿ�����ʱ���Ʋ�����λ��ϣ�Ϊ0���ڸ�λ
INT8U Rx_Ctrl_Err:1;		//Ϊ1��ʾSFI41�ӿ�����ʱ���Ʋ�����������Ϊ0����
INT8U Reserved:5;			//Reserved
};

struct strEVB16_FPGA_REG0B
{
INT8U Pos:6;				//��ӦSFI4.1�ӿڵ�������ʱ����ʱֵ
INT8U Reserved:2;			//Reserved
};

struct strEVB16_FPGA_REG10
{
INT8U isUserorPRBS:1;		//Ϊ1��ʾ��������Ϊ�û��Զ������У�Ϊ0��ʾ��m����
INT8U Reserved:7;			//Reserved
};

struct strEVB16_FPGA_UserLen  //��ѡ���û���������ʱ����Ч���512bit�����4��bit��ʵ�ʸ���Ҫ��1
{
INT8U UserLen7_0;		//�û��������г��ȵĵ�8λ
INT8U UserLen8:1;		//�û��������г��ȵ����λ
INT8U Reserved:7;		//Reserved
};

struct strEVB16_FPGA_REG14
{
INT8U User_Start:1;		//����������Ϊ�û��Զ�������ʱ��Ϊ1�������ԣ�Ϊ0��λ���ԡ�����������Ϊm����ʱ��Ϊ0
INT8U User_Not_Rdy:1;	//����������Ϊ�û��Զ�������ʱ����дUser_StartΪ1��������ʱ��Ϊ1��ʾ���ͻ�û׼���ã�Ϊ0��ʾ׼����
INT8U Reserved:6;		//Reserved
};

struct strEVB16_FPGA_REG17
{
INT8U Insert:1;			//д0 -- 1 -- 0�ڷ��������в���һ��bit���룬�˶������ܼĴ���0x19��Bert_InsErr_En��Ӱ��
INT8U Reserved:7;		//Reserved
};

struct strEVB16_FPGA_REG18
{
INT8U Out_Inv:1;		//Ϊ1���Ʒ���������࣬Ϊ0������
INT8U In_Inv:1;			//Ϊ1���ƽ������뷴�࣬Ϊ0������
INT8U Reserved:6;		//Reserved
};

struct strEVB16_FPGA_REG19
{
INT8U En:1;		//Ϊ1��ʾ�����ڷ���������������в������룬Ϊ0����������
INT8U Reserved:7;		//Reserved
};

struct strEVB16_FPGA_InsErrTime	//�Է���Ƶ�ʵ�64��ƵΪ��λ(����1��bit����)��ʵ��ʱ��Ҫ��1
{
INT8U Time7_0;			//��������ʱ��ʱ���������8λ
INT8U Time15_8;
INT8U Time23_16;
INT8U Time31_24;  		//��������ʱ��ʱ���������8λ
};

struct strEVB16_FPGA_SyncErrMax	//ÿ���жϼ����14848��bit�����������С�����õ���������ʱ����Ϊͬ������ΪPRBS��ʱ��һ������Ϊ0x5C6������Ϊ�Զ�������ʱ��һ����Ϊ0x004
{
INT8U ErrMax7_0;		//��ͬ��ʱ�ж�ͬ�������������8λ
INT8U ErrMax13_8:6;		//��ͬ��ʱ�ж�ͬ�������������6λ
INT8U Reserved:2;		//Reserved
};

struct strEVB16_FPGA_SyncStatus
{
INT8U Sync_OK:1;		//Ϊ1��ʾ���մ���ͬ��״̬��Ϊ0��ʾ��������״̬
INT8U Reserved:7;		//Reserved
};

struct strEVB16_FPGA_ReSync
{
INT8U start:1;			//��0��1����������������������ͬ��
INT8U Reserved:7;		//Reserved
};
struct strEVB16_FPGA_Single_Repeat
{
INT8U Single_Repeat:1;	//1���Σ�0�ظ�
INT8U Reserved:7;		//Reserved
};

struct strEVB16_FPGA_Single_Repeat_Over_Flag
{
INT8U Single_Repeat_Over_Flag:1;	//1���ڵ��β��ԣ�0���β������
INT8U Single_Err_Flag:1;			//���β��Ե���ǰ��״̬ʱ(�п��ܲ���ʱ�仹û�����)�Ƿ������룬Ϊ1��ʾ�����룬Ϊ0��ʾû�����롣
INT8U Reserved:6;					//Reserved
};

struct strEVB16_FPGA_Single_Time_Set
{
INT8U Single_Time_Set7_0;			//����������Ե�ʱ�����8λ���Է���Ƶ�ʵ�64��ƵΪ��λ��ʵ������Ҫ��1��������32λ���ʱ��ʱ������ֵΪȫ1��
INT8U Single_Time_Set15_8;
INT8U Single_Time_Set23_16;
INT8U Single_Time_Set31_24;
INT8U Single_Time_Set39_32;			//����������Ե�ʱ������8λ
};

struct strEVB16_FPGA_ErrTimeCnt //��дErrTime7_0�Ĵ�������ֵ����������ǰ���Ե������ۼӸ����Ͳ���ʱ�䣬ͬʱ�����´β��� �����ζ�д������ܳ���27s��
{
INT8U ErrTime7_0;		//������Ե�ʱ�����8λ���Է���Ƶ�ʵ�64��ƵΪ��λ��ʵ������Ҫ��1��������32λ���ʱ��ʱ������ֵΪȫ1��
INT8U ErrTime15_8;
INT8U ErrTime23_16;
INT8U ErrTime31_24;		
INT8U ErrTime39_32;		//������Ե�ʱ������8λ   
};

struct strEVB16_FPGA_ErrNumCnt
{
INT8U ErrNum7_0;		//������Ե������ۼӸ��������8λ��������40λ������ʱ������ֵΪȫ1��
INT8U ErrNum15_8;
INT8U ErrNum23_16;
INT8U ErrNum31_24;		
INT8U ErrNum39_32;		//������Ե������ۼӸ��������8λ
};

//����FPGA�Ĵ���
struct strEVB16_FPGA_FPGA
{
INT8U	FPGA_Ver; 									//[0x00h]
INT8U   FPGA_TYPE1;									//[0x01h]
INT8U   FPGA_TYPE2;									//[0x02h]
INT8U   FPGA_TYPE3;									//[0x03h]
INT8U   Reserved4;									//[0x04h]
struct  strEVB16_FPGA_REG05  		Reset;			//[0x05h]
struct	strEVB16_FPGA_REG06 		Loop;			//[0x06h]
struct	strEVB16_FPGA_REG07 		StatusStatic;	//[0x07h]
struct  strEVB16_FPGA_REG08 		StatusDynamic;  //[0x08h]
struct	strEVB16_FPGA_ReAlign 		AlignStart;		//[0x09h]
struct	strEVB16_FPGA_REG0A 		AlignStatus;	//[0x0Ah]
struct	strEVB16_FPGA_REG0B 		SamplePos;		//[0x0Bh]
INT8U	Reserved12to14[3];							//[0x0Ch~0x0Eh], 3byte
INT8U	WorkMode;									//[0x0Fh] Work_Mode��Ϊ1���SDH֡�����������⣬Ϊ0���PRBS
struct	strEVB16_FPGA_REG10			Pattern;		//[0x10h] ���Ƹ�λOLT��Rst�źŵ���ʱֵ��һ����Ϊ0x05��Ҫ����ʵ�������������
INT8U 	PRBS;										//[0x11h]
struct 	strEVB16_FPGA_UserLen		UserLen;		//[0x12h~0x13h], 2byte
struct	strEVB16_FPGA_REG14			UserStatus;		//[0x14h]
INT8U	Reserved21to22[2];							//[0x15h~0x16h], 2byte
struct	strEVB16_FPGA_REG17			InsErrBit;		//[0x17h]
struct	strEVB16_FPGA_REG18			INV;			//[0x18h]
struct	strEVB16_FPGA_REG19			InsErrEn;		//[0x19h]
INT8U	Reserved26to27[2];							//[0x1Ah~0x1Bh], 2byte
struct	strEVB16_FPGA_InsErrTime 	InsErrTime; 	//[0x1Ch~0x1Fh], 4byte
struct	strEVB16_FPGA_SyncErrMax	SyncErrMax;   	//[0x20h~0x21h], 2byte
struct	strEVB16_FPGA_SyncStatus	SyncStatus;		//[0x22h]
struct	strEVB16_FPGA_ReSync		SyncStart;		//[0x23h]
struct	strEVB16_FPGA_ErrTimeCnt	ErrTime;		//[0x24h~0x27h], 4byte
struct	strEVB16_FPGA_ErrNumCnt		ErrNum;			//[0x28h~0x2Bh], 4byte

INT8U	Reserved46to47[2];											//[0x2Eh~0x2Fh], 2byte
struct	strEVB16_FPGA_Single_Repeat				Single_or_Repeat;	//[0x30h]
struct 	strEVB16_FPGA_Single_Repeat_Over_Flag	Single_Over_Flag;	//[0x31h]
INT8U	Reserved50to55[6];											//[0x32h~0x37h], 6byte

struct 	strEVB16_FPGA_Single_Time_Set			Single_Time_Value;	//[0x38h~0x3Ch], 5byte

INT8U	Reserved61to63[3];							//[0x3Dh~0x3Fh], 3byte
INT8U	UserPattern[64];							//[0x40h~0x7Fh], 64byte  ����û��Զ������еĴ洢�����64��byte(512bit)�����4��bit�����ݷ��ʹӵ�λ��ַ��ʼ��ÿһ��Byte�ǴӸ�λD7��ʼ��D0��˳����
};

union uEVB16_FPGA
{
struct strEVB16_FPGA_FPGA  	sStr;
INT8U  						pStr[128];
};//EVB16_FPGA;

/*
---- ����
---- bit 0 ��CLK_TYPE0
---- bit 1 ��CLK_TYPE1
---- bit 2 ��XFP_RxLOS ��Ϊ1��ʾXFPģ���LOS�澯
---- bit 3 ��XFP_NR  ��Ϊ1��ʾXFP���ͻ����������
---- bit 4 ��XFP_ABS ��Ϊ0��ʾXFPģ����ϣ�Ϊ1��ʾû�в��ϡ�
---- bit 5 ��XFP_INT ��Ϊ0��ʾ�и澯
---- bit 6 ���������ɶ�д
---- bit 7 ���������ɶ�д
*/
typedef struct _ARM_INPUT
{
INT8U OscType:2;
INT8U RxLos:1;
INT8U Nr:1;
INT8U Abs:1;
INT8U Int:1;
INT8U Rsv:2;
}ARM_INPUT;

/*
----д(�ɶ�)��
---- bit 0 ��CK_Src_Sel	��0ѡ�ڲ��ο�ʱ��
---- bit 1 ��CK_Sel0
---- bit 2 ��CK_Sel1
---- bit 3 ��CK_Sel2
---- bit 4 ��GN_Reset	��0��λ
---- bit 5 ��XFP_DESEL	��0����XFPģ����ӦI2C
---- bit 6 ��XFP_PDRST	���½��ظ�λ����ʹģ�鴦��Powerdownģʽ��Ϊ0����
---- bit 7 ��XFP_TxDIS	��Ϊ1�ر�XFP����
*/
typedef struct _ARM_OUTPUT
{
INT8U RefClk:1;                   // Defined by hardware
INT8U Freq:3;
INT8U GnRst:1;
INT8U Desel:1;
INT8U PdRst:1;
INT8U TxDis:1;
}ARM_OUTPUT;

struct Si570Reg_DEF                 // 6byte, Si570����ֵ R7��R12
{
    INT8U  R7to12[6];
};

struct EVB16
{
/******************************************************************************/
/*                           0x00~0x7F                                        */
/******************************************************************************/
    INT8U      FlashCtl;            // 0x00, 'E':clear flash page0, 'W': write Flash
    INT8U      SubVer;              // 0x01, Sublevel version number
    INT8U      MianVer;             // 0x02, Main version number
    INT8U      TestReg;             // 0x03, read out = ~write in
    INT8U      ReserveRegA[16];     // 0x04-0x13, 16 bytes Reserved register
    INT16U     CoreVolt;            // 0x14-0x15, ADuC7020 core voltage, 1LSB: 0.1mV
    INT16S     CoreTemp;            // 0x16-0x17, ADuC7020 core temperature, 1LSB: 1/256��
    ARM_INPUT  ArmInput;            // 0x18, ADuC7020 input port state
    ARM_OUTPUT ArmOutput;           // 0x19, ADuC7020 output port set

    INT16U     VoAdcValue;          // 0x1A-0x1B, VCC_ONU ADC value
    INT16U     VaAdcValue;          // 0x1C-0x1D, VCCA ADC value
    INT16U     V25AdcValue;         // 0x1E-0x1F, +2.5V ADC value
    INT16U     V18AdcValue;         // 0x20-0x21, +1.8V ADC value
    INT16U     V10AdcValue;         // 0x22-0x23, +1.0V ADC value

    INT16U     OnuVolt;             // 0x24-0x25, VCC_ONU calculate value, 1LSB: 1mV
    INT16U     AnalogVolt;          // 0x26-0x27, VCCA calculate value, 1LSB: 1mV
    INT16U     V25Volt;             // 0x28-0x29, +2.5V calculate value, 1LSB: 1mV
    INT16U     V18Volt;             // 0x2A-0x2B, +1.8V calculate value, 1LSB: 1mV
    INT16U     V10Volt;             // 0x2C-0x2D, +1.0V calculate value, 1LSB: 1mV

	INT8U      I2Ccommand;			// 0x2E-0x2F, 1byte
	// ="R" if start reading; ="W" if start writing;
	// return the error code after operation completed:
	// error=0 if success; =1 if error occurs;
	INT8U      I2Cslave_address;	// 0x2F, 1byte
	INT8U      I2Creg_start_add;	// 0x30, 1byte
	INT8U      I2Creg_length;		// 0x31, 1byte
	INT8U      I2Creg_dat_arr[8];	// 0x32-0x39, 8byte

	INT8U      Si570cmd;			// 0x3A, 1byte
	INT8U      Si570R7;				// 0x3B, 1byte
	INT8U      Si570R8;				// 0x3C, 1byte
	INT8U      Si570R9;				// 0x3D, 1byte
	INT8U      Si570R10;			// 0x3E, 1byte
	INT8U      Si570R11;			// 0x3F, 1byte
	INT8U      Si570R12;			// 0x40, 1byte
	INT8U      Si570R135;			// 0x41, 1byte
	INT8U      Si570R137;			// 0x42, 1byte

	struct  Si570Reg_DEF Si570Regs;	// 0x43-0x48, 6byte, Si570����ֵ

    INT8U      ReserveRegB[50];     // 0x49-0x7A, 51byte,Reserved register
    INT8U      PassWord[4];         // 0x7B, 4 bytes password
    INT8U      TableIndex;          // 0x7F, Table index
    INT8U      ReserveRegC[4];      // 0x80-0x83, 4byte,Reserved register
    INT8U      BoardPn[16];         // 0x84-0x93, 16byte,Board PN
    INT8U      BoardSn[16];         // 0x94-0xA3, 16byte,Board SN
    INT8U      ReserveRegD[92];     // 0xA4-0xFF, 4byte,Reserved register
};

union uEVB16
{
struct EVB16 sStr;
INT8U        pStr[256];
};//EVB16;

typedef struct _GN2012_REG02
{
INT8U POLINVFAULT:1;  				//
INT8U POLINVRXLOS:1;
INT8U OPENDRAINRXLOS:1;
INT8U OPENDRAINFAULT:1;
INT8U RSVD:4;
}GN2012_REG02;

typedef struct _GN2012_REG03
{
INT8U PRBSGENSTART:1;				//
INT8U PRBSCHKCLEARERR:1;
INT8U RSVD:6;
}GN2012_REG03;

typedef struct _GN2012_REG04
{
INT8U FAULTMASKRXLOL:1;				//
INT8U FAULTMASKTXLOL:1;
INT8U FAULTMASKRXLOS:1;
INT8U RSVD3_3:1;
INT8U FAULTMASKTXLOS:1;
INT8U RSVD5_7:3;
}GN2012_REG04;

typedef struct _GN2012_LOOPBK
{
INT8U LBRXINEN:1;					//
INT8U LBRXINPRBSGEN:1;
INT8U LBRXINTXDATA:1;
INT8U LBRXINREFCLK:1;
INT8U LBRXOUTEN:1;
INT8U LBRXOUTTXDATA:1;
INT8U LBRXOUTPRBSGEN:1;
INT8U LBRXOUTRXCLK:1;
INT8U LBTXINEN:1;					//
INT8U LBTXINPRBSGEN:1;
INT8U LBTXINTXDATA:1;
INT8U LBTXINREFCLK:1;
INT8U LBTXOUTEN:1;
INT8U LBTXOUTTXDATA:1;
INT8U LBTXOUTPRBSGEN:1;
INT8U LBTXOUTRXCLK:1;
INT8U PRBSGENCLKSEL:1;
INT8U PRBSCHKCLKSEL:1;
INT8U RSVD:6;
}GN2012_LOOPBK;

typedef struct _GN2012_REG0E
{
INT8U TXPOLINV:1;
INT8U RSVD1_1:1;
INT8U TXPLLBYPASS:1;
INT8U TXPLLAUTOBYPASSEN:1;
INT8U TXPLLRATESEL:2;
INT8U RSVD6_7:2;
}GN2012_REG0E;

typedef struct _GN2012_REG12
{
INT8U RSVD0_2:3;
INT8U TXPLLCLKDIVRATIO:2;
INT8U RSVD5_5:1;
INT8U TXPLLSONETIJTSETTING:2;
}GN2012_REG12;

typedef struct _GN2012_REG13
{
INT8U TXPLLLOS:1;			//Tx CDR loss of signal when HIGH.
INT8U TXPLLLOL:1;			//Tx CDR loss of lock when HIGH.
INT8U RSVD:6;
}GN2012_REG13;

typedef struct _GN2012_REG18
{
INT8U TXPLLLOS:1;
INT8U TXPLLLOL:1;
INT8U RSVD2_7:6;
}GN2012_REG18;

typedef struct _GN2012_REG1D
{
INT8U RXPLLLOS:1;
INT8U RXPLLLOL:1;
INT8U RSVD2_7:6;
}GN2012_REG1D;

typedef struct _GN2012_RXSDO
{
INT8U RXSDOSWING:4;
INT8U RSVD4_7:4;
INT8U RXSDOPOSTTAP:5;
INT8U RSVD13_15:3;
INT8U RXSDOFORCEMUTE:1;
INT8U RXSDOAUTOMUTEEN:1;
INT8U RXSDOPWRDNONMUTE:1;
INT8U RSVD19_23:5;
}GN2012_RXSDO;

typedef struct _GN2012_REG65
{
INT8U TXSDOSWING:4;
INT8U RSVD:4;
}GN2012_REG65;

typedef struct _GN2012_REG66
{
INT8U TXSDOPOSTTAP:4;
INT8U RSVD:4;
}GN2012_REG66;

typedef struct _GN2012_TXPWRDN
{
INT8U TXPDTXPATH:1;
INT8U TXPDTXCDR:1;
INT8U RSVD2_2:1;
INT8U TXPDTXSDO:1;
INT8U TXPDTXSDOPE:1;
INT8U RSVD5_7:3;
INT8U TXPDEQ:1;
INT8U TXPDLOS:1;
INT8U RSVD10_14:5;
INT8U TXPDPHADJ:1;
INT8U TXPDRATEDET:1;
INT8U TXPDVEYEMON:1;
INT8U TXPDHEYEMON:1;
INT8U TXPDPKDET:1;
INT8U RSVD20_20:1;
INT8U TXPDCKDIVOUT:1;
INT8U RSVD22_23:2;
INT8U TXPDSONETIJT:1;
INT8U RSVD25_31:7;
}GN2012_TXPWRDN;

typedef struct _GN2012_RXPWRDN
{
INT8U RXPDRXPATH:1;
INT8U RXPDRXCDR:1;
INT8U RXPDRXSDO:1;
INT8U RXPDRXSDOPE:1;
INT8U RSVD4_7:4;
INT8U RXPDLA:1;
INT8U RXPDSLICEADJ:1;
INT8U RXPDLOS:1;
INT8U RSVD11_15:5;
INT8U RXPDRATEDET:1;
INT8U RXPDVEYEMON:1;
INT8U RXPDHEYEMON:1;
INT8U RXPDPKDET:1;
INT8U RSVD20_20:1;
INT8U RXPDCKDIVOUT:1;
INT8U RSVD22_23:2;
INT8U RSVD24_27:4;
INT8U RXPDSONETIJT:1;
INT8U RSVD29_31:3;
}GN2012_RXPWRDN;

typedef struct _GN2012_PWRDN
{
INT8U PDREFCLK:1;
INT8U PDPRBSGEN:1;
INT8U PDPRBSCHK:1;
INT8U PDTEMPSENSOR:1;
INT8U PDSUPPLYSENSOR:1;
INT8U PDADC:1;
INT8U RSVD6_7:2;
INT8U RSVD8_11:4;
INT8U RSVD12_15:4;
}GN2012_PWRDN;

//����GN2012�Ĵ���
struct strEVB16_GN2012
{
INT8U			RSVD0; 							//[0x00h]
INT8U   		RSVD1;							//[0x01h]
GN2012_REG02 	TOP_REG2;						//[0x02h]
GN2012_REG03 	TOP_REG3; 						//[0x03h]
GN2012_REG04 	TOP_REG4; 						//[0x04h]
INT8U			RSVD5to6[2];					//[0x05h~0x06h], 2byte
GN2012_LOOPBK   LOOPBK;							//[0x07h~0x09h], 3byte
INT8U			RSVD10to13[4];					//[0x0Ah~0x0Dh], 2byte
GN2012_REG0E	TXPLL_REG5;						//[0x0Eh]
INT8U			RSVD15to17[3];					//[0x0Fh~0x11h], 3byte
GN2012_REG12	TXPLL_REG9;						//[0x12h]
GN2012_REG13	TXPLL_REG10;					//[0x13h]
INT8U			RSVD20to23[4];					//[0x14h~0x17h], 4byte
GN2012_REG18	RXPLL_REG5;						//[0x18h]
INT8U			RSVD25to28[4];					//[0x19h~0x1Ch], 4byte
GN2012_REG1D	RXPLL_REG10;					//[0x1Dh]
INT8U			RSVD30to74[45];					//[0x1Eh~0x4Ah], 45byte
GN2012_RXSDO	RXSDO;							//[0x4Bh~0x4Dh], 3byte
INT8U			RSVD78to100[23];				//[0x4Eh~0x64h], 23byte
GN2012_REG65	TXSDO_REG27;					//[0x65h]
GN2012_REG66	TXSDO_REG28;  					//[0x66h]
INT8U			RSVD103to129[27];				//[0x67h~0x81h], 27byte
GN2012_TXPWRDN  TXPWRDN;						//[0x82h~0x85h], 4byte
GN2012_RXPWRDN	RXPWRDN;						//[0x86h~0x89h], 4byte
INT8U			RSVD138;						//[0x8Ah]
GN2012_PWRDN	PWRDN;							//[0x8Bh~0x8Ch], 2byte
INT8U			RSVD141to255[115];				//[0x8Dh~0xFFh], 115byte
};

union uEVB16_GN2012
{
struct strEVB16_GN2012 	sStr;
INT8U        			pStr[256];
};//EVB16_GN2012;

//����VSC8479�Ĵ���
typedef struct _VSC8479_TXCLKSRC
{
INT8U TXREFSRC:1;
INT8U TXREFSELF:1;
INT8U TXCOUTMUTEB:1;
INT8U TXPCLKMUTEB:1;
INT8U TXPHSADJ:2;
INT8U TXMCLKSELF:1;
INT8U TXMCLKMUTEB:1;
}VSC8479_TXCLKSRC;

typedef struct _VSC8479_TXFIFOSRC
{
INT8U TXFIFORESB:1;
INT8U TXFIFORARB:1;
INT8U Reserved:1;
INT8U TXDATAINV:1;
INT8U TXMSBSELB:1;
INT8U PDEN:1;
INT8U PDREFSRC:1;
INT8U EXREFSELF:1;
}VSC8479_TXFIFOSRC;

typedef struct _VSC8479_RXCLKSRC
{
INT8U RXREFSRC:1;
INT8U RXREFSELF:1;
INT8U RXPOCLKMUTEB:1;
INT8U RXMCLKSELF:1;
INT8U RXMCLKMUTEB:1;
INT8U TXLLOOPENB:1;
INT8U RXDLOOPENB:1;
INT8U TXLINETIMSELB:1;
}VSC8479_RXCLKSRC;

typedef struct _VSC8479_RXCTRLSRC
{
INT8U RXLOSDATSQB:1;
INT8U RXLOCKREFB:1;
INT8U Reserved2_2:1;
INT8U RXDATAINV:1;
INT8U RXMSBSELB:1;
INT8U RXDOUTMUTEB:1;
INT8U Reserved6_6:1;
INT8U BISTEN:1;
}VSC8479_RXCTRLSRC;

typedef struct _VSC8479_STATUSSRC
{
INT8U TXFIFOERRB:1;
INT8U TXLOCKERRB:1;
INT8U RXLOSB:1;
INT8U RXLOCKERRB:1;
INT8U BISTFAIL:1;
INT8U Reserved5_7:3;
}VSC8479_STATUSSRC;

typedef struct _VSC8479_TXDUOBINSRC
{
INT8U TXDUOBINEN:1;
INT8U Reserved:7;
}VSC8479_TXDUOBINSRC;

typedef struct _VSC8479_STATUSMODE
{
INT8U TXFIFOERRB:1;
INT8U TXLOCKERRB:1;
INT8U RXLOSB:1;
INT8U RXLOCKERRB:1;
INT8U BISTFAIL:1;
INT8U Reserved5_5:1;
INT8U PINMUXEN:1;
INT8U SIINV:1;
}VSC8479_STATUSMODE;

typedef struct _VSC8479_TXCLK
{
INT8U TXREFSRC:1;
INT8U TXREFSELF:1;
INT8U TXCOUTMUTEB:1;
INT8U TXPCLKMUTEB:1;
INT8U TXPHSADJ:2;
INT8U TXMCLKSELF:1;
INT8U TXMCLKMUTEB:1;
}VSC8479_TXCLK;

typedef struct _VSC8479_TXFIFO
{
INT8U TXFIFORESB:1;
INT8U TXFIFOARB:1;
INT8U Reserved:1;
INT8U TXDATAINV:1;
INT8U TXMSBSELB:1;
INT8U PDEN:1;
INT8U PDREFSRC:1;
INT8U EXREFSELF:1;
}VSC8479_TXFIFO;

typedef struct _VSC8479_RXCLK
{
INT8U RXREFSRC:1;
INT8U RXREFSELF:1;
INT8U RXPOCLKMUTEB:1;
INT8U RXMCLKMUTEB:1;
INT8U TXLLOOPENB:2;
INT8U RXDLOOPENB:1;
INT8U TXLINETIMSELB:1;
}VSC8479_RXCLK;

typedef struct _VSC8479_RXCTRL
{
INT8U RXLOSDATSQB:1;
INT8U RXLOCKREFB:1;
INT8U Reserved:1;
INT8U RXDATAINV:1;
INT8U RXMSBSELB:1;
INT8U RXDOUTMUTEB:1;
INT8U RX_LCKREF_DATSQB_EN:1;
INT8U BISTEN:1;
}VSC8479_RXCTRL;

typedef struct _VSC8479_STATUS
{
INT8U TXFIFOERRB:1;
INT8U TXLOCKERRB:1;
INT8U RXLOSB:1;
INT8U RXLOCKERRB:1;
INT8U BISTFAIL:1;
INT8U Reserved:3;
}VSC8479_STATUS;

typedef struct _VSC8479_RXLOSCTRL
{
INT8U RX_LOS_RANGE:3;
INT8U RX_LOS_ENABLE:1;
INT8U RX_LOT_ENABLE:1;
INT8U RX_LOS_DAC_ENABLE:1;
INT8U RX_LOS_AUTO_ANA:1;
INT8U Reserved:1;
}VSC8479_RXLOSCTRL;

typedef struct _VSC8479_TXODSLEW
{
INT8U TX_OD_SLEW:2;
INT8U Reserved:6;
}VSC8479_TXODSLEW;

typedef struct _VSC8479_TXDUOBIN
{
INT8U TXDUOBINEN:1;
INT8U Reserved:7;
}VSC8479_TXDUOBIN;

typedef struct _VSC8479_BISTCTRL
{
INT8U PATTERN:2;
INT8U Reserved2_4:3;
INT8U BISTSTART:1;
INT8U BISTBUSY:1;
INT8U SYNCLOSS:1;
}VSC8479_BISTCTRL;

struct strEVB16_VSC8479
{
VSC8479_TXCLKSRC		TXCLKSRC; 				//[0x00h]
VSC8479_TXFIFOSRC  		TXFIFOSRC;				//[0x01h]
VSC8479_RXCLKSRC		RXCLKSRC; 				//[0x02h]
VSC8479_RXCTRLSRC		RXCTRLSRC; 				//[0x03h]
VSC8479_STATUSSRC		STATUSSRC;				//[0x04h]
VSC8479_TXDUOBINSRC		TXDUOBINSRC;  			//[0x05h]
INT8U					Reserved6; 				//[0x06h]
VSC8479_STATUSMODE		STATUSMODE; 			//[0x07h]
VSC8479_TXCLK			TXCLK; 					//[0x08h]
VSC8479_TXFIFO  		TXFIFO;					//[0x09h]
VSC8479_RXCLK			RXCLK; 					//[0x0Ah]
VSC8479_RXCTRL			RXCTRL; 				//[0x0Bh]
VSC8479_STATUS			STATUS;    				//[0x0Ch]
INT8U					Reserved13to15[3]; 		//[0x0Dh~0x0Fh], 3byte
INT8U					RXGAIN;    				//[0x10h]
INT8U					RXBOOST;    			//[0x11h]
INT8U					RXOC;    				//[0x12h]
VSC8479_RXLOSCTRL		RXLOSCTRL;    			//[0x13h]
INT8U					RXLOSTHR;    			//[0x14h]
INT8U					Reserved21; 			//[0x15h]
INT8U					TXODSW;    				//[0x16h]
INT8U					TXODPE;    				//[0x17h]
VSC8479_TXODSLEW		TXODSLEW;    			//[0x18h]
VSC8479_TXDUOBIN		TXDUOBIN;    			//[0x19h]
INT8U					Reserved26to40[15]; 	//[0x1Ah~0x28h], 15byte
VSC8479_BISTCTRL		BISTCTRL;    			//[0x29h]
INT8U					Reserved42to126[85]; 	//[0x2Ah~0x7Eh], 85byte
INT8U					RESET;
};
enum enum_10GBERT_SignalIn
{  
	SBERT10G_SignalIn_XFP,
	SBERT10G_SignalIn_SMA
};

enum enum_10GBERT_Pattern
{  
	SBERT10G_PRBS7,
	SBERT10G_PRBS9=9,
	SBERT10G_PRBS11=11,
	SBERT10G_PRBS15=15,
	SBERT10G_PRBS20=20,
	SBERT10G_PRBS21=21,
	SBERT10G_PRBS23=23,
	SBERT10G_PRBS29=29,
	SBERT10G_PRBS31=31,
	SBERT10G_SDH=-1
};

enum enum_10GBERT_Rate
{  
	SBERT10G_R103125Mbps=0
};

enum enum_10GBERT_CLK
{  
	SBERT10G_INTCLK,
	SBERT10G_EXTCLK
};

union uEVB16_VSC8479
{
struct strEVB16_VSC8479 sStr;
INT8U        			pStr[128];
};//EVB16_VSC8479;

#pragma pack ()  /*ȡ���Զ����ֽڶ��뷽ʽ*/


#define SINGLE_MODE 1
#define REPEAT_MODE 0 
#define SEVB16_R9953Mbps (9.953)
#define SEVB16_R103125Mbps (10.3125)

int evb16_init (char sn[], int *instrumentHandle); 	//����sn��ȡ�豸�˿ں�
int evb16_GetCoreTemp (int instrumentHandle, float *Temper);
int evb16_GetCoreVolt (int instrumentHandle, float *Volt);
int evb16_SetClkSrc (int instrumentHandle, int XfpOut, int ClockSrc); //XfpOut=1, �ر�XFP����� ClockSrc=0��ѡ���ڲ�ʱ��
int evb16_GetBoardSN (int panel, int control);
int evb16_SetBERTInit (int instrumentHandle, int inv_out, int inv_in, int sma_out, int xfp_out, int pattern, int clk, int signal);
int evb16_SetBERTStart (int instrumentHandle, int *align, int *sync, int Signal);
int evb16_GetBERTStatus (int instrumentHandle, int signal, int *lock, int *sync, unsigned long long *ErrNum, double *ErrTime, double *ErrRate, char StatusString[]);

int evb16_SetVsc8479Reset (int instrumentHandle);
int evb16_SetVsc8479RxCtrlSrc (int instrumentHandle, int bisten);
int evb16_SetVsc8479TxClkSrc (int instrumentHandle);
int evb16_SetVsc8479Txfifosrc (int instrumentHandle);
int evb16_SetVsc8479RxClkSrc  (int instrumentHandle);
int evb16_SetVsc8479Init (int instrumentHandle);

int evb16_GetGn2012TxCDR (int instrumentHandle, int *LOS, int *LOL);
int evb16_GetGn2012RxCDR_Tx (int instrumentHandle, int *LOS, int *LOL);
int evb16_GetGn2012RxCDR_Rx (int instrumentHandle, int *LOS, int *LOL);

int evb16_SetGn2012InitTx (int instrumentHandle);
int evb16_SetGn2012InitRx (int instrumentHandle, int signal);

int evb16_SetFPGARst (int instrumentHandle, int rst);
int evb16_SetFPGAWorkmode (int instrumentHandle, int mode);					//Mode=1,���SDH֡
int evb16_SetFPGALoop (int instrumentHandle);
int evb16_SetFPGAPattern (int instrumentHandle, int Pattern);				//PatternΪ1��ʾ��������Ϊ�û��Զ������У�Ϊ0��ʾ��m����
int evb16_SetFPGAPRBS (int instrumentHandle, int PRBS);
int evb16_SetFPGAUserStart (int instrumentHandle, int Userstart);
int evb16_SetFPGAInv (int instrumentHandle, int InINV, int OutINV);
int evb16_SetFPGAInsErrEn  (int instrumentHandle, int InsErrEn);
int evb16_SetFPGAInsErrTime (int instrumentHandle, unsigned int InsErrTime);
int evb16_SetFPGASyncErrMax (int instrumentHandle, unsigned short int SyncErrMax);
int evb16_SetFPGAAlignStart (int instrumentHandle, int Align);
int evb16_GetFPGAAlignStatus (int instrumentHandle, int *AlignOK, int *Ctrl_OK, int *Ctrl_Err);
int evb16_SetFPGASyncStart (int instrumentHandle, int Sync);
int evb16_GetFPGASyncStatus (int instrumentHandle, int *SyncOK);
int evb16_GetFPGARxLosLol(int instrumentHandle,int signal);
int evb16_SetFPGASyncRestart(int instrumentHandle);
int evb16_SetFPGAClearErr (int instrumentHandle);
int evb16_GetFPGAErrCount (int instrumentHandle, int rate, unsigned long long *ErrNum, double *ErrTime, double *ErrRate);

int evb16_SetRate(int instrumentHandle,double FOUT1);
int evb16_GetSi570Flash(int instrumentHandle);
int evb16_GetSi570Reg(int instrumentHandle, union uEVB16 *EVB16);
int evb16_SaveSi570NVM(int instrumentHandle);
int evb16_SetSi570Reg(int instrumentHandle);
int evb16_TestSi570Write(int instrumentHandle,unsigned char Si570byteaddr,unsigned char Si570data);
int evb16_TestSi570Read(int instrumentHandle,unsigned char Si570byteaddr,unsigned char* Si570data);
//int evb16_TestSi570(int instrumentHandle,unsigned char Si570addr);

int WEPP (int handle, unsigned char EPP_ADD, unsigned char EPP_DAT);
int REPP (int handle, unsigned char EPP_ADD, unsigned char *EPP_DAT);
int WI2C (int handle, unsigned char device_ADD, unsigned char I2C_ADD, unsigned char I2C_DAT);
int RI2C (int handle, unsigned char device_ADD, unsigned char I2C_ADD, unsigned char *I2C_DAT);

int evb16_ModeSet(int instrumentHandle,int Mode,int settime);	
int evb16_CheckSingleStatus(int instrumentHandle,int *Status);

void Init_Display_SetPanel(void);
void Test_Open_Timer(void);
void Test_Close_Timer(void);

#endif

