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
	SetF320I2CRate_DLL(0);//F320 I2C速率; 0=50KHz, 1=100KHz 
	
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
					if (error && (*USB_Handle-0x800)>=(lpdwNumDevices-1)) {MessagePopup("提示","F320的序列号和B2BERT的序列号不一致");return -1;}
					
					error = strcmp (B2BERT.sStrB2BERT.vendorSN, SN);
					if (error && (*USB_Handle-0x800)>=(lpdwNumDevices-1)) {MessagePopup("提示","选用的B2BERT序列号和B2BERT实际序列号不一致");return -1;}
					
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
	if (error<0) {MessagePopup("提示","B2BERT电压检测失败！");return -1;}
	
	SPI_Rate  = 119;	//速率，计算公式SCK = 12000/( ioArray[0]+1)K，如ioArray[0] = 119，则SPI速率为100K
	SCK_PHASE = 0;    	//相位，0：在SCK周期的第一个边沿采样数据，1：在SCK周期的第二个边沿采样数据
	SCK_POLARITY = 0x80;//极性，0：SCK在空闲状态时处于低电平，1：SCK在空闲状态时处于高电平
	                    //bit7自动设置CS：为1就自动设置CS，在发起SPI操作时自动置低，结束后自动置高
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
1.需要优化设置自定义码型时，自定义启动位的设置
2.需要添加插入误码时的设定
*/
	int error;
	
	//复位 0-1-0
	error = B2BERT_SET_BURST_Rst (USB_Handle, 0); 
	if (error) return -1;
	error = B2BERT_SET_BURST_Rst (USB_Handle, 1); 
	if (error) return -1;
	error = B2BERT_SET_BURST_Rst (USB_Handle, 0); 
	if (error) return -1;
	
	//设置还回模式为工作模式
//	error = B2BERT_SET_BURST_Loop (USB_Handle, 0); 
//	if (error) return -1;
		
	//选择码型为m序列，0=m序列，1=自定义序列
	error = B2BERT_SET_BURST_Pattern (USB_Handle, Pattern); 
	if (error) return -1;
	
	if (Pattern == B2BERT_PATT_USER)	//设置自定义序列长度 
	{				
		error = B2BERT_SET_BURST_UserPatternLen (USB_Handle, UserPatternLen); 
		if (error) return -1;
	}

	//选择PRBS
	error = B2BERT_SET_BURST_PRBS (USB_Handle, PRBS); 
	if (error) return -1;
						  
	//关闭自定义码型启动位
	error = B2BERT_SET_BURST_UserStart (USB_Handle, 0); 
	if (error) return -1;
	
	//输入输出不反相
	error = B2BERT_SET_BURST_INV (USB_Handle, InINV, OutINV); 
	if (error) return -1;
	   
	//选择是否插入误码
	error = B2BERT_SET_BURST_InsErrEn (USB_Handle, 0); 
	if (error) return -1;
	
	//设置插入误码时间
	error = B2BERT_SET_BURST_InsErrTime (USB_Handle, 0); 
	if (error) return -1;
		
	//设置同步门限
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
	
	//复位 0-1-0 
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
	{MessagePopup("Error", "B2BERT Burst Bert部分中延时控制部件工作错误");return -1;} 
	
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
	
	//???设置自定义码型部分参数， 此处未对PRBS码型情况作处理，与脚本不符需要验证
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
		} while (count<5 && User_Rdy!=0); //User_Rdy为0表示发送已准备好
		
		if (count>=5) {MessagePopup("Error", "发送自定义码型失败");return -1;} 
	}
	
	//OLTRst相关参数设置，当global 0b的OLT_Rst_Off为0时添加
	if (B2BERT_OLTRst_OUT_OPEN==OLTRst_Out)
	{
		error = B2BERT_SET_BURST_OLTRst_Pos (USB_Handle, OLTRst_Pos); 
		if (error) return -1;
		
		error = B2BERT_SET_BURST_OLTRst_Width (USB_Handle, OLTRst_Width); 
		if (error) return -1;
	}
	
	//SDA SDD的相关设置
	//设置OLT_SD有效电平，为0则为0，为1则为1
	error = B2BERT_SET_BURST_SD_Level (USB_Handle, SD_Level); 
	if (error) return -1;
	
	//设置SDA/SDD检测位置，相对于OLT_Rst下降沿
	error = B2BERT_SET_BURST_SDChk_Pos (USB_Handle, SDChk_Pos); 
	if (error) return -1;
	
	//设置SDA/SDD检测框
	error = B2BERT_SET_BURST_SDChk_Width (USB_Handle, SDChk_Width); 
	if (error) return -1;
	
	//清除SDA/SDD检测结果
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

	//发送连续特殊帧
	error = B2BERT_SET_BURST_State (USB_Handle, 3); 
	if (error) return -1;
	
	//复位OLTRst 0-1-0 
	error = B2BERT_SET_BURST_OLTRst (USB_Handle, 0); 
	if (error) return -1;
	error = B2BERT_SET_BURST_OLTRst (USB_Handle, 1); 
	if (error) return -1;
	error = B2BERT_SET_BURST_OLTRst (USB_Handle, 0); 
	if (error) return -1;
	
	//查询延时测量是否完成		  
	count=0;
	do
	{
		error = B2BERT_GET_BURST_TrainStatus (USB_Handle, &BitOK, &DlyOK);
		if (error) return -1;
		count++;
	} while (count<5 && (DlyOK==0 || BitOK==0));
	
	if (count>=5) {return -3;} 
	
	//设置空闲
	error = B2BERT_SET_BURST_State (USB_Handle, 5); 
	if (error) return -1;
	
	*align = 1;
	
	return 0;
}

