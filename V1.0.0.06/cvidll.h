/****************************************************************************
 *
 * File:                cvidll.h
 *
 * Author:              SUPERXON LTD.(Roger Li)
 *
 * Description:         �Զ�����ͨ�ú�����̬���ļ�
 *
 * Time:                2012-3-22 
 *
 * 
�汾��ʷ

======================================
����ʱ�䣺2015-03-25
���°汾��V1.1.2.42
�����ˣ�  Eric.Yao
1.����MyDLLReadModuleSN������֧��8077Э���Ʒ��ģ�����룻

======================================
����ʱ�䣺2015-04-22      
���°汾��V1.1.2.41
�����ˣ�  Eric.Yao
1����ӿͻ�DEONET��CustomerNameArray

======================================
����ʱ�䣺2015-03-25
���°汾��V1.1.2.40
�����ˣ�  Eric.Yao
1.MyDLL_GET_SEVB_SN�������Ӷ�EVB31deSN��ȡ֧�֣�

======================================
����ʱ�䣺2015-02-06
���°汾��V1.1.2.39
�����ˣ�  Eric.Yao
1.����MyDLLGetInnerSN�������ܣ���0xA2��240-255�ֽ�û��InnerSNʱ�������0xA0��240-255�ֽڶ�ȡInnerSN��

======================================
����ʱ�䣺2014-07-08
���°汾��V1.1.2.38
�����ˣ�  Eric.Yao
1����ӿͻ�MI��CustomerNameArray
2����Ӳ�Ʒ����XGPON_ONU��ProductTypeArray

======================================
����ʱ�䣺2014-01-20
���°汾��V1.1.2.37
�����ˣ�  Eric.Yao
1����ӿͻ�MC��CustomerNameArray

======================================
����ʱ�䣺2013-12-27
���°汾��V1.1.2.36
�����ˣ�  Eric.Yao 
��������
1���޸����ݿ��˻������룻

======================================
����ʱ�䣺2013-10-12
���°汾��V1.1.2.35
�����ˣ�  Eric.Yao 
��������
1���������߱�ʾ����PRODUCTLINE_OFA��ʾ�ɶ�걸�����

======================================
����ʱ�䣺2013-10-11
���°汾��V1.1.2.34
�����ˣ�  Eric.Yao
1����ӿͻ�AD��CustomerNameArray

======================================
����ʱ�䣺2013-07-18
���°汾��V1.1.2.33
�����ˣ�  Roger
1.���SEVB0027-4001��USB���кŶ�ȡ����

======================================
����ʱ�䣺2013-05-02
���°汾��V1.1.2.32
�����ˣ�  Sally
1.��ȡinnersn�����У��轫A2�������л���Ĭ��table

======================================
����ʱ�䣺2013-03-11
���°汾��V1.1.2.31
�����ˣ�  Eric.Yao
1����ӿͻ�DZ��CustomerNameArray
2��MyDLL_8472_Getxxxϵ�к������þֲ�union����

======================================
����ʱ�䣺2013-02-03
���°汾��V1.1.2.30
�����ˣ�  Eric.Yao
1����ӿͻ�AP��CustomerNameArray

======================================
����ʱ�䣺2013-01-11
���°汾��V1.1.2.29
�����ˣ�  Sally
1��ProductTypeArray������SFP_PLUS��Ʒ����

======================================
����ʱ�䣺2012-12-13
���°汾��V1.1.2.28
�����ˣ�  Roger
1����Ӻ���MyDLL_DBOA_Init
2��ɾ������
#define  DB_ID "superxon"
#define  DB_PW "superxontestdata"
#define  DBOA_IP  "172.16.0.75"
#define  DBOA_IP2 "172.30.0.75"   //192.168.30.75
#define  DBOA_DB  "sup_web"
#define  DBOA_ID  "root"   
#define  DBOA_PW  "123"   

======================================
����ʱ�䣺2012-11-29
���°汾��V1.1.2.27
�����ˣ�  Eric.Yao
1����ӿͻ�FLEX(ΰ����)��CustomerNameArray

======================================
����ʱ�䣺2012-11-27
���°汾��V1.1.2.26
�����ˣ�  Roger
1�����WNetAddConnection2��������1219�������Ĵ���

======================================
����ʱ�䣺2012-09-29
���°汾��V1.1.2.25
�����ˣ�  Sally
1������change_batch_osa��reform_osa����
2��MyDLLDeleteSpace�ѱ�����MyDLL_TRIM����

======================================
����ʱ�䣺2012-09-18
���°汾��V1.1.2.24
�����ˣ�  Sally
1������������ͳһ��Ϊ��д

======================================
����ʱ�䣺2012-09-10
���°汾��V1.1.2.23
�����ˣ�  Eric.Yao
1����ӿͻ�ZY��CustomerNameArray

======================================
����ʱ�䣺2012-07-06
���°汾��V1.1.2.22
�����ˣ�  Roger 
1�����OQA����
2����Ӻ���MyDLLGetPCBA
****************************************************************************/

