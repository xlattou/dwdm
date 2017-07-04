#include "cvidll.h"      
#include <formatio.h>
#include <userint.h>
#include <ansi_c.h>
#include "B2BERT.h"

//union uB2BERT_GLOBAL B2BERT_GLOBAL;
//union uB2BERT_NORM   B2BERT_NORM; 
//union uB2BERT_BURST  B2BERT_BURST; 
//union uB2BERT B2BERT; 

int B2BERT_Init (int *USB_Handle, char *SN)
{
	int  error, iIndex, lpdwNumDevices; 
	char myUSBSN[30];    
	int	 I2Cslave_found, reg_add;
	int  SPI_Rate, SCK_PHASE, SCK_POLARITY;  
	unsigned char USBVersion;
	union uB2BERT B2BERT;
	
	B2BERT_Addr=0x84;  
	
	I2Cslave_found =0;
	
	//set I2C rate
	SetF320I2CRate_DLL(0);//F320 I2C����; 0=50KHz, 1=100KHz 
	
	error = GetF320Numbers(&lpdwNumDevices); 
	if(error==0 && lpdwNumDevices>0)
	{
		for (iIndex=0x800; iIndex<lpdwNumDevices+0x800; iIndex++) 
		{
			error = I2C_HOST_INITIALIZATION_DLL (iIndex); 
			if (error!=-1) //USB host found
			{   
				*USB_Handle = iIndex;
				
				error = I2C_SLAVE_SEARCH_DLL(iIndex, B2BERT_Addr);  
				if (error==0) //I2C EVB found  
				{
					//read F320 version
					error = GET_Version_F320_DLL(*USB_Handle-0x800, &USBVersion);
						
					//read F320 SN
					memset (myUSBSN, 0, sizeof(myUSBSN));
					error = GetF320SerialNumber(*USB_Handle-0x800, myUSBSN);
					
					//read eeprom information from EVB_addr
					error = I2C_BYTEs_READ_DLL (*USB_Handle, B2BERT_Addr, 0, 256, B2BERT.pStrB2BERT);
					
					//compare SN
					error = strcmp (B2BERT.sStrB2BERT.vendorSN, myUSBSN);
					if (error && (*USB_Handle-0x800)>=(lpdwNumDevices-1)) {MessagePopup("��ʾ","F320�����кź�B2BERT�����кŲ�һ��");return -1;}
					
					error = strcmp (B2BERT.sStrB2BERT.vendorSN, SN);
					if (error && (*USB_Handle-0x800)>=(lpdwNumDevices-1)) {MessagePopup("��ʾ","ѡ�õ�B2BERT���кź�B2BERTʵ�����кŲ�һ��");return -1;}
					
					if (error==0) {I2Cslave_found=1;break;}
				}
			}
		}
	}
	
	if (I2Cslave_found==0) 
	{
		return -1;
	}
	
	error = (B2BERT.sStrB2BERT.Core_Voltage/10000.0>3.6 || B2BERT.sStrB2BERT.Core_Voltage/10000.0<3.0); 
	if (error<0) {MessagePopup("��ʾ","B2BERT��ѹ���ʧ�ܣ�");return -1;}
	
	SPI_Rate  = 119;	//���ʣ����㹫ʽSCK = 12000/( ioArray[0]+1)K����ioArray[0] = 119����SPI����Ϊ100K
	SCK_PHASE = 0;    	//��λ��0����SCK���ڵĵ�һ�����ز������ݣ�1����SCK���ڵĵڶ������ز�������
	SCK_POLARITY = 0x80;//���ԣ�0��SCK�ڿ���״̬ʱ���ڵ͵�ƽ��1��SCK�ڿ���״̬ʱ���ڸߵ�ƽ
	                    //bit7�Զ�����CS��Ϊ1���Զ�����CS���ڷ���SPI����ʱ�Զ��õͣ��������Զ��ø�
	error = F320_SPI_Init_DLL(*USB_Handle-0x800, SPI_Rate, SCK_PHASE, SCK_POLARITY);		

	return 0;
}

int B2BERT_SET_CPU_Haddr (int USB_Handle, int Haddr)
{
	int error, reg_add;
	union uB2BERT_GLOBAL B2BERT_GLOBAL; 
	
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.CPU_Haddr = Haddr;
	
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.CPU_Haddr)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Set CPU_Haddr error");return -1;} 
	
	return 0;
}

int B2BERT_SET_RATE (int USB_Handle, int rate)
{
	int error, reg_add;
	union uB2BERT_GLOBAL B2BERT_GLOBAL;
	
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.REG06)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) return -1;
	
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.REG06.isGPONorEPON = rate;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Set rate error");return -1;} 
	
	return 0;	
}

int B2BERT_SET_Sel_Rx_ONU (int USB_Handle, int sel_rx_onu)
{
	int error, reg_add;
	union uB2BERT_GLOBAL B2BERT_GLOBAL;
	
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.REG06)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) return -1;
	
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.REG06.Sel_Rx_ONU = sel_rx_onu;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Select ONU error");return -1;} 
	
	return 0;	
}

int B2BERT_SET_GLOBAL_Rst (int USB_Handle, int Rst)
{
	int error, reg_add;
	union uB2BERT_GLOBAL B2BERT_GLOBAL;
	
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.GLOBALRst)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) return -1;
	
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.GLOBALRst.bit0 = Rst;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Set GLOBAL Rst error");return -1;} 
	
	return 0;
}

int B2BERT_SET_GLOBAL_BurstCtrlRst (int USB_Handle, int Rst)
{
	int error, reg_add;
	union uB2BERT_GLOBAL B2BERT_GLOBAL;
	
	//0x0C
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.BurstCtrlRst)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) return -1;
	
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.BurstCtrlRst.bit0 = Rst;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Set GLOBAL Burst Ctrl Rst error");return -1;} 
	
	return 0;
}

int B2BERT_GET_GLOBAL_BurstCtrlStatus (int USB_Handle, int *BurstCtrlOK, int *BurstCtrlErr)
{
	int error, reg_add; 
	union uB2BERT_GLOBAL B2BERT_GLOBAL;

	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.BurstCtrlStatus)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) return -1;
	
	*BurstCtrlOK = B2BERT_GLOBAL.sStrB2BERT_GLOBAL.BurstCtrlStatus.Burst_Ctrl_OK; 
	*BurstCtrlErr= B2BERT_GLOBAL.sStrB2BERT_GLOBAL.BurstCtrlStatus.Burst_Ctrl_Err;
	
	return 0;
}

int B2BERT_SET_NORM_Rst (int USB_Handle, int Rst)
{
	int error, reg_add;
	union uB2BERT_NORM B2BERT_NORM; 
	
	reg_add = (int)(&B2BERT_NORM.sStrB2BERT_NORM.NormRst)-(int)(&B2BERT_NORM.sStrB2BERT_NORM.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) return -1;
	
	B2BERT_NORM.sStrB2BERT_NORM.NormRst.bit0 = Rst;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) {MessagePopup("Error", "B2BERT Set Norm Rst error");return -1;} 
	
	return 0;
}

int B2BERT_SET_NORM_Loop (int USB_Handle, int Rx2Tx_Loop, int Tx2Rx_Loop, int VSC_Rx2Tx_Loop, int VSC_Tx2Rx_Loop)
{
	int error, reg_add;
	union uB2BERT_NORM B2BERT_NORM; 
	
	reg_add = (int)(&B2BERT_NORM.sStrB2BERT_NORM.Loop)-(int)(&B2BERT_NORM.sStrB2BERT_NORM.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) return -1;
	
	B2BERT_NORM.sStrB2BERT_NORM.Loop.Rx2Tx_Loop = Rx2Tx_Loop;
	B2BERT_NORM.sStrB2BERT_NORM.Loop.Tx2Rx_Loop = Tx2Rx_Loop;
	B2BERT_NORM.sStrB2BERT_NORM.Loop.VSC_Rx2Tx_Loop = VSC_Rx2Tx_Loop;
	B2BERT_NORM.sStrB2BERT_NORM.Loop.VSC_Tx2Rx_Loop = VSC_Tx2Rx_Loop;
	
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) {MessagePopup("Error", "B2BERT Set Norm Loop error");return -1;} 
	
	return 0;
}

int B2BERT_SET_BURST_Rst (int USB_Handle, int Rst)
{
	int error, reg_add;
	union uB2BERT_BURST  B2BERT_BURST; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.BurstRst)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) return -1;
	
	B2BERT_BURST.sStrB2BERT_BURST.BurstRst.bit0 = Rst;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST Rst error");return -1;} 
	
	return 0;
}

int B2BERT_SET_BURST_OLTRst (int USB_Handle, int Rst)
{
	int error, reg_add;
	union uB2BERT_BURST  B2BERT_BURST; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.OLTRst)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) return -1;
	
	B2BERT_BURST.sStrB2BERT_BURST.OLTRst.bit0 = Rst;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST OLT Rst error");return -1;} 
	
	return 0;
}