int B2BERT_SET_BURST_Start (int USB_Handle, int ONU0State, int ONU1State)
{
	int error;
	
	//选择ONU0
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 2); 
	if (error) return -1;
	
	//设置工作模式
	error = B2BERT_SET_BURST_State (USB_Handle, ONU0State); 
	if (error) return -1;

	//选择ONU1
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 3); 
	if (error) return -1;
	
	//设置工作模式
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
	
	//读取同步状态
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
			Delay(0.01);  /***如果还未同步，延时10ms再查询同步状态**Eric.Yao**20130704***/
		}
		count++;
	} while (count<50 && Sync!=1);
	
	//极有可能不能同步
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
	
	//选全局寄存器 
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 0); 
	if (error) return -1;
	
	if (RstSel == B2BERT_NORM_needGLOBALRst)	//全局复位 0， 用于单独使用Norm模式时使用
	{
		error = B2BERT_SET_GLOBAL_Rst (USB_Handle, 0); 
		if (error) return -1;
		error = B2BERT_SET_GLOBAL_Rst (USB_Handle, 1); 
		if (error) return -1;
		error = B2BERT_SET_GLOBAL_Rst (USB_Handle, 0); 
		if (error) return -1;
	}
	else		//撤销全局复位 0， 为了避免对BURST寄存器的影响，只能设置到0 用于前面已经对BURST寄存器完成了设置
	{
		error = B2BERT_SET_GLOBAL_Rst (USB_Handle, 0); 
		if (error) return -1;
	}
	
	//配置速率和选ONU
	error = B2BERT_SET_RATE (USB_Handle, rate); 
	if (error) return -1;
	error = B2BERT_SET_Sel_Rx_ONU (USB_Handle, SelONU); 
	if (error) return -1;
	
	//选Norm寄存器
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 1); 
	if (error) return -1;
	
	//复位 0-1-0 
	error = B2BERT_SET_NORM_Rst (USB_Handle, 0); 
	if (error) return -1;
	error = B2BERT_SET_NORM_Rst (USB_Handle, 1); 
	if (error) return -1;
	error = B2BERT_SET_NORM_Rst (USB_Handle, 0); 
	if (error) return -1;
		
	//设置还回模式为正常工作模式
	error = B2BERT_SET_NORM_Loop (USB_Handle, 0, 0, 0, 0); 
	if (error) return -1;
		
	//选择码型为m序列，0=m序列，1=自定义序列
	error = B2BERT_SET_NORM_Pattern (USB_Handle, Pattern); 
	if (error) return -1;

	//设置自定义序列长度 
	
	//选择PRBS
	error = B2BERT_SET_NORM_PRBS (USB_Handle, PRBS); 
	if (error) return -1;
						  
	//关闭自定义码型启动位
	error = B2BERT_SET_NORM_User (USB_Handle, 0); 
	if (error) return -1;
	
	//输入输出不反相
	error = B2BERT_SET_NORM_INV (USB_Handle, InINV, OutINV); 
	if (error) return -1;
	   
	//选择是否插入误码
	error = B2BERT_SET_NORM_InsErrEn (USB_Handle, 0); 
	if (error) return -1;
	
	//设置插入误码时间
	error = B2BERT_SET_NORM_InsErrTime (USB_Handle, 0); 
	if (error) return -1;
		
	//设置同步门限
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
	
	//选全局寄存器
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 0); 
	if (error) return -1;
	
	//全局复位 0-1-0 
	error = B2BERT_SET_GLOBAL_Rst (USB_Handle, 0); 
	if (error) return -1;
	error = B2BERT_SET_GLOBAL_Rst (USB_Handle, 1); 
	if (error) return -1;
	error = B2BERT_SET_GLOBAL_Rst (USB_Handle, 0); 
	if (error) return -1;
	
	//设置速率
	error = B2BERT_SET_RATE (USB_Handle, rate); 
	if (error) return -1;
	
	//选择NORM工作模式下接收的ONU 为ONU0
	error = B2BERT_SET_Sel_Rx_ONU (USB_Handle, SelONU); 
	if (error) return -1;
	
	//设置OLT_Rst输出控制
	error = B2BERT_SET_GLOBAL_OLTRst_Out (USB_Handle, OLTRstOut); 
	if (error) return -1;
		  
	//设置OLT_Rst输出电平控制
	error = B2BERT_SET_GLOBAL_OLTRst_Level (USB_Handle, OLTRstLevel); 
	if (error) return -1;
	
	//设置Burst发送模式 0x07 当为1是单次发送，0为周期发送
	error = B2BERT_SET_GLOBAL_PeriodTrigSel (USB_Handle, BurstPeriod_Sel); 
	if (error) return -1;
		 
	//设置BURST发送周期
	error = B2BERT_SET_GLOBAL_BurstPeriod (USB_Handle, BurstPeriod); 
	if (error) return -1;
	
	//设置Reset的延时和Period、Trig、Burst_En的延时，前面Reset
	error = B2BERT_SET_GLOBAL_OLTRst_Dly (USB_Handle, 0x05);
	if (error) return -1;
	
	error = B2BERT_SET_GLOBAL_GLOBAL_Dly (USB_Handle, 0x0E);
	if (error) return -1;
	
	//打开Trigger输出
	error = B2BERT_SET_GLOBAL_TrigOut (USB_Handle, Trig_Out); 
	if (error) return -1;
	
	//设置Trigger有效电平模式 
	error = B2BERT_SET_GLOBAL_TrigLevel (USB_Handle, Trig_Level); 
	if (error) return -1;
	
	//设置Trigger的Position
	error = B2BERT_SET_GLOBAL_TrigPos (USB_Handle, Trig_Pos); 
	if (error) return -1;
	
	//设置Trigger的width
	error = B2BERT_SET_GLOBAL_TrigWidth (USB_Handle, Trig_Width); 
	if (error) return -1;
	
	return 0;
}

