#include "EVB11.h"   
#include <ansi_c.h>
#include <utility.h>
#include <userint.h>

EVB11_Addr=0x84; 
			
int EVB11_Init (int *EVB11Handle, char *SN)      
{
	int error, i;
	char str[256], str1[256], myUSBSN[30];
	int reg_add, reg_dat;
	int I2Chost_found = 0;
	int I2Cslave_found = 0;
	int iIndex;
	int status;
	int lpdwNumDevices;
	/*ioArray[0]：速率，计算公式SCK = 12000/( ioArray[0]+1)K，如ioArray[0] = 119，则SPI速率为100K
	ioArray[1]：相位，0：在SCK周期的第一个边沿采样数据，1：在SCK周期的第二个边沿采样数据
	ioArray[1]：极性，0：SCK在空闲状态时处于低电平，1：SCK在空闲状态时处于高电平
	默认设置为100K，第一个边沿采样数据，SCK在空闲状态时处于低电平
	*/

	EVB11_Addr=0x84;   

	//Search I2C host controller and I2C slave EVB at EVB_addr 
	I2Chost_found = 0;
	I2Cslave_found = 0;

	status = GetF320Numbers(&lpdwNumDevices);
	if( (status == 0) && (lpdwNumDevices>0) )
	{
		for (iIndex=0x800; iIndex<lpdwNumDevices+0x800; iIndex++)
		{	
			//更新界面设置Si534频率
		//	SetCtrlVal (panel, PANEL1_RNG_SET_FS, 0);
		//	on_SET_FS (panel, 0, EVENT_COMMIT, 0, 0, 0);
			error = SET_FS_F320_DLL (*EVB11Handle-0x800, 0);  

			error = I2C_HOST_INITIALIZATION_DLL (iIndex); 
			if (error!=-1) //USB host found
			{   
				*EVB11Handle = iIndex; 
			
				error = I2C_SLAVE_SEARCH_DLL(iIndex, EVB11_Addr);
				if (error==0) //I2C EVB found
				{	
				//read F320 version
					error = GET_Version_F320_DLL(*EVB11Handle-0x800, &USBVersion);
			
				//read F320 SN	
					memset (myUSBSN, 0, sizeof(myUSBSN));
					error = GetF320SerialNumber(*EVB11Handle-0x800, myUSBSN);
					
				//read eeprom information from EVB_addr
					error = I2C_BYTEs_READ_DLL (*EVB11Handle, EVB11_Addr, 0, 256, EVB11.pStrF0);
					
				//compare SN
					error = strcmp (EVB11.sStrF0.vendorSN, myUSBSN);
					if (error && (*EVB11Handle-0x800)>=(lpdwNumDevices-1)) {MessagePopup("提示","F320的序列号和EVB11的序列号不一致");return -1;}
			
					error = strcmp (EVB11.sStrF0.vendorSN, SN);
					if (error && (*EVB11Handle-0x800)>=(lpdwNumDevices-1)) {MessagePopup("提示","选用的EVB11序列号和EVB11实际序列号不一致");return -1;}
			
					if (error==0) {I2Cslave_found=1;break;}
				}				  	
		  	}
		}
	}
	if (I2Cslave_found==0) 
	{
		return -1;
	}
	
	error = (EVB11.sStrF0.Core_Voltage/10000.0>3.6 || EVB11.sStrF0.Core_Voltage/10000.0<3.0); 
	if (error<0) {MessagePopup("提示","EVB11电压检测失败！");return -1;}
	
	//Set Password
	//error = I2C_4BYTEs_WRITE_DLL (USBHandle, EVB_addr, 123, 'J', 'a', 'c', 'k', 0.1);//写0级密码
	EVB11.sStrF0.Password = 0x6B63614A; //"Jack" 
	reg_add =(int)(&EVB11.sStrF0.Password) - (int)(&EVB11.sStrF0.FirstByte); 
	error = I2C_BYTEs_WRITE_DLL (*EVB11Handle, EVB11_Addr, reg_add, 4, EVB11.pStrF0, 0.1);			

	//table0
	EVB11.sStrF0.LUTIndex=0;
	reg_add = (int)(&EVB11.sStrF0.LUTIndex)-(int)(&EVB11.sStrF0.FirstByte); 
	error = I2C_BYTEs_WRITE_DLL (*EVB11Handle, EVB11_Addr, reg_add, 1, EVB11.pStrF0, 0.1);			
   
	return 0;
}

int EVB11_GET_SN (int panel, int control)
{
	int  lpdwNumDevices, contrlIndex, iIndex, error;
	char myUSBSN[30], buf[30];  
	
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
			error = I2C_SLAVE_SEARCH_DLL(iIndex, EVB11_Addr);  
			if (error==0) //I2C EVB found 
			{
				//read F320 SN
				memset (myUSBSN, 0, sizeof(myUSBSN));
				error = GetF320SerialNumber(iIndex-0x800, myUSBSN);

			//	MyDLLCheckSN (myUSBSN); 
				
				if (strstr (myUSBSN, "SEVB11")==NULL) continue;
				
				//read eeprom information from EVB_addr
				error = I2C_BYTEs_READ_DLL (iIndex, EVB11_Addr, 0, 256, EVB11.pStrF0);
				if (error<0) {MessagePopup("提示","读取设备序列号失败，error code -2");return -1;} 
				EVB11.sStrF0.vendorSN[16]=0;
				
				InsertListItem (panel, control, contrlIndex, EVB11.sStrF0.vendorSN, contrlIndex);
				contrlIndex++;
			}
		}
	}
	if (contrlIndex<1) {MessagePopup("提示","没有找到对应的EVB11，请确认选择的SEVB类型是否正确");return -1;}  

	return 0;
}

int EVB11_SET_OLT_TxDIS (int EVB11Handle, BYTE TxDIS)
{
	int error, reg_add;  
	
	error = I2C_BYTEs_READ_DLL (EVB11Handle, EVB11_Addr, 0, 256, EVB11.pStrF0);
	if (error<0) return -1;	

	EVB11.sStrF0.sOutput.OLT_TxDIS = TxDIS; 

	reg_add = (int)(&EVB11.sStrF0.sOutput)-(int)(&EVB11.sStrF0.FirstByte);
	error = I2C_BYTEs_WRITE_DLL (EVB11Handle, EVB11_Addr, reg_add, 1, EVB11.pStrF0, 0.1);
	if (error) return -1;
		
	return 0;
}

int EVB11_SET_OLT1G_TxDIS (int EVB11Handle, BYTE TxDIS)
{
	int error, reg_add;  
	
	error = I2C_BYTEs_READ_DLL (EVB11Handle, EVB11_Addr, 0, 256, EVB11.pStrF0);
	if (error<0) return -1;	

	EVB11.sStrF0.sOutput.OLT1G_TxDIS = TxDIS; 

	reg_add = (int)(&EVB11.sStrF0.sOutput)-(int)(&EVB11.sStrF0.FirstByte);
	error = I2C_BYTEs_WRITE_DLL (EVB11Handle, EVB11_Addr, reg_add, 1, EVB11.pStrF0, 0.1);
	if (error) return -1;
		
	return 0;
}