int B2BERT_SET_BURST_SyncReStar (int USB_Handle, int SyncReStar)
{
	int error, reg_add;
	union uB2BERT_BURST  B2BERT_BURST; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.SyncReStar)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) return -1;
	
	B2BERT_BURST.sStrB2BERT_BURST.SyncReStar.ReSync = SyncReStar;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST SyncReStar error");return -1;} 
	
	return 0;
}

/*
int B2BERT_SET_BURST_OLTRst_Pos_Sel (int USB_Handle, int OLTRst_Pos_Sel)
{
	int error, reg_add;
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.OLTRst_Pos_Sel)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) return -1;
	
	B2BERT_BURST.sStrB2BERT_BURST.OLTRst_Pos_Sel.bit0 = OLTRst_Pos_Sel;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST OLTRst_Pos_Sel error");return -1;} 
	
	return 0;
}
*/
int B2BERT_SET_BURST_ClearErr (int USB_Handle)
{
	int error, reg_add;
	union uB2BERT_BURST  B2BERT_BURST; 
	
	B2BERT_BURST.sStrB2BERT_BURST.ErrTime.ErrTime7_0 = 0;
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.ErrTime)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST ClearErr error");return -1;} 
	
	return 0;
}

/*
int B2BERT_SET_BURST_AlignStar (int USB_Handle, int Align)
{
	int error, reg_add;
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.Align)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) return -1;
	
	B2BERT_BURST.sStrB2BERT_BURST.Align.Align_ReStr = Align;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST Align error");return -1;} 
	
	return 0;
}

int B2BERT_SET_BURST_Loop (int USB_Handle, int Loop)
{
	int error, reg_add;
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.Loop)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) return -1;
	
	B2BERT_BURST.sStrB2BERT_BURST.Loop.Tx2Rx_Loop = Loop;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST Loop error");return -1;} 
	
	return 0;
}
*/
int B2BERT_SET_BURST_Pattern (int USB_Handle, int Pattern)
{
	int error, reg_add;
	union uB2BERT_BURST  B2BERT_BURST; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.Pattern)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) return -1;
	
	B2BERT_BURST.sStrB2BERT_BURST.Pattern.isUserorPRBS = Pattern;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST Pattern error");return -1;} 
	
	return 0;
}

int B2BERT_SET_BURST_PRBS (int USB_Handle, int PRBS)
{
	int error, reg_add;
	union uB2BERT_BURST  B2BERT_BURST; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.PRBS)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) return -1;
	
	B2BERT_BURST.sStrB2BERT_BURST.PRBS = PRBS;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST PRBS error");return -1;} 
	
	return 0;
}

int B2BERT_SET_BURST_UserStart (int USB_Handle, int UserStart)
{
	int error, reg_add;
	union uB2BERT_BURST  B2BERT_BURST; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.UserStatus)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) return -1;
	
	B2BERT_BURST.sStrB2BERT_BURST.UserStatus.User_Start = UserStart;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST User state error");return -1;} 
	
	return 0;
}

int B2BERT_GET_BURST_UserState (int USB_Handle, int *UserState)
{
	int error, reg_add;
	union uB2BERT_BURST  B2BERT_BURST; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.UserStatus)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) return -1;
	
	*UserState = B2BERT_BURST.sStrB2BERT_BURST.UserStatus.User_Rdy;
	
	return 0;
}

int B2BERT_SET_BURST_State (int USB_Handle, int state)
{
	int error, reg_add;
	union uB2BERT_BURST  B2BERT_BURST; 
	
	B2BERT_BURST.sStrB2BERT_BURST.Burst_State = state; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.Burst_State)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set Burst state error");return -1;} 
	
	return 0;
}

int B2BERT_SET_BURST_SD_Level (int USB_Handle, int SD_Level)
{
	int error, reg_add;
	union uB2BERT_BURST  B2BERT_BURST; 
	
	B2BERT_BURST.sStrB2BERT_BURST.SD_Level = SD_Level;
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.SD_Level)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST SD Level error");return -1;} 
	
	return 0;
}

int B2BERT_SET_BURST_SDChk_Pos (int USB_Handle, unsigned char SDChk_Pos)
{
	int error, reg_add;
	union uB2BERT_BURST  B2BERT_BURST; 
	
	B2BERT_BURST.sStrB2BERT_BURST.SDChk_Pos = SDChk_Pos; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.SDChk_Pos)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST SDChk_Pos error");return -1;} 
	
	return 0;
}

int B2BERT_SET_BURST_SDChk_Width (int USB_Handle, unsigned int SDChk_Width)
{
	int error, reg_add; 
	union uB2BERT_BURST  B2BERT_BURST; 
	
	B2BERT_BURST.sStrB2BERT_BURST.SDChk_Width.width7_0  = (SDChk_Width & 0xFF)>>0;
	B2BERT_BURST.sStrB2BERT_BURST.SDChk_Width.width15_8 = (SDChk_Width & 0xFF00)>>8; 
	B2BERT_BURST.sStrB2BERT_BURST.SDChk_Width.width18_16= (SDChk_Width & 0xFF0000)>>16; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.SDChk_Width)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 3, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST SDChk_Width error");return -1;} 

	return 0;
}

int B2BERT_SET_BURST_UserPatternLen (int USB_Handle, unsigned short UserPatternLen)
{
	int error, reg_add; 
	union uB2BERT_BURST  B2BERT_BURST; 
	
	B2BERT_BURST.sStrB2BERT_BURST.UserLen.UserLen7_0  = (UserPatternLen & 0xFF)>>0;
	B2BERT_BURST.sStrB2BERT_BURST.UserLen.UserLen11_8 = (UserPatternLen & 0xFF00)>>8; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.UserLen)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 2, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST UserPatternLen error");return -1;} 

	return 0;
}

int B2BERT_SET_BURST_UserPatternData (int USB_Handle, unsigned char *UserPatternData)
{
	int error, reg_add;
	int i, j;
	union uB2BERT_BURST  B2BERT_BURST; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.UserPattern)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte);
	
	for (i=0; i<4; i++)
	{
		B2BERT_BURST.sStrB2BERT_BURST.UserPattern.Table=i;
		
		error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
		if (error) {MessagePopup("Error", "B2BERT Set BURST UserPatternLen table error");return -1;} 
		
		for (j=0; j<128; j++)
		{
			B2BERT_BURST.sStrB2BERT_BURST.UserPattern.Data[j]=UserPatternData[j+i*128];
		}
		error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add+1, 128, B2BERT_BURST.pStrB2BERT_BURST); 
		if (error) {MessagePopup("Error", "B2BERT Set BURST UserPatternLen data error");return -1;} 
	}
	
	return 0;
}

/*
int B2BERT_SET_BURST_ClearSD (int USB_Handle)
{
	int error, reg_add;
	
	B2BERT_BURST.sStrB2BERT_BURST.SDChk_Status.SDA = 0; 
	B2BERT_BURST.sStrB2BERT_BURST.SDChk_Status.SDD = 0;
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.SDChk_Status)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST ClearSD error");return -1;} 
	
	return 0;
}
*/

int B2BERT_GET_BURST_SDChk_Status (int USB_Handle, int *SDA, int *SDD)
{
	int error, reg_add;
	union uB2BERT_BURST  B2BERT_BURST; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.SDChk_Status)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte);
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Get BURST SDChk_Status error");return -1;}
	
	*SDA = B2BERT_BURST.sStrB2BERT_BURST.SDChk_Status.SDA; 
	*SDD = B2BERT_BURST.sStrB2BERT_BURST.SDChk_Status.SDD;
	
//	error = B2BERT_SET_BURST_ClearSD (USB_Handle);
//	if (error) return -1; 
	
	return 0;
}

int B2BERT_SET_BURST_INV (int USB_Handle, int InINV, int OutINV)
{
	int error, reg_add;
	union uB2BERT_BURST  B2BERT_BURST; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.INV)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) return -1;
	
	B2BERT_BURST.sStrB2BERT_BURST.INV.In_Inv = InINV;
	B2BERT_BURST.sStrB2BERT_BURST.INV.Out_Inv= OutINV; 
	
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST INV error");return -1;} 
	
	return 0;
	
}

int B2BERT_SET_BURST_InsErrEn (int USB_Handle, int InsErrEn)
{
	int error, reg_add;
	union uB2BERT_BURST  B2BERT_BURST; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.InsErrEn)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) return -1;
	
	B2BERT_BURST.sStrB2BERT_BURST.InsErrEn.InsErrEnbit0= InsErrEn; 
	
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST InsErrEn error");return -1;} 
	
	return 0;
}

int B2BERT_SET_BURST_InsErrTime (int USB_Handle, unsigned int InsErrTime)
{
	int error, reg_add; 
	union uB2BERT_BURST  B2BERT_BURST; 
	
	B2BERT_BURST.sStrB2BERT_BURST.InsErrTime.Time7_0  = (InsErrTime & 0xFF)>>0;
	B2BERT_BURST.sStrB2BERT_BURST.InsErrTime.Time15_8 = (InsErrTime & 0xFF00)>>8; 
	B2BERT_BURST.sStrB2BERT_BURST.InsErrTime.Time23_16= (InsErrTime & 0xFF0000)>>16; 
	B2BERT_BURST.sStrB2BERT_BURST.InsErrTime.Time31_24= (InsErrTime & 0xFF000000)>>24; 
		
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.InsErrTime)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 4, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST InsErrTime error");return -1;} 

	return 0;
}