int B2BERT_GET_NORM_Align (int USB_Handle, int *align)
{
	int error, count;
	int AlignOK, Ctrl_OK, Ctrl_Err;
	
	/*
	AlignOK： 为1表示SFI41接口中延时控制状态机调整完毕，为0正在调整
	Ctrl_OK： 为1表示SFI41接口中延时控制部件复位完毕，为0正在复位
	Ctrl_Err：为1表示SFI41接口中延时控制部件工作错误，为0正常
	*/
	
	//启动延时调整 0-1-0
	error = B2BERT_SET_NORM_AlignStar (USB_Handle, 0); 
	if (error) return -1;
	error = B2BERT_SET_NORM_AlignStar (USB_Handle, 1); 
	if (error) return -1;
	error = B2BERT_SET_NORM_AlignStar (USB_Handle, 0); 
	if (error) return -1;
	
	//读取调整结果
	count=0;
	do
	{
		error = B2BERT_GET_NORM_AlignStatus (USB_Handle, &AlignOK, &Ctrl_OK, &Ctrl_Err);
		if (error) return -1;
		count++;
	} while (count<5 && (AlignOK==0 || Ctrl_OK==0)); 
	
	//极有可能不能同步
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
	
	//读取同步状态
	count=0;
	do
	{
		error = B2BERT_GET_NORM_SyncStatus (USB_Handle, &Sync);
		if (error) return -1;
		count++;
	} while (count<5 && Sync!=1);
	
	//极有可能不能同步
	if (count>=5) {*sync_state=0; return -2;} 
	else {*sync_state=1;}
	
	error = B2BERT_SET_NORM_ClearErr (USB_Handle);
	if (error) return -1; 
	
	return 0;
}