int EVB11_SET_TRIG_LEVEL (int EVB11Handle, int tigger_level)
{
	int error, reg_add;

	error = I2C_BYTEs_READ_DLL (EVB11Handle, EVB11_Addr, 0, 256, EVB11.pStrF0);
	if (error<0) return -1;	
	
	EVB11.sStrF0.sOutput.OLT_RxTRIG = tigger_level;
	
	reg_add = (int)(&EVB11.sStrF0.sOutput)-(int)(&EVB11.sStrF0.FirstByte);
	error = I2C_BYTEs_WRITE_DLL (EVB11Handle, EVB11_Addr, reg_add, 1, EVB11.pStrF0, 0.1); 
	if (error<0) {MessagePopup ("error", "EVB5 set Trig level fail"); return -1;} 
	
	return 0;
}

int EVB11_READ_OLT1G_RxLOS (int EVB11Handle, BYTE *RxLOS)
{
	int error;
	
	error = I2C_BYTEs_READ_DLL (EVB11Handle, EVB11_Addr, 0, 256, EVB11.pStrF0);
	if (error<0) return -1;				
	
	*RxLOS = EVB11.sStrF0.sDin2.OLT1G_RxLOS;	
			
	return 0;	
}

int EVB11_READ_OLT_RxLOS (int EVB11Handle, BYTE *RxLOS)
{
	int error;
	
	error = I2C_BYTEs_READ_DLL (EVB11Handle, EVB11_Addr, 0, 256, EVB11.pStrF0);
	if (error<0) return -1;				
	
	*RxLOS = EVB11.sStrF0.sDin2.OLT_RxLOS;	
			
	return 0;	
}

