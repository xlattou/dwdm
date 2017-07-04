//V14.0 //20100106
//ע�⣬�����VC++���ã���ȫ�ֱ���ǰ��Ҫ�ӡ�extern����
//���硰int I2C_position;������Ҫ��ɡ�extern int I2C_position;��
//ͬʱ���������.C�ļ��ж�����Щȫ�ֱ���������ֵ��
//ע�⣬�����CVI���ã�������ı䣬�������������.C�ļ��и���Щȫ�ֱ�������ֵ��

#ifndef _CH341A_DLL_H_
#define _CH341A_DLL_H_ 

#ifdef __cplusplus
extern "C" {
#endif

int __stdcall GetDLLVersion_DLL (void);

//get dll file version
void __stdcall GetDLLVersion2_DLL (char *ver); 


int __stdcall F320StreamC2(int iIndex, unsigned char iCommand, int iWriteLength, int iReadLength, unsigned char *ioBuffer);

/*====================================================================================*/
// USB 
/*====================================================================================*/ 
unsigned char OutBuf[300], InBuf[300]; 
int USBStatus;
//int USBiMode;// λ1 λ0: I2C �ٶ�/SCL Ƶ��, 00=����20KHz,01=��׼100KHz,10=����400KHz,11=����750KHz 
//int USBHandle; // host�豸��ţ��������溯����iIndex
/*Լ��USBHandle
CH341��0x000 <= USBHandle <= 0x0FF; (����ֵ:0x000��0x001������) 
LPT��  0x100 <= USBHandle <= 0x3FF; (����ֵ:0x378��0x278)
F320�� 0x800 <= USBHandle <= 0x9FF; (����ֵ:0x800��0x801������) 
���У�
F320��0x800ƫ��������Ϊ�ӵģ�Ϊ�����û����ʱ������CH341��index��
���û�����DLLʱ��DLL�������0x800��ƫ�����Զ���ȥ��
*/


void __stdcall SetUSBiMode_DLL (int iMode);		  //CH341 I2C����; I2C�ӿ��ٶ�/SCLƵ��, 00=����/20KHz,01=��׼/100KHz(Ĭ��ֵ),10=����/400KHz,11=����/750KHz

int __stdcall InitialUSB_DLL (int iIndex);
int __stdcall CloseUSB_DLL (int iIndex);

int __stdcall GetUSBHostDescr(  // ��ȡ�豸������
				int	iIndex,  // ָ��CH341�豸���
				unsigned char *OutBuf);  // ָ��һ���㹻��Ļ�����,���ڱ���������


int __stdcall GetUSBHostConfigDescr(  // ��ȡ����������
				int	iIndex,  // ָ��CH341�豸���
				unsigned char *OutBuf);  // ָ��һ���㹻��Ļ�����,���ڱ���������

int __stdcall USBHost_GetStatus_DLL (int iIndex, unsigned int *iStatus);


int __stdcall USBHost_GetInput_DLL (int iIndex, unsigned int *iStatus);
// λ7-λ0��ӦCH341��D7-D0����
// λ8��ӦCH341��ERR#����, λ9��ӦCH341��PEMP����, λ10��ӦCH341��INT#����, λ11��ӦCH341��SLCT����, λ23��ӦCH341��SDA����
// λ13��ӦCH341��BUSY/WAIT#����, λ14��ӦCH341��AUTOFD#/DATAS#����,λ15��ӦCH341��SLCTIN#/ADDRS#����


int __stdcall USBHost_SetD5D0_DLL (int iIndex, int iSetDirOut, int iSetDataOut);
// ����D5-D0�����ŵ�I/O����,ĳλ��0���Ӧ����Ϊ����,ĳλ��1���Ӧ����Ϊ���,���ڷ�ʽ��Ĭ��ֵΪ0x00ȫ������
// ����D5-D0�����ŵ��������,���I/O����Ϊ���,��ôĳλ��0ʱ��Ӧ��������͵�ƽ,ĳλ��1ʱ��Ӧ��������ߵ�ƽ


int __stdcall USBHost_ResetDevice_DLL(int iIndex);


int __stdcall I2C_HOST_INITIALIZATION_DLL (int iIndex);
//û�ҵ�host�ͷ���-1�����򷵻�0

int __stdcall I2C_SLAVE_SEARCH_DLL (int iIndex, int device_addr);
//û�ҵ�host�ͷ���-1�����򷵻�0

int __stdcall I2C_BYTE_CURRENT_ADDRESS_READ_DLL (int iIndex, int device_addr, unsigned char *rom_value);
//���ֽ�������

int __stdcall I2C_BYTE_READ_DLL (int iIndex, int device_addr, int rom_startaddress, unsigned char *rom_value);
//���ֽ�ָ����ַ��

int __stdcall I2C_BYTEs_READ_DLL (int iIndex, int device_addr, int rom_startaddress, int rom_Length, unsigned char *rom_value_arr);
//���ֽ�ָ���׵�ַ������
//ע�⣬rom_value_arr[256]����Ҫȫ������ȥ��������rom_value_arr[rom_startaddress]��rom_value_arr[rom_startaddress+rom_Length-1]�������

int __stdcall I2C_BYTE_CURRENT_ADDRESS_WRITE_DLL (int iIndex, int device_addr, int rom_startaddress, float T_wait);
//ƫ��������д


int __stdcall I2C_BYTE_WRITE_DLL (int iIndex, int device_addr, int rom_startaddress, unsigned char rom_value, float T_wait);
//���ֽ�ָ���׵�ַд

int __stdcall I2C_BYTEs_WRITE_DLL (int iIndex, int device_addr, int rom_startaddress, int rom_Length, unsigned char *rom_value_arr, float T_wait);
//���ֽ�ָ���׵�ַ����д 
//ע�⣬rom_value_arr[256]Ҫȫ������ȥ��������rom_value_arr[rom_startaddress]��rom_value_arr[rom_startaddress+rom_Length-1]������ݡ�
//ע�⣬����EEPROM��һ���ֻ��8byte��д��������7020��һ���û����󳤶ȵ����ơ�

int __stdcall I2C_2BYTEs_READ_DLL(int iIndex, int device_addr, int rom_startaddress, unsigned char *rom_value1, unsigned char *rom_value2);
//˫�ֽ�ָ���׵�ַ�� 
//ע�⣬ rom_value1�ĵ�ַ��rom_startaddress��rom_value2�ĵ�ַ��rom_startaddress+1

int __stdcall I2C_4BYTEs_READ_DLL(int iIndex, int device_addr, int rom_startaddress, unsigned char *rom_value1, unsigned char *rom_value2, unsigned char *rom_value3, unsigned char *rom_value4); 
//���ֽ�ָ���׵�ַ�� 
//ע�⣬ rom_value1�ĵ�ַ��rom_startaddress��rom_value2�ĵ�ַ��rom_startaddress+1��rom_value3�ĵ�ַ��rom_startaddress+2��rom_value4�ĵ�ַ��rom_startaddress+3

int __stdcall I2C_2BYTEs_WRITE_DLL(int iIndex, int device_addr, int rom_startaddress, unsigned char rom_value1, unsigned char rom_value2, float T_wait);
//˫�ֽ�ָ���׵�ַд 
//ע�⣬ rom_value1�ĵ�ַ��rom_startaddress��rom_value2�ĵ�ַ��rom_startaddress+1

int __stdcall I2C_4BYTEs_WRITE_DLL(int iIndex, int device_addr, int rom_startaddress, unsigned char rom_value1, unsigned char rom_value2, unsigned char rom_value3, unsigned char rom_value4, float T_wait); 
//���ֽ�ָ���׵�ַд 
//ע�⣬ rom_value1�ĵ�ַ��rom_startaddress��rom_value2�ĵ�ַ��rom_startaddress+1��rom_value3�ĵ�ַ��rom_startaddress+2��rom_value4�ĵ�ַ��rom_startaddress+3


/*============F320================================*/
void __stdcall RESET_F320_DLL (int iIndex);
//��λF320
int __stdcall GetF320Numbers(int *lpdwNumDevices); 
//��ȡ��ǰ����PC��F320������
int __stdcall GET_Version_F320_DLL (int iIndex, unsigned char *FVER);
//��ȡF320��firmware�汾��
int __stdcall GetF320SerialNumber(int iIndex, unsigned char *SN); 
//��ȡF320��SN[16]
int __stdcall SetF320SerialNumber(int iIndex, unsigned char *SN); 
//����F320��SN[16]

//��һ��F320תI2C�����ʸ�DLL�ڲ��ı���F320I2CRate: 0=50KHz, 1=100KHz, 2=150KHz, others: Fi2c=24MHz/3/(256-I2CRate) 
//Ȼ����ú���I2C_HOST_INITIALIZATION_DLL�ɽ�I2C���ʽ�������
void __stdcall SetF320I2CRate_DLL (int I2CRate);

//��һ��I2C��ʱ���ReSTART֮ǰ��һ��STOP�ı�־
//Ȼ����ú���I2C_HOST_INITIALIZATION_DLL���ʽ�������
void __stdcall SetF320I2CSTOPbeforeReSTART_DLL (int I2CSTOPbeforeReSTART); 

//��λ��I2C������������F320I2CRate: 0=50KHz, 1=100KHz, 2=150KHz, others: Fi2c=24MHz/3/(256-I2CRate)
//�������Ƿ�����I2C��ʱ���ReSTART֮ǰ��һ��STOP
int __stdcall I2C_HOST_RESET_F320_DLL (int iIndex, int I2CRate, int I2CSTOPbeforeReSTART); 

int __stdcall F320ExecuteCmd_DLL(int iIndex, unsigned char iCommand, int iWriteLength, int iReadLength, unsigned char *ioBuffer);
//ִ��F320����

int __stdcall F320_SPI_Init_DLL(int iIndex, int SPI_Rate, int SCK_PHASE, int SCK_POLARITY);
//F320ExecuteCmd(0, SPI_RST, 3, 0, ioArray) ��λ����������SPI�����ʣ���λ������ 
//SPI_Rate�����ʣ����㹫ʽSCK = 12000/( ioArray[0]+1)K����ioArray[0] = 119����SPI����Ϊ100K
//SCK_PHASE����λ��0����SCK���ڵĵ�һ�����ز������ݣ�1����SCK���ڵĵڶ������ز�������
//SCK_POLARITY��bit0��SCK���ԣ�0��SCK�ڿ���״̬ʱ���ڵ͵�ƽ��1��SCK�ڿ���״̬ʱ���ڸߵ�ƽ
//              bit7��CS������ʽ��0����Ҫ�û�����SetF320_SPI_NSS_DLL()�������ģ�1��ÿ�η���SPI����ʱ�Զ��õ��ø�
//Ĭ������Ϊ100K����һ�����ز������ݣ�SCK�ڿ���״̬ʱ���ڵ͵�ƽ

int __stdcall SetF320_SPI_NSS_DLL(int iIndex, int NSS); 
//F320ExecuteCmd(0, SPI_NSS, 1, 0, NSS);����NSS��ƽ��0���͵�ƽ��1���ߵ�ƽ

int __stdcall SetF320_SPI_CS9_CS0_DLL(int iIndex, int CS9_CS0); 
//F320ExecuteCmd(iIndex, SET_CS, 1, 0, CS9_CS0);����CS9_CS0��ƽ��CS9_CS0��bit9..bit0�ֱ��ӦGPIO��CS9_CS0��ƽ

int __stdcall SPI_BYTEs_READWRITE_F320_DLL (int iIndex, int iRWLength, unsigned char *iBuffer, unsigned char *oBuffer);
//F320תSPI��д��iRWLength���Լ280

int __stdcall SetF320_FPGA_PROGB_DLL(int iIndex, int PROGB);
//����FPGA_PROGB��ƽ��0���͵�ƽ��1���ߵ�ƽ

int __stdcall GET_TM_F320_DLL (int iIndex, unsigned char *TM);
//��ȡF320���LED��ʱ�䣬TM��1LSB=1s
int __stdcall SET_TM_F320_DLL (int iIndex, unsigned char TM);
//����F320���LED��ʱ�䣬TM��1LSB=1s

int __stdcall SEL_SPIIIC_F320_DLL (int iIndex, unsigned char SI);
//����F320���SEL_SPIIIC�ܽŵĵ�ƽ���͵�ƽѡ��SPI���ߵ�ƽѡ��I2C

int __stdcall SET_FS_F320_DLL (int iIndex, unsigned char FS);
//���evb5����F320���FS�ܽŵĵ�ƽ.
//���evb11����F320���FS�ܽŵĵ�ƽ��
//    ����������1�����ݣ����ݽ������£�
//    0x00��ѡ�������Ƶ��115.52MHz
//    0x01: ѡ�������Ƶ��161.13281MHz
//    0x02: ѡ�������Ƶ��167.33165MHz
//    0x03: ѡ�������Ƶ��173.37075MHz
//    ����������Ч��������һ��Ƶ��ѡ����

int __stdcall SDN_VCC_F320_DLL (int iIndex, unsigned char VCC);
//    ����������һ�����ݣ����ݽ������£�
//    ���ݵ�bit0������OLT��Դ�Ŀ��ϣ�1Ϊ�򿪣�0Ϊ�ض�
//    ���ݵ�bit1������ONU��Դ�Ŀ��ϣ�1Ϊ�򿪣�0Ϊ�ض�
//    ����bit��Ч��Ĭ��ȫ������

int __stdcall SEL_IIC_F320_DLL (int iIndex, unsigned char IIC);
//    ����������һ�����ݣ����ݽ������£�
//    ���ݵ�bit0��OLT IIC����ʹ�ܣ�1Ϊʹ�ܣ�0��ֹ
//    ���ݵ�bit1��ONU1 IIC����ʹ�ܣ�1Ϊʹ�ܣ�0��ֹ
//    ���ݵ�bit2��BERT IIC����ʹ�ܣ�1Ϊʹ�ܣ�0��ֹ
//    ���ݵ�bit3��ARM IIC����ʹ�ܣ�1Ϊʹ�ܣ�0��ֹ
//    ����bit��Ч��Ĭ��ȫ��ʹ��

int __stdcall EPP_BYTEs_WRITE_F320_DLL (int iIndex, int rom_startaddress, int rom_Length, unsigned char *rom_value_arr);
//EPP���ֽ�ָ���׵�ַ����д 
//ע�⣬rom_value_arr[256]Ҫȫ������ȥ��������rom_value_arr[rom_startaddress]��rom_value_arr[rom_startaddress+rom_Length-1]������ݡ�

int __stdcall EPP_BYTEs_READ_F320_DLL  (int iIndex, int rom_startaddress, int rom_Length, unsigned char *rom_value_arr);
//EPP���ֽ�ָ���׵�ַ������
//ע�⣬rom_value_arr[256]����Ҫȫ������ȥ��������rom_value_arr[rom_startaddress]��rom_value_arr[rom_startaddress+rom_Length-1]�������


//SEVB0027-4001 ʱ���ź��л��� ͨ���л�����
//Channel����0-7
int __stdcall SEL_CLK_F320_DLL (int iIndex, unsigned char Channel);   
int __stdcall CLK_Get_F320_DLL (int iIndex, unsigned char *Channel);

/*====================================================================================*/
// firmware downloader
/*====================================================================================*/ 
char FirmwareFile[300], EEPROMFile[300];
int RemainEEPROM, WRITEorVERIFY;
unsigned char Data_Array[16*1024*1024/8]; 
unsigned char EEPROM_Array[62*1024]; 

int __stdcall READ_FIRMWARE_DLL(char *hexFile, unsigned char *Data_Array);
int __stdcall ERASE_PAGE0_DLL (int iIndex, int MCU_position);
int __stdcall START_DLL (int iIndex);
int __stdcall RUN_DLL(int iIndex);
int __stdcall ERASE_DLL(int iIndex, int RemainEEPROM);
int __stdcall WRITEorVERIFY_DLL(int iIndex, int RemainEEPROM, int WRITEorVERIFY, unsigned char *Data_Array); 
int __stdcall I2C_MasterWriteSlave_DLL(int iIndex, int my_device_addr);
int __stdcall I2C_MasterReadSlave_DLL (int iIndex, int my_device_addr);


/*====================================================================================*/
// EEPROM 
/*====================================================================================*/ 
#pragma pack (1) /* set maximum alignment to 1 */

typedef signed char         INT8S;               //8bit
typedef unsigned char       INT8U;               //8bit
typedef unsigned char       BYTE;                //8bit 
typedef unsigned short int  INT16U;              //16bit
typedef signed short int    INT16S;              //16bit
typedef unsigned int        INT32U;              //32bit
typedef signed int          INT32S;              //32bit  

/*====================================================================================*/
// EEPROM A0
/*====================================================================================*/ 
struct InfCompCodesDef
{
INT8U bit0:1;			 //1X Copper Passive                      
INT8U bit1:1;			 //1X Copper Active
INT8U bit2:1;		     //1X LX
INT8U bit3:1;			 //1X SX
INT8U bit4:1;			 //10G Base-SR
INT8U bit5:1;			 //10G Base-LR  
INT8U bit6:1;			 //10G Base-LRM  
INT8U bit7:1;			 //Unallocated 
};

struct ESCON_SONETCCDef
{
INT8U bit0:1;			 //OC-48, short reach                      
INT8U bit1:1;			 //OC-48, intermediate reach
INT8U bit2:1;		     //OC-48, long reach
INT8U bit3:1;			 //SONET reach specifier bit1-bit2
INT8U bit4:1;			 //SONET reach specifier
INT8U bit5:1;			 //OC-192, short reach
INT8U bit6:1;			 //ESCON SMF, 1310nm Laser
INT8U bit7:1;			 //ESCON MMF, 1310nm LED
};

struct SONETCCDef
{
INT8U bit0:1;			 //OC-3, short reach                     
INT8U bit1:1;			 //OC-3, single mode, inter. reach
INT8U bit2:1;		     //OC-3, single mode, long reach
INT8U bit3:1;			 //Reserved
INT8U bit4:1;            //OC-12, short reach
INT8U bit5:1;			 //OC-12, single mode, inter. reach
INT8U bit6:1;			 //OC-12, single mode, long reach
INT8U bit7:1;			 //Reserved
};

struct ETHERNETCCDef
{
INT8U bit0:1;			 //1000BASE-SX                  
INT8U bit1:1;			 //1000BASE-LX
INT8U bit2:1;		     //1000BASE-CX
INT8U bit3:1;			 //1000BASE-T
INT8U bit4:1;            //100BASE-LX/LX10
INT8U bit5:1;			 //100BASE-FX
INT8U bit6:1;			 //BASE-BX10
INT8U bit7:1;			 //BASE-PX
};

struct FibreChannelDef
{
INT8U bit0:1;			 //Electrical inter-enclosure (EL)                  
INT8U bit1:1;			 //Longwave laser (LC)
INT8U bit2:1;		     //Reserved
INT8U bit3:1;			 //medium distance (M)
INT8U bit4:1;            //long distance (L)
INT8U bit5:1;			 //intermediate distance (I)
INT8U bit6:1;			 //short distance (S)
INT8U bit7:1;			 //very long distance (V)
};

struct TransTecDef
{
INT8U bit0:1;			 //Reserved   
INT8U bit1:1;			 //Copper FC-BaseT
INT8U bit2:1; 			 //Copper Passive
INT8U bit3:1;		     //Copper Active
INT8U bit4:1;            //Longwave laser (LL)
INT8U bit5:1;			 //Shortwave laser with OFC (SL)
INT8U bit6:1;			 //Shortwave laser w/o OFC (SN)
INT8U bit7:1;			 //Electrical intra-enclosure (EL)
};


struct TransMediaDef
{
INT8U bit0:1;			 //Single Mode (SM)   
INT8U bit1:1;			 //Reserved
INT8U bit2:1; 			 //Multi-Mode, 50um (M5)
INT8U bit3:1;		     //Multi-Mode, 62.5um (M6)
INT8U bit4:1;            //Video Coax (TV)
INT8U bit5:1;			 //Miniature Coax (MI)
INT8U bit6:1;			 //Shielded Twisted Pair (TP)
INT8U bit7:1;			 //Twin Axial Pair (TW)
};


struct FibreChannelSpeedDef
{
INT8U bit0:1;			 //100 MBytes/sec 
INT8U bit1:1;			 //Reserved
INT8U bit2:1; 			 //200 MBytes/sec
INT8U bit3:1;		     //Reserved
INT8U bit4:1;            //400 MBytes/sec
INT8U bit5:1;			 //Reserved
INT8U bit6:1;			 //800 MBytes/sec
INT8U bit7:1;			 //Reserved
};


struct StatCtrlBitsDef
{
INT8U  dataReady:1;
INT8U  LOS:1;
INT8U  TX_Fault:1;
INT8U  softRateSelect:1;
INT8U  rateSelectState:1;
INT8U  reserved:1;
INT8U  softTxDis:1;	
INT8U  txDisableState:1;
};

struct Options64Def
{
INT8U bit0:1;			 //Linear Receiver Output Implemented
INT8U bit1:1;			 //Power Class Declaration
INT8U bit2:1; 			 //Cooled Transceiver Declaration
INT8U bit3_7:5;			 //Reserved 
};


struct Options65Def
{
INT8U bit0:1;			 //Reserved
INT8U bit1:1;			 //Rx_LOS
INT8U bit2:1; 			 //Loss Inverted
INT8U bit3:1;		     //TX_FAULT
INT8U bit4:1;            //TX_DISABLE
INT8U bit5:1;			 //RATE_SELECT
INT8U bit6_7:2;			 //Reserved
};

struct DiagnosticMonitorDef
{
INT8U bit0_1:2;			 //Reserved
INT8U bit2:1; 			 //Address Modes
INT8U bit3:1;		     //Received power type
INT8U bit4:1;            //Externally calibrated
INT8U bit5:1;			 //Internally calibrated
INT8U bit6:1;			 //If Digital diagnostic monitoring, Must be "0"
INT8U bit7:1;			 //If Digital diagnostic monitoring, Must be "1"
};

struct EnhancedOptionsDef
{
INT8U bit0:1;			 //Reserved
INT8U bit1:1;			 //Optional Rate
INT8U bit2:1; 			 //Optional Application
INT8U bit3:1;		     //RATE_SELECT
INT8U bit4:1;            //RX_LOS
INT8U bit5:1;			 //TX_FAULT
INT8U bit6:1;			 //TX_DISABLE
INT8U bit7:1;			 //Optional Alarm/warning flags
};

struct strA0 		  
{
  //BASE ID FIELDS
  
      //Type of serial transceiver 
  INT8U  identifier;	                 //pStrA0[0]
      //Extended identifier of type of serial transceiver
  INT8U  extIdentifier;	                 //pStrA0[1]
      //Code for connector type
  INT8U  connector;		                 //pStrA0[2]
      //Code for electronic compatibility or optical compatibility
  //INT8U  transceiver[8];		         //pStrA0[3~10]
  //Infiniband Compliance Codes
  struct InfCompCodesDef infCompCodes;
  //Part of SONET Compliance Codes
  struct ESCON_SONETCCDef escon_SonetCC;
  //Part of SONET Compliance Codes
  struct SONETCCDef sonetCC;
  //Gigabit Ethernet Compliance Codes
  struct ETHERNETCCDef ethernetCC;
  //Fiber Channel link length & part of transmitter technology
  struct FibreChannelDef fibreChannel;
  //Part of Fiber Channel transmitter technology
  struct TransTecDef transTec;
  //Fiber Channel Transmission media
  struct TransMediaDef transMedia;
  //Fiber Channel speed
  struct FibreChannelSpeedDef fibreChannelSpeed;
  
      //Code for serial encoding algorithm
  INT8U  encoding;		                 //pStrA0[11]
      //Nominal bit rate, units of 100 MBits/sec
  INT8U  nominalBitRate;	             //pStrA0[12] 	
      //Reserved for SFF-8079
  INT8U  rateIdentifier;		         //pStrA0[13]
      //Link length supported for 9/125 ��m fiber, units of km
  INT8U  length9im_km;		             //pStrA0[14]
      //Link length supported for 9/125 ��m fiber, units of 100m
  INT8U  length9im;		             //pStrA0[15]
      //Link length supported for 50/125 ��m fiber, units of 10m
  INT8U  length50im;		             //pStrA0[16]
      //Link length supported for 62.5/125 ��m fiber, units of 10m
  INT8U  length62p5im;	                 //pStrA0[17]
      //Link length supported for copper, units of meters
  INT8U  lengthCopper;	                 //pStrA0[18]
      //Reserved
  INT8U	 lengthOM3;		                 //pStrA0[19]
      //Vendor name
  INT8U  vendorName[16];		             //pStrA0[20~35]
      //Reserved
  INT8U  reserved3;		                 //pStrA0[36]
      //Vendor IEEE company ID
  INT8U  vendorOUI[3];		             //pStrA0[37~39]
      //Part number provided by vendor
  INT8U  vendorPN[16];		             //pStrA0[40~55]
      //Revision level for part number provided by vendor
  INT8U  vendorRev[4];		             //pStrA0[56~59]
      //Laser wavelength
  INT8U laserWaveLength[2];             //pStrA0[60~61]
      //Reserved
  INT8U  reserved4;                      //pStrA0[62]
      //Check code for Base ID Fields
  INT8U  cc_Base;		                 //pStrA0[63]
  
      //EXTENDED ID FIELDS
  
      //Indicates which optional transceiver signals are implemented
  struct Options64Def options64;		//pStrA0[64]
  struct Options65Def options65;		//pStrA0[65]
      //Upper bit rate margin, units of %
  INT8U  maxBiteRate;                    //pStrA0[66]
      //Lower bit rate margin, units of %
  INT8U  minBiteRate;                    //pStrA0[67]
      //Serial number provided by vendor
  INT8U vendorSN[16];                    //pStrA0[68~83]	
      //Vendor��s manufacturing date code
  INT8U dateCode[8];                     //pStrA0[84~91]
      //Indicates which type of diagnostic monitoring is implemented in the transceiver
  struct DiagnosticMonitorDef diagMonitorType; //pStrA0[92]
      //Indicates which optional enhanced features are implemented in the transceiver
  struct EnhancedOptionsDef  enhancedOptions; //pStrA0[93]
      //Indicates which revision of SFF-8472 the transceiver complies with
  INT8U sff_8472Compliance;              //pStrA0[94]
      //Check code for the Extended ID Fields		
  INT8U cc_Ext;                          //pStrA0[95]
  
      //VENDOR SPECIFIC ID FIELDS
  INT8U vendorSpecReserved[32];          //pStrA0[96~127]
  											
      //Reserved for SFF8079
  INT8U sff8079Reserved[128];            //pStrA0[128~255] 
  
};		

union uA0
{ 
  struct strA0 sStrA0;
  INT8U  pStrA0[256];
};

union uA0 A0;                
void __stdcall A0_transform(unsigned char *rom_value_arr, unsigned char *A0_str);  

//////////////////////////////////////////////////////////////
/*A2*/
/////////////////////////////////////////////////////////////////
struct status_110def
{
INT8U  Data_Ready:1;
INT8U  LOS:1;
INT8U  TX_Fault:1;
INT8U  Reserved1:3;
INT8U  Soft_Tx_Dis:1;	
INT8U  Tx_Dis:1;
};

struct FLAG_112DEF
{
INT8U  Tx_P_Low:1;
INT8U  Tx_P_Hgh:1;
INT8U  Tx_Bias_Low:1;
INT8U  Tx_Bias_Hgh:1;
INT8U  vcc_Low:1;
INT8U  vcc_Hgh:1;
INT8U  Temp_Low:1;
INT8U  Temp_Hgh:1;
};

struct FLAG_113DEF
{
INT8U  Reserved:6;
INT8U  Rx_P_Low:1;
INT8U  Rx_P_Hgh:1;
};
 
struct strA2 		  
{
INT8U	reserved8[128];					//pStrA2[219~255]  
INT8U	LUTIndex;						//pStrA2[127] 
INT32U	Password;  						//pStrA2[123~126]   										
INT8U   mode;                      		//pStrA2[122]
INT8U   version;                   		//pStrA2[121]
INT8U   BEN_i;                			//pStrA2[120]
INT8U	BAK_i;                      	//pStrA2[119]      																																	
INT8U	reserved6;                  	//pStrA2[118]      																																	
struct	FLAG_113DEF FLAG_WARN_117;  	//pStrA2[117]                 																														
struct	FLAG_112DEF FLAG_WARN_116;  	//pStrA2[116]       																																
INT8U	reserved5[2];               	//pStrA2[114~115]               																															
struct	FLAG_113DEF FLAG_ALARM_113; 	//pStrA2[113]																															
struct	FLAG_112DEF FLAG_ALARM_112 ;	//pStrA2[112]    	//Optional Alarm and Warning Flag Bits                                       																																							
INT8U	PowerLeveling;              	//pStrA2[111]    	//Reserved for SFF-8079        																						
struct	status_110def status_110;   	//pStrA2[110]    	//Optional Status/Control Bits              																						
INT8U	reserved3[4];               	//pStrA2[106~109]	//Reserved for future definition of digitized analog input                   																																						
INT16U	rx_Power;                   	//pStrA2[104~105]	//Measured RX input power.           																						
INT16U	tx_Power;                   	//pStrA2[102~103]	//Measured TX output power.                                                  																						
INT16U	tx_Bias;                    	//pStrA2[100~101]	//Internally measured TX Bias Current                                        																									
INT16U	vcc;                        	//pStrA2[98~99]  	//Internally measured supply voltage in transceiver                          																																									
INT16S	temperature;                	//pStrA2[96~97]  	//Internally measured module temperature     																																																																				
INT8U	cc_Ext;                     	//pStrA2[95]     	//Check code for the Extended ID Fields           																									
INT8U	reserved2[3];               	//pStrA2[92~94]  	//Reserved                  																								
INT16S	voltageOffset;              	//pStrA2[90~91]  	//Offset of Supply Voltage Linear Calibartion      																																											
INT16U	voltageSlope;               	//pStrA2[88~89]  	//Slope of Supply Voltage Linear Calibartion        																																														
INT16S	tempOffset;                 	//pStrA2[86~87]  	//Offset of Temperature Linear Calibartion                                    																																				
INT16U	tempSlope;                  	//pStrA2[84~85]  	//Slope of Temperature Linear Calibartion                                     																																				
INT16S	tx_PwrOffset;               	//pStrA2[82~83]  	//Offset of Transmitter Coupled Output Power Linear Calibartion               																																						
INT16U	tx_PwrSlope;                	//pStrA2[80~81]  	//Slope of Transmitter Coupled Output Power Linear Calibartion                																																						
INT16S	tx_IOffset;                 	//pStrA2[78~79]  	//Offset of Laser Bias Current Linear Calibartion                             																																						
INT16U	tx_ISlope;                  	//pStrA2[76~77]  	//Slope of Laser Bias Current Linear Calibartion           																																													
float	rx_PWR0;                    	//pStrA2[72~75]  	//Polynomial Fit Coefficient of Order 0 for Rx Optical Power Calibartion.     																 
float	rx_PWR1;                    	//pStrA2[68~71]  	//Polynomial Fit Coefficient of Order 1 for Rx Optical Power Calibartion.																																							
float	rx_PWR2;                    	//pStrA2[64~67]  	//Polynomial Fit Coefficient of Order 2 for Rx Optical Power Calibartion.     																																									
float	rx_PWR3;                    	//pStrA2[60~63]  	//Polynomial Fit Coefficient of Order 3 for Rx Optical Power Calibartion.																																											
float	rx_PWR4;	                	//pStrA2[56~59]  	//Polynomial Fit Coefficient of Order 4 for Rx Optical Power Calibartion     																																																													
INT8U	reserved1[16];		        	//pStrA2[40~55]  	//Reserved for future monitored quantities 																							
INT16U	rxPowerLowWarning;           	//pStrA2[38~39]		//Reserved         																														
INT16U	rxPowerHighWarning;	         	//pStrA2[36~37]		//RX Power High Warning  																												
INT16U	rxPowerLowAlarm;	         	//pStrA2[34~35]		//RX Power Low Alarm  																											
INT16U	rxPowerHighAlarm;	         	//pStrA2[32~33]		//RX Power High Alarm 																											
INT16U	txPowerLowWarning;	         	//pStrA2[30~31]		//TX Power Low Warning   																									
INT16U	txPowerHighWarning;	         	//pStrA2[28~29]		//TX Power High Warning   																												
INT16U	txPowerLowAlarm;	         	//pStrA2[26~27]		//TX Power Low Alarm     																													
INT16U	txPowerHighAlarm;	         	//pStrA2[24~25]		//TX Power High Alarm   																														
INT16U	biasLowWarning;	             	//pStrA2[22~23]		//Bias Low Warning     																									
INT16U	biasHighWarning;	         	//pStrA2[20~21]		//Bias High Warning   																													
INT16U	biasLowAlarm;	             	//pStrA2[18~19]		//Bias Low Alarm        																										
INT16U	biasHighAlarm;	             	//pStrA2[16~17]		//Bias High Alarm       																										
INT16U	voltageLowWarning;	         	//pStrA2[14~15]		//Voltage Low Warning   																									
INT16U	voltageHighWarning;	         	//pStrA2[12~13]		//Voltage High Warning   																									
INT16U	voltageLowAlarm;	         	//pStrA2[10~11]		//Voltage Low Alarm 				
INT16U	voltageHighAlarm;	         	//pStrA2[8~9]  		//Voltage High Alarm  							
INT16S	tempLowWarning;              	//pStrA2[6~7]  		//Temperature Low Warning  																											
INT16S	tempHighWarning;             	//pStrA2[4~5] 		//Temperature High Warning  																																																				
INT16S	tempLowAlarm;	             	//pStrA2[2~3] 		//Temperature Low Alarm  																																																			
INT16S	tempHighAlarm;	             	//pStrA2[0~1] 		//Temperature High Alarm 																													 
}; 

union uA2
{ 
  struct strA2 sStrA2;
  INT8U  pStrA2[256];
};

union uA2 A2;

#ifdef __cplusplus
}
#endif

#endif  
