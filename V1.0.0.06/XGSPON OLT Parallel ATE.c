#include "combobox.h"
#include "CVIDLL.h" 
#include "function.h" 
#include "EVB5.h"  
#include "CH341A_DLL.h"   
#include "HP8156A.h"
#include "GBERT.h"
#include "EVB16.h" 
#include "EVB11.h"
#include "global.h"    
#include "toolbox.h"
#include "XGSPON OLT Parallel ATE.h" 
//#include "XGSPON_OLT.h" 
#include <cvirte.h>		
#include <userint.h>
#include "About.h"
#include "TxCalibration.h"  
#include "FPY.h"
#include "Measure Data.h"		// c:\Users\Wenyao.xi\Desktop\06\V1.0.0.06\

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static global variables
static int panMain;
static int panInst;
static int panInit; 
static int panOrder;	//随工单录入界面 
static int panPopu;		//退出程序确认对话框 
static int panConf;     //功能配置
static int panCaliR_Lock;		//锁定模式主光路校准界面
static int panCaliR_Free;		//衰减模式主光路校准界面 
static int panAbout;
static int panMenu; 
static int panAdva;    //高级功能界面 
static int panCtr;     //控制命令界面 
static int panSearchData_chan0; // 通道0测试数据界面

//控制命令界面Table    
static int TabPanel_Init;
static int TabPanel_GetSN; 
static int TabPanel_GetTemper;
static int TabPanel_TunAOP1G; 
static int TabPanel_TunAOP10G; 
static int TabPanel_TunER1G; 
static int TabPanel_TunER10G; 
static int TabPanel_TunCross1G; 
static int TabPanel_TunCross10G; 
static int TabPanel_TxCal1G; 
static int TabPanel_TxCal10G; 
static int TabPanel_TunAPD; 
static int TabPanel_TunSD;
static int TabPanel_RxCal; 
static int TabPanel_Other;


//多线程变量
static int ghThreadLocalVar;	//多线程本地变量
static int gFlag;				//多线程启动标志
static unsigned int gIndexVal[CHANNEL_MAX]     = {0, 1, 2, 3, 4, 5, 6, 7}; //多线程函数参数值,实际传入的是测试通道值
static int 			gThreadFuncId[CHANNEL_MAX] = {0, 0, 0, 0, 0, 0, 0, 0}; //多线程ID
static char 		szErrorMsg[CMT_MAX_MESSAGE_BUF_SIZE];
static char	   		g_Operator[10];

int ThreadHandle;   
int Chan0_Data_lines = 1;

//==============================================================================
// Static functions
int CVICALLBACK tun_DWDM_10G (void *pvFuncData);
int CVICALLBACK test_DWDM_10G (void *pvFuncData);	
int CVICALLBACK test_1G (void *pvFuncData);
int CVICALLBACK QAtest_10G (void *pvFuncData);
int CVICALLBACK QAtest_1G (void *pvFuncData);
int CVICALLBACK tun_DWDM (void *pvFuncData);
int CVICALLBACK test_DWDM (void *pvFuncData);


static void CleanUpThreads (void);
static void GetLock (int threadLock, int *obtainedLock);
//==============================================================================
// 
void Set_RingCtrl(int tabPanel,int SubPanel,int value);  
void Set_Control_Table(void);
//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions


int testParameterLock1(int channel, struct struTestData *data, int panel, int control, char *Info, char *ERInfo)
{
	int bOK=TRUE;
	BOOL Status;
	int error=0;
	//ER
	if (bOK) 
	{
		strcpy (Info, "测试消光比...");
		Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		Status = testER10G(channel, data);
		if (Status) {sprintf (Info, "测试消光比=%.2fdB 成功", data->ER);}
		else		{sprintf (Info, "测试消光比=%.2fdB 失败", data->ER);bOK=FALSE;data->ErrorCode=ERR_TES_ER;error=-1;} 	  
		Insert_Info(panMain, gCtrl_BOX[channel], Info); 
	}
	//光谱
	if (bOK)
	{
		strcpy (Info, "测试光谱...");
		Insert_Info(panMain, gCtrl_BOX[channel], Info); 				
		Status = testSpectrum(channel, data); 
		if (Status) {sprintf (Info, "测试光谱：成功 中心波长=%.2f  带宽=%.2f  SMSR=%.2f", data->PeakWL,  data->Bandwidth, data->SMSR);}
		else		{sprintf (Info, "测试光谱：失败 中心波长=%.2f  带宽=%.2f  SMSR=%.2f", data->PeakWL,  data->Bandwidth, data->SMSR);bOK=FALSE;data->ErrorCode=ERR_TES_TEST_SPECTRUM;error=-1;}	   
		Insert_Info(panMain, gCtrl_BOX[channel], Info); 
	}
	return error;
}

int testParameterLock(int channel, struct struTestData *data, int panel, int control, char *Info, char *ERInfo)
{
	int error=0;
	int obtainedLock = 0;
	double delay;
	int bOK=TRUE;
	BOOL Status;
	BOOL Status_AOP = FALSE;
	BOOL Status_ER = FALSE;
	BOOL Status_WL = FALSE;
	int numWL = 0;
	int TecValue = 0xF900;

	GetLock(ThreadLock, &obtainedLock); 
	if (obtainedLock)
    {
		//切换时钟
		if (CLOCK_TYPE_NONE != my_struInstrument[channel].CLOCK_TYPE)
		{
			errChk(SEVB0027_4001_SetChannel(inst_SEVB0027_4001, my_struInstrument[channel].CLOCK_CHAN));
		}	 
	
		if (my_struInstrument[channel].DCA_TYPE==DCA_TYPE_CSA8000) 
		{
			if (!CSA8000_Set_O(inst_CSA8000, my_struCSA8000.Channel_O, my_struCSA8000.X_Scale, my_struCSA8000.X_Position, my_struCSA8000.Y_Scale, my_struCSA8000.Y_Position, my_struCSA8000.Y_Offset, my_struCSA8000.Wavelength, my_struCSA8000.Filter, my_struCSA8000.MaskName)) 
			{
				MessagePopup("Error", "CSA8000 Config for Optics Channel error!");
				error=-1; 
				goto Error;
			} 
		}
		else if (my_struInstrument[channel].DCA_TYPE==DCA_TYPE_Ag86100)
		{
			if (!Ag86100_SET_O(inst_Ag86100, my_struAg86100.ERFactor, my_struAg86100.X_Scale, my_struAg86100.X_Position, my_struAg86100.Y_Scale, my_struAg86100.Y_Offset, my_struAg86100.Filter, my_struAg86100.Wavelength, my_struAg86100.MaskName, my_struAg86100.MaskMargin,my_struAg86100.Channel_O))   
			{
				MessagePopup("Error", "Initial Ag86100 error!");
				error=-1;              
				goto Error;
			}
		}
		else if (my_struInstrument[channel].DCA_TYPE==DCA_TYPE_NONE)
		{
			;
		}	
		else 
		{
			sprintf (Info, "通道%d 示波器型号配置错误    ", channel); 
			MessagePopup("Error", Info);
			error=-1;
			goto Error;
		}
		
		if (my_struConfig.DT_Tun_DWDM)
		{
			// 先调试DWDM
			// 调试波长
			if (bOK && (error==0))
			{
				//设置默认tec温度值
				SET_10G_DWDM(INST_EVB[channel], TecValue);
				strcpy (Info, "调试波长...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
				Status_WL = testSpectrum(channel, data);
				if (Status_WL == FALSE)
				{
					Status_WL = tunSpectrum_WL_DWDM(channel,data, &TecValue);
				}
				// 重调光功率
				Status_AOP = tuningAOP10G(channel, data);
				// 最后再检查一遍光波长
				Status_WL = testSpectrum(channel, data);
				Status_WL = tunSpectrum_WL_DWDM(channel,data, &TecValue);
				Status = Status_WL && Status_AOP;
				data->Bandwidth = data->Bandwidth * 1000000000;
				if (Status) 
				{	// 添加SMSR
					sprintf (Info, "调试波长：成功 中心波长=%.2f  带宽=%.2f  SMSR=%.2f", data->PeakWL,  data->Bandwidth, data->SMSR);
				}
				else
				{
					if (Status_WL == TRUE && (Status_AOP == FALSE))
					{
						sprintf (Info, "调试波长：成功；光功率: 失败");
						bOK=FALSE;
						error=-1;
					}
					if (Status_WL == FALSE)
					{
						sprintf (Info, "调试波长：失败 中心波长=%.2f  带宽=%.2f  SMSR=%.2f", data->PeakWL,  data->Bandwidth, data->SMSR);
						bOK=FALSE;
						error=-1;
						
					}
				}
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
				
			}
			
			// 调试消光比
			//ER  
			if (bOK && my_struConfig.Sel_T_ER && (error==0))
			{	
			 	strcpy (Info, "调试消光比...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info);          
				Status = tuningER10G(panMain, channel, data);
				if (Status) {sprintf (Info, "调试消光比=%.2fdB;光功率=%.2fdB成功,ModDac=0x%X", data->ER,data->AOP,data->DACMod);}
				else	  	
				{
				 	sprintf (Info, "调试消光比=%.2fdB 失败;光功率=%.2fdB成功,ModDac=0x%X", data->ER,data->AOP,data->DACMod);bOK=FALSE;data->ErrorCode=ERR_TUN_ER;error=-1;    
					Insert_Info(panMain, gCtrl_BOX[channel], Info); 
					if(data->AOP_CheckFlag)
					{
						bOK=TRUE;data->ErrorCode=0;error=0;
						strcpy (Info, "重调试光功率...");
						Insert_Info(panMain, gCtrl_BOX[channel], Info);          
						Status = tuningAOP10G_R(channel, data);
						if (Status) {sprintf (Info, "重调试光功率=%.2fdBm 成功;Ibias=%.2fmA,DAC=0x%X", data->AOP,data->A2_Ibias,data->DACApc);}
						else	    {sprintf (Info, "重调试光功率=%.2fdBm 失败;Ibias=%.2fmA,DAC=0x%X", data->AOP,data->A2_Ibias,data->DACApc);error=-1;data->ErrorCode=ERR_TUN_AOP;}       
						Insert_Info(panMain, gCtrl_BOX[channel], Info); 
					
						if(0 == error)
						{
						 	strcpy (Info, "调试消光比...");
							Insert_Info(panMain, gCtrl_BOX[channel], Info);          
							Status = tuningER10G(panMain, channel, data);
							if (Status) {sprintf (Info, "重调试消光比=%.2fdB;光功率=%.2fdB成功,ModDac=0x%X", data->ER,data->AOP,data->DACMod);}
							else        {sprintf (Info, "重调试消光比=%.2fdB 失败;光功率=%.2fdB成功,ModDac=0x%X", data->ER,data->AOP,data->DACMod);bOK=FALSE;data->ErrorCode=ERR_TUN_ER;error=-1; }
						}
					}
					else
					{
						sprintf (Info, "调试消光比=%.2fdB 失败;光功率=%.2fdB成功,ModDac=0x%X", data->ER,data->AOP,data->DACMod);bOK=FALSE;data->ErrorCode=ERR_TUN_ER;error=-1;
					}
				}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			} 
			
			//调试交叉点
			if (bOK  && my_struConfig.Sel_T_Eye && (error==0))
			{
				strcpy (Info, "调试交叉点...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info);
				SET_10G_DWDM_Cross(INST_EVB[channel]);
				Status = tuningCross_10G(channel, data);                   
				if (Status) {sprintf (Info, "调试交叉点=%.2f 成功", data->TxCross);}  
				else	  	{sprintf (Info, "调试交叉点=%.2f 失败", data->TxCross);bOK=FALSE;error=-1;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info);  
			}
					
			if (bOK && (error==0))
			{
			    // 复测一下，光功率，波长和消光比
			    strcpy (Info, "复测光功率、波长和消光比...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info);
				SET_10G_DWDM(INST_EVB[channel], m_WLDac_normal);
				Status_ER = tuningER10G(panMain, channel, data);
				// 重调光功率
				//Status_AOP = tuningAOP10G(channel, data);
				Status_AOP = testDWDMAOP10G(channel, data);
				// 最后再检查一遍光波长
				Status_WL = testSpectrum(channel, data); 
				//Status_WL = tunSpectrum_WL_DWDM(channel,data, &TecValue);
				Status = Status_WL && Status_AOP && Status_ER;
				// 读取常温中心波长DAC 和核温
				(void)ReadNormalTemper(channel, data);
				if (Status) {sprintf (Info, "复测光功率、波长和消光比：成功");}
				else	    {sprintf (Info, "复测光功率、波长和消光比：失败");bOK=FALSE;error=-1;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			}
			
			//调试眼图
		 	if (bOK && my_struConfig.Sel_T_Eye && (error==0)) 
			{
				strcpy (Info, "测试发端眼图...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info);          
				Status = testOEye(channel, data);
				if (Status) {sprintf (Info, "测试发端眼图：成功");}
				else	    {sprintf (Info, "测试发端眼图：失败");bOK=FALSE;data->ErrorCode=ERR_TUN_TEST_O_EYE;error=-1;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			}
			
			//test mask
			if (bOK && my_struConfig.Sel_T_Mask && (error==0)) 
			{
				strcpy (Info, "测试发端模板...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info);          
				Status = testOEye10G_Mask(channel, data);
				if (Status) {sprintf (Info, "测试发端模板：成功");}
				else	    {sprintf (Info, "测试发端模板：失败");bOK=FALSE;data->ErrorCode=ERR_TUN_TEST_O_MASK;error=-1;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			}
			
		}
		else if (my_struConfig.DT_Test_DWDM)
		{
			// 后测试DWDM
			// ER
			if (bOK && my_struConfig.Sel_T_ER) 
			{
			 	strcpy (Info, "测试消光比...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
				Status = testER10G(channel, data);
				if (Status) {sprintf (Info, "测试消光比=%.2fdB 成功", data->ER);}
				else	    {sprintf (Info, "测试消光比=%.2fdB 失败", data->ER);bOK=FALSE;data->ErrorCode=ERR_TES_ER;error=-1;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			}
			
			//test eye
		 	if (bOK && my_struConfig.Sel_T_Eye) 
			{
				strcpy (Info, "测试发端眼图...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
				Status = testOEye (channel, data);
				if (Status) {sprintf (Info, "测试发端眼图：成功");}
				else	    {sprintf (Info, "测试发端眼图：失败");bOK=FALSE;data->ErrorCode=ERR_TES_TEST_O_EYE;error=-1;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			}	

			//test mask
			if (bOK && my_struConfig.Sel_T_Mask) 
			{
				strcpy (Info, "测试发端模板...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
				Status = testOEye10G_Mask (channel, data);
				if (Status) {sprintf (Info, "测试发端模板：成功");}
				else	    {sprintf (Info, "测试发端模板：失败");bOK=FALSE;data->ErrorCode=ERR_TES_TEST_O_MASK;error=-1;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			}
			
			//cross
			if (bOK && my_struConfig.Sel_T_Cross)
			{
				strcpy (Info, "测试交叉点...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
				Status = testCross(channel, data);     
				if (Status) {sprintf (Info, "测试交叉点=%.2f 成功", data->TxCross);}
				else	    {sprintf (Info, "测试交叉点=%.2f 失败", data->TxCross);bOK=FALSE;data->ErrorCode=ERR_TUN_AOP;error=-1;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			}

			//光谱
			if (bOK && my_struConfig.Sel_T_Spectrum)
			{
				strcpy (Info, "测试光谱...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 			    
				Status = testSpectrum(channel, data);
				data->Bandwidth = data->Bandwidth * 1000000000;
				if (Status) {sprintf (Info, "测试光谱：成功 中心波长=%.2f  带宽=%.2f  SMSR=%.2f", data->PeakWL,  data->Bandwidth, data->SMSR);}
				else	  	{sprintf (Info, "测试光谱：失败 中心波长=%.2f  带宽=%.2f  SMSR=%.2f", data->PeakWL,  data->Bandwidth, data->SMSR);bOK=FALSE;data->ErrorCode=ERR_TES_TEST_SPECTRUM;error=-1;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			}
			
		}
		
Error:	// 注意 只能在此函数块内部使用errChk宏	
        CmtReleaseLock(ThreadLock);
    }
	else
	{
		Insert_Info(panel, control, "获取设备控制权失败    ");
		return -1;
	}

	return error;
}

/*--------------------------------------------------------------------------*/
/* Wait for threads to finish and reset the function ids				    */
/*--------------------------------------------------------------------------*/
static void CleanUpThreads (void)
{
	int i;
	
	if(0 != ThreadHandle)
	{
		CmtDiscardThreadPool (ThreadHandle);
		ThreadHandle = 0;
	}
}

static void GetLock (int tmpthreadLock, int *obtainedLock)
{
	double timeout = 300;  //timeout 300s, 5 min
    double startTime = Timer();
	
    do
    {
        // NOTE - If needed, you can process messages, etc, here.
        CmtTryToGetLock(tmpthreadLock, obtainedLock);
    } while (!*obtainedLock && Timer() - startTime < timeout);
}

int main (int argc, char *argv[])
{
    int error = 0;
	char version[256], buf[50];       
	int index;
	
	if (InitCVIRTE (0, argv, 0) == 0)
	{
		return -1;	/* out of memory */
	}
	
    errChk (panMain = 		LoadPanel (0, "XGSPON OLT Parallel ATE.uir", PAN_MAIN));
	errChk (panInst = 		LoadPanel (0, "XGSPON OLT Parallel ATE.uir", PAN_INST));
	errChk (panInit = 		LoadPanel (0, "XGSPON OLT Parallel ATE.uir", PAN_INIT));
	errChk (panPopu = 		LoadPanel (0, "XGSPON OLT Parallel ATE.uir", PAN_POPU));
	errChk (panOrder = 		LoadPanel (0, "XGSPON OLT Parallel ATE.uir", PAN_ORDER)); 
	errChk (panConf = 		LoadPanel (0, "XGSPON OLT Parallel ATE.uir", PAN_CONF)); 
	errChk (panCaliR_Lock = LoadPanel (0, "XGSPON OLT Parallel ATE.uir", PAN_RCALL)); 
	errChk (panCaliR_Free = LoadPanel (0, "XGSPON OLT Parallel ATE.uir", PAN_RCAL_F));     
	errChk (panAdva = 		LoadPanel (0, "XGSPON OLT Parallel ATE.uir", PAN_ADVA)); 
	errChk (panSN = 		LoadPanel (0, "XGSPON OLT Parallel ATE.uir", PAN_SN)); 
	errChk (panCtr = 		LoadPanel (0, "XGSPON OLT Parallel ATE.uir", PAN_CTR));  
	errChk (phCaliT = 		LoadPanel (0, "TxCalibration.uir", PAN_CALI_T)); 
	errChk (phCaliLimit = 	LoadPanel (0, "TxCalibration.uir", PAN_CALI_L)); 
	errChk (phCheckT = 		LoadPanel (0, "TxCalibration.uir", PAN_CHE_T)); 
	errChk (panAbout = 		LoadPanel (0, "About.uir", PAN_ABOUT));   
	errChk (panSearchData_chan0 = 		LoadPanel (0, "XGSPON OLT Parallel ATE.uir", PAN_SEAR0));
	
	Combo_NewComboBox (panOrder, PAN_ORDER_COMBO_ORDER);    
	
	panMenu = GetPanelMenuBar (panMain);

	MyDLLGetServerType(&my_struConfig.servertype); 
		
	sprintf (version, "Combo PON OLT并行测试软件\nEXP0000xxx\n%s/%s",SOFTVER,SOFTDATE);
    SetCtrlVal (panAbout, PAN_ABOUT_TEXTMSG, version); 
	
	//校准界面加载控件值
	for (index=0; index<CHANNEL_MAX; index++)
	{
		sprintf (buf, "%d", index);
  		SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (1, 3*index+1), buf); 
		SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (1, 3*index+2), buf);
		SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (1, 3*index+3), buf);  
	
		SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (2, 3*index+1), "发端光路");
		SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (2, 3*index+2), "收端光路锁定模式");  
		SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (2, 3*index+3), "收端光路衰减模式"); 

		SetTableCellVal (phCheckT, PAN_CHE_T_TABLE, MakePoint (1, index+1), buf);
		
  		SetTableCellVal (phCaliT, PAN_CALI_T_TABLE, MakePoint (1, 3*index+1), buf); 
		SetTableCellVal (phCaliT, PAN_CALI_T_TABLE, MakePoint (1, 3*index+2), buf);
		SetTableCellVal (phCaliT, PAN_CALI_T_TABLE, MakePoint (1, 3*index+3), buf);        
	}
	
	showphID = License_Flag_INIT; 
	ThreadHandle = 0;
	g_HZDCA_Init_Flag = FALSE;    
	
	if (!MyDLL_License_Load())
	{
		return -1;
	}
	
	/* Create the thread local variable */
	CmtNewThreadLocalVar (sizeof(unsigned int), NULL, NULL, NULL, &ghThreadLocalVar);
	
	CmtNewLock("TxDCA", 0, &ThreadLock);
	CmtNewLock("ModuleSN", 0, &ThreadLock_ModuleSN);
	CmtNewLock("SuperCMD", 0, &ThreadLock_SuperCMD);    
	CmtNewLock("JW8504",0,&ThreadLock_JW8504);
	CmtNewLock("pss",0,&Thread_PSS_PowerPeter);	   //用于PSS多通道光功率计切换  
    
    /* display the panel and run the user interface */
    
	errChk (RunUserInterface ());
	
Error:
    /* clean up */
    DiscardPanel (panMain);
	
	/* Discard the thread local variable */
	CmtDiscardThreadLocalVar (ghThreadLocalVar);
	
	CmtDiscardLock(ThreadLock);
	CmtDiscardLock(ThreadLock_ModuleSN);       
	CmtDiscardLock(ThreadLock_SuperCMD); 
}

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK panelCB (int panel, int event, void *callbackData,
        int eventData1, int eventData2)
{
    if (event == EVENT_CLOSE)
	{
		if (gFlag)
		{
			MessagePopup ("提示", "请先终止测试，然后再退出程序");
			return -1;
		}
		
		InstallPopup (panPopu);
	}
    return 0;
}

int CVICALLBACK QuitCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			quittest();
			QuitUserInterface (0);
			break;
		}
	return 0;
}

int CVICALLBACK On_Run (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int channel;
	int error;
	char ErrorInfo[100];
	switch (event)
	{
		case EVENT_COMMIT:

			gFlag = TRUE;
		
			//修改界面控件属性
			SetCtrlVal (panMain, PAN_MAIN_BUT_STOP, 0); 
			SetCtrlAttribute (panMain, PAN_MAIN_BUT_RUN, ATTR_DIMMED, 1); 
			SetMenuBarAttribute (panMenu, 0, ATTR_DIMMED, 1);
			
			for (channel=0; channel<CHANNEL_MAX; channel++)
			{
				SetCtrlAttribute (panMain, gCtrl_BIN[channel], ATTR_DIMMED, 1);  
				my_struInstrument[channel].ChannelStart = FALSE;
				my_struInstrument[channel].ChannelEnd   = FALSE;
				if (my_struInstrument[channel].ChannelEn)
				{
					//打开电源
					error=SET_EVB_SHDN(channel, 1);
					if(error<0)
					{
						sprintf(ErrorInfo,"Channel%d Power On Fail",channel);
					 	MessagePopup ("error", ErrorInfo); 
					}
				}
			}
			
			error = CmtNewThreadPool(CHANNEL_MAX, &ThreadHandle);
			if (error<0)	
			{
				CmtGetErrorMessage (error, szErrorMsg);
				MessagePopup ("Error Starting Thread Function", szErrorMsg);
			}
			
			break;
	}
	return 0;
}

int CVICALLBACK On_Stop (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int channel;
	switch (event)
	{
		case EVENT_COMMIT:

			gFlag = FALSE;
			
			CleanUpThreads ();

			//修改界面控件属性
			SetCtrlVal (panMain, PAN_MAIN_BUT_RUN, 0);
			SetCtrlAttribute (panMain, PAN_MAIN_BUT_RUN, ATTR_DIMMED, 0); 
			SetMenuBarAttribute (panMenu, 0, ATTR_DIMMED, 0); 
			
			for (channel=0; channel<CHANNEL_MAX; channel++)
			{
				SetCtrlAttribute (panMain, gCtrl_BIN[channel], ATTR_DIMMED, !my_struInstrument[channel].ChannelEn);  
			}
			
			break;
	}
	return 0;
}

void CVICALLBACK On_Initial (int menuBar, int menuItem, void *callbackData,
		int panel)
{
 	int error;
	int channel;
	char Info[256];
	
	channel = 0; 
	error = Initial (panInit, panMain, &channel, 1);
	if(error<0) 
	{
		SetCtrlAttribute (panMain, PAN_MAIN_BUT_RUN, ATTR_DIMMED, 1);
	}
	else		
	{
		SetCtrlAttribute (panMain, PAN_MAIN_BUT_RUN, ATTR_DIMMED, 0);
	}  
}

int CVICALLBACK On_Config_OK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
int  status, dimmed;
char tempStr[50], testitem[50]; 
char str0[10], str1[10], str2[5];
int  channel;
BOOL Status;

int error;

	switch (event)
		{
		case EVENT_COMMIT:
		
			//校准标志位复位
			my_struTxCal.TxCal_Flag = FALSE;
			sRxCal_Lock.RxCal_Flag = FALSE;
			sRxCal_Free.RxCal_Flag = FALSE;
			
			GetCtrlIndex (panel, PAN_CONF_RING_PN, &my_struConfig.PNIndex);
			if(my_struConfig.PNIndex>=0)
			{
				GetLabelFromIndex (panel, PAN_CONF_RING_PN, my_struConfig.PNIndex, my_struConfig.PN);  
			}
			else
			{
				strcpy (my_struConfig.PN, "");
			}
			
			
			//更新产品版本 
			GetCtrlIndex (panel, PAN_CONF_RING_BOM, &my_struConfig.VerIndex);
			if(my_struConfig.VerIndex>=0)
			{
				GetLabelFromIndex (panel, PAN_CONF_RING_BOM, my_struConfig.VerIndex, my_struConfig.Ver);
			}
			else
			{
				strcpy (my_struConfig.Ver, "");
			}

			//更新产品EEPROM版本
			GetCtrlIndex (panel, PAN_CONF_RING_EED, &my_struConfig.Ver_E2_Index); 
			if(my_struConfig.Ver_E2_Index>=0)
			{
				GetLabelFromIndex (panel, PAN_CONF_RING_EED, my_struConfig.Ver_E2_Index, my_struConfig.Ver_E2);
			}
			else
			{
				strcpy (my_struConfig.Ver_E2, "");
			}
			
			//function
//			GetCtrlVal (panel, PAN_CONF_RAD_BUT_TUN_10G, 	&my_struConfig.DT_Tun_10G);
//			GetCtrlVal (panel, PAN_CONF_RAD_BUT_TEST_10G, 	&my_struConfig.DT_Test_10G);
//			GetCtrlVal (panel, PAN_CONF_RAD_BUT_QATEST_10G, &my_struConfig.DT_QATest_10G);
			// DWDM
			GetCtrlVal (panel, PAN_CONF_RAD_BUT_TUN_DWDM, 	&my_struConfig.DT_Tun_DWDM);
			GetCtrlVal (panel, PAN_CONF_RAD_BUT_TEST_DWDM, 	&my_struConfig.DT_Test_DWDM);

			GetCtrlVal (panel, PAN_CONF_RAD_LOW, &my_struConfig.Temper_Low); 
			GetCtrlVal (panel, PAN_CONF_RAD_ROOM, &my_struConfig.Temper_Room); 
			GetCtrlVal (panel, PAN_CONF_RAD_HIGH, &my_struConfig.Temper_High); 
			
			//item
			GetCtrlVal (panel, PAN_CONF_CHE_BOX_T_AOP, &my_struConfig.Sel_T_AOP); 
			GetCtrlVal (panel, PAN_CONF_CHE_BOX_T_ER, &my_struConfig.Sel_T_ER);
			GetCtrlVal (panel, PAN_CONF_CHE_BOX_T_DIS, &my_struConfig.Sel_T_TxDis); 
			GetCtrlVal (panel, PAN_CONF_CHE_BOX_T_DIS_SOFT, &my_struConfig.Sel_T_TxDis_soft); 
			GetCtrlVal (panel, PAN_CONF_CHE_BOX_T_EYE, &my_struConfig.Sel_T_Eye);
			GetCtrlVal (panel, PAN_CONF_CHE_BOX_T_MASK, &my_struConfig.Sel_T_Mask); 
			GetCtrlVal (panel, PAN_CONF_CHE_BOX_T_SPECTRUM, &my_struConfig.Sel_T_Spectrum); 
			GetCtrlVal (panel, PAN_CONF_CHE_BOX_R_SENS, &my_struConfig.Sel_R_Sens); 
			GetCtrlVal (panel, PAN_CONF_CHE_BOX_R_SENS_REAL, &my_struConfig.Sel_R_Sens_Real);
			GetCtrlVal (panel, PAN_CONF_CHE_BOX_R_OVER, &my_struConfig.Sel_R_Over); 
			GetCtrlVal (panel, PAN_CONF_CHE_BOX_R_SDHYS, &my_struConfig.Sel_R_SDHys); 
			GetCtrlVal (panel, PAN_CONF_CHE_BOX_CALI, &my_struConfig.Sel_Calibrate);
			GetCtrlVal (panel, PAN_CONF_CHE_BOX_CALI_TEST, &my_struConfig.Sel_Calibrate_Test);
			GetCtrlVal (panel, PAN_CONF_CHE_BOX_EE_PROTECT, &my_struConfig.Sel_EE_P); 
			GetCtrlVal (panel, PAN_CONF_CHE_BOX_T_CROSS, &my_struConfig.Sel_T_Cross);
			
			/***获取PN类型**Eric.Yao**20141023***/
			status = DB_GET_PN_TYPE(tempStr);
			if (status)
			{										  
				if (strcmp (tempStr, "XGSPON_OLT") ==0) 
				{
					my_struConfig.PN_TYPE=PN_TYPE_XGSPON_OLT;	
				}   
				else 
				{
					MessagePopup ("提示", "数据库AutoDT_Spec_Tracking中定义的PN_TYPE有错误！"); 
					return -1;
				} 
			}
			
			//读取配置	 
			Status = DB_GET_ConfigInfo();
			if(!Status)
			{
				return -1;
			}
			
			error = DB_Read_Spec_TxCalThreshold();
			if(error) 
			{
					return -1;   
			}  
			
			Status = DB_GET_DDMI_Unit(); 
			if(!Status) 
			{
				return -1;
		    }
			
			//获取光谱仪参数
			if (my_struConfig.Sel_T_Spectrum)
			{
				Status = DB_GET_Spectrum();
				if(!Status) return -1;
			}	  
			
			GetCtrlAttribute (panel, PAN_CONF_RING_EED, ATTR_DIMMED, &dimmed);
			if (dimmed==0)	//需要读取EED信息
			{
				if (!DB_GET_EEPROM_Ex()) 
				{
					return -1;   
				}
			}
			
			if(!my_struConfig.Sel_Calibrate) 
			{
				if (!ConfirmPopup ("提示", "请确认不需要选择校准功能？"))
				{
					return -1;
				}
			}
			
			if(!my_struConfig.Sel_Calibrate_Test) 
			{
				if (!ConfirmPopup ("提示", "请确认不需要选择校准测试功能？"))
				{
					return -1;
				}
			}
			
			//RSSI模式由数据库的配置决定 以及EX1中查询出温度的slope和offset
//			if (DB_READ_Spec_Tracking_Ex(&my_struConfig.RSSI_TYPE)<0)    //读取Spec_Tracking_Ex和 Spec_Tracking_Ex1里面的配置
//			{
//				return -1; 		  
//			}
			
			//读取表for Calibrate  
			if(my_struConfig.Sel_Calibrate_Test || my_struConfig.Sel_Calibrate)
			{
				status = DB_GET_Config_Monitor_Info ();
				if(status)
				{
					return -1;
				}
			
				//只读取标志位=2的校准点，
				if (my_struConfig.Sel_Calibrate)
				{
					status = DB_READ_Spec_Monitor_Ex(1);
					if (status)
					{
						return -1;
					}
				}
				else
				{
					status = DB_READ_Spec_Monitor_Ex(2);
					if (status) 
					{
						return -1; 	
					}
				}
			}

			if (!DB_GET_BarCode()) 
			{
				return -1;
			}
			
			//读取Firmware版本配置
			if ((my_struConfig.ChiefChip==CHIEFCHIP_ADUC7020) || (CHIEFCHIP_ADUC7023 == my_struConfig.ChiefChip))	 /***增加7023读取固件**Eric.Yao**20130909***/
			{
				if (!DB_GET_Firmware_Ver())
				{
					return -1;
				}
			} 			  
			
			RemovePopup (0);
			
			SetCtrlVal (panMain, PAN_MAIN_STR_PN, my_struConfig.PN); 
			SetCtrlVal (panMain, PAN_MAIN_STR_BOM, my_struConfig.Ver);
			
			SetConfig();  
			GetControl();  
			
			if (my_struConfig.DT_Tun_10G)  
			{
				strcpy (testitem, "调试10G"); 
			}
			else if (my_struConfig.DT_Test_10G)     
			{
				if (my_struConfig.Temper_Low)	
				{
					strcpy (testitem, "低温测试10G"); 
				}
				else if (my_struConfig.Temper_High)
				{
					strcpy (testitem, "高温测试10G"); 
				}
				else							
				{
					strcpy (testitem, "测试10G"); 
				}
			}
			else  if(my_struConfig.DT_QATest_10G)							   		
			{
				if (my_struConfig.Temper_Low)	
				{
					strcpy (testitem, "低温QA测试10G"); 
				}
				else if (my_struConfig.Temper_High)
				{
					strcpy (testitem, "高温QA测试10G"); 
				}
				else							
				{
					strcpy (testitem, "QA测试10G");
				}
			}
			else if (my_struConfig.DT_Test_1G)     
			{
				if (my_struConfig.Temper_Low)	
				{
					strcpy (testitem, "低温测试1G"); 
				}
				else if (my_struConfig.Temper_High)
				{
					strcpy (testitem, "高温测试1G"); 
				}
				else							
				{
					strcpy (testitem, "测试1G"); 
				}
			}
			else  if(my_struConfig.DT_QATest_1G)							   		
			{
				if (my_struConfig.Temper_Low)	
				{
					strcpy (testitem, "低温QA测试1G"); 
				}
				else if (my_struConfig.Temper_High)
				{
					strcpy (testitem, "高温QA测试1G"); 
				}
				else							
				{
					strcpy (testitem, "QA测试1G");
				}
			}
			else
			{
				strcpy (testitem, "未知工序");   
			}
			
			SetCtrlVal (panMain, PAN_MAIN_TEXTMSG_INFO, testitem);  
			
			SetCtrlAttribute (panMain, PAN_MAIN_TEXTMSG_INFO, ATTR_VISIBLE, 1); 
			if (my_struConfig.DT_QATest_10G)
			{
				SetCtrlAttribute (panMain, PAN_MAIN_TEXTMSG_INFO, ATTR_TEXT_COLOR, VAL_RED); 
			}
			else
			{
				SetCtrlAttribute (panMain, PAN_MAIN_TEXTMSG_INFO, ATTR_TEXT_COLOR, VAL_BLACK); 
			}
			
			   
			//添加初始化系统代码   
			channel = 0;
			error = Initial(panInit, panMain, &channel, 1);
			if(error<0) 
			{
				SetCtrlAttribute (panMain, PAN_MAIN_BUT_RUN, ATTR_DIMMED, 1);
			}
			else		
			{
				SetCtrlAttribute (panMain, PAN_MAIN_BUT_RUN, ATTR_DIMMED, 0);
			}  
			SetActiveCtrl (panMain, PAN_MAIN_STR_SN);
				
			break;
		}
	return 0;
}


void CVICALLBACK On_Config (int menuBar, int menuItem, void *callbackData,
		int panel)
{
		int count;
	
	InstallPopup (panConf);  
	
	MyDLLGETPartNumber (panConf, PAN_CONF_RING_PN);
	
	//读取配置文件
	GetConfig(); 
	//根据文件存放的信息设置界面上的显示信息
	//避免PNIndex>PAN_CONF_RING_PN数目的bug
	GetNumListItems (panConf, PAN_CONF_RING_PN, &count);
	if (my_struConfig.PNIndex>(count-1)) 
	{
		my_struConfig.PNIndex=0;
	}
	
	SetCtrlIndex (panConf, PAN_CONF_RING_PN, my_struConfig.PNIndex); 
	// DWDM调试测试
	SetCtrlVal (panConf, PAN_CONF_RAD_BUT_TUN_DWDM, my_struConfig.DT_Tun_DWDM);
	SetCtrlVal (panConf, PAN_CONF_RAD_BUT_TEST_DWDM, my_struConfig.DT_Test_DWDM);
	// 测试温度
	SetCtrlVal (panConf, PAN_CONF_RAD_LOW, my_struConfig.Temper_Low); 
	SetCtrlVal (panConf, PAN_CONF_RAD_ROOM, my_struConfig.Temper_Room); 
	SetCtrlVal (panConf, PAN_CONF_RAD_HIGH, my_struConfig.Temper_High); 

	Update_Config_Ver(panConf, TRUE);
}

int CVICALLBACK On_Login_OK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int status, error;
	int channel;
	int progresshandle;
	char Info[256]; 
	int i;
	struct struInstrument localInst;
	
	switch (event)
		{
		case EVENT_COMMIT:
			
			status = MyDLL_License_Check(my_struLicense.username, my_struLicense.password, my_struLicense.power);
			if (!status) 
			{
				return -1;
			}
				
			if (License_Flag_panADV==showphID) 
			{
				//显示高级功能配置界面
				if (stricmp (my_struLicense.power, "admin")!=0) 		
				{
					MessagePopup("提示","输入的账号没有使用此功能的权限！");
					return -1;
				} 
				
				DisplayPanel (panAdva);
				GetConfig(); 
			
				SetCtrlVal (panAdva, PAN_ADVA_CHE_NPI, my_struConfig.isNPI_flag);  
			}
			else if (License_Flag_panCALI==showphID) 
			{
				//显示校准配置界面
				if (stricmp (my_struLicense.power, "admin")!=0) 		
				{
					MessagePopup("提示","输入的账号没有使用此功能的权限！");
					return -1;
				} 
				
				DisplayPanel (phCaliLimit); 
				
				//显示校准文件相关信息
			 	GetCaliConfigFile ();
				
				//创建进度对话框
				progresshandle = CreateProgressDialog ("加载校准门限数据", "数据加载进度", 1, VAL_NO_INNER_MARKERS, "");
				
				for (channel=0; channel<CHANNEL_MAX; channel++)
				{
					UpdateProgressDialog (progresshandle, (int)(100.*channel/CHANNEL_MAX), 0); 
					
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (2, channel*3+1), "发端光路");
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (3, channel*3+1), 0.); 
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (4, channel*3+1), my_struTxCal.power_min[channel]); 
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (5, channel*3+1), my_struTxCal.power_max[channel]); 
				
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (2, channel*3+2), "收端光路锁定模式");  
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (3, channel*3+2), sRxCal_Lock.power_in[channel]);  
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (4, channel*3+2), sRxCal_Lock.power_min[channel]); 
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (5, channel*3+2), sRxCal_Lock.power_max[channel]); 
					
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (2, channel*3+3), "收端光路衰减模式");  
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (3, channel*3+3), sRxCal_Free.power_in[channel]);  
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (4, channel*3+3), sRxCal_Free.power_min[channel]); 
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (5, channel*3+3), sRxCal_Free.power_max[channel]); 
				}
				
				DiscardProgressDialog (progresshandle); 
			}
			else if(License_Flag_panInst ==showphID)
			{			
				InstallPopup (panInst);
				
				GetConfig_Inst();
				
				for (i=0; i<CHANNEL_MAX; i++)
				{
					localInst = my_struInstrument[i]; 
					InsertTree (panInst, PAN_INST_TREE, &localInst, 1); 
				}
			}
			else if (License_Flag_USER==showphID)
			{
				strcpy(g_Operator, my_struLicense.username);
				SetCtrlVal (panMain, PAN_MAIN_STR_OPERATOR, g_Operator);
			}
			else if (License_Flag_INIT == showphID)
			{
				showphID = License_Flag_panMain; 
				
				DisplayPanel (panMain);
				strcpy(g_Operator, my_struLicense.username);
				SetCtrlVal (panMain, PAN_MAIN_STR_OPERATOR, g_Operator);
				
				for (channel=0; channel<CHANNEL_MAX; channel++)
				{
					my_struTxCal.flag[channel]=TRUE;    //开机可以不用校准 
					sRxCal_Lock.flag[channel] = FALSE;  
					sRxCal_Free.flag[channel] = FALSE;
				}
				
				//获取系统配置
				error = Initial (panInit, panMain, &channel, 0);  
				if(error<0) 
				{
					SetCtrlAttribute (panMain, PAN_MAIN_BUT_RUN, ATTR_DIMMED, 1);
				}
			 	else		
			 	{
					SetCtrlAttribute (panMain, PAN_MAIN_BUT_RUN, ATTR_DIMMED, 0);
				}  
			}
			else if(License_Flag_panCtr == showphID)
			{
			 	//显示控制命令配置界面
				if (stricmp (my_struLicense.power, "admin")!=0) 		
				{
					MessagePopup("提示","输入的账号没有使用此功能的权限！");
					return -1;
				} 
				
				DisplayPanel (panCtr);
				GetControl();  
				Set_Control_Table();  
				
			}
			else
			{
				MessagePopup("提示","程序界面显示逻辑异常    "); 
				return -1;	
			}
			
			break;
		}
	return 0;
}

