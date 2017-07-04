#include <ansi_c.h>
#include <userint.h>
#include "cvidll.h"
#include "EVB16.h"

int evb16_init (char sn[], int *instrumentHandle)
{
	int	 I2Cslave_found, lpdwNumDevices, iIndex;
	char myUSBSN[30];
	
	union uEVB16 local_evb16;  
	union uEVB16_FPGA local_evb16_fpga; 
	int error;
	unsigned char EVB16_F320_Firm_Ver;
	unsigned char EVB16_FPGA_Firm_Ver;
	int reg_add;

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
				*instrumentHandle = iIndex;

				error = I2C_SLAVE_SEARCH_DLL(iIndex, AddrEVB16);
				if (error==0) //I2C EVB found
				{
					//read F320 SN
					memset (myUSBSN, 0, sizeof(myUSBSN));
					error = GetF320SerialNumber(*instrumentHandle-0x800, myUSBSN);

					//read eeprom information from EVB_addr
					error = I2C_BYTEs_READ_DLL (*instrumentHandle, AddrEVB16, 0, 256, local_evb16.pStr);
					local_evb16.sStr.BoardSn[16]=0;
					//compare SN
					error = strcmp (local_evb16.sStr.BoardSn, myUSBSN);
					if (error && (*instrumentHandle-0x800)>=(lpdwNumDevices-1))
					{MessagePopup("提示","F320的序列号和EVB16的序列号不一致"); return -1;}

					error = strcmp (local_evb16.sStr.BoardSn, sn);
					if (error && (*instrumentHandle-0x800)>=(lpdwNumDevices-1))
					{MessagePopup("提示","选用的EVB16序列号和EVB16实际序列号不一致"); return -1;}

					if (error==0) {I2Cslave_found=1;break;}
				}
			}
		}
	}

	if (I2Cslave_found==0)
	{
		return -1;
	}

	// check vcc
	error = (local_evb16.sStr.CoreVolt/10000.0>3.6 || local_evb16.sStr.CoreVolt/10000.0<3.0);
	if (error<0) {MessagePopup("提示","EVB16电压检测失败");return -1;}

	//Set Password
//	EVB12.sStr.Password = 0x6B63614A; //"Jack"
//	reg_add =(int)(&EVB12.sStr.Password) - (int)(&EVB12.sStr.FirstByte);
//	error = I2C_BYTEs_WRITE_DLL (*inst_EVB12, EVB12_Addr, reg_add, 4, EVB12.pStr, 0.1);
//	if (error<0) {MessagePopup("提示","EVB12密码写入失败");return -1;}
	
	error = GET_Version_F320_DLL(*instrumentHandle-0x800, &EVB16_F320_Firm_Ver);

	if(EVB16_F320_Firm_Ver < 32) {MessagePopup("提示","F320 Firmware版本太低\n要求0x20及以上版本!"); return -1;}

//	USBHandle = *instrumentHandle;
	
	reg_add = (int)(&local_evb16_fpga.sStr.FPGA_Ver)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);

	REPP(*instrumentHandle, reg_add, &EVB16_FPGA_Firm_Ver);

	if(EVB16_FPGA_Firm_Ver < 32) {MessagePopup("提示","FPGA Firmware版本太低\n要求0x20及以上版本!"); return -1;}

	return 0;
}

int evb16_GetBoardSN (int panel, int control)
{
	int  lpdwNumDevices, contrlIndex, iIndex;
	char myUSBSN[30], buf[30];
	union uEVB16 local_evb16; 
	int error;

	//F320 I2C速率; 0=50KHz, 1=100KHz
	SetF320I2CRate_DLL(0);
	//read usb device
	error = GetF320Numbers(&lpdwNumDevices);
	if(error!=0 || lpdwNumDevices<=0)
	{MessagePopup("提示","读取设备的序列号失败，error code -1");return -1;}
	//read evb sn
	ClearListCtrl (panel, control);
	contrlIndex=0;
	for (iIndex=0x800; iIndex<lpdwNumDevices+0x800; iIndex++)
	{
		error = I2C_HOST_INITIALIZATION_DLL (iIndex);
		if (error!=-1) //USB host found
		{
			error = I2C_SLAVE_SEARCH_DLL(iIndex, AddrEVB16);
			if (error==0) //I2C EVB found
			{
				//read F320 SN
				memset (myUSBSN, 0, sizeof(myUSBSN));
				error = GetF320SerialNumber(iIndex-0x800, myUSBSN);

				MyDLLCheckSN (myUSBSN);

				if (strstr (myUSBSN, "SEVB16")==NULL) continue;

				//read eeprom information from EVB_addr
				error = I2C_BYTEs_READ_DLL (iIndex, AddrEVB16, 0, 256, local_evb16.pStr);
				if (error<0) {MessagePopup("提示","读取设备序列号失败，error code -2");return -1;}
				local_evb16.sStr.BoardSn[16]=0;

				InsertListItem (panel, control, contrlIndex, local_evb16.sStr.BoardSn, contrlIndex);
				contrlIndex++;
			}
		}
	}
	if (contrlIndex<1) {MessagePopup("提示","没有找到对应的EVB16，请确认选择的SEVB类型是否正确");return -1;}

	return 0;
}

//XfpOut=1, 关闭XFP输出， ClockSrc=0，选择内部时钟
int evb16_SetClkSrc (int instrumentHandle, int XfpOut, int ClockSrc)
{
	union uEVB16 local_evb16; 
	int error;
	int reg_add;
/*
    INT8U RefClk:1;                   // Defined by hardware
    INT8U Freq:3;
    INT8U GnRst:1;
    INT8U Desel:1;
    INT8U PdRst:1;
    INT8U TxDis:1;
*/
	//0xD0
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16, 0, 256, local_evb16.pStr);
	if (error) {MessagePopup("Error", "evb16 get regedit error"); return -1;}
	
	local_evb16.sStr.ArmOutput.TxDis = 1;
	local_evb16.sStr.ArmOutput.PdRst = 1;
	local_evb16.sStr.ArmOutput.Desel = 0;
	local_evb16.sStr.ArmOutput.GnRst = 1;
	local_evb16.sStr.ArmOutput.Freq =  0;
	local_evb16.sStr.ArmOutput.RefClk = ClockSrc;
	reg_add = (int)(&local_evb16.sStr.ArmOutput)-(int)(&local_evb16.sStr.FlashCtl);
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, 1, local_evb16.pStr, T_wait);
	if (error) {MessagePopup("Error", "evb16 set regedit error"); return -1;}

	if (XfpOut) //关闭XFP输出
	{
		//0xC0
		local_evb16.sStr.ArmOutput.TxDis = 1;
		local_evb16.sStr.ArmOutput.PdRst = 1;
		local_evb16.sStr.ArmOutput.Desel = 0;
		local_evb16.sStr.ArmOutput.GnRst = 0;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, 1, local_evb16.pStr, T_wait);
		if (error) {MessagePopup("Error", "evb16 set regedit error"); return -1;}

		//0xD0
		local_evb16.sStr.ArmOutput.TxDis = 1;
		local_evb16.sStr.ArmOutput.PdRst = 1;
		local_evb16.sStr.ArmOutput.Desel = 0;
		local_evb16.sStr.ArmOutput.GnRst = 1;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, 1, local_evb16.pStr, T_wait);
		if (error) {MessagePopup("Error", "evb16 set regedit error"); return -1;}
	}
	else
	{
		//0x80
		local_evb16.sStr.ArmOutput.TxDis = 1;
		local_evb16.sStr.ArmOutput.PdRst = 0;
		local_evb16.sStr.ArmOutput.Desel = 0;
		local_evb16.sStr.ArmOutput.GnRst = 0;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, 1, local_evb16.pStr, T_wait);
		if (error) {MessagePopup("Error", "evb16 set regedit error"); return -1;}

		//0x10
		local_evb16.sStr.ArmOutput.TxDis = 0;
		local_evb16.sStr.ArmOutput.PdRst = 0;
		local_evb16.sStr.ArmOutput.Desel = 0;
		local_evb16.sStr.ArmOutput.GnRst = 1;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, 1, local_evb16.pStr, T_wait);
		if (error) {MessagePopup("Error", "evb16 set regedit error"); return -1;}
	}

	return 0;
}

int evb16_GetCoreTemp (int instrumentHandle, float *Temper)
{
	union uEVB16 local_evb16; 
	int error;
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16, 0, 256, local_evb16.pStr);
	if (error) {MessagePopup("Error", "evb16 get regedit error"); return -1;}

	*Temper = local_evb16.sStr.CoreTemp/256.;

	return 0;
}

int evb16_GetCoreVolt (int instrumentHandle, float *Volt)
{
	union uEVB16 local_evb16; 
	int error;

	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16, 0, 256, local_evb16.pStr);
	if (error) {MessagePopup("Error", "evb16 get regedit error"); return -1;}

	*Volt = local_evb16.sStr.CoreVolt/10000.;

	return 0;
}

int evb16_SetVsc8479Reset (int instrumentHandle)
{
	union uEVB16_VSC8479 local_evb16_vsc8479; 
	
	int error;
	int reg_add;
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16_8479, 0, 128, local_evb16_vsc8479.pStr);
	if (error) {MessagePopup("Error", "evb16 get regedit error"); return -1;}
	
	local_evb16_vsc8479.sStr.RESET=0;

	reg_add = (int)(&local_evb16_vsc8479.sStr.RESET)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) {MessagePopup("Error", "evb16 set Vsc8479 error"); return -1;}

	return 0;
}

int evb16_SetVsc8479RxCtrlSrc (int instrumentHandle, int bisten)
{
	union uEVB16_VSC8479 local_evb16_vsc8479;
	
	int error;
	int reg_add;
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16_8479, 0, 128, local_evb16_vsc8479.pStr);
	if (error) {MessagePopup("Error", "evb16 get regedit error"); return -1;}
	
	//0x3B
	local_evb16_vsc8479.sStr.RXCTRLSRC.BISTEN    	= bisten;
	local_evb16_vsc8479.sStr.RXCTRLSRC.Reserved6_6= 0x00;
	local_evb16_vsc8479.sStr.RXCTRLSRC.RXDOUTMUTEB= 0x01;
	local_evb16_vsc8479.sStr.RXCTRLSRC.RXMSBSELB	= 0x01;
	local_evb16_vsc8479.sStr.RXCTRLSRC.RXDATAINV	= 0x01;
	local_evb16_vsc8479.sStr.RXCTRLSRC.Reserved2_2= 0x00;
	local_evb16_vsc8479.sStr.RXCTRLSRC.RXLOCKREFB	= 0x01;
	local_evb16_vsc8479.sStr.RXCTRLSRC.RXLOSDATSQB= 0x01;

	reg_add = (int)(&local_evb16_vsc8479.sStr.RXCTRLSRC)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) {MessagePopup("Error", "evb16 set Vsc8479 error"); return -1;}

	return 0;
}