int B2BERT_SET_BURST_SyncErrMax (int USB_Handle, unsigned short int SyncErrMax)
{
	int error, reg_add; 
	union uB2BERT_BURST  B2BERT_BURST; 
	
	B2BERT_BURST.sStrB2BERT_BURST.SyncErrMax.Err_Max7_0  = (SyncErrMax & 0xFF)>>0;
	B2BERT_BURST.sStrB2BERT_BURST.SyncErrMax.Err_Max13_8 = (SyncErrMax & 0xFF00)>>8; 
		
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.SyncErrMax)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 2, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST InsErrTime error");return -1;} 

	return 0;
}

int B2BERT_SET_GLOBAL_BurstPeriod (int USB_Handle, unsigned int BurstPeriod)
{
	int error, reg_add; 
	union uB2BERT_GLOBAL B2BERT_GLOBAL;
	
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Burst_Period.Burst_Period7_0  = (BurstPeriod & 0xFF)>>0;
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Burst_Period.Burst_Period15_8 = (BurstPeriod & 0xFF00)>>8; 
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Burst_Period.Burst_Period19_16= (BurstPeriod & 0xFF0000)>>16; 
	
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Burst_Period)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 3, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Set BurstPeriod error");return -1;} 

	return 0;
}

int B2BERT_SET_GLOBAL_TrigPos (int USB_Handle, unsigned int TrigPos)
{
	int error, reg_add;
	union uB2BERT_GLOBAL B2BERT_GLOBAL;
	
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Trig_Pos.Trig_Pos7_0  = (TrigPos & 0xFF)>>0;
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Trig_Pos.Trig_Pos15_8 = (TrigPos & 0xFF00)>>8; 
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Trig_Pos.Trig_Pos19_16= (TrigPos & 0xFF0000)>>16; 
	
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Trig_Pos)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 3, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Set TrigPos error");return -1;} 

	return 0;
}

int B2BERT_SET_GLOBAL_TrigWidth (int USB_Handle, unsigned int TrigWidth)
{
	int error, reg_add; 
	union uB2BERT_GLOBAL B2BERT_GLOBAL;
	
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Trig_Width.Trig_Width7_0  = (TrigWidth & 0xFF)>>0;
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Trig_Width.Trig_Width15_8 = (TrigWidth & 0xFF00)>>8; 
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Trig_Width.Trig_Width19_16= (TrigWidth & 0xFF0000)>>16; 
	
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Trig_Width)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 3, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Set TrigWidth error");return -1;} 

	return 0;
}

int B2BERT_SET_BURST_ParaConfig (int USB_Handle, int Pattern, unsigned short UserPatternLen, int PRBS, int InINV, int OutINV, unsigned short int SyncErrMax)
{
/*
1.��Ҫ�Ż������Զ�������ʱ���Զ�������λ������
2.��Ҫ��Ӳ�������ʱ���趨
*/
	int error;
	
	//��λ 0-1-0
	error = B2BERT_SET_BURST_Rst (USB_Handle, 0); 
	if (error) return -1;
	error = B2BERT_SET_BURST_Rst (USB_Handle, 1); 
	if (error) return -1;
	error = B2BERT_SET_BURST_Rst (USB_Handle, 0); 
	if (error) return -1;
	
	//���û���ģʽΪ����ģʽ
//	error = B2BERT_SET_BURST_Loop (USB_Handle, 0); 
//	if (error) return -1;
		
	//ѡ������Ϊm���У�0=m���У�1=�Զ�������
	error = B2BERT_SET_BURST_Pattern (USB_Handle, Pattern); 
	if (error) return -1;
	
	if (Pattern == B2BERT_PATT_USER)	//�����Զ������г��� 
	{				
		error = B2BERT_SET_BURST_UserPatternLen (USB_Handle, UserPatternLen); 
		if (error) return -1;
	}

	//ѡ��PRBS
	error = B2BERT_SET_BURST_PRBS (USB_Handle, PRBS); 
	if (error) return -1;
						  
	//�ر��Զ�����������λ
	error = B2BERT_SET_BURST_UserStart (USB_Handle, 0); 
	if (error) return -1;
	
	//�������������
	error = B2BERT_SET_BURST_INV (USB_Handle, InINV, OutINV); 
	if (error) return -1;
	   
	//ѡ���Ƿ��������
	error = B2BERT_SET_BURST_InsErrEn (USB_Handle, 0); 
	if (error) return -1;
	
	//���ò�������ʱ��
	error = B2BERT_SET_BURST_InsErrTime (USB_Handle, 0); 
	if (error) return -1;
		
	//����ͬ������
	error = B2BERT_SET_BURST_SyncErrMax (USB_Handle, SyncErrMax);
	if (error) return -1; 

	return 0;
}

int B2BERT_SET_BurstCtrlRst (int USB_Handle)
{
	int error, CtrlOK, CtrlErr; 
	int count;
	
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 0); 
	if (error) return -1;
	
	//��λ 0-1-0 
	error = B2BERT_SET_GLOBAL_BurstCtrlRst (USB_Handle, 0); 
	if (error) return -1;
	error = B2BERT_SET_GLOBAL_BurstCtrlRst (USB_Handle, 1); 
	if (error) return -1;
	error = B2BERT_SET_GLOBAL_BurstCtrlRst (USB_Handle, 0); 
	if (error) return -1;
	
	count=0;
	do
	{
		error = B2BERT_GET_GLOBAL_BurstCtrlStatus (USB_Handle, &CtrlOK, &CtrlErr);
		if (error) return -1;
		count++;
	} while (count<5 && CtrlOK!=1);
	
	if (CtrlErr)			
	{MessagePopup("Error", "B2BERT Burst Bert��������ʱ���Ʋ�����������");return -1;} 
	
	return 0; 
}

int B2BERT_SET_BURST_PreGuideLen (int USB_Handle, unsigned char PreGuideLen)
{
	int error, reg_add; 
	union uB2BERT_BURST  B2BERT_BURST; 
	
	B2BERT_BURST.sStrB2BERT_BURST.PreGuideLen = PreGuideLen;
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.PreGuideLen)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST PreGuideLen error");return -1;} 

	return 0;
}

int B2BERT_SET_BURST_OLTRst_Width (int USB_Handle, unsigned char OLTRst_Width)
{
	int error, reg_add; 
	union uB2BERT_BURST  B2BERT_BURST; 
	
	B2BERT_BURST.sStrB2BERT_BURST.OLTRst_Width = OLTRst_Width;
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.OLTRst_Width)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST OLTRst_Width error");return -1;} 

	return 0;
}

int B2BERT_SET_BURST_PreambleLen (int USB_Handle, unsigned short int PreambleLen)
{
	int error, reg_add; 
	union uB2BERT_BURST  B2BERT_BURST; 
	
	B2BERT_BURST.sStrB2BERT_BURST.PreambleLen.Len7_0  = (PreambleLen & 0xFF)>>0;
	B2BERT_BURST.sStrB2BERT_BURST.PreambleLen.Len11_8 = (PreambleLen & 0xFF00)>>8; 
		
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.PreambleLen)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 2, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST PreambleLen error");return -1;} 

	return 0;
}

int B2BERT_SET_BURST_DataLen (int USB_Handle, unsigned int DataLen)
{
	int error, reg_add; 
	union uB2BERT_BURST  B2BERT_BURST; 
	
	B2BERT_BURST.sStrB2BERT_BURST.DataLen.Len7_0  = (DataLen & 0xFF)>>0;
	B2BERT_BURST.sStrB2BERT_BURST.DataLen.Len15_8 = (DataLen & 0xFF00)>>8; 
	B2BERT_BURST.sStrB2BERT_BURST.DataLen.Len18_16= (DataLen & 0xFF0000)>>16; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.DataLen)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 3, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST DataLen error");return -1;} 

	return 0;
}

int B2BERT_SET_BURST_TxDelayLen (int USB_Handle, unsigned int TxDelayLen)
{
	int error, reg_add; 
	union uB2BERT_BURST  B2BERT_BURST; 
	
	B2BERT_BURST.sStrB2BERT_BURST.TxDelayLen.Len7_0  = (TxDelayLen & 0xFF)>>0;
	B2BERT_BURST.sStrB2BERT_BURST.TxDelayLen.Len15_8 = (TxDelayLen & 0xFF00)>>8; 
	B2BERT_BURST.sStrB2BERT_BURST.TxDelayLen.Len18_16= (TxDelayLen & 0xFF0000)>>16; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.TxDelayLen)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 3, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST TxDelayLen error");return -1;} 

	return 0;
}