int CVICALLBACK On_Login_Quit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			
			if (!MyDLL_License_Close()) 
			{
				return -1;
			}
			
			if (License_Flag_panMain==showphID)  
			{	 //当前界面是主程序界面
				panelCB (panMain,EVENT_CLOSE, 0,0,0); 
			}
			else if (License_Flag_INIT == showphID)
			{
				//当前界面还没有打开主界面，直接退出
				QuitUserInterface (0); 
			}
			
			break;
		}
	return 0;
}

int CVICALLBACK On_P (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int channel;
	int val;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal (panel, control, &val); 
			
			 switch (control)
			 {
				 case PAN_MAIN_BIN_CHAN0:
					 channel=0;
					 break;
				 case PAN_MAIN_BIN_CHAN1:
					 channel=1; 
					 break;
				 case PAN_MAIN_BIN_CHAN2:
					 channel=2; 
					 break;
				 case PAN_MAIN_BIN_CHAN3:
					 channel=3; 
					 break;
				 case PAN_MAIN_BIN_CHAN4:
					 channel=4; 
					 break;
				 case PAN_MAIN_BIN_CHAN5:
					 channel=5; 
					 break;
				 case PAN_MAIN_BIN_CHAN6:
					 channel=6; 
					 break;
				 case PAN_MAIN_BIN_CHAN7:
					 channel=7; 
					 break;
					 
				 default:
					 MessagePopup ("提示", "此通道无效");
					 break;
			 }
			
			 SET_EVB_SHDN (channel,val);
			 
			break;
	}
	return 0;
}	 

int CVICALLBACK On_Login_Password (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int myKey;
	switch (event)
		{
		case EVENT_KEYPRESS:
			myKey = GetKeyPressEventVirtualKey (eventData2);
   			if (myKey==1280)//"Enter"
			{
				On_Login_OK (panel, control, EVENT_COMMIT, 0, 0, 0);
			}
			break;
		}
	return 0;
}

void CVICALLBACK On_His (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	char filename[MAX_PATHNAME_LEN], tempstr[1024];	
	
	GetProjectDir (filename);
	strcat (filename, "\\版本历史.txt");
	
    sprintf (tempstr, "Notepad.exe %s", filename);
    LaunchExecutable (tempstr);
}

void CVICALLBACK On_About (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	DisplayPanel (panAbout);
}

int  CVICALLBACK On_About_EXIT(int panel, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK:

			HidePanel (panel);  
			
			break;
	}
	return 0;
}

int CVICALLBACK On_Order (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char buf[256];
	char tempstr[256], BatchArray[8][3]={"PT","CT","GT","RT","WT","DT","ST","VT"}, WorkOrder[30]; 
	int  BatchNum=8, i, error;
    char str0[256],str1[256],str2[256]; 
	switch (event)
	{
		case EVENT_COMMIT:
			memset(str0,0,sizeof(str0));
			memset(str1,0,sizeof(str1)); 
			memset(str2,0,sizeof(str2)); 
			GetCtrlVal (panel, PAN_ORDER_COMBO_ORDER, buf);
			Scan(buf, "%s>%s[xt59]%s[xt59]%s", str0,str1,str2);   
			strcpy (WorkOrder, str0);  
			error = DB_Read_Order (WorkOrder);			//MySQL切换至Oracle	wenyao.xi 2015-11-27   
			if (error) 	{Insert_Info(panMain, PAN_MAIN_TEXTBOX_CHAN0, "查询OA数据库异常");}  
			else    	{Insert_Info(panMain, PAN_MAIN_TEXTBOX_CHAN0, "查询OA数据库成功");} 
	
			//设置批次
			SetCtrlVal (panMain, PAN_MAIN_STR_BATCH, WorkOrder); 
			
			strcpy(my_struConfig.batch, WorkOrder);
	
			RemovePopup (0); 
	}
	return 0;
}

int CVICALLBACK On_Advance_OK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:   
			
			HidePanel (panAdva);
			
			break;
	}
	return 0;
}

int CVICALLBACK On_Advance_App (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:	   
		
			//保存到配置文件	 
			GetCtrlVal (panel, PAN_ADVA_CHE_NPI, &my_struConfig.isNPI_flag); 
				
			SetConfig(); 
			SetConfig_Inst();
			
			break;
	}
	return 0;
}

int CVICALLBACK On_Advance_Cancle (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel (panAdva); 
			break;
	}
	return 0;
}

void CVICALLBACK On_Advance (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	if (!MyDLL_License_Load()) return ;  
	
	showphID = License_Flag_panADV;  
}

void CVICALLBACK On_User (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	MyDLL_License_Load ();
	showphID = License_Flag_USER;  
}

void CVICALLBACK On_Inst (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	MyDLL_License_Load ();
	
	showphID = License_Flag_panInst;
}