int evb16_SetVsc8479TxClkSrc (int instrumentHandle)
{
	union uEVB16_VSC8479 local_evb16_vsc8479;
	
	int error;
	int reg_add;
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16_8479, 0, 128, local_evb16_vsc8479.pStr);
	if (error) {MessagePopup("Error", "evb16 get regedit error"); return -1;}
	
	//0xFF
	local_evb16_vsc8479.sStr.TXCLKSRC.TXMCLKMUTEB = 0x01;
	local_evb16_vsc8479.sStr.TXCLKSRC.TXMCLKSELF  = 0x01;
	local_evb16_vsc8479.sStr.TXCLKSRC.TXPHSADJ    = 0x03;
	local_evb16_vsc8479.sStr.TXCLKSRC.TXPCLKMUTEB = 0x01;
	local_evb16_vsc8479.sStr.TXCLKSRC.TXCOUTMUTEB = 0x01;
	local_evb16_vsc8479.sStr.TXCLKSRC.TXREFSELF   = 0x01;
	local_evb16_vsc8479.sStr.TXCLKSRC.TXREFSRC    = 0x01;

	reg_add = (int)(&local_evb16_vsc8479.sStr.TXCLKSRC)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) {MessagePopup("Error", "evb16 set Vsc8479 error"); return -1;}

	return 0;
}

int evb16_SetVsc8479Txfifosrc (int instrumentHandle)
{
	union uEVB16_VSC8479 local_evb16_vsc8479; 
	
	int error;
	int reg_add;
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16_8479, 0, 128, local_evb16_vsc8479.pStr);
	if (error) {MessagePopup("Error", "evb16 get regedit error"); return -1;}
	
	//0xFB
	local_evb16_vsc8479.sStr.TXFIFOSRC.EXREFSELF 	= 0x01;
	local_evb16_vsc8479.sStr.TXFIFOSRC.PDREFSRC 	= 0x01;
	local_evb16_vsc8479.sStr.TXFIFOSRC.PDEN  		= 0x01;
	local_evb16_vsc8479.sStr.TXFIFOSRC.TXMSBSELB  = 0x01;
	local_evb16_vsc8479.sStr.TXFIFOSRC.TXDATAINV  = 0x01;
	local_evb16_vsc8479.sStr.TXFIFOSRC.Reserved 	= 0x00;
	local_evb16_vsc8479.sStr.TXFIFOSRC.TXFIFORARB = 0x01;
	local_evb16_vsc8479.sStr.TXFIFOSRC.TXFIFORESB = 0x01;

	reg_add = (int)(&local_evb16_vsc8479.sStr.TXFIFOSRC)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) {MessagePopup("Error", "evb16 set Vsc8479 error"); return -1;}

	return 0;
}

int evb16_SetVsc8479RxClkSrc  (int instrumentHandle)
{
	union uEVB16_VSC8479 local_evb16_vsc8479;
	
	int error;
	int reg_add;
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16_8479, 0, 128, local_evb16_vsc8479.pStr);
	if (error) {MessagePopup("Error", "evb16 get regedit error"); return -1;}
	
	//0xFF
	local_evb16_vsc8479.sStr.RXCLKSRC.TXLINETIMSELB   = 0x01;
	local_evb16_vsc8479.sStr.RXCLKSRC.RXDLOOPENB   	= 0x01;
	local_evb16_vsc8479.sStr.RXCLKSRC.TXLLOOPENB 		= 0x01;
	local_evb16_vsc8479.sStr.RXCLKSRC.RXMCLKMUTEB		= 0x01;
	local_evb16_vsc8479.sStr.RXCLKSRC.RXMCLKSELF    	= 0x01;
	local_evb16_vsc8479.sStr.RXCLKSRC.RXPOCLKMUTEB  	= 0x01;
	local_evb16_vsc8479.sStr.RXCLKSRC.RXREFSELF 		= 0x01;
	local_evb16_vsc8479.sStr.RXCLKSRC.RXREFSRC 		= 0x01;

	reg_add = (int)(&local_evb16_vsc8479.sStr.RXCLKSRC)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) {MessagePopup("Error", "evb16 set Vsc8479 error"); return -1;}

	return 0;
}

int evb16_SetVsc8479Init (int instrumentHandle)
{
	union uEVB16_VSC8479 local_evb16_vsc8479;
	
	int error;
	int reg_add;
	
	error = evb16_SetVsc8479TxClkSrc (instrumentHandle);
	if (error) return -1;
	error = evb16_SetVsc8479Txfifosrc (instrumentHandle);
	if (error) return -1;
	error = evb16_SetVsc8479RxClkSrc (instrumentHandle);
	if (error) return -1;
	error = evb16_SetVsc8479RxCtrlSrc (instrumentHandle, 1);
	if (error) return -1;
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16_8479, 0, 128, local_evb16_vsc8479.pStr);
	if (error) {MessagePopup("Error", "evb16 get regedit error"); return -1;}

	reg_add = (int)(&local_evb16_vsc8479.sStr.STATUSSRC)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	local_evb16_vsc8479.pStr[reg_add] = 0x00;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) return -1;

//	local_evb16_vsc8479.sStr.TXDUOBINSRC = 0x01;
	reg_add = (int)(&local_evb16_vsc8479.sStr.TXDUOBINSRC)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	local_evb16_vsc8479.pStr[reg_add] = 0x01;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) return -1;

//	local_evb16_vsc8479.sStr.STATUSMODE = 0x9F;
	reg_add = (int)(&local_evb16_vsc8479.sStr.STATUSMODE)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	local_evb16_vsc8479.pStr[reg_add] = 0x9F;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) return -1;

//	local_evb16_vsc8479.sStr.TXCLK  = 0x0B;
	reg_add = (int)(&local_evb16_vsc8479.sStr.TXCLK)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	local_evb16_vsc8479.pStr[reg_add] = 0x0B;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) return -1;

//	local_evb16_vsc8479.sStr.TXFIFO = 0xC5;
	reg_add = (int)(&local_evb16_vsc8479.sStr.TXFIFO)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	local_evb16_vsc8479.pStr[reg_add] = 0xC5;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) return -1;

//	local_evb16_vsc8479.sStr.RXCLK = 0xE7;
	reg_add = (int)(&local_evb16_vsc8479.sStr.RXCLK)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	local_evb16_vsc8479.pStr[reg_add] = 0xE7;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) return -1;

//	local_evb16_vsc8479.sStr.STATUS = 0x0C;
	reg_add = (int)(&local_evb16_vsc8479.sStr.STATUS)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	local_evb16_vsc8479.pStr[reg_add] = 0x0F;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) return -1;

//	local_evb16_vsc8479.sStr.RXCTRL = 0x27;
	reg_add = (int)(&local_evb16_vsc8479.sStr.RXCTRL)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	local_evb16_vsc8479.pStr[reg_add] = 0x27;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) return -1;

//	local_evb16_vsc8479.sStr.RXGAIN = 0x7F;
	reg_add = (int)(&local_evb16_vsc8479.sStr.RXGAIN)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	local_evb16_vsc8479.pStr[reg_add] = 0x7F;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) return -1;

//	local_evb16_vsc8479.sStr.RXBOOST = 0x00;
	reg_add = (int)(&local_evb16_vsc8479.sStr.RXBOOST)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	local_evb16_vsc8479.pStr[reg_add] = 0x00;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) return -1;

//	local_evb16_vsc8479.sStr.RXOC = 0x00;
	reg_add = (int)(&local_evb16_vsc8479.sStr.RXOC)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	local_evb16_vsc8479.pStr[reg_add] = 0x00;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) return -1;

//	local_evb16_vsc8479.sStr.RXLOSCTRL = 0x2C;
	reg_add = (int)(&local_evb16_vsc8479.sStr.RXLOSCTRL)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	local_evb16_vsc8479.pStr[reg_add] = 0x2C;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) return -1;

//	local_evb16_vsc8479.sStr.RXLOSTHR = 0x15;
	reg_add = (int)(&local_evb16_vsc8479.sStr.RXLOSTHR)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	local_evb16_vsc8479.pStr[reg_add] = 0x15;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) return -1;

//	local_evb16_vsc8479.sStr.TXODSW = 0x07;
	reg_add = (int)(&local_evb16_vsc8479.sStr.TXODSW)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	local_evb16_vsc8479.pStr[reg_add] = 0x07;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) return -1;

//	local_evb16_vsc8479.sStr.TXODPE = 0x12;
	reg_add = (int)(&local_evb16_vsc8479.sStr.TXODPE)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	local_evb16_vsc8479.pStr[reg_add] = 0x12;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) return -1;

//	local_evb16_vsc8479.sStr.TXODSLEW = 0x05;
	reg_add = (int)(&local_evb16_vsc8479.sStr.TXODSLEW)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	local_evb16_vsc8479.pStr[reg_add] = 0x05;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) return -1;

//	local_evb16_vsc8479.sStr.TXDUOBIN = 0x00;
	reg_add = (int)(&local_evb16_vsc8479.sStr.TXDUOBIN)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	local_evb16_vsc8479.pStr[reg_add] = 0x00;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) return -1;

//	local_evb16_vsc8479.sStr.BISTCTRL = 0x02;
	reg_add = (int)(&local_evb16_vsc8479.sStr.BISTCTRL)-(int)(&local_evb16_vsc8479.sStr.TXCLKSRC);
	local_evb16_vsc8479.pStr[reg_add] = 0x02;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_8479, reg_add, 1, local_evb16_vsc8479.pStr, T_wait);
	if (error) return -1;

	return 0;
}

int evb16_SetFPGARst (int instrumentHandle, int rst)
{
	union uEVB16_FPGA local_evb16_fpga; 
	int error;
	int reg_add;
	
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, 0, 128, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}
	
	reg_add = (int)(&local_evb16_fpga.sStr.Reset)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, reg_add, 1, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}

	local_evb16_fpga.sStr.Reset.Rst = rst;
	error = EPP_BYTEs_WRITE_F320_DLL (instrumentHandle, reg_add, 1, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 set FPGA error"); return -1;}

	return 0;
}

