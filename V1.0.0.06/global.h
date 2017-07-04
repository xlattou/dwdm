/****************************************************************************
 *
 * File:                global.h
 *
 * Author:              SUPERXON INC.(Roger Li)
 *
 * Description:         ȫ�ֱ��������ļ�
 *
 * Time:                2013-5-23
 *
 * version:				V1.4.0.18
 * 
 * Update Description:  new document

�汾��ʷ 
===================================
����ʱ�䣺2013-5-23
���°汾��V1.4.0.18
�����ˣ�  Roger
�������ݣ�
1������������
ERR_UX3328_SUM_UPDATE       =-439,  //ux3328����A2У��λ�쳣
ERR_UX3328_SUM_CHECK        =-440,  //ux3328���A2У��λ�쳣

===================================
����ʱ�䣺2013-1-21
���°汾��V1.4.0.17
�����ˣ�  Eric.Yao
�������ݣ�
1������������
ERR_TES_TE			        =-438,	//����TE���� 

===================================
����ʱ�䣺2012-12-20
���°汾��V1.4.0.16
�����ˣ�  Eric.Yao
�������ݣ�
1������������
ERR_TES_SE_MOD80_DAC		=-436,  //����80�ȵ��Ƶ�������Ԥ��ֵ

===================================
����ʱ�䣺2012-7-13
���°汾��V1.4.0.15
�����ˣ�  Roger
�������ݣ�
1��add error code -433

===================================
����ʱ�䣺2012-5-31
���°汾��V1.4.0.14
�����ˣ�  Roger
�������ݣ�
1��add error code -432 -118 -119

===================================
����ʱ�䣺2012-4-17
���°汾��V1.4.0.13
�����ˣ�  Roger
�������ݣ�
1����������ID��PRODUCTLINE_HJ

V1.4.0.12
1 add error code -430

V1.4.0.11
����DRIVERCHIP_UX3328��CHIEFCHIP_UX3328

V1.4.0.10
1 add error code -324 -325 -326 -327 -328 -329 -429
2 Add EVB11

V1.4.0.9
������Ʒ���� PN_TYPE_XFP
��������оƬ DRIVERCHIP_GN2010
�����޷�оƬ AMPLIFIERCHIP_SY88933V,AMPLIFIERCHIP_SY88903AL
****************************************************************************/
#ifndef _GLOBAL_H_
#define _GLOBAL_H_ 

#include "CH341A_DLL.h"  
#include <windows.h> 
#include <visatype.h> 
#include <formatio.h>  
#include <utility.h> 
#include <userint.h>  