int B2BERT_SET_BURST_OLTRst_Pos (int USB_Handle, unsigned short OLTRst_Pos)
{
	int error, reg_add; 
	union uB2BERT_BURST B2BERT_BURST; 
	
	B2BERT_BURST.sStrB2BERT_BURST.OLTRst_Pos.Pos7_0  = (OLTRst_Pos & 0xFF)>>0;
	B2BERT_BURST.sStrB2BERT_BURST.OLTRst_Pos.Pos11_8 = (OLTRst_Pos & 0x0F00)>>8; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.OLTRst_Pos)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 2, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Set BURST OLTRst_Pos error");return -1;} 

	return 0;
}

int B2BERT_SET_BURST_TransConfig (int USB_Handle, int Pattern, unsigned char PreGuideLen, unsigned short int PreAmbleLen, unsigned int DataLen, unsigned int TxDelay, int OLTRst_Out, unsigned short OLTRst_Pos, unsigned char OLTRst_Width, int SD_Level, unsigned char SDChk_Pos, unsigned int SDChk_Width)
{
	int error, count, User_Rdy;
	int SDA, SDD;
																																																							
	//set preguidlen
	error = B2BERT_SET_BURST_PreGuideLen (USB_Handle, PreGuideLen); 
	if (error) return -1;
	
	//set preamblelen
	error = B2BERT_SET_BURST_PreambleLen (USB_Handle, PreAmbleLen); 
	if (error) return -1;
	
	//set datalen
	error = B2BERT_SET_BURST_DataLen (USB_Handle, DataLen); 
	if (error) return -1;
	
	//set delay
	error = B2BERT_SET_BURST_TxDelayLen (USB_Handle, TxDelay); 
	if (error) return -1;
	
	//???�����Զ������Ͳ��ֲ����� �˴�δ��PRBS���������������ű�������Ҫ��֤
	if (Pattern==B2BERT_PATT_USER)
	{
		error = B2BERT_SET_BURST_UserStart (USB_Handle, 1); 
		if (error) return -1;
		
		count=0;
		do
		{
			error = B2BERT_GET_BURST_UserState (USB_Handle, &User_Rdy);
			if (error) return -1;
			count++;
		} while (count<5 && User_Rdy!=0); //User_RdyΪ0��ʾ������׼����
		
		if (count>=5) {MessagePopup("Error", "�����Զ�������ʧ��");return -1;} 
	}
	
	//OLTRst��ز������ã���global 0b��OLT_Rst_OffΪ0ʱ���
	if (B2BERT_OLTRst_OUT_OPEN==OLTRst_Out)
	{
		error = B2BERT_SET_BURST_OLTRst_Pos (USB_Handle, OLTRst_Pos); 
		if (error) return -1;
		
		error = B2BERT_SET_BURST_OLTRst_Width (USB_Handle, OLTRst_Width); 
		if (error) return -1;
	}
	
	//SDA SDD���������
	//����OLT_SD��Ч��ƽ��Ϊ0��Ϊ0��Ϊ1��Ϊ1
	error = B2BERT_SET_BURST_SD_Level (USB_Handle, SD_Level); 
	if (error) return -1;
	
	//����SDA/SDD���λ�ã������OLT_Rst�½���
	error = B2BERT_SET_BURST_SDChk_Pos (USB_Handle, SDChk_Pos); 
	if (error) return -1;
	
	//����SDA/SDD����
	error = B2BERT_SET_BURST_SDChk_Width (USB_Handle, SDChk_Width); 
	if (error) return -1;
	
	//���SDA/SDD�����
	error = B2BERT_GET_BURST_SDChk_Status (USB_Handle, &SDA, &SDD); 
	if (error) return -1;
	
	return 0; 
}

int B2BERT_GET_BURST_TrainStatus (int USB_Handle, int *Bit_OK, int *Dly_OK)
{
	int error, reg_add;
	union uB2BERT_BURST B2BERT_BURST;
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.Train_Status)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) return -1;
	
	*Bit_OK=B2BERT_BURST.sStrB2BERT_BURST.Train_Status.Bit_OK;
	*Dly_OK=B2BERT_BURST.sStrB2BERT_BURST.Train_Status.Dly_OK; 
	
	return 0;
}

int B2BERT_GET_BURST_Align (int USB_Handle, int *align)
{
	int error, count, BitOK, DlyOK;
	
	*align = 0;

	//������������֡
	error = B2BERT_SET_BURST_State (USB_Handle, 3); 
	if (error) return -1;
	
	//��λOLTRst 0-1-0 
	error = B2BERT_SET_BURST_OLTRst (USB_Handle, 0); 
	if (error) return -1;
	error = B2BERT_SET_BURST_OLTRst (USB_Handle, 1); 
	if (error) return -1;
	error = B2BERT_SET_BURST_OLTRst (USB_Handle, 0); 
	if (error) return -1;
	
	//��ѯ��ʱ�����Ƿ����		  
	count=0;
	do
	{
		error = B2BERT_GET_BURST_TrainStatus (USB_Handle, &BitOK, &DlyOK);
		if (error) return -1;
		count++;
	} while (count<5 && (DlyOK==0 || BitOK==0));
	
	if (count>=5) {return -3;} 
	
	//���ÿ���
	error = B2BERT_SET_BURST_State (USB_Handle, 5); 
	if (error) return -1;
	
	*align = 1;
	
	return 0;
}

int B2BERT_SET_BURST_Start (int USB_Handle, int ONU0State, int ONU1State)
{
	int error;
	
	//ѡ��ONU0
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 2); 
	if (error) return -1;
	
	//���ù���ģʽ
	error = B2BERT_SET_BURST_State (USB_Handle, ONU0State); 
	if (error) return -1;

	//ѡ��ONU1
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 3); 
	if (error) return -1;
	
	//���ù���ģʽ
	error = B2BERT_SET_BURST_State (USB_Handle, ONU1State); 
	if (error) return -1;
	
	return 0;
}

int B2BERT_GET_BURST_SyncStatus (int USB_Handle, int *Sync)
{
	int error, reg_add;
	union uB2BERT_BURST B2BERT_BURST;
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.SyncStatus)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Get Burst SyncStatus error");return -1;} 
	
	*Sync=B2BERT_BURST.sStrB2BERT_BURST.SyncStatus.Status;
	
	return 0;
}

int B2BERT_GET_BURST_Sync (int USB_Handle, int *sync_state)
{
	int error, count, Sync;
	
	*sync_state = 0;
	
	//SyncReStar 0-1-0
	error = B2BERT_SET_BURST_SyncReStar (USB_Handle, 0);
	if (error) 
	{
		return -1;
	} 
	error = B2BERT_SET_BURST_SyncReStar (USB_Handle, 1);
	if (error) 
	{
		return -1;
	} 
	error = B2BERT_SET_BURST_SyncReStar (USB_Handle, 0);
	if (error) 
	{
		return -1;
	} 
	
	//��ȡͬ��״̬
	count=0;
	do
	{
		error = B2BERT_GET_BURST_SyncStatus (USB_Handle, &Sync);
		if (error) 
		{
			return -1;
		}
		
		if (Sync!=1)
		{
			Delay(0.01);  /***�����δͬ������ʱ10ms�ٲ�ѯͬ��״̬**Eric.Yao**20130704***/
		}
		count++;
	} while (count<50 && Sync!=1);
	
	//���п��ܲ���ͬ��
	if (count>=50) 
	{
		*sync_state =0; 
		return -2;
	} 
	else 
	{
		*sync_state=1;
	}
	
	error = B2BERT_SET_BURST_ClearErr (USB_Handle);
	if (error) 
	{
		return -1;
	} 
	
	return 0;
}

int B2BERT_GET_BURST_ErrCount (int USB_Handle, int rate, unsigned int *ErrNum, double *ErrTime, double *ErrRate)
{
	int error, reg_add;
	unsigned int errtime_count;
	union uB2BERT_BURST B2BERT_BURST;
	
	error = B2BERT_SET_BURST_ClearErr (USB_Handle);
	if (error) return -1; 
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.ErrTime)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 8, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) return -1;
	
	*ErrNum = B2BERT_BURST.sStrB2BERT_BURST.ErrNum.ErrNum7_0+
			  B2BERT_BURST.sStrB2BERT_BURST.ErrNum.ErrNum15_8*256+
			  B2BERT_BURST.sStrB2BERT_BURST.ErrNum.ErrNum23_16*65536+
			  B2BERT_BURST.sStrB2BERT_BURST.ErrNum.ErrNum31_24*16777216;

	errtime_count= B2BERT_BURST.sStrB2BERT_BURST.ErrTime.ErrTime7_0+
				   B2BERT_BURST.sStrB2BERT_BURST.ErrTime.ErrTime15_8*256+
				   B2BERT_BURST.sStrB2BERT_BURST.ErrTime.ErrTime23_16*65536+
				   B2BERT_BURST.sStrB2BERT_BURST.ErrTime.ErrTime31_24*16777216;
	
	if (rate == B2BERT_RATE_EPON)
	{
		*ErrTime=(errtime_count+1)*8./1.25E+9;
	}
	else if (rate == B2BERT_RATE_GPON)
	{
		*ErrTime=(errtime_count+1)*8./1.24416E+9; 
	}
	else
	{MessagePopup("Error", "B2BERT select rate type error");return -1;} 
	
	*ErrRate=*ErrNum/((errtime_count+1)*8.);
	
	return 0;
}