int evb16_SetFPGAAlignStart (int instrumentHandle, int Align)
{
	union uEVB16_FPGA local_evb16_fpga;
	int error;
	int reg_add;
	
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, 0, 128, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}
	
	//需要验证脚本0xFF值与此处的0x01的区别
	reg_add = (int)(&local_evb16_fpga.sStr.AlignStart)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
	local_evb16_fpga.sStr.AlignStart.start = Align;
	error = EPP_BYTEs_WRITE_F320_DLL (instrumentHandle, reg_add, 1, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 set FPGA error"); return -1;}

	return 0;
}

int evb16_GetFPGAAlignStatus (int instrumentHandle, int *AlignOK, int *Ctrl_OK, int *Ctrl_Err)
{
	union uEVB16_FPGA local_evb16_fpga;
	int error;
	int reg_add;
	
	reg_add = (int)(&local_evb16_fpga.sStr.AlignStatus)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, reg_add, 1, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}

	*AlignOK = local_evb16_fpga.sStr.AlignStatus.Rx_Align_OK;
	*Ctrl_OK = local_evb16_fpga.sStr.AlignStatus.Rx_Ctrl_OK;
	*Ctrl_Err= local_evb16_fpga.sStr.AlignStatus.Rx_Ctrl_Err;

	return 0;
}

int evb16_SetFPGASyncStart (int instrumentHandle, int Sync)
{
	union uEVB16_FPGA local_evb16_fpga;
	int error;
	int reg_add;
	
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, 0, 128, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}
	
	//需要验证脚本0xFF值与此处的0x01的区别
	reg_add = (int)(&local_evb16_fpga.sStr.SyncStart)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
	local_evb16_fpga.sStr.SyncStart.start = Sync;
	error = EPP_BYTEs_WRITE_F320_DLL (instrumentHandle, reg_add, 1, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 set FPGA error"); return -1;}

	return 0;
}

int evb16_GetFPGASyncStatus (int instrumentHandle, int *SyncOK)
{
	union uEVB16_FPGA local_evb16_fpga;
	int error;
	int reg_add;
	
	//0x22
	reg_add = (int)(&local_evb16_fpga.sStr.SyncStatus)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, reg_add, 1, local_evb16_fpga.pStr);
	if (error) 
	{
	//	MessagePopup("Error", "evb16 get FPGA error"); 
		return -1;
	}

	*SyncOK = local_evb16_fpga.sStr.SyncStatus.Sync_OK;

	return 0;
}

int evb16_GetFPGARxLosLol(int instrumentHandle,int signal)
{
	int Rxlos, Rxlol;
	int error;

	Rxlos = 0;
	Rxlol = 0;
	if (signal==0) //0 XFP信号输入
	{
		//get RxCDR_Rx los lock
		error = evb16_GetGn2012RxCDR_Tx (instrumentHandle, &Rxlos, &Rxlol);
		if (error) return -1;
	}
	else
	{
		//get RxCDR_Rx los lock
		error = evb16_GetGn2012RxCDR_Rx (instrumentHandle, &Rxlos, &Rxlol);
		if (error) return -1;
	}
	//如果不是CDR lock error,则进行同步
	if ((0==Rxlos) && (0==Rxlol))
	{
		return 0;
	}
	return 1;
}

int evb16_SetFPGASyncRestart(int instrumentHandle)
{
	int count,Sync_Ok;
	int error;
	
	//启动NORM部分VSC8479和FPGA的接口延时调整
	error = evb16_SetFPGASyncStart (instrumentHandle, 0);
	if (error) return -1;
	error = evb16_SetFPGASyncStart (instrumentHandle, 1);
	if (error) return -1;
	error = evb16_SetFPGASyncStart (instrumentHandle, 0);
	if (error) return -1;

	//读取调整结果
	count=0;
	do
	{
		error = evb16_GetFPGASyncStatus (instrumentHandle, &Sync_Ok);
		if (error) return -1;
		count++;
	} while (count<5 && Sync_Ok==0);

	return 0;
}

int evb16_SetFPGAWorkmode (int instrumentHandle, int mode)
{
	union uEVB16_FPGA local_evb16_fpga;
	int error;
	int reg_add;
	
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, 0, 128, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}
	
	reg_add = (int)(&local_evb16_fpga.sStr.WorkMode)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);

	local_evb16_fpga.sStr.WorkMode = mode;
	error = EPP_BYTEs_WRITE_F320_DLL (instrumentHandle, reg_add, 1, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 set FPGA error"); return -1;}

	return 0;
}

int evb16_SetFPGALoop (int instrumentHandle)
{
	union uEVB16_FPGA local_evb16_fpga;
	int error;
	int reg_add;
	
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, 0, 128, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}

	local_evb16_fpga.sStr.Loop.Rx2Tx_Loop = 0;
	local_evb16_fpga.sStr.Loop.Tx2Rx_Loop = 0;
	local_evb16_fpga.sStr.Loop.Reserved2_3 = 0;
	local_evb16_fpga.sStr.Loop.XFP_LOS = 0;
	local_evb16_fpga.sStr.Loop.XFP_NR = 0;
	local_evb16_fpga.sStr.Loop.XFP_ABS = 0;
	local_evb16_fpga.sStr.Loop.Reserved7 = 0;

	reg_add = (int)(&local_evb16_fpga.sStr.Loop)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
	error = EPP_BYTEs_WRITE_F320_DLL (instrumentHandle, reg_add, 1, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 set FPGA error"); return -1;}

	return 0;
}

int evb16_SetFPGAPattern (int instrumentHandle, int Pattern)
{
	union uEVB16_FPGA local_evb16_fpga;
	int error;
	int reg_add;
	
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, 0, 128, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}
	
	local_evb16_fpga.sStr.Pattern.isUserorPRBS = Pattern;

	reg_add = (int)(&local_evb16_fpga.sStr.Pattern)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
	error = EPP_BYTEs_WRITE_F320_DLL (instrumentHandle, reg_add, 1, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 set FPGA error"); return -1;}

	return 0;
}

int evb16_SetFPGAPRBS (int instrumentHandle, int PRBS)
{
	union uEVB16_FPGA local_evb16_fpga;
	int error;
	int reg_add;
	
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, 0, 128, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}
	
	local_evb16_fpga.sStr.PRBS = PRBS;

	reg_add = (int)(&local_evb16_fpga.sStr.PRBS)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
	error = EPP_BYTEs_WRITE_F320_DLL (instrumentHandle, reg_add, 1, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 set FPGA error"); return -1;}

	return 0;
}

int evb16_SetFPGAUserStart (int instrumentHandle, int Userstart)
{
	union uEVB16_FPGA local_evb16_fpga;
	int error;
	int reg_add;
	
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, 0, 128, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}
	
	reg_add = (int)(&local_evb16_fpga.sStr.UserStatus)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, reg_add, 1, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}

	local_evb16_fpga.sStr.UserStatus.User_Start = Userstart;
	error = EPP_BYTEs_WRITE_F320_DLL (instrumentHandle, reg_add, 1, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 set FPGA error"); return -1;}

	return 0;
}

int evb16_SetFPGAInv (int instrumentHandle, int InINV, int OutINV)
{
	union uEVB16_FPGA local_evb16_fpga;
	int error;
	int reg_add;
	
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, 0, 128, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}
	
	reg_add = (int)(&local_evb16_fpga.sStr.INV)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, reg_add, 1, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}

	local_evb16_fpga.sStr.INV.In_Inv  = InINV;
	local_evb16_fpga.sStr.INV.Out_Inv = OutINV;
	error = EPP_BYTEs_WRITE_F320_DLL (instrumentHandle, reg_add, 1, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 set FPGA error"); return -1;}

	return 0;
}

int evb16_SetFPGAInsErrEn  (int instrumentHandle, int InsErrEn)
{
	union uEVB16_FPGA local_evb16_fpga;
	int error;
	int reg_add;
	
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, 0, 128, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}
	
	reg_add = (int)(&local_evb16_fpga.sStr.InsErrEn)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, reg_add, 1, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}

	local_evb16_fpga.sStr.InsErrEn.En  = InsErrEn;
	error = EPP_BYTEs_WRITE_F320_DLL (instrumentHandle, reg_add, 1, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 set FPGA error"); return -1;}

	return 0;
}

int evb16_SetFPGAInsErrTime (int instrumentHandle, unsigned int InsErrTime)
{
	union uEVB16_FPGA local_evb16_fpga;
	int error;
	int reg_add;
	
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, 0, 128, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}
	
	local_evb16_fpga.sStr.InsErrTime.Time7_0  = (InsErrTime & 0xFF)>>0;
	local_evb16_fpga.sStr.InsErrTime.Time15_8 = (InsErrTime & 0xFF00)>>8;
	local_evb16_fpga.sStr.InsErrTime.Time23_16= (InsErrTime & 0xFF0000)>>16;
	local_evb16_fpga.sStr.InsErrTime.Time31_24= (InsErrTime & 0xFF000000)>>24;

	reg_add = (int)(&local_evb16_fpga.sStr.InsErrTime)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
	error = EPP_BYTEs_WRITE_F320_DLL (instrumentHandle, reg_add, 4, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 set FPGA error"); return -1;}

	return 0;
}

int evb16_SetFPGASyncErrMax (int instrumentHandle, unsigned short int SyncErrMax)
{
	union uEVB16_FPGA local_evb16_fpga;
	int error;
	int reg_add;
	
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, 0, 128, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}
	
	local_evb16_fpga.sStr.SyncErrMax.ErrMax7_0  = (SyncErrMax & 0xFF)>>0;
	local_evb16_fpga.sStr.SyncErrMax.ErrMax13_8 = (SyncErrMax & 0xFF00)>>8;

	reg_add = (int)(&local_evb16_fpga.sStr.SyncErrMax)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
	error = EPP_BYTEs_WRITE_F320_DLL (instrumentHandle, reg_add, 2, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 set FPGA error"); return -1;}

	return 0;
}