int B2BERT_SET_NORM_Close (int USB_Handle)
{
	int error;
	
	//选全局寄存器 
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 1); 
	if (error) return -1;
	
	error = B2BERT_SET_NORM_Rst (USB_Handle, 1); 
	if (error) return -1;
	
	return 0; 	
}

/*
创建patt文件函数
文件格式
BitLength=数据长度（单位是bit）+0x0A+BitData=+0x0A+{数据内容}
注意，最后得到的数据内容长度总是512字节，无效的数据用0填充
*/
int B2BERT_CreatPatternFile (const int datalen, unsigned char *data, char *filename)
{
	char buf[B2_USERPATT_MAX_PATTFILESIZE], bufcrc[20], *tempbuf2="BitData=";
	int  buflen, i, pos, bytelen, filehandle;
	unsigned long crc;
		
	memset (buf, 0, B2_USERPATT_MAX_PATTFILESIZE);
	
	//bit长度转化到byte长度取整
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
	buf[buflen]=0x0A;		//添加换行符 
	
	//get "BitCRC=xxx" 
	sprintf (bufcrc, "BitCRC=%X", crc); 
	for (i=0; i<strlen (bufcrc); i++)
	{buf[i+buflen+1]=bufcrc[i];}
	buf[i+buflen+1]=0x0A;	//添加换行符 
	
	buflen = i+buflen+1; 
	
	//get "BitData=xxx"
	for (i=0; i<strlen (tempbuf2); i++)
	{buf[i+buflen+1]=tempbuf2[i];}
	buf[i+buflen+1]=0x0A;	//添加换行符 
	
	pos=i+buflen+1;
	
	for (i=0; i<bytelen; i++) 
	{buf[i+pos+1]=data[i];} 	

	//get file handle
	filehandle = OpenFile (filename, VAL_READ_WRITE, VAL_TRUNCATE, VAL_BINARY);
	if (filehandle<0) {MessagePopup("Error", "open pattern file error");return -1;} 
	
	WriteFile (filehandle, buf, B2_USERPATT_MAX_PATTFILESIZE);
	
	//关闭bin文件
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
	
	//关闭bin文件
	CloseFile (filehandle);

	//获取bitlength
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

	//获取crc
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
	} while (count<5 && User_Rdy!=0); //User_Rdy为0表示发送已准备好
	
	if (count>=5) {MessagePopup("Error", "发送自定义码型失败");return -1;} 
	
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
	
//------------------------配置ADN2817------------------------//
	//定义LTR模式
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x08, 0xD9, T_wait); 
	if (error) return -1;
//	error = I2C_BYTE_READ_DLL (USB_Handle, 0x80, 0x05, &val);
//	if (error) return -1; 

	//写CtrlB
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x09, 0x00, T_wait); 
	if (error) return -1;