int B2BERT_GET_NORM_ErrCount (int USB_Handle, int rate, unsigned int *ErrNum, double *ErrTime, double *ErrRate)
{
	int error, reg_add;
	unsigned int errtime_count;
	union uB2BERT_NORM B2BERT_NORM; 
	
	error = B2BERT_SET_NORM_ClearErr (USB_Handle);
	if (error) return -1; 
	
	reg_add = (int)(&B2BERT_NORM.sStrB2BERT_NORM.ErrTime)-(int)(&B2BERT_NORM.sStrB2BERT_NORM.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 8, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) return -1;
	
	*ErrNum = B2BERT_NORM.sStrB2BERT_NORM.ErrNum.ErrNum7_0+
			  B2BERT_NORM.sStrB2BERT_NORM.ErrNum.ErrNum15_8*256+
			  B2BERT_NORM.sStrB2BERT_NORM.ErrNum.ErrNum23_16*65536+
			  B2BERT_NORM.sStrB2BERT_NORM.ErrNum.ErrNum31_24*16777216;

	errtime_count= B2BERT_NORM.sStrB2BERT_NORM.ErrTime.ErrTime7_0+
				   B2BERT_NORM.sStrB2BERT_NORM.ErrTime.ErrTime15_8*256+
				   B2BERT_NORM.sStrB2BERT_NORM.ErrTime.ErrTime23_16*65536+
				   B2BERT_NORM.sStrB2BERT_NORM.ErrTime.ErrTime31_24*16777216;
	
	if (rate == B2BERT_RATE_EPON)
	{
		*ErrTime=(errtime_count+1)*16./1.25E+9;
	}
	else if (rate == B2BERT_RATE_GPON)
	{
		*ErrTime=(errtime_count+1)*16./2.48832E+9; 
	}
	else
	{MessagePopup("Error", "B2BERT select rate type error");return -1;} 
	
	*ErrRate=*ErrNum/((errtime_count+1)*8.);
	
	return 0;
}

int B2BERT_SET_NORM (int USB_Handle, int RstSel, int rate, int SelONU, int InINV, int OutINV, int Pattern, int PRBS, unsigned short int SyncErrMax)
{
	int error;
	
	//ѡȫ�ּĴ��� 
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 0); 
	if (error) return -1;
	
	if (RstSel == B2BERT_NORM_needGLOBALRst)	//ȫ�ָ�λ 0�� ���ڵ���ʹ��Normģʽʱʹ��
	{
		error = B2BERT_SET_GLOBAL_Rst (USB_Handle, 0); 
		if (error) return -1;
		error = B2BERT_SET_GLOBAL_Rst (USB_Handle, 1); 
		if (error) return -1;
		error = B2BERT_SET_GLOBAL_Rst (USB_Handle, 0); 
		if (error) return -1;
	}
	else		//����ȫ�ָ�λ 0�� Ϊ�˱����BURST�Ĵ�����Ӱ�죬ֻ�����õ�0 ����ǰ���Ѿ���BURST�Ĵ������������
	{
		error = B2BERT_SET_GLOBAL_Rst (USB_Handle, 0); 
		if (error) return -1;
	}
	
	//�������ʺ�ѡONU
	error = B2BERT_SET_RATE (USB_Handle, rate); 
	if (error) return -1;
	error = B2BERT_SET_Sel_Rx_ONU (USB_Handle, SelONU); 
	if (error) return -1;
	
	//ѡNorm�Ĵ���
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 1); 
	if (error) return -1;
	
	//��λ 0-1-0 
	error = B2BERT_SET_NORM_Rst (USB_Handle, 0); 
	if (error) return -1;
	error = B2BERT_SET_NORM_Rst (USB_Handle, 1); 
	if (error) return -1;
	error = B2BERT_SET_NORM_Rst (USB_Handle, 0); 
	if (error) return -1;
		
	//���û���ģʽΪ��������ģʽ
	error = B2BERT_SET_NORM_Loop (USB_Handle, 0, 0, 0, 0); 
	if (error) return -1;
		
	//ѡ������Ϊm���У�0=m���У�1=�Զ�������
	error = B2BERT_SET_NORM_Pattern (USB_Handle, Pattern); 
	if (error) return -1;

	//�����Զ������г��� 
	
	//ѡ��PRBS
	error = B2BERT_SET_NORM_PRBS (USB_Handle, PRBS); 
	if (error) return -1;
						  
	//�ر��Զ�����������λ
	error = B2BERT_SET_NORM_User (USB_Handle, 0); 
	if (error) return -1;
	
	//�������������
	error = B2BERT_SET_NORM_INV (USB_Handle, InINV, OutINV); 
	if (error) return -1;
	   
	//ѡ���Ƿ��������
	error = B2BERT_SET_NORM_InsErrEn (USB_Handle, 0); 
	if (error) return -1;
	
	//���ò�������ʱ��
	error = B2BERT_SET_NORM_InsErrTime (USB_Handle, 0); 
	if (error) return -1;
		
	//����ͬ������
	error = B2BERT_SET_NORM_SyncErrMax (USB_Handle, SyncErrMax);
	if (error) return -1; 
	
	return 0; 
}

int B2BERT_SET_GLOBAL_OLT_TxDis (int USB_Handle, int TxDis)
{
	int error, reg_add;
	union uB2BERT_GLOBAL B2BERT_GLOBAL;
	
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.OLT_TxDis)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) return -1;
	
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.OLT_TxDis.TxDis = TxDis;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Set GLOBAL TxDisEn error");return -1;} 
	
	return 0;
}

int B2BERT_SET_GLOBAL_PeriodTrigSel (int USB_Handle, int TrigSel)
{
	int error, reg_add;
	union uB2BERT_GLOBAL B2BERT_GLOBAL;
	
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Trigger)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) return -1;
	
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Trigger.Period_Trig_Sel = TrigSel;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Set GLOBAL TrigSel error");return -1;} 
	
	return 0;
}

int B2BERT_SET_GLOBAL_PeriodTrig (int USB_Handle, int Trig)
{
	int error, reg_add;
	union uB2BERT_GLOBAL B2BERT_GLOBAL;
	
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Trigger)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) return -1;
	
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Trigger.Period_Trig = Trig;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Set GLOBAL Trig error");return -1;} 
	
	return 0;
}

int B2BERT_SET_GLOBAL_TrigOut (int USB_Handle, int TrigOut)
{
	int error, reg_add;
	union uB2BERT_GLOBAL B2BERT_GLOBAL;
	
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Trig_Ctrl)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) return -1;
	
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Trig_Ctrl.Trig_Off = TrigOut;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Set GLOBAL TrigOut error");return -1;} 
	
	return 0;
}

int B2BERT_SET_GLOBAL_TrigLevel (int USB_Handle, int TrigLevel)
{
	int error, reg_add;
	union uB2BERT_GLOBAL B2BERT_GLOBAL;
	
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Trig_Ctrl)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) return -1;
	
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.Trig_Ctrl.Trig_Sel = TrigLevel;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Set GLOBAL TrigLevel error");return -1;} 
	
	return 0;
}

int B2BERT_SET_GLOBAL_OLTRst_Out (int USB_Handle, int OLTRstOut)
{
	int error, reg_add;
	union uB2BERT_GLOBAL B2BERT_GLOBAL;
	
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.OLTRst)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) return -1;
	
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.OLTRst.OLT_Rst_Out = OLTRstOut;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Set GLOBAL OLTRstOut error");return -1;} 
	
	return 0;
}

int B2BERT_SET_GLOBAL_OLTRst_Level (int USB_Handle, int OLTRstLevel)
{
	int error, reg_add;
	union uB2BERT_GLOBAL B2BERT_GLOBAL;
	
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.OLTRst)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) return -1;
	
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.OLTRst.OLT_Rst_Level = OLTRstLevel;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Set GLOBAL OLTRstLevel error");return -1;} 
	
	return 0;
}

int B2BERT_SET_NORM_Pattern (int USB_Handle, int Pattern)
{
	int error, reg_add;
	union uB2BERT_NORM B2BERT_NORM; 
	
	reg_add = (int)(&B2BERT_NORM.sStrB2BERT_NORM.Pattern)-(int)(&B2BERT_NORM.sStrB2BERT_NORM.FirstByte); 
	
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) return -1;
	
	B2BERT_NORM.sStrB2BERT_NORM.Pattern.isUserorPRBS = Pattern;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) {MessagePopup("Error", "B2BERT Set Norm Pattern error");return -1;} 
	
	return 0;
}

int B2BERT_SET_NORM_PRBS (int USB_Handle, int PRBS)
{
	int error, reg_add;
	union uB2BERT_NORM B2BERT_NORM; 
	
	B2BERT_NORM.sStrB2BERT_NORM.PRBS = PRBS; 
	
	reg_add = (int)(&B2BERT_NORM.sStrB2BERT_NORM.PRBS)-(int)(&B2BERT_NORM.sStrB2BERT_NORM.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) {MessagePopup("Error", "B2BERT Set Norm PRBS error");return -1;} 
	
	return 0;
}