int evb16_SetGn2012InitTx (int instrumentHandle)
{
	int count, LOS, LOL;  
	union uEVB16_GN2012 local_evb16_gn2012;
	int error;
	int reg_add;
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16_TX2012, 0, 256, local_evb16_gn2012.pStr);
	if (error) {MessagePopup("Error", "evb16 get GN2010 error"); return -1;}
	
	//0x04
	reg_add = (int)(&local_evb16_gn2012.sStr.TOP_REG4)-(int)(&local_evb16_gn2012.sStr.RSVD0);
	local_evb16_gn2012.pStr[reg_add] = 0x0D;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_TX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
	if (error) return -1;

	//0x07
	reg_add = (int)(&local_evb16_gn2012.sStr.LOOPBK)-(int)(&local_evb16_gn2012.sStr.RSVD0);
	local_evb16_gn2012.pStr[reg_add+0] = 0x30;
	local_evb16_gn2012.pStr[reg_add+1] = 0x00;
	local_evb16_gn2012.pStr[reg_add+2] = 0x1C;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_TX2012, reg_add, 3, local_evb16_gn2012.pStr, T_wait);
	if (error) return -1;

	//0x0E
	reg_add = (int)(&local_evb16_gn2012.sStr.TXPLL_REG5)-(int)(&local_evb16_gn2012.sStr.RSVD0);
	local_evb16_gn2012.pStr[reg_add] = 0x20;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_TX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
	if (error) return -1;
	//0x12
	reg_add = (int)(&local_evb16_gn2012.sStr.TXPLL_REG9)-(int)(&local_evb16_gn2012.sStr.RSVD0);
	local_evb16_gn2012.pStr[reg_add] = 0x90;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_TX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
	if (error) return -1;
	//0x18
	reg_add = (int)(&local_evb16_gn2012.sStr.RXPLL_REG5)-(int)(&local_evb16_gn2012.sStr.RSVD0);
	local_evb16_gn2012.pStr[reg_add] = 0x21;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_TX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
	if (error) return -1;

	//0x65
	reg_add = (int)(&local_evb16_gn2012.sStr.TXSDO_REG27)-(int)(&local_evb16_gn2012.sStr.RSVD0);
//	local_evb16_gn2012.pStr[reg_add] = 0x08;  版本C输出幅度改为0x0F较好
	local_evb16_gn2012.pStr[reg_add] = 0x0F;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_TX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
	if (error) return -1;
	//0x66
	reg_add = (int)(&local_evb16_gn2012.sStr.TXSDO_REG28)-(int)(&local_evb16_gn2012.sStr.RSVD0);
//	local_evb16_gn2012.pStr[reg_add] = 0x01;
	local_evb16_gn2012.pStr[reg_add] = 0x02;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_TX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
	if (error) return -1;

	//0x1E
	reg_add = (int)(&local_evb16_gn2012.sStr.RSVD30to74[0])-(int)(&local_evb16_gn2012.sStr.RSVD0);
	local_evb16_gn2012.pStr[reg_add] = 0x50;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_TX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
	if (error) return -1;
	//0x1F
	reg_add = (int)(&local_evb16_gn2012.sStr.RSVD30to74[1])-(int)(&local_evb16_gn2012.sStr.RSVD0);
	local_evb16_gn2012.pStr[reg_add] = 0x01;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_TX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
	if (error) return -1;
	//0x21
	reg_add = (int)(&local_evb16_gn2012.sStr.RSVD30to74[3])-(int)(&local_evb16_gn2012.sStr.RSVD0);
	local_evb16_gn2012.pStr[reg_add] = 0x01;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_TX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
	if (error) return -1;

	//0x4B
	reg_add = (int)(&local_evb16_gn2012.sStr.RXSDO)-(int)(&local_evb16_gn2012.sStr.RSVD0);
	local_evb16_gn2012.pStr[reg_add+0] = 0x0E;
	local_evb16_gn2012.pStr[reg_add+1] = 0x05;
	local_evb16_gn2012.pStr[reg_add+2] = 0x00;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_TX2012, reg_add, 3, local_evb16_gn2012.pStr, T_wait);
	if (error) return -1;

	//0x82
	reg_add = (int)(&local_evb16_gn2012.sStr.TXPWRDN)-(int)(&local_evb16_gn2012.sStr.RSVD0);
	local_evb16_gn2012.pStr[reg_add+0] = 0x00;
	local_evb16_gn2012.pStr[reg_add+1] = 0x00;
	local_evb16_gn2012.pStr[reg_add+2] = 0x7F;
	local_evb16_gn2012.pStr[reg_add+3] = 0x0E;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_TX2012, reg_add, 4, local_evb16_gn2012.pStr, T_wait);
	if (error) return -1;

	//0x86
	reg_add = (int)(&local_evb16_gn2012.sStr.RXPWRDN)-(int)(&local_evb16_gn2012.sStr.RSVD0);
	local_evb16_gn2012.pStr[reg_add+0] = 0x0A;
	local_evb16_gn2012.pStr[reg_add+1] = 0x07;
	local_evb16_gn2012.pStr[reg_add+2] = 0x7F;
	local_evb16_gn2012.pStr[reg_add+3] = 0x0F;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_TX2012, reg_add, 4, local_evb16_gn2012.pStr, T_wait);
	if (error) return -1;

	//0x8B
	reg_add = (int)(&local_evb16_gn2012.sStr.PWRDN)-(int)(&local_evb16_gn2012.sStr.RSVD0);
	local_evb16_gn2012.pStr[reg_add+0] = 0x3F;
	local_evb16_gn2012.pStr[reg_add+1] = 0x0F;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_TX2012, reg_add, 2, local_evb16_gn2012.pStr, T_wait);
	if (error) return -1;

	//0x13
	count=0;
	do
	{
		error = evb16_GetGn2012TxCDR (instrumentHandle, &LOS, &LOL);
		if (error) return -1;
		count++;
	} while (count<20 && (LOS!=0 || LOL!=0));

	if (count>=20) {MessagePopup("Error", "evb16 get TX CDR lock error"); return -1;}

	return 0;
}

int evb16_GetGn2012TxCDR (int instrumentHandle, int *LOS, int *LOL)
{
	union uEVB16_GN2012 local_evb16_gn2012;
	int error;
	int reg_add;
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16_TX2012, 0, 256, local_evb16_gn2012.pStr);
	if (error) {MessagePopup("Error", "evb16 get GN2010 error"); return -1;}
	
	//0x13
	reg_add = (int)(&local_evb16_gn2012.sStr.TXPLL_REG10)-(int)(&local_evb16_gn2012.sStr.RSVD0);
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16_TX2012, 0, 256, local_evb16_gn2012.pStr);
	if (error) {MessagePopup("Error", "evb16 get GN2012 error"); return -1;}

	*LOS = local_evb16_gn2012.sStr.TXPLL_REG10.TXPLLLOS;
	*LOL = local_evb16_gn2012.sStr.TXPLL_REG10.TXPLLLOL;

	return 0;
}