int CVICALLBACK On_Timer (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int channel;
	int error;
	
	BOOL bMOD0;
	char Info[500] = "";
	
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			if (gFlag)
			{
				for (channel = 0; channel < CHANNEL_MAX; channel++)
				{
					if (my_struInstrument[channel].ChannelEn)
					{
						if (!my_struInstrument[channel].ChannelStart)
						{	 
							error = checktestbed(channel, &bMOD0);
							if (error) 
							{
								strcpy (Info, "检测测试板是否有模块出现异常");
								Insert_Info(panMain, gCtrl_BOX[channel], Info);  
								continue;
							}
							else if (!bMOD0)	  	
							{
								ResetTextBox (panMain, gCtrl_BOX[channel], "");      
								strcpy (Info, "等待被测模块接入测试系统...");
								Insert_Info(panMain, gCtrl_BOX[channel], Info);   
								continue;
							}
							else
							{

								if (my_struConfig.DT_Tun_DWDM)
								{   // 调试DWDM
									// 为tables显示数据添加一行
									InsertTableRows(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, 1, 1, VAL_CELL_NUMERIC);
									error = CmtScheduleThreadPoolFunction (ThreadHandle, tun_DWDM_10G, &gIndexVal[channel], &gThreadFuncId[channel]); 
								}
								else if (my_struConfig.DT_Test_DWDM)
								{   // 测试DWDM
									InsertTableRows(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, 1, 1, VAL_CELL_NUMERIC);
									error = CmtScheduleThreadPoolFunction (ThreadHandle, test_DWDM_10G, &gIndexVal[channel], &gThreadFuncId[channel]); 
								}
								else
								{
									MessagePopup ("提示", "当前程序不支持此测试功能");
									On_Stop (panel, PAN_MAIN_BUT_STOP, EVENT_COMMIT, 0,0,0);
									error=-1;
								}
							
								if (error<0)	
								{
									CmtGetErrorMessage (error, szErrorMsg);
									MessagePopup ("Error Starting Thread Function", szErrorMsg);
								}
								
								my_struInstrument[channel].ChannelStart = TRUE;
								my_struInstrument[channel].ChannelEnd   = FALSE;
							}
						}
						else if (my_struInstrument[channel].ChannelEnd)
						{
							if (gThreadFuncId[channel] != 0) 
							{
								CmtWaitForThreadPoolFunctionCompletion (ThreadHandle, gThreadFuncId[channel], 0);
			
								CmtReleaseThreadPoolFunctionID (ThreadHandle, gThreadFuncId[channel]);
			
								gThreadFuncId[channel] = 0;	   
							}  
							
							my_struInstrument[channel].ChannelStart = FALSE;
							my_struInstrument[channel].ChannelEnd   = FALSE; 
						}
					}
				}
			}

			break;
	}
	return 0;
}

void CVICALLBACK On_Calibrate_R_Lock (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	int error;
	int channel;
	int progresshandle;
	BOOL timeOutFlag;
	
	errChk(GetCaliConfigFile ());
	
	DisplayPanel (panCaliR_Lock);
	
	progresshandle = CreateProgressDialog ("设备配置状态", "设备配置进度", 1, VAL_NO_INNER_MARKERS, "");
	SetCtrlAttribute(panCaliR_Lock,PAN_RCALL_BUT_OK_TDP,ATTR_VISIBLE ,0);
	SetCtrlAttribute(panCaliR_Lock,PAN_RCALL_BUT_OK,ATTR_VISIBLE ,1);
	//for BERT
	for (channel=0;channel<CHANNEL_MAX;channel++)
	{
		UpdateProgressDialog (progresshandle, (int)(100.*channel/CHANNEL_MAX), 0);
		
		if (my_struInstrument[channel].ChannelEn)
		{
			//for BERT	
			if(my_struConfig.DT_Test_1G || my_struConfig.DT_QATest_1G)
			{
				error = Init_BERT (channel, -1);
				if (error) 
				{
					return;  
				}
			
			}
			else
			{
				error = Init_10GBERT (channel);
				if (error) 
				{
					return;
				}
			}
			
			SetTableCellAttribute (panCaliR_Lock, PAN_RCALL_TABLE, MakePoint (1, channel+1), ATTR_CELL_DIMMED, 0);
			
			DB_get_cali (CALI_TYPE_RX_MAINLOCK, channel, &timeOutFlag);
			if (timeOutFlag)
			{
				SetTableCellVal (panCaliR_Lock, PAN_RCALL_TABLE, MakePoint (1, channel+1), 0.00);
			}
			else
			{
				sRxCal_Lock.power_in[channel] = -15;
				SetTableCellVal (panCaliR_Lock, PAN_RCALL_TABLE, MakePoint (1, channel+1), sRxCal_Lock.power_in[channel]-sRxCal_Lock.power[channel]); 
			}
			
			errChk (Set_Att_Lock_Enable (channel));   
			
			errChk(Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, sRxCal_Lock.power_in[channel]));
		}
		else
		{
			SetTableCellAttribute (panCaliR_Lock, PAN_RCALL_TABLE, MakePoint (1, channel+1), ATTR_CELL_DIMMED, 1);	
		}
	}
	
Error:
	
	DiscardProgressDialog (progresshandle);
}

void CVICALLBACK On_Calibrate_R_Free(int menuBar, int menuItem, void *callbackData,
		int panel)
{
	int error;
	int channel;
	int progresshandle;
	BOOL timeOutFlag;
	
	errChk(GetCaliConfigFile ());
	
	DisplayPanel (panCaliR_Free);
	
	progresshandle = CreateProgressDialog ("设备配置状态", "设备配置进度", 1, VAL_NO_INNER_MARKERS, "");
	SetCtrlAttribute(panCaliR_Free,PAN_RCAL_F_BUT_OK_TDP,ATTR_VISIBLE ,0);
	SetCtrlAttribute(panCaliR_Free,PAN_RCAL_F_BUT_OK,ATTR_VISIBLE ,1);
	
	//for BERT
	for (channel=0;channel<CHANNEL_MAX;channel++)
	{
		UpdateProgressDialog (progresshandle, (int)(100.*channel/CHANNEL_MAX), 0);
		
		if (my_struInstrument[channel].ChannelEn)
		{
			//for BERT	
			if(my_struConfig.DT_Test_1G || my_struConfig.DT_QATest_1G)
			{
				error = Init_BERT (channel, -1);
				if (error) 
				{
					return;  
				}
			
			}
			else
			{
				error = Init_10GBERT (channel);
				if (error) 
				{
					return;
				}
			}
			
			SetTableCellAttribute (panCaliR_Free, PAN_RCALL_TABLE, MakePoint (1, channel+1), ATTR_CELL_DIMMED, 0);
			
			DB_get_cali (CALI_TYPE_RX_MAINFREE, channel, &timeOutFlag);
			if (timeOutFlag)
			{
				SetTableCellVal (panCaliR_Free, PAN_RCALL_TABLE, MakePoint (1, channel+1), 0.00);
			}
			else
			{
				// 写死校准值-15
				sRxCal_Free.power_in[channel] = -15;
				SetTableCellVal (panCaliR_Free, PAN_RCALL_TABLE, MakePoint (1, channel+1), sRxCal_Free.power_in[channel]-sRxCal_Free.power[channel]); 		// -sRxCal_Free.power[channel]
			}
			
			errChk (Set_Att_Lock_Disable (channel));   
			
			errChk (Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN, sRxCal_Free.power_in[channel]));
		}
		else
		{
			SetTableCellAttribute (panCaliR_Free, PAN_RCALL_TABLE, MakePoint (1, channel+1), ATTR_CELL_DIMMED, 1);	
		}
	}
	
Error:
	
	DiscardProgressDialog (progresshandle);
}

void CVICALLBACK On_Calibrate_T (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	int error,i;
	int channel;
	int progresshandle;
	float val=0.0;
	
	if (!my_struConfig.Sel_T_AOP)
	{MessagePopup("提示", "没有选择发端光功率测试，可不进行发端校准！");return;} 
		
	DisplayPanel (phCaliT); 

	//清空校准结果显示
	FillTableCellRange (phCaliT, PAN_CALI_T_TABLE, MakeRect (1, 2, 24, 2), val);
	
	//创建进度对话框
	progresshandle = CreateProgressDialog ("设备配置状态", "设备配置进度", 1, VAL_NO_INNER_MARKERS, "");
	
	for (channel=0; channel<CHANNEL_MAX; channel++)
	{
		UpdateProgressDialog (progresshandle, (int)(100.*channel/CHANNEL_MAX), 0); 
	
		if (my_struInstrument[channel].ChannelEn)
		{
			SetTableCellRangeAttribute (phCaliT, PAN_CALI_T_TABLE, MakeRect (channel*3+1, 2, 3, 3), ATTR_CELL_DIMMED, 0); 
			errChk(SET_EVB_SHDN(channel, 1));
			
			//加载校准记录
			for (i=0; i<3; i++)
			{
				SetTableCellVal (phCaliT, PAN_CALI_T_TABLE, MakePoint (2, channel*3+i+1), my_struTxCal.power_st[channel]); 
				SetTableCellVal (phCaliT, PAN_CALI_T_TABLE, MakePoint (3, channel*3+i+1), my_struTxCal.power_st[channel]-my_struTxCal.power[channel]); 
			}
		}
		else
		{
			SetTableCellRangeAttribute (phCaliT, PAN_CALI_T_TABLE, MakeRect (channel*3+1, 2, 3, 3), ATTR_CELL_DIMMED, 1);
		}  
	}	 
	
Error:
	
	DiscardProgressDialog (progresshandle);
}

void CVICALLBACK On_Init (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	int error = 0;
	int channel;
	char Info[256];
	
	channel = 0; 
	error = Initial (panInit, panMain, &channel, 1);
	if(error<0) 
	{
		SetCtrlAttribute (panMain, PAN_MAIN_BUT_RUN, ATTR_DIMMED, 1);
	}
	else		
	{
		SetCtrlAttribute (panMain, PAN_MAIN_BUT_RUN, ATTR_DIMMED, 0);
	}  
}

int CVICALLBACK On_Inst_AddTree (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	struct struInstrument localInst;
	int index;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal (panel, PAN_INST_NUM_CHANNEL, &localInst.Channel);
			GetCtrlVal (panel, PAN_INST_BIN_CHAN_EN, &localInst.ChannelEn);
			
			//光功率计
			GetCtrlVal (panel, PAN_INST_RING_PM_TYPE, &localInst.Power_TYPE);
			GetCtrlVal (panel, PAN_INST_NUM_PM, &localInst.PMSII);
			GetCtrlVal (panel, PAN_INST_STR_SN_PM, localInst.SN_PM); 

			//衰减器
			GetCtrlVal (panel, PAN_INST_RING_ATT_MAI_TYPE, &localInst.ATT_TYPE_MAIN); 
			GetCtrlVal (panel, PAN_INST_NUM_ATT,&localInst.ATT_MAIN_COM);       
			GetCtrlIndex (panel, PAN_INST_RING_GVPM_MAI, &index); 
			if(index>=0) {GetLabelFromIndex (panel, PAN_INST_RING_GVPM_MAI, index, localInst.SN_ATT_MAIN);}  
			else         {strcpy (localInst.SN_ATT_MAIN, "");}
			
			//参考光路衰减器
			GetCtrlVal (panel, PAN_INST_RING_ATT_MAI_TYPE_REF, &localInst.ATT_REF_TYPE); 
			GetCtrlVal (panel, PAN_INST_NUM_ATT_REF,&localInst.ATT_REF_COM);       
			GetCtrlIndex (panel, PAN_INST_RING_GVPM_REF, &index); 
			if(index>=0) {GetLabelFromIndex (panel, PAN_INST_RING_GVPM_REF, index, localInst.SN_ATT_REF);}  
			else         {strcpy (localInst.SN_ATT_REF, "");}

			//EVB5
			GetCtrlVal (panel, PAN_INST_RING_SEVB_TYPE, &localInst.SEVB_TYPE); 
			GetCtrlIndex (panel, PAN_INST_RING_SEVB, &index); 
			if(index>=0) {GetLabelFromIndex (panel, PAN_INST_RING_SEVB, index, localInst.SEVB);}  
			else         {strcpy (localInst.SEVB, "");}

			//1G BERT
			GetCtrlVal (panel, PAN_INST_RING_BERT_TYPE, &localInst.BERT_TYPE); 
			GetCtrlIndex (panel, PAN_INST_RING_BERT, &index); 
			if(index>=0) {GetLabelFromIndex (panel, PAN_INST_RING_BERT, index, localInst.SN_BERT);}  
			else         {strcpy (localInst.SN_BERT, "");}
			
			//10G BERT
			GetCtrlVal (panel, PAN_INST_RING_10GBERT_TYPE, &localInst.BERT_TYPE_10G); 
			GetCtrlIndex (panel, PAN_INST_RING_GBERT10G, &index); 
			if(index>=0) {GetLabelFromIndex (panel, PAN_INST_RING_GBERT10G, index, localInst.SN_10GBERT);}  
			else         {strcpy (localInst.SN_10GBERT, "");}
			
			//EVB Light
			GetCtrlVal (panel, PAN_INST_RING_SEVB_LIGHT_TYPE, &localInst.EVB_Light_TYPE); 
			GetCtrlIndex (panel, PAN_INST_RING_EVB_LIGHT, &index); 
			if(index>=0) {GetLabelFromIndex (panel, PAN_INST_RING_EVB_LIGHT, index, localInst.EVB_Light);}  
			else         {strcpy (localInst.EVB_Light, "");}

			//DCA
			GetCtrlVal (panel, PAN_INST_RING_DCA_TYPE, &localInst.DCA_TYPE);
			GetCtrlVal (panel, PAN_INST_STR_DCA, localInst.DCA_Addr); 
			GetCtrlVal (panel, PAN_INST_STR_SN_DCA, localInst.SN_DCA); 
			
			//SPECTRUM
			GetCtrlVal (panel, PAN_INST_RING_SPECTRUM_TYPE, &localInst.SPECTRUM_TYPE);
			GetCtrlVal (panel, PAN_INST_STR_SPECTRUM, localInst.Spectrum_Addr); 
			GetCtrlVal (panel, PAN_INST_STR_SN_SPECTRUM, localInst.SN_Spectrum); 
			
			GetCtrlVal (panel, PAN_INST_STR_FIBER, localInst.Fiber); 
			MyDLLCheckSN(localInst.Fiber);
			
			//光开关
			GetCtrlVal (panel, PAN_INST_RING_SW_TYPE, &localInst.SW_TYPE); 
			GetCtrlVal (panel, PAN_INST_NUM_SW, &localInst.SW); 
			GetCtrlVal (panel, PAN_INST_NUM_SW_CHAN, &localInst.SW_CHAN);   
			GetCtrlIndex (panel, PAN_INST_RING_FSW, &index); 
			if(index>=0) {GetLabelFromIndex (panel, PAN_INST_RING_FSW, index, localInst.SW_SN);}  
			else         {strcpy (localInst.SW_SN, "");}   
			
			//时钟切换板
			GetCtrlVal (panel, PAN_INST_RING_CLOCK_TYPE, &localInst.CLOCK_TYPE); 
			GetCtrlIndex (panel, PAN_INST_RING_CLOCK, &index); 
			if(index>=0) {GetLabelFromIndex (panel, PAN_INST_RING_CLOCK, index, localInst.CLOCK);}  
			else         {strcpy (localInst.CLOCK, "");}
			GetCtrlVal (panel, PAN_INST_NUM_CLOCK_CHAN, &localInst.CLOCK_CHAN);
			
			//添加到设备列表
			InsertTree (panel, PAN_INST_TREE, &localInst, 0);
			
			break;
	}
	return 0;
}

int CVICALLBACK On_Inst_Tree (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char label1[256];
	int  ParentIndex, channel, ImageIndex;
	
	switch (event)
	{
		case EVENT_COMMIT:
			;
			break;
        case EVENT_SELECTION_CHANGE:   
			
			if (eventData1) 
			{
				GetTreeItemParent (panel, PAN_INST_TREE, eventData2, &ParentIndex);
				if (-1 == ParentIndex)
				{   //选中了父项
					GetLabelFromIndex (panel, PAN_INST_TREE, eventData2, label1); 
					GetTreeItemAttribute (panel, PAN_INST_TREE, eventData2, ATTR_IMAGE_INDEX, &ImageIndex);
					ParentIndex = eventData2;
				}
				else
				{   //选中了子项
					GetLabelFromIndex (panel, PAN_INST_TREE, ParentIndex, label1);  
					GetTreeItemAttribute (panel, PAN_INST_TREE, ParentIndex, ATTR_IMAGE_INDEX, &ImageIndex);
				}

				sscanf (label1,"通道%d",&channel);
	            SetCtrlVal (panel, PAN_INST_NUM_CHANNEL, channel);
				
				//显示设备相关信息，注意此时结构体INSTR必须要有值，所以要保证之前必须运行过函数GetConfig_Inst 
				if (TREE_IMAGE_OK==ImageIndex || TREE_IMAGE_WARING==ImageIndex)
				{
					SetCtrlVal (panel, PAN_INST_BIN_CHAN_EN, 1);	
				}
				else
				{
					SetCtrlVal (panel, PAN_INST_BIN_CHAN_EN, 0);
				}
				
				//显示光功率计信息
				SetCtrlVal (panel, PAN_INST_RING_PM_TYPE, my_struInstrument[channel].Power_TYPE); 
				SetCtrlVal (panel, PAN_INST_NUM_PM, my_struInstrument[channel].PMSII);  
				SetCtrlVal (panel, PAN_INST_STR_SN_PM, my_struInstrument[channel].SN_PM); 

				//衰减器
				SetCtrlVal (panel, PAN_INST_RING_ATT_MAI_TYPE, my_struInstrument[channel].ATT_TYPE_MAIN);
				SetCtrlVal(panel,  PAN_INST_NUM_ATT ,my_struInstrument[channel].ATT_MAIN_COM);     
				ClearListCtrl (panel, PAN_INST_RING_GVPM_MAI); 
				InsertListItem (panel, PAN_INST_RING_GVPM_MAI, 0, my_struInstrument[channel].SN_ATT_MAIN, 0); 
				
				//SEVB
				SetCtrlVal (panel, PAN_INST_RING_SEVB_TYPE, my_struInstrument[channel].SEVB_TYPE); 
				ClearListCtrl (panel, PAN_INST_RING_SEVB); 
				InsertListItem (panel, PAN_INST_RING_SEVB, 0, my_struInstrument[channel].SEVB, 0); 

				//1G BERT
				SetCtrlVal (panel, PAN_INST_RING_BERT_TYPE, my_struInstrument[channel].BERT_TYPE); 
				ClearListCtrl (panel, PAN_INST_RING_BERT); 
				InsertListItem (panel, PAN_INST_RING_BERT, 0, my_struInstrument[channel].SN_BERT, 0); 
				
				//10G BERT
				SetCtrlVal (panel, PAN_INST_RING_GBERT10G, my_struInstrument[channel].BERT_TYPE_10G); 
				ClearListCtrl (panel, PAN_INST_RING_GBERT10G); 
				InsertListItem (panel, PAN_INST_RING_GBERT10G, 0, my_struInstrument[channel].SN_10GBERT, 0);
		
				//SEVB LIGHT
				SetCtrlVal (panel, PAN_INST_RING_SEVB_LIGHT_TYPE, my_struInstrument[channel].EVB_Light_TYPE); 
				ClearListCtrl (panel, PAN_INST_RING_EVB_LIGHT); 
				InsertListItem (panel, PAN_INST_RING_EVB_LIGHT, 0, my_struInstrument[channel].EVB_Light, 0); 
				
				//DCA
				SetCtrlVal (panel, PAN_INST_RING_DCA_TYPE, my_struInstrument[channel].DCA_TYPE); 
				SetCtrlVal (panel, PAN_INST_STR_DCA, my_struInstrument[channel].DCA_Addr); 
				SetCtrlVal (panel, PAN_INST_STR_SN_DCA, my_struInstrument[channel].SN_DCA); 
				
				//SPECTRUM
				SetCtrlVal (panel, PAN_INST_RING_SPECTRUM_TYPE, my_struInstrument[channel].SPECTRUM_TYPE); 
				SetCtrlVal (panel, PAN_INST_STR_SPECTRUM, my_struInstrument[channel].Spectrum_Addr); 
				SetCtrlVal (panel, PAN_INST_STR_SN_SPECTRUM, my_struInstrument[channel].SN_Spectrum); 

				//FIBER
				SetCtrlVal (panel, PAN_INST_STR_FIBER, my_struInstrument[channel].Fiber); 
				
				// for sw
				SetCtrlVal (panel, PAN_INST_RING_SW_TYPE, my_struInstrument[channel].SW_TYPE); 
				SetCtrlVal (panel, PAN_INST_NUM_SW, my_struInstrument[channel].SW); 
				SetCtrlVal (panel, PAN_INST_NUM_SW_CHAN, my_struInstrument[channel].SW_CHAN); 
				ClearListCtrl (panel, PAN_INST_RING_FSW);
				InsertListItem (panel, PAN_INST_RING_FSW, 0, my_struInstrument[channel].SW_SN, 0); 
				
				display_FSW (panel, my_struInstrument[channel].SW_TYPE);
				
				// for clock
				SetCtrlVal (panel, PAN_INST_RING_CLOCK_TYPE, my_struInstrument[channel].CLOCK_TYPE); 
				ClearListCtrl (panel, PAN_INST_RING_CLOCK); 
				InsertListItem (panel, PAN_INST_RING_CLOCK, 0, my_struInstrument[channel].CLOCK, 0); 
				SetCtrlVal (panel, PAN_INST_NUM_CLOCK_CHAN, my_struInstrument[channel].CLOCK_CHAN); 
			}
		
            break;
	}
	return 0;
}  


int CVICALLBACK On_Sel_FSW (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int fsw_type;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal (panel, PAN_INST_RING_SW_TYPE, &fsw_type); 
			
			display_FSW(panel, fsw_type);
			
			break;
	}
	return 0;
}

int CVICALLBACK On_Get_10GBERT_SN (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int index; 
	
	switch (event)
	{
		case EVENT_COMMIT:

			GetCtrlVal (panel, PAN_INST_RING_10GBERT_TYPE, &index);
			if (index==BERT_TYPE_GBERT10000)  
			{
				GBERT_GET_SN (panel, PAN_INST_RING_GBERT10G);
			}  
			else if (index==BERT_TYPE_SBERT10000)  
			{
				evb16_GetBoardSN (panel, PAN_INST_RING_GBERT10G);
			}	
			else
			{
				MessagePopup("Error", "Can not find this BERT type");
			}

			break;
	}
	return 0;
}

int CVICALLBACK On_Search_Clock (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			MyDLL_GET_SEVB_SN (SEVB_TYPE_SEVB27_4001, panel, PAN_INST_RING_CLOCK); 
			
			break;
	}
	return 0;
}

int CVICALLBACK On_Search_BERT (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int index; 
	
	switch (event)
	{
		case EVENT_COMMIT:

			GetCtrlVal (panel, PAN_INST_RING_BERT_TYPE, &index);
			if (index==BERT_TYPE_SBERT)  
			{
				MyDLL_GET_SEVB_SN (SEVB_TYPE_SEVB5, panel, PAN_INST_RING_BERT);             
			}  
			else if (index==BERT_TYPE_GBERT)  
			{
				GBERT_GET_SN (panel, PAN_INST_RING_BERT);                  
			}	
			else if (index==BERT_TYPE_GBERT10000)  
			{
				GBERT_GET_SN (panel, PAN_INST_RING_BERT);
			}  
			else if (index==BERT_TYPE_SBERT10000)  
			{
				evb16_GetBoardSN (panel, PAN_INST_RING_BERT);
			}	
			else
			{
				MessagePopup("Error", "Can not find this BERT type");
			}

			break;
	}
	return 0;
}

int CVICALLBACK On_Search_SEVB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int index;
	
	switch (event)
	{
		case EVENT_COMMIT:

			GetCtrlVal (panel, PAN_INST_RING_SEVB_TYPE, &index); 
			MyDLL_GET_SEVB_SN (index, panel, PAN_INST_RING_SEVB);

			break;
	}
	return 0;
}

int CVICALLBACK On_Search_ATT (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			GVPM_GET_SN (panel, PAN_INST_RING_GVPM_MAI); 
			
			break;
	}
	return 0;
}

int CVICALLBACK On_Search_ATT_REF (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			GVPM_GET_SN (panel, PAN_INST_RING_GVPM_REF); 
			
			break;
	}
	return 0;
}

int CVICALLBACK On_Search_FSW (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			MyDLL_GET_SEVB_SN (SEVB_TYPE_SEVB5, panel, PAN_INST_RING_FSW);  

			break;
	}
	return 0;
}