int B2BERT_SET_NORM_User (int USB_Handle, int UserStart)
{
	int error, reg_add;
	union uB2BERT_NORM B2BERT_NORM; 
	
	reg_add = (int)(&B2BERT_NORM.sStrB2BERT_NORM.UserStatus)-(int)(&B2BERT_NORM.sStrB2BERT_NORM.FirstByte); 
	
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) return -1;
	
	B2BERT_NORM.sStrB2BERT_NORM.UserStatus.User_Start = UserStart;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) {MessagePopup("Error", "B2BERT Set Norm User state error");return -1;} 
	
	return 0;
}

int B2BERT_SET_NORM_INV (int USB_Handle, int InINV, int OutINV)
{
	int error, reg_add;
	union uB2BERT_NORM B2BERT_NORM; 
	
	reg_add = (int)(&B2BERT_NORM.sStrB2BERT_NORM.INV)-(int)(&B2BERT_NORM.sStrB2BERT_NORM.FirstByte); 
	
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) return -1;
	
	B2BERT_NORM.sStrB2BERT_NORM.INV.In_Inv = InINV;
	B2BERT_NORM.sStrB2BERT_NORM.INV.Out_Inv= OutINV;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) {MessagePopup("Error", "B2BERT Set Norm User state error");return -1;} 
	
	return 0;
}

int B2BERT_SET_NORM_AlignStar (int USB_Handle, int Align)
{
	int error, reg_add;
	union uB2BERT_NORM B2BERT_NORM; 
	
	reg_add = (int)(&B2BERT_NORM.sStrB2BERT_NORM.Align)-(int)(&B2BERT_NORM.sStrB2BERT_NORM.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) return -1;
	
	B2BERT_NORM.sStrB2BERT_NORM.Align.Align_ReStr = Align;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) {MessagePopup("Error", "B2BERT Set Norm Align Star error");return -1;} 
	
	return 0;
}

int B2BERT_GET_NORM_AlignStatus (int USB_Handle, int *AlignOK, int *Ctrl_OK, int *Ctrl_Err)
{
	int error, reg_add;
	union uB2BERT_NORM B2BERT_NORM; 
	
	reg_add = (int)(&B2BERT_NORM.sStrB2BERT_NORM.AlignStatus)-(int)(&B2BERT_NORM.sStrB2BERT_NORM.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) {MessagePopup("Error", "B2BERT Get Norm AlignOK error");return -1;}
	
	*AlignOK = B2BERT_NORM.sStrB2BERT_NORM.AlignStatus.Align_OK;
	*Ctrl_OK = B2BERT_NORM.sStrB2BERT_NORM.AlignStatus.Ctrl_OK;
	*Ctrl_Err= B2BERT_NORM.sStrB2BERT_NORM.AlignStatus.Ctrl_Err;
	
	return 0;
}

int B2BERT_SET_NORM_SyncReStar (int USB_Handle, int SyncReStar)
{
	int error, reg_add;
	union uB2BERT_NORM B2BERT_NORM; 
	
	reg_add = (int)(&B2BERT_NORM.sStrB2BERT_NORM.SyncReStar)-(int)(&B2BERT_NORM.sStrB2BERT_NORM.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) return -1;
	
	B2BERT_NORM.sStrB2BERT_NORM.SyncReStar.ReSync = SyncReStar;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) {MessagePopup("Error", "B2BERT Set Norm Align SyncReStar error");return -1;} 
	
	return 0;
}

int B2BERT_GET_NORM_SyncStatus (int USB_Handle, int *Sync)
{
	int error, reg_add;
	union uB2BERT_NORM B2BERT_NORM; 
	
	reg_add = (int)(&B2BERT_NORM.sStrB2BERT_NORM.SyncStatus)-(int)(&B2BERT_NORM.sStrB2BERT_NORM.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) {MessagePopup("Error", "B2BERT Get Norm SyncStatus error");return -1;} 
	
	*Sync = B2BERT_NORM.sStrB2BERT_NORM.SyncStatus.Status;
	
	return 0;
}

int B2BERT_SET_NORM_ClearErr (int USB_Handle)
{
	int error, reg_add;
	union uB2BERT_NORM B2BERT_NORM; 
	
	B2BERT_NORM.sStrB2BERT_NORM.ErrTime.ErrTime7_0 = 0; 
	
	reg_add = (int)(&B2BERT_NORM.sStrB2BERT_NORM.ErrTime)-(int)(&B2BERT_NORM.sStrB2BERT_NORM.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) {MessagePopup("Error", "B2BERT Set Norm ClearErr error");return -1;} 
	
	return 0;
}

int B2BERT_SET_NORM_InsErrEn (int USB_Handle, int InsErrEn)
{
	int error, reg_add;
	union uB2BERT_NORM B2BERT_NORM; 
	
	reg_add = (int)(&B2BERT_NORM.sStrB2BERT_NORM.InsErrEn)-(int)(&B2BERT_NORM.sStrB2BERT_NORM.FirstByte); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) return -1;
	
	B2BERT_NORM.sStrB2BERT_NORM.InsErrEn.InsErrEnbit0 = InsErrEn;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) {MessagePopup("Error", "B2BERT Set Norm InsErrEn error");return -1;} 
	
	return 0;
}

int B2BERT_SET_NORM_InsErrTime (int USB_Handle, unsigned int InsErrTime)
{
	int error, reg_add;
	union uB2BERT_NORM B2BERT_NORM; 
	
	B2BERT_NORM.sStrB2BERT_NORM.InsErrTime.Time7_0  = (InsErrTime & 0xFF)>>0;
	B2BERT_NORM.sStrB2BERT_NORM.InsErrTime.Time15_8 = (InsErrTime & 0xFF00)>>8; 
	B2BERT_NORM.sStrB2BERT_NORM.InsErrTime.Time23_16= (InsErrTime & 0xFF0000)>>16; 
	B2BERT_NORM.sStrB2BERT_NORM.InsErrTime.Time31_24= (InsErrTime & 0xFF000000)>>24; 
	
	reg_add = (int)(&B2BERT_NORM.sStrB2BERT_NORM.InsErrTime)-(int)(&B2BERT_NORM.sStrB2BERT_NORM.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 4, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) {MessagePopup("Error", "B2BERT Set Norm InsErrTime error");return -1;} 
	
	return 0;
}

int B2BERT_SET_NORM_SyncErrMax (int USB_Handle, unsigned short int SyncErrMax)
{
	int error, reg_add;
	union uB2BERT_NORM B2BERT_NORM; 
	
	B2BERT_NORM.sStrB2BERT_NORM.SyncErrMax.Err_Max7_0  = (SyncErrMax & 0xFF)>>0;
	B2BERT_NORM.sStrB2BERT_NORM.SyncErrMax.Err_Max13_8 = (SyncErrMax & 0xFF00)>>8; 
	
	reg_add = (int)(&B2BERT_NORM.sStrB2BERT_NORM.SyncErrMax)-(int)(&B2BERT_NORM.sStrB2BERT_NORM.FirstByte); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 2, B2BERT_NORM.pStrB2BERT_NORM); 
	if (error) {MessagePopup("Error", "B2BERT Set Norm SyncErrMax error");return -1;} 
	
	return 0;
}

int B2BERT_SET_BURST_COMMON (int USB_Handle, int rate, int SelONU, int BurstPeriod_Sel, unsigned int BurstPeriod, int Trig_Out, int Trig_Level, unsigned int Trig_Pos, unsigned int Trig_Width, int OLTRstOut, int OLTRstLevel)
{
	int error;
	
	//ѡȫ�ּĴ���
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 0); 
	if (error) return -1;
	
	//ȫ�ָ�λ 0-1-0 
	error = B2BERT_SET_GLOBAL_Rst (USB_Handle, 0); 
	if (error) return -1;
	error = B2BERT_SET_GLOBAL_Rst (USB_Handle, 1); 
	if (error) return -1;
	error = B2BERT_SET_GLOBAL_Rst (USB_Handle, 0); 
	if (error) return -1;
	
	//��������
	error = B2BERT_SET_RATE (USB_Handle, rate); 
	if (error) return -1;
	
	//ѡ��NORM����ģʽ�½��յ�ONU ΪONU0
	error = B2BERT_SET_Sel_Rx_ONU (USB_Handle, SelONU); 
	if (error) return -1;
	
	//����OLT_Rst�������
	error = B2BERT_SET_GLOBAL_OLTRst_Out (USB_Handle, OLTRstOut); 
	if (error) return -1;
		  
	//����OLT_Rst�����ƽ����
	error = B2BERT_SET_GLOBAL_OLTRst_Level (USB_Handle, OLTRstLevel); 
	if (error) return -1;
	
	//����Burst����ģʽ 0x07 ��Ϊ1�ǵ��η��ͣ�0Ϊ���ڷ���
	error = B2BERT_SET_GLOBAL_PeriodTrigSel (USB_Handle, BurstPeriod_Sel); 
	if (error) return -1;
		 
	//����BURST��������
	error = B2BERT_SET_GLOBAL_BurstPeriod (USB_Handle, BurstPeriod); 
	if (error) return -1;
	
	//����Reset����ʱ��Period��Trig��Burst_En����ʱ��ǰ��Reset
	error = B2BERT_SET_GLOBAL_OLTRst_Dly (USB_Handle, 0x05);
	if (error) return -1;
	
	error = B2BERT_SET_GLOBAL_GLOBAL_Dly (USB_Handle, 0x0E);
	if (error) return -1;
	
	//��Trigger���
	error = B2BERT_SET_GLOBAL_TrigOut (USB_Handle, Trig_Out); 
	if (error) return -1;
	
	//����Trigger��Ч��ƽģʽ 
	error = B2BERT_SET_GLOBAL_TrigLevel (USB_Handle, Trig_Level); 
	if (error) return -1;
	
	//����Trigger��Position
	error = B2BERT_SET_GLOBAL_TrigPos (USB_Handle, Trig_Pos); 
	if (error) return -1;
	
	//����Trigger��width
	error = B2BERT_SET_GLOBAL_TrigWidth (USB_Handle, Trig_Width); 
	if (error) return -1;
	
	return 0;
}