int evb16_SetGn2012InitRx (int instrumentHandle, int signal)
{
	int count, LOS, LOL;
	union uEVB16_GN2012 local_evb16_gn2012;
	int error;
	int reg_add;
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16_TX2012, 0, 256, local_evb16_gn2012.pStr);
	if (error) {MessagePopup("Error", "evb16 get GN2010 error"); return -1;}

	//0x02 需要按工作方式配置
	if (signal==0) //0 XFP信号输入
	{
		//0x02
		reg_add = (int)(&local_evb16_gn2012.sStr.TOP_REG2)-(int)(&local_evb16_gn2012.sStr.RSVD0);
		local_evb16_gn2012.pStr[reg_add] = 0x0E;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_RX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
		if (error) return -1;
		//0x04
		reg_add = (int)(&local_evb16_gn2012.sStr.TOP_REG4)-(int)(&local_evb16_gn2012.sStr.RSVD0);
		local_evb16_gn2012.pStr[reg_add] = 0x0D;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_RX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
		if (error) return -1;

		//0x07
		reg_add = (int)(&local_evb16_gn2012.sStr.LOOPBK)-(int)(&local_evb16_gn2012.sStr.RSVD0);
		local_evb16_gn2012.pStr[reg_add+0] = 0x30;
		local_evb16_gn2012.pStr[reg_add+1] = 0x00;
		local_evb16_gn2012.pStr[reg_add+2] = 0x1C;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_RX2012, reg_add, 2, local_evb16_gn2012.pStr, T_wait);
		if (error) return -1;
	}
	else
	{
		//0x04
		reg_add = (int)(&local_evb16_gn2012.sStr.TOP_REG4)-(int)(&local_evb16_gn2012.sStr.RSVD0);
		local_evb16_gn2012.pStr[reg_add] = 0x1A;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_RX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
		if (error) return -1;

		//0x07
		reg_add = (int)(&local_evb16_gn2012.sStr.LOOPBK)-(int)(&local_evb16_gn2012.sStr.RSVD0);
		local_evb16_gn2012.pStr[reg_add+0] = 0x00;
		local_evb16_gn2012.pStr[reg_add+1] = 0x00;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_RX2012, reg_add, 2, local_evb16_gn2012.pStr, T_wait);
		if (error) return -1;
	}

	//0x0E
	reg_add = (int)(&local_evb16_gn2012.sStr.TXPLL_REG5)-(int)(&local_evb16_gn2012.sStr.RSVD0);
	local_evb16_gn2012.pStr[reg_add] = 0x20;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_RX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
	if (error) return -1;

	if (signal==0) //0 XFP信号输入
	{
		//0x12
		reg_add = (int)(&local_evb16_gn2012.sStr.TXPLL_REG9)-(int)(&local_evb16_gn2012.sStr.RSVD0);
		local_evb16_gn2012.pStr[reg_add] = 0x90;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_RX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
		if (error) return -1;
	}
	else
	{
		//0x12
		reg_add = (int)(&local_evb16_gn2012.sStr.TXPLL_REG9)-(int)(&local_evb16_gn2012.sStr.RSVD0);
		local_evb16_gn2012.pStr[reg_add] = 0x22;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_RX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
		if (error) return -1;
	}
	//0x18
	reg_add = (int)(&local_evb16_gn2012.sStr.RXPLL_REG5)-(int)(&local_evb16_gn2012.sStr.RSVD0);
	local_evb16_gn2012.pStr[reg_add] = 0x21;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_RX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
	if (error) return -1;

	if (signal==0) //0 XFP信号输入
	{
		//0x65
		reg_add = (int)(&local_evb16_gn2012.sStr.TXSDO_REG27)-(int)(&local_evb16_gn2012.sStr.RSVD0);
		local_evb16_gn2012.pStr[reg_add] = 0x0E;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_RX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
		if (error) return -1;
		//0x66
		reg_add = (int)(&local_evb16_gn2012.sStr.TXSDO_REG28)-(int)(&local_evb16_gn2012.sStr.RSVD0);
		local_evb16_gn2012.pStr[reg_add] = 0x05;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_RX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
		if (error) return -1;
		//1E,1F,21恢复缺省值
		//0x1E
		reg_add = (int)(&local_evb16_gn2012.sStr.RSVD30to74[0])-(int)(&local_evb16_gn2012.sStr.RSVD0);
		local_evb16_gn2012.pStr[reg_add] = 0x00;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_TX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
		if (error) return -1;
		//0x1F
		reg_add = (int)(&local_evb16_gn2012.sStr.RSVD30to74[1])-(int)(&local_evb16_gn2012.sStr.RSVD0);
		local_evb16_gn2012.pStr[reg_add] = 0x00;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_TX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
		if (error) return -1;
		//0x21
		reg_add = (int)(&local_evb16_gn2012.sStr.RSVD30to74[3])-(int)(&local_evb16_gn2012.sStr.RSVD0);
		local_evb16_gn2012.pStr[reg_add] = 0x01;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_TX2012, reg_add, 1, local_evb16_gn2012.pStr, T_wait);
		if (error) return -1;

		//0x4B
		reg_add = (int)(&local_evb16_gn2012.sStr.RXSDO)-(int)(&local_evb16_gn2012.sStr.RSVD0);
		local_evb16_gn2012.pStr[reg_add+0] = 0x0E;
		local_evb16_gn2012.pStr[reg_add+1] = 0x05;
		local_evb16_gn2012.pStr[reg_add+2] = 0x00;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_RX2012, reg_add, 3, local_evb16_gn2012.pStr, T_wait);
		if (error) return -1;

		//0x82
		reg_add = (int)(&local_evb16_gn2012.sStr.TXPWRDN)-(int)(&local_evb16_gn2012.sStr.RSVD0);
		local_evb16_gn2012.pStr[reg_add+0] = 0x10;
		local_evb16_gn2012.pStr[reg_add+1] = 0x00;
		local_evb16_gn2012.pStr[reg_add+2] = 0x7F;
		local_evb16_gn2012.pStr[reg_add+3] = 0x0E;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_RX2012, reg_add, 4, local_evb16_gn2012.pStr, T_wait);
		if (error) return -1;

		//0x86
		reg_add = (int)(&local_evb16_gn2012.sStr.RXPWRDN)-(int)(&local_evb16_gn2012.sStr.RSVD0);
		local_evb16_gn2012.pStr[reg_add+0] = 0x0A;
		local_evb16_gn2012.pStr[reg_add+1] = 0x07;
		local_evb16_gn2012.pStr[reg_add+2] = 0x7F;
		local_evb16_gn2012.pStr[reg_add+3] = 0x0F;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_RX2012, reg_add, 4, local_evb16_gn2012.pStr, T_wait);
		if (error) return -1;
	}
	else
	{
		//0x4D
		reg_add = (int)(&local_evb16_gn2012.sStr.RXSDO)-(int)(&local_evb16_gn2012.sStr.RSVD0);
		local_evb16_gn2012.pStr[reg_add+2] = 0x00;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_RX2012, reg_add+2, 1, local_evb16_gn2012.pStr, T_wait);
		if (error) return -1;

		//0x82
		reg_add = (int)(&local_evb16_gn2012.sStr.TXPWRDN)-(int)(&local_evb16_gn2012.sStr.RSVD0);
		local_evb16_gn2012.pStr[reg_add+0] = 0x1B;
		local_evb16_gn2012.pStr[reg_add+1] = 0x83;
		local_evb16_gn2012.pStr[reg_add+2] = 0x7F;
		local_evb16_gn2012.pStr[reg_add+3] = 0x0F;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_RX2012, reg_add, 4, local_evb16_gn2012.pStr, T_wait);
		if (error) return -1;

		//0x86
		reg_add = (int)(&local_evb16_gn2012.sStr.RXPWRDN)-(int)(&local_evb16_gn2012.sStr.RSVD0);
		local_evb16_gn2012.pStr[reg_add+0] = 0x00;
		local_evb16_gn2012.pStr[reg_add+1] = 0x00;
		local_evb16_gn2012.pStr[reg_add+2] = 0x77;
		local_evb16_gn2012.pStr[reg_add+3] = 0x0F;
		error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_RX2012, reg_add, 4, local_evb16_gn2012.pStr, T_wait);
		if (error) return -1;
	}

	//0x8B
	reg_add = (int)(&local_evb16_gn2012.sStr.PWRDN)-(int)(&local_evb16_gn2012.sStr.RSVD0);
	local_evb16_gn2012.pStr[reg_add+0] = 0x3F;
	local_evb16_gn2012.pStr[reg_add+1] = 0x0F;
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16_RX2012, reg_add, 2, local_evb16_gn2012.pStr, T_wait);
	if (error) return -1;
//  接收方向的CDR是否锁定，最好放到Start时去判断，在初始化时不判断。
/*
	if (signal==0) //0 XFP信号输入
	{
		//0x13
		count=0;
		do
		{
			error = evb16_GetGn2012RxCDR_Tx (instrumentHandle, &LOS, &LOL);
			if (error) return -1;
			count++;
		} while (count<20 && (LOS!=0 || LOL!=0));

		if (count>=20) {MessagePopup("Error", "evb16 get RX CDR_Tx lock error"); return -1;}
	}
	else
	{
		//0x1D
		count=0;
		do
		{
			error = evb16_GetGn2012RxCDR_Rx (instrumentHandle, &LOS, &LOL);
			if (error) return -1;
			count++;
		} while (count<20 && (LOS!=0 || LOL!=0));

		if (count>=20) {MessagePopup("Error", "evb16 get RX CDR_Rx lock error"); return -1;}
	}
*/
	return 0;
}

int evb16_GetGn2012RxCDR_Tx (int instrumentHandle, int *LOS, int *LOL)
{
	union uEVB16_GN2012 local_evb16_gn2012;
	int error;
	int reg_add;
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16_TX2012, 0, 256, local_evb16_gn2012.pStr);
	if (error) {MessagePopup("Error", "evb16 get GN2010 error"); return -1;}
	
	//0x13
	reg_add = (int)(&local_evb16_gn2012.sStr.TXPLL_REG10)-(int)(&local_evb16_gn2012.sStr.RSVD0);
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16_RX2012, 0, 256, local_evb16_gn2012.pStr);
	if (error) {MessagePopup("Error", "evb16 get GN2012 error"); return -1;}

	*LOS = local_evb16_gn2012.sStr.TXPLL_REG10.TXPLLLOS;
	*LOL = local_evb16_gn2012.sStr.TXPLL_REG10.TXPLLLOL;

	return 0;
}

int evb16_GetGn2012RxCDR_Rx (int instrumentHandle, int *LOS, int *LOL)
{
	union uEVB16_GN2012 local_evb16_gn2012;
	int error;
	int reg_add;
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16_TX2012, 0, 256, local_evb16_gn2012.pStr);
	if (error) {MessagePopup("Error", "evb16 get GN2010 error"); return -1;}
	
	//0x1D
	reg_add = (int)(&local_evb16_gn2012.sStr.RXPLL_REG10)-(int)(&local_evb16_gn2012.sStr.RSVD0);
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16_RX2012, 0, 256, local_evb16_gn2012.pStr);
	if (error) {MessagePopup("Error", "evb16 get GN2012 error"); return -1;}

	*LOS = local_evb16_gn2012.sStr.RXPLL_REG10.RXPLLLOS;
	*LOL = local_evb16_gn2012.sStr.RXPLL_REG10.RXPLLLOL;

	return 0;
}

int evb16_SetFPGAClearErr (int instrumentHandle)
{
	union uEVB16_FPGA local_evb16_fpga;
	int error;
	int reg_add;
	
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, 0, 128, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}
	
	local_evb16_fpga.sStr.ErrTime.ErrTime7_0 = 0;

	reg_add = (int)(&local_evb16_fpga.sStr.ErrTime)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
	error = EPP_BYTEs_WRITE_F320_DLL (instrumentHandle, reg_add, 1, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 set FPGA error"); return -1;}

	return 0;
}

int evb16_GetFPGAErrCount (int instrumentHandle, int rate, unsigned long long *ErrNum, double *ErrTime, double *ErrRate)
{
	unsigned int errtime;
	int error;
	int reg_add;
	
	union uEVB16_FPGA local_evb16_fpga; 

	error = evb16_SetFPGAClearErr (instrumentHandle);
	if (error) return -1;

	reg_add = (int)(&local_evb16_fpga.sStr.ErrTime)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
	error = EPP_BYTEs_READ_F320_DLL  (instrumentHandle, reg_add, 10, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}

	*ErrNum = (unsigned long long)local_evb16_fpga.sStr.ErrNum.ErrNum7_0+
			  (unsigned long long)local_evb16_fpga.sStr.ErrNum.ErrNum15_8*256+
			  (unsigned long long)local_evb16_fpga.sStr.ErrNum.ErrNum23_16*256*256+
			  (unsigned long long)local_evb16_fpga.sStr.ErrNum.ErrNum31_24*256*256*256+
			  (unsigned long long)local_evb16_fpga.sStr.ErrNum.ErrNum39_32*256*256*256*256;

	errtime = (unsigned long long)local_evb16_fpga.sStr.ErrTime.ErrTime7_0+
			  (unsigned long long)local_evb16_fpga.sStr.ErrTime.ErrTime15_8*256+
			  (unsigned long long)local_evb16_fpga.sStr.ErrTime.ErrTime23_16*256*256+
			  (unsigned long long)local_evb16_fpga.sStr.ErrTime.ErrTime31_24*256*256*256+
			  (unsigned long long)local_evb16_fpga.sStr.ErrTime.ErrTime39_32*256*256*256*256;


	//此处需要确定速率
	*ErrTime=(double)(errtime+1)*64./10.3125E+9;
	*ErrRate=(double)*ErrNum/(((double)errtime+1)*64.);


	return 0;
}   