int CVICALLBACK On_Search_DCA (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int index; 
	char dac_add[50];
	
	switch (event)
	{
		case EVENT_COMMIT:

			GetCtrlVal (panel, PAN_INST_RING_DCA_TYPE, &index);
			GetCtrlVal (panel, PAN_INST_STR_DCA, dac_add); 
			if (index == DCA_TYPE_CSA8000) 
			{
				if (!CSA8000_Init(&inst_CSA8000, dac_add)) 
				{
					MessagePopup("提示", "Initial CSA8000 error!");
					return -1;
				}  
			
				if (!CSA8000_GET_MainSN(inst_CSA8000, my_struCSA8000.MainSN)) 
				{
					MessagePopup("提示", "读取CSA8000主机序列号错误！"); 
					return -1;
				} 
				
				SetCtrlVal (panel, PAN_INST_STR_SN_DCA, my_struCSA8000.MainSN);	 
			}
			else if (index == DCA_TYPE_Ag86100)
			{
				if (!Ag86100_Init(&inst_Ag86100, dac_add,my_struAg86100.Channel_O)) 
				{
					MessagePopup("提示", "Initial Ag86100 error!"); 
					return -1;
				} 
			
				if (!Ag86100_GET_MainSN(inst_Ag86100, my_struAg86100.MainSN)) 
				{
					MessagePopup("提示", "读取Ag86100主机序列号错误！");
					return -1;
				} 
				
				SetCtrlVal (panel, PAN_INST_STR_SN_DCA, my_struAg86100.MainSN);
			}
			else 
			{
				MessagePopup("提示", "Can not find this DCA!"); 
				return -1;
			}   

			break;
	}
	return 0;
}

int CVICALLBACK On_Inst_Check (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK On_Inst_OK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int i;
	struct struInstrument localInst;
	
	switch (event)
	{
		case EVENT_COMMIT:

			RemovePopup (0); 
			
			//更新主界面设备列表
			for (i=0; i<CHANNEL_MAX; i++)
			{
				localInst = my_struInstrument[i]; 
				InsertTree (panMain, PAN_MAIN_TREE, &localInst, 1); 
			}
			break;
	}
	return 0;
}

int CVICALLBACK On_Inst_Save (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int channel, ParentIndex, ImageIndex, index;
	char label[256],sn[256];
	union uInst
	{
	struct struInstrument sStr;
	unsigned char pStr[sizeof(struct struInstrument)];
	} uInst;

	switch (event)
	{
		case EVENT_COMMIT:
			
			//保存设备配置到存储文件
			for (channel=0; channel<CHANNEL_MAX; channel++)
			{
				//通过创建本地union变量，赋值0后，再传给测试结果存储结构体
				memset (uInst.pStr, 0, sizeof (uInst.pStr));
				my_struInstrument[channel] = uInst.sStr; 

				sprintf (label, "通道%d", channel);
				GetTreeItemFromLabel (panel, PAN_INST_TREE, VAL_ALL, 0, VAL_FIRST, VAL_NEXT_PLUS_SELF, 0, label, &ParentIndex);
				if (ParentIndex>=0)
				{
					GetTreeItemAttribute (panel, PAN_INST_TREE, ParentIndex, ATTR_IMAGE_INDEX, &ImageIndex);
					if (TREE_IMAGE_OK==ImageIndex || TREE_IMAGE_WARING==ImageIndex) 
					{
						my_struInstrument[channel].ChannelEn=1;
					}
					else
					{
						my_struInstrument[channel].ChannelEn=0; 	
					}
					my_struInstrument[channel].Channel=channel;
					
					index = ParentIndex;
					
					GetLabelFromIndex (panel, PAN_INST_TREE, ++index, label);
					sscanf (label, LAB_PM, &my_struInstrument[channel].Power_TYPE, &my_struInstrument[channel].PMSII, my_struInstrument[channel].SN_PM);
					
					GetLabelFromIndex (panel, PAN_INST_TREE, ++index, label);
					sscanf (label, LAB_ATT_MAI, &my_struInstrument[channel].ATT_TYPE_MAIN, my_struInstrument[channel].SN_ATT_MAIN,&my_struInstrument[channel].ATT_MAIN_COM);
					
					GetLabelFromIndex (panel, PAN_INST_TREE, ++index, label);
					sscanf (label, LAB_ATT_REF, &my_struInstrument[channel].ATT_REF_TYPE, my_struInstrument[channel].SN_ATT_REF,&my_struInstrument[channel].ATT_REF_COM);
				
					GetLabelFromIndex (panel, PAN_INST_TREE, ++index, label);
					sscanf (label, LAB_SEVB, &my_struInstrument[channel].SEVB_TYPE, my_struInstrument[channel].SEVB);
					
					GetLabelFromIndex (panel, PAN_INST_TREE, ++index, label);
					sscanf (label, LAB_BERT, &my_struInstrument[channel].BERT_TYPE, my_struInstrument[channel].SN_BERT);
					
					GetLabelFromIndex (panel, PAN_INST_TREE, ++index, label);
					sscanf (label, LAB_BERT_10G, &my_struInstrument[channel].BERT_TYPE_10G, my_struInstrument[channel].SN_10GBERT);
					
					GetLabelFromIndex (panel, PAN_INST_TREE, ++index, label);
					sscanf (label, LAB_SEVB_LIGHT, &my_struInstrument[channel].EVB_Light_TYPE, my_struInstrument[channel].EVB_Light);
					
					GetLabelFromIndex (panel, PAN_INST_TREE, ++index, label);
					sscanf (label, LAB_DCA, &my_struInstrument[channel].DCA_TYPE, my_struInstrument[channel].DCA_Addr, my_struInstrument[channel].SN_DCA);
					
					GetLabelFromIndex (panel, PAN_INST_TREE, ++index, label);
					sscanf (label, LAB_SPECTRUM, &my_struInstrument[channel].SPECTRUM_TYPE, my_struInstrument[channel].Spectrum_Addr, my_struInstrument[channel].SN_Spectrum);
			
					GetLabelFromIndex (panel, PAN_INST_TREE, ++index, label);
					sscanf (label, LAB_FIBER, sn);
					//如果光纤有变更，需要校准
					if (0!= stricmp(sn, my_struInstrument[channel].Fiber)) 
					{
						//换光纤，强制校准收端和发端光路
						sRxCal_Lock.flag[channel] = FALSE;  
						sRxCal_Free.flag[channel] = FALSE;
						my_struTxCal.flag[channel]=FALSE; 
					}
					strcpy (my_struInstrument[channel].Fiber, sn);
					
					//光开关
					GetLabelFromIndex (panel, PAN_INST_TREE, ++index, label);
					sscanf (label, LAB_SW, &my_struInstrument[channel].SW_TYPE, &my_struInstrument[channel].SW, &my_struInstrument[channel].SW_CHAN, my_struInstrument[channel].SW_SN);
		
					//时钟切换板
					GetLabelFromIndex (panel, PAN_INST_TREE, ++index, label);
					sscanf (label, LAB_CLOCK, &my_struInstrument[channel].CLOCK_TYPE, my_struInstrument[channel].CLOCK, &my_struInstrument[channel].CLOCK_CHAN);
				}
			}
		
			//写入配置文件
			SetConfig_Inst ();
			
			break;
	}
	return 0;
}

int CVICALLBACK On_Pup_Cancel (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			RemovePopup (0);
			
			break;
	}
	return 0;
}

int CVICALLBACK On_Pup_User (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			RemovePopup (0);  
			
			MyDLL_License_Load ();
			showphID = License_Flag_USER;  
			
			break;
	}
	return 0;
}

int CVICALLBACK On_Pup_Exit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			quittest();
			
			RemovePopup (0);
			
			QuitUserInterface (0);

			break;
	}
	return 0;
}

int CVICALLBACK On_Config_PN (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			Update_Config_Ver(panConf, TRUE);
			
			break;
	}
	return 0;
}

int CVICALLBACK On_Config_Temper (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			SetCtrlVal (panel, PAN_CONF_RAD_HIGH, 0);
			SetCtrlVal (panel, PAN_CONF_RAD_ROOM, 0);
			SetCtrlVal (panel, PAN_CONF_RAD_LOW, 0);
			SetCtrlVal (panel, control, 1);
			
			Update_Config_Ver(panConf, FALSE);

			break;
	}
	return 0;
}

int CVICALLBACK On_Config_Item (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlVal (panel, PAN_CONF_RAD_BUT_TUN_DWDM, 0);
			SetCtrlVal (panel, PAN_CONF_RAD_BUT_TEST_DWDM, 0);
			SetCtrlVal (panel, control, 1);

			Update_Config_Ver(panConf, FALSE);

			break;
	}
	return 0;
}

int CVICALLBACK On_Calibrate_R_Lock_OK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int err = 0;
	int channel;
	float temp_cal; 
		
	switch (event)
	{
		case EVENT_COMMIT:
	
			for (channel=0; channel<CHANNEL_MAX; channel++)
			{
				if (my_struInstrument[channel].ChannelEn)
				{
					//判断校准数据是否正常
					GetTableCellVal (panCaliR_Lock, PAN_RCALL_TABLE, MakePoint (1, channel+1), &temp_cal);
					
					temp_cal= sRxCal_Lock.power_in[channel] - temp_cal;
					
					if(temp_cal>sRxCal_Lock.power_max[channel] || temp_cal<sRxCal_Lock.power_min[channel])
					{
						MessagePopup("提示", "收端校准光功率超出设置范围！"); 
						return -1;
					}
			
					if(control==PAN_RCALL_BUT_OK)
					{
						//更新收端校准值
						sRxCal_Lock.power[channel] = temp_cal; 
						sRxCal_Lock.power_array[channel*4+3] = temp_cal;
						
						//update Flag
						sRxCal_Lock.flag[channel]=TRUE;
					}
					else
					{
						//更新收端校准值
						sRxCal_Lock_TDP.power[channel] = temp_cal; 
						sRxCal_Lock_TDP.power_array[channel*4+3] = temp_cal;
						
						//update Flag
						sRxCal_Lock_TDP.flag[channel]=TRUE;
					}
			
					
			
					err = Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, sRxCal_Lock.power_in[channel]+temp_cal);
					if (err) 
					{
						MessagePopup("Error", "Set Att error!");
					}
					
					err = DB_save_rxcali (channel, CALI_TYPE_RX_MAINLOCK);
					if (err)
					{
						return -1;
					}
				}
			}
			
			//将校准参数保存到文件
			err = SetCaliConfigFile ();
			if (err) 
			{
				return -1;
			}
			
			//退出校准界面
			HidePanel (panel);    
			
			break;
	}
	return 0;
}

int CVICALLBACK On_Calibrate_R_Free_OK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int err = 0;
	int channel;
	float temp_cal; 
		
	switch (event)
	{
		case EVENT_COMMIT:
	
			for (channel=0; channel<CHANNEL_MAX; channel++)
			{
				if (my_struInstrument[channel].ChannelEn)
				{
					//判断校准数据是否正常
					GetTableCellVal (panCaliR_Free, PAN_RCALL_TABLE, MakePoint (1, channel+1), &temp_cal);
					
					temp_cal= sRxCal_Free.power_in[channel] - temp_cal;
					
					if(temp_cal>sRxCal_Free.power_max[channel] || temp_cal<sRxCal_Free.power_min[channel])
					{
						MessagePopup("提示", "收端校准光功率超出设置范围！"); 
						return -1;
					}
			
					if(control==PAN_RCAL_F_BUT_OK)
					{
						
						//更新收端校准值
						sRxCal_Free.power[channel] = temp_cal; 
						sRxCal_Free.power_array[channel*4+3] = temp_cal;
			
						//update Flag
						sRxCal_Free.flag[channel]=TRUE;
					}
					else
					{
							//更新收端校准值
						sRxCal_Free_TDP.power[channel] = temp_cal; 
						sRxCal_Free_TDP.power_array[channel*4+3] = temp_cal;
			
						//update Flag
						sRxCal_Free_TDP.flag[channel]=TRUE;
					}	
			
					err = Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN, sRxCal_Free.power_in[channel]+temp_cal);
					if (err) 
					{
						MessagePopup("Error", "Set Att error!");
					}
					
					err = DB_save_rxcali (channel, CALI_TYPE_RX_MAINFREE);
					if (err)
					{
						return -1;
					}
				}
			}
			
			//将校准参数保存到文件
			err = SetCaliConfigFile ();
			if (err) 
			{
				return -1;
			}
			
			//退出校准界面
			HidePanel (panel);    
			
			break;
	}
	return 0;
}

int CVICALLBACK On_CalRx_Quit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel (panel); 
			break;
	}
	return 0;
}

int CVICALLBACK On_Advance_Quit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			HidePanel (panAdva);  
			
			break;
	}
	return 0;
}

void CVICALLBACK On_SGD (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	//清除批次
	SetCtrlVal (panMain, PAN_MAIN_STR_BATCH, ""); 
	
	if (0==stricmp (my_struConfig.PN, ""))
	{
		MessagePopup("提示", "请先选择产品型号    "); 
		return;
	}
	
	InstallPopup (panOrder); 
	
	SetCtrlVal (panOrder, PAN_ORDER_TEXTMSG, "正在查询OA系统，请稍等...    "); 
	SetWaitCursor (1); 
//	DBOA_Read_pch (my_struConfig.PN, my_struConfig.Ver, panOrder, PAN_ORDER_COMBO_ORDER);
	DB_Read_pch (my_struConfig.PN, my_struConfig.Ver, panOrder, PAN_ORDER_COMBO_ORDER);		   //MySQL切换至Oracle	wenyao.xi 2015-11-27     
	SetWaitCursor (0); 
	SetCtrlVal (panOrder, PAN_ORDER_TEXTMSG, "查询OA系统结束    ");  
}

void CVICALLBACK On_Quit (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	panelCB (panMain,EVENT_CLOSE, 0,0,0);
}

int CVICALLBACK On_Get_EVB_Light_SN (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int EVB_Light_TYPE = -1;
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal (panel, PAN_INST_RING_SEVB_LIGHT_TYPE, &EVB_Light_TYPE);    
			MyDLL_GET_SEVB_SN (EVB_Light_TYPE, panel, PAN_INST_RING_EVB_LIGHT); 

			break;
	}
	return 0;
}