int B2BERT_GET_NORM_Align (int USB_Handle, int *align)
{
	int error, count;
	int AlignOK, Ctrl_OK, Ctrl_Err;
	
	/*
	AlignOK�� Ϊ1��ʾSFI41�ӿ�����ʱ����״̬��������ϣ�Ϊ0���ڵ���
	Ctrl_OK�� Ϊ1��ʾSFI41�ӿ�����ʱ���Ʋ�����λ��ϣ�Ϊ0���ڸ�λ
	Ctrl_Err��Ϊ1��ʾSFI41�ӿ�����ʱ���Ʋ�����������Ϊ0����
	*/
	
	//������ʱ���� 0-1-0
	error = B2BERT_SET_NORM_AlignStar (USB_Handle, 0); 
	if (error) return -1;
	error = B2BERT_SET_NORM_AlignStar (USB_Handle, 1); 
	if (error) return -1;
	error = B2BERT_SET_NORM_AlignStar (USB_Handle, 0); 
	if (error) return -1;
	
	//��ȡ�������
	count=0;
	do
	{
		error = B2BERT_GET_NORM_AlignStatus (USB_Handle, &AlignOK, &Ctrl_OK, &Ctrl_Err);
		if (error) return -1;
		count++;
	} while (count<5 && (AlignOK==0 || Ctrl_OK==0)); 
	
	//���п��ܲ���ͬ��
	if (count>=5) {*align =0; return -1;} 
	else {*align=1;}

	return 0;
}

int B2BERT_GET_NORM_Sync (int USB_Handle, int *sync_state)
{
	int error, count, Sync;
	
	*sync_state = 0;
	
	//SyncReStar 0-1-0
	error = B2BERT_SET_NORM_SyncReStar (USB_Handle, 0);
	if (error) return -1; 
	error = B2BERT_SET_NORM_SyncReStar (USB_Handle, 1);
	if (error) return -1; 
	error = B2BERT_SET_NORM_SyncReStar (USB_Handle, 0);
	if (error) return -1; 
	
	//��ȡͬ��״̬
	count=0;
	do
	{
		error = B2BERT_GET_NORM_SyncStatus (USB_Handle, &Sync);
		if (error) return -1;
		count++;
	} while (count<5 && Sync!=1);
	
	//���п��ܲ���ͬ��
	if (count>=5) {*sync_state=0; return -2;} 
	else {*sync_state=1;}
	
	error = B2BERT_SET_NORM_ClearErr (USB_Handle);
	if (error) return -1; 
	
	return 0;
}

int B2BERT_SET_NORM_Close (int USB_Handle)
{
	int error;
	
	//ѡȫ�ּĴ��� 
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 1); 
	if (error) return -1;
	
	error = B2BERT_SET_NORM_Rst (USB_Handle, 1); 
	if (error) return -1;
	
	return 0; 	
}

/*
����patt�ļ�����
�ļ���ʽ
BitLength=���ݳ��ȣ���λ��bit��+0x0A+BitData=+0x0A+{��������}
ע�⣬���õ����������ݳ�������512�ֽڣ���Ч��������0���
*/
int B2BERT_CreatPatternFile (const int datalen, unsigned char *data, char *filename)
{
	char buf[B2_USERPATT_MAX_PATTFILESIZE], bufcrc[20], *tempbuf2="BitData=";
	int  buflen, i, pos, bytelen, filehandle;
	unsigned long crc;
		
	memset (buf, 0, B2_USERPATT_MAX_PATTFILESIZE);
	
	//bit����ת����byte����ȡ��
	bytelen = datalen%8;
	if (bytelen>0)
		bytelen = datalen/8+1;
	else
		bytelen = datalen/8;	
	
	//get crc32 check byte
	crc = MyDLLCRC32 (data, bytelen);
	
	//get "BitLength=xxx"
	sprintf (buf, "BitLength=%d", datalen);
	buflen = strlen (buf);
	buf[buflen]=0x0A;		//��ӻ��з� 
	
	//get "BitCRC=xxx" 
	sprintf (bufcrc, "BitCRC=%X", crc); 
	for (i=0; i<strlen (bufcrc); i++)
	{buf[i+buflen+1]=bufcrc[i];}
	buf[i+buflen+1]=0x0A;	//��ӻ��з� 
	
	buflen = i+buflen+1; 
	
	//get "BitData=xxx"
	for (i=0; i<strlen (tempbuf2); i++)
	{buf[i+buflen+1]=tempbuf2[i];}
	buf[i+buflen+1]=0x0A;	//��ӻ��з� 
	
	pos=i+buflen+1;
	
	for (i=0; i<bytelen; i++) 
	{buf[i+pos+1]=data[i];} 	

	//get file handle
	filehandle = OpenFile (filename, VAL_READ_WRITE, VAL_TRUNCATE, VAL_BINARY);
	if (filehandle<0) {MessagePopup("Error", "open pattern file error");return -1;} 
	
	WriteFile (filehandle, buf, B2_USERPATT_MAX_PATTFILESIZE);
	
	//�ر�bin�ļ�
	CloseFile (filehandle);
	
	return 0;
}

int B2BERT_ReadPatternFile (const char *filename, unsigned short *datalen, unsigned char *data)
{
	int size, filehandle, i, mylen, bytelen;
	char buf[B2_USERPATT_MAX_PATTFILESIZE];
	char *p;
	unsigned long crc;
	
	GetFileInfo (filename, &size);
	
	//get file handle
	filehandle = OpenFile (filename, VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_BINARY);
	if (filehandle<0) {MessagePopup("Error", "open pattern file error");return -1;} 
		
	ReadFile (filehandle, buf, size);
	if (size>B2_USERPATT_MAX_PATTFILESIZE) {MessagePopup("Error", "pattern file size error");return -1;}
	
	//�ر�bin�ļ�
	CloseFile (filehandle);

	//��ȡbitlength
	sscanf(buf, "BitLength=%d", &mylen);
	*datalen = mylen;
	
	//get pattern data
	p = strstr (buf, "BitData=");
	
	bytelen = mylen%8;
	if (bytelen>0)
		bytelen = mylen/8+1;
	else
		bytelen = mylen/8;	
	
	memset (data, 0, B2_USERPATT_MAX_BYTENUMBER);
	
	for (i=0; i<bytelen; i++) 
	{data[i]=p[i+9];} 	

	//��ȡcrc
	p = strstr (buf, "BitCRC=");
	sscanf(p, "BitCRC=%X", &crc);

	if (crc != MyDLLCRC32 (data, bytelen)) {MessagePopup("Error", "pattern file check error");return -1;}       
		
	return 0;
}

int B2BERT_SET_BURST_UserPatternOpen (int USB_Handle)
{   
	int error, count, User_Rdy;

	error = B2BERT_SET_BURST_Pattern (USB_Handle, 1); 
	if (error) return -1;
	
	error = B2BERT_SET_BURST_UserStart (USB_Handle, 1); 
	if (error) return -1;
	
	count=0;
	do
	{
		error = B2BERT_GET_BURST_UserState (USB_Handle, &User_Rdy);
		if (error) return -1;
		count++;
	} while (count<5 && User_Rdy!=0); //User_RdyΪ0��ʾ������׼����
	
	if (count>=5) {MessagePopup("Error", "�����Զ�������ʧ��");return -1;} 
	
	return 0;
}

int B2BERT_SET_BURST_UserPatternClose (int USB_Handle)
{   
	int error;

	error = B2BERT_SET_BURST_Pattern (USB_Handle, 0); 
	if (error) return -1;
	
	error = B2BERT_SET_BURST_UserStart (USB_Handle, 0); 
	if (error) return -1;
	
	return 0;
}

int B2BERT_SET_CDR (int USB_Handle)
{															  
	int   error, count, LOS, LOL;
	float T_wait=0.001;
	unsigned char val;
	
//------------------------����ADN2817------------------------//
	//����LTRģʽ
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x08, 0xD9, T_wait); 
	if (error) return -1;