#ifndef _CVIDLL_H_
#define _CVIDLL_H_ 

#include <cvidef.h>
#include <windows.h> 
#include <ansi_c.h>
#include <utility.h>
#include <winnetwk.h>    
#include <cvirte.h>		
#include <userint.h>
#include "global.h"
/*Here are the function prototypes suitable for including in a program which uses the dll. */
 
#define  VER	"1.1.2.35"

/*************************************************************for base64*************************************************************/
#define BASE64_PAD64 '='

static char base64_alphabet[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                          'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                          'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a',
                          'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                          'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
                          't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1',
                          '2', '3', '4', '5', '6', '7', '8', '9', '+',
                          '/'};
                          
static unsigned char base64_suffix_map[256] = 
	{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255,  62, 255, 255, 255,  63,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,
     255, 255, 255, 255, 255,  0,   1,    2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
     15,   16,  17,  18,  19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255, 255,  26,  27,  28,
     29,   30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
     49,   50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}; 
static char cmove_bits(unsigned char src, unsigned lnum, unsigned rnum);
/*************************************************************end for base64*************************************************************/ 

/*************************************************************end for getdiskn************************************************************/   
//  Max number of drives assuming primary/secondary, master/slave topology
#define  MAX_IDE_DRIVES  		4
#define  IDENTIFY_BUFFER_SIZE  	512

//  IOCTL commands
#define  DFP_GET_VERSION          0x00074080
#define  DFP_SEND_DRIVE_COMMAND   0x0007c084
#define  DFP_RECEIVE_DRIVE_DATA   0x0007c088

#define  FILE_DEVICE_SCSI              0x0000001b
#define  IOCTL_SCSI_MINIPORT_IDENTIFY  ((FILE_DEVICE_SCSI << 16) + 0x0501)
#define  IOCTL_SCSI_MINIPORT 0x0004D008  //  see NTDDSCSI.H for definition

//  Valid values for the bCommandReg member of IDEREGS.
#define  IDE_ATAPI_IDENTIFY  0xA1  //  Returns ID sector for ATAPI.
#define  IDE_ATA_IDENTIFY    0xEC  //  Returns ID sector for ATA.