//	error = I2C_BYTE_READ_DLL (USB_Handle, 0x80, 0x06, &val);
//	if (error) return -1; 
	
	//写CtrlC
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x11, 0x00, T_wait); 
	if (error) return -1;
	//写CtrlD
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x22, 0x00, T_wait); 
	if (error) return -1;
	//写CtrlE
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x1F, 0x02, T_wait); 
	if (error) return -1;
	//写Sel_Mode
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x34, 0x00, T_wait); 
	if (error) return -1;
	//锁定
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x09, 0x20, T_wait); 
	if (error) return -1;
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x09, 0x00, T_wait); 
	if (error) return -1;
	
	//清除标志
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x09, 0x48, T_wait); 
	if (error) return -1;
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x80, 0x09, 0x00, T_wait); 
	if (error) return -1;
//	error = I2C_BYTE_READ_DLL (USB_Handle, 0x80, 0x04, &val);
//	if (error) return -1; 
//------------------------配置ADN2817------------------------// 
	
//------------------------配置ADN2855------------------------// 
	//写CtrlA
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x86, 0x08, 0xD9, T_wait); 
	if (error) return -1;
//	error = I2C_BYTE_READ_DLL (USB_Handle, 0x86, 0x05, &val);
//	if (error) return -1; 
	//写CtrlB
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x86, 0x09, 0x00, T_wait); 
	if (error) return -1;
//	error = I2C_BYTE_READ_DLL (USB_Handle, 0x86, 0x06, &val);
//	if (error) return -1; 
	//写CtrlC
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x86, 0x11, 0x10, T_wait); 
	if (error) return -1;
	//写CtrlD
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x86, 0x22, 0x00, T_wait); 
	if (error) return -1;

	//锁定
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x86, 0x09, 0x00, T_wait); 
	if (error) return -1;
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x86, 0x09, 0x20, T_wait); 
	if (error) return -1;
	error = I2C_BYTE_WRITE_DLL (USB_Handle, 0x86, 0x09, 0x00, T_wait); 
	if (error) return -1;
//------------------------配置ADN2855------------------------// 
	
//------------------------检查ADN2817 LOS LOL信号------------------------// 
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 0); 
	if (error) return -1;
	
	count=0;
	do
	{
		error = B2BERT_GET_GLOBAL_AD2817_Status (USB_Handle, &LOS, &LOL);
		if (error) return -1;
		count++;
	} while (count<5 && (LOS!=0 || LOL!=0));
	if (count>=5) {MessagePopup("Error", "AD2817告警位检查失败");return -1;}
//------------------------检查ADN2817 LOS LOL信号------------------------// 
	
	//复位FPGA延时控制器 
	error = B2BERT_SET_GLOBAL_BurstCtrlRst (USB_Handle, 0); 
	if (error) return -1;
	error = B2BERT_SET_GLOBAL_BurstCtrlRst (USB_Handle, 1); 
	if (error) return -1;
	error = B2BERT_SET_GLOBAL_BurstCtrlRst (USB_Handle, 0); 
	if (error) return -1;

	//复位FPGA相位控制器 
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
		
	//设置BURST0、BURST0、NORM工作状态
	error = B2BERT_SET_BURST_Start (USB_Handle, 5, 5);
	if (error) 
	{
		goto Error;
	}

//---------------设置CDR和FPGA相位延时--------------//
	error = B2BERT_SET_CPU_Haddr (USB_Handle, 2); 
	if (error) 
	{
		goto Error;
	}
	
	error = B2BERT_SET_BURST_State (USB_Handle, 1); 
	if (error) return -1;
	
	//复位OLTRst 0-1-0 
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
	
	//配置CDR和FPGA延时
	error = B2BERT_SET_CDR (USB_Handle);
	if (error) 
	{
		goto Error;
	}
	
	//关闭ONU1和ONU2信号发送
	error = B2BERT_SET_BURST_Start (USB_Handle, 0, 0);
	if (error) 
	{
		goto Error;
	}
//---------------设置CDR和FPGA相位延时--------------// 
	
	//训练ONU0
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

	//训练ONU1
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