//	error = I2C_BYTE_READ_DLL (USB_Handle, 0x80, 0x05, &val);
//	if (error) return -1; 

	//дCtrlB
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x09, 0x00, T_wait); 
	if (error) return -1;
//	error = I2C_BYTE_READ_DLL (USB_Handle, 0x80, 0x06, &val);
//	if (error) return -1; 
	
	//дCtrlC
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x11, 0x00, T_wait); 
	if (error) return -1;
	//дCtrlD
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x22, 0x00, T_wait); 
	if (error) return -1;
	//дCtrlE
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x1F, 0x02, T_wait); 
	if (error) return -1;
	//дSel_Mode
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x34, 0x00, T_wait); 
	if (error) return -1;
	//����
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x09, 0x20, T_wait); 
	if (error) return -1;
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x09, 0x00, T_wait); 
	if (error) return -1;
	
	//�����־
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x09, 0x48, T_wait); 
	if (error) return -1;
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x09, 0x00, T_wait); 
	if (error) return -1;
//	error = I2C_BYTE_READ_DLL (USB_Handle, 0x80, 0x04, &val);
//	if (error) return -1; 
//------------------------����ADN2817------------------------// 
	
//------------------------����ADN2855------------------------// 
	//дCtrlA
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x86, 0x08, 0xD9, T_wait); 
	if (error) return -1;
//	error = I2C_BYTE_READ_DLL (USB_Handle, 0x86, 0x05, &val);
//	if (error) return -1; 
	//дCtrlB
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x86, 0x09, 0x00, T_wait); 
	if (error) return -1;
//	error = I2C_BYTE_READ_DLL (USB_Handle, 0x86, 0x06, &val);
//	if (error) return -1; 
	//дCtrlC
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x86, 0x11, 0x10, T_wait); 
	if (error) return -1;
	//дCtrlD
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x86, 0x22, 0x00, T_wait); 
	if (error) return -1;

	//����
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x86, 0x09, 0x00, T_wait); 
	if (error) return -1;
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x86, 0x09, 0x20, T_wait); 
	if (error) return -1;
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x86, 0x09, 0x00, T_wait); 
	if (error) return -1;
//------------------------����ADN2855------------------------// 
	
//------------------------���ADN2817 LOS LOL�ź�------------------------// 
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 0); 
	if (error) return -1;
	
	count=0;
	do
	{
		error = B2BERT_GET_GLOBAL_AD2817_Status (USB_Handle, &LOS, &LOL);
		if (error) return -1;
		count++;
	} while (count<5 && (LOS!=0 || LOL!=0));
	if (count>=5) {MessagePopup("Error", "AD2817�澯λ���ʧ��");return -1;}
//------------------------���ADN2817 LOS LOL�ź�------------------------// 
	
	//��λFPGA��ʱ������ 
	error = B2BERT_SET_GLOBAL_BurstCtrlRst (USB_Handle, 0); 
	if (error) return -1;
	error = B2BERT_SET_GLOBAL_BurstCtrlRst (USB_Handle, 1); 
	if (error) return -1;
	error = B2BERT_SET_GLOBAL_BurstCtrlRst (USB_Handle, 0); 
	if (error) return -1;

	//��λFPGA��λ������ 
	error = B2BERT_SET_GLOBAL_FPGA_Dly (USB_Handle, 0); 
	if (error) return -1;
	error = B2BERT_SET_GLOBAL_FPGA_Dly (USB_Handle, 1); 
	if (error) return -1;
	error = B2BERT_SET_GLOBAL_FPGA_Dly (USB_Handle, 0); 
	if (error) return -1;
	
	return 0;
}

/*
int B2BERT_GET_BURST_SDChk_Status_noClear (int USB_Handle, int *SDA, int *SDD)
{
	int error, reg_add;
	
	reg_add = (int)(&B2BERT_BURST.sStrB2BERT_BURST.SDChk_Status)-(int)(&B2BERT_BURST.sStrB2BERT_BURST.FirstByte);
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_BURST.pStrB2BERT_BURST); 
	if (error) {MessagePopup("Error", "B2BERT Get BURST SDChk_Status error");return -1;}
	
	*SDA = B2BERT_BURST.sStrB2BERT_BURST.SDChk_Status.SDA; 
	*SDD = B2BERT_BURST.sStrB2BERT_BURST.SDChk_Status.SDD;
	
	return 0;
}
*/

int B2BERT_GET_GLOBAL_AD2817_Status (int inst_B2, int *LOS, int *LOL)
{
	int error, reg_add;
	union uB2BERT_GLOBAL B2BERT_GLOBAL;
	
 	//reg_add == 0x12
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.ADN2817Status)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	error = EPP_BYTEs_READ_F320_DLL  (inst_B2, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Get AD2817 Status error");return -1;}
	
	*LOS = B2BERT_GLOBAL.sStrB2BERT_GLOBAL.ADN2817Status.BTx_LOS;
	*LOL = B2BERT_GLOBAL.sStrB2BERT_GLOBAL.ADN2817Status.BTx_LOL;
	
	return 0;
}

int B2BERT_SET_GLOBAL_FPGA_Dly (int USB_Handle, int Rst)
{
	int error, reg_add;
	union uB2BERT_GLOBAL B2BERT_GLOBAL;
	
	//reg_add == 0x63
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.AD2855_Dly)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	error = EPP_BYTEs_READ_F320_DLL  (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) return -1;
	
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.AD2855_Dly.Burst_RST_Dly = Rst;
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Set GLOBAL Burst Delay Rst error");return -1;} 
	
	return 0;
}

int B2BERT_SET_GLOBAL_OLTRst_Dly (int USB_Handle, unsigned char Dly)
{
	int error, reg_add;
	union uB2BERT_GLOBAL B2BERT_GLOBAL;
	
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.OLTRst_Dly.Rst_Delay_Control = Dly;
	
	//reg_add == 0x10
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.OLTRst_Dly)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Set GLOBAL OLTRst Delay error");return -1;} 
	
	return 0; 
}

int B2BERT_SET_GLOBAL_GLOBAL_Dly (int USB_Handle, unsigned char Dly)
{
	int error, reg_add;
	union uB2BERT_GLOBAL B2BERT_GLOBAL;
	
	B2BERT_GLOBAL.sStrB2BERT_GLOBAL.GLOBAL_Dly.En_Delay_Control = Dly;
	
	//reg_add == 0x11
	reg_add = (int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.GLOBAL_Dly)-(int)(&B2BERT_GLOBAL.sStrB2BERT_GLOBAL.FPGA_Ver); 
	error = EPP_BYTEs_WRITE_F320_DLL (USB_Handle, reg_add, 1, B2BERT_GLOBAL.pStrB2BERT_GLOBAL); 
	if (error) {MessagePopup("Error", "B2BERT Set GLOBAL Delay error");return -1;} 
	
	return 0; 
}

int BERT_Align(int USB_Handle)
{
	int error, align, count;
		
	//����BURST0��BURST0��NORM����״̬
	error = B2BERT_SET_BURST_Start (USB_Handle, 5, 5);
	if (error) 
	{
		goto Error;
	}

//---------------����CDR��FPGA��λ��ʱ--------------//
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 2); 
	if (error) 
	{
		goto Error;
	}
	
	error = B2BERT_SET_BURST_State (USB_Handle, 1); 
	if (error) return -1;
	
	//��λOLTRst 0-1-0 
	error = B2BERT_SET_BURST_OLTRst (USB_Handle, 0); 
	if (error) 
	{
		goto Error;
	}
	error = B2BERT_SET_BURST_OLTRst (USB_Handle, 1); 
	if (error) 
	{
		goto Error;
	}
	error = B2BERT_SET_BURST_OLTRst (USB_Handle, 0); 
	if (error) 
	{
		goto Error;
	}
	
	//����CDR��FPGA��ʱ
	error = B2BERT_SET_CDR (USB_Handle);
	if (error) 
	{
		goto Error;
	}
	
	//�ر�ONU1��ONU2�źŷ���
	error = B2BERT_SET_BURST_Start (USB_Handle, 0, 0);
	if (error) 
	{
		goto Error;
	}
//---------------����CDR��FPGA��λ��ʱ--------------// 
	
	//ѵ��ONU0
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 2); 
	if (error) 
	{
		goto Error;
	}
	
	count=0;
	do
	{
		error = B2BERT_GET_BURST_Align (USB_Handle, &align);
		count++;
	} while (align!=1 && count<5);
	if (error || align!=1)
	{
		MessagePopup("Error", "B2BERT set ONU main align error");
		goto Error;
	} 

	//ѵ��ONU1
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 3); 
	if (error) 
	{
		goto Error;
	}
	
	count=0;
	do
	{
		error = B2BERT_GET_BURST_Align (USB_Handle, &align);
		count++;
	} while (align!=1 && count<5);
	if (error || align!=1)
	{
		MessagePopup("Error", "B2BERT set ONU ref align error");
		goto Error;
	} 

	return 0;
Error:
	return -1;
}