int evb16_SetBERTInit (int instrumentHandle, int inv_out, int inv_in, int sma_out, int xfp_out, int pattern, int clk, int signal)
{
	int error;
	//XfpOut=1, 关闭XFP输出， ClockSrc=0，选择内部时钟
	error = evb16_SetClkSrc (instrumentHandle, xfp_out, clk);
	if (error) return -1;

	error = evb16_SetVsc8479Reset (instrumentHandle);
	if (error) return -1;

	error = evb16_SetVsc8479RxCtrlSrc (instrumentHandle, 0);
	if (error) return -1;

	//FPGA复位 0-1-0
	error = evb16_SetFPGARst (instrumentHandle, 0);
	if (error) return -1;
	error = evb16_SetFPGARst (instrumentHandle, 1);
	if (error) return -1;
	error = evb16_SetFPGARst (instrumentHandle, 0);
	if (error) return -1;

	//选择SDH或PRBS输出 SDH=1
	error = evb16_SetFPGAWorkmode (instrumentHandle, 0);
	if (error) return -1;

	//内部还回，02 TxtoRx, 01:RxtoTx, 00:正常工作
	error = evb16_SetFPGALoop (instrumentHandle);
	if (error) return -1;

	//Pattern为1表示测试序列为用户自定义序列，为0表示是m序列
	error = evb16_SetFPGAPattern (instrumentHandle, 0);
	if (error) return -1;

	error = evb16_SetFPGAPRBS (instrumentHandle, pattern);
	if (error) return -1;

	error = evb16_SetFPGAUserStart (instrumentHandle, 0);
	if (error) return -1;

	error = evb16_SetFPGAInv (instrumentHandle, inv_in, inv_out);
	if (error) return -1;

	error = evb16_SetFPGAInsErrEn (instrumentHandle, 0);
	if (error) return -1;

	error = evb16_SetFPGAInsErrTime (instrumentHandle, 0x00FF0000);
	if (error) return -1;

	error = evb16_SetFPGASyncErrMax (instrumentHandle, 0x05C6);
	if (error) return -1;

	//临时使用
	error = evb16_SetVsc8479Init (instrumentHandle);
	if (error) return -1;

	error = evb16_SetGn2012InitTx (instrumentHandle);
	if (error) return -1;

	error = evb16_SetGn2012InitRx (instrumentHandle, signal);
	if (error) return -1;

	return 0;
}

int evb16_SetBERTStart (int instrumentHandle, int *align, int *sync, int Signal)
{
	int AlignOK, Ctrl_OK, Ctrl_Err, Sync_Ok;
	int count, LOS, LOL;
	int error;

// 先判断接收方向CDR是否锁定
	if (signal==0) //0 XFP信号输入
	{
		//0x13
		count=0;
		do
		{
			error = evb16_GetGn2012RxCDR_Tx (instrumentHandle, &LOS, &LOL);
			if (error) return -1;
			count++;
		} while (count<20 && (LOS!=0 || LOL!=0));

		if (count>=20) 
		{
			//MessagePopup("Error", "evb16 get RX CDR_Tx lock error"); 
			return -1;
		}
	}
	else		//同轴头信号输入
	{
		//0x1D
		count=0;
		do
		{
			error = evb16_GetGn2012RxCDR_Rx (instrumentHandle, &LOS, &LOL);
			if (error) return -1;
			count++;
		} while (count<20 && (LOS!=0 || LOL!=0));

		if (count>=20) 
		{
		//	MessagePopup("Error", "evb16 get RX CDR_Rx lock error"); 
			return -1;
		}
	}

	*align=0;
	*sync =0;

	//启动NORM部分VSC8479和FPGA的接口延时调整
	error = evb16_SetFPGAAlignStart (instrumentHandle, 0);
	if (error) return -1;
	error = evb16_SetFPGAAlignStart (instrumentHandle, 1);
	if (error) return -1;
	error = evb16_SetFPGAAlignStart (instrumentHandle, 0);
	if (error) return -1;

	//读取调整结果
	count=0;
	do
	{
		error = evb16_GetFPGAAlignStatus (instrumentHandle, &AlignOK, &Ctrl_OK, &Ctrl_Err);
		if (error) return -1;
		count++;
	} while (count<5 && (AlignOK==0 || Ctrl_OK==0));

	//极有可能不能同步
	if (count>=5) {*align = 0; return -1;}
	else {*align=1;}

	//启动PRBS同步调整
	error = evb16_SetFPGASyncStart (instrumentHandle, 0);
	if (error) return -1;
	error = evb16_SetFPGASyncStart (instrumentHandle, 1);
	if (error) return -1;
	error = evb16_SetFPGASyncStart (instrumentHandle, 0);
	if (error) return -1;

	//读取调整结果
	count=0;
	do
	{
		error = evb16_GetFPGASyncStatus (instrumentHandle, &Sync_Ok);
		if (error) return -1;
		count++;
	} while (count<5 && Sync_Ok==0);

	//极有可能不能同步
	if (count>=5) {*sync = 0; return -1;}
	else {*sync=1;}

	error = evb16_SetFPGAClearErr (instrumentHandle);
	if (error) return -1;

	return 0;
}

int evb16_GetBERTStatus (int instrumentHandle, int signal, int *lock, int *sync, unsigned long long *ErrNum, double *ErrTime, double *ErrRate, char StatusString[])
{
	int AlignOK, Ctrl_OK, Ctrl_Err, Sync_Ok;
	int Txlos, Txlol, Rxlos, Rxlol;
	
	int error;

	//读取CDR状态
	*lock=0;

	//get TxCDR los lock
	error = evb16_GetGn2012TxCDR (instrumentHandle, &Txlos, &Txlol);
	if (error) return -1;

	if (signal==0) //0 XFP信号输入
	{
		//get RxCDR_Rx los lock
		error = evb16_GetGn2012RxCDR_Tx (instrumentHandle, &Rxlos, &Rxlol);
		if (error) return -1;
	}
	else
	{
		//get RxCDR_Rx los lock
		error = evb16_GetGn2012RxCDR_Rx (instrumentHandle, &Rxlos, &Rxlol);
		if (error) return -1;
	}

	if (Txlos!=0 || Txlol!=0 || Rxlos!=0 || Rxlol!=0)
	{
		*lock=0;
	}
	else
	{
		*lock=1;
	}

	//读取同步状态
	*sync=0;

	//get align
	error = evb16_GetFPGAAlignStatus (instrumentHandle, &AlignOK, &Ctrl_OK, &Ctrl_Err);
	if (error) return -1;

	//get sync
	error = evb16_GetFPGASyncStatus (instrumentHandle, &Sync_Ok);
	if (error) return -1;

	if (AlignOK==0 || Ctrl_OK==0 || Sync_Ok==0)
	{
//		strcpy(StatusString, "Initial configuration error;");
		*sync=0;
	}
	else
	{
		*sync=1;
	}
	
	//rate变量暂时没有使用
	error = evb16_GetFPGAErrCount (instrumentHandle, 0, ErrNum, ErrTime, ErrRate);
	if (error) return -1;

	return 0;
}

int WEPP(int handle, unsigned char EPP_ADD, unsigned char EPP_DAT)
{int error;
 unsigned char rom_value_arr[256];

 	rom_value_arr[EPP_ADD] = EPP_DAT;
	error = EPP_BYTEs_WRITE_F320_DLL  (handle, EPP_ADD, 1, rom_value_arr);

	return error;
}

int REPP(int handle, unsigned char EPP_ADD, unsigned char *EPP_DAT)
{int error;
 unsigned char rom_value_arr[256];

	error = EPP_BYTEs_READ_F320_DLL  (handle, EPP_ADD, 1, rom_value_arr);
	*EPP_DAT = rom_value_arr[EPP_ADD];

	return error;
}

int WI2C(int handle, unsigned char device_ADD, unsigned char I2C_ADD, unsigned char I2C_DAT)
{int error;
 unsigned char rom_value_arr[256];

	rom_value_arr[I2C_ADD] = I2C_DAT;
	error = I2C_BYTEs_WRITE_DLL (handle, device_ADD, I2C_ADD, 1, rom_value_arr, 0.001);

	return error;
}

int RI2C(int handle, unsigned char device_ADD, unsigned char I2C_ADD, unsigned char *I2C_DAT)
{int error;
 unsigned char rom_value_arr[256];

	error = I2C_BYTEs_READ_DLL (handle, device_ADD, I2C_ADD, 1, rom_value_arr);
	*I2C_DAT = rom_value_arr[I2C_ADD];

	return error;
}
//=================================================================================================
// Si570 functions
#define FDCO_MAX 5670
#define FDCO_MIN 4850
unsigned char HS_DIV[6] = {11, 9, 7, 6, 5, 4};
//unsigned char HS_DIV[6] = {4, 5, 6, 7, 9, 11};
unsigned char INITIAL_HSDIV, INITIAL_N1;
unsigned long INITIAL_RFREQ_MSB,INITIAL_RFREQ_LSB;
double INITIAL_RFREQ;
double FOUT0=622.08;//出厂输出频率622.08MHz
double FXTAL;//晶振频率