int CVICALLBACK On_Sel_SEVB_Light (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{		
	int SPECTRUM_TYPE = -1;
	
	switch (event)
	{
		case EVENT_COMMIT:
							   
			GetCtrlVal (panel, PAN_INST_RING_SPECTRUM_TYPE, &SPECTRUM_TYPE); 
			
			if (SPECTRUM_TYPE == SPECTRUM_TYPE_NONE)
			{							 
				SetCtrlAttribute (panel, PAN_INST_STR_SPECTRUM, ATTR_VISIBLE, 0);
				SetCtrlAttribute (panel, PAN_INST_STR_SN_SPECTRUM, ATTR_VISIBLE, 0);  
			}
			else if (SPECTRUM_TYPE == SPECTRUM_TYPE_AQ633X)
			{
				SetCtrlAttribute (panel, PAN_INST_STR_SPECTRUM, ATTR_VISIBLE, 1);
				SetCtrlAttribute (panel, PAN_INST_STR_SN_SPECTRUM, ATTR_VISIBLE, 1);  
			} 
			else if (SPECTRUM_TYPE == SPECTRUM_TYPE_AG8614X) 
			{
				SetCtrlAttribute (panel, PAN_INST_STR_SPECTRUM, ATTR_VISIBLE, 1);
				SetCtrlAttribute (panel, PAN_INST_STR_SN_SPECTRUM, ATTR_VISIBLE, 1);  
			}
			else 
			{
				MessagePopup("Error", "Can not find this spectrum type");
			}

			break;
	}
	return 0;
}

int CVICALLBACK tun_DWDM_10G (void *pvFuncData)
{
	int channel;
	struct struTestData data;
	struct struProcessLOG prolog;
	int	timestart, cnt;
	char Info[500];
	BOOL SaveData=FALSE, bOK=TRUE, bMOD0; 
	int error;
	int  SensTime; 
	char ERInfo[50] = ""; 
	
	BOOL timeOutFlag;
	int  TunAopErAlternatelyFlag=0;
	BOOL Status;
	
	int  count;

	/* 	Get the index corresponding to the thread */
	channel = *(int *)pvFuncData;
	m_adjustSpeed = FALSE;
	/*	Get the address of the thread local variable */
	CmtGetThreadLocalVar (ghThreadLocalVar, &data);
	CmtGetThreadLocalVar (ghThreadLocalVar, &prolog); 
	
	ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
	errChk(SET_EVB_SHDN(channel, 1));
	errChk(EVB5_SET_BEN_Level(INST_EVB[channel], 0)); 
	
	strcpy (prolog.Log_Action, ProcessTypeArray[PROCESS_TYPE_TRX_10GTune]);   
	
	if (gFlag)
	{
		bOK=TRUE; 
		SaveData=FALSE;
		bMOD0 = TRUE;   //清除测试板是否有模块标示
		ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		
		SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_YELLOW);
		SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		
		//检查光路校准和点检是否过期
		if (!sRxCal_Lock.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			errChk(DB_get_cali (CALI_TYPE_RX_MAINLOCK, channel, &timeOutFlag));
		}
		
		if (!sRxCal_Free.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			errChk(DB_get_cali (CALI_TYPE_RX_MAINFREE, channel, &timeOutFlag));
		}
		
		//开始调测
	    ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		timestart=Timer();
		strcpy (Info, "开始进行测试...");
		Insert_Info(panMain, gCtrl_BOX[channel], Info);  
		
		// 光开关设置为2通道
		my_struInstrument[channel].SW_TYPE = SW_TYPE_COFFSW02;
		if (SW_TYPE_NONE != my_struInstrument[channel].SW_TYPE)
		{
			//切换成一通道
			my_struInstrument[channel].SW_CHAN = 2;
			Fiber_SW_SetChannel(my_struInstrument[channel].SW_TYPE, my_struInstrument[channel].SW, my_struInstrument[channel].SW_Handle, my_struInstrument[channel].SW_CHAN);
		}
		
		//初始化配置模块
		if (bOK)
		{
			strcpy (Info, "初始化模块...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			Status = InitModule(channel, &data, &prolog); 
			if (Status) {strcpy (Info, "初始化模块：成功");}
			else	  	{strcpy (Info, "初始化模块：失败");bOK=FALSE;}                      
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
		}
		
		//get bosasn
		if (bOK)
		{
			strcpy (Info, "读取条码...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			Status = GetBosaSN(channel, &data);
			if (Status) {sprintf (Info, "读取条码=%s 成功", data.OpticsSN);SaveData=TRUE;}
			else	    {sprintf (Info, "读取条码=%s 失败", data.OpticsSN);bOK=FALSE;data.ErrorCode=ERR_TUN_READ_INNERSN;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			strcpy (prolog.SN, data.OpticsSN);
		}		    

		//test temperature
		if (bOK)
		{
			strcpy (Info, "测试模块温度...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			Status = testTemper(panMain, channel, &data);
			if (Status) {sprintf (Info, "测试模块温度=%.2f℃ 成功", data.Temperatrue);}  
			else	  	{sprintf (Info, "测试模块温度=%.2f℃ 失败", data.Temperatrue);bOK=FALSE;data.ErrorCode=ERR_TUN_TEST_TEMPER;SaveData=FALSE;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
		}
		
		//test station
		if (bOK && !my_struConfig.isNPI_flag)
		{
			strcpy (Info, "测试工序...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
		//	Status = teststation(data, prolog);
			if (Status) {sprintf (Info, "测试工序：成功");}  
			else	  	{sprintf (Info, "测试工序：失败");bOK=FALSE;data.ErrorCode=ERR_TUN_TEST_STATION;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
		}
			
		//当选择示波器时选用此方法
		//AOP
		if (bOK && my_struConfig.Sel_T_AOP)
		{	
			strcpy (Info, "调试光功率...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			//设置默认tec温度值
			(void)SET_10G_DWDM(INST_EVB[channel], 0xF900);
			// 设置默认EA
			(void)SET_10G_DWDM_EA(INST_EVB[channel], 0x0300);
			// 设置默认ER
			//(void)SET_10G_DWDM_ER(INST_EVB[channel], 0x0168);
			Status = tuningAOP10G(channel, &data);
			if (Status) {sprintf (Info, "调试光功率=%.2fdBm 成功;Ibias=%.2fmA,DAC=0x%X", data.AOP,data.A2_Ibias,data.DACApc);}
			else	    {sprintf (Info, "调试光功率=%.2fdBm 失败;Ibias=%.2fmA,DAC=0x%X", data.AOP,data.A2_Ibias,data.DACApc);bOK=FALSE;data.ErrorCode=ERR_TUN_AOP;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
		}
		
		// er 眼图 mask 光谱等
		if (bOK)
		{
			strcpy (Info, "切换光路...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			error = testParameterLock(channel, &data, panMain, gCtrl_BOX[channel], Info, ERInfo);
			if (error){bOK=FALSE;}
		}
	
		//calTxPower
 	if (bOK && my_struConfig.Sel_Calibrate)
		{	
		 	strcpy (Info, "发端校准...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			Status = calTxPower10G_Ex(channel, &data);
			if (Status) {sprintf (Info, "发端校准：成功,发端监控=%.2fdBm",data.A2_TxMon);}
			else	  	{sprintf (Info, "发端校准：失败,发端监控=%.2fdBm",data.A2_TxMon);bOK=FALSE;data.ErrorCode=ERR_TUN_CAL_TXPOWER;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
		}
			
		//test tx disable power
		if (bOK )  
		{
			strcpy (Info, "测试发端硬件关断光功率..."); 
			Insert_Info(panMain, gCtrl_BOX[channel], Info);     
			error = testTxDis_10G(channel, &data);
			if (error) 	{sprintf (Info, "测试发端硬件关断光功率=%.2fdBm 失败", data.TxOffPower);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_TXDIS;} 
			else		{sprintf (Info, "测试发端硬件关断光功率=%.2fdBm 成功", data.TxOffPower);} 
			Insert_Info(panMain, gCtrl_BOX[channel], Info);     
		}
		
		//灵敏度测试
		if (bOK && my_struConfig.Sel_R_Sens && !my_struConfig.Sel_R_Sens_Real) 
		{
			strcpy (Info, "测试灵敏度...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);      
			Status = testTheSens(channel, &data);
			if (Status) {sprintf (Info, "测试灵敏度=%.2fdBm 成功", data.Sens);}
			else	    {sprintf (Info, "测试灵敏度=%.2fdBm 失败", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);      
		}

		//带纤灵敏度
		if (bOK && my_struConfig.Sel_R_Sens) 	 
		{
			strcpy (Info, "切换光路...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			strcpy (Info, "测试带纤灵敏度...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);         
			Status = testTheLoadSens(channel, &data);
			if (Status) {sprintf (Info, "测试带纤灵敏度=%.2fdBm 成功", data.Sens);}
			else	    {sprintf (Info, "测试带纤灵敏度=%.2fdBm 失败", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);         
		}
		
		if (bOK && my_struConfig.Sel_R_SDHys)
		{
			strcpy (Info, "测试收端LOS...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);     
			Status = testRxLOS(channel, &data);
			if (Status) {sprintf (Info, "测试收端LOS：成功");}  
			else	  	{sprintf (Info, "测试收端LOS：失败");bOK=FALSE;data.ErrorCode=ERR_TES_SD;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);     
		}

		if(!bOK && my_struConfig.Sel_R_SDHys &&  (ERR_TES_SD == data.ErrorCode))
		{
		
			strcpy (Info, "调试收端LOS...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = tuningSD1(channel, &data);  
			if (Status) {sprintf (Info, "调试收端LOS：成功;LOS_DAC=0x%X",data.ucRsd);data.ErrorCode=0;bOK=TRUE;}  
			else	  	{sprintf (Info, "调试收端LOS：失败");bOK=FALSE;data.ErrorCode=ERR_TUN_TEST_LOS;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		
			if (bOK && my_struConfig.Sel_R_SDHys)
			{
				strcpy (Info, "测试收端LOS...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
				Status = testRxLOS(channel, &data);
				if (Status) {sprintf (Info, "测试收端LOS：成功");}  
				else	  	{sprintf (Info, "测试收端LOS：失败");bOK=FALSE;data.ErrorCode=ERR_TES_SD;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			}
		}
		
		//test over 过载测试时间短，放在前面单独测试
		if (bOK && my_struConfig.Sel_R_Over)
		{
			strcpy (Info, "测试过载...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			Status = testFixOver_10G(channel, &data);
			if (Status) {sprintf (Info, "测试过载=%.2fdBm 成功", data.Over);}
			else	  	{sprintf (Info, "测试过载=%.2fdBm 失败", data.Over);bOK=FALSE;data.ErrorCode=ERR_TUN_TEST_OVER;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
		}			  
	
		
	
		if (bOK && my_struConfig.Sel_R_Sens && my_struConfig.Sel_R_Sens_Real) 
		{
			strcpy (Info, "测试灵敏度值...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			error = testDetailSens_10G (channel, &data);
			if (error) {sprintf (Info, "灵敏度测试=%.2fdBm 失败", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TUN_TEST_SENS;} 
			else	   {sprintf (Info, "灵敏度测试=%.2fdBm 成功", data.Sens);}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
		}
		  
		//收端监控校准 
		if (bOK && my_struConfig.Sel_Calibrate)
		{	
		 	strcpy (Info, "收端校准...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
			Status = caltestRxPower_Ex (panMain, channel, &data);   
			if (Status) {sprintf (Info, "收端校准：成功");}
			else	  	{sprintf (Info, "收端校准：失败");bOK=FALSE;data.ErrorCode=ERR_TUN_CAL_RXPOWER;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
		}		   
	
		/****************结束调测*************************/
		
		//调试成功结果赋值
		strcpy (prolog.PN, my_struConfig.PN); 	//PN
		strcpy (prolog.SN, data.OpticsSN);		//SN			
		strcpy (prolog.Operator, g_Operator);//Operator		 
		prolog.ResultsID=0;    						//ResultsID
		//Log_Action  在测试函数开始位置赋值   
		strcpy (prolog.Action_Time, data.TestDate);	//Action_time
		strcpy (prolog.Parameter, "Status");		//Parameter
		strcpy (prolog.SoftVersion, SOFTVER); 
		sprintf (prolog.StationID, "%s-%d", my_struProcessLOG.StationID, channel); 
		if (bOK)
		{
			strcpy (data.Status, "PASS");
			strcpy (prolog.P_Value, "PASS");
			strcpy (prolog.Comments, "");
			strcat (prolog.Comments, data.Comments);
			//-------------------对Log_Action赋值--------------------// 
			//保存调试结果
			(void)SaveFlash_Dwdm(channel);
		}
		else
		{
			if (data.ErrorCode==0) 
			{
				data.ErrorCode=ERR_TES_NO_DEFINE;
			}
			strcpy (data.Status, "FAIL");
			strcpy (prolog.P_Value, "FAIL");
			strcpy (prolog.Comments, Info);
			strcat (prolog.Comments, ERInfo);
			strcat (prolog.Comments, data.Comments);
		}
		prolog.RunTime=Timer()-timestart;  
		// 将数据写入tables
		WriteDateTables(panSearchData_chan0, data);
		//保存调试数据
		if (SaveData)
		{
			strcpy (Info, "保存调试数据...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
			Status = SaveDataBase(data, prolog);
			if (Status) {strcpy (Info, "保存调试数据：成功");}
			else	    {strcpy (Info, "保存调试数据：失败");bOK=FALSE;data.ErrorCode=ERR_TUN_SAVE_DATA;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
		}
		/**************************************************/
		
		//显示运行结果
		prolog.RunTime=Timer()-timestart; 
		if (bOK)	//当测试成功不显示错误代码
		{
			sprintf (Info, "调试运行时间=%ds，\n光纤已使用次数=%d次", prolog.RunTime, data.FiberTime);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_ON_COLOR, VAL_GREEN);
			SetCtrlVal (panMain, gCtrl_LED[channel], 1);
		}
		else
		{
			sprintf (Info, "调试运行时间=%ds，\n光纤已使用次数=%d次，\n错误代码=%d", prolog.RunTime, data.FiberTime, data.ErrorCode);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_RED);
			SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		} 
		Insert_Info(panMain, gCtrl_BOX[channel], Info);
		
#if DEBUG
		Display_PASS_Fail_Rate (DB_Get_Handle(), my_struConfig.batch, prolog.PN, my_struProcessLOG.StationID, prolog.Log_Action, panMain, PAN_MAIN_NUMERIC_PASS, PAN_MAIN_NUMERIC_FAIL, PAN_MAIN_NUMERIC_TOTAL, PAN_MAIN_NUMERIC_PASS_RATE);
#endif 
		
		//检测模块是否在测试板上
		if (bMOD0)
		{
			strcpy (Info, "请将被测模块从测试系统取出...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			do 
			{
				error = checktestbed(channel, &bMOD0);
				if (error) 
				{
					strcpy (Info, "检测测试板是否有模块出现异常");
					Insert_Info(panMain, gCtrl_BOX[channel], Info);  
				}
				else if (!bMOD0)	  	
				{
					break;
				} 
				else
				{
					strcpy (Info, "请将被测模块从测试系统取出...");
					Update_RESULTSInfo(panMain, gCtrl_BOX[channel], Info, FALSE);
				}
			} while (bMOD0 && gFlag);
			
			my_struInstrument[channel].ChannelStart = FALSE;
			m_adjustSpeed = FALSE;
		}	 
	}
	
Error:
	
	my_struInstrument[channel].ChannelStart = FALSE;
	
	strcpy (Info, "此通道调测已停止");
	Insert_Info(panMain, gCtrl_BOX[channel], Info);  

	return error;
}

int CVICALLBACK test_DWDM_10G (void *pvFuncData)
{
	int channel;
	struct struTestData data;
	struct struProcessLOG prolog;
	int	timestart, cnt;
	char Info[500];
	BOOL SaveData=FALSE, bOK=TRUE, bMOD0; 
	int error;
	int  SensTime; 
	int DACtemp = 0;
	
	char ERInfo[50] = ""; 
	
	BOOL timeOutFlag;
	int  TunAopErAlternatelyFlag=0;
	BOOL Status;
	
	int  count;

	/* 	Get the index corresponding to the thread */
	channel = *(int *)pvFuncData;
	
	/*	Get the address of the thread local variable */
	CmtGetThreadLocalVar (ghThreadLocalVar, &data);
	CmtGetThreadLocalVar (ghThreadLocalVar, &prolog); 
	
	ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
	errChk(SET_EVB_SHDN(channel, 1));
	errChk(EVB5_SET_BEN_Level(INST_EVB[channel], 0)); 
	
	if     (my_struConfig.Temper_Low) {strcpy (prolog.Log_Action, ProcessTypeArray[PROCESS_TYPE_TRX_10GTest_L]);}
	else if(my_struConfig.Temper_Room){strcpy (prolog.Log_Action, ProcessTypeArray[PROCESS_TYPE_TRX_10GTest]);} 
	else   							  {strcpy (prolog.Log_Action, ProcessTypeArray[PROCESS_TYPE_TRX_10GTest_H]);}  
	
	if (gFlag)
	{
		bOK=TRUE; 
		SaveData=FALSE;
		bMOD0 = TRUE;   //清除测试板是否有模块标示
		ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		
		SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_YELLOW);
		SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		
		//检查光路校准和点检是否过期
		if (!sRxCal_Lock.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			//errChk(DB_get_cali (CALI_TYPE_RX_MAINLOCK, channel, &timeOutFlag));
		}
		
		if (!sRxCal_Free.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			//errChk(DB_get_cali (CALI_TYPE_RX_MAINFREE, channel, &timeOutFlag));
		}
		
		//开始调测
	    ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		timestart=Timer();
		strcpy (Info, "开始进行测试...");
		Insert_Info(panMain, gCtrl_BOX[channel], Info);   
		
		//初始化配置模块
		if (bOK)
		{
			strcpy (Info, "初始化模块...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = InitModule(channel, &data, &prolog); 
			if (Status) {strcpy (Info, "初始化模块：成功");}
			else	  	{strcpy (Info, "初始化模块：失败");bOK=FALSE;}                      
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		// 光开关设置为2通道
		my_struInstrument[channel].SW_TYPE = SW_TYPE_COFFSW02;
		if (SW_TYPE_NONE != my_struInstrument[channel].SW_TYPE)
		{
			//切换成一通道
			my_struInstrument[channel].SW_CHAN = 2;
			Fiber_SW_SetChannel(my_struInstrument[channel].SW_TYPE, my_struInstrument[channel].SW, my_struInstrument[channel].SW_Handle, my_struInstrument[channel].SW_CHAN);
		}
		
		//get bosasn
		if (bOK)
		{
			strcpy (Info, "读取条码...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = GetBosaSN(channel, &data);
			if (Status) {sprintf (Info, "读取条码=%s 成功", data.OpticsSN);SaveData=TRUE;}
			else	    {sprintf (Info, "读取条码=%s 失败", data.OpticsSN);bOK=FALSE;data.ErrorCode=ERR_TES_READ_INNERSN;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			strcpy (prolog.SN, data.OpticsSN);
		}	
		
		//test temperature
		if (bOK)
		{
			strcpy (Info, "测试模块温度...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testTemper (panMain, channel, &data);
			// 获取常温DAC
			DACtemp = Get_normal_DAC_DWDM(channel);
			if (DACtemp == 0)
			{
				strcpy (Info, "常温DAC获取失败");
				Insert_Info(panMain, gCtrl_BOX[channel], Info);
				bOK=FALSE;
			}
			if (Status) {sprintf (Info, "测试模块温度=%.2f℃ 成功", data.Temperatrue);}  
			else	  	{sprintf (Info, "测试模块温度=%.2f℃ 失败", data.Temperatrue);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_TEMPER;SaveData=FALSE;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		//test station  	
		if (bOK && !my_struConfig.isNPI_flag)
		{
			strcpy (Info, "测试工序...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = teststation(data, prolog);
			if (Status) {sprintf (Info, "测试工序：成功");}  
			else	  	{sprintf (Info, "测试工序：失败");bOK=FALSE;data.ErrorCode=ERR_TES_TEST_STATION;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		//AOP
		if (bOK && my_struConfig.Sel_T_AOP) 
		{
			strcpy (Info, "测试光功率...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = caltestTxPower(channel, &data);  
			if (Status) {sprintf (Info, "测试光功率=%.2fdBm 成功;Ibias=%.2fmA", data.AOP,data.A2_Ibias);}
			else	    {sprintf (Info, "测试光功率=%.2fdBm 失败;Ibias=%.2fmA", data.AOP,data.A2_Ibias);bOK=FALSE;data.ErrorCode=ERR_TES_AOP;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}   

		// 高温测试波长
		if (bOK && my_struConfig.Temper_High)
		{
			strcpy (Info, "高温测试波长...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			error = TestWL_TemperHigh(channel, &data, panMain, gCtrl_BOX[channel], Info, ERInfo);
			if (error)
			{
				sprintf (Info, "测试高温波长=%.2f 失败;", data.PeakWL);
				Insert_Info(panMain, gCtrl_BOX[channel], Info);
				bOK=FALSE;
			} 
			else
			{
				sprintf (Info, "测试高温波长=%.2f 成功;", data.PeakWL);
				Insert_Info(panMain, gCtrl_BOX[channel], Info);
			}
		} 
		
		// er 眼图 mask 光谱等
		if (bOK)
		{
			error = testParameterLock(channel, &data, panMain, gCtrl_BOX[channel], Info, ERInfo);
			if (error){bOK=FALSE;} 
		} 
	
		//cal TxPower
		if (my_struConfig.Sel_Calibrate_Test && bOK && my_struConfig.Sel_T_AOP)
		{
			strcpy (Info, "发端校准测试...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = caltestTxPower(channel, &data);//  caltestTxPower10G
			if (Status) {sprintf (Info,"发端校准测试=%.2fdBm 成功", data.A2_TxMon);}
			else	    {sprintf (Info,"发端校准测试=%.2fdBm 失败", data.A2_TxMon);bOK=FALSE;data.ErrorCode=ERR_TES_CAL_TXPOWER;}        
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
		}
		
		//test sens
		if (bOK && my_struConfig.Sel_R_Sens) 
		{
			strcpy (Info, "测试灵敏度...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);         
			Status = testTheSens(channel, &data);
			if (Status) {sprintf (Info, "测试灵敏度=%.2fdBm 成功", data.Sens);}
			else	    {sprintf (Info, "测试灵敏度=%.2fdBm 失败", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);         
		}

		//带纤灵敏度
		if (bOK && my_struConfig.Sel_R_Sens) 	 
		{
			strcpy (Info, "切换光路...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			strcpy (Info, "测试带纤灵敏度...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);         
			Status = testTheLoadSens(channel, &data);
			if (Status) {sprintf (Info, "测试带纤灵敏度=%.2fdBm 成功", data.Sens);}
			else	    {sprintf (Info, "测试带纤灵敏度=%.2fdBm 失败", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);         
		}
		
		if (bOK && my_struConfig.Sel_R_SDHys)
		{
			strcpy (Info, "测试收端LOS...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			Status = testRxLOS(channel, &data);
			if (Status) {sprintf (Info, "测试收端LOS：成功");}  
			else	  	{sprintf (Info, "测试收端LOS：失败");bOK=FALSE;data.ErrorCode=ERR_TUN_TEST_LOS;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
		}

		//test over
		if (bOK && my_struConfig.Sel_R_Over) 
		{
			strcpy (Info, "测试过载...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);        
			Status = testFixOver_10G(channel, &data); 
			if (Status) {sprintf (Info, "测试过载=%.2fdBm 成功", data.Over);}
			else	    {sprintf (Info, "测试过载=%.2fdBm 失败", data.Over);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_OVER;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);        
		}

		//cal RxPower  
			if (my_struConfig.Sel_Calibrate_Test && bOK)
			{
				strcpy (Info, "收端监控测试...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info);          
				Status = caltestRxPower_Ex (panMain, channel, &data);
				if (Status) {sprintf (Info, "收端监控测试：成功");}
				else	    {sprintf (Info, "收端监控测试：失败");bOK=FALSE;data.ErrorCode=ERR_TES_CAL_RXPOWER;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			}	

		/****************结束调测*************************/		   
	
		/****************结束调测*************************/
		// 将数据写入tables
		WriteDateTables(panSearchData_chan0, data);
		
		//调试成功结果赋值
		strcpy (prolog.PN, my_struConfig.PN); 	//PN
		strcpy (prolog.SN, data.OpticsSN);		//SN			
		strcpy (prolog.Operator, g_Operator);//Operator		 
		prolog.ResultsID=0;    						//ResultsID
		//Log_Action  在测试函数开始位置赋值   
		strcpy (prolog.Action_Time, data.TestDate);	//Action_time
		strcpy (prolog.Parameter, "Status");		//Parameter
		strcpy (prolog.SoftVersion, SOFTVER); 
		sprintf (prolog.StationID, "%s-%d", my_struProcessLOG.StationID, channel); 
		if (bOK)
		{
			strcpy (data.Status, "PASS");
			strcpy (prolog.P_Value, "PASS");
			strcpy (prolog.Comments, "");
			strcat (prolog.Comments, data.Comments);
			//-------------------对Log_Action赋值--------------------//  
			//保存调试结果
			(void)SaveFlash_Dwdm(channel);
		}
		else
		{
			if (data.ErrorCode==0) 
			{
				data.ErrorCode=ERR_TES_NO_DEFINE;
			}
			strcpy (data.Status, "FAIL");
			strcpy (prolog.P_Value, "FAIL");
			strcpy (prolog.Comments, Info);
			strcat (prolog.Comments, ERInfo);
			strcat (prolog.Comments, data.Comments);
		}
		prolog.RunTime=Timer()-timestart;  
		
		//保存调试数据
		if (SaveData)
		{
			strcpy (Info, "保存调试数据...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
			Status = SaveDataBase(data, prolog);
			if (Status) {strcpy (Info, "保存调试数据：成功");}
			else	    {strcpy (Info, "保存调试数据：失败");bOK=FALSE;data.ErrorCode=ERR_TUN_SAVE_DATA;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
		}
		/**************************************************/
		
		//显示运行结果
		prolog.RunTime=Timer()-timestart; 
		if (bOK)	//当测试成功不显示错误代码
		{
			sprintf (Info, "调试运行时间=%ds，\n光纤已使用次数=%d次", prolog.RunTime, data.FiberTime);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_ON_COLOR, VAL_GREEN);
			SetCtrlVal (panMain, gCtrl_LED[channel], 1);
		}
		else
		{
			sprintf (Info, "调试运行时间=%ds，\n光纤已使用次数=%d次，\n错误代码=%d", prolog.RunTime, data.FiberTime, data.ErrorCode);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_RED);
			SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		} 
		Insert_Info(panMain, gCtrl_BOX[channel], Info);
		
#if DEBUG
		Display_PASS_Fail_Rate (DB_Get_Handle(), my_struConfig.batch, prolog.PN, my_struProcessLOG.StationID, prolog.Log_Action, panMain, PAN_MAIN_NUMERIC_PASS, PAN_MAIN_NUMERIC_FAIL, PAN_MAIN_NUMERIC_TOTAL, PAN_MAIN_NUMERIC_PASS_RATE);
#endif  
		
		//检测模块是否在测试板上
		if (bMOD0)
		{
			strcpy (Info, "请将被测模块从测试系统取出...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			do 
			{
				error = checktestbed(channel, &bMOD0);
				if (error) 
				{
					strcpy (Info, "检测测试板是否有模块出现异常");
					Insert_Info(panMain, gCtrl_BOX[channel], Info);  
				}
				else if (!bMOD0)	  	
				{
					break;
				} 
				else
				{
					strcpy (Info, "请将被测模块从测试系统取出...");
					Update_RESULTSInfo(panMain, gCtrl_BOX[channel], Info, FALSE);  
					//Delay(20);
					//break;
				}
			} while (bMOD0 && gFlag);
			
			my_struInstrument[channel].ChannelStart = FALSE;
		}	 
	}
	
Error:
	
	my_struInstrument[channel].ChannelStart = FALSE;
	
	strcpy (Info, "此通道调测已停止");
	Insert_Info(panMain, gCtrl_BOX[channel], Info);  

	return error;
}

int CVICALLBACK test_1G (void *pvFuncData)
{
	int channel;
	struct struTestData data;
	struct struProcessLOG prolog;
	int	timestart, cnt;
	char Info[500];
	BOOL SaveData=FALSE, bOK=TRUE, bMOD0; 
	int error;
	int  SensTime; 
	
	char ERInfo[50] = ""; 
	
	BOOL timeOutFlag;
	int  TunAopErAlternatelyFlag=0;
	BOOL Status;
	
	int  count;

	/* 	Get the index corresponding to the thread */
	channel = *(int *)pvFuncData;
	
	/*	Get the address of the thread local variable */
	CmtGetThreadLocalVar (ghThreadLocalVar, &data);
	CmtGetThreadLocalVar (ghThreadLocalVar, &prolog); 
	
	ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
	errChk(SET_EVB_SHDN(channel, 1));
	errChk(EVB5_SET_BEN_Level(INST_EVB[channel], 0)); 
	
	if     (my_struConfig.Temper_Low) {strcpy (prolog.Log_Action, ProcessTypeArray[PROCESS_TYPE_TRX_Testing_L]);}
	else if(my_struConfig.Temper_Room){strcpy (prolog.Log_Action, ProcessTypeArray[PROCESS_TYPE_TRX_Testing]);} 
	else   							  {strcpy (prolog.Log_Action, ProcessTypeArray[PROCESS_TYPE_TRX_Testing_H]);}  
	
	if (gFlag)
	{
		bOK=TRUE; 
		SaveData=FALSE;
		bMOD0 = TRUE;   //清除测试板是否有模块标示
		ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		
		SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_YELLOW);
		SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		
		//检查光路校准和点检是否过期
		if (!sRxCal_Lock.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			errChk(DB_get_cali (CALI_TYPE_RX_MAINLOCK, channel, &timeOutFlag));
		}
		
		if (!sRxCal_Free.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			errChk(DB_get_cali (CALI_TYPE_RX_MAINFREE, channel, &timeOutFlag));
		}
		
		//开始调测
	    ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		timestart=Timer();
		strcpy (Info, "开始进行测试...");
		Insert_Info(panMain, gCtrl_BOX[channel], Info);   
		
		//初始化配置模块
		if (bOK)
		{
			strcpy (Info, "初始化模块...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = InitModule(channel, &data, &prolog); 
			if (Status) {strcpy (Info, "初始化模块：成功");}
			else	  	{strcpy (Info, "初始化模块：失败");bOK=FALSE;}                      
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		//get bosasn
		if (bOK)
		{
			strcpy (Info, "读取条码...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = GetBosaSN(channel, &data);
			if (Status) {sprintf (Info, "读取条码=%s 成功", data.OpticsSN);SaveData=TRUE;}
			else	    {sprintf (Info, "读取条码=%s 失败", data.OpticsSN);bOK=FALSE;data.ErrorCode=ERR_TES_READ_INNERSN;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			strcpy (prolog.SN, data.OpticsSN);
		}	
		
		//test temperature
		if (bOK)
		{
			strcpy (Info, "测试模块温度...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testTemper (panMain, channel, &data);
			if (Status) {sprintf (Info, "测试模块温度=%.2f℃ 成功", data.Temperatrue);}  
			else	  	{sprintf (Info, "测试模块温度=%.2f℃ 失败", data.Temperatrue);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_TEMPER;SaveData=FALSE;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		//test station  	
		if (bOK && !my_struConfig.isNPI_flag)
		{
			strcpy (Info, "测试工序...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = teststation(data, prolog);
			if (Status) {sprintf (Info, "测试工序：成功");}  
			else	  	{sprintf (Info, "测试工序：失败");bOK=FALSE;data.ErrorCode=ERR_TES_TEST_STATION;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		//test sens  灵敏度测试时间与调试AOP，ER，测试光谱复用
		if (bOK && my_struConfig.Sel_R_Sens && !my_struConfig.Sel_R_Sens_Real)  
		{
			strcpy (Info, "灵敏度测试...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testFixSens_1G(channel, &data);
			if (Status) {sprintf (Info, "灵敏度测试=%.2fdBm 成功", data.Sens);}
			else	    {sprintf (Info, "灵敏度测试=%.2fdBm 失败", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;}  
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		if (bOK && my_struConfig.Sel_R_Sens && my_struConfig.Sel_R_Sens_Real) 
		{
			strcpy (Info, "测试灵敏度值...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			error = testDetailSens_1G (channel, &data);
			if (error) {sprintf (Info, "灵敏度测试=%.2fdBm 失败", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;} 
			else	   {sprintf (Info, "灵敏度测试=%.2fdBm 成功", data.Sens);}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
			
		//test overload
		if (bOK && my_struConfig.Sel_R_Over)    //放到相关发端测试、眼图测试之后
		{
			strcpy (Info, "测试过载...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testFixOver_1G(channel, &data);
			if (Status) {sprintf (Info, "测试过载=%.2fdBm 成功", data.Over);}
			else	    {sprintf (Info, "测试过载=%.2fdBm 失败", data.Over);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_OVER;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}	
		/****************结束调测*************************/		   
	
		/****************结束调测*************************/
		
		//调试成功结果赋值
		strcpy (prolog.PN, my_struConfig.PN); 	//PN
		strcpy (prolog.SN, data.OpticsSN);		//SN			
		strcpy (prolog.Operator, g_Operator);//Operator		 
		prolog.ResultsID=0;    						//ResultsID
		//Log_Action  在测试函数开始位置赋值   
		strcpy (prolog.Action_Time, data.TestDate);	//Action_time
		strcpy (prolog.Parameter, "Status");		//Parameter
		strcpy (prolog.SoftVersion, SOFTVER); 
		sprintf (prolog.StationID, "%s-%d", my_struProcessLOG.StationID, channel); 
		if (bOK)
		{
			strcpy (data.Status, "PASS");
			strcpy (prolog.P_Value, "PASS");
			strcpy (prolog.Comments, "");
			strcat (prolog.Comments, data.Comments);
			//-------------------对Log_Action赋值--------------------//  
		}
		else
		{
			if (data.ErrorCode==0) 
			{
				data.ErrorCode=ERR_TES_NO_DEFINE;
			}
			strcpy (data.Status, "FAIL");
			strcpy (prolog.P_Value, "FAIL");
			strcpy (prolog.Comments, Info);
			strcat (prolog.Comments, ERInfo);
			strcat (prolog.Comments, data.Comments);
		}
		prolog.RunTime=Timer()-timestart;  
		
		//保存调试数据
		if (SaveData)
		{
			strcpy (Info, "保存调试数据...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
			Status = SaveDataBase(data, prolog);
			if (Status) {strcpy (Info, "保存调试数据：成功");}
			else	    {strcpy (Info, "保存调试数据：失败");bOK=FALSE;data.ErrorCode=ERR_TUN_SAVE_DATA;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
		}
		/**************************************************/
		
		//显示运行结果
		prolog.RunTime=Timer()-timestart; 
		if (bOK)	//当测试成功不显示错误代码
		{
			sprintf (Info, "调试运行时间=%ds，\n光纤已使用次数=%d次", prolog.RunTime, data.FiberTime);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_ON_COLOR, VAL_GREEN);
			SetCtrlVal (panMain, gCtrl_LED[channel], 1);
		}
		else
		{
			sprintf (Info, "调试运行时间=%ds，\n光纤已使用次数=%d次，\n错误代码=%d", prolog.RunTime, data.FiberTime, data.ErrorCode);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_RED);
			SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		} 
		Insert_Info(panMain, gCtrl_BOX[channel], Info);
		
#if DEBUG
		Display_PASS_Fail_Rate (DB_Get_Handle(), my_struConfig.batch, prolog.PN, my_struProcessLOG.StationID, prolog.Log_Action, panMain, PAN_MAIN_NUMERIC_PASS, PAN_MAIN_NUMERIC_FAIL, PAN_MAIN_NUMERIC_TOTAL, PAN_MAIN_NUMERIC_PASS_RATE);
#endif  
		
		//检测模块是否在测试板上
		if (bMOD0)
		{
			strcpy (Info, "请将被测模块从测试系统取出...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			do 
			{
				error = checktestbed(channel, &bMOD0);
				if (error) 
				{
					strcpy (Info, "检测测试板是否有模块出现异常");
					Insert_Info(panMain, gCtrl_BOX[channel], Info);  
				}
				else if (!bMOD0)	  	
				{
					break;
				} 
				else
				{
					strcpy (Info, "请将被测模块从测试系统取出...");
					Update_RESULTSInfo(panMain, gCtrl_BOX[channel], Info, FALSE);    
				}
			} while (bMOD0 && gFlag);
			
			my_struInstrument[channel].ChannelStart = FALSE;
		}	 
	}
	
Error:
	
	my_struInstrument[channel].ChannelStart = FALSE;
	
	strcpy (Info, "此通道调测已停止");
	Insert_Info(panMain, gCtrl_BOX[channel], Info);  

	return error;
}

int CVICALLBACK QAtest_1G (void *pvFuncData)
{
	int channel;
	struct struTestData data;
	struct struProcessLOG prolog;
	int	timestart, cnt;
	char Info[500];
	BOOL SaveData=FALSE, bOK=TRUE, bMOD0; 
	int error;
	int  SensTime; 
	
	char ERInfo[50] = ""; 
	
	BOOL timeOutFlag;
	int  TunAopErAlternatelyFlag=0;
	BOOL Status;
	
	int  count;

	/* 	Get the index corresponding to the thread */
	channel = *(int *)pvFuncData;
	
	/*	Get the address of the thread local variable */
	CmtGetThreadLocalVar (ghThreadLocalVar, &data);
	CmtGetThreadLocalVar (ghThreadLocalVar, &prolog); 
	
	ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
	errChk(SET_EVB_SHDN(channel, 1));
	errChk(EVB5_SET_BEN_Level(INST_EVB[channel], 0)); 
	
	if     (my_struConfig.Temper_Low) {strcpy (prolog.Log_Action, ProcessTypeArray[PROCESS_TYPE_TRX_QA_1GTest_L]);}
	else if(my_struConfig.Temper_Room){strcpy (prolog.Log_Action, ProcessTypeArray[PROCESS_TYPE_TRX_QA_1GTest]);} 
	else   							  {strcpy (prolog.Log_Action, ProcessTypeArray[PROCESS_TYPE_TRX_QA_1GTest_H]);}
	
	if (gFlag)
	{
		bOK=TRUE; 
		SaveData=FALSE;
		bMOD0 = TRUE;   //清除测试板是否有模块标示
		ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		
		SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_YELLOW);
		SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		
		//检查光路校准和点检是否过期
		if (!sRxCal_Lock.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			errChk(DB_get_cali (CALI_TYPE_RX_MAINLOCK, channel, &timeOutFlag));
		}
		
		if (!sRxCal_Free.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			errChk(DB_get_cali (CALI_TYPE_RX_MAINFREE, channel, &timeOutFlag));
		}
		
		//开始调测
	    ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		timestart=Timer();
		strcpy (Info, "开始进行测试...");
		Insert_Info(panMain, gCtrl_BOX[channel], Info);   
		
		//初始化配置模块
		if (bOK)
		{
			strcpy (Info, "初始化模块...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = InitModule(channel, &data, &prolog); 
			if (Status) {strcpy (Info, "初始化模块：成功");}
			else	  	{strcpy (Info, "初始化模块：失败");bOK=FALSE;}                      
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		//get bosasn
		if (bOK)
		{
			strcpy (Info, "读取条码...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = GetBosaSN(channel, &data);
			if (Status) {sprintf (Info, "读取条码=%s 成功", data.OpticsSN);SaveData=TRUE;}
			else	    {sprintf (Info, "读取条码=%s 失败", data.OpticsSN);bOK=FALSE;data.ErrorCode=ERR_TES_READ_INNERSN;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			strcpy (prolog.SN, data.OpticsSN);
		}	
		
		//test temperature
		if (bOK)
		{
			strcpy (Info, "测试模块温度...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testTemper (panMain, channel, &data);
			if (Status) {sprintf (Info, "测试模块温度=%.2f℃ 成功", data.Temperatrue);}  
			else	  	{sprintf (Info, "测试模块温度=%.2f℃ 失败", data.Temperatrue);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_TEMPER;SaveData=FALSE;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		//test station  	
		if (bOK && !my_struConfig.isNPI_flag)
		{
			strcpy (Info, "测试工序...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = teststation(data, prolog);
			if (Status) {sprintf (Info, "测试工序：成功");}  
			else	  	{sprintf (Info, "测试工序：失败");bOK=FALSE;data.ErrorCode=ERR_TES_TEST_STATION;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		//test sens  灵敏度测试时间与调试AOP，ER，测试光谱复用
		if (bOK && my_struConfig.Sel_R_Sens && !my_struConfig.Sel_R_Sens_Real)  
		{
			strcpy (Info, "灵敏度测试...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testFixSens_1G(channel, &data);
			if (Status) {sprintf (Info, "灵敏度测试=%.2fdBm 成功", data.Sens);}
			else	    {sprintf (Info, "灵敏度测试=%.2fdBm 失败", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;}  
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		if (bOK && my_struConfig.Sel_R_Sens && my_struConfig.Sel_R_Sens_Real) 
		{
			strcpy (Info, "测试灵敏度值...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			error = testDetailSens_1G (channel, &data);
			if (error) {sprintf (Info, "灵敏度测试=%.2fdBm 失败", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;} 
			else	   {sprintf (Info, "灵敏度测试=%.2fdBm 成功", data.Sens);}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
			
		//test overload
		if (bOK && my_struConfig.Sel_R_Over)    //放到相关发端测试、眼图测试之后
		{
			strcpy (Info, "测试过载...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testFixOver_1G(channel, &data);
			if (Status) {sprintf (Info, "测试过载=%.2fdBm 成功", data.Over);}
			else	    {sprintf (Info, "测试过载=%.2fdBm 失败", data.Over);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_OVER;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}	
		/****************结束调测*************************/		   
	
		/****************结束调测*************************/
		
		//调试成功结果赋值
		strcpy (prolog.PN, my_struConfig.PN); 	//PN
		strcpy (prolog.SN, data.OpticsSN);		//SN			
		strcpy (prolog.Operator, g_Operator);//Operator		 
		prolog.ResultsID=0;    						//ResultsID
		//Log_Action  在测试函数开始位置赋值   
		strcpy (prolog.Action_Time, data.TestDate);	//Action_time
		strcpy (prolog.Parameter, "Status");		//Parameter
		strcpy (prolog.SoftVersion, SOFTVER); 
		sprintf (prolog.StationID, "%s-%d", my_struProcessLOG.StationID, channel); 
		if (bOK)
		{
			strcpy (data.Status, "PASS");
			strcpy (prolog.P_Value, "PASS");
			strcpy (prolog.Comments, "");
			strcat (prolog.Comments, data.Comments);
			//-------------------对Log_Action赋值--------------------//  
		}
		else
		{
			if (data.ErrorCode==0) 
			{
				data.ErrorCode=ERR_TES_NO_DEFINE;
			}
			strcpy (data.Status, "FAIL");
			strcpy (prolog.P_Value, "FAIL");
			strcpy (prolog.Comments, Info);
			strcat (prolog.Comments, ERInfo);
			strcat (prolog.Comments, data.Comments);
		}
		prolog.RunTime=Timer()-timestart;  
		
		//保存调试数据
		if (SaveData)
		{
			strcpy (Info, "保存调试数据...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
			Status = SaveDataBase(data, prolog);
			if (Status) {strcpy (Info, "保存调试数据：成功");}
			else	    {strcpy (Info, "保存调试数据：失败");bOK=FALSE;data.ErrorCode=ERR_TUN_SAVE_DATA;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
		}
		/**************************************************/
		
		//显示运行结果
		prolog.RunTime=Timer()-timestart; 
		if (bOK)	//当测试成功不显示错误代码
		{
			sprintf (Info, "调试运行时间=%ds，\n光纤已使用次数=%d次", prolog.RunTime, data.FiberTime);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_ON_COLOR, VAL_GREEN);
			SetCtrlVal (panMain, gCtrl_LED[channel], 1);
		}
		else
		{
			sprintf (Info, "调试运行时间=%ds，\n光纤已使用次数=%d次，\n错误代码=%d", prolog.RunTime, data.FiberTime, data.ErrorCode);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_RED);
			SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		} 
		Insert_Info(panMain, gCtrl_BOX[channel], Info);
		
#if DEBUG
		Display_PASS_Fail_Rate (DB_Get_Handle(), my_struConfig.batch, prolog.PN, my_struProcessLOG.StationID, prolog.Log_Action, panMain, PAN_MAIN_NUMERIC_PASS, PAN_MAIN_NUMERIC_FAIL, PAN_MAIN_NUMERIC_TOTAL, PAN_MAIN_NUMERIC_PASS_RATE);
#endif  
		
		//检测模块是否在测试板上
		if (bMOD0)
		{
			strcpy (Info, "请将被测模块从测试系统取出...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			do 
			{
				error = checktestbed(channel, &bMOD0);
				if (error) 
				{
					strcpy (Info, "检测测试板是否有模块出现异常");
					Insert_Info(panMain, gCtrl_BOX[channel], Info);  
				}
				else if (!bMOD0)	  	
				{
					break;
				} 
				else
				{
					strcpy (Info, "请将被测模块从测试系统取出...");
					Update_RESULTSInfo(panMain, gCtrl_BOX[channel], Info, FALSE);    
				}
			} while (bMOD0 && gFlag);
			
			my_struInstrument[channel].ChannelStart = FALSE;
		}	 
	}
	
Error:
	
	my_struInstrument[channel].ChannelStart = FALSE;
	
	strcpy (Info, "此通道调测已停止");
	Insert_Info(panMain, gCtrl_BOX[channel], Info);  

	return error;
}
// 调试DWDM
int CVICALLBACK tun_DWDM(void *pvFuncData)
{
	return 0;
}
// 测试DWDM
int CVICALLBACK test_DWDM(void * pvFuncData)
{
	return 0;
}
int CVICALLBACK QAtest_10G(void *pvFuncData)
{
	int channel;
	struct struTestData data;
	struct struProcessLOG prolog;
	int	timestart, cnt;
	char Info[500];
	BOOL SaveData=FALSE, bOK=TRUE, bMOD0; 
	int error;
	int  SensTime; 
	
	char ERInfo[50] = "";
	
	BOOL timeOutFlag;
	int  TunAopErAlternatelyFlag=0;
	BOOL Status;
	
	int  count;

	/* 	Get the index corresponding to the thread */
	channel = *(int *)pvFuncData;
	
	/*	Get the address of the thread local variable */
	CmtGetThreadLocalVar (ghThreadLocalVar, &data);
	CmtGetThreadLocalVar (ghThreadLocalVar, &prolog); 
	
	ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
	errChk(SET_EVB_SHDN(channel, 1));
	errChk(EVB5_SET_BEN_Level(INST_EVB[channel], 0)); 
	
	if     (my_struConfig.Temper_Low) {strcpy (prolog.Log_Action, ProcessTypeArray[PROCESS_TYPE_TRX_QA_10GTest_L]);}
	else if(my_struConfig.Temper_Room){strcpy (prolog.Log_Action, ProcessTypeArray[PROCESS_TYPE_TRX_QA_10GTest]);} 
	else   							  {strcpy (prolog.Log_Action, ProcessTypeArray[PROCESS_TYPE_TRX_QA_10GTest_H]);}
	
	if (gFlag)
	{
		bOK=TRUE; 
		SaveData=FALSE;
		bMOD0 = TRUE;   //清除测试板是否有模块标示
		ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		
		SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_YELLOW);
		SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		
		//检查光路校准和点检是否过期
		if (!sRxCal_Lock.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			errChk(DB_get_cali (CALI_TYPE_RX_MAINLOCK, channel, &timeOutFlag));
		}
		
		if (!sRxCal_Free.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			errChk(DB_get_cali (CALI_TYPE_RX_MAINFREE, channel, &timeOutFlag));
		}
		
		//开始调测
	    ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		timestart=Timer();
		strcpy (Info, "开始进行测试...");
		Insert_Info(panMain, gCtrl_BOX[channel], Info);   
		
		//初始化配置模块
		if (bOK)
		{
			strcpy (Info, "初始化模块...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = InitModule(channel, &data, &prolog); 
			if (Status) {strcpy (Info, "初始化模块：成功");}
			else	  	{strcpy (Info, "初始化模块：失败");bOK=FALSE;}                      
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		//get bosasn
		if (bOK)
		{
			strcpy (Info, "读取条码...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = GetBosaSN(channel, &data);
			if (Status) {sprintf (Info, "读取条码=%s 成功", data.OpticsSN);SaveData=TRUE;}
			else	    {sprintf (Info, "读取条码=%s 失败", data.OpticsSN);bOK=FALSE;data.ErrorCode=ERR_TES_READ_INNERSN;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			strcpy (prolog.SN, data.OpticsSN);
		}					 
		//test temperature
		if (bOK)
		{
			strcpy (Info, "测试模块温度...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testTemper (panMain, channel, &data);
			if (Status) {sprintf (Info, "测试模块温度=%.2f℃ 成功", data.Temperatrue);}  
			else	  	{sprintf (Info, "测试模块温度=%.2f℃ 失败", data.Temperatrue);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_TEMPER;SaveData=FALSE;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		//test station  	
		if (bOK)
		{
			strcpy (Info, "测试工序...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = teststation(data, prolog);
			if (Status) {sprintf (Info, "测试工序：成功");}  
			else	  	{sprintf (Info, "测试工序：失败");bOK=FALSE;data.ErrorCode=ERR_TES_TEST_STATION;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}		   
	/*	//test SD
		if (bOK && my_struConfig.Sel_R_SDHys)   
		{																		
			strcpy (Info, "测试SD信号...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testSD();
			if (Status) {strcpy (Info, "测试SD信号：成功");}
			else	    {strcpy (Info, "测试SD信号：失败");bOK=FALSE;my_struTestData.ErrorCode=ERR_TES_SD;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}	
	*/

		//AOP
		if (bOK && my_struConfig.Sel_T_AOP) 
		{
			strcpy (Info, "测试光功率...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testAOP10G(channel, &data);
			if (Status) {sprintf (Info, "测试光功率=%.2fdBm 成功;Ibias=%.2fmA", data.AOP,data.A2_Ibias);}
			else	    {sprintf (Info, "测试光功率=%.2fdBm 失败;Ibias=%.2fmA", data.AOP,data.A2_Ibias);bOK=FALSE;data.ErrorCode=ERR_TES_AOP;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}  

		// er 眼图 mask 光谱等
		if (bOK)
		{
			strcpy (Info, "切换光路...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			error = testParameterLock(channel, &data, panMain, gCtrl_BOX[channel], Info, ERInfo);
			if (error){bOK=FALSE;} //此处不需要显示相关结果，testParameterLock函数已经显示 
		} 
	
		//cal TxPower
		if (my_struConfig.Sel_Calibrate_Test && bOK && my_struConfig.Sel_T_AOP)
		{
			strcpy (Info, "发端校准测试...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = caltestTxPower10G(channel, &data);
			if (Status) {sprintf (Info,"发端校准测试=%.2fdBm 成功", data.RxTr);}
			else	    {sprintf (Info,"发端校准测试=%.2fdBm 失败", data.RxTr);bOK=FALSE;data.ErrorCode=ERR_TES_CAL_TXPOWER;}        
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
		}
		
		//test overload
		if (bOK && my_struConfig.Sel_R_Over)    //放到相关发端测试、眼图测试之后
		{
			strcpy (Info, "测试过载...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testFixOver_10G(channel, &data);
			if (Status) {sprintf (Info, "测试过载=%.2fdBm 成功", data.Over);}
			else	    {sprintf (Info, "测试过载=%.2fdBm 失败", data.Over);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_OVER;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}	
		
		//test current
		if (bOK && (my_struInstrument[channel].SEVB_TYPE==SEVB_TYPE_SEVB5|| my_struInstrument[channel].SEVB_TYPE==SEVB_TYPE_SEVB27))
		{
			strcpy (Info, "测试电流...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);                   
			Status = testCurrent(channel, &data);
			if (Status) {sprintf (Info, "测试收端电流=%.2f 发端电流=%.2f 成功;Ibias=%.2fmA ", data.RxI, data.TxI,data.A2_Ibias);} 
			else	    {sprintf (Info, "测试收端电流=%.2f 发端电流=%.2f 失败", data.RxI, data.TxI);bOK=FALSE;data.ErrorCode=ERR_TUN_TEST_CURRENT;}        
			Insert_Info(panMain, gCtrl_BOX[channel], Info);                   
		}
		
		//AOP
		if (bOK && my_struConfig.Sel_T_TxDis) 
		{
			strcpy (Info, "测试发端硬件关断光功率...");                
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testTxDis_10G(channel, &data);
			if (error) 	{sprintf (Info, "测试发端硬件关断光功率=%.2fdBm 失败", data.TxOffPower);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_TXDIS;} 
			else		{sprintf (Info, "测试发端硬件关断光功率=%.2fdBm 成功", data.TxOffPower);}  
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}  
		
		//test sens  灵敏度测试时间与调试AOP，ER，测试光谱复用
		if (bOK && my_struConfig.Sel_R_Sens && !my_struConfig.Sel_R_Sens_Real)  
		{
			strcpy (Info, "灵敏度测试...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testFixSens_10G(channel, &data);
			if (Status) {sprintf (Info, "灵敏度测试=%.2fdBm 成功", data.Sens);}
			else	    {sprintf (Info, "灵敏度测试=%.2fdBm 失败", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;}  
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		if (bOK && my_struConfig.Sel_R_Sens && my_struConfig.Sel_R_Sens_Real) 
		{
			strcpy (Info, "测试灵敏度值...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			error = testDetailSens_10G (channel, &data);
			if (error) {sprintf (Info, "灵敏度测试=%.2fdBm 失败", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;} 
			else	   {sprintf (Info, "灵敏度测试=%.2fdBm 成功", data.Sens);}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		//cal RxPower  
		if (my_struConfig.Sel_Calibrate_Test && bOK)
		{
			strcpy (Info, "收端校准测试...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = caltest_RxPower10G (panMain, channel, &data);
			if (Status) {sprintf (Info, "收端校准测试：成功");}
			else	    
			{
				sprintf (Info, "收端校准测试：失败");
				bOK=FALSE;
				if (data.ErrorCode==0) 
				{
					data.ErrorCode=ERR_TES_CAL_RXPOWER;
				}
			}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}  	   
	
		/****************结束调测*************************/
		
		//调试成功结果赋值
		strcpy (prolog.PN, my_struConfig.PN); 	//PN
		strcpy (prolog.SN, data.OpticsSN);		//SN			
		strcpy (prolog.Operator, g_Operator);//Operator		 
		prolog.ResultsID=0;    						//ResultsID
		//Log_Action  在测试函数开始位置赋值   
		strcpy (prolog.Action_Time, data.TestDate);	//Action_time
		strcpy (prolog.Parameter, "Status");		//Parameter
		strcpy (prolog.SoftVersion, SOFTVER); 
		sprintf (prolog.StationID, "%s-%d", my_struProcessLOG.StationID, channel); 
		if (bOK)
		{
			strcpy (data.Status, "PASS");
			strcpy (prolog.P_Value, "PASS");
			strcpy (prolog.Comments, "");
			strcat (prolog.Comments, data.Comments);
			//-------------------对Log_Action赋值--------------------//  
		}
		else
		{
			if (data.ErrorCode==0) 
			{
				data.ErrorCode=ERR_TES_NO_DEFINE;
			}
			strcpy (data.Status, "FAIL");
			strcpy (prolog.P_Value, "FAIL");
			strcpy (prolog.Comments, Info);
			strcat (prolog.Comments, ERInfo);
			strcat (prolog.Comments, data.Comments);
		}
		prolog.RunTime=Timer()-timestart;  
		
		//保存调试数据
		if (SaveData)
		{
			strcpy (Info, "保存调试数据...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
			Status = SaveDataBase(data, prolog);
			if (Status) {strcpy (Info, "保存调试数据：成功");}
			else	    {strcpy (Info, "保存调试数据：失败");bOK=FALSE;data.ErrorCode=ERR_TUN_SAVE_DATA;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
		}
		/**************************************************/
		
		//显示运行结果
		prolog.RunTime=Timer()-timestart; 
		if (bOK)	//当测试成功不显示错误代码
		{
			sprintf (Info, "调试运行时间=%ds，\n光纤已使用次数=%d次", prolog.RunTime, data.FiberTime);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_ON_COLOR, VAL_GREEN);
			SetCtrlVal (panMain, gCtrl_LED[channel], 1);
		}
		else
		{
			sprintf (Info, "调试运行时间=%ds，\n光纤已使用次数=%d次，\n错误代码=%d", prolog.RunTime, data.FiberTime, data.ErrorCode);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_RED);
			SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		} 
		Insert_Info(panMain, gCtrl_BOX[channel], Info);

#if DEBUG
		Display_PASS_Fail_Rate (DB_Get_Handle(), my_struConfig.batch, prolog.PN, my_struProcessLOG.StationID, prolog.Log_Action, panMain, PAN_MAIN_NUMERIC_PASS, PAN_MAIN_NUMERIC_FAIL, PAN_MAIN_NUMERIC_TOTAL, PAN_MAIN_NUMERIC_PASS_RATE);
#endif 
		
		//检测模块是否在测试板上
		if (bMOD0)
		{
			strcpy (Info, "请将被测模块从测试系统取出...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			do 
			{
				error = checktestbed(channel, &bMOD0);
				if (error) 
				{
					strcpy (Info, "检测测试板是否有模块出现异常");
					Insert_Info(panMain, gCtrl_BOX[channel], Info);  
				}
				else if (!bMOD0)	  	
				{
					break;
				} 
				else
				{
					strcpy (Info, "请将被测模块从测试系统取出...");
					Update_RESULTSInfo(panMain, gCtrl_BOX[channel], Info, FALSE);
				}
			} while (bMOD0 && gFlag);
			
			my_struInstrument[channel].ChannelStart = FALSE;
		}	 
	}
	
Error:
	
	my_struInstrument[channel].ChannelStart = FALSE;
	
	strcpy (Info, "此通道调测已停止");
	Insert_Info(panMain, gCtrl_BOX[channel], Info);  

	return error;
}
 
void CVICALLBACK On_Control (int menuBar, int menuItem, void *callbackData,
		int panel)
{ 
	if (!MyDLL_License_Load()) return ; 
	showphID = License_Flag_panCtr;  
}

int CVICALLBACK On_Ctr_Save (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int index,tabPanel_jitter;
	char str[30];
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			//InitModule
			GetCtrlVal(TabPanel_Init,InitModule_RING_I2cAddr,&my_struControl.Init_I2cAddr);  
			GetCtrlVal(TabPanel_Init,InitModule_RING_PassWord,&my_struControl.Init_PowerWord_Index);
	//		GetCtrlIndex(TabPanel_Init, InitModule_RING_PassWord,&index); 
	//		GetLabelFromIndex (TabPanel_Init, InitModule_RING_PassWord,index, my_struControl.Init_PowerWord);  
			GetCtrlVal(TabPanel_Init,InitModule_STR_GetVersion,my_struControl.Init_GetVersion);   
			
			//GetBosaSN  
			GetCtrlVal(TabPanel_GetSN,GetBosaSN_RING_I2cAddr,&my_struControl.GetSN_I2cAddr); 
			GetCtrlVal(TabPanel_GetSN,GetBosaSN_RING_Table,&my_struControl.GetSN_Table);  
			
			//GetTemper
			GetCtrlVal(TabPanel_GetTemper,TestTemper_STR_GetTempCase,my_struControl.GetTemper_CaseCommd);   
			GetCtrlVal(TabPanel_GetTemper,TestTemper_STR_GetTempCore,my_struControl.GetTemper_CoreCommd);  
			GetCtrlVal(TabPanel_GetTemper,TestTemper_NUM_GetTempOffset,&my_struControl.GetTemper_IndexOffset);  
			
			//TunAOP 1G
			GetCtrlVal(TabPanel_TunAOP1G,TunAOP1G_STR_MODE_M,my_struControl.TunAOP1G_MODE_M); 
			GetCtrlVal(TabPanel_TunAOP1G,TunAOP1G_STR_MODE_A,my_struControl.TunAOP1G_MODE_A); 
			GetCtrlVal(TabPanel_TunAOP1G,TunAOP1G_STR_DAC_SET,my_struControl.TunAOP1G_DAC_SET); 
			GetCtrlVal(TabPanel_TunAOP1G,TunAOP1G_RING_LUT_NAME,&my_struControl.TunAOP1G_LUT_NAME_INDEX);    
			GetCtrlVal(TabPanel_TunAOP1G,TunAOP1G_NUM_LUK_INDEX,&my_struControl.TunAOP1G_LUK_INDEX); 
			GetCtrlVal(TabPanel_TunAOP1G,TunAOP1G_NUM_SLOPE_AMP,&my_struControl.TunAOP1G_LUK_SLOPE_AMP);  
			GetCtrlVal(TabPanel_TunAOP1G,TunAOP1G_NUM_OFFSET_AMP,&my_struControl.TunAOP1G_LUK_OFFSET_AMP);  
			GetCtrlVal(TabPanel_TunAOP1G,TunAOP1G_STR_SAVE,my_struControl.TunAOP1G_SAVE); 
	
			//TunAOP 10G
			GetCtrlVal(TabPanel_TunAOP10G,TunAOP10G_STR_MODE_M,my_struControl.TunAOP10G_MODE_M); 
			GetCtrlVal(TabPanel_TunAOP10G,TunAOP10G_STR_MODE_A,my_struControl.TunAOP10G_MODE_A); 
			GetCtrlVal(TabPanel_TunAOP10G,TunAOP10G_STR_DAC_SET,my_struControl.TunAOP10G_DAC_SET); 
			GetCtrlVal(TabPanel_TunAOP10G,TunAOP10G_RING_LUT_NAME,&my_struControl.TunAOP10G_LUT_NAME_INDEX); 
			GetCtrlVal(TabPanel_TunAOP10G,TunAOP10G_NUM_LUK_INDEX,&my_struControl.TunAOP10G_LUK_INDEX);
			GetCtrlVal(TabPanel_TunAOP10G,TunAOP10G_NUM_SLOPE_AMP,&my_struControl.TunAOP10G_LUK_SLOPE_AMP);  
			GetCtrlVal(TabPanel_TunAOP10G,TunAOP10G_NUM_OFFSET_AMP,&my_struControl.TunAOP10G_LUK_OFFSET_AMP); 
			GetCtrlVal(TabPanel_TunAOP10G,TunAOP10G_STR_SAVE,my_struControl.TunAOP10G_SAVE);  
			
			//TunER 1G  
			GetCtrlVal(TabPanel_TunER1G,TunER1G_STR_MODE_M,my_struControl.TunER1G_MODE_M); 
			GetCtrlVal(TabPanel_TunER1G,TunER1G_STR_MODE_A,my_struControl.TunER1G_MODE_A); 
			GetCtrlVal(TabPanel_TunER1G,TunER1G_STR_DAC_SET,my_struControl.TunER1G_DAC_SET); 
			GetCtrlVal(TabPanel_TunER1G,TunER1G_RING_LUT_NAME,&my_struControl.TunER1G_LUT_NAME_INDEX); 
			GetCtrlVal(TabPanel_TunER1G,TunER1G_NUM_LUK_INDEX,&my_struControl.TunER1G_LUK_INDEX); 
			GetCtrlVal(TabPanel_TunER1G,TunER1G_NUM_SLOPE_AMP,&my_struControl.TunER1G_LUK_SLOPE_AMP);  
			GetCtrlVal(TabPanel_TunER1G,TunER1G_NUM_OFFSET_AMP,&my_struControl.TunER1G_LUK_OFFSET_AMP); 
			GetCtrlVal(TabPanel_TunER1G,TunER1G_STR_SAVE,my_struControl.TunER1G_SAVE);  
			
			//TunER 10G  
			GetCtrlVal(TabPanel_TunER10G,TunER10G_STR_MODE_M,my_struControl.TunER10G_MODE_M); 
			GetCtrlVal(TabPanel_TunER10G,TunER10G_STR_MODE_A,my_struControl.TunER10G_MODE_A); 
			GetCtrlVal(TabPanel_TunER10G,TunER10G_STR_DAC_SET,my_struControl.TunER10G_DAC_SET); 
			GetCtrlVal(TabPanel_TunER10G,TunER10G_RING_LUT_NAME,&my_struControl.TunER10G_LUT_NAME_INDEX); 
			GetCtrlVal(TabPanel_TunER10G,TunER10G_NUM_LUK_INDEX,&my_struControl.TunER10G_LUK_INDEX); 
			GetCtrlVal(TabPanel_TunER10G,TunER10G_NUM_SLOPE_AMP,&my_struControl.TunER10G_LUK_SLOPE_AMP);  
			GetCtrlVal(TabPanel_TunER10G,TunER10G_NUM_OFFSET_AMP,&my_struControl.TunER10G_LUK_OFFSET_AMP); 
			GetCtrlVal(TabPanel_TunER10G,TunER10G_STR_SAVE,my_struControl.TunER10G_SAVE);  
			
			//TunCross 1G  
			GetCtrlVal(TabPanel_TunCross1G,TunCross1G_STR_MODE_M,my_struControl.TunCross1G_MODE_M); 
			GetCtrlVal(TabPanel_TunCross1G,TunCross1G_STR_MODE_A,my_struControl.TunCross1G_MODE_A); 
			GetCtrlVal(TabPanel_TunCross1G,TunCross1G_STR_DAC_SET,my_struControl.TunCross1G_DAC_SET); 
			GetCtrlVal(TabPanel_TunCross1G,TunCross1G_RING_LUT_NAME,&my_struControl.TunCross1G_LUT_NAME_INDEX); 
			GetCtrlVal(TabPanel_TunCross1G,TunCross1G_NUM_LUK_INDEX,&my_struControl.TunCross1G_LUK_INDEX);
			GetCtrlVal(TabPanel_TunCross1G,TunCross1G_NUM_SLOPE_AMP,&my_struControl.TunCross1G_LUK_SLOPE_AMP);  
			GetCtrlVal(TabPanel_TunCross1G,TunCross1G_NUM_OFFSET_AMP,&my_struControl.TunCross1G_LUK_OFFSET_AMP);
			GetCtrlVal(TabPanel_TunCross1G,TunCross1G_STR_SAVE,my_struControl.TunCross1G_SAVE); 
			
			//TunCross 10G  
			GetCtrlVal(TabPanel_TunCross10G,TunCross1G_STR_MODE_M,my_struControl.TunCross10G_MODE_M); 
			GetCtrlVal(TabPanel_TunCross10G,TunCross1G_STR_MODE_A,my_struControl.TunCross10G_MODE_A); 
			GetCtrlVal(TabPanel_TunCross10G,TunCross1G_STR_DAC_SET,my_struControl.TunCross10G_DAC_SET); 
			GetCtrlVal(TabPanel_TunCross10G,TunCross1G_RING_LUT_NAME,&my_struControl.TunCross10G_LUT_NAME_INDEX); 
			GetCtrlVal(TabPanel_TunCross10G,TunCross1G_NUM_LUK_INDEX,&my_struControl.TunCross10G_LUK_INDEX);
			GetCtrlVal(TabPanel_TunCross10G,TunCross1G_NUM_SLOPE_AMP,&my_struControl.TunCross10G_LUK_SLOPE_AMP);  
			GetCtrlVal(TabPanel_TunCross10G,TunCross1G_NUM_OFFSET_AMP,&my_struControl.TunCross10G_LUK_OFFSET_AMP);
			GetCtrlVal(TabPanel_TunCross10G,TunCross1G_STR_SAVE,my_struControl.TunCross10G_SAVE);
			
			//TxCal 1G   
			GetCtrlVal(TabPanel_TxCal1G,TxCal1G_STR_MODE_E,my_struControl.TxCal1G_MODE_E); 
			GetCtrlVal(TabPanel_TxCal1G,TxCal1G_STR_MODE_F,my_struControl.TxCal1G_MODE_F);
			GetCtrlVal(TabPanel_TxCal1G,TxCal1G_STR_DAC_GET,my_struControl.TxCal1G_ADC_GET);   
			GetCtrlVal(TabPanel_TxCal1G,TxCal1G_STR_ThreAddr_Min,my_struControl.TxCal1G_ADD_MIN);   
			GetCtrlVal(TabPanel_TxCal1G,TxCal1G_STR_ThreAddr_Max,my_struControl.TxCal1G_ADD_MAX);
			GetCtrlVal(TabPanel_TxCal1G,TxCal1G_STR_Slope_Addr, my_struControl.TxCal1G_SLOPE_ADD); 
			GetCtrlVal(TabPanel_TxCal1G,TxCal1G_NUM_SLOPE_AMP,&my_struControl.TxCal1G_SLOPE_AMP);  
			GetCtrlVal(TabPanel_TxCal1G,TxCal1G_NUM_OFFSET_AMP,&my_struControl.TxCal1G_OFFSET_AMP);  
			
			//TxCal 10G   
			GetCtrlVal(TabPanel_TxCal10G,TxCal10G_STR_MODE_E,my_struControl.TxCal10G_MODE_E); 
			GetCtrlVal(TabPanel_TxCal10G,TxCal10G_STR_MODE_F,my_struControl.TxCal10G_MODE_F);
			GetCtrlVal(TabPanel_TxCal10G,TxCal10G_STR_DAC_GET,my_struControl.TxCal10G_ADC_GET);   
			GetCtrlVal(TabPanel_TxCal10G,TxCal10G_STR_ThreAddr_Min,my_struControl.TxCal10G_ADD_MIN);   
			GetCtrlVal(TabPanel_TxCal10G,TxCal10G_STR_ThreAddr_Max,my_struControl.TxCal10G_ADD_MAX); 
			GetCtrlVal(TabPanel_TxCal10G,TxCal10G_STR_Slope_Addr, my_struControl.TxCal10G_SLOPE_ADD);
			GetCtrlVal(TabPanel_TxCal10G,TxCal10G_NUM_SLOPE_AMP,&my_struControl.TxCal10G_SLOPE_AMP);  
			GetCtrlVal(TabPanel_TxCal10G,TxCal10G_NUM_OFFSET_AMP,&my_struControl.TxCal10G_OFFSET_AMP);
			
			//TunAPD 		    
			GetCtrlVal(TabPanel_TunAPD,TunAPD_STR_MODE_M,my_struControl.TunAPD_MODE_M); 
			GetCtrlVal(TabPanel_TunAPD,TunAPD_STR_MODE_A,my_struControl.TunAPD_MODE_A); 
			GetCtrlVal(TabPanel_TunAPD,TunAPD_STR_DAC_SET,my_struControl.TunAPD_DAC_SET); 
			GetCtrlVal(TabPanel_TunAPD,TunAPD_STR_DAC_SET_RANGE,my_struControl.TunAPD_DAC_SET_RANGE); 
			GetCtrlVal(TabPanel_TunAPD,TunAPD_RING_LUT_NAME,&my_struControl.TunAPD_LUT_NAME_INDEX); 
			GetCtrlVal(TabPanel_TunAPD,TunAPD_NUM_LUK_INDEX,&my_struControl.TunAPD_LUK_INDEX);
			GetCtrlVal(TabPanel_TunAPD,TunAPD_NUM_SLOPE_AMP,&my_struControl.TunAPD_LUK_SLOPE_AMP);  
			GetCtrlVal(TabPanel_TunAPD,TunAPD_NUM_OFFSET_AMP,&my_struControl.TunAPD_LUK_OFFSET_AMP);
			GetCtrlVal(TabPanel_TunAPD,TunAPD_STR_SAVE,my_struControl.TunAPD_SAVE); 
			
			//LOS
			GetCtrlVal(TabPanel_TunSD,TunSD_STR_MODE_M,my_struControl.TunSD_MODE_M); 
			GetCtrlVal(TabPanel_TunSD,TunSD_STR_MODE_A,my_struControl.TunSD_MODE_A); 
			GetCtrlVal(TabPanel_TunSD,TunSD_STR_DAC_SET,my_struControl.TunSD_DAC_SET); 
			GetCtrlVal(TabPanel_TunSD,TunSD_STR_FUN_OFF,my_struControl.TunSD_FUN_OFF);  
			GetCtrlVal(TabPanel_TunSD,TunSD_STR_FUN_ON,my_struControl.TunSD_FUN_ON); 
			GetCtrlVal(TabPanel_TunSD,TunSD_RING_LUT_NAME,&my_struControl.TunSD_LUT_NAME_INDEX); 
			GetCtrlVal(TabPanel_TunSD,TunSD_NUM_LUK_INDEX,&my_struControl.TunSD_LUK_INDEX);
			GetCtrlVal(TabPanel_TunSD,TunSD_NUM_SLOPE_AMP,&my_struControl.TunSD_LUK_SLOPE_AMP);  
			GetCtrlVal(TabPanel_TunSD,TunSD_NUM_OFFSET_AMP,&my_struControl.TunSD_LUK_OFFSET_AMP);
			GetCtrlVal(TabPanel_TunSD,TunSD_STR_SAVE,my_struControl.TunSD_SAVE);   
			
			//TunSD
			GetCtrlVal(TabPanel_TunSD,TunSD_STR_MODE_M,my_struControl.TunSD_MODE_M); 
			GetCtrlVal(TabPanel_TunSD,TunSD_STR_MODE_A,my_struControl.TunSD_MODE_A); 
			GetCtrlVal(TabPanel_TunSD,TunSD_STR_DAC_SET,my_struControl.TunSD_DAC_SET); 
			GetCtrlVal(TabPanel_TunSD,TunSD_RING_LUT_NAME,&my_struControl.TunSD_LUT_NAME_INDEX); 
			GetCtrlVal(TabPanel_TunSD,TunSD_NUM_LUK_INDEX,&my_struControl.TunSD_LUK_INDEX);
			GetCtrlVal(TabPanel_TunSD,TunSD_NUM_SLOPE_AMP,&my_struControl.TunSD_LUK_SLOPE_AMP);  
			GetCtrlVal(TabPanel_TunSD,TunSD_NUM_OFFSET_AMP,&my_struControl.TunSD_LUK_OFFSET_AMP);
			GetCtrlVal(TabPanel_TunSD,TunSD_STR_SAVE,my_struControl.TunSD_SAVE); 
			
			//RxCal			    
			GetCtrlVal(TabPanel_RxCal,RxCal_STR_MODE_E,my_struControl.RxCal_MODE_E); 
			GetCtrlVal(TabPanel_RxCal,RxCal_STR_MODE_F,my_struControl.RxCal_MODE_F); 
			GetCtrlVal(TabPanel_RxCal,RxCal_STR_MODE_T,my_struControl.RxCal_MODE_T);  
			GetCtrlVal(TabPanel_RxCal,RxCal_STR_ADC_GET,my_struControl.RxCal_ADC_GET); 
			GetCtrlVal(TabPanel_RxCal,RxCal_RING_LUT_NAME,&my_struControl.RxCal_LUT_NAME_INDEX); 
			GetCtrlVal(TabPanel_RxCal,RxCal_NUM_LUK_INDEX,&my_struControl.RxCal_LUK_INDEX);
			GetCtrlVal(TabPanel_RxCal,RxCal_NUM_SLOPE_AMP,&my_struControl.RxCal_LUK_SLOPE_AMP);  
			GetCtrlVal(TabPanel_RxCal,RxCal_NUM_OFFSET_AMP,&my_struControl.RxCal_LUK_OFFSET_AMP);
			GetCtrlVal(TabPanel_RxCal,RxCal_STR_SAVE,my_struControl.RxCal_SAVE); 
			GetCtrlVal(TabPanel_RxCal,RxCal_STR_SET_RSSI_KL,my_struControl.RxCal_SET_KL);     
			GetCtrlVal(TabPanel_RxCal,RxCal_STR_SET_RSSI_KH,my_struControl.RxCal_SET_KH);
			GetCtrlVal(TabPanel_RxCal,RxCal_NUM_RSSI_K_AMP,&my_struControl.RxCal_RSSI_K_AMP);
			GetCtrlVal(TabPanel_RxCal,RxCal_STR_RSSI_TEMP_POINT,my_struControl.RxCal_SET_TEMP_PIONT);     
			
			
			SetControl();   
		//	HidePanel (panCtr);
			break;
	}
	return 0;
}

void Set_Control_Table(void)
{
	int index;
	
	GetPanelHandleFromTabPage (panCtr, PAN_CTR_TAB, 0, &TabPanel_Init);
	GetPanelHandleFromTabPage (panCtr, PAN_CTR_TAB, 1, &TabPanel_GetSN); 
	GetPanelHandleFromTabPage (panCtr, PAN_CTR_TAB, 2, &TabPanel_GetTemper); 
	GetPanelHandleFromTabPage (panCtr, PAN_CTR_TAB, 3, &TabPanel_TunAOP1G); 			  
	GetPanelHandleFromTabPage (panCtr, PAN_CTR_TAB, 4, &TabPanel_TunAOP10G);
	GetPanelHandleFromTabPage (panCtr, PAN_CTR_TAB, 5, &TabPanel_TunER1G);
	GetPanelHandleFromTabPage (panCtr, PAN_CTR_TAB, 6, &TabPanel_TunER10G); 
	GetPanelHandleFromTabPage (panCtr, PAN_CTR_TAB, 7, &TabPanel_TunCross1G); 
	GetPanelHandleFromTabPage (panCtr, PAN_CTR_TAB, 8, &TabPanel_TunCross10G); 
	GetPanelHandleFromTabPage (panCtr, PAN_CTR_TAB, 9, &TabPanel_TxCal1G);  
	GetPanelHandleFromTabPage (panCtr, PAN_CTR_TAB, 10, &TabPanel_TxCal10G);
	GetPanelHandleFromTabPage (panCtr, PAN_CTR_TAB, 11, &TabPanel_TunAPD);
	GetPanelHandleFromTabPage (panCtr, PAN_CTR_TAB, 12, &TabPanel_TunSD); 
	GetPanelHandleFromTabPage (panCtr, PAN_CTR_TAB, 13, &TabPanel_RxCal); 
	GetPanelHandleFromTabPage (panCtr, PAN_CTR_TAB, 14, &TabPanel_Other); 
				
	//Set ControlTable  
	//InitModule
	Set_RingCtrl(TabPanel_Init,InitModule_RING_I2cAddr,my_struControl.Init_I2cAddr);
	Set_RingCtrl(TabPanel_Init,InitModule_RING_PassWord,my_struControl.Init_PowerWord_Index); 
	SetCtrlVal(TabPanel_Init,InitModule_STR_GetVersion,my_struControl.Init_GetVersion);
	
	//GetBosaSN  
	Set_RingCtrl(TabPanel_GetSN,GetBosaSN_RING_I2cAddr,my_struControl.GetSN_I2cAddr); 
	Set_RingCtrl(TabPanel_GetSN,GetBosaSN_RING_Table,my_struControl.GetSN_Table);  
	
	//GetTemper   
	SetCtrlVal(TabPanel_GetTemper,TestTemper_STR_GetTempCase,my_struControl.GetTemper_CaseCommd);   
	SetCtrlVal(TabPanel_GetTemper,TestTemper_STR_GetTempCore,my_struControl.GetTemper_CoreCommd);  
	SetCtrlVal(TabPanel_GetTemper,TestTemper_NUM_GetTempOffset,my_struControl.GetTemper_IndexOffset); 
	
	//TunAOP 1G
	SetCtrlVal(TabPanel_TunAOP1G,TunAOP1G_STR_MODE_M,my_struControl.TunAOP1G_MODE_M); 
	SetCtrlVal(TabPanel_TunAOP1G,TunAOP1G_STR_MODE_A,my_struControl.TunAOP1G_MODE_A); 
	SetCtrlVal(TabPanel_TunAOP1G,TunAOP1G_STR_DAC_SET,my_struControl.TunAOP1G_DAC_SET); 
	Set_RingCtrl(TabPanel_TunAOP1G,TunAOP1G_RING_LUT_NAME,my_struControl.TunAOP1G_LUT_NAME_INDEX);  
	SetCtrlVal(TabPanel_TunAOP1G,TunAOP1G_NUM_LUK_INDEX,my_struControl.TunAOP1G_LUK_INDEX); 
	SetCtrlVal(TabPanel_TunAOP1G,TunAOP1G_NUM_SLOPE_AMP,my_struControl.TunAOP1G_LUK_SLOPE_AMP); 
	SetCtrlVal(TabPanel_TunAOP1G,TunAOP1G_NUM_OFFSET_AMP,my_struControl.TunAOP1G_LUK_OFFSET_AMP); 
	SetCtrlVal(TabPanel_TunAOP1G,TunAOP1G_STR_SAVE,my_struControl.TunAOP1G_SAVE); 
	
	//TunAOP 10G
	SetCtrlVal(TabPanel_TunAOP10G,TunAOP10G_STR_MODE_M,my_struControl.TunAOP10G_MODE_M); 
	SetCtrlVal(TabPanel_TunAOP10G,TunAOP10G_STR_MODE_A,my_struControl.TunAOP10G_MODE_A); 
	SetCtrlVal(TabPanel_TunAOP10G,TunAOP10G_STR_DAC_SET,my_struControl.TunAOP10G_DAC_SET); 
	Set_RingCtrl(TabPanel_TunAOP10G,TunAOP10G_RING_LUT_NAME,my_struControl.TunAOP10G_LUT_NAME_INDEX);  
	SetCtrlVal(TabPanel_TunAOP10G,TunAOP10G_NUM_LUK_INDEX,my_struControl.TunAOP10G_LUK_INDEX); 
	SetCtrlVal(TabPanel_TunAOP10G,TunAOP10G_NUM_SLOPE_AMP,my_struControl.TunAOP10G_LUK_SLOPE_AMP); 
	SetCtrlVal(TabPanel_TunAOP10G,TunAOP10G_NUM_OFFSET_AMP,my_struControl.TunAOP10G_LUK_OFFSET_AMP); 
	SetCtrlVal(TabPanel_TunAOP10G,TunAOP10G_STR_SAVE,my_struControl.TunAOP10G_SAVE); 
	
	//TunER 1G
	SetCtrlVal(TabPanel_TunER1G,TunER1G_STR_MODE_M,my_struControl.TunER1G_MODE_M); 
	SetCtrlVal(TabPanel_TunER1G,TunER1G_STR_MODE_A,my_struControl.TunER1G_MODE_A); 
	SetCtrlVal(TabPanel_TunER1G,TunER1G_STR_DAC_SET,my_struControl.TunER1G_DAC_SET); 
	Set_RingCtrl(TabPanel_TunER1G,TunER1G_RING_LUT_NAME,my_struControl.TunER1G_LUT_NAME_INDEX);  
	SetCtrlVal(TabPanel_TunER1G,TunER1G_NUM_LUK_INDEX,my_struControl.TunER1G_LUK_INDEX);
	SetCtrlVal(TabPanel_TunER1G,TunER1G_NUM_SLOPE_AMP,my_struControl.TunER1G_LUK_SLOPE_AMP); 
	SetCtrlVal(TabPanel_TunER1G,TunER1G_NUM_OFFSET_AMP,my_struControl.TunER1G_LUK_OFFSET_AMP); 
	SetCtrlVal(TabPanel_TunER1G,TunER1G_STR_SAVE,my_struControl.TunER1G_SAVE); 
	
	//TunER 10G
	SetCtrlVal(TabPanel_TunER10G,TunER10G_STR_MODE_M,my_struControl.TunER10G_MODE_M); 
	SetCtrlVal(TabPanel_TunER10G,TunER10G_STR_MODE_A,my_struControl.TunER10G_MODE_A); 
	SetCtrlVal(TabPanel_TunER10G,TunER10G_STR_DAC_SET,my_struControl.TunER10G_DAC_SET); 
	Set_RingCtrl(TabPanel_TunER10G,TunER10G_RING_LUT_NAME,my_struControl.TunER10G_LUT_NAME_INDEX);  
	SetCtrlVal(TabPanel_TunER10G,TunER10G_NUM_LUK_INDEX,my_struControl.TunER10G_LUK_INDEX); 
	SetCtrlVal(TabPanel_TunER1G,TunER10G_NUM_SLOPE_AMP,my_struControl.TunER10G_LUK_SLOPE_AMP); 
	SetCtrlVal(TabPanel_TunER1G,TunER10G_NUM_OFFSET_AMP,my_struControl.TunER10G_LUK_OFFSET_AMP); 
	SetCtrlVal(TabPanel_TunER10G,TunER10G_STR_SAVE,my_struControl.TunER10G_SAVE); 
	
	//TunCross 1G
	SetCtrlVal(TabPanel_TunCross1G,TunCross1G_STR_MODE_M,my_struControl.TunCross1G_MODE_M); 
	SetCtrlVal(TabPanel_TunCross1G,TunCross1G_STR_MODE_A,my_struControl.TunCross1G_MODE_A); 
	SetCtrlVal(TabPanel_TunCross1G,TunCross1G_STR_DAC_SET,my_struControl.TunCross1G_DAC_SET); 
	Set_RingCtrl(TabPanel_TunCross1G,TunCross1G_RING_LUT_NAME,my_struControl.TunCross1G_LUT_NAME_INDEX);  
	SetCtrlVal(TabPanel_TunCross1G,TunCross1G_NUM_LUK_INDEX,my_struControl.TunCross1G_LUK_INDEX);
	SetCtrlVal(TabPanel_TunCross1G,TunCross1G_NUM_SLOPE_AMP,my_struControl.TunCross1G_LUK_SLOPE_AMP); 
	SetCtrlVal(TabPanel_TunCross1G,TunCross1G_NUM_OFFSET_AMP,my_struControl.TunCross1G_LUK_OFFSET_AMP); 
	SetCtrlVal(TabPanel_TunCross1G,TunCross1G_STR_SAVE,my_struControl.TunCross1G_SAVE); 
	
	//TunCross 10G
	SetCtrlVal(TabPanel_TunCross10G,TunCros10G_STR_MODE_M,my_struControl.TunCross10G_MODE_M); 
	SetCtrlVal(TabPanel_TunCross10G,TunCros10G_STR_MODE_A,my_struControl.TunCross10G_MODE_A); 
	SetCtrlVal(TabPanel_TunCross10G,TunCros10G_STR_DAC_SET,my_struControl.TunCross10G_DAC_SET); 
	Set_RingCtrl(TabPanel_TunCross10G,TunCros10G_RING_LUT_NAME,my_struControl.TunCross10G_LUT_NAME_INDEX);  
	SetCtrlVal(TabPanel_TunCross10G,TunCros10G_NUM_LUK_INDEX,my_struControl.TunCross10G_LUK_INDEX); 
	SetCtrlVal(TabPanel_TunCross1G,TunCros10G_NUM_SLOPE_AMP,my_struControl.TunCross10G_LUK_SLOPE_AMP); 
	SetCtrlVal(TabPanel_TunCross1G,TunCros10G_NUM_OFFSET_AMP,my_struControl.TunCross10G_LUK_OFFSET_AMP); 
	SetCtrlVal(TabPanel_TunCross10G,TunCros10G_STR_SAVE,my_struControl.TunCross10G_SAVE); 
	
	//TxCal 1G   
	SetCtrlVal(TabPanel_TxCal1G,TxCal1G_STR_MODE_E,my_struControl.TxCal1G_MODE_E); 
	SetCtrlVal(TabPanel_TxCal1G,TxCal1G_STR_MODE_F,my_struControl.TxCal1G_MODE_F);
	SetCtrlVal(TabPanel_TxCal1G,TxCal1G_STR_DAC_GET,my_struControl.TxCal1G_ADC_GET);   
	SetCtrlVal(TabPanel_TxCal1G,TxCal1G_STR_ThreAddr_Min,my_struControl.TxCal1G_ADD_MIN);   
	SetCtrlVal(TabPanel_TxCal1G,TxCal1G_STR_ThreAddr_Max,my_struControl.TxCal1G_ADD_MAX); 
	SetCtrlVal(TabPanel_TxCal1G,TxCal1G_STR_Slope_Addr, my_struControl.TxCal1G_SLOPE_ADD); 
	SetCtrlVal(TabPanel_TxCal1G,TxCal1G_NUM_SLOPE_AMP,my_struControl.TxCal1G_SLOPE_AMP);  
	SetCtrlVal(TabPanel_TxCal1G,TxCal1G_NUM_OFFSET_AMP,my_struControl.TxCal1G_OFFSET_AMP);
	
	//TxCal 10G   
	SetCtrlVal(TabPanel_TxCal10G,TxCal10G_STR_MODE_E,my_struControl.TxCal10G_MODE_E); 
	SetCtrlVal(TabPanel_TxCal10G,TxCal10G_STR_MODE_F,my_struControl.TxCal10G_MODE_F);
	SetCtrlVal(TabPanel_TxCal10G,TxCal10G_STR_DAC_GET,my_struControl.TxCal10G_ADC_GET);   
	SetCtrlVal(TabPanel_TxCal10G,TxCal10G_STR_ThreAddr_Min,my_struControl.TxCal10G_ADD_MIN);   
	SetCtrlVal(TabPanel_TxCal10G,TxCal10G_STR_ThreAddr_Max,my_struControl.TxCal10G_ADD_MAX); 
	SetCtrlVal(TabPanel_TxCal10G,TxCal10G_STR_Slope_Addr, my_struControl.TxCal10G_SLOPE_ADD); 
	SetCtrlVal(TabPanel_TxCal10G,TxCal10G_NUM_SLOPE_AMP,my_struControl.TxCal10G_SLOPE_AMP);  
	SetCtrlVal(TabPanel_TxCal10G,TxCal10G_NUM_OFFSET_AMP,my_struControl.TxCal10G_OFFSET_AMP);
	
	//TunAPD 		    
	SetCtrlVal(TabPanel_TunAPD,TunAPD_STR_MODE_M,my_struControl.TunAPD_MODE_M); 
	SetCtrlVal(TabPanel_TunAPD,TunAPD_STR_MODE_A,my_struControl.TunAPD_MODE_A); 
	SetCtrlVal(TabPanel_TunAPD,TunAPD_STR_DAC_SET,my_struControl.TunAPD_DAC_SET); 
	SetCtrlVal(TabPanel_TunAPD,TunAPD_STR_DAC_SET_RANGE,my_struControl.TunAPD_DAC_SET_RANGE); 
	SetCtrlVal(TabPanel_TunAPD,TunAPD_RING_LUT_NAME,my_struControl.TunAPD_LUT_NAME_INDEX); 
	SetCtrlVal(TabPanel_TunAPD,TunAPD_NUM_LUK_INDEX,my_struControl.TunAPD_LUK_INDEX);
	SetCtrlVal(TabPanel_TunAPD,TunAPD_NUM_SLOPE_AMP,my_struControl.TunAPD_LUK_SLOPE_AMP);  
	SetCtrlVal(TabPanel_TunAPD,TunAPD_NUM_OFFSET_AMP,my_struControl.TunAPD_LUK_OFFSET_AMP);
	SetCtrlVal(TabPanel_TunAPD,TunAPD_STR_SAVE,my_struControl.TunAPD_SAVE); 
	
	//LOS
	SetCtrlVal(TabPanel_TunSD,TunSD_STR_MODE_M,my_struControl.TunSD_MODE_M); 
	SetCtrlVal(TabPanel_TunSD,TunSD_STR_MODE_A,my_struControl.TunSD_MODE_A); 
	SetCtrlVal(TabPanel_TunSD,TunSD_STR_DAC_SET,my_struControl.TunSD_DAC_SET); 
	SetCtrlVal(TabPanel_TunSD,TunSD_STR_FUN_OFF,my_struControl.TunSD_FUN_OFF);  
	SetCtrlVal(TabPanel_TunSD,TunSD_STR_FUN_ON,my_struControl.TunSD_FUN_ON); 
	SetCtrlVal(TabPanel_TunSD,TunSD_RING_LUT_NAME,my_struControl.TunSD_LUT_NAME_INDEX); 
	SetCtrlVal(TabPanel_TunSD,TunSD_NUM_LUK_INDEX,my_struControl.TunSD_LUK_INDEX);
	SetCtrlVal(TabPanel_TunSD,TunSD_NUM_SLOPE_AMP,my_struControl.TunSD_LUK_SLOPE_AMP);  
	SetCtrlVal(TabPanel_TunSD,TunSD_NUM_OFFSET_AMP,my_struControl.TunSD_LUK_OFFSET_AMP);
	SetCtrlVal(TabPanel_TunSD,TunSD_STR_SAVE,my_struControl.TunSD_SAVE);  
	
	//RxCal			    
	SetCtrlVal(TabPanel_RxCal,RxCal_STR_MODE_E,my_struControl.RxCal_MODE_E); 
	SetCtrlVal(TabPanel_RxCal,RxCal_STR_MODE_F,my_struControl.RxCal_MODE_F); 
	SetCtrlVal(TabPanel_RxCal,RxCal_STR_MODE_T,my_struControl.RxCal_MODE_T);  
	SetCtrlVal(TabPanel_RxCal,RxCal_STR_ADC_GET,my_struControl.RxCal_ADC_GET); 
	SetCtrlVal(TabPanel_RxCal,RxCal_RING_LUT_NAME,my_struControl.RxCal_LUT_NAME_INDEX); 
	SetCtrlVal(TabPanel_RxCal,RxCal_NUM_LUK_INDEX,my_struControl.RxCal_LUK_INDEX);
	SetCtrlVal(TabPanel_RxCal,RxCal_NUM_SLOPE_AMP,my_struControl.RxCal_LUK_SLOPE_AMP);  
	SetCtrlVal(TabPanel_RxCal,RxCal_NUM_OFFSET_AMP,my_struControl.RxCal_LUK_OFFSET_AMP);
	SetCtrlVal(TabPanel_RxCal,RxCal_STR_SAVE,my_struControl.RxCal_SAVE); 
	SetCtrlVal(TabPanel_RxCal,RxCal_STR_SET_RSSI_KL,my_struControl.RxCal_SET_KL);     
	SetCtrlVal(TabPanel_RxCal,RxCal_STR_SET_RSSI_KH,my_struControl.RxCal_SET_KH);
	SetCtrlVal(TabPanel_RxCal,RxCal_NUM_RSSI_K_AMP,my_struControl.RxCal_RSSI_K_AMP);
	SetCtrlVal(TabPanel_RxCal,RxCal_STR_RSSI_TEMP_POINT,my_struControl.RxCal_SET_TEMP_PIONT); 
	
}

void Set_RingCtrl(int tabPanel,int SubPanel,int value)
{
	int itemIndex;
					
	GetIndexFromValue (tabPanel, SubPanel,&itemIndex,value); 
	if(itemIndex<0)
	{
		MessagePopup("提示","未找到相应控件Value!");
	}
	 SetCtrlIndex (tabPanel,SubPanel,itemIndex);
	
}

int CVICALLBACK Quit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			HidePanel (panel);  
			break;
	}
	return 0;
}
int SearchApcState(int handle)
{
//	// 默认锁定在0通道
//	int temp = 0;
//	int error = 0;
//	char strOpt[256] = {0};
//	char *paraState = "close";

//	error = SetCommand(handle,"gn2010_get_cfg(loop)",strOpt);
//	if(error != 0)
//	{
//		return -1;
//	}
//	sscanf(strOpt,"0x%x", &temp);
//	memmset(strOpt,0,sizeof(strOpt));
//	if(/*判断temp的返回值*/)
//	{
//		// AOP当前是直通状态，则打开功率闭环控制系统
//		error = SetCommand(handle,"gn2010_set_cfg(loop, close)",strOpt);
//		Assert(error == 0);
//	}
	return 0;
}
int ReadingAop(int handle,int *aopVlue)
{
//	int error = 0;
//	int valueTemp;
//	char strOupt[255] = {0};
//	error = SetCommand (handle, "gn2010_read(108,1)", strOupt);
//	assert(error == 0);
//	sscanf(strOupt,"0x%x",&valueTemp);
//	*aopVlue = valueTemp;
	return 0;
}
int SettingingAop(int handle, int DacValue)
{
//	int error = 0;
//	int valueTemp;
//	char strOupt[255] = {0};
//	char strInput[255] = {0};
//	sprintf(strInput, "gn2010_write(89,0x%02X)", DacValue);  
//	SetCommand(handle,strInput, strOupt);
//	if(0 != strcmp(strOupt, "OK"))
//	{
//		MessagePopup ("ERROR", "输入命令出错!     "); 
//		return -1;
//	}
	return 0;
}
int AdjustWaveLen(int handle, int *waveLen)
{
//	BOOL temperChged = FALSE;
//	//assert(dataTmp != NULL);
//	if(temperChged == FALSE)
//	{
//		temperChged = TRUE;
//		// 下发默认温度
//		
//	}
	return 0;
}

int AdjustAop(int handle, int setDac, int *aopVlue)
{
//	assert(aopVlue != NULL);
//	// 先设置一个默认值
//	SettingingAop(0,0xD0);
//	do
//	{
//		ReadingAop(0, aopVlue);
//		if(/*AopVlue在合法范围内*/)
//		{
//			break;
//		}
//		/*调整DAC值*/
//		SettingingAop(0,setDac);
//		count++;
//	}while(count < 12)
	return 0;
}

void CVICALLBACK Callback_Test1_170612 (int menuBar, int menuItem, void *callbackData,
		int panel)
{
//	int AopVlue = 0;
//	int setDac = 0;
//	int count = 0;
//	BOOL Status = FALSE;
//	struTestData dataTmp;
//	// 使用string.h头文件
//	bzero(&dataTmp,sizeof(dataTmp));
//	// 查询APC工作状态
//	// 默认锁定在0通道
//	SearchApcState(0);

//	// 调整光功率AOP
//	AdjustAop(0, setDac, &AopVlue);

//	// 调节ER
//	Status = tuningER10G(panMain, 0, dataTmp);

//	// 调节波长
//	AdjustWaveLen				   DT_DWDP_TEST     my_struConfig.DT_QATest_10G
	if (my_struConfig.DT_QATest_10G)
	{
		my_struConfig.DT_QATest_10G = FALSE;
		my_struConfig.DT_DWDP_TEST = TRUE;
	}
}

void CVICALLBACK Callback_SearchData_Chan0 (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	// 显示窗口
	DisplayPanel(panSearchData_chan0);
	Initial_Measure(panSearchData_chan0);
}

void CVICALLBACK Callback_SearchData_Chan1 (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	//HidePanel(panSearchData_chan0);
	double  CoreNormalTemper = 0;
	char aaa[] = "0000000000002";
	GetDBNormalCoreTemper(&CoreNormalTemper, aaa); 
}

void CVICALLBACK On_Calibrate_R_Lock_Line (int menuBar, int menuItem, void *callbackData,
		int panel)
{
		int error;
	int channel;
	int progresshandle;
	BOOL timeOutFlag;
	
	errChk(GetCaliConfigFile ());
	
	DisplayPanel (panCaliR_Lock);
	
	progresshandle = CreateProgressDialog ("设备配置状态", "设备配置进度", 1, VAL_NO_INNER_MARKERS, "");
	SetCtrlAttribute(panCaliR_Lock,PAN_RCALL_BUT_OK_TDP,ATTR_VISIBLE ,1);
	SetCtrlAttribute(panCaliR_Lock,PAN_RCALL_BUT_OK,ATTR_VISIBLE ,0);
	

	
	//for BERT
	for (channel=0;channel<CHANNEL_MAX;channel++)
	{
		UpdateProgressDialog (progresshandle, (int)(100.*channel/CHANNEL_MAX), 0);
		
		if (my_struInstrument[channel].ChannelEn)
		{
			//for BERT	
			if(my_struConfig.DT_Test_1G || my_struConfig.DT_QATest_1G)
			{
				error = Init_BERT (channel, -1);
				if (error) 
				{
					return;  
				}
			
			}
			else
			{
				error = Init_10GBERT (channel);
				if (error) 
				{
					return;
				}
			}
			
				//切换光路
			my_struInstrument[channel].SW_TYPE = 4;
			if (SW_TYPE_NONE != my_struInstrument[channel].SW_TYPE)
			{
				//切换成一通道
				my_struInstrument[channel].SW_CHAN = 1;
				Fiber_SW_SetChannel(my_struInstrument[channel].SW_TYPE, my_struInstrument[channel].SW, my_struInstrument[channel].SW_Handle, my_struInstrument[channel].SW_CHAN);
			}
			
			SetTableCellAttribute (panCaliR_Lock, PAN_RCALL_TABLE, MakePoint (1, channel+1), ATTR_CELL_DIMMED, 0);
			
			DB_get_cali (CALI_TYPE_RX_MAINLOCK, channel, &timeOutFlag);
			if (timeOutFlag)
			{
				SetTableCellVal (panCaliR_Lock, PAN_RCALL_TABLE, MakePoint (1, channel+1), 0.00);
			}
			else
			{
				SetTableCellVal (panCaliR_Lock, PAN_RCALL_TABLE, MakePoint (1, channel+1), sRxCal_Lock.power_in[channel]-sRxCal_Lock.power[channel]); 
			}
			
			errChk (Set_Att_Lock_Enable (channel));   
			
			errChk(Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, sRxCal_Lock.power_in[channel]));
		}
		else
		{
			SetTableCellAttribute (panCaliR_Lock, PAN_RCALL_TABLE, MakePoint (1, channel+1), ATTR_CELL_DIMMED, 1);	
		}
	}
	
Error:
	
	DiscardProgressDialog (progresshandle);
}

void CVICALLBACK On_Calibrate_R_Free_Line (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	int error;
	int channel;
	int progresshandle;
	BOOL timeOutFlag;
	
	errChk(GetCaliConfigFile ());
	
	DisplayPanel (panCaliR_Free);
	
	progresshandle = CreateProgressDialog ("设备配置状态", "设备配置进度", 1, VAL_NO_INNER_MARKERS, "");
	SetCtrlAttribute(panCaliR_Free,PAN_RCAL_F_BUT_OK_TDP,ATTR_VISIBLE ,1);
	SetCtrlAttribute(panCaliR_Free,PAN_RCAL_F_BUT_OK,ATTR_VISIBLE ,0);
	

	
	//for BERT
	for (channel=0;channel<CHANNEL_MAX;channel++)
	{
		UpdateProgressDialog (progresshandle, (int)(100.*channel/CHANNEL_MAX), 0);
		
		if (my_struInstrument[channel].ChannelEn)
		{
			//for BERT	
			if(my_struConfig.DT_Test_1G || my_struConfig.DT_QATest_1G)
			{
				error = Init_BERT (channel, -1);
				if (error) 
				{
					return;  
				}
			
			}
			else
			{
				error = Init_10GBERT (channel);
				if (error) 
				{
					return;
				}
			}
			
				//切换光路
			my_struInstrument[channel].SW_TYPE = 4;
			if (SW_TYPE_NONE != my_struInstrument[channel].SW_TYPE)
			{
				//切换成一通道
				my_struInstrument[channel].SW_CHAN = 1;
				Fiber_SW_SetChannel(my_struInstrument[channel].SW_TYPE, my_struInstrument[channel].SW, my_struInstrument[channel].SW_Handle, my_struInstrument[channel].SW_CHAN);
			}
			
			SetTableCellAttribute (panCaliR_Free, PAN_RCALL_TABLE, MakePoint (1, channel+1), ATTR_CELL_DIMMED, 0);
			
			DB_get_cali (CALI_TYPE_RX_MAINFREE, channel, &timeOutFlag);
			if (timeOutFlag)
			{
				SetTableCellVal (panCaliR_Free, PAN_RCALL_TABLE, MakePoint (1, channel+1), 0.00);
			}
			else
			{
				SetTableCellVal (panCaliR_Free, PAN_RCALL_TABLE, MakePoint (1, channel+1), sRxCal_Free.power_in[channel]-sRxCal_Free.power[channel]); 
			}
			
			errChk (Set_Att_Lock_Disable (channel));   
			
			errChk (Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN, sRxCal_Free.power_in[channel]));
		}
		else
		{
			SetTableCellAttribute (panCaliR_Free, PAN_RCALL_TABLE, MakePoint (1, channel+1), ATTR_CELL_DIMMED, 1);	
		}
	}
	
Error:
	
	DiscardProgressDialog (progresshandle);
}