#ifdef __cplusplus
    extern "C" {
#endif

#define PN_SFF					0
#define PN_SFP					1
#define PN_ONU					2 
#define PN_SDH  				3
#define PN_GBE  				4
#define PN_ONU_DDMI				5
#define PN_GPON_OLT_SFP			6		

//Ϊ�˱��ֳ���������ԣ���ʱ������#define�Ķ��壬�����������
enum ePN_TYPE
{  
PN_TYPE_SFF,
PN_TYPE_SFP,
PN_TYPE_ONU,
PN_TYPE_SDH,
PN_TYPE_GBE,  
PN_TYPE_ONU_DDMI,
PN_TYPE_GPON_OLT_SFP,
PN_TYPE_GPON_ONT,
PN_TYPE_XFP,
PN_TYPE_10G_EPON_ONU,
PN_TYPE_10G_EPON_OLT,
PN_TYPE_SFP_PLUS,
PN_TYPE_XGPON_ONU,   /***����XGPON ONU����**Eric.Yao**20141023***/  
PN_TYPE_XGPON_OLT,
PN_TYPE_XGSPON_OLT,		/***����XGAPON OLT ����**wenyao.xi**2017-0329***/
PN_TYPE_COMBO_PON_OLT	/***����COMBO PON OLT����**Eric.Yao**20170213***/

};
		
//for c class or i class
#define PN_c 	0 
#define PN_i 	1

//for RSSI
/* V5.0�汾��Firmware����RSSIModeΪ��ֵͬʱ�Ĵ������
RSSIMode=0��V2.2a��ʽ����APD�������޹ܽ�RSSI�жϣ���ѯADC����1�β�ƽ�������¶Ȳ����Ľ�����ն˹⹦�ʣ�  
RSSIMode=1��V3.5a��ʽ����APD�������йܽ�RSSI�жϣ��ж�ADC����1�β�ƽ�������¶Ȳ����Ľ�����ն˹⹦�ʣ� 
RSSIMode=2��V3.5b��ʽ����APD�������йܽ�RSSI�жϣ��ж�ADC����1�β�ƽ�������¶Ȳ����ֶ�����ն˹⹦�ʣ� 
RSSIMode=3��V4.3a��ʽ����APD�������йܽ�RSSI�жϣ��ж�ADC����4��Ҫƽ�������¶Ȳ����ֶ�����ն˹⹦�ʣ�
*/
#define RSSI_TYPE_DDMI		0	//RSSIMode=0
#define RSSI_TYPE_RSSI1		1   //RSSIMode=1
#define RSSI_TYPE_RSSI2		2   //RSSIMode=2
#define RSSI_TYPE_RSSI3		3   //RSSIMode=3

//laser driver type
#define DiverType_MAX3738  0
#define DiverType_MAX3656  1
#define DiverType_VSC7965  2
#define DiverType_VSC7967  3

//Ϊ�˱��ֳ���������ԣ���ʱ������#define�Ķ��壬�����������
enum eDriverChip
{  
DRIVERCHIP_MAX3738,
DRIVERCHIP_MAX3656,
DRIVERCHIP_VSC7965,
DRIVERCHIP_VSC7967,
DRIVERCHIP_UX2222,  
DRIVERCHIP_UX2228,
DRIVERCHIP_GN2010,
DRIVERCHIP_NT22L33,
DRIVERCHIP_NT25L90,
DRIVERCHIP_UX3328,
DRIVERCHIP_GN1157
};

//======================���в�����ض���======================//
#define CHANNEL_MAX 8 			//���в������ͨ����

//���в������οؼ�ͼ��,ע����ؼ���ͼ���б����
#define TREE_IMAGE_OK     0		//���в������οؼ�ͼ��
#define TREE_IMAGE_WARING 1		//���в������οؼ�ͼ��
#define TREE_IMAGE_ERROR  2		//���в������οؼ�ͼ��

//�����豸��ǩ��ʽ����
#define  LAB_PM         "�⹦�ʼ� %d %d %s"
#define  LAB_ATT    	"˥���� %d %s" 
#define  LAB_ATT_MAI    "����·˥���� %d %s %d"
#define  LAB_ATT_REF    "�ο���·˥���� %d %s %d"
#define  LAB_SEVB       "���԰� %d %s"
#define  LAB_SEVB_LIGHT "��Դ %d %s"  
#define  LAB_BERT       "������ %d %s"
#define  LAB_BERT_10G    "10G������ %d %s"  
#define  LAB_SPECTRUM   "������ %d %s %s"
#define  LAB_DCA        "ʾ���� %d %s %s"
#define  LAB_FIBER      "�������� %s"
#define  LAB_SW         "�⿪�� %d %d %d %s"
#define  LAB_CLOCK      "ʱ���л��� %d %s %d"
#define  LAB_EXT_BERT   "�ⲿ������ %d %s" 
//======================���в�����ض���======================//

//�߳���
int ThreadLock;                //�����·�л�
int ThreadLock_ModuleSN;       //����ɨ��ģ��SN
int ThreadLock_SuperCMD;		//����SuperMaster����
int ThreadLock_JW8504;			 //���ڶ�ͨ��˥���� added by lxf
int Thread_PSS_PowerPeter;	   //����PSS��ͨ���⹦�ʼ��л�  added by lxf  

//��Ϣ����ɫ��
static int COLOR_CHAN[CHANNEL_MAX]={VAL_GREEN,   VAL_YELLOW,   VAL_CYAN,   VAL_MAGENTA,
							        VAL_DK_GREEN,VAL_DK_YELLOW,VAL_DK_CYAN,VAL_DK_MAGENTA};
//======================���в�����ض���======================//

/***************������صı���*************************/
//for evb
//int USB_Handle;		//���ڵ���װ��evb���豸��������ܻ������evb1��evb5�� 
//int EVB5_Handle;
int EVB6_Handle; 
int EVB8_Handle;
int inst_EVB16;
int inst_EVB22;
int INST_EVB[CHANNEL_MAX];
int INST_EVB_Light[CHANNEL_MAX];
int inst_SEVB0027_4001; 

//for power meter  
ViSession inst_OLP18C;
ViSession inst_HP8153A; 
ViSession inst_MT9810; 
//ViSession inst_PMSII;  
ViSession instr_3700C;			/*3700C���**Eric.Yao*/  
ViSession instr_6810B; 			/*6810B���**Eric.Yao*/ 
ViSession INST_PM[CHANNEL_MAX];

//for DCA
ViSession inst_CSA8000;
ViSession inst_Ag86100; 

//for BERT
//int	GBERT_USBHandle;
//int GBERT_ErrorCode;
//int	SBERT_USBHandle; 
int INST_BERT[CHANNEL_MAX]; 
int INST_BERT_10G[CHANNEL_MAX]; 


ViSession inst_Ag86130A; 
ViSession inst_E5020;
ViSession inst_TG1B1A; 
//int	inst_B2BERT; 
//int	inst_SBERT2500; 
//int inst_SBERT10000;
//int inst_GBERT10000;

#define  SBERT_CAL   0.6
int INST_BERT[CHANNEL_MAX]; 

//for ATT  
//ViSession inst_HP8156A;
//ViSession inst_OLA55M;
//ViSession inst_HP8157A; 
//ViSession inst_FVA3100;  
int EVOACOM; 
//int	inst_GVPM;  
int INST_ATT[CHANNEL_MAX];
int INST_ATT_MAIN[CHANNEL_MAX]; //˫·˥�����е�����·˥����
int INST_ATT_REF[CHANNEL_MAX]; //˫·˥�����еĲο���·˥����

//for ������
ViSession inst_Ag86142;
ViSession Inst_AQ633X;
ViSession Inst_MS9740; 
ViSession Inst_AQ637X;


#define PowMeter_TYPE_NONE 	    -1
#define PowMeter_TYPE_OLP18C 	0
#define PowMeter_TYPE_HP8153A 	1  
#define PowMeter_TYPE_MT9810 	2
#define PowMeter_TYPE_PMSII 	3 
#define PowMeter_TYPE_EVBPD 	4 
#define PowMeter_TYPE_6810B		5		/*�����⹦�ʼ�**Eric.Yao*/
#define PowMeter_TYPE_3700C		6		/*�������������**Eric.Yao*/
#define PowMeter_TYPE_LIV20		7		/*����LIV2.0**Eric.Yao*/ 
#define PowerMeter_TYPE_PSS_OPM 8     /*��������˼���ʼ�**xiaofangli*/   

#define DCA_TYPE_NONE 			-1
#define DCA_TYPE_CSA8000		0
#define DCA_TYPE_Ag86100 		1 
#define DCA_TYPE_SEVB22 		2 
 
#define BERT_TYPE_NONE			-1   
#define BERT_TYPE_GBERT			0
#define BERT_TYPE_Ag86130 		1  
#define BERT_TYPE_OptoBERT 		2  
#define BERT_TYPE_SBERT 		3  
#define BERT_TYPE_E5020			4  
#define BERT_TYPE_B2BERT		5  
#define BERT_TYPE_SBERT2500     6
#define BERT_TYPE_TG1B1A		7
#define BERT_TYPE_SBERT10000    8  
#define BERT_TYPE_GBERT10000    9 

#define ATT_TYPE_NONE   		-1  
#define ATT_TYPE_HP8156A   		0
#define ATT_TYPE_OLA55M	   		1
#define ATT_TYPE_HP8157A		2
#define ATT_TYPE_FVA3100		3  
#define ATT_TYPE_EVOA			4
#define ATT_TYPE_GVPM			5
#define ATT_TYPE_JW8504			6   /*�����ѻ�˥����**xiaofangli*/ 

#define LASER_TYPE_FP			0
#define LASER_TYPE_DFB			1

#define SEVB_TYPE_NONE	   		-1  
#define SEVB_TYPE_SEVB1			1
#define SEVB_TYPE_SEVB2			2
#define SEVB_TYPE_SEVB3			3   
#define SEVB_TYPE_SEVB4			4 
#define SEVB_TYPE_SEVB5			5
#define SEVB_TYPE_SEVB6			6 
#define SEVB_TYPE_SEVB11		11
#define SEVB_TYPE_SEVB12		12
#define SEVB_TYPE_SEVB27     	27 
#define SEVB_TYPE_SEVB27_4001	2701 
#define SEVB_TYPE_B2BERT		101  

#define SPECTRUM_TYPE_NONE		-1
#define SPECTRUM_TYPE_AG8614X	0
#define SPECTRUM_TYPE_AQ633X 	1
#define SPECTRUM_TYPE_MS9740 	2
#define SPECTRUM_TYPE_AQ637X    3

#define FIBER_TYPE_NONE	   		-1  
#define FIBER_TYPE_FIBER	   	0

#define SW_TYPE_NONE	   		-1  
#define SW_TYPE_FSW			    0      //����34���⿪��
#define SW_TYPE_COFFSW		    1      //��������⿪��
#define SW_TYPE_10G		    	2      //����10G�⿪��
#define SW_TYPE_JHFSW			3	   //�Ϻ��ѻ۹⿪��
#define SW_TYPE_COFFSW02		4      //��������⿪��

//ʱ���л�
#define CLOCK_TYPE_NONE	   		 	-1  
#define CLOCK_TYPE_SEVB0027_4001	0
/***************���� ������صı���********************/

//�����ն�У׼�����ṹ��
struct  struRxCal 
{
BOOL 	RxCal_Flag;
char	RxCal_FileName[MAX_PATHNAME_LEN]; 
double 	RxCal_Power_In;
double 	RxCal_Power_Min; 
double 	RxCal_Power_Max;
double 	RxCal_Power; 
double 	RxCal_Power_Array[4];
//���в���У׼���ݶ���
BOOL flag[CHANNEL_MAX];
float power_in[CHANNEL_MAX];
float power_min[CHANNEL_MAX];
float power_max[CHANNEL_MAX];
float power[CHANNEL_MAX];
float power_array[CHANNEL_MAX*4];
};

//���巢��У׼�����ṹ��
struct  struTxCal 
{
BOOL 	TxCal_Flag;
char	TxCal_Power_FileName[MAX_PATHNAME_LEN]; 
double 	TxCal_Power_ST;
double 	TxCal_Power_Min; 
double 	TxCal_Power_Max;
double 	TxCal_Power; 
double 	TxCal_Power_Array[4];  
char	TxCal_ER_FileName[MAX_PATHNAME_LEN]; 
double 	TxCal_ER_ST;
double 	TxCal_ER_Min; 
double 	TxCal_ER_Max;
double 	TxCal_ER; 
double 	TxCal_ER_Array[4]; 
float   TxCal_Er_Slope;			//ʾ����ERУ׼����
float   TxCal_Er_Offset;  		//ʾ����ERУ׼����
BOOL flag[CHANNEL_MAX];
float power_st[CHANNEL_MAX];
float power_min[CHANNEL_MAX];
float power_max[CHANNEL_MAX];
float power[CHANNEL_MAX];      //У׼ֵ
float power_array[CHANNEL_MAX*4];
};

struct struTxCheck 
{
char  sn[50];	  //���ģ�����к�
float Power_Min;  //���ģ��ϵͳ��������
float Power_Max;  //���ģ��ϵͳ��������
float Power_Sys;  //���ģ��ϵͳ����
char  sn_array[CHANNEL_MAX][50];
float power_min[CHANNEL_MAX];
float power_max[CHANNEL_MAX];
float power_sys[CHANNEL_MAX];
};

/****************** Visa ***********************************/
extern BOOL Visa_Write(ViSession vi, ViBuf buf); 
extern BOOL Visa_OPC(ViSession vi, int Timeout);  //Timeout s
extern BOOL Visa_Query(ViSession vi, ViBuf buf); 
/******************end for Visa ****************************/

enum eChiefChip
{  
CHIEFCHIP_ADUC7020,
CHIEFCHIP_DS1856,
CHIEFCHIP_TINY13,
CHIEFCHIP_MEGA88,
CHIEFCHIP_ADUC7023,
CHIEFCHIP_UX3328,
CHIEFCHIP_DS4830
};

enum eAmplifierChip
{  
AMPLIFIERCHIP_DEFAULT,
AMPLIFIERCHIP_SY88933V,
AMPLIFIERCHIP_SY88903AL,
AMPLIFIERCHIP_SY88923
};

enum eMCU_CMD_TYPE
{  
MCU_CMD_DEFAULT,		//Ĭ�Ϸ��ʽӿڣ����Լ�ͨ��I2C��ַ����
MCU_CMD_SUPERMASTER		//ֻ��ͨ��supermaster�ӿڷ���
};

int ProductLineID;						//����ID 

#define PRODUCTLINE_CD			0		//��ʾ�ɶ�
#define PRODUCTLINE_FZ			1		//��ʾ����
#define PRODUCTLINE_HJ			2		//��ʾ�ɶ��Ƽ� 

enum eATE_ERR_CODE
{  
ERR_BOSA_TEST_IOP   		=-101,
ERR_BOSA_TEST_IO    		=-102,
ERR_BOSA_TEST_SENS  		=-103,
ERR_BOSA_TEST_GAIN  		=-104,
ERR_BOSA_TEST_VBR			=-105,
ERR_BOSA_TEST_IDARK			=-106, 
ERR_BOSA_TUNE_VAPD  		=-107, 
ERR_BOSA_TEST_SE       		=-108,
ERR_BOSA_START_BERT			=-109,
ERR_BOSA_READ_ERRBIT		=-110,
ERR_BOSA_APD_VBR			=-111,
ERR_BOSA_FQC_APD			=-112,
ERR_BOSA_FQC_LIV			=-113,
ERR_BOSA_FQC_ROSA			=-114,
ERR_BOSA_20Fiberlink		=-115, 
ERR_BOSA_TRACKERR_LOW	    =-116,
ERR_BOSA_TRACKERR_HIGH      =-117,
ERR_BOSA_TEST_POCMP			=-118,
ERR_BOSA_TEST_ITH			=-119, 
ERR_BOSA_TEST_VFIO			=-120, 
ERR_BOSA_TEST_IMIO			=-121, 
ERR_BOSA_TEST_POIO			=-122, 
ERR_BOSA_TEST_VFPO			=-123, 
ERR_BOSA_TEST_IOPO			=-124, 
ERR_BOSA_TEST_IMPO			=-125, 
ERR_BOSA_TEST_SEPO			=-126,
ERR_BOSA_TEST_TEMPER		=-127,
ERR_BOSA_NO_DEFINE			=-199, 
ERR_TRA_APD_EN				=-201, 
ERR_TRA_APD_VBR				=-202, 
ERR_TRA_APD_VBR_TARGET		=-203, 
ERR_TRA_APD_VBR_TEMPER		=-204, 
ERR_TRA_APD_VBR_COUNT		=-205, 
ERR_TRA_PASSWORD			=-207, 
ERR_TRA_I2C					=-208, 
ERR_TRA_OVER				=-212,   
ERR_TRA_SENS				=-213,   
ERR_TRA_RXCAL				=-214,  
ERR_TRA_SET_APD_PROTECT		=-215, 
ERR_TRA_DOWN_ONU			=-216,  
ERR_TRA_DOWN_GBE			=-217, 
ERR_TRA_DOWN_SDH			=-218, 
ERR_TRA_APD_WELD			=-220,
ERR_TRA_APD_5uA				=-221, 
ERR_TRA_TX_WELD				=-222,
ERR_TRA_NO_DEFINE			=-299,  
ERR_TUN_READ_INNERSN		=-301,
ERR_TUN_TEST_RAPD   		=-302,
ERR_TUN_TEST_TEMPER 		=-303,
ERR_TUN_TEST_STATION		=-304,
ERR_TUN_SDA					=-305,
ERR_TUN_TEST_OVER			=-306, 
ERR_TUN_TEST_SENS_START		=-307, 
ERR_TUN_AOP	        		=-308,
ERR_TUN_P0					=-309, 
ERR_TUN_P1					=-310, 
ERR_TUN_DAC_255or0			=-311,
ERR_TUN_ER					=-312,
ERR_TUN_TEST_O_EYE			=-313,
ERR_TUN_TEST_O_MASK			=-314,
ERR_TUN_TEST_CURRENT		=-315,
ERR_TUN_TEST_SPECTRUM		=-316,
ERR_TUN_TEST_SENS			=-317,
ERR_TUN_CAL_TXPOWER			=-318, 
ERR_TUN_CAL_RXPOWER			=-319,
ERR_TUN_SAVE_DATA			=-320,
ERR_TUN_LD_SE_LOW			=-321, 
ERR_TUN_CLEAR_TXFAIL        =-322, 
ERR_TUN_CAL_RX_MONOTONY     =-323,
ERR_TUN_BIASCURR            =-324, 
ERR_TUN_TXCROSS             =-325,
ERR_TUN_TEST_RXRESPON		=-326,
ERR_TUN_TEST_LOS			=-327,
ERR_TUN_TXRMSJ			    =-328,
ERR_TUN_APD                 =-329,
ERR_TUN_EAVOL               =-330,
ERR_TUN_EATEMP              =-331,
ERR_TUN_BIAS_MAX            =-332, 
ERR_TUN_NO_DEFINE			=-399,
ERR_TES_READ_INNERSN		=-401, 
ERR_TES_TEST_TEMPER 		=-402, 
ERR_TES_AOP					=-403, 
ERR_TES_ER					=-404, 
ERR_TES_ER_LUT				=-405, 
ERR_TES_SD					=-406, 
ERR_TES_TEST_O_EYE			=-407,
ERR_TES_TEST_SPECTRUM		=-408, 
ERR_TES_TEST_OVER			=-409, 
ERR_TES_TEST_STATION		=-410,  
ERR_TES_TEST_TXDIS			=-411, 
ERR_TES_TEST_O_MASK			=-412,    
ERR_TES_INPUT_SN			=-413, 
ERR_TES_TEST_INIT			=-414, 
ERR_TES_CAL_TXPOWER			=-415, 
ERR_TES_CAL_RXPOWER			=-416, 
ERR_TES_TEST_CURRENT        =-417,
ERR_TES_SAVE_EEPROM			=-418, 
ERR_TES_REPEAT				=-419, 
ERR_TES_STABILITY			=-420,
ERR_TES_FIBER_LINK			=-421, 
ERR_TES_SAVE_DATA			=-422, 
ERR_TES_TEST_SENS_START		=-423, 
ERR_TES_TEST_SENS			=-424, 
ERR_TES_ONU_TXSD			=-425,  
ERR_TES_CLEAR_TXFAIL        =-426,  
ERR_TES_CAL_RX_28dBm        =-427, 
ERR_TES_FIBLINKandSPECTRUM	=-428,
ERR_TES_CAL_TEMPERATURE		=-429,
ERR_TES_AOP_MAX				=-430,
ERR_TES_CAL_RX_POWER2		=-431, 	//��ͬ���ʵ����3�Σ������쳬��2dB
ERR_TES_CAL_RX_POW			=-432, 	//�����ն˼�ع��ʵ��쳣
ERR_TES_AOP_TC				=-433,	//TCǰ��AOP�������
ERR_TES_SE_MIN				=-434,	//SEС����Сֵ
ERR_TES_SE_HALFROOM			=-435,	//����SEС�ڳ��µ�һ�� 
ERR_TES_SE_MOD80_DAC		=-436,  //����80�ȵ��Ƶ�������Ԥ��ֵ
ERR_TEST_TXDIS_FAULT	    =-437,  //����TxDisʱ ����TxFault�쳣
ERR_TES_TE			        =-438,	//����TE���� 
ERR_UX3328_SUM_UPDATE       =-439,  //ux3328����A2У��λ�쳣
ERR_UX3328_SUM_CHECK        =-440,  //ux3328���A2У��λ�쳣
ERR_UX3328_E2_CHECK         =-441,  //ux3328�ϵ���EEPROM��־λ�쳣
ERR_TSSI         			=-442,  //TSSI����쳣 
ERR_TES_NO_DEFINE			=-499, 
ERR_SYS_LINK_FAIL			=-701, 
ERR_SYS_LOST				=-702,
ERR_SYS_LOST_UP				=-703, 
ERR_SYS_LOST_DOWN			=-704, 
ERR_SYS_FIBER_LINK			=-705, 
ERR_SYS_NO_DEFINE			=-799, 
ERR_EEP_MON_TX				=-801,
ERR_EEP_MON_RX				=-802,
ERR_EEP_SHORT_VCC			=-803, 
ERR_EEP_SHORT_VAPD			=-804, 
ERR_EEP_SHORT_GND			=-805, 
ERR_EEP_MON_BIAS			=-806,  
ERR_EEP_MON_VCC				=-807,  
ERR_EEP_MON_LOS				=-808, 
ERR_EEP_MON_SD				=-809, 
ERR_EEP_MON_TEMPER			=-810, 
ERR_EEP_TX_FAULT			=-811,   
ERR_EEP_TX_DISABLE			=-812,   
ERR_EEP_NO_DEFINE			=-899, 
ERR_K3_CHECK_MAC   			=-901,
ERR_K3_CHECK_IAD_MAC  		=-902,
ERR_K3_DOWN_FIRMWARE  		=-903,
ERR_K3_PONSYS_INIT   		=-904,
ERR_K3_CHECK_TXPOW   		=-905,
ERR_K3_UPDATE_MAC   		=-906,
ERR_K3_MODULE_SN   			=-907, 
ERR_K3_WRITE_IADMAC   		=-908, 
ERR_K3_LOGIN       			=-909,
ERR_K3_DOWN_IMG       		=-910,
ERR_K3_FEED_VOL       		=-911,
ERR_K3_TEST_VOL       		=-912,
ERR_K3_GET_VERSION     	 	=-913,  
ERR_K3_CHECK_HARVER      	=-914, 
ERR_K3_CHECK_SOFVER      	=-915, 
ERR_K3_CHECK_HARLOGO     	=-916,
ERR_K3_SET_PROTOCOL  	 	=-917,
ERR_K3_TEST_VOICE    	 	=-918,
ERR_K3_CHECK_STATUS  	 	=-919,
ERR_K3_ONLINE               =-920,
ERR_K3_GET_ID               =-921,
ERR_K3_SET_AU               =-922,
ERR_K3_CHECK_ONLINE         =-923,
ERR_K3_STREAM_TEST          =-924,
ERR_K3_NO_DEFINE   			=-999, 
ERR_NO_DEFINE				=-1
} eATE_ERR_CODE;

enum eRSSI_TIME
{  
RSSI_TIME_N1N3,
RSSI_TIME_N2, 
RSSI_TIME_L2L3, 
RSSI_TIME_L1L2   
};

/*
�ͻ����������0��ʾ��׼Ʒ
1��ʾ��AL��������Ҫ���������Ƿ���AL��Ʒ��innersn�Ƿ���Ҫ���⴦��������Ʒ��ʱȫ������Ϊ��׼Ʒ
*/
enum eCUSTOMER
{  
CUSTOMER_STANDARD, 	//��׼��Ʒ    
CUSTOMER_AL,		//ALCATEL���Ʋ�Ʒ
CUSTOMER_AP,		//Alphion���Ʋ�Ʒ
CUSTOMER_01,		//01 ���Ʋ�Ʒ
CUSTOMER_OP,		//OP ���Ʋ�Ʒ
CUSTOMER_ZT,		//ZT ����
CUSTOMER_FH			//FH ����
};

//**************��·У׼��춨��**************//
int phCaliT;  
int phCaliLimit;  			//��·У׼�������ý���
int phCheckT;				//���˹�·������  

//#define CALIBRATION_NO	5
#define CALIBRATION_MAX 120       //120���ӣ�2Сʱ
#define CALIBRATION_MAX_OSA 240   //240���ӣ�4Сʱ

enum enum_CALIBRATION_TYPE
{
CALI_TYPE_RX,					//�ն˹�·У׼��GPON OLT ATEʹ����������ն˲ο���·У׼  	
CALI_TYPE_TX,
CALI_TYPE_TX_CHECKUP,
CALI_TYPE_RX_MAINLOCK,			//�ն�����·����ģʽУ׼��GPON OLT ATEʹ��
CALI_TYPE_RX_MAINFREE,			//�ն�����·��ͨģʽУ׼��GPON OLT ATEʹ��
CALI_TYPE_RX_OLT,				//OLT�ն˹�·У׼��EPON ONU ATEʹ��  
CALI_TYPE_LIV_AOP,
CALI_TYPE_LIV_ITH
}; 

static ViString CALI_TYPE[] = {
"RX_CALI", 
"TX_CALI",
"TX_CHECKUP",
"RX_CALI_MAINLOCK", 
"RX_CALI_MAINFREE",
"RX_CALI_OLT",
"LIV_AOP",
"LIV_ITH"
};

static ViString LUK_NAME[] = {
"LUK", 
"BASE"
};

//**************��·У׼��춨��**************//

//======================������涨��======================// 
int showphID;   //0=��������棬1=�߼����ܽ��棬2=У׼���ý���, 3=����û��� 4=��׼ģ�������޽���, 5=�������ý��� 

#define License_Flag_INIT    -1  //��������ʱ�ĳ�ʼֵ
#define License_Flag_panMain  0
#define License_Flag_panADV   1
#define License_Flag_panCALI  2 
#define License_Flag_USER  	  3
#define License_Flag_panPara  5 
#define License_Flag_panInst  6
#define License_Flag_panCtr   7	 
//======================������涨��======================// 

//���붨��
static ViString PassWord[] = {
"0x53,0x4F,0x45,0x42",
"0xD3,0x4F,0x45,0x42", 
"00,00,0x10,0x11"
};
#define PW_534F4542   1	  
#define PW_D34F4542   2
#define PW_00001011   3

//nt25L90�Ĵ�������
#define NT25L90_REG_NUMBER  21
#define NT25L90_MON_NUMBER  16

static ViString NT25L90_REG_NAME[NT25L90_REG_NUMBER] = {
"TXSET0",
"TXSET1",
"IMODSET",
"TCSTART",
"TSLOPE",
"SPARE",
"APCSET",
"BIASINIT",
"APCCTRL0",
"MDMAX",
"BIASMAX", 
"RXSET0", 
"RXSET1", 
"LOSLEVEL", 
"CONTROL0", 
"BIASDAC MSB", 
"BIASDAC LSB", 
"WATCHDOG", 
"STATUS0", 
"STATUS1", 
"CONTROL1" 
};

static ViString NT25L90_MON_NAME[NT25L90_MON_NUMBER] = {
"TEMP MSB",
"TEMP LSB",
"TX_POWER MSB",
"TX_POWER LSB",
"TX_BIAS MSB",
"TX_BIAS LSB",
"TX_MOD MSB",
"TX_MOD LSB",
"RX_POWER MSB",
"RX_POWER LSB", 
"VDD_TX MSB", 
"VDD_TX LSB", 
"VDD_RX MSB", 
"VDD_RX LSB", 
"VDD_DIG MSB", 
"VDD_DIG LSB" 
};

void Update_RESULTSInfo(int panel, int control, unsigned char *resultstr, BOOL bInsert);
void set_led_status(int panel, int control, int status_led, int status_cursor);
void Insert_Info(int panel, int control, unsigned char *resultstr);

#ifdef __cplusplus
    }
#endif
	
#endif  	