//根据出厂值计算HSDIV,N1,RFREQ,FXTAL
void CalculateFactoryFrequency(union uEVB16 *EVB16)
{
	union uEVB16 local_evb16;
	
	local_evb16=*EVB16;
	//然后根据出厂值计算HSDIV,N1,RFREQ,FXTAL
	//Calculate HSDIV and N1
	INITIAL_HSDIV = ((local_evb16.sStr.Si570Regs.R7to12[0] & 0xE0) >> 5) + 4; // Get value fo INITIAL_HSDIV from REG[0]
	INITIAL_N1 = (( local_evb16.sStr.Si570Regs.R7to12[0] & 0x1F ) << 2 ) + (( local_evb16.sStr.Si570Regs.R7to12[1] & 0xC0 ) >> 6 );
	if(INITIAL_N1 == 0)
	{
		INITIAL_N1 = 1;// This is a corner case of N1
	}
	else if(INITIAL_N1 & 1 != 0)
	{
		INITIAL_N1 = INITIAL_N1 + 1; // As per datasheet, illegal odd divider values should be rounded up to the nearest even value
	}
	// Read initial value for RFREQ.
	INITIAL_RFREQ_LSB = ( local_evb16.sStr.Si570Regs.R7to12[2] & 0x0F ) * pow (2, 24)
						   + local_evb16.sStr.Si570Regs.R7to12[3] * pow (2, 16)
						   + local_evb16.sStr.Si570Regs.R7to12[4] * pow (2, 8)
						   + local_evb16.sStr.Si570Regs.R7to12[5] * pow (2, 0);
	INITIAL_RFREQ_MSB =  (( local_evb16.sStr.Si570Regs.R7to12[1] & 0x3F ) << 4 ) + (( local_evb16.sStr.Si570Regs.R7to12[2] & 0xF0 ) >> 4 ) ;


	// RFREQ conversion (reconstruct the integer portion from the registers)
	INITIAL_RFREQ = INITIAL_RFREQ_LSB;
	INITIAL_RFREQ = INITIAL_RFREQ /(1<<28);
	INITIAL_RFREQ += INITIAL_RFREQ_MSB;

	// Crystal Frequency (FXTAL) calculation
	FXTAL = (FOUT0 * INITIAL_N1 * INITIAL_HSDIV) / INITIAL_RFREQ;

	*EVB16 = local_evb16;
}
//根据寄存器值计算对应的频率
double CalculateFrequencyForReg(union uEVB16 local_evb16)
{
	unsigned char CURRENT_HSDIV, CURRENT_N1;
	unsigned long CURRENT_RFREQ_MSB,CURRENT_RFREQ_LSB;
	double CURRENT_RFREQ;
	double FOUT_CURRENT;
	//然后根据寄存器值计算HSDIV,N1,RFREQ
	//Calculate HSDIV and N1
	CURRENT_HSDIV = ((local_evb16.sStr.Si570R7 & 0xE0) >> 5) + 4; // Get value fo CURRENT_HSDIV from REG[0]
	CURRENT_N1 = (( local_evb16.sStr.Si570R7 & 0x1F ) << 2 ) + (( local_evb16.sStr.Si570R8 & 0xC0 ) >> 6 );
	if(CURRENT_N1 == 0)
	{
		CURRENT_N1 = 1;// This is a corner case of N1
	}
	else if(CURRENT_N1 & 1 != 0)
	{
		CURRENT_N1 = CURRENT_N1 + 1; // As per datasheet, illegal odd divider values should be rounded up to the nearest even value
	}
	// Read initial value for RFREQ.
	CURRENT_RFREQ_LSB = ( local_evb16.sStr.Si570R9 & 0x0F ) * pow (2, 24)
						   + local_evb16.sStr.Si570R10 * pow (2, 16)
						   + local_evb16.sStr.Si570R11 * pow (2, 8)
						   + local_evb16.sStr.Si570R12 * pow (2, 0);
	CURRENT_RFREQ_MSB =  (( local_evb16.sStr.Si570R8 & 0x3F ) << 4 ) + (( local_evb16.sStr.Si570R9 & 0xF0 ) >> 4 ) ;


	// RFREQ conversion (reconstruct the integer portion from the registers)
	CURRENT_RFREQ = CURRENT_RFREQ_LSB;
	CURRENT_RFREQ = CURRENT_RFREQ /(1<<28);
	CURRENT_RFREQ += CURRENT_RFREQ_MSB;

	//计算当前频率
	FOUT_CURRENT = ( FXTAL * CURRENT_RFREQ)/( INITIAL_N1 * INITIAL_HSDIV );
	return  FOUT_CURRENT;
}
//根据设置的新频率计算对应的寄存器值
int CalculateRegForNewFrequency(union uEVB16 *EVB16, double FOUT1)
{
	unsigned __int64 NEW_RFREQ_LONG;
	//unsigned long NEW_RFREQ_MSB,NEW_RFREQ_LSB,NEW_RFREQ_LONG;
	double NEW_RFREQ;
	unsigned int divider_max,divider_min,divider_curr;
	unsigned char hs_div,n1,n1_tmp;
	unsigned char i, div_index, valid_flag;  
	union uEVB16 local_evb16;
	
	local_evb16 = *EVB16;

	//根据出厂值计算HSDIV,N1,RFREQ,FXTAL
	CalculateFactoryFrequency(&local_evb16);

	//接下来根据新的设置频率进行计算
	// Find dividers (get the max and min divider range for the HS_DIV and N1)
	divider_max =  floor(FDCO_MAX / FOUT1);
	divider_min =  ceil(FDCO_MIN / FOUT1);
	valid_flag = 0;
	for(i=0;i<=129;i+=2)
	{
		if(0==i) n1=1;
		else n1=i;
		for(div_index=0;div_index<6;div_index++)
		{
			 hs_div = HS_DIV[div_index];
			 if(((hs_div * n1) >= divider_min)
			    && ((hs_div * n1) <= divider_max))
			 {
			 	valid_flag = 1;
				break;
			 }
		}
		if(1 == valid_flag) break;
	}
	if(0 == valid_flag) return -1;
	/*
	divider_max =  floor(FDCO_MAX / FOUT1);
	divider_min =  ceil(FDCO_MIN / FOUT1);
	divider_curr = divider_min;
	valid_flag=0;
	while ( divider_curr <= divider_max)
	{
		for(i=0;i<6;i++)
		{
			// get the next possible n1 value
			hs_div = HS_DIV[i];
			n1 = (float)(divider_curr) / (float)(hs_div);
			// Determine if curr_n1 is an integer and an even number or one
			// then it will be a valid divider option for the new frequency
			n1_tmp = floor(n1);
			n1_tmp = n1 - n1_tmp;
			if(n1_tmp == 0.0)  // Then curr_n1 is an integer
			{
				if((n1 != 0) && ( (n1 == 1) || ((n1 & 1) == 0) )) // Then the calculated N1 is
				{                                  // either 1 or an even number
					valid_flag = 1;
            	}
			}
			if(valid_flag == 1) break;       // Divider was found, exit loop
		}
		if(valid_flag == 1) break;          // Divider was found, exit loop
		divider_curr = divider_curr + 1;            // If a valid divider is not found,
                                          			// increment curr_div and loop
	}
	*/
	// New RFREQ calculation
	NEW_RFREQ = (FOUT1 * n1 * hs_div) / FXTAL;
	NEW_RFREQ_LONG = NEW_RFREQ * (1<<28);
	//NEW_RFREQ_MSB = NEW_RFREQ;//取NEW_RFREQ整数部分
	//NEW_RFREQ_LSB = (NEW_RFREQ -  NEW_RFREQ_MSB) * (1<<28); //取NEW_RFREQ小数部分再乘以2^28
	//NEW_RFREQ_MSB = NEW_RFREQ_MSB *  (1<<28);
	//NEW_RFREQ_LONG = NEW_RFREQ_MSB + NEW_RFREQ_LSB;
	// Subtract 4 because of the offset of HS_DIV.
	hs_div = hs_div - 4;

	// convert new N1 to the binary representation
	if(n1 == 1)
	{
		n1 = 0;  //Corner case for N1. If N1=1, it is represented as "00000000"
	}
	else if((n1 & 1) == 0)
	{
		n1 = n1 - 1; // If n1 is even, round down to closest odd number. See the
	}
	local_evb16.sStr.Si570R7 = (hs_div << 5) + (n1 >> 2);
	local_evb16.sStr.Si570R8 = ((NEW_RFREQ_LONG >> 32) & 0x3f) + (n1 << 6);
	local_evb16.sStr.Si570R9 = (NEW_RFREQ_LONG >> 24);
	local_evb16.sStr.Si570R10 = (NEW_RFREQ_LONG >> 16);
	local_evb16.sStr.Si570R11 = (NEW_RFREQ_LONG >> 8);
	local_evb16.sStr.Si570R12 = NEW_RFREQ_LONG & 0xFF;
	
	*EVB16 = local_evb16;  
	return 0;
}

int evb16_SetRate(int instrumentHandle, double FOUT1)
{
	int reg_add, error;
	float T_wait=0.1;
	union uEVB16 local_evb16; 
	
	//local_evb16.sStr.Si570Regs.R7to12[0] = 0x00;
	//local_evb16.sStr.Si570Regs.R7to12[1] = 0x42;
	//local_evb16.sStr.Si570Regs.R7to12[2] = 0xB8;
	//local_evb16.sStr.Si570Regs.R7to12[3] = 0xBB;
	//local_evb16.sStr.Si570Regs.R7to12[4] = 0xE4;
	//local_evb16.sStr.Si570Regs.R7to12[5] = 0x72;
	//根据设置的新频率计算对应的寄存器值
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16, 0, 256, local_evb16.pStr); 
	if (error) return -1; 
	
	error = CalculateRegForNewFrequency(&local_evb16, FOUT1);
	if (error) return -1;

	//将新的寄存器值写入7020
	reg_add = (int)(&local_evb16.sStr.Si570R7)-(int)(&local_evb16.sStr.FlashCtl);
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, 6, local_evb16.pStr, T_wait);
	if (error) return -1;

	//写S,固件就设置Si570寄存器
	local_evb16.sStr.Si570cmd = 'S';
	reg_add = (int)(&local_evb16.sStr.Si570cmd)-(int)(&local_evb16.sStr.FlashCtl);
	error = I2C_BYTE_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, local_evb16.sStr.Si570cmd, T_wait);
	if (error) return -1;

	//延时
	//Delay(0.1);

	return 0;
}

int evb16_GetSi570Flash(int instrumentHandle)
{
	int reg_add, error;
	float T_wait=0.1;
	
	union uEVB16 local_evb16; 
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16, 0, 256, local_evb16.pStr);
	if (error) {MessagePopup("Error", "evb16 get regedit error"); return -1;}

	//写F,固件就会读FLASH存储的Si570出厂寄存器值
	local_evb16.sStr.Si570cmd = 'F';
	reg_add = (int)(&local_evb16.sStr.Si570cmd)-(int)(&local_evb16.sStr.FlashCtl);
	error = I2C_BYTE_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, local_evb16.sStr.Si570cmd, T_wait);
	if (error) return -1;
	Delay(1);

	//读Si570出厂值
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16, 0, 128, local_evb16.pStr);
	if (error) return -1;
	Delay(1);
	return 0;
}

int evb16_GetSi570Reg(int instrumentHandle, union uEVB16 *EVB16)
{
	int reg_add, error;
	float T_wait=0.1;
	
	union uEVB16 local_evb16; 
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16, 0, 256, local_evb16.pStr);
	if (error) {MessagePopup("Error", "evb16 get regedit error"); return -1;}

	//写R,固件就读取Si570寄存器值
	local_evb16.sStr.Si570cmd = 'R';
	reg_add = (int)(&local_evb16.sStr.Si570cmd)-(int)(&local_evb16.sStr.FlashCtl);
	error = I2C_BYTE_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, local_evb16.sStr.Si570cmd, T_wait);
	if (error) return -1;
	Delay(1);

	//读Si570寄存器值
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16, 0, 128, local_evb16.pStr);
	if (error) return -1;
	
	*EVB16 = local_evb16;
	return 0;
}