//  GETVERSIONOUTPARAMS contains the data returned from the 
//  Get Driver Version function.
typedef struct _GETVERSIONOUTPARAMS
{
   BYTE bVersion;      // Binary driver version.
   BYTE bRevision;     // Binary driver revision.
   BYTE bReserved;     // Not used.
   BYTE bIDEDeviceMap; // Bit map of IDE devices.
   DWORD fCapabilities; // Bit mask of driver capabilities.
   DWORD dwReserved[4]; // For future use.
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;

//  IDE registers
typedef struct _IDEREGS
{
   BYTE bFeaturesReg;       // Used for specifying SMART "commands".
   BYTE bSectorCountReg;    // IDE sector count register
   BYTE bSectorNumberReg;   // IDE sector number register
   BYTE bCylLowReg;         // IDE low order cylinder value
   BYTE bCylHighReg;        // IDE high order cylinder value
   BYTE bDriveHeadReg;      // IDE drive/head register
   BYTE bCommandReg;        // Actual IDE command.
   BYTE bReserved;          // reserved for future use.  Must be zero.
} IDEREGS, *PIDEREGS, *LPIDEREGS;

//  SENDCMDINPARAMS contains the input parameters for the 
//  Send Command to Drive function.
typedef struct _SENDCMDINPARAMS
{
   DWORD     cBufferSize;   //  Buffer size in bytes
   IDEREGS   irDriveRegs;   //  Structure with drive register values.
   BYTE bDriveNumber;       //  Physical drive number to send 
                            //  command to (0,1,2,3).
   BYTE bReserved[3];       //  Reserved for future expansion.
   DWORD     dwReserved[4]; //  For future use.
   BYTE      bBuffer[1];    //  Input buffer.
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;

// Status returned from driver
typedef struct _DRIVERSTATUS
{
   BYTE  bDriverError;  //  Error code from driver, or 0 if no error.
   BYTE  bIDEStatus;    //  Contents of IDE Error register.
                        //  Only valid when bDriverError is SMART_IDE_ERROR.
   BYTE  bReserved[2];  //  Reserved for future expansion.
   DWORD  dwReserved[2];  //  Reserved for future expansion.
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;

// Structure returned by PhysicalDrive IOCTL for several commands
typedef struct _SENDCMDOUTPARAMS
{
   DWORD         cBufferSize;   //  Size of bBuffer in bytes
   DRIVERSTATUS  DriverStatus;  //  Driver status structure.
   BYTE          bBuffer[1];    //  Buffer of arbitrary length in which to store the data read from the drive.
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;

BYTE 	IdOutCmd [sizeof (SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];
BOOL 	DoIDENTIFY (HANDLE, PSENDCMDINPARAMS, PSENDCMDOUTPARAMS, BYTE, BYTE, PDWORD);
char 	*ConvertToString (DWORD diskdata [256], int firstIndex, int lastIndex);
/*************************************************************end for getdiskn************************************************************/

//���幤�չ��̽ṹ�� 
struct  struProcessLOG	  
{
char	PN[50];				//PN
char	SN[50];		   		//ģ������к�
char	Operator[10];		//����Ա���
int		ResultsID;			//Results���index
char   	Log_Action[50];		//���̶���
char	Action_Time[50];	//��¼ʱ��
char	Parameter[50];		//�������͵Ķ���
char	P_Value[50];		//����ֵ
char	Comments[4000];		//��ע 
char	SoftVersion[50];	//����汾
char	StationID[50];		//��λ���������  
int		RunTime;			//���Գ�������ʱ��
int		RunCount;			//��ǰ��λ���д��� 
} my_struProcessLOG;

extern int 	 __stdcall DLLIMPORT MyDLLFloattoInt(float numeric);						//����������ת������׼32λ����
extern float __stdcall DLLIMPORT MyDLLInttoFloat(int numeric);  						//��׼32λ��������ת����������

/*******************************************************************/   
/*
�� �� �� �ƣ�	MyDLLAddConnection  
�� �� �� ����	��������ӳ���̷� 
�� �� ˵ ����	LocalName:	�̷�  ����"H:" 
             	RemotePath: ���繲��·�� \\192.168.1.11\test1\test2 ע��Ҫ��ת���ַ�
				lpUserName���û���
				lpPassword������
����ֵ ˵����	���ӳɹ�����0������Ϊ��0  
/*******************************************************************/  
extern int 	__stdcall DLLIMPORT MyDLLAddConnection(IN LPSTR LocalName, IN LPSTR RemotePath, IN LPCSTR lpUserName, IN LPCSTR lpPassword);

extern int 	__stdcall DLLIMPORT MyDLLCancelConnection(IN LPSTR LocalName); 
extern int  __stdcall DLLIMPORT MyDLLBase64_encode(const char *data_in, char *data_out);
extern int  __stdcall DLLIMPORT MyDLLBase64_decode(const char *data_in, char *data_out); 
extern int  __stdcall DLLIMPORT MyDLLReadPhysicalDrive(char *HardDriveSerialNumber); 	//0:fail;1:OK, ��ȡӲ�������ַ��ֻ��WindowsXPʹ�ù���Vista�����ڰ�ȫԭ����ܻᱻ��ֹ��û�в��Թ�
extern BOOL __stdcall DLLIMPORT MyDLLGetComputerName(char *ComputerName); 				//��ȡ���������
extern BOOL __stdcall DLLIMPORT MyDLLGetServerIP(char *ServerIP);						//REGKEY_HKCU, "Software\\Superxon", "Server" 
extern BOOL __stdcall DLLIMPORT MyDLLGetRegInfo(const char *DiskSN, char *RegInfo);		//���������DiskSN����У���ַ���RegInfo
extern int	__stdcall DLLIMPORT MyDLLCheckLicense(void);
extern BOOL __stdcall DLLIMPORT MyDLLCalSystemDate(void);								//��MyDLLGetServerIP�����л�ȡSQLserver��IP�����ݴ�IP��Ϊϵͳ�ı�׼ʱ��У׼
extern void	__stdcall DLLIMPORT MyDLLCheckSN(char *s);									//��������ַ�ɾ��(a-z, A-Z, 0-9),����SNת��Ϊ��д,֧���ַ�������Ϊ128 
extern void __stdcall DLLIMPORT MyDLLCheckStr_Ex(char *s);								//��������ַ�ɾ��(a-z, A-Z, 0-9),����SNת��Ϊ��д,֧���ַ�������Ϊ128, ����С����.  
extern void __stdcall DLLIMPORT MyDLLGETPartNumber(int panel, int control);				//��exeĿ¼�µ�data�ļ���PartNumber.txt��ȡPN����ֵ��panelָ���Ľ����control��ring�ؼ���     											
extern void __stdcall DLLIMPORT MyDLLGETPartNumber_ex(int panel, int control, char file[]);			//��ָ���ļ���ȡ�ѡ�;�����ŷָ����Ϣ����ֵ��panelָ���Ľ����control��ring�ؼ���     											
																
//080715:���ע�Ṧ�ܺ���,ʹ���ⲿ�ֺ�������Ҫ�ڹ���Ŀ¼������ļ�license.uir��license.h
/*************************************************************Login*************************************************************/
struct  struLicense
{
char username[50];	//�����û�����
char password[50];	//�����û�����
char power[1024];	//�����û�Ȩ��
} my_struLicense; 

int panelHandle_log; 
int	g_passwordICtrlID;
int panPW;
int g_pwCtrlID_old;
int g_pwCtrlID_new; 
int g_pwCtrlID_new2; 

extern BOOL __stdcall DLLIMPORT MyDLL_License_Load(void); 			//loadע�����
extern BOOL __stdcall DLLIMPORT MyDLL_License_Check(char *username, char *password, char *power);	//���ע����Ϣ���������˺ŵ���Ϣ
extern BOOL __stdcall DLLIMPORT MyDLL_License_Close(void); 			//�ر�ע�����
extern int  __stdcall DLLIMPORT MyDLL_License_Entry_Username(void); //Username������س� 
extern int  __stdcall DLLIMPORT MyDLL_License_Entry_Password(void); //Password������س�  
extern int __stdcall DLLIMPORT MyDLL_License_PW_Open(void); 		//��������½���
extern int __stdcall DLLIMPORT MyDLL_License_PW_Close(void); 		//�ر�������½���
extern int __stdcall DLLIMPORT MyDLL_License_PW_Update(void); 		//�ύ�������
extern void __stdcall DLLIMPORT MyDLL_License_PW_Entry (int control);//��������½���������س�

//090327 ���ע����Ʊ���
#define  LICENSE_ENABLE      1
#define  LICENSE_DISENABLE   0
BOOL	 license_status;
/*************************************************************Login*************************************************************/ 

/*******************************************************************/   
/*090327 ���CRC32У��ֵ���㺯��   
�� �� �� �ƣ�	MyDLLCRC32  
�� �� �� ����	ִ�ж����ݶε�CRC32ѭ������У��  
�� �� ˵ ����	buffer[in]:��У������  
             	size[in]:  ��У�����ݳ���  
����ֵ ˵����	ѭ������У����  
/*******************************************************************/  
extern unsigned long __stdcall DLLIMPORT MyDLLCRC32(unsigned char *buffer, unsigned long size);

extern void __stdcall DLLIMPORT MyDLLFormatDate(char *s);			//ת�����ڸ�ʽ 04-03-2009 13:29:42 ��Ϊ 2009-04-03 13:29:42�� ����д�����ݿ��������ڱȽ�

/*******************************************************************/   
/*090629 ���SEVB���кŶ�ȡ����   
�� �� �� �ƣ�	MyDLL_GET_SEVB_SN  
�� �� �� ����	���������SEVB���ͣ���ֱ�ӽ���ȡ����SEVB��Ÿ�ֵָ���Ŀؼ� 
�� �� ˵ ����	SEVB_TYPE:SEVB���ͣ���ʱ��֧��SEVB1��SEVB5��SEVB6��SEVB12�����ѡ��SEVB12��ֻɸѡ���豸���SBERT��ͷ���豸���к�
             	panel������ID 
				control���ؼ�ID ��֧�ֿؼ�����ring or list
����ֵ ˵����	�������н����0��ʾ�������гɹ���<0��ʾ��������ʧ��  
/*******************************************************************/  
extern int __stdcall DLLIMPORT MyDLL_GET_SEVB_SN(const int SEVB_TYPE, int panel, int control); 

/*******************************************************************/   
/*090820 ���CRC16У��ֵ���㺯��   
�� �� �� �ƣ�	MyDLLCRC16  
�� �� �� ����	ִ�ж����ݶε�CRC16ѭ������У��  
�� �� ˵ ����	buffer[in]:��У������  
             	size[in]:  ��У�����ݳ���  
����ֵ ˵����	ѭ������У����  
/*******************************************************************/  
extern unsigned short __stdcall DLLIMPORT MyDLLCRC16(unsigned char *buffer, unsigned long size);

/*******************************************************************/   
/*
�������ƣ�	MyDLLHEXstrtoINTnum
�������ڣ�	2009-10-28
�������ߣ�	Roger.Li
����������	��16�����ַ�����Ϊ64bit���������֣����֧��16����0~9��A~F��ASCII�ַ�  0xFFFFFFFFFFFFFFFF�� 
����˵����	*str [in] �����ַ���  
            *num [out]ת����� 
����ֵ˵����0�������гɹ�����0�������д���-2�����ַ������ȳ���16���ַ�
/*******************************************************************/  
extern int __stdcall DLLIMPORT MyDLLHEXstrtoINTnum(char *str, unsigned long long *num);

/*******************************************************************/   
/*
�������ƣ�	MyDLL_TRIM
�������ڣ�	2009-12-09
�������ߣ�	Roger.Li
����������	�������ַ����еĿո��ַ�ȥ�����ո����ַ�ǰ�� 
����˵����	*s [in and out] �����ַ�����ת�����  
����ֵ˵����0�������гɹ�����0�������д���
/*******************************************************************/  
extern int __stdcall DLLIMPORT MyDLL_TRIM(char *s);								

//��ȡ����ϵͳʱ��
extern int __stdcall DLLIMPORT MyDLL_GetDateTime(char *DateTime);  
//��ȡ������ϵͳʱ��
extern int __stdcall DLLIMPORT MyDLLGetSystemDate(char *DateTime);

enum enum_LIFE
{   
LIFE_FIBER_STAND,
LIFE_FIBER,
LIFE_SFP,
LIFE_SFF
} LIFE_TYPE; 

#define  FIBER_RUNTIME_MAX   	300 		//����һ���Կ�ʹ�ô������ֵ 
#define  FIBER_RUNTIME_HIS_MAX	1500		//�����ܹ���ʹ�ô������ֵ

#define  SFP_RUNTIME_MAX   		15000 		//SFPһ���Կ�ʹ�ô������ֵ 
#define  SFP_RUNTIME_HIS_MAX	15000		//SFP�ܹ���ʹ�ô������ֵ

#define  SFF_RUNTIME_MAX   		5000 		//SFFһ���Կ�ʹ�ô������ֵ 
#define  SFF_RUNTIME_HIS_MAX	5000		//SFF�ܹ���ʹ�ô������ֵ

/*******************************************************************/   
/*
�������ƣ�	MyDLL_FiberManage_Init
�������ڣ�	2010-05-05
�������ߣ�	Roger.Li
����������	��������fiber����Ƿ����ʹ��
����˵����	hdbc [in] ���ݿ����Ӿ�� 
            *fibersn [int] fiber���
����ֵ˵����0�������гɹ�����0�������д���
/*******************************************************************/  
extern int __stdcall DLLIMPORT MyDLL_FiberManage_Init (int hdbc, char *fibersn);
/*��չ����˵�������������������Ͳ�������չ������Ϊ������Ԫ��ԭ����������չ�������Ա��ְ汾������*/
extern int __stdcall DLLIMPORT MyDLL_FiberManage_Init_EX (int hdbc, char sn[], enum enum_LIFE LIFE_TYPE);

/*******************************************************************/   
/*
�������ƣ�	MyDLL_FiberManage_Run
�������ڣ�	2010-05-05
�������ߣ�	Roger.Li
����������	���������fiber��ŵ�״̬
����˵����	hdbc [in] ���ݿ����Ӿ�� 
            *fibersn [int] fiber���
			*runtime [int] ��ǰ������ʹ�ô���
����ֵ˵����0�������гɹ�����0�������д���
/*******************************************************************/  
extern int __stdcall DLLIMPORT MyDLL_FiberManage_Run (int hdbc, char *fibersn, int *runtime);
extern int __stdcall DLLIMPORT MyDLL_FiberManage_Run_EX (int hdbc, char sn[], int *runtime, enum enum_LIFE LIFE_TYPE); 

/*******************************************************************/   
/*
�������ƣ�	MyDLL_FiberManage_End
�������ڣ�	2010-05-05
�������ߣ�	Roger.Li
����������	�ָ������fiber��ŵ�״̬ ��Active
����˵����	hdbc [in] ���ݿ����Ӿ�� 
            *fibersn [int] fiber���
����ֵ˵����0�������гɹ�����0�������д���
/*******************************************************************/  
extern int __stdcall DLLIMPORT MyDLL_FiberManage_End (int hdbc, char *fibersn);
extern int __stdcall DLLIMPORT MyDLL_FiberManage_End_EX (int hdbc, char sn[], enum enum_LIFE LIFE_TYPE); 

/*******************************************************************/   
/*
�������ƣ�	MyDLLReadModuleSN
�������ڣ�	2010-05-21
�������ߣ�	Roger.Li
����������	��ȡģ������к�
����˵����	handle [in] �豸��� 
            *modulesn [out]   module SN
����ֵ˵����0�������гɹ�����0�������д���
/*******************************************************************/  
extern int __stdcall DLLIMPORT MyDLLReadModuleSN (int handle, char *modulesn);

/*******************************************************************/   
/*
�������ƣ�	MyDLLDeleteSpace
�������ڣ�	2010-06-18
�������ߣ�	Roger.Li
����������	�������ַ����еĿո��ַ�ȫ��ȥ�� 
����˵����	*str [in and out] �����ַ�����ת�����  
����ֵ˵����0�������гɹ�����0�������д���
/*******************************************************************/ 
//MyDLLDeleteSpace�ѱ�����MyDLL_TRIM����
//extern int __stdcall DLLIMPORT MyDLLDeleteSpace(char *str);

extern int __stdcall DLLIMPORT MyDLLGetProductLine (int *ProductLineID);

/***************************�˺Ź����ֶ���***************************************/
#define  ACCOUNT_TYPE_MAX	 7

static ViString AccountTypeArray[] = {
"",
"Operator",
"Firmware",
"Admin",
"Fiber",
"Audit",
"Repair"
};

static ViString AccountTypeArray_chs[] = {
"",
"����Ա",
"�̼�����Ա",
"�߼�����Ա",
"���˹���Ա",
"���Ա",
"ά��Ա"
};
/***************************�˺Ź����ֶ���***************************************/ 

/***************************�豸�����ֶ���***************************************/
#define  INSTRUMENT_TYPE_MAX	 9  

enum enum_INSTR_TYPE
{   
INSTR_TYPE_NONE,
INSTR_TYPE_PM,
INSTR_TYPE_ATT, 
INSTR_TYPE_DCA, 
INSTR_TYPE_BERT, 
INSTR_TYPE_SEVB5, 
INSTR_TYPE_SEVB6,
INSTR_TYPE_SPECTRUM,   
INSTR_TYPE_FIBER
}; 

static ViString InstrumentTypeArray[] = {
"",
"�⹦�ʼ�",
"��˥����",
"ʾ����",
"������",
"SEVB5",
"SEVB6",
"������",   
"����"
};

#define  INSTRUMENT_STATUS_MAX	 4 

static ViString InstrumentStatusArray[] = {
"",
"ʹ��",
"����",
"ά��"
};
/***************************�豸�����ֶ���***************************************/ 

#define  CUSTOMER_MAX	 20 

static ViString CustomerNameArray[] = {
"",
"UT",
"ZT",
"FH",
"HW",
"OP",
"AT",
"AL",
"RH",
"01",
"DY", 
"SUPERXON",
"ZY",
"FLEX",
"AP",
"DZ",
"AD",
"MC",
"MI",
"DEONET"
};

//��Ʒ���Ͷ���
#define  ProductTYPE_MAX	 13 

static ViString ProductTypeArray[] = {
"",
"EPON OLT",
"GPON OLT",
"EPON ONU_P",			//pigtail
"EPON ONU_R", 			//receptacle
"GPON ONT",
"GBE",
"SDH",
"XFP",
"10G OLT",
"10G ONU",
"SFP_PLUS",
"XGPON_ONU"
};
//��Ʒ���Ͷ���

/***************************���򲿷ֶ���***************************************/
#define  PROCESS_TYPE_MAX	 50

enum enum_PROCESS_TYPE
{   
PROCESS_TYPE_NONE,
PROCESS_TYPE_OSA_APD,
PROCESS_TYPE_OSA_APD_QA, 
PROCESS_TYPE_TRX_Assemble,
PROCESS_TYPE_TRX_ChangeBatch, 
PROCESS_TYPE_TRX_DownFirmware, 
PROCESS_TYPE_TRX_E2Check, 
PROCESS_TYPE_OSA_Fiberlink,
PROCESS_TYPE_TRX_Flunk,   
PROCESS_TYPE_TRX_FQC, 
PROCESS_TYPE_OSA_FQC,
PROCESS_TYPE_OSA_LIV, 
PROCESS_TYPE_OSA_LIV_QA, 
PROCESS_TYPE_OSA_LIV_TC1, 
PROCESS_TYPE_OSA_LIV_TC1_QA, 
PROCESS_TYPE_OSA_LIV_TC2, 
PROCESS_TYPE_OSA_LIV_TC2_QA,
PROCESS_TYPE_TRX_Obsolete,  
PROCESS_TYPE_OSA_RXTrack, 
PROCESS_TYPE_OSA_TXTrack,
PROCESS_TYPE_TRX_Penalty,  
PROCESS_TYPE_TRX_PowerRepeat, 
PROCESS_TYPE_TRX_PowerStability, 
PROCESS_TYPE_TRX_PowerTest,
PROCESS_TYPE_TRX_QA_10GTest,
PROCESS_TYPE_TRX_QA_10GTest_L,
PROCESS_TYPE_TRX_QA_10GTest_H,
PROCESS_TYPE_TRX_QA_1GTest,
PROCESS_TYPE_TRX_QA_1GTest_L,
PROCESS_TYPE_TRX_QA_1GTest_H,
PROCESS_TYPE_TRX_QA_1GTest_Rx,
PROCESS_TYPE_TRX_QA_10GTest_Rx_L,
PROCESS_TYPE_TRX_QA_10GTest_Rx_H,
PROCESS_TYPE_TRX_QA_E2Check,
PROCESS_TYPE_TRX_QA_Testing, 
PROCESS_TYPE_TRX_QA_Testing_L,
PROCESS_TYPE_TRX_QA_Testing_H,
PROCESS_TYPE_OSA_ROSA,
PROCESS_TYPE_OSA_ROSA_QA,
PROCESS_TYPE_TRX_RXTesting, 
PROCESS_TYPE_TRX_RXTesting_H,
PROCESS_TYPE_TRX_RXTesting_L,
PROCESS_TYPE_TRX_System,
PROCESS_TYPE_TRX_10GTest,
PROCESS_TYPE_TRX_10GTest_H,
PROCESS_TYPE_TRX_10GTest_L,
PROCESS_TYPE_TRX_Testing,
PROCESS_TYPE_TRX_Testing_H,
PROCESS_TYPE_TRX_Testing_L,
PROCESS_TYPE_TRX_10GTestRx,
PROCESS_TYPE_TRX_TrackingSN,
PROCESS_TYPE_TRX_10GTune,
PROCESS_TYPE_TRX_TuneRX,
PROCESS_TYPE_TRX_Tuning,
PROCESS_TYPE_TRX_Tuning_H,
PROCESS_TYPE_TRX_Tuning_L,
PROCESS_TYPE_TRX_Vapd_Tune,
PROCESS_TYPE_TRX_OQA, 
PROCESS_TYPE_OSA_ChangeBatch,
PROCESS_TYPE_OSA_Reform
}; 

static ViString ProcessTypeArray[] = {
"",
"APD_OQC",
"APD_OQC-QA",
"ASSEMBLE",
"CHANGE BATCH",
"DOWNLOAD FIRMWARE",   
"EEPROM CHECK",
"FIBERLINK",
"FLUNK",
"FQC",
"FQC_OSA",   
"LIV",
"LIV-QA",
"LIV_TC1",
"LIV_TC1-QA",
"LIV_TC2",   
"LIV_TC2-QA",
"OBSOLETE",
"OSA RX TRACK",
"OSA TX TRACK",
"PENALTY",   
"POWERREPEAT",
"POWERSTABILITY",
"POWER TEST",
"QATEST_10G",
"QATEST_10G_LOW", 
"QATEST_10G_HIGH", 
"QATEST_1G",
"QATEST_1G_LOW", 
"QATEST_1G_HIGH", 
"QATEST_10G_RX",
"QATEST_10G_RX_LOW", 
"QATEST_10G_RX_HIGH",
"QAE2CHECK",
"QATESTING",
"QATESTING_LOW",
"QATESTING_HIGH",
"ROSA SENS TEST",   
"ROSA SENS TEST-QA",
"RXTESTING",
"RXTESTING_HIGH", 
"RXTESTING_LOW",
"SYSTEM",
"TEST_10G",
"TEST_10G_HIGH",
"TEST_10G_LOW",
"TESTING",
"TESTING_HIGH",
"TESTING_LOW",
"TESTRX_10G",
"TRACKING SN",   
"TUN_10G",
"TUN_RX",
"TUNING",
"TUNING_HIGH",
"TUNING_LOW",
"VAPD TUNING",
"OQA",
"CHANGE_BATCH_OSA",
"REFORM_OSA"
};
/***************************���򲿷ֶ���***************************************/ 

extern int __stdcall DLLIMPORT MyDLLGetOAServer (char server[]);  

/*******************************************************************/   
/*
�������ƣ�	MyDLLGetInnerSN
�������ڣ�	2011-08-09
�������ߣ�	Roger.Li
����������	��ȡģ����ڲ�ʶ����룬Ĭ�϶�ȡ��ַA2 table0��240-255����
����Ʒ������PN_TYPE_GPON_OLT_SFP���ȶ�ȡA0��96-103�������������Ϣ��ALCATEL������A0��240-255��������ΪInnerSN,�����ȡA2 table0��240-255��
����Ʒ������PN_TYPE_XFP���л���ȡ��ַ��A0 table1��240-255�� 
Ĭ�ϲ�Ʒ����¼��ֵ������-1
����˵����	handle [in] �豸��� 
            sn[]   [out] ʶ����
����ֵ˵����0�������гɹ�����0�������д���
/*******************************************************************/  
extern int __stdcall DLLIMPORT MyDLLGetInnerSN (int handle, enum ePN_TYPE pn_type, char sn[]);

/*
�������ƣ�	MyDLLGetPCBA
�������ڣ�	2012-07-06
�������ߣ�	Roger.Li
����������	��ȡģ����ڲ�pcba���Σ�Ĭ�϶�ȡ��ַA2 table0��224-239����
����Ʒ������PN_TYPE_XFP��PN_TYPE_10G_EPON_OLT�������쳣����ΪĿǰ�ݲ�֧��
Ĭ�ϲ�Ʒ����¼��ֵ������-1
����˵����	handle [in] �豸��� 
            sn[]   [out] ʶ����
����ֵ˵����0�������гɹ�����0�������д���
*/
int __stdcall DLLIMPORT MyDLLGetPCBA (int handle, enum ePN_TYPE pn_type, char pcba[]);

/*******************************************************************/   
/*
�������ƣ�	MyDLLSetSysTimeFormat
�������ڣ�	2011-08-18
�������ߣ�	Roger.Li
����������	�޸�ϵͳʱ���ʽΪyyyy-MM-dd HH:mm:ss   
����ֵ˵����0�������гɹ�����0�������д���
/*******************************************************************/  
extern int __stdcall DLLIMPORT MyDLLSetSysTimeFormat (void);  

/*******************************************************************/   
/*
�������ƣ�	MyDLL_8472_GetRxPWR
�������ڣ�	2011-09-18
�������ߣ�	Roger.Li
����������	����8472Э���ȡA2���ն˼��ֵ���Ѿ���У׼ϵ������  
����ֵ˵����0�������гɹ�����0�������д���
/*******************************************************************/  
int __stdcall DLLIMPORT MyDLL_8472_GetRxPWR (int inst, float *RxPWR); 
int __stdcall DLLIMPORT MyDLL_8472_GetIbias (int inst, float *val);
int __stdcall DLLIMPORT MyDLL_8472_GetTemperatrue (int inst, float *val);
int __stdcall DLLIMPORT MyDLL_8472_GetTemper_NoAck (int inst, float *val);					//��A2�¶ȣ�����noack�������ڼ��ģ���¶��Ƿ���Բ��ԣ���ģ���Ƿ��ڲ��԰��ϣ������쳣ʱ�����¶�999  
int __stdcall DLLIMPORT MyDLL_8472_GetTxPWR (int inst, float *val); 
int __stdcall DLLIMPORT MyDLL_8472_GetVcc (int inst, float *val);
int __stdcall DLLIMPORT MyDLL_8472_GetRxPWR_EED (int inst, float *RxPWR, int EED_Data[]); 	//����EED�ļ����� 
int __stdcall DLLIMPORT MyDLL_8472_GetTxPWR_EED (int inst, float *val, int EED_Data[]);		//����EED�ļ����� 

/*******************************************************************/   
/*
�������ƣ�	MyDLLGetServerType
�������ڣ�	2012-03-22
�������ߣ�	Roger.Li
����������	��ȡע������ã���ȡ�������ݿ����ͣ�ֵ�ֱ�Ϊsql��oracle����Сд������
[HKEY_CURRENT_USER\Software\superxon]
"server_type"="sql" 
����ֵ˵����0�������гɹ�����0�������д���
/*******************************************************************/ 
enum eServerType
{  
SERVER_SQL,
SERVER_ORACLE
};

int __stdcall DLLIMPORT MyDLLGetServerType (enum eServerType *server_type); 

/*******************************************************************/   
/*
�������ƣ�	MyDLL_DB_Init
�������ڣ�	2012-04-18
�������ߣ�	Roger.Li
����������	���ݿ����Ӻ���
1���ȹر�ָ��hdbc���
2������DB_INIT_MULTITHREADED
3���������ݿ��������ã�����MyDLLGetServerType����������
����ֵ˵����0�������гɹ�����0�������д���
/*******************************************************************/ 
int __stdcall DLLIMPORT MyDLL_DB_Init (int *hdbc); 

/*******************************************************************/   
/*
�������ƣ�	MyDLL_DBOA_Init
�������ڣ�	2012-12-13
�������ߣ�	Roger.Li
����������	OA���ݿ����Ӻ���
1���ȹر�ָ��hdbc���
2������DB_INIT_MULTITHREADED
����ֵ˵����0 �������гɹ�����0�������д���
            -2 ��ȡע����OA���ݿ�IP����  
/*******************************************************************/ 
int __stdcall DLLIMPORT MyDLL_DBOA_Init (int *hdbc);

#endif   