int EVB11_SET_OLT_VTR(int EVB11Handle, unsigned char SHUTValue)
{
	int PWR_ONU, PWR_OLT, error; 
	
	PWR_ONU = 1;
	PWR_OLT = SHUTValue;

	error = SDN_VCC_F320_DLL (EVB11Handle-0x800, (PWR_ONU<<1)+PWR_OLT);
	return 0;
}
 /*
int CVICALLBACK On_Set_EVBSN (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int error;
 int i, reg_add, reg_length; 
 unsigned char temp_arr[256];

 char EVBSN[30];
 char EVBPN[30];

	switch (event)
		{
		case EVENT_COMMIT:
	
			SetWaitCursor (1);
			GetCtrlVal (panel, PANEL1_NUM_HOSTHandle, &USBHandle);
			GetCtrlVal (panel, PANEL1_STR_EVBSN, EVBSN);
			GetCtrlVal (panel, PANEL1_STR_EVBPN, EVBPN);

			//set password
			reg_add=0x7B;
			EVB.sStrF0.Password = 0x6B63614A; //"Jack" 
			reg_add =(int)(&EVB.sStrF0.Password) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 4, EVB.pStrF0, 0.1);			
	
			//table0
 			EVB.sStrF0.LUTIndex=0;
 			reg_add = (int)(&EVB.sStrF0.LUTIndex)-(int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.1);			
		
			for(i=0;i<16; i++)
			{	EVB.sStrF0.vendorSN[i] = EVBSN[i];
				EVB.sStrF0.vendorPN[i] = EVBPN[i];
			}
	
 			reg_add = (int)(&EVB.sStrF0.vendorPN)-(int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 16+16, EVB.pStrF0, 0.1); 
												
			//'W'
			EVB.sStrF0.FirstByte = 'W';
			reg_add =(int)(&EVB.sStrF0.FirstByte) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.2);
			SetCtrlVal (panel, PANEL1_BIN_SET_MODE, 0);
			
			SetWaitCursor (0);
			
			break;
		}
	return 0;
}

int CVICALLBACK on_Get_USBSN (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int error;
 char USBSN[30];

	switch (event)
		{
		case EVENT_COMMIT:
			SetCtrlVal (panel, PANEL1_STR_USBSN, "");

			GetCtrlVal (panel, PANEL1_NUM_HOSTHandle, &USBHandle);
			memset (USBSN, 0, sizeof(USBSN));
			error = GetF320SerialNumber(USBHandle-0x800, USBSN);
			if (error<0) return -1; //USB host not found!
			SetCtrlVal (panel, PANEL1_STR_USBSN, USBSN);

			break;
		}
	return 0;
}

int CVICALLBACK On_Set_USBSN (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int error;
 char USBSN[30];

	switch (event)
		{
		case EVENT_COMMIT:
	
			SetWaitCursor (1);

 			//search USB Host
			GetCtrlVal (panel, PANEL1_NUM_HOSTHandle, &USBHandle);
			error = I2C_HOST_INITIALIZATION_DLL (USBHandle); 
			if (error<0) return -1; //USB host not found!
			
			memset (USBSN, 0, sizeof(USBSN));
			GetCtrlVal (panel, PANEL1_STR_USBSN, USBSN);
			error = SetF320SerialNumber(USBHandle-0x800, USBSN);
			if (error<0) return -1; //USB host not found!
			
			SetWaitCursor (0);
			
			break;
		}
	return 0;
}


int CVICALLBACK on_TrigShape (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int reg_add, error;


	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel,  PANEL1_RNG_TrigShape, &EVB.sStrF0.RSSI_mode);
			reg_add = (int)(&EVB.sStrF0.RSSI_mode)-(int)(&EVB.sStrF0.FirstByte);
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.1); 
			
			SetCtrlVal (panel, PANEL1_BIN_TrigLevel, EVB.sStrF0.sOutput.OLT_RxTRIG);
			
			switch (EVB.sStrF0.RSSI_mode) 
			{
			case 0: //level
				SetCtrlAttribute (panel, PANEL1_BIN_TrigLevel, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_RSSI_PW, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_RSSI_PWt, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BIN_TrigPolarity, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BUT_RSSI_Trig_Gen, ATTR_DIMMED, 1);

			break;
			
			case 1: //pulse
				SetCtrlAttribute (panel, PANEL1_BIN_TrigLevel, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_RSSI_PW, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_RSSI_PWt, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_BIN_TrigPolarity, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_BUT_RSSI_Trig_Gen, ATTR_DIMMED, 0);

			break;
			}
			
			
			break;
		}
	return 0;
}

int CVICALLBACK on_Clock (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int reg_add, error;
 float clock;

	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel,  PANEL1_RNG_Clock, &clock);
			EVB.sStrF0.myPWMDAT0 = 41.78/2./clock;
			reg_add = (int)(&EVB.sStrF0.myPWMDAT0)-(int)(&EVB.sStrF0.FirstByte);
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 2, EVB.pStrF0, 0.1);
			clock = 41.78/2./EVB.sStrF0.myPWMDAT0;
			SetCtrlVal (panel,  PANEL1_RNG_Clock, clock);
			
			break;
		}
	return 0;
}


int CVICALLBACK on_TrigLevel (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int reg_add, error, TRxTrigger;


	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel,  PANEL1_BIN_TrigLevel, &TRxTrigger);
			EVB.sStrF0.sOutput.OLT_RxTRIG= TRxTrigger;

			reg_add = (int)(&EVB.sStrF0.sOutput)-(int)(&EVB.sStrF0.FirstByte);
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.1); 

			break;
		}
	return 0;
}

int CVICALLBACK on_TrigPolarity (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int reg_add, error, RSSI_polarity;

	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel,  PANEL1_BIN_TrigPolarity, &RSSI_polarity);
			EVB.sStrF0.RSSI_polarity= RSSI_polarity;

			reg_add = (int)(&EVB.sStrF0.RSSI_polarity)-(int)(&EVB.sStrF0.FirstByte);
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.1); 

			break;
		}
	return 0;
}

int CVICALLBACK on_RSSI_PW (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{float t;
 int reg_add, error;


	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel,  PANEL1_NUM_RSSI_PW, &EVB.sStrF0.RSSI_PW);
			//t = 1.6 + EVB.sStrF0.RSSI_PW * 0.26; //us
			SetCtrlVal (panel,  PANEL1_NUM_RSSI_PWt, 0.264742857 * EVB.sStrF0.RSSI_PW + 1.478095238); 
			
			reg_add = (int)(&EVB.sStrF0.RSSI_PW)-(int)(&EVB.sStrF0.FirstByte);
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 2, EVB.pStrF0, 0.1); 

			break;
		}
	return 0;
}

int CVICALLBACK on_RSSI_Trig_Gen (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int reg_add, error; 
 unsigned char reg_dat, arr[256];

 
	switch (event)
		{
		case EVENT_COMMIT:
			reg_add = (int)(&EVB.sStrF0.sOutput)-(int)(&EVB.sStrF0.FirstByte);
			error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0); 
			EVB.sStrF0.sOutput.OLT_RxTRIG = 1;
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.1); 

			//回读sOutput，此时EVB.sStrF0.sOutput.OLT_RxTRIG应该被7020自动清理了。
			error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0); 

			break;
		}
	return 0;
}

int CVICALLBACK on_BENShape (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int reg_add, error;


	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel,  PANEL1_RNG_BENShape, &EVB.sStrF0.BEN_mode);
			reg_add = (int)(&EVB.sStrF0.BEN_mode)-(int)(&EVB.sStrF0.FirstByte);
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.1); 
			
			SetCtrlVal (panel, PANEL1_BIN_BENLevel, EVB.sStrF0.sOutput.ONU1_TxBEN);
			
			switch (EVB.sStrF0.BEN_mode) 
			{
			case 0: //level
				SetCtrlAttribute (panel, PANEL1_BIN_BENLevel, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PW, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PWt, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BIN_BENPolarity, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BUT_BEN_Trig_Gen, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay1, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay2, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay3, ATTR_DIMMED, 1);
			break;
			
			case 1: //pulse
				SetCtrlAttribute (panel, PANEL1_BIN_BENLevel, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PW, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PWt, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_BIN_BENPolarity, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_BUT_BEN_Trig_Gen, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay1, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay2, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay3, ATTR_DIMMED, 1);
			break;
			
			case 2: //clock
				SetCtrlAttribute (panel, PANEL1_BIN_BENLevel, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PW, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PWt, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BIN_BENPolarity, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BUT_BEN_Trig_Gen, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay1, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay2, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay3, ATTR_DIMMED, 1);
			break;

			case 3: //BEN_TRG, N1N2N3
				SetCtrlAttribute (panel, PANEL1_BIN_BENLevel, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PW, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PWt, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BIN_BENPolarity, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BUT_BEN_Trig_Gen, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay1, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay2, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay3, ATTR_DIMMED, 0);
			break;

			case 4: //BEN_TRG, N1N3
				SetCtrlAttribute (panel, PANEL1_BIN_BENLevel, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PW, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PWt, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BIN_BENPolarity, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BUT_BEN_Trig_Gen, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay1, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay2, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay3, ATTR_DIMMED, 0);
			break;

			case 5: //BEN_TRG, N2
				SetCtrlAttribute (panel, PANEL1_BIN_BENLevel, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PW, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PWt, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BIN_BENPolarity, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BUT_BEN_Trig_Gen, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay1, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay2, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay3, ATTR_DIMMED, 1);
			break;

			case 6: //BEN_TRG, L2L3
				SetCtrlAttribute (panel, PANEL1_BIN_BENLevel, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PW, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PWt, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BIN_BENPolarity, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BUT_BEN_Trig_Gen, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay1, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay2, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay3, ATTR_DIMMED, 0);
			break;
		}

				
			break;
		}
	return 0;
}

int CVICALLBACK on_BENLevel (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int reg_add, error, BEN;


	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel, PANEL1_BIN_BENLevel, &BEN);
			EVB.sStrF0.sOutput.ONU1_TxBEN= BEN;

			reg_add = (int)(&EVB.sStrF0.sOutput)-(int)(&EVB.sStrF0.FirstByte);
			//error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.1); 

			break;
		}
	return 0;
}

int CVICALLBACK on_BENPolarity (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int reg_add, error, polarity;

	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel,  PANEL1_BIN_BENPolarity, &polarity);
			EVB.sStrF0.BEN_polarity= polarity;

			reg_add = (int)(&EVB.sStrF0.BEN_polarity)-(int)(&EVB.sStrF0.FirstByte);
			//error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.1); 

			break;
		}
	return 0;
}

int CVICALLBACK on_BEN_PW (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{float t;
 int reg_add, error;


	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel,  PANEL1_NUM_BEN_PW, &EVB.sStrF0.BEN_PW);
			//t = 1.6 + EVB.sStrF0.RSSI_PW * 0.26; //us
			SetCtrlVal (panel,  PANEL1_NUM_BEN_PWt, 0.264742857 * EVB.sStrF0.BEN_PW + 1.478095238); 
			
			reg_add = (int)(&EVB.sStrF0.BEN_PW)-(int)(&EVB.sStrF0.FirstByte);
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 2, EVB.pStrF0, 0.1); 

			break;
		}
	return 0;
}

int CVICALLBACK on_BEN_Trig_Gen (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int reg_add, error; 
 unsigned char reg_dat, arr[256];

 
	switch (event)
		{
		case EVENT_COMMIT:
			reg_add = (int)(&EVB.sStrF0.sOutput)-(int)(&EVB.sStrF0.FirstByte);
			error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0); 
			EVB.sStrF0.sOutput.ONU1_TxBEN = 1;
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.1); 
			
			//回读sOutput，此时EVB.sStrF0.sOutput.ONU1_TxBEN应该被7020自动清理了。
			error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0); 

			break;
		}
	return 0;
}


int CVICALLBACK on_BENTRG_Delay (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{float t;
 int reg_add, error;

	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel,  PANEL1_NUM_BENTRG_Delay1, &EVB.sStrF0.delay1);
			GetCtrlVal (panel,  PANEL1_NUM_BENTRG_Delay2, &EVB.sStrF0.delay2);
			GetCtrlVal (panel,  PANEL1_NUM_BENTRG_Delay3, &EVB.sStrF0.delay3);
			
			//t = 1.6 + EVB.sStrF0.RSSI_PW * 0.26; //us
			//SetCtrlVal (panel,  PANEL1_NUM_BEN_PWt, 0.264742857 * EVB.sStrF0.BEN_PW + 1.478095238); 
			
			reg_add = (int)(&EVB.sStrF0.delay1)-(int)(&EVB.sStrF0.FirstByte);
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 3, EVB.pStrF0, 0.1); 

			break;
		}
	return 0;
}























int CVICALLBACK on_ReadEVB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int error, i;
 char str[256];
 
	switch (event)
		{
		case EVENT_COMMIT:
			DefaultCtrl (panel, PANEL1_LED_READ);

			error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, 0, 256, EVB.pStrF0); 
			if (error<0) return -1; //EVB access error! 
			SetCtrlVal (panel, PANEL1_LED_READ, 1);
			
			//firmware version 
			sprintf(str, "%c.%c", EVB.sStrF0.FirmwareVersion+0x30, EVB.sStrF0.FirmwareVersionSuffix+0x30);
			SetCtrlVal (panel, PANEL1_STR_FWVersion_EVB, str);
			//Core_Voltage
			SetCtrlVal (panel, PANEL1_NUM_Core_Voltage_EVB, EVB.sStrF0.Core_Voltage/10000.);
			//Core_Temperature
			SetCtrlVal (panel, PANEL1_NUM_CoreTP_EVB, EVB.sStrF0.Core_Temperature/256.);
			//vendorSN
			strncpy(str, EVB.sStrF0.vendorSN, 16);
			str[16]=0;
			SetCtrlVal (panel, PANEL1_STR_EVBSN, str);
			//vendorPN
			strncpy(str, EVB.sStrF0.vendorPN, 16);
			str[16]=0;
			SetCtrlVal (panel, PANEL1_STR_EVBPN, str); 
			SetCtrlVal (panel, PANEL1_LED_EVB_INIT, 1);
			SetCtrlVal (panel, PANEL1_NUM_HOSTHandle, USBHandle);
			
			//sOutput
			SetCtrlVal (panel, PANEL1_BIN_ONU1_PDRST, EVB.sStrF0.sInput.ONU1_PDRST);
			SetCtrlVal (panel, PANEL1_BIN_OLT_TxDIS, EVB.sStrF0.sOutput.OLT_TxDIS); 
			SetCtrlVal (panel, PANEL1_BIN_OLT_RxRST, EVB.sStrF0.sOutput.OLT_RxRST); 
			SetCtrlVal (panel, PANEL1_BIN_OLT1G_TxDIS, EVB.sStrF0.sOutput.OLT1G_TxDIS);
			SetCtrlVal (panel, PANEL1_BIN_OLT1G_RxRST, EVB.sStrF0.sOutput.OLT1G_RxRST); 
			SetCtrlVal (panel, PANEL1_BIN_ONU1_TxDIS, EVB.sStrF0.sOutput.ONU1_TxDIS);
 			SetCtrlVal (panel, PANEL1_BIN_ONU1_DESEL, EVB.sStrF0.sOutput.ONU1_DESEL);

			//sInput
			SetCtrlVal (panel, PANEL1_NUM_ONU1_RxLOS,  EVB.sStrF0.sDin1.ONU1_RxLOS); 
			SetCtrlVal (panel, PANEL1_NUM_ONU2_RxLOS,  EVB.sStrF0.sDin1.ONU2_RxLOS); 
			SetCtrlVal (panel, PANEL1_NUM_OLT_RxLOS,   EVB.sStrF0.sDin2.OLT_RxLOS); 
			SetCtrlVal (panel, PANEL1_NUM_OLT1G_RxLOS, EVB.sStrF0.sDin2.OLT1G_RxLOS); 
			SetCtrlVal (panel, PANEL1_NUM_ONU1_ABS,    EVB.sStrF0.sDin1.ONU1_ABS); 
			SetCtrlVal (panel, PANEL1_NUM_ONU2_ABS,    EVB.sStrF0.sDin1.ONU2_ABS); 
			SetCtrlVal (panel, PANEL1_NUM_ONU1_TxSD,   EVB.sStrF0.sDin2.ONU1_TxSD); 
			SetCtrlVal (panel, PANEL1_NUM_ONU2_TxSD,   EVB.sStrF0.sDin2.ONU2_TxSD); 
			SetCtrlVal (panel, PANEL1_NUM_ONU1_NR,     EVB.sStrF0.sDin1.ONU1_NR); 
			SetCtrlVal (panel, PANEL1_NUM_ONU2_NR,     EVB.sStrF0.sDin1.ONU2_NR); 
			SetCtrlVal (panel, PANEL1_NUM_ONU1_INT,    EVB.sStrF0.sDin1.ONU1_INT); 
			SetCtrlVal (panel, PANEL1_NUM_ONU2_INT,    EVB.sStrF0.sDin1.ONU2_INT); 
			SetCtrlVal (panel, PANEL1_NUM_BERT_LOL,    EVB.sStrF0.sDin2.BERT_LOL); 
			SetCtrlVal (panel, PANEL1_NUM_BERT_LOS,    EVB.sStrF0.sDin2.BERT_LOS); 
 

			//ADC
			SetCtrlVal (panel, PANEL1_NUM_ADC0, EVB.sStrF0.ADC0_MON); 
			SetCtrlVal (panel, PANEL1_NUM_ADC1, EVB.sStrF0.ADC1_MON); 
			SetCtrlVal (panel, PANEL1_NUM_ADC2, EVB.sStrF0.ADC2_Vx_MON); 
			SetCtrlVal (panel, PANEL1_NUM_ADC3, EVB.sStrF0.ADC3_Ix_MON); 
			SetCtrlVal (panel, PANEL1_NUM_ADC4, EVB.sStrF0.ADC4_MON); 
			
			
			SetCtrlVal (panel, PANEL1_NUM_ADC_12V_MON, (float) (EVB.sStrF0.ADC_12V_MON*1E-3));   //1LSB=1mV	
			SetCtrlVal (panel, PANEL1_NUM_3VMCU_MON,   (float) (EVB.sStrF0.ADC_3VMCU_MON*1E-3)); //1LSB=1mV	
			SetCtrlVal (panel, PANEL1_NUM_2VCDR_MON,   (float) (EVB.sStrF0.ADC_2VCDR_MON*1E-3)); //1LSB=1mV	
			SetCtrlVal (panel, PANEL1_NUM_OLT5V_MON, (float) (EVB.sStrF0.ADC_5VOLT_MON*1E-3));   //1LSB=1mV	
			SetCtrlVal (panel, PANEL1_NUM_OLT3V_MON, (float) (EVB.sStrF0.ADC_3VOLT_MON*1E-3));   //1LSB=1mV	
			SetCtrlVal (panel, PANEL1_NUM_ONU3V_MON, (float) (EVB.sStrF0.ADC_3VONU_MON*1E-3));   //1LSB=1mV	

			SetCtrlVal (panel, PANEL1_NUM_IOLT5V_MON,  (float) (EVB.sStrF0.ADC_IOLT5V_MON*1E-1)); //1LSB=0.1mA
			SetCtrlVal (panel, PANEL1_NUM_ITOLT_MON,  (float) (EVB.sStrF0.ADC_ITOLT_MON*1E-1)); //1LSB=0.1mA
			SetCtrlVal (panel, PANEL1_NUM_IROLT_MON,  (float) (EVB.sStrF0.ADC_IROLT_MON*1E-1)); //1LSB=0.1mA
			SetCtrlVal (panel, PANEL1_NUM_ITONU1_MON,  (float) (EVB.sStrF0.ADC_ITONU1_MON*1E-1)); //1LSB=0.1mA
			SetCtrlVal (panel, PANEL1_NUM_IRONU1_MON,  (float) (EVB.sStrF0.ADC_IRONU1_MON*1E-1)); //1LSB=0.1mA

			SetCtrlVal (panel, PANEL1_NUM_ADC_SCOLT_MON, (float) (EVB.sStrF0.ADC_SCOLT_MON*1E-3));   //1LSB=1mV	
			SetCtrlVal (panel, PANEL1_NUM_ADC_SCONU_MON, (float) (EVB.sStrF0.ADC_SCONU_MON*1E-3));   //1LSB=1mV	
			SetCtrlVal (panel, PANEL1_NUM_ADC_SCOLT_MON_O, (float) (EVB.sStrF0.ADC_SCOLT_MON_O*1E-3));   //1LSB=1mV	
			SetCtrlVal (panel, PANEL1_NUM_ADC_SCONU_MON_O, (float) (EVB.sStrF0.ADC_SCONU_MON_O*1E-3));   //1LSB=1mV	
			SetCtrlVal (panel, PANEL1_NUM_ADC_SCOLT_MON_C, (float) (EVB.sStrF0.ADC_SCOLT_MON_C*1E-3));   //1LSB=1mV	
			SetCtrlVal (panel, PANEL1_NUM_ADC_SCONU_MON_C, (float) (EVB.sStrF0.ADC_SCONU_MON_C*1E-3));   //1LSB=1mV	

			//DAC
			SetCtrlVal (panel, PANEL1_NUM_ADC12_DAC0, EVB.sStrF0.DAC0_SET_OLT5V); 
			SetCtrlVal (panel, PANEL1_NUM_ADC13_DAC1, EVB.sStrF0.DAC1_SET_OLT3V); 
			SetCtrlVal (panel, PANEL1_NUM_ADC14_DAC2, EVB.sStrF0.DAC2_SET_ONU3V); 
			SetCtrlVal (panel, PANEL1_NUM_ADC15_DAC3, EVB.sStrF0.DAC3_SW_CASE); 

			//V_adj
			SetCtrlVal (panel, PANEL1_NUM_OLT5V_expect, EVB.sStrF0.OLT5V_expect);
			SetCtrlVal (panel, PANEL1_NUM_OLT3V_expect, EVB.sStrF0.OLT3V_expect);
			SetCtrlVal (panel, PANEL1_NUM_ONU3V_expect, EVB.sStrF0.ONU3V_expect);
			SetCtrlVal (panel, PANEL1_NUM_OLT5VAdj_loop, EVB.sStrF0.OLT5VAdj_loop);
			SetCtrlVal (panel, PANEL1_NUM_OLT3VAdj_loop, EVB.sStrF0.OLT3VAdj_loop);
			SetCtrlVal (panel, PANEL1_NUM_ONU3VAdj_loop, EVB.sStrF0.ONU3VAdj_loop);
			
			


			//短路检测
			SetCtrlVal (panel, PANEL1_LED_OLT_SC_GND, EVB.sStrF0.sSC.OLT_SC_GND);
			SetCtrlVal (panel, PANEL1_LED_OLT_SC_VCC, EVB.sStrF0.sSC.OLT_SC_VCC);
			SetCtrlVal (panel, PANEL1_LED_OLT_SC_VPD, EVB.sStrF0.sSC.OLT_SC_VAPD);
			SetCtrlVal (panel, PANEL1_LED_ONU_SC_GND, EVB.sStrF0.sSC.ONU_SC_GND);
			SetCtrlVal (panel, PANEL1_LED_ONU_SC_VCC, EVB.sStrF0.sSC.ONU_SC_VCC);
			SetCtrlVal (panel, PANEL1_LED_ONU_SC_VPD, EVB.sStrF0.sSC.ONU_SC_VAPD);

			//control mode
			SetCtrlVal (panel, PANEL1_BIN_SET_MODE, EVB.sStrF0.FirstByte); 
			SetCtrlVal (panel, PANEL1_NUM_SW_CASE, EVB.sStrF0.sInput.SW_CASE); 
			
			//TRIG
			SetCtrlVal (panel, PANEL1_RNG_TrigShape, EVB.sStrF0.RSSI_mode);
			SetCtrlVal (panel, PANEL1_BIN_TrigLevel, EVB.sStrF0.sOutput.OLT_RxTRIG);
			SetCtrlVal (panel, PANEL1_BIN_TrigPolarity, EVB.sStrF0.RSSI_polarity);
			SetCtrlVal (panel, PANEL1_NUM_RSSI_PW, EVB.sStrF0.RSSI_PW);
			SetCtrlVal (panel,  PANEL1_NUM_RSSI_PWt, 0.264742857 * EVB.sStrF0.RSSI_PW + 1.478095238);
			
			switch (EVB.sStrF0.RSSI_mode) 
			{
			case 0: //level
				SetCtrlAttribute (panel, PANEL1_BIN_TrigLevel, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_RSSI_PW, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_RSSI_PWt, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BIN_TrigPolarity, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BUT_RSSI_Trig_Gen, ATTR_DIMMED, 1);
			break;

			case 1: //pulse
				SetCtrlAttribute (panel, PANEL1_BIN_TrigLevel, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_RSSI_PW, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_RSSI_PWt, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_BIN_TrigPolarity, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_BUT_RSSI_Trig_Gen, ATTR_DIMMED, 0);
			break;
			}

			//BEN
			SetCtrlVal (panel, PANEL1_RNG_BENShape, EVB.sStrF0.BEN_mode);
			SetCtrlVal (panel, PANEL1_BIN_BENLevel, EVB.sStrF0.sOutput.ONU1_TxBEN);
			SetCtrlVal (panel, PANEL1_BIN_BENPolarity, EVB.sStrF0.BEN_polarity);
			SetCtrlVal (panel, PANEL1_NUM_BEN_PW, EVB.sStrF0.BEN_PW);
			SetCtrlVal (panel,  PANEL1_NUM_BEN_PWt, 0.264742857 * EVB.sStrF0.BEN_PW + 1.478095238);
											
			switch (EVB.sStrF0.BEN_mode) 
			{
			case 0: //level
				SetCtrlAttribute (panel, PANEL1_BIN_BENLevel, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PW, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PWt, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BIN_BENPolarity, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BUT_BEN_Trig_Gen, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay1, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay2, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay3, ATTR_DIMMED, 1);
			break;

			case 1: //pulse
				SetCtrlAttribute (panel, PANEL1_BIN_BENLevel, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PW, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PWt, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_BIN_BENPolarity, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_BUT_BEN_Trig_Gen, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay1, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay2, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay3, ATTR_DIMMED, 1);
 			break;

			case 2: //clock
				SetCtrlAttribute (panel, PANEL1_BIN_BENLevel, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PW, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PWt, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BIN_BENPolarity, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BUT_BEN_Trig_Gen, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay1, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay2, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay3, ATTR_DIMMED, 1);
			break;

			case 3: //BEN_TRG, N1N2N3
				SetCtrlAttribute (panel, PANEL1_BIN_BENLevel, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PW, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PWt, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BIN_BENPolarity, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BUT_BEN_Trig_Gen, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay1, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay2, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay3, ATTR_DIMMED, 0);
			break;

			case 4: //BEN_TRG, N1N3
				SetCtrlAttribute (panel, PANEL1_BIN_BENLevel, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PW, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PWt, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BIN_BENPolarity, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BUT_BEN_Trig_Gen, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay1, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay2, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay3, ATTR_DIMMED, 0);
			break;

			case 5: //BEN_TRG, N2
				SetCtrlAttribute (panel, PANEL1_BIN_BENLevel, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PW, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PWt, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BIN_BENPolarity, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BUT_BEN_Trig_Gen, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay1, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay2, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay3, ATTR_DIMMED, 1);
			break;

			case 6: //BEN_TRG, L2L3
				SetCtrlAttribute (panel, PANEL1_BIN_BENLevel, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PW, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BEN_PWt, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BIN_BENPolarity, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_BUT_BEN_Trig_Gen, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay1, ATTR_DIMMED, 1);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay2, ATTR_DIMMED, 0);
				SetCtrlAttribute (panel, PANEL1_NUM_BENTRG_Delay3, ATTR_DIMMED, 0);
			break;
		}
			
			//DELAY
			SetCtrlVal (panel,  PANEL1_NUM_BENTRG_Delay1, EVB.sStrF0.delay1);
			SetCtrlVal (panel,  PANEL1_NUM_BENTRG_Delay2, EVB.sStrF0.delay2);
			SetCtrlVal (panel,  PANEL1_NUM_BENTRG_Delay3, EVB.sStrF0.delay3);

			break;
		}
	return 0;
}


int CVICALLBACK on_Set_DAC0 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int error;
 int reg_add;
 
	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel, PANEL1_NUM_ADC12_DAC0, &EVB.sStrF0.DAC0_SET_OLT5V);	
			reg_add =(int)(&EVB.sStrF0.DAC0_SET_OLT5V) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 2, EVB.pStrF0, 0.05);

			break;
		}
	return 0;
}

int CVICALLBACK on_Set_DAC1 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int error;
 int reg_add;
 
	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel, PANEL1_NUM_ADC13_DAC1, &EVB.sStrF0.DAC1_SET_OLT3V);	
			reg_add =(int)(&EVB.sStrF0.DAC1_SET_OLT3V) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 2, EVB.pStrF0, 0.05);

			break;
		}
	return 0;
}

int CVICALLBACK on_Set_DAC2 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int error;
 int reg_add;
 
	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel, PANEL1_NUM_ADC14_DAC2, &EVB.sStrF0.DAC2_SET_ONU3V);	
			reg_add =(int)(&EVB.sStrF0.DAC2_SET_ONU3V) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 2, EVB.pStrF0, 0.05);

			break;
		}
	return 0;
}

int CVICALLBACK on_Set_DAC3 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int error;
 int reg_add;
 
	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel, PANEL1_NUM_ADC15_DAC3, &EVB.sStrF0.DAC3_SW_CASE);	
			reg_add =(int)(&EVB.sStrF0.DAC3_SW_CASE) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 2, EVB.pStrF0, 0.05);

			break;
		}
	return 0;
}




int CVICALLBACK On_SW_CASE (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int error;
 int reg_add;
 float T_wait=0.1;
 int temp_char;
 
 	switch (event)
		{
		case EVENT_COMMIT:
			reg_add =(int)(&EVB.sStrF0.sInput) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0);

			GetCtrlVal (panel, PANEL1_NUM_SW_CASE, &temp_char);
			EVB.sStrF0.sInput.SW_CASE = temp_char;
			
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, T_wait);

			break;
		}
	return 0;
}


int CVICALLBACK On_Set_MODE (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int error;
 int reg_add;
 float T_wait=0.1;
 
 	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel, PANEL1_BIN_SET_MODE, &EVB.sStrF0.FirstByte);     
			reg_add =(int)(&EVB.sStrF0.FirstByte) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, T_wait);

			break;
		}
	return 0;
}

int CVICALLBACK On_Set_DAC0_LUT (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int i, table_index;
 int reg_add, error;
 unsigned char myARR[256];

	switch (event)
		{
		case EVENT_COMMIT:
			SetWaitCursor (1);
			
			GetCtrlVal (panel, PANEL1_NUM_ADC12_DAC0, &EVB.sStrF0.DAC0_SET_OLT5V); 
			for(i=0; i<192; i++)
			{   myDAC.DAC_ARR[0][i]=EVB.sStrF0.DAC0_SET_OLT5V;
			}

			for(table_index=1; table_index<1+3; table_index++)
			{//set table_index
 				EVB.sStrF0.LUTIndex=table_index;
 				reg_add = (int)(&EVB.sStrF0.LUTIndex)-(int)(&EVB.sStrF0.FirstByte); 
				error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.1);			
			 //save LUT
				for(i=0; i<128; i++)
					myARR[i+128] = myDAC.DAC_LUT[table_index-1][i];
				error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, 128, 128, myARR, 0.1); 
			 //check LUT
				error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, 128, 128, myARR);
				for(i=0; i<128; i++)
				{	if (myARR[i+128] != myDAC.DAC_LUT[table_index-1][i]) //write check error
					{   MessagePopup ("ERROR", "LUT check error!");
						break;
					}
				}
			}

			//'W'
			EVB.sStrF0.FirstByte = 'W';
			reg_add =(int)(&EVB.sStrF0.FirstByte) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.2);
			SetCtrlVal (panel, PANEL1_BIN_SET_MODE, 0);

			//set table_index=0
			EVB.sStrF0.LUTIndex=0;
			reg_add = (int)(&EVB.sStrF0.LUTIndex)-(int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.1);			
			
			SetWaitCursor (0);
			break;
		}
	return 0;
}

int CVICALLBACK On_Set_DAC1_LUT (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int i, table_index;
 int reg_add, error;
 unsigned char myARR[256];

	switch (event)
		{
		case EVENT_COMMIT:
			SetWaitCursor (1);

			GetCtrlVal (panel, PANEL1_NUM_ADC13_DAC1, &EVB.sStrF0.DAC1_SET_OLT3V); 
			for(i=0; i<192; i++)
			{   myDAC.DAC_ARR[1][i]=EVB.sStrF0.DAC1_SET_OLT3V;
			}

			for(table_index=4; table_index<4+3; table_index++)
			{//set table_index
 				EVB.sStrF0.LUTIndex=table_index;
 				reg_add = (int)(&EVB.sStrF0.LUTIndex)-(int)(&EVB.sStrF0.FirstByte); 
				error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.1);			
			 //save LUT
				for(i=0; i<128; i++)
					myARR[i+128] = myDAC.DAC_LUT[table_index-1][i];
				error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, 128, 128, myARR, 0.1); 
			 //check LUT
				error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, 128, 128, myARR);
				for(i=0; i<128; i++)
				{	if (myARR[i+128] != myDAC.DAC_LUT[table_index-1][i]) //write check error
					{   MessagePopup ("ERROR", "LUT check error!");
						break;
					}
				}
			}

			//'W'
			EVB.sStrF0.FirstByte = 'W';
			reg_add =(int)(&EVB.sStrF0.FirstByte) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.2);
			SetCtrlVal (panel, PANEL1_BIN_SET_MODE, 0);

			//set table_index=0
			EVB.sStrF0.LUTIndex=0;
			reg_add = (int)(&EVB.sStrF0.LUTIndex)-(int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.1);			
			
			SetWaitCursor (0);
			break;
		}
	return 0;
}

int CVICALLBACK On_Set_DAC2_LUT (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int i, table_index;
 int reg_add, error;
 unsigned char myARR[256];

	switch (event)
		{
		case EVENT_COMMIT:
			SetWaitCursor (1);

			GetCtrlVal (panel, PANEL1_NUM_ADC14_DAC2, &EVB.sStrF0.DAC2_SET_ONU3V); 
			for(i=0; i<192; i++)
			{   myDAC.DAC_ARR[2][i]=EVB.sStrF0.DAC2_SET_ONU3V;
			}

			for(table_index=7; table_index<7+3; table_index++)
			{//set table_index
 				EVB.sStrF0.LUTIndex=table_index;
 				reg_add = (int)(&EVB.sStrF0.LUTIndex)-(int)(&EVB.sStrF0.FirstByte); 
				error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.1);			
			 //save LUT
				for(i=0; i<128; i++)
					myARR[i+128] = myDAC.DAC_LUT[table_index-1][i];
				error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, 128, 128, myARR, 0.1); 
			 //check LUT
				error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, 128, 128, myARR);
				for(i=0; i<128; i++)
				{	if (myARR[i+128] != myDAC.DAC_LUT[table_index-1][i]) //write check error
					{   MessagePopup ("ERROR", "LUT check error!");
						break;
					}
				}
			}

			//'W'
			EVB.sStrF0.FirstByte = 'W';
			reg_add =(int)(&EVB.sStrF0.FirstByte) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.2);
			SetCtrlVal (panel, PANEL1_BIN_SET_MODE, 0);

			//set table_index=0
			EVB.sStrF0.LUTIndex=0;
			reg_add = (int)(&EVB.sStrF0.LUTIndex)-(int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.1);			
			
			SetWaitCursor (0);
			break;
		}
	return 0;
}

int CVICALLBACK On_Set_DAC3_LUT (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int i, table_index;
 int reg_add, error;
 unsigned char myARR[256];

	switch (event)
		{
		case EVENT_COMMIT:
			SetWaitCursor (1);

			GetCtrlVal (panel, PANEL1_NUM_ADC15_DAC3, &EVB.sStrF0.DAC3_SW_CASE); 
			for(i=0; i<192; i++)
			{   myDAC.DAC_ARR[3][i]=EVB.sStrF0.DAC3_SW_CASE;
			}

			for(table_index=10; table_index<10+3; table_index++)
			{//set table_index
 				EVB.sStrF0.LUTIndex=table_index;
 				reg_add = (int)(&EVB.sStrF0.LUTIndex)-(int)(&EVB.sStrF0.FirstByte); 
				error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.1);			
			 //save LUT
				for(i=0; i<128; i++)
					myARR[i+128] = myDAC.DAC_LUT[table_index-1][i];
				error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, 128, 128, myARR, 0.1); 
			 //check LUT
				error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, 128, 128, myARR);
				for(i=0; i<128; i++)
				{	if (myARR[i+128] != myDAC.DAC_LUT[table_index-1][i]) //write check error
					{   MessagePopup ("ERROR", "LUT check error!");
						break;
					}
				}
			}

			//'W'
			EVB.sStrF0.FirstByte = 'W';
			reg_add =(int)(&EVB.sStrF0.FirstByte) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.2);
			SetCtrlVal (panel, PANEL1_BIN_SET_MODE, 0);

			//set table_index=0
			EVB.sStrF0.LUTIndex=0;
			reg_add = (int)(&EVB.sStrF0.LUTIndex)-(int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.1);			
			
			SetWaitCursor (0);
			break;
		}
	return 0;
}






int CVICALLBACK on_ONU1_PDRST (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int temp_char;
 int reg_add, error;
 
	switch (event)
		{
		case EVENT_COMMIT:

			reg_add =(int)(&EVB.sStrF0.sOutput) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0);

			GetCtrlVal (panel, PANEL1_BIN_ONU1_PDRST, &temp_char);
			EVB.sStrF0.sInput.ONU1_PDRST = temp_char; 

			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.00);//写RAM，可以不等待
			
			break;
		}
	return 0;
}

int CVICALLBACK on_OLT_RxRST (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int temp_char;
 int reg_add, error;
 
	switch (event)
		{
		case EVENT_COMMIT:

			reg_add =(int)(&EVB.sStrF0.sOutput) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0);

			GetCtrlVal (panel, PANEL1_BIN_OLT_RxRST, &temp_char);
			EVB.sStrF0.sOutput.OLT_RxRST = temp_char; 

			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.00);//写RAM，可以不等待
			
			break;
		}
	return 0;
}

int CVICALLBACK on_OLT_TxDIS (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int temp_char;
 int reg_add, error;
 
	switch (event)
		{
		case EVENT_COMMIT:

			reg_add =(int)(&EVB.sStrF0.sOutput) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0);

			GetCtrlVal (panel, PANEL1_BIN_OLT_TxDIS, &temp_char);
			EVB.sStrF0.sOutput.OLT_TxDIS = temp_char; 

			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.00);//写RAM，可以不等待
			
			break;
		}
	return 0;
}

int CVICALLBACK on_OLT1G_RxRST (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int temp_char;
 int reg_add, error;
 
	switch (event)
		{
		case EVENT_COMMIT:

			reg_add =(int)(&EVB.sStrF0.sOutput) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0);

			GetCtrlVal (panel, PANEL1_BIN_OLT1G_RxRST, &temp_char);
			EVB.sStrF0.sOutput.OLT1G_RxRST = temp_char; 

			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.00);//写RAM，可以不等待
			
			break;
		}
	return 0;
}

int CVICALLBACK on_OLT1G_TxDIS (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int temp_char;
 int reg_add, error;
 
	switch (event)
		{
		case EVENT_COMMIT:

			reg_add =(int)(&EVB.sStrF0.sOutput) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0);

			GetCtrlVal (panel, PANEL1_BIN_OLT1G_TxDIS, &temp_char);
			EVB.sStrF0.sOutput.OLT1G_TxDIS = temp_char; 

			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.00);//写RAM，可以不等待
			
			break;
		}
	return 0;
}


int CVICALLBACK on_Set_OLT5V (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int error, i;
 int reg_add;
 float T_wait=0.1;
 
 
	switch (event)
		{
		case EVENT_COMMIT:
			SetWaitCursor (1);
			
			SetCtrlVal (panel, PANEL1_NUM_OLT5VAdj_loop, 0); 
			GetCtrlVal (panel, PANEL1_NUM_OLT5V_expect, &EVB.sStrF0.OLT5V_expect);
			reg_add = (int)(&EVB.sStrF0.OLT5V_expect)-(int)(&EVB.sStrF0.FirstByte);
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 4, EVB.pStrF0, T_wait); 

			EVB.sStrF0.OLT5VAdj_en = 1;
			reg_add = (int)(&EVB.sStrF0.OLT5VAdj_en)-(int)(&EVB.sStrF0.FirstByte);
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, T_wait); 
			
			reg_add = (int)(&EVB.sStrF0.Tune_OLT5V_MON)-(int)(&EVB.sStrF0.FirstByte);
			
			i=0;
			do
			{   Delay(0.1); 
				error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, reg_add, 4, EVB.pStrF0);
				SetCtrlVal (panel, PANEL1_NUM_OLT5V_MON, EVB.sStrF0.Tune_OLT5V_MON/1000.); 
				SetCtrlVal (panel, PANEL1_NUM_OLT5VAdj_loop, EVB.sStrF0.OLT5VAdj_loop);
				i++;
			}while ((EVB.sStrF0.OLT5VAdj_en==1) && (i<30) && ((error>=0)));
					
			error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, 0, 128, EVB.pStrF0);
			SetCtrlVal (panel, PANEL1_NUM_OLT5VAdj_loop, EVB.sStrF0.OLT5VAdj_loop);
			SetCtrlVal (panel, PANEL1_NUM_OLT5V_MON, EVB.sStrF0.ADC_5VOLT_MON/1000.); 
			SetCtrlVal (panel, PANEL1_NUM_ADC12_DAC0, EVB.sStrF0.DAC0_SET_OLT5V); 
			
			SetWaitCursor (0);			
			break;
		}
	return 0;
}

int CVICALLBACK on_Set_OLT3V (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int error, i;
 int reg_add;
 float T_wait=0.1;
 
 
	switch (event)
		{
		case EVENT_COMMIT:
			SetWaitCursor (1);
			
			SetCtrlVal (panel, PANEL1_NUM_OLT3VAdj_loop, 0); 
			GetCtrlVal (panel, PANEL1_NUM_OLT3V_expect, &EVB.sStrF0.OLT3V_expect);
			reg_add = (int)(&EVB.sStrF0.OLT3V_expect)-(int)(&EVB.sStrF0.FirstByte);
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 4, EVB.pStrF0, T_wait); 

			EVB.sStrF0.OLT3VAdj_en = 1;
			reg_add = (int)(&EVB.sStrF0.OLT3VAdj_en)-(int)(&EVB.sStrF0.FirstByte);
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, T_wait); 
			
			reg_add = (int)(&EVB.sStrF0.Tune_OLT3V_MON)-(int)(&EVB.sStrF0.FirstByte);
			
			i=0;
			do
			{   Delay(0.1); 
				error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, reg_add, 4, EVB.pStrF0);
				SetCtrlVal (panel, PANEL1_NUM_OLT3V_MON, EVB.sStrF0.Tune_OLT3V_MON/1000.); 
				SetCtrlVal (panel, PANEL1_NUM_OLT3VAdj_loop, EVB.sStrF0.OLT3VAdj_loop);
				i++;
			}while ((EVB.sStrF0.OLT3VAdj_en==1) && (i<30) && ((error>=0)));
					
			error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, 0, 128, EVB.pStrF0);
			SetCtrlVal (panel, PANEL1_NUM_OLT3VAdj_loop, EVB.sStrF0.OLT3VAdj_loop);
			SetCtrlVal (panel, PANEL1_NUM_OLT3V_MON, EVB.sStrF0.ADC_3VOLT_MON/1000.); 
			SetCtrlVal (panel, PANEL1_NUM_ADC13_DAC1, EVB.sStrF0.DAC1_SET_OLT3V); 
			
			SetWaitCursor (0);			
			break;
		}
	return 0;
}

int CVICALLBACK on_Set_ONU3V (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int error, i;
 int reg_add;
 float T_wait=0.1;
 
 
	switch (event)
		{
		case EVENT_COMMIT:
			SetWaitCursor (1);
			
			SetCtrlVal (panel, PANEL1_NUM_ONU3VAdj_loop, 0); 
			GetCtrlVal (panel, PANEL1_NUM_ONU3V_expect, &EVB.sStrF0.ONU3V_expect);
			reg_add = (int)(&EVB.sStrF0.ONU3V_expect)-(int)(&EVB.sStrF0.FirstByte);
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 4, EVB.pStrF0, T_wait); 

			EVB.sStrF0.ONU3VAdj_en = 1;
			reg_add = (int)(&EVB.sStrF0.ONU3VAdj_en)-(int)(&EVB.sStrF0.FirstByte);
			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, T_wait); 
			
			reg_add = (int)(&EVB.sStrF0.Tune_ONU3V_MON)-(int)(&EVB.sStrF0.FirstByte);

			i=0;
			do
			{   Delay(0.1); 
				error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, reg_add, 4, EVB.pStrF0);
				SetCtrlVal (panel, PANEL1_NUM_ONU3V_MON, EVB.sStrF0.Tune_ONU3V_MON/1000.); 
				SetCtrlVal (panel, PANEL1_NUM_ONU3VAdj_loop, EVB.sStrF0.ONU3VAdj_loop);
				i++;
			}while ((EVB.sStrF0.ONU3VAdj_en==1) && (i<30) && ((error>=0)));
					
			error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, 0, 128, EVB.pStrF0);
			SetCtrlVal (panel, PANEL1_NUM_ONU3VAdj_loop, EVB.sStrF0.ONU3VAdj_loop);
			SetCtrlVal (panel, PANEL1_NUM_ONU3V_MON, EVB.sStrF0.ADC_3VONU_MON/1000.); 
			SetCtrlVal (panel, PANEL1_NUM_ADC14_DAC2, EVB.sStrF0.DAC2_SET_ONU3V); 

			SetWaitCursor (0);			
			break;
		}
	return 0;
}


int CVICALLBACK on_ONU1_TxDIS (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int temp_char;
 int reg_add, error;
 
	switch (event)
		{
		case EVENT_COMMIT:

			reg_add =(int)(&EVB.sStrF0.sOutput) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0);

			GetCtrlVal (panel, PANEL1_BIN_ONU1_TxDIS, &temp_char);
			EVB.sStrF0.sOutput.ONU1_TxDIS = temp_char; 

			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.00);//写RAM，可以不等待
			
			break;
		}
	return 0;
}

int CVICALLBACK on_ONU1_DESEL (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int temp_char;
 int reg_add, error;
 
	switch (event)
		{
		case EVENT_COMMIT:

			reg_add =(int)(&EVB.sStrF0.sOutput) - (int)(&EVB.sStrF0.FirstByte); 
			error = I2C_BYTEs_READ_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0);

			GetCtrlVal (panel, PANEL1_BIN_ONU1_DESEL, &temp_char);
			EVB.sStrF0.sOutput.ONU1_DESEL = temp_char; 

			error = I2C_BYTEs_WRITE_DLL (USBHandle, EVB_addr, reg_add, 1, EVB.pStrF0, 0.00);//写RAM，可以不等待
			SDN_VCC_F320_DLL(USBHandle-0x800, 0);
			SDN_VCC_F320_DLL(USBHandle-0x800, 3);
			break;
		}
	return 0;
}

int CVICALLBACK on_TEXTMSG_135 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_LEFT_DOUBLE_CLICK:
			SetCtrlAttribute (panel, PANEL1_STR_EVBPN, ATTR_CTRL_MODE, VAL_HOT);

			break;
		}
	return 0;
}

int CVICALLBACK on_PWR_TRX (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int PWR_ONU, PWR_OLT;
 int error;
 
	switch (event)
		{
		case EVENT_COMMIT:
		
			GetCtrlVal (panel, PANEL1_BIN_PWR_ONU, &PWR_ONU);
			GetCtrlVal (panel, PANEL1_BIN_PWR_OLT, &PWR_OLT);

			error = SDN_VCC_F320_DLL (USBHandle-0x800, (PWR_ONU<<1)+PWR_OLT);
		break;
		}
	return 0;
}

int CVICALLBACK on_SET_FS (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int FS;
//    0x00：选择晶振输出频率115.52MHz
//    0x01: 选择晶振输出频率161.13281MHz
//    0x02: 选择晶振输出频率167.33165MHz
//    0x03: 选择晶振输出频率173.37075MHz
 int error; 
 
	switch (event)
		{
		case EVENT_COMMIT:
		
			GetCtrlVal (panel, PANEL1_RNG_SET_FS, &FS);

			error = SET_FS_F320_DLL (USBHandle-0x800, FS);
		break;
		}
	return 0;
}

int CVICALLBACK on_SEL_I2C (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{int IIC;
//    数据的bit0：OLT IIC总线使能，1为使能，0禁止
//    数据的bit1：ONU1 IIC总线使能，1为使能，0禁止
//    数据的bit2：BERT IIC总线使能，1为使能，0禁止
//    数据的bit3：ARM IIC总线使能，1为使能，0禁止	
 int error;
 int I2C_OLT, I2C_ONU1, I2C_BERT, I2C_7020;
 
 	switch (event)
 		{
		case EVENT_COMMIT:
			GetCtrlVal (panel, PANEL1_BIN_I2C_OLT, &I2C_OLT);
			GetCtrlVal (panel, PANEL1_BIN_I2C_ONU1, &I2C_ONU1);
			GetCtrlVal (panel, PANEL1_BIN_I2C_BERT, &I2C_BERT);
			GetCtrlVal (panel, PANEL1_BIN_I2C_7020, &I2C_7020);
		 
			error = SEL_IIC_F320_DLL (USBHandle-0x800, (I2C_7020<<3)+(I2C_BERT<<2)+(I2C_ONU1<<1)+I2C_OLT);

			break;									  
		}
	return 0;
}
	 */