int evb16_SaveSi570NVM(int instrumentHandle)
{
	int reg_add, error;
	float T_wait=0.1;
	union uEVB16 local_evb16; 
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16, 0, 256, local_evb16.pStr);
	if (error) {MessagePopup("Error", "evb16 get regedit error"); return -1;}
	
	//写B,固件就保存Si570出厂值到FLASH
	local_evb16.sStr.Si570cmd = 'B';
	reg_add = (int)(&local_evb16.sStr.Si570cmd)-(int)(&local_evb16.sStr.FlashCtl);
	error = I2C_BYTE_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, local_evb16.sStr.Si570cmd, T_wait);
	if (error) return -1;
	Delay(1);

	//读Si570出厂值
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16, 0, 128, local_evb16.pStr);
	if (error) return -1;
	return 0;
}

int evb16_SetSi570Reg(int instrumentHandle)
{
	int reg_add, error;
	float T_wait=0.1;
	union uEVB16 local_evb16; 
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16, 0, 256, local_evb16.pStr);
	if (error) {MessagePopup("Error", "evb16 get regedit error"); return -1;}

	//将新的寄存器值写入7020
	reg_add = (int)(&local_evb16.sStr.Si570R7)-(int)(&local_evb16.sStr.FlashCtl);
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, 6, local_evb16.pStr, T_wait);
	if (error) return -1;

	//写S,固件就设置Si570寄存器
	local_evb16.sStr.Si570cmd = 'S';
	reg_add = (int)(&local_evb16.sStr.Si570cmd)-(int)(&local_evb16.sStr.FlashCtl);
	error = I2C_BYTE_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, local_evb16.sStr.Si570cmd, T_wait);
	if (error) return -1;

	//延时
	//Delay(1);

	return 0;

}

int evb16_TestSi570Write(int instrumentHandle,unsigned char Si570byteaddr,unsigned char Si570data)
{
	int reg_add, error;
	float T_wait=0.1;
	union uEVB16 local_evb16; 
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16, 0, 256, local_evb16.pStr);
	if (error) {MessagePopup("Error", "evb16 get regedit error"); return -1;}

	local_evb16.sStr.I2Ccommand = 'W';
	local_evb16.sStr.I2Cslave_address = 0xCA;
	local_evb16.sStr.I2Creg_start_add = Si570byteaddr;
	local_evb16.sStr.I2Creg_length = 1;
	local_evb16.sStr.I2Creg_dat_arr[0] =Si570data;
	//将写相应寄存器值写入7020
	reg_add = (int)(&local_evb16.sStr.I2Ccommand)-(int)(&local_evb16.sStr.FlashCtl);
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, 5, local_evb16.pStr, T_wait);
	if (error) return -1;
	//写T,固件就测试写Si570 I2C值
	local_evb16.sStr.Si570cmd = 'T';
	reg_add = (int)(&local_evb16.sStr.Si570cmd)-(int)(&local_evb16.sStr.FlashCtl);
	error = I2C_BYTE_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, local_evb16.sStr.Si570cmd, T_wait);
	if (error) return -1;
	Delay(0.1);

	return 0;
}

int evb16_TestSi570Read(int instrumentHandle,unsigned char Si570byteaddr,unsigned char* Si570data)
{
	int reg_add, error;
	float T_wait=0.1;
	union uEVB16 local_evb16; 
	
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16, 0, 256, local_evb16.pStr);
	if (error) {MessagePopup("Error", "evb16 get regedit error"); return -1;}

	local_evb16.sStr.I2Ccommand = 'R';
	local_evb16.sStr.I2Cslave_address = 0xCA;
	local_evb16.sStr.I2Creg_start_add = Si570byteaddr;
	local_evb16.sStr.I2Creg_length = 1;
	//将写相应寄存器值写入7020
	reg_add = (int)(&local_evb16.sStr.I2Ccommand)-(int)(&local_evb16.sStr.FlashCtl);
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, 5, local_evb16.pStr, T_wait);
	if (error) return -1;
	//写T,固件就测试读Si570 I2C值
	local_evb16.sStr.Si570cmd = 'T';
	reg_add = (int)(&local_evb16.sStr.Si570cmd)-(int)(&local_evb16.sStr.FlashCtl);
	error = I2C_BYTE_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, local_evb16.sStr.Si570cmd, T_wait);
	if (error) return -1;
	Delay(0.1);
	//读Si570寄存器值
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16, 0, 128, local_evb16.pStr);
	if (error) return -1;
	*Si570data = local_evb16.sStr.I2Creg_dat_arr[0];
	return 0;
}

/*
int evb16_TestSi570(int instrumentHandle,unsigned char Si570addr)
{
	int reg_add, error;
	float T_wait=0.1;

	local_evb16.sStr.I2Ccommand = 'W';
	local_evb16.sStr.I2Cslave_address = Si570addr;
	local_evb16.sStr.I2Creg_start_add = 135;
	local_evb16.sStr.I2Creg_length = 1;
	local_evb16.sStr.I2Creg_dat_arr[0] =0x01;
	//将写相应寄存器值写入7020
	reg_add = (int)(&local_evb16.sStr.I2Ccommand)-(int)(&local_evb16.sStr.FlashCtl);
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, 5, local_evb16.pStr, T_wait);
	if (error) return -1;
	//写T,固件就测试读取Si570 I2C值
	local_evb16.sStr.Si570cmd = 'T';
	reg_add = (int)(&local_evb16.sStr.Si570cmd)-(int)(&local_evb16.sStr.FlashCtl);
	error = I2C_BYTE_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, local_evb16.sStr.Si570cmd, T_wait);
	if (error) return -1;
	Delay(0.1);

	//下一步读寄存器
	local_evb16.sStr.I2Ccommand = 'R';
	local_evb16.sStr.I2Cslave_address = Si570addr;
	local_evb16.sStr.I2Creg_start_add = 7;
	local_evb16.sStr.I2Creg_length = 6;
	//将写相应寄存器值写入7020
	reg_add = (int)(&local_evb16.sStr.I2Ccommand)-(int)(&local_evb16.sStr.FlashCtl);
	error = I2C_BYTEs_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, 4, local_evb16.pStr, T_wait);
	if (error) return -1;
	//写T,固件就测试读取Si570 I2C值
	local_evb16.sStr.Si570cmd = 'T';
	reg_add = (int)(&local_evb16.sStr.Si570cmd)-(int)(&local_evb16.sStr.FlashCtl);
	error = I2C_BYTE_WRITE_DLL (instrumentHandle, AddrEVB16, reg_add, local_evb16.sStr.Si570cmd, T_wait);
	if (error) return -1;
	Delay(0.1);

	//读Si570寄存器值
	error = I2C_BYTEs_READ_DLL (instrumentHandle, AddrEVB16, 0, 128, local_evb16.pStr);
	if (error) return -1;

	return 0;
} */
//=================================================================================================
int evb16_ModeSet(int instrumentHandle,int Mode,int settime)
{
	union uEVB16_FPGA local_evb16_fpga;
	
	long long set_value=0;
	int error;
	int reg_add;
	
	error = EPP_BYTEs_READ_F320_DLL (instrumentHandle, 0, 128, local_evb16_fpga.pStr);
	if (error) {MessagePopup("Error", "evb16 get FPGA error"); return -1;}
		
	if(Mode==REPEAT_MODE)
	{
		reg_add = (int)(&local_evb16_fpga.sStr.Single_or_Repeat)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
		if(WEPP(instrumentHandle, reg_add, 0))				 	                                                  // 重复模式
		{
		//	MessagePopup("Error", "evb16 set FPGA error"); 
			return -1;
		}
		local_evb16_fpga.sStr.Single_Time_Value.Single_Time_Set7_0  = 0xFF;
		local_evb16_fpga.sStr.Single_Time_Value.Single_Time_Set15_8 = 0xFF;
		local_evb16_fpga.sStr.Single_Time_Value.Single_Time_Set23_16= 0xFF;
		local_evb16_fpga.sStr.Single_Time_Value.Single_Time_Set31_24= 0xFF;
		local_evb16_fpga.sStr.Single_Time_Value.Single_Time_Set39_32= 0xFF;
		
		reg_add = (int)(&local_evb16_fpga.sStr.Single_Time_Value)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
		if(EPP_BYTEs_WRITE_F320_DLL (instrumentHandle, reg_add, 5, local_evb16_fpga.pStr))
		{
		//	MessagePopup("Error", "evb16 set FPGA error"); 
			return -1;
		}
	}
	else if(Mode==SINGLE_MODE)
	{
		reg_add = (int)(&local_evb16_fpga.sStr.Single_or_Repeat)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
		if(WEPP(instrumentHandle, reg_add, 1))				 	                                                  // 单次模式
		{
		//	MessagePopup("Error", "evb16 set FPGA error"); 
			return -1;
		}
		set_value=(long long)((10.3125E+9)*settime/64.0)-3;
		
		local_evb16_fpga.sStr.Single_Time_Value.Single_Time_Set7_0  = (unsigned char)set_value;
		local_evb16_fpga.sStr.Single_Time_Value.Single_Time_Set15_8 = (unsigned char)(set_value>>8);
		local_evb16_fpga.sStr.Single_Time_Value.Single_Time_Set23_16= (unsigned char)(set_value>>16);
		local_evb16_fpga.sStr.Single_Time_Value.Single_Time_Set31_24= (unsigned char)(set_value>>24);
		local_evb16_fpga.sStr.Single_Time_Value.Single_Time_Set39_32= (unsigned char)(set_value>>32);
		reg_add = (int)(&local_evb16_fpga.sStr.Single_Time_Value)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
		if(EPP_BYTEs_WRITE_F320_DLL (instrumentHandle, reg_add, 5, local_evb16_fpga.pStr))
		{
		//	MessagePopup("Error", "evb16 set FPGA error"); 
			return -1;
		}
	}
	else
	{
		MessagePopup("Error", "evb16 set MODE error"); 
		return -1;
	}
	
	return 0;
}

int evb16_CheckSingleStatus(int instrumentHandle,int *Status)
{
	union uEVB16_FPGA local_evb16_fpga;
	int error;
	int reg_add;
	
	reg_add = (int)(&local_evb16_fpga.sStr.Single_Over_Flag)-(int)(&local_evb16_fpga.sStr.FPGA_Ver);
	if(EPP_BYTEs_READ_F320_DLL  (instrumentHandle, reg_add, 1, local_evb16_fpga.pStr))
    {
		MessagePopup("Error", "evb16 get FPGA error"); 
		return -1;
	}	
	if(local_evb16_fpga.sStr.Single_Over_Flag.Single_Repeat_Over_Flag)
		*Status=1;
	else
		*Status=0;  	
	
	return 0; 	
}   
