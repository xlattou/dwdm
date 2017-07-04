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
static int panOrder;	//�湤��¼����� 
static int panPopu;		//�˳�����ȷ�϶Ի��� 
static int panConf;     //��������
static int panCaliR_Lock;		//����ģʽ����·У׼����
static int panCaliR_Free;		//˥��ģʽ����·У׼���� 
static int panAbout;
static int panMenu; 
static int panAdva;    //�߼����ܽ��� 
static int panCtr;     //����������� 
static int panSearchData_chan0; // ͨ��0�������ݽ���

//�����������Table    
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


//���̱߳���
static int ghThreadLocalVar;	//���̱߳��ر���
static int gFlag;				//���߳�������־
static unsigned int gIndexVal[CHANNEL_MAX]     = {0, 1, 2, 3, 4, 5, 6, 7}; //���̺߳�������ֵ,ʵ�ʴ�����ǲ���ͨ��ֵ
static int 			gThreadFuncId[CHANNEL_MAX] = {0, 0, 0, 0, 0, 0, 0, 0}; //���߳�ID
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
		strcpy (Info, "���������...");
		Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		Status = testER10G(channel, data);
		if (Status) {sprintf (Info, "���������=%.2fdB �ɹ�", data->ER);}
		else		{sprintf (Info, "���������=%.2fdB ʧ��", data->ER);bOK=FALSE;data->ErrorCode=ERR_TES_ER;error=-1;} 	  
		Insert_Info(panMain, gCtrl_BOX[channel], Info); 
	}
	//����
	if (bOK)
	{
		strcpy (Info, "���Թ���...");
		Insert_Info(panMain, gCtrl_BOX[channel], Info); 				
		Status = testSpectrum(channel, data); 
		if (Status) {sprintf (Info, "���Թ��ף��ɹ� ���Ĳ���=%.2f  ����=%.2f  SMSR=%.2f", data->PeakWL,  data->Bandwidth, data->SMSR);}
		else		{sprintf (Info, "���Թ��ף�ʧ�� ���Ĳ���=%.2f  ����=%.2f  SMSR=%.2f", data->PeakWL,  data->Bandwidth, data->SMSR);bOK=FALSE;data->ErrorCode=ERR_TES_TEST_SPECTRUM;error=-1;}	   
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
		//�л�ʱ��
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
			sprintf (Info, "ͨ��%d ʾ�����ͺ����ô���    ", channel); 
			MessagePopup("Error", Info);
			error=-1;
			goto Error;
		}
		
		if (my_struConfig.DT_Tun_DWDM)
		{
			// �ȵ���DWDM
			// ���Բ���
			if (bOK && (error==0))
			{
				//����Ĭ��tec�¶�ֵ
				SET_10G_DWDM(INST_EVB[channel], TecValue);
				strcpy (Info, "���Բ���...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
				Status_WL = testSpectrum(channel, data);
				if (Status_WL == FALSE)
				{
					Status_WL = tunSpectrum_WL_DWDM(channel,data, &TecValue);
				}
				// �ص��⹦��
				Status_AOP = tuningAOP10G(channel, data);
				// ����ټ��һ��Ⲩ��
				Status_WL = testSpectrum(channel, data);
				Status_WL = tunSpectrum_WL_DWDM(channel,data, &TecValue);
				Status = Status_WL && Status_AOP;
				data->Bandwidth = data->Bandwidth * 1000000000;
				if (Status) 
				{	// ���SMSR
					sprintf (Info, "���Բ������ɹ� ���Ĳ���=%.2f  ����=%.2f  SMSR=%.2f", data->PeakWL,  data->Bandwidth, data->SMSR);
				}
				else
				{
					if (Status_WL == TRUE && (Status_AOP == FALSE))
					{
						sprintf (Info, "���Բ������ɹ����⹦��: ʧ��");
						bOK=FALSE;
						error=-1;
					}
					if (Status_WL == FALSE)
					{
						sprintf (Info, "���Բ�����ʧ�� ���Ĳ���=%.2f  ����=%.2f  SMSR=%.2f", data->PeakWL,  data->Bandwidth, data->SMSR);
						bOK=FALSE;
						error=-1;
						
					}
				}
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
				
			}
			
			// ���������
			//ER  
			if (bOK && my_struConfig.Sel_T_ER && (error==0))
			{	
			 	strcpy (Info, "���������...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info);          
				Status = tuningER10G(panMain, channel, data);
				if (Status) {sprintf (Info, "���������=%.2fdB;�⹦��=%.2fdB�ɹ�,ModDac=0x%X", data->ER,data->AOP,data->DACMod);}
				else	  	
				{
				 	sprintf (Info, "���������=%.2fdB ʧ��;�⹦��=%.2fdB�ɹ�,ModDac=0x%X", data->ER,data->AOP,data->DACMod);bOK=FALSE;data->ErrorCode=ERR_TUN_ER;error=-1;    
					Insert_Info(panMain, gCtrl_BOX[channel], Info); 
					if(data->AOP_CheckFlag)
					{
						bOK=TRUE;data->ErrorCode=0;error=0;
						strcpy (Info, "�ص��Թ⹦��...");
						Insert_Info(panMain, gCtrl_BOX[channel], Info);          
						Status = tuningAOP10G_R(channel, data);
						if (Status) {sprintf (Info, "�ص��Թ⹦��=%.2fdBm �ɹ�;Ibias=%.2fmA,DAC=0x%X", data->AOP,data->A2_Ibias,data->DACApc);}
						else	    {sprintf (Info, "�ص��Թ⹦��=%.2fdBm ʧ��;Ibias=%.2fmA,DAC=0x%X", data->AOP,data->A2_Ibias,data->DACApc);error=-1;data->ErrorCode=ERR_TUN_AOP;}       
						Insert_Info(panMain, gCtrl_BOX[channel], Info); 
					
						if(0 == error)
						{
						 	strcpy (Info, "���������...");
							Insert_Info(panMain, gCtrl_BOX[channel], Info);          
							Status = tuningER10G(panMain, channel, data);
							if (Status) {sprintf (Info, "�ص��������=%.2fdB;�⹦��=%.2fdB�ɹ�,ModDac=0x%X", data->ER,data->AOP,data->DACMod);}
							else        {sprintf (Info, "�ص��������=%.2fdB ʧ��;�⹦��=%.2fdB�ɹ�,ModDac=0x%X", data->ER,data->AOP,data->DACMod);bOK=FALSE;data->ErrorCode=ERR_TUN_ER;error=-1; }
						}
					}
					else
					{
						sprintf (Info, "���������=%.2fdB ʧ��;�⹦��=%.2fdB�ɹ�,ModDac=0x%X", data->ER,data->AOP,data->DACMod);bOK=FALSE;data->ErrorCode=ERR_TUN_ER;error=-1;
					}
				}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			} 
			
			//���Խ����
			if (bOK  && my_struConfig.Sel_T_Eye && (error==0))
			{
				strcpy (Info, "���Խ����...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info);
				SET_10G_DWDM_Cross(INST_EVB[channel]);
				Status = tuningCross_10G(channel, data);                   
				if (Status) {sprintf (Info, "���Խ����=%.2f �ɹ�", data->TxCross);}  
				else	  	{sprintf (Info, "���Խ����=%.2f ʧ��", data->TxCross);bOK=FALSE;error=-1;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info);  
			}
					
			if (bOK && (error==0))
			{
			    // ����һ�£��⹦�ʣ������������
			    strcpy (Info, "����⹦�ʡ������������...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info);
				SET_10G_DWDM(INST_EVB[channel], m_WLDac_normal);
				Status_ER = tuningER10G(panMain, channel, data);
				// �ص��⹦��
				//Status_AOP = tuningAOP10G(channel, data);
				Status_AOP = testDWDMAOP10G(channel, data);
				// ����ټ��һ��Ⲩ��
				Status_WL = testSpectrum(channel, data); 
				//Status_WL = tunSpectrum_WL_DWDM(channel,data, &TecValue);
				Status = Status_WL && Status_AOP && Status_ER;
				// ��ȡ�������Ĳ���DAC �ͺ���
				(void)ReadNormalTemper(channel, data);
				if (Status) {sprintf (Info, "����⹦�ʡ�����������ȣ��ɹ�");}
				else	    {sprintf (Info, "����⹦�ʡ�����������ȣ�ʧ��");bOK=FALSE;error=-1;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			}
			
			//������ͼ
		 	if (bOK && my_struConfig.Sel_T_Eye && (error==0)) 
			{
				strcpy (Info, "���Է�����ͼ...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info);          
				Status = testOEye(channel, data);
				if (Status) {sprintf (Info, "���Է�����ͼ���ɹ�");}
				else	    {sprintf (Info, "���Է�����ͼ��ʧ��");bOK=FALSE;data->ErrorCode=ERR_TUN_TEST_O_EYE;error=-1;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			}
			
			//test mask
			if (bOK && my_struConfig.Sel_T_Mask && (error==0)) 
			{
				strcpy (Info, "���Է���ģ��...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info);          
				Status = testOEye10G_Mask(channel, data);
				if (Status) {sprintf (Info, "���Է���ģ�壺�ɹ�");}
				else	    {sprintf (Info, "���Է���ģ�壺ʧ��");bOK=FALSE;data->ErrorCode=ERR_TUN_TEST_O_MASK;error=-1;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			}
			
		}
		else if (my_struConfig.DT_Test_DWDM)
		{
			// �����DWDM
			// ER
			if (bOK && my_struConfig.Sel_T_ER) 
			{
			 	strcpy (Info, "���������...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
				Status = testER10G(channel, data);
				if (Status) {sprintf (Info, "���������=%.2fdB �ɹ�", data->ER);}
				else	    {sprintf (Info, "���������=%.2fdB ʧ��", data->ER);bOK=FALSE;data->ErrorCode=ERR_TES_ER;error=-1;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			}
			
			//test eye
		 	if (bOK && my_struConfig.Sel_T_Eye) 
			{
				strcpy (Info, "���Է�����ͼ...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
				Status = testOEye (channel, data);
				if (Status) {sprintf (Info, "���Է�����ͼ���ɹ�");}
				else	    {sprintf (Info, "���Է�����ͼ��ʧ��");bOK=FALSE;data->ErrorCode=ERR_TES_TEST_O_EYE;error=-1;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			}	

			//test mask
			if (bOK && my_struConfig.Sel_T_Mask) 
			{
				strcpy (Info, "���Է���ģ��...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
				Status = testOEye10G_Mask (channel, data);
				if (Status) {sprintf (Info, "���Է���ģ�壺�ɹ�");}
				else	    {sprintf (Info, "���Է���ģ�壺ʧ��");bOK=FALSE;data->ErrorCode=ERR_TES_TEST_O_MASK;error=-1;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			}
			
			//cross
			if (bOK && my_struConfig.Sel_T_Cross)
			{
				strcpy (Info, "���Խ����...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
				Status = testCross(channel, data);     
				if (Status) {sprintf (Info, "���Խ����=%.2f �ɹ�", data->TxCross);}
				else	    {sprintf (Info, "���Խ����=%.2f ʧ��", data->TxCross);bOK=FALSE;data->ErrorCode=ERR_TUN_AOP;error=-1;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			}

			//����
			if (bOK && my_struConfig.Sel_T_Spectrum)
			{
				strcpy (Info, "���Թ���...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 			    
				Status = testSpectrum(channel, data);
				data->Bandwidth = data->Bandwidth * 1000000000;
				if (Status) {sprintf (Info, "���Թ��ף��ɹ� ���Ĳ���=%.2f  ����=%.2f  SMSR=%.2f", data->PeakWL,  data->Bandwidth, data->SMSR);}
				else	  	{sprintf (Info, "���Թ��ף�ʧ�� ���Ĳ���=%.2f  ����=%.2f  SMSR=%.2f", data->PeakWL,  data->Bandwidth, data->SMSR);bOK=FALSE;data->ErrorCode=ERR_TES_TEST_SPECTRUM;error=-1;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			}
			
		}
		
Error:	// ע�� ֻ���ڴ˺������ڲ�ʹ��errChk��	
        CmtReleaseLock(ThreadLock);
    }
	else
	{
		Insert_Info(panel, control, "��ȡ�豸����Ȩʧ��    ");
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
		
	sprintf (version, "Combo PON OLT���в������\nEXP0000xxx\n%s/%s",SOFTVER,SOFTDATE);
    SetCtrlVal (panAbout, PAN_ABOUT_TEXTMSG, version); 
	
	//У׼������ؿؼ�ֵ
	for (index=0; index<CHANNEL_MAX; index++)
	{
		sprintf (buf, "%d", index);
  		SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (1, 3*index+1), buf); 
		SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (1, 3*index+2), buf);
		SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (1, 3*index+3), buf);  
	
		SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (2, 3*index+1), "���˹�·");
		SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (2, 3*index+2), "�ն˹�·����ģʽ");  
		SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (2, 3*index+3), "�ն˹�·˥��ģʽ"); 

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
	CmtNewLock("pss",0,&Thread_PSS_PowerPeter);	   //����PSS��ͨ���⹦�ʼ��л�  
    
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
			MessagePopup ("��ʾ", "������ֹ���ԣ�Ȼ�����˳�����");
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
		
			//�޸Ľ���ؼ�����
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
					//�򿪵�Դ
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

			//�޸Ľ���ؼ�����
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
		
			//У׼��־λ��λ
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
			
			
			//���²�Ʒ�汾 
			GetCtrlIndex (panel, PAN_CONF_RING_BOM, &my_struConfig.VerIndex);
			if(my_struConfig.VerIndex>=0)
			{
				GetLabelFromIndex (panel, PAN_CONF_RING_BOM, my_struConfig.VerIndex, my_struConfig.Ver);
			}
			else
			{
				strcpy (my_struConfig.Ver, "");
			}

			//���²�ƷEEPROM�汾
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
			
			/***��ȡPN����**Eric.Yao**20141023***/
			status = DB_GET_PN_TYPE(tempStr);
			if (status)
			{										  
				if (strcmp (tempStr, "XGSPON_OLT") ==0) 
				{
					my_struConfig.PN_TYPE=PN_TYPE_XGSPON_OLT;	
				}   
				else 
				{
					MessagePopup ("��ʾ", "���ݿ�AutoDT_Spec_Tracking�ж����PN_TYPE�д���"); 
					return -1;
				} 
			}
			
			//��ȡ����	 
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
			
			//��ȡ�����ǲ���
			if (my_struConfig.Sel_T_Spectrum)
			{
				Status = DB_GET_Spectrum();
				if(!Status) return -1;
			}	  
			
			GetCtrlAttribute (panel, PAN_CONF_RING_EED, ATTR_DIMMED, &dimmed);
			if (dimmed==0)	//��Ҫ��ȡEED��Ϣ
			{
				if (!DB_GET_EEPROM_Ex()) 
				{
					return -1;   
				}
			}
			
			if(!my_struConfig.Sel_Calibrate) 
			{
				if (!ConfirmPopup ("��ʾ", "��ȷ�ϲ���Ҫѡ��У׼���ܣ�"))
				{
					return -1;
				}
			}
			
			if(!my_struConfig.Sel_Calibrate_Test) 
			{
				if (!ConfirmPopup ("��ʾ", "��ȷ�ϲ���Ҫѡ��У׼���Թ��ܣ�"))
				{
					return -1;
				}
			}
			
			//RSSIģʽ�����ݿ�����þ��� �Լ�EX1�в�ѯ���¶ȵ�slope��offset
//			if (DB_READ_Spec_Tracking_Ex(&my_struConfig.RSSI_TYPE)<0)    //��ȡSpec_Tracking_Ex�� Spec_Tracking_Ex1���������
//			{
//				return -1; 		  
//			}
			
			//��ȡ��for Calibrate  
			if(my_struConfig.Sel_Calibrate_Test || my_struConfig.Sel_Calibrate)
			{
				status = DB_GET_Config_Monitor_Info ();
				if(status)
				{
					return -1;
				}
			
				//ֻ��ȡ��־λ=2��У׼�㣬
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
			
			//��ȡFirmware�汾����
			if ((my_struConfig.ChiefChip==CHIEFCHIP_ADUC7020) || (CHIEFCHIP_ADUC7023 == my_struConfig.ChiefChip))	 /***����7023��ȡ�̼�**Eric.Yao**20130909***/
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
				strcpy (testitem, "����10G"); 
			}
			else if (my_struConfig.DT_Test_10G)     
			{
				if (my_struConfig.Temper_Low)	
				{
					strcpy (testitem, "���²���10G"); 
				}
				else if (my_struConfig.Temper_High)
				{
					strcpy (testitem, "���²���10G"); 
				}
				else							
				{
					strcpy (testitem, "����10G"); 
				}
			}
			else  if(my_struConfig.DT_QATest_10G)							   		
			{
				if (my_struConfig.Temper_Low)	
				{
					strcpy (testitem, "����QA����10G"); 
				}
				else if (my_struConfig.Temper_High)
				{
					strcpy (testitem, "����QA����10G"); 
				}
				else							
				{
					strcpy (testitem, "QA����10G");
				}
			}
			else if (my_struConfig.DT_Test_1G)     
			{
				if (my_struConfig.Temper_Low)	
				{
					strcpy (testitem, "���²���1G"); 
				}
				else if (my_struConfig.Temper_High)
				{
					strcpy (testitem, "���²���1G"); 
				}
				else							
				{
					strcpy (testitem, "����1G"); 
				}
			}
			else  if(my_struConfig.DT_QATest_1G)							   		
			{
				if (my_struConfig.Temper_Low)	
				{
					strcpy (testitem, "����QA����1G"); 
				}
				else if (my_struConfig.Temper_High)
				{
					strcpy (testitem, "����QA����1G"); 
				}
				else							
				{
					strcpy (testitem, "QA����1G");
				}
			}
			else
			{
				strcpy (testitem, "δ֪����");   
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
			
			   
			//��ӳ�ʼ��ϵͳ����   
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
	
	//��ȡ�����ļ�
	GetConfig(); 
	//�����ļ���ŵ���Ϣ���ý����ϵ���ʾ��Ϣ
	//����PNIndex>PAN_CONF_RING_PN��Ŀ��bug
	GetNumListItems (panConf, PAN_CONF_RING_PN, &count);
	if (my_struConfig.PNIndex>(count-1)) 
	{
		my_struConfig.PNIndex=0;
	}
	
	SetCtrlIndex (panConf, PAN_CONF_RING_PN, my_struConfig.PNIndex); 
	// DWDM���Բ���
	SetCtrlVal (panConf, PAN_CONF_RAD_BUT_TUN_DWDM, my_struConfig.DT_Tun_DWDM);
	SetCtrlVal (panConf, PAN_CONF_RAD_BUT_TEST_DWDM, my_struConfig.DT_Test_DWDM);
	// �����¶�
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
				//��ʾ�߼��������ý���
				if (stricmp (my_struLicense.power, "admin")!=0) 		
				{
					MessagePopup("��ʾ","������˺�û��ʹ�ô˹��ܵ�Ȩ�ޣ�");
					return -1;
				} 
				
				DisplayPanel (panAdva);
				GetConfig(); 
			
				SetCtrlVal (panAdva, PAN_ADVA_CHE_NPI, my_struConfig.isNPI_flag);  
			}
			else if (License_Flag_panCALI==showphID) 
			{
				//��ʾУ׼���ý���
				if (stricmp (my_struLicense.power, "admin")!=0) 		
				{
					MessagePopup("��ʾ","������˺�û��ʹ�ô˹��ܵ�Ȩ�ޣ�");
					return -1;
				} 
				
				DisplayPanel (phCaliLimit); 
				
				//��ʾУ׼�ļ������Ϣ
			 	GetCaliConfigFile ();
				
				//�������ȶԻ���
				progresshandle = CreateProgressDialog ("����У׼��������", "���ݼ��ؽ���", 1, VAL_NO_INNER_MARKERS, "");
				
				for (channel=0; channel<CHANNEL_MAX; channel++)
				{
					UpdateProgressDialog (progresshandle, (int)(100.*channel/CHANNEL_MAX), 0); 
					
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (2, channel*3+1), "���˹�·");
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (3, channel*3+1), 0.); 
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (4, channel*3+1), my_struTxCal.power_min[channel]); 
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (5, channel*3+1), my_struTxCal.power_max[channel]); 
				
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (2, channel*3+2), "�ն˹�·����ģʽ");  
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (3, channel*3+2), sRxCal_Lock.power_in[channel]);  
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (4, channel*3+2), sRxCal_Lock.power_min[channel]); 
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (5, channel*3+2), sRxCal_Lock.power_max[channel]); 
					
					SetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (2, channel*3+3), "�ն˹�·˥��ģʽ");  
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
					my_struTxCal.flag[channel]=TRUE;    //�������Բ���У׼ 
					sRxCal_Lock.flag[channel] = FALSE;  
					sRxCal_Free.flag[channel] = FALSE;
				}
				
				//��ȡϵͳ����
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
			 	//��ʾ�����������ý���
				if (stricmp (my_struLicense.power, "admin")!=0) 		
				{
					MessagePopup("��ʾ","������˺�û��ʹ�ô˹��ܵ�Ȩ�ޣ�");
					return -1;
				} 
				
				DisplayPanel (panCtr);
				GetControl();  
				Set_Control_Table();  
				
			}
			else
			{
				MessagePopup("��ʾ","���������ʾ�߼��쳣    "); 
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
			{	 //��ǰ���������������
				panelCB (panMain,EVENT_CLOSE, 0,0,0); 
			}
			else if (License_Flag_INIT == showphID)
			{
				//��ǰ���滹û�д������棬ֱ���˳�
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
					 MessagePopup ("��ʾ", "��ͨ����Ч");
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
	strcat (filename, "\\�汾��ʷ.txt");
	
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
			error = DB_Read_Order (WorkOrder);			//MySQL�л���Oracle	wenyao.xi 2015-11-27   
			if (error) 	{Insert_Info(panMain, PAN_MAIN_TEXTBOX_CHAN0, "��ѯOA���ݿ��쳣");}  
			else    	{Insert_Info(panMain, PAN_MAIN_TEXTBOX_CHAN0, "��ѯOA���ݿ�ɹ�");} 
	
			//��������
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
		
			//���浽�����ļ�	 
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
								strcpy (Info, "�����԰��Ƿ���ģ������쳣");
								Insert_Info(panMain, gCtrl_BOX[channel], Info);  
								continue;
							}
							else if (!bMOD0)	  	
							{
								ResetTextBox (panMain, gCtrl_BOX[channel], "");      
								strcpy (Info, "�ȴ�����ģ��������ϵͳ...");
								Insert_Info(panMain, gCtrl_BOX[channel], Info);   
								continue;
							}
							else
							{

								if (my_struConfig.DT_Tun_DWDM)
								{   // ����DWDM
									// Ϊtables��ʾ�������һ��
									InsertTableRows(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, 1, 1, VAL_CELL_NUMERIC);
									error = CmtScheduleThreadPoolFunction (ThreadHandle, tun_DWDM_10G, &gIndexVal[channel], &gThreadFuncId[channel]); 
								}
								else if (my_struConfig.DT_Test_DWDM)
								{   // ����DWDM
									InsertTableRows(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, 1, 1, VAL_CELL_NUMERIC);
									error = CmtScheduleThreadPoolFunction (ThreadHandle, test_DWDM_10G, &gIndexVal[channel], &gThreadFuncId[channel]); 
								}
								else
								{
									MessagePopup ("��ʾ", "��ǰ����֧�ִ˲��Թ���");
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
	
	progresshandle = CreateProgressDialog ("�豸����״̬", "�豸���ý���", 1, VAL_NO_INNER_MARKERS, "");
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
	
	progresshandle = CreateProgressDialog ("�豸����״̬", "�豸���ý���", 1, VAL_NO_INNER_MARKERS, "");
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
				// д��У׼ֵ-15
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
	{MessagePopup("��ʾ", "û��ѡ�񷢶˹⹦�ʲ��ԣ��ɲ����з���У׼��");return;} 
		
	DisplayPanel (phCaliT); 

	//���У׼�����ʾ
	FillTableCellRange (phCaliT, PAN_CALI_T_TABLE, MakeRect (1, 2, 24, 2), val);
	
	//�������ȶԻ���
	progresshandle = CreateProgressDialog ("�豸����״̬", "�豸���ý���", 1, VAL_NO_INNER_MARKERS, "");
	
	for (channel=0; channel<CHANNEL_MAX; channel++)
	{
		UpdateProgressDialog (progresshandle, (int)(100.*channel/CHANNEL_MAX), 0); 
	
		if (my_struInstrument[channel].ChannelEn)
		{
			SetTableCellRangeAttribute (phCaliT, PAN_CALI_T_TABLE, MakeRect (channel*3+1, 2, 3, 3), ATTR_CELL_DIMMED, 0); 
			errChk(SET_EVB_SHDN(channel, 1));
			
			//����У׼��¼
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
			
			//�⹦�ʼ�
			GetCtrlVal (panel, PAN_INST_RING_PM_TYPE, &localInst.Power_TYPE);
			GetCtrlVal (panel, PAN_INST_NUM_PM, &localInst.PMSII);
			GetCtrlVal (panel, PAN_INST_STR_SN_PM, localInst.SN_PM); 

			//˥����
			GetCtrlVal (panel, PAN_INST_RING_ATT_MAI_TYPE, &localInst.ATT_TYPE_MAIN); 
			GetCtrlVal (panel, PAN_INST_NUM_ATT,&localInst.ATT_MAIN_COM);       
			GetCtrlIndex (panel, PAN_INST_RING_GVPM_MAI, &index); 
			if(index>=0) {GetLabelFromIndex (panel, PAN_INST_RING_GVPM_MAI, index, localInst.SN_ATT_MAIN);}  
			else         {strcpy (localInst.SN_ATT_MAIN, "");}
			
			//�ο���·˥����
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
			
			//�⿪��
			GetCtrlVal (panel, PAN_INST_RING_SW_TYPE, &localInst.SW_TYPE); 
			GetCtrlVal (panel, PAN_INST_NUM_SW, &localInst.SW); 
			GetCtrlVal (panel, PAN_INST_NUM_SW_CHAN, &localInst.SW_CHAN);   
			GetCtrlIndex (panel, PAN_INST_RING_FSW, &index); 
			if(index>=0) {GetLabelFromIndex (panel, PAN_INST_RING_FSW, index, localInst.SW_SN);}  
			else         {strcpy (localInst.SW_SN, "");}   
			
			//ʱ���л���
			GetCtrlVal (panel, PAN_INST_RING_CLOCK_TYPE, &localInst.CLOCK_TYPE); 
			GetCtrlIndex (panel, PAN_INST_RING_CLOCK, &index); 
			if(index>=0) {GetLabelFromIndex (panel, PAN_INST_RING_CLOCK, index, localInst.CLOCK);}  
			else         {strcpy (localInst.CLOCK, "");}
			GetCtrlVal (panel, PAN_INST_NUM_CLOCK_CHAN, &localInst.CLOCK_CHAN);
			
			//��ӵ��豸�б�
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
				{   //ѡ���˸���
					GetLabelFromIndex (panel, PAN_INST_TREE, eventData2, label1); 
					GetTreeItemAttribute (panel, PAN_INST_TREE, eventData2, ATTR_IMAGE_INDEX, &ImageIndex);
					ParentIndex = eventData2;
				}
				else
				{   //ѡ��������
					GetLabelFromIndex (panel, PAN_INST_TREE, ParentIndex, label1);  
					GetTreeItemAttribute (panel, PAN_INST_TREE, ParentIndex, ATTR_IMAGE_INDEX, &ImageIndex);
				}

				sscanf (label1,"ͨ��%d",&channel);
	            SetCtrlVal (panel, PAN_INST_NUM_CHANNEL, channel);
				
				//��ʾ�豸�����Ϣ��ע���ʱ�ṹ��INSTR����Ҫ��ֵ������Ҫ��֤֮ǰ�������й�����GetConfig_Inst 
				if (TREE_IMAGE_OK==ImageIndex || TREE_IMAGE_WARING==ImageIndex)
				{
					SetCtrlVal (panel, PAN_INST_BIN_CHAN_EN, 1);	
				}
				else
				{
					SetCtrlVal (panel, PAN_INST_BIN_CHAN_EN, 0);
				}
				
				//��ʾ�⹦�ʼ���Ϣ
				SetCtrlVal (panel, PAN_INST_RING_PM_TYPE, my_struInstrument[channel].Power_TYPE); 
				SetCtrlVal (panel, PAN_INST_NUM_PM, my_struInstrument[channel].PMSII);  
				SetCtrlVal (panel, PAN_INST_STR_SN_PM, my_struInstrument[channel].SN_PM); 

				//˥����
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
					MessagePopup("��ʾ", "Initial CSA8000 error!");
					return -1;
				}  
			
				if (!CSA8000_GET_MainSN(inst_CSA8000, my_struCSA8000.MainSN)) 
				{
					MessagePopup("��ʾ", "��ȡCSA8000�������кŴ���"); 
					return -1;
				} 
				
				SetCtrlVal (panel, PAN_INST_STR_SN_DCA, my_struCSA8000.MainSN);	 
			}
			else if (index == DCA_TYPE_Ag86100)
			{
				if (!Ag86100_Init(&inst_Ag86100, dac_add,my_struAg86100.Channel_O)) 
				{
					MessagePopup("��ʾ", "Initial Ag86100 error!"); 
					return -1;
				} 
			
				if (!Ag86100_GET_MainSN(inst_Ag86100, my_struAg86100.MainSN)) 
				{
					MessagePopup("��ʾ", "��ȡAg86100�������кŴ���");
					return -1;
				} 
				
				SetCtrlVal (panel, PAN_INST_STR_SN_DCA, my_struAg86100.MainSN);
			}
			else 
			{
				MessagePopup("��ʾ", "Can not find this DCA!"); 
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
			
			//�����������豸�б�
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
			
			//�����豸���õ��洢�ļ�
			for (channel=0; channel<CHANNEL_MAX; channel++)
			{
				//ͨ����������union��������ֵ0���ٴ������Խ���洢�ṹ��
				memset (uInst.pStr, 0, sizeof (uInst.pStr));
				my_struInstrument[channel] = uInst.sStr; 

				sprintf (label, "ͨ��%d", channel);
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
					//��������б������ҪУ׼
					if (0!= stricmp(sn, my_struInstrument[channel].Fiber)) 
					{
						//�����ˣ�ǿ��У׼�ն˺ͷ��˹�·
						sRxCal_Lock.flag[channel] = FALSE;  
						sRxCal_Free.flag[channel] = FALSE;
						my_struTxCal.flag[channel]=FALSE; 
					}
					strcpy (my_struInstrument[channel].Fiber, sn);
					
					//�⿪��
					GetLabelFromIndex (panel, PAN_INST_TREE, ++index, label);
					sscanf (label, LAB_SW, &my_struInstrument[channel].SW_TYPE, &my_struInstrument[channel].SW, &my_struInstrument[channel].SW_CHAN, my_struInstrument[channel].SW_SN);
		
					//ʱ���л���
					GetLabelFromIndex (panel, PAN_INST_TREE, ++index, label);
					sscanf (label, LAB_CLOCK, &my_struInstrument[channel].CLOCK_TYPE, my_struInstrument[channel].CLOCK, &my_struInstrument[channel].CLOCK_CHAN);
				}
			}
		
			//д�������ļ�
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
					//�ж�У׼�����Ƿ�����
					GetTableCellVal (panCaliR_Lock, PAN_RCALL_TABLE, MakePoint (1, channel+1), &temp_cal);
					
					temp_cal= sRxCal_Lock.power_in[channel] - temp_cal;
					
					if(temp_cal>sRxCal_Lock.power_max[channel] || temp_cal<sRxCal_Lock.power_min[channel])
					{
						MessagePopup("��ʾ", "�ն�У׼�⹦�ʳ������÷�Χ��"); 
						return -1;
					}
			
					if(control==PAN_RCALL_BUT_OK)
					{
						//�����ն�У׼ֵ
						sRxCal_Lock.power[channel] = temp_cal; 
						sRxCal_Lock.power_array[channel*4+3] = temp_cal;
						
						//update Flag
						sRxCal_Lock.flag[channel]=TRUE;
					}
					else
					{
						//�����ն�У׼ֵ
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
			
			//��У׼�������浽�ļ�
			err = SetCaliConfigFile ();
			if (err) 
			{
				return -1;
			}
			
			//�˳�У׼����
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
					//�ж�У׼�����Ƿ�����
					GetTableCellVal (panCaliR_Free, PAN_RCALL_TABLE, MakePoint (1, channel+1), &temp_cal);
					
					temp_cal= sRxCal_Free.power_in[channel] - temp_cal;
					
					if(temp_cal>sRxCal_Free.power_max[channel] || temp_cal<sRxCal_Free.power_min[channel])
					{
						MessagePopup("��ʾ", "�ն�У׼�⹦�ʳ������÷�Χ��"); 
						return -1;
					}
			
					if(control==PAN_RCAL_F_BUT_OK)
					{
						
						//�����ն�У׼ֵ
						sRxCal_Free.power[channel] = temp_cal; 
						sRxCal_Free.power_array[channel*4+3] = temp_cal;
			
						//update Flag
						sRxCal_Free.flag[channel]=TRUE;
					}
					else
					{
							//�����ն�У׼ֵ
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
			
			//��У׼�������浽�ļ�
			err = SetCaliConfigFile ();
			if (err) 
			{
				return -1;
			}
			
			//�˳�У׼����
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
	//�������
	SetCtrlVal (panMain, PAN_MAIN_STR_BATCH, ""); 
	
	if (0==stricmp (my_struConfig.PN, ""))
	{
		MessagePopup("��ʾ", "����ѡ���Ʒ�ͺ�    "); 
		return;
	}
	
	InstallPopup (panOrder); 
	
	SetCtrlVal (panOrder, PAN_ORDER_TEXTMSG, "���ڲ�ѯOAϵͳ�����Ե�...    "); 
	SetWaitCursor (1); 
//	DBOA_Read_pch (my_struConfig.PN, my_struConfig.Ver, panOrder, PAN_ORDER_COMBO_ORDER);
	DB_Read_pch (my_struConfig.PN, my_struConfig.Ver, panOrder, PAN_ORDER_COMBO_ORDER);		   //MySQL�л���Oracle	wenyao.xi 2015-11-27     
	SetWaitCursor (0); 
	SetCtrlVal (panOrder, PAN_ORDER_TEXTMSG, "��ѯOAϵͳ����    ");  
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
		bMOD0 = TRUE;   //������԰��Ƿ���ģ���ʾ
		ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		
		SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_YELLOW);
		SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		
		//����·У׼�͵���Ƿ����
		if (!sRxCal_Lock.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			errChk(DB_get_cali (CALI_TYPE_RX_MAINLOCK, channel, &timeOutFlag));
		}
		
		if (!sRxCal_Free.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			errChk(DB_get_cali (CALI_TYPE_RX_MAINFREE, channel, &timeOutFlag));
		}
		
		//��ʼ����
	    ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		timestart=Timer();
		strcpy (Info, "��ʼ���в���...");
		Insert_Info(panMain, gCtrl_BOX[channel], Info);  
		
		// �⿪������Ϊ2ͨ��
		my_struInstrument[channel].SW_TYPE = SW_TYPE_COFFSW02;
		if (SW_TYPE_NONE != my_struInstrument[channel].SW_TYPE)
		{
			//�л���һͨ��
			my_struInstrument[channel].SW_CHAN = 2;
			Fiber_SW_SetChannel(my_struInstrument[channel].SW_TYPE, my_struInstrument[channel].SW, my_struInstrument[channel].SW_Handle, my_struInstrument[channel].SW_CHAN);
		}
		
		//��ʼ������ģ��
		if (bOK)
		{
			strcpy (Info, "��ʼ��ģ��...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			Status = InitModule(channel, &data, &prolog); 
			if (Status) {strcpy (Info, "��ʼ��ģ�飺�ɹ�");}
			else	  	{strcpy (Info, "��ʼ��ģ�飺ʧ��");bOK=FALSE;}                      
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
		}
		
		//get bosasn
		if (bOK)
		{
			strcpy (Info, "��ȡ����...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			Status = GetBosaSN(channel, &data);
			if (Status) {sprintf (Info, "��ȡ����=%s �ɹ�", data.OpticsSN);SaveData=TRUE;}
			else	    {sprintf (Info, "��ȡ����=%s ʧ��", data.OpticsSN);bOK=FALSE;data.ErrorCode=ERR_TUN_READ_INNERSN;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			strcpy (prolog.SN, data.OpticsSN);
		}		    

		//test temperature
		if (bOK)
		{
			strcpy (Info, "����ģ���¶�...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			Status = testTemper(panMain, channel, &data);
			if (Status) {sprintf (Info, "����ģ���¶�=%.2f�� �ɹ�", data.Temperatrue);}  
			else	  	{sprintf (Info, "����ģ���¶�=%.2f�� ʧ��", data.Temperatrue);bOK=FALSE;data.ErrorCode=ERR_TUN_TEST_TEMPER;SaveData=FALSE;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
		}
		
		//test station
		if (bOK && !my_struConfig.isNPI_flag)
		{
			strcpy (Info, "���Թ���...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
		//	Status = teststation(data, prolog);
			if (Status) {sprintf (Info, "���Թ��򣺳ɹ�");}  
			else	  	{sprintf (Info, "���Թ���ʧ��");bOK=FALSE;data.ErrorCode=ERR_TUN_TEST_STATION;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
		}
			
		//��ѡ��ʾ����ʱѡ�ô˷���
		//AOP
		if (bOK && my_struConfig.Sel_T_AOP)
		{	
			strcpy (Info, "���Թ⹦��...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			//����Ĭ��tec�¶�ֵ
			(void)SET_10G_DWDM(INST_EVB[channel], 0xF900);
			// ����Ĭ��EA
			(void)SET_10G_DWDM_EA(INST_EVB[channel], 0x0300);
			// ����Ĭ��ER
			//(void)SET_10G_DWDM_ER(INST_EVB[channel], 0x0168);
			Status = tuningAOP10G(channel, &data);
			if (Status) {sprintf (Info, "���Թ⹦��=%.2fdBm �ɹ�;Ibias=%.2fmA,DAC=0x%X", data.AOP,data.A2_Ibias,data.DACApc);}
			else	    {sprintf (Info, "���Թ⹦��=%.2fdBm ʧ��;Ibias=%.2fmA,DAC=0x%X", data.AOP,data.A2_Ibias,data.DACApc);bOK=FALSE;data.ErrorCode=ERR_TUN_AOP;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
		}
		
		// er ��ͼ mask ���׵�
		if (bOK)
		{
			strcpy (Info, "�л���·...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			error = testParameterLock(channel, &data, panMain, gCtrl_BOX[channel], Info, ERInfo);
			if (error){bOK=FALSE;}
		}
	
		//calTxPower
 	if (bOK && my_struConfig.Sel_Calibrate)
		{	
		 	strcpy (Info, "����У׼...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			Status = calTxPower10G_Ex(channel, &data);
			if (Status) {sprintf (Info, "����У׼���ɹ�,���˼��=%.2fdBm",data.A2_TxMon);}
			else	  	{sprintf (Info, "����У׼��ʧ��,���˼��=%.2fdBm",data.A2_TxMon);bOK=FALSE;data.ErrorCode=ERR_TUN_CAL_TXPOWER;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
		}
			
		//test tx disable power
		if (bOK )  
		{
			strcpy (Info, "���Է���Ӳ���ضϹ⹦��..."); 
			Insert_Info(panMain, gCtrl_BOX[channel], Info);     
			error = testTxDis_10G(channel, &data);
			if (error) 	{sprintf (Info, "���Է���Ӳ���ضϹ⹦��=%.2fdBm ʧ��", data.TxOffPower);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_TXDIS;} 
			else		{sprintf (Info, "���Է���Ӳ���ضϹ⹦��=%.2fdBm �ɹ�", data.TxOffPower);} 
			Insert_Info(panMain, gCtrl_BOX[channel], Info);     
		}
		
		//�����Ȳ���
		if (bOK && my_struConfig.Sel_R_Sens && !my_struConfig.Sel_R_Sens_Real) 
		{
			strcpy (Info, "����������...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);      
			Status = testTheSens(channel, &data);
			if (Status) {sprintf (Info, "����������=%.2fdBm �ɹ�", data.Sens);}
			else	    {sprintf (Info, "����������=%.2fdBm ʧ��", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);      
		}

		//����������
		if (bOK && my_struConfig.Sel_R_Sens) 	 
		{
			strcpy (Info, "�л���·...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			strcpy (Info, "���Դ���������...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);         
			Status = testTheLoadSens(channel, &data);
			if (Status) {sprintf (Info, "���Դ���������=%.2fdBm �ɹ�", data.Sens);}
			else	    {sprintf (Info, "���Դ���������=%.2fdBm ʧ��", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);         
		}
		
		if (bOK && my_struConfig.Sel_R_SDHys)
		{
			strcpy (Info, "�����ն�LOS...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);     
			Status = testRxLOS(channel, &data);
			if (Status) {sprintf (Info, "�����ն�LOS���ɹ�");}  
			else	  	{sprintf (Info, "�����ն�LOS��ʧ��");bOK=FALSE;data.ErrorCode=ERR_TES_SD;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);     
		}

		if(!bOK && my_struConfig.Sel_R_SDHys &&  (ERR_TES_SD == data.ErrorCode))
		{
		
			strcpy (Info, "�����ն�LOS...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = tuningSD1(channel, &data);  
			if (Status) {sprintf (Info, "�����ն�LOS���ɹ�;LOS_DAC=0x%X",data.ucRsd);data.ErrorCode=0;bOK=TRUE;}  
			else	  	{sprintf (Info, "�����ն�LOS��ʧ��");bOK=FALSE;data.ErrorCode=ERR_TUN_TEST_LOS;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		
			if (bOK && my_struConfig.Sel_R_SDHys)
			{
				strcpy (Info, "�����ն�LOS...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
				Status = testRxLOS(channel, &data);
				if (Status) {sprintf (Info, "�����ն�LOS���ɹ�");}  
				else	  	{sprintf (Info, "�����ն�LOS��ʧ��");bOK=FALSE;data.ErrorCode=ERR_TES_SD;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			}
		}
		
		//test over ���ز���ʱ��̣�����ǰ�浥������
		if (bOK && my_struConfig.Sel_R_Over)
		{
			strcpy (Info, "���Թ���...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			Status = testFixOver_10G(channel, &data);
			if (Status) {sprintf (Info, "���Թ���=%.2fdBm �ɹ�", data.Over);}
			else	  	{sprintf (Info, "���Թ���=%.2fdBm ʧ��", data.Over);bOK=FALSE;data.ErrorCode=ERR_TUN_TEST_OVER;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
		}			  
	
		
	
		if (bOK && my_struConfig.Sel_R_Sens && my_struConfig.Sel_R_Sens_Real) 
		{
			strcpy (Info, "����������ֵ...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			error = testDetailSens_10G (channel, &data);
			if (error) {sprintf (Info, "�����Ȳ���=%.2fdBm ʧ��", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TUN_TEST_SENS;} 
			else	   {sprintf (Info, "�����Ȳ���=%.2fdBm �ɹ�", data.Sens);}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);          
		}
		  
		//�ն˼��У׼ 
		if (bOK && my_struConfig.Sel_Calibrate)
		{	
		 	strcpy (Info, "�ն�У׼...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
			Status = caltestRxPower_Ex (panMain, channel, &data);   
			if (Status) {sprintf (Info, "�ն�У׼���ɹ�");}
			else	  	{sprintf (Info, "�ն�У׼��ʧ��");bOK=FALSE;data.ErrorCode=ERR_TUN_CAL_RXPOWER;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
		}		   
	
		/****************��������*************************/
		
		//���Գɹ������ֵ
		strcpy (prolog.PN, my_struConfig.PN); 	//PN
		strcpy (prolog.SN, data.OpticsSN);		//SN			
		strcpy (prolog.Operator, g_Operator);//Operator		 
		prolog.ResultsID=0;    						//ResultsID
		//Log_Action  �ڲ��Ժ�����ʼλ�ø�ֵ   
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
			//-------------------��Log_Action��ֵ--------------------// 
			//������Խ��
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
		// ������д��tables
		WriteDateTables(panSearchData_chan0, data);
		//�����������
		if (SaveData)
		{
			strcpy (Info, "�����������...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
			Status = SaveDataBase(data, prolog);
			if (Status) {strcpy (Info, "����������ݣ��ɹ�");}
			else	    {strcpy (Info, "����������ݣ�ʧ��");bOK=FALSE;data.ErrorCode=ERR_TUN_SAVE_DATA;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
		}
		/**************************************************/
		
		//��ʾ���н��
		prolog.RunTime=Timer()-timestart; 
		if (bOK)	//�����Գɹ�����ʾ�������
		{
			sprintf (Info, "��������ʱ��=%ds��\n������ʹ�ô���=%d��", prolog.RunTime, data.FiberTime);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_ON_COLOR, VAL_GREEN);
			SetCtrlVal (panMain, gCtrl_LED[channel], 1);
		}
		else
		{
			sprintf (Info, "��������ʱ��=%ds��\n������ʹ�ô���=%d�Σ�\n�������=%d", prolog.RunTime, data.FiberTime, data.ErrorCode);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_RED);
			SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		} 
		Insert_Info(panMain, gCtrl_BOX[channel], Info);
		
#if DEBUG
		Display_PASS_Fail_Rate (DB_Get_Handle(), my_struConfig.batch, prolog.PN, my_struProcessLOG.StationID, prolog.Log_Action, panMain, PAN_MAIN_NUMERIC_PASS, PAN_MAIN_NUMERIC_FAIL, PAN_MAIN_NUMERIC_TOTAL, PAN_MAIN_NUMERIC_PASS_RATE);
#endif 
		
		//���ģ���Ƿ��ڲ��԰���
		if (bMOD0)
		{
			strcpy (Info, "�뽫����ģ��Ӳ���ϵͳȡ��...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			do 
			{
				error = checktestbed(channel, &bMOD0);
				if (error) 
				{
					strcpy (Info, "�����԰��Ƿ���ģ������쳣");
					Insert_Info(panMain, gCtrl_BOX[channel], Info);  
				}
				else if (!bMOD0)	  	
				{
					break;
				} 
				else
				{
					strcpy (Info, "�뽫����ģ��Ӳ���ϵͳȡ��...");
					Update_RESULTSInfo(panMain, gCtrl_BOX[channel], Info, FALSE);
				}
			} while (bMOD0 && gFlag);
			
			my_struInstrument[channel].ChannelStart = FALSE;
			m_adjustSpeed = FALSE;
		}	 
	}
	
Error:
	
	my_struInstrument[channel].ChannelStart = FALSE;
	
	strcpy (Info, "��ͨ��������ֹͣ");
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
		bMOD0 = TRUE;   //������԰��Ƿ���ģ���ʾ
		ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		
		SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_YELLOW);
		SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		
		//����·У׼�͵���Ƿ����
		if (!sRxCal_Lock.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			//errChk(DB_get_cali (CALI_TYPE_RX_MAINLOCK, channel, &timeOutFlag));
		}
		
		if (!sRxCal_Free.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			//errChk(DB_get_cali (CALI_TYPE_RX_MAINFREE, channel, &timeOutFlag));
		}
		
		//��ʼ����
	    ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		timestart=Timer();
		strcpy (Info, "��ʼ���в���...");
		Insert_Info(panMain, gCtrl_BOX[channel], Info);   
		
		//��ʼ������ģ��
		if (bOK)
		{
			strcpy (Info, "��ʼ��ģ��...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = InitModule(channel, &data, &prolog); 
			if (Status) {strcpy (Info, "��ʼ��ģ�飺�ɹ�");}
			else	  	{strcpy (Info, "��ʼ��ģ�飺ʧ��");bOK=FALSE;}                      
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		// �⿪������Ϊ2ͨ��
		my_struInstrument[channel].SW_TYPE = SW_TYPE_COFFSW02;
		if (SW_TYPE_NONE != my_struInstrument[channel].SW_TYPE)
		{
			//�л���һͨ��
			my_struInstrument[channel].SW_CHAN = 2;
			Fiber_SW_SetChannel(my_struInstrument[channel].SW_TYPE, my_struInstrument[channel].SW, my_struInstrument[channel].SW_Handle, my_struInstrument[channel].SW_CHAN);
		}
		
		//get bosasn
		if (bOK)
		{
			strcpy (Info, "��ȡ����...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = GetBosaSN(channel, &data);
			if (Status) {sprintf (Info, "��ȡ����=%s �ɹ�", data.OpticsSN);SaveData=TRUE;}
			else	    {sprintf (Info, "��ȡ����=%s ʧ��", data.OpticsSN);bOK=FALSE;data.ErrorCode=ERR_TES_READ_INNERSN;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			strcpy (prolog.SN, data.OpticsSN);
		}	
		
		//test temperature
		if (bOK)
		{
			strcpy (Info, "����ģ���¶�...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testTemper (panMain, channel, &data);
			// ��ȡ����DAC
			DACtemp = Get_normal_DAC_DWDM(channel);
			if (DACtemp == 0)
			{
				strcpy (Info, "����DAC��ȡʧ��");
				Insert_Info(panMain, gCtrl_BOX[channel], Info);
				bOK=FALSE;
			}
			if (Status) {sprintf (Info, "����ģ���¶�=%.2f�� �ɹ�", data.Temperatrue);}  
			else	  	{sprintf (Info, "����ģ���¶�=%.2f�� ʧ��", data.Temperatrue);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_TEMPER;SaveData=FALSE;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		//test station  	
		if (bOK && !my_struConfig.isNPI_flag)
		{
			strcpy (Info, "���Թ���...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = teststation(data, prolog);
			if (Status) {sprintf (Info, "���Թ��򣺳ɹ�");}  
			else	  	{sprintf (Info, "���Թ���ʧ��");bOK=FALSE;data.ErrorCode=ERR_TES_TEST_STATION;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		//AOP
		if (bOK && my_struConfig.Sel_T_AOP) 
		{
			strcpy (Info, "���Թ⹦��...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = caltestTxPower(channel, &data);  
			if (Status) {sprintf (Info, "���Թ⹦��=%.2fdBm �ɹ�;Ibias=%.2fmA", data.AOP,data.A2_Ibias);}
			else	    {sprintf (Info, "���Թ⹦��=%.2fdBm ʧ��;Ibias=%.2fmA", data.AOP,data.A2_Ibias);bOK=FALSE;data.ErrorCode=ERR_TES_AOP;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}   

		// ���²��Բ���
		if (bOK && my_struConfig.Temper_High)
		{
			strcpy (Info, "���²��Բ���...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			error = TestWL_TemperHigh(channel, &data, panMain, gCtrl_BOX[channel], Info, ERInfo);
			if (error)
			{
				sprintf (Info, "���Ը��²���=%.2f ʧ��;", data.PeakWL);
				Insert_Info(panMain, gCtrl_BOX[channel], Info);
				bOK=FALSE;
			} 
			else
			{
				sprintf (Info, "���Ը��²���=%.2f �ɹ�;", data.PeakWL);
				Insert_Info(panMain, gCtrl_BOX[channel], Info);
			}
		} 
		
		// er ��ͼ mask ���׵�
		if (bOK)
		{
			error = testParameterLock(channel, &data, panMain, gCtrl_BOX[channel], Info, ERInfo);
			if (error){bOK=FALSE;} 
		} 
	
		//cal TxPower
		if (my_struConfig.Sel_Calibrate_Test && bOK && my_struConfig.Sel_T_AOP)
		{
			strcpy (Info, "����У׼����...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = caltestTxPower(channel, &data);//  caltestTxPower10G
			if (Status) {sprintf (Info,"����У׼����=%.2fdBm �ɹ�", data.A2_TxMon);}
			else	    {sprintf (Info,"����У׼����=%.2fdBm ʧ��", data.A2_TxMon);bOK=FALSE;data.ErrorCode=ERR_TES_CAL_TXPOWER;}        
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
		}
		
		//test sens
		if (bOK && my_struConfig.Sel_R_Sens) 
		{
			strcpy (Info, "����������...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);         
			Status = testTheSens(channel, &data);
			if (Status) {sprintf (Info, "����������=%.2fdBm �ɹ�", data.Sens);}
			else	    {sprintf (Info, "����������=%.2fdBm ʧ��", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);         
		}

		//����������
		if (bOK && my_struConfig.Sel_R_Sens) 	 
		{
			strcpy (Info, "�л���·...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			strcpy (Info, "���Դ���������...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);         
			Status = testTheLoadSens(channel, &data);
			if (Status) {sprintf (Info, "���Դ���������=%.2fdBm �ɹ�", data.Sens);}
			else	    {sprintf (Info, "���Դ���������=%.2fdBm ʧ��", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);         
		}
		
		if (bOK && my_struConfig.Sel_R_SDHys)
		{
			strcpy (Info, "�����ն�LOS...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			Status = testRxLOS(channel, &data);
			if (Status) {sprintf (Info, "�����ն�LOS���ɹ�");}  
			else	  	{sprintf (Info, "�����ն�LOS��ʧ��");bOK=FALSE;data.ErrorCode=ERR_TUN_TEST_LOS;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
		}

		//test over
		if (bOK && my_struConfig.Sel_R_Over) 
		{
			strcpy (Info, "���Թ���...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);        
			Status = testFixOver_10G(channel, &data); 
			if (Status) {sprintf (Info, "���Թ���=%.2fdBm �ɹ�", data.Over);}
			else	    {sprintf (Info, "���Թ���=%.2fdBm ʧ��", data.Over);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_OVER;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);        
		}

		//cal RxPower  
			if (my_struConfig.Sel_Calibrate_Test && bOK)
			{
				strcpy (Info, "�ն˼�ز���...");
				Insert_Info(panMain, gCtrl_BOX[channel], Info);          
				Status = caltestRxPower_Ex (panMain, channel, &data);
				if (Status) {sprintf (Info, "�ն˼�ز��ԣ��ɹ�");}
				else	    {sprintf (Info, "�ն˼�ز��ԣ�ʧ��");bOK=FALSE;data.ErrorCode=ERR_TES_CAL_RXPOWER;}       
				Insert_Info(panMain, gCtrl_BOX[channel], Info);          
			}	

		/****************��������*************************/		   
	
		/****************��������*************************/
		// ������д��tables
		WriteDateTables(panSearchData_chan0, data);
		
		//���Գɹ������ֵ
		strcpy (prolog.PN, my_struConfig.PN); 	//PN
		strcpy (prolog.SN, data.OpticsSN);		//SN			
		strcpy (prolog.Operator, g_Operator);//Operator		 
		prolog.ResultsID=0;    						//ResultsID
		//Log_Action  �ڲ��Ժ�����ʼλ�ø�ֵ   
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
			//-------------------��Log_Action��ֵ--------------------//  
			//������Խ��
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
		
		//�����������
		if (SaveData)
		{
			strcpy (Info, "�����������...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
			Status = SaveDataBase(data, prolog);
			if (Status) {strcpy (Info, "����������ݣ��ɹ�");}
			else	    {strcpy (Info, "����������ݣ�ʧ��");bOK=FALSE;data.ErrorCode=ERR_TUN_SAVE_DATA;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
		}
		/**************************************************/
		
		//��ʾ���н��
		prolog.RunTime=Timer()-timestart; 
		if (bOK)	//�����Գɹ�����ʾ�������
		{
			sprintf (Info, "��������ʱ��=%ds��\n������ʹ�ô���=%d��", prolog.RunTime, data.FiberTime);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_ON_COLOR, VAL_GREEN);
			SetCtrlVal (panMain, gCtrl_LED[channel], 1);
		}
		else
		{
			sprintf (Info, "��������ʱ��=%ds��\n������ʹ�ô���=%d�Σ�\n�������=%d", prolog.RunTime, data.FiberTime, data.ErrorCode);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_RED);
			SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		} 
		Insert_Info(panMain, gCtrl_BOX[channel], Info);
		
#if DEBUG
		Display_PASS_Fail_Rate (DB_Get_Handle(), my_struConfig.batch, prolog.PN, my_struProcessLOG.StationID, prolog.Log_Action, panMain, PAN_MAIN_NUMERIC_PASS, PAN_MAIN_NUMERIC_FAIL, PAN_MAIN_NUMERIC_TOTAL, PAN_MAIN_NUMERIC_PASS_RATE);
#endif  
		
		//���ģ���Ƿ��ڲ��԰���
		if (bMOD0)
		{
			strcpy (Info, "�뽫����ģ��Ӳ���ϵͳȡ��...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			do 
			{
				error = checktestbed(channel, &bMOD0);
				if (error) 
				{
					strcpy (Info, "�����԰��Ƿ���ģ������쳣");
					Insert_Info(panMain, gCtrl_BOX[channel], Info);  
				}
				else if (!bMOD0)	  	
				{
					break;
				} 
				else
				{
					strcpy (Info, "�뽫����ģ��Ӳ���ϵͳȡ��...");
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
	
	strcpy (Info, "��ͨ��������ֹͣ");
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
		bMOD0 = TRUE;   //������԰��Ƿ���ģ���ʾ
		ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		
		SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_YELLOW);
		SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		
		//����·У׼�͵���Ƿ����
		if (!sRxCal_Lock.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			errChk(DB_get_cali (CALI_TYPE_RX_MAINLOCK, channel, &timeOutFlag));
		}
		
		if (!sRxCal_Free.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			errChk(DB_get_cali (CALI_TYPE_RX_MAINFREE, channel, &timeOutFlag));
		}
		
		//��ʼ����
	    ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		timestart=Timer();
		strcpy (Info, "��ʼ���в���...");
		Insert_Info(panMain, gCtrl_BOX[channel], Info);   
		
		//��ʼ������ģ��
		if (bOK)
		{
			strcpy (Info, "��ʼ��ģ��...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = InitModule(channel, &data, &prolog); 
			if (Status) {strcpy (Info, "��ʼ��ģ�飺�ɹ�");}
			else	  	{strcpy (Info, "��ʼ��ģ�飺ʧ��");bOK=FALSE;}                      
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		//get bosasn
		if (bOK)
		{
			strcpy (Info, "��ȡ����...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = GetBosaSN(channel, &data);
			if (Status) {sprintf (Info, "��ȡ����=%s �ɹ�", data.OpticsSN);SaveData=TRUE;}
			else	    {sprintf (Info, "��ȡ����=%s ʧ��", data.OpticsSN);bOK=FALSE;data.ErrorCode=ERR_TES_READ_INNERSN;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			strcpy (prolog.SN, data.OpticsSN);
		}	
		
		//test temperature
		if (bOK)
		{
			strcpy (Info, "����ģ���¶�...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testTemper (panMain, channel, &data);
			if (Status) {sprintf (Info, "����ģ���¶�=%.2f�� �ɹ�", data.Temperatrue);}  
			else	  	{sprintf (Info, "����ģ���¶�=%.2f�� ʧ��", data.Temperatrue);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_TEMPER;SaveData=FALSE;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		//test station  	
		if (bOK && !my_struConfig.isNPI_flag)
		{
			strcpy (Info, "���Թ���...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = teststation(data, prolog);
			if (Status) {sprintf (Info, "���Թ��򣺳ɹ�");}  
			else	  	{sprintf (Info, "���Թ���ʧ��");bOK=FALSE;data.ErrorCode=ERR_TES_TEST_STATION;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		//test sens  �����Ȳ���ʱ�������AOP��ER�����Թ��׸���
		if (bOK && my_struConfig.Sel_R_Sens && !my_struConfig.Sel_R_Sens_Real)  
		{
			strcpy (Info, "�����Ȳ���...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testFixSens_1G(channel, &data);
			if (Status) {sprintf (Info, "�����Ȳ���=%.2fdBm �ɹ�", data.Sens);}
			else	    {sprintf (Info, "�����Ȳ���=%.2fdBm ʧ��", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;}  
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		if (bOK && my_struConfig.Sel_R_Sens && my_struConfig.Sel_R_Sens_Real) 
		{
			strcpy (Info, "����������ֵ...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			error = testDetailSens_1G (channel, &data);
			if (error) {sprintf (Info, "�����Ȳ���=%.2fdBm ʧ��", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;} 
			else	   {sprintf (Info, "�����Ȳ���=%.2fdBm �ɹ�", data.Sens);}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
			
		//test overload
		if (bOK && my_struConfig.Sel_R_Over)    //�ŵ���ط��˲��ԡ���ͼ����֮��
		{
			strcpy (Info, "���Թ���...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testFixOver_1G(channel, &data);
			if (Status) {sprintf (Info, "���Թ���=%.2fdBm �ɹ�", data.Over);}
			else	    {sprintf (Info, "���Թ���=%.2fdBm ʧ��", data.Over);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_OVER;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}	
		/****************��������*************************/		   
	
		/****************��������*************************/
		
		//���Գɹ������ֵ
		strcpy (prolog.PN, my_struConfig.PN); 	//PN
		strcpy (prolog.SN, data.OpticsSN);		//SN			
		strcpy (prolog.Operator, g_Operator);//Operator		 
		prolog.ResultsID=0;    						//ResultsID
		//Log_Action  �ڲ��Ժ�����ʼλ�ø�ֵ   
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
			//-------------------��Log_Action��ֵ--------------------//  
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
		
		//�����������
		if (SaveData)
		{
			strcpy (Info, "�����������...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
			Status = SaveDataBase(data, prolog);
			if (Status) {strcpy (Info, "����������ݣ��ɹ�");}
			else	    {strcpy (Info, "����������ݣ�ʧ��");bOK=FALSE;data.ErrorCode=ERR_TUN_SAVE_DATA;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
		}
		/**************************************************/
		
		//��ʾ���н��
		prolog.RunTime=Timer()-timestart; 
		if (bOK)	//�����Գɹ�����ʾ�������
		{
			sprintf (Info, "��������ʱ��=%ds��\n������ʹ�ô���=%d��", prolog.RunTime, data.FiberTime);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_ON_COLOR, VAL_GREEN);
			SetCtrlVal (panMain, gCtrl_LED[channel], 1);
		}
		else
		{
			sprintf (Info, "��������ʱ��=%ds��\n������ʹ�ô���=%d�Σ�\n�������=%d", prolog.RunTime, data.FiberTime, data.ErrorCode);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_RED);
			SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		} 
		Insert_Info(panMain, gCtrl_BOX[channel], Info);
		
#if DEBUG
		Display_PASS_Fail_Rate (DB_Get_Handle(), my_struConfig.batch, prolog.PN, my_struProcessLOG.StationID, prolog.Log_Action, panMain, PAN_MAIN_NUMERIC_PASS, PAN_MAIN_NUMERIC_FAIL, PAN_MAIN_NUMERIC_TOTAL, PAN_MAIN_NUMERIC_PASS_RATE);
#endif  
		
		//���ģ���Ƿ��ڲ��԰���
		if (bMOD0)
		{
			strcpy (Info, "�뽫����ģ��Ӳ���ϵͳȡ��...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			do 
			{
				error = checktestbed(channel, &bMOD0);
				if (error) 
				{
					strcpy (Info, "�����԰��Ƿ���ģ������쳣");
					Insert_Info(panMain, gCtrl_BOX[channel], Info);  
				}
				else if (!bMOD0)	  	
				{
					break;
				} 
				else
				{
					strcpy (Info, "�뽫����ģ��Ӳ���ϵͳȡ��...");
					Update_RESULTSInfo(panMain, gCtrl_BOX[channel], Info, FALSE);    
				}
			} while (bMOD0 && gFlag);
			
			my_struInstrument[channel].ChannelStart = FALSE;
		}	 
	}
	
Error:
	
	my_struInstrument[channel].ChannelStart = FALSE;
	
	strcpy (Info, "��ͨ��������ֹͣ");
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
		bMOD0 = TRUE;   //������԰��Ƿ���ģ���ʾ
		ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		
		SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_YELLOW);
		SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		
		//����·У׼�͵���Ƿ����
		if (!sRxCal_Lock.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			errChk(DB_get_cali (CALI_TYPE_RX_MAINLOCK, channel, &timeOutFlag));
		}
		
		if (!sRxCal_Free.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			errChk(DB_get_cali (CALI_TYPE_RX_MAINFREE, channel, &timeOutFlag));
		}
		
		//��ʼ����
	    ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		timestart=Timer();
		strcpy (Info, "��ʼ���в���...");
		Insert_Info(panMain, gCtrl_BOX[channel], Info);   
		
		//��ʼ������ģ��
		if (bOK)
		{
			strcpy (Info, "��ʼ��ģ��...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = InitModule(channel, &data, &prolog); 
			if (Status) {strcpy (Info, "��ʼ��ģ�飺�ɹ�");}
			else	  	{strcpy (Info, "��ʼ��ģ�飺ʧ��");bOK=FALSE;}                      
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		//get bosasn
		if (bOK)
		{
			strcpy (Info, "��ȡ����...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = GetBosaSN(channel, &data);
			if (Status) {sprintf (Info, "��ȡ����=%s �ɹ�", data.OpticsSN);SaveData=TRUE;}
			else	    {sprintf (Info, "��ȡ����=%s ʧ��", data.OpticsSN);bOK=FALSE;data.ErrorCode=ERR_TES_READ_INNERSN;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			strcpy (prolog.SN, data.OpticsSN);
		}	
		
		//test temperature
		if (bOK)
		{
			strcpy (Info, "����ģ���¶�...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testTemper (panMain, channel, &data);
			if (Status) {sprintf (Info, "����ģ���¶�=%.2f�� �ɹ�", data.Temperatrue);}  
			else	  	{sprintf (Info, "����ģ���¶�=%.2f�� ʧ��", data.Temperatrue);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_TEMPER;SaveData=FALSE;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		//test station  	
		if (bOK && !my_struConfig.isNPI_flag)
		{
			strcpy (Info, "���Թ���...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = teststation(data, prolog);
			if (Status) {sprintf (Info, "���Թ��򣺳ɹ�");}  
			else	  	{sprintf (Info, "���Թ���ʧ��");bOK=FALSE;data.ErrorCode=ERR_TES_TEST_STATION;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		//test sens  �����Ȳ���ʱ�������AOP��ER�����Թ��׸���
		if (bOK && my_struConfig.Sel_R_Sens && !my_struConfig.Sel_R_Sens_Real)  
		{
			strcpy (Info, "�����Ȳ���...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testFixSens_1G(channel, &data);
			if (Status) {sprintf (Info, "�����Ȳ���=%.2fdBm �ɹ�", data.Sens);}
			else	    {sprintf (Info, "�����Ȳ���=%.2fdBm ʧ��", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;}  
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		if (bOK && my_struConfig.Sel_R_Sens && my_struConfig.Sel_R_Sens_Real) 
		{
			strcpy (Info, "����������ֵ...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			error = testDetailSens_1G (channel, &data);
			if (error) {sprintf (Info, "�����Ȳ���=%.2fdBm ʧ��", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;} 
			else	   {sprintf (Info, "�����Ȳ���=%.2fdBm �ɹ�", data.Sens);}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
			
		//test overload
		if (bOK && my_struConfig.Sel_R_Over)    //�ŵ���ط��˲��ԡ���ͼ����֮��
		{
			strcpy (Info, "���Թ���...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testFixOver_1G(channel, &data);
			if (Status) {sprintf (Info, "���Թ���=%.2fdBm �ɹ�", data.Over);}
			else	    {sprintf (Info, "���Թ���=%.2fdBm ʧ��", data.Over);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_OVER;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}	
		/****************��������*************************/		   
	
		/****************��������*************************/
		
		//���Գɹ������ֵ
		strcpy (prolog.PN, my_struConfig.PN); 	//PN
		strcpy (prolog.SN, data.OpticsSN);		//SN			
		strcpy (prolog.Operator, g_Operator);//Operator		 
		prolog.ResultsID=0;    						//ResultsID
		//Log_Action  �ڲ��Ժ�����ʼλ�ø�ֵ   
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
			//-------------------��Log_Action��ֵ--------------------//  
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
		
		//�����������
		if (SaveData)
		{
			strcpy (Info, "�����������...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
			Status = SaveDataBase(data, prolog);
			if (Status) {strcpy (Info, "����������ݣ��ɹ�");}
			else	    {strcpy (Info, "����������ݣ�ʧ��");bOK=FALSE;data.ErrorCode=ERR_TUN_SAVE_DATA;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
		}
		/**************************************************/
		
		//��ʾ���н��
		prolog.RunTime=Timer()-timestart; 
		if (bOK)	//�����Գɹ�����ʾ�������
		{
			sprintf (Info, "��������ʱ��=%ds��\n������ʹ�ô���=%d��", prolog.RunTime, data.FiberTime);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_ON_COLOR, VAL_GREEN);
			SetCtrlVal (panMain, gCtrl_LED[channel], 1);
		}
		else
		{
			sprintf (Info, "��������ʱ��=%ds��\n������ʹ�ô���=%d�Σ�\n�������=%d", prolog.RunTime, data.FiberTime, data.ErrorCode);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_RED);
			SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		} 
		Insert_Info(panMain, gCtrl_BOX[channel], Info);
		
#if DEBUG
		Display_PASS_Fail_Rate (DB_Get_Handle(), my_struConfig.batch, prolog.PN, my_struProcessLOG.StationID, prolog.Log_Action, panMain, PAN_MAIN_NUMERIC_PASS, PAN_MAIN_NUMERIC_FAIL, PAN_MAIN_NUMERIC_TOTAL, PAN_MAIN_NUMERIC_PASS_RATE);
#endif  
		
		//���ģ���Ƿ��ڲ��԰���
		if (bMOD0)
		{
			strcpy (Info, "�뽫����ģ��Ӳ���ϵͳȡ��...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			do 
			{
				error = checktestbed(channel, &bMOD0);
				if (error) 
				{
					strcpy (Info, "�����԰��Ƿ���ģ������쳣");
					Insert_Info(panMain, gCtrl_BOX[channel], Info);  
				}
				else if (!bMOD0)	  	
				{
					break;
				} 
				else
				{
					strcpy (Info, "�뽫����ģ��Ӳ���ϵͳȡ��...");
					Update_RESULTSInfo(panMain, gCtrl_BOX[channel], Info, FALSE);    
				}
			} while (bMOD0 && gFlag);
			
			my_struInstrument[channel].ChannelStart = FALSE;
		}	 
	}
	
Error:
	
	my_struInstrument[channel].ChannelStart = FALSE;
	
	strcpy (Info, "��ͨ��������ֹͣ");
	Insert_Info(panMain, gCtrl_BOX[channel], Info);  

	return error;
}
// ����DWDM
int CVICALLBACK tun_DWDM(void *pvFuncData)
{
	return 0;
}
// ����DWDM
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
		bMOD0 = TRUE;   //������԰��Ƿ���ģ���ʾ
		ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		
		SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_YELLOW);
		SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		
		//����·У׼�͵���Ƿ����
		if (!sRxCal_Lock.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			errChk(DB_get_cali (CALI_TYPE_RX_MAINLOCK, channel, &timeOutFlag));
		}
		
		if (!sRxCal_Free.flag[channel] && my_struInstrument[channel].ATT_TYPE_MAIN != ATT_TYPE_NONE) 
		{
			errChk(DB_get_cali (CALI_TYPE_RX_MAINFREE, channel, &timeOutFlag));
		}
		
		//��ʼ����
	    ResetTextBox (panMain, gCtrl_BOX[channel], ""); 
		timestart=Timer();
		strcpy (Info, "��ʼ���в���...");
		Insert_Info(panMain, gCtrl_BOX[channel], Info);   
		
		//��ʼ������ģ��
		if (bOK)
		{
			strcpy (Info, "��ʼ��ģ��...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = InitModule(channel, &data, &prolog); 
			if (Status) {strcpy (Info, "��ʼ��ģ�飺�ɹ�");}
			else	  	{strcpy (Info, "��ʼ��ģ�飺ʧ��");bOK=FALSE;}                      
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		//get bosasn
		if (bOK)
		{
			strcpy (Info, "��ȡ����...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = GetBosaSN(channel, &data);
			if (Status) {sprintf (Info, "��ȡ����=%s �ɹ�", data.OpticsSN);SaveData=TRUE;}
			else	    {sprintf (Info, "��ȡ����=%s ʧ��", data.OpticsSN);bOK=FALSE;data.ErrorCode=ERR_TES_READ_INNERSN;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			strcpy (prolog.SN, data.OpticsSN);
		}					 
		//test temperature
		if (bOK)
		{
			strcpy (Info, "����ģ���¶�...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testTemper (panMain, channel, &data);
			if (Status) {sprintf (Info, "����ģ���¶�=%.2f�� �ɹ�", data.Temperatrue);}  
			else	  	{sprintf (Info, "����ģ���¶�=%.2f�� ʧ��", data.Temperatrue);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_TEMPER;SaveData=FALSE;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		//test station  	
		if (bOK)
		{
			strcpy (Info, "���Թ���...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = teststation(data, prolog);
			if (Status) {sprintf (Info, "���Թ��򣺳ɹ�");}  
			else	  	{sprintf (Info, "���Թ���ʧ��");bOK=FALSE;data.ErrorCode=ERR_TES_TEST_STATION;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}		   
	/*	//test SD
		if (bOK && my_struConfig.Sel_R_SDHys)   
		{																		
			strcpy (Info, "����SD�ź�...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testSD();
			if (Status) {strcpy (Info, "����SD�źţ��ɹ�");}
			else	    {strcpy (Info, "����SD�źţ�ʧ��");bOK=FALSE;my_struTestData.ErrorCode=ERR_TES_SD;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}	
	*/

		//AOP
		if (bOK && my_struConfig.Sel_T_AOP) 
		{
			strcpy (Info, "���Թ⹦��...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testAOP10G(channel, &data);
			if (Status) {sprintf (Info, "���Թ⹦��=%.2fdBm �ɹ�;Ibias=%.2fmA", data.AOP,data.A2_Ibias);}
			else	    {sprintf (Info, "���Թ⹦��=%.2fdBm ʧ��;Ibias=%.2fmA", data.AOP,data.A2_Ibias);bOK=FALSE;data.ErrorCode=ERR_TES_AOP;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}  

		// er ��ͼ mask ���׵�
		if (bOK)
		{
			strcpy (Info, "�л���·...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			error = testParameterLock(channel, &data, panMain, gCtrl_BOX[channel], Info, ERInfo);
			if (error){bOK=FALSE;} //�˴�����Ҫ��ʾ��ؽ����testParameterLock�����Ѿ���ʾ 
		} 
	
		//cal TxPower
		if (my_struConfig.Sel_Calibrate_Test && bOK && my_struConfig.Sel_T_AOP)
		{
			strcpy (Info, "����У׼����...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = caltestTxPower10G(channel, &data);
			if (Status) {sprintf (Info,"����У׼����=%.2fdBm �ɹ�", data.RxTr);}
			else	    {sprintf (Info,"����У׼����=%.2fdBm ʧ��", data.RxTr);bOK=FALSE;data.ErrorCode=ERR_TES_CAL_TXPOWER;}        
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
		}
		
		//test overload
		if (bOK && my_struConfig.Sel_R_Over)    //�ŵ���ط��˲��ԡ���ͼ����֮��
		{
			strcpy (Info, "���Թ���...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testFixOver_10G(channel, &data);
			if (Status) {sprintf (Info, "���Թ���=%.2fdBm �ɹ�", data.Over);}
			else	    {sprintf (Info, "���Թ���=%.2fdBm ʧ��", data.Over);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_OVER;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}	
		
		//test current
		if (bOK && (my_struInstrument[channel].SEVB_TYPE==SEVB_TYPE_SEVB5|| my_struInstrument[channel].SEVB_TYPE==SEVB_TYPE_SEVB27))
		{
			strcpy (Info, "���Ե���...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);                   
			Status = testCurrent(channel, &data);
			if (Status) {sprintf (Info, "�����ն˵���=%.2f ���˵���=%.2f �ɹ�;Ibias=%.2fmA ", data.RxI, data.TxI,data.A2_Ibias);} 
			else	    {sprintf (Info, "�����ն˵���=%.2f ���˵���=%.2f ʧ��", data.RxI, data.TxI);bOK=FALSE;data.ErrorCode=ERR_TUN_TEST_CURRENT;}        
			Insert_Info(panMain, gCtrl_BOX[channel], Info);                   
		}
		
		//AOP
		if (bOK && my_struConfig.Sel_T_TxDis) 
		{
			strcpy (Info, "���Է���Ӳ���ضϹ⹦��...");                
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testTxDis_10G(channel, &data);
			if (error) 	{sprintf (Info, "���Է���Ӳ���ضϹ⹦��=%.2fdBm ʧ��", data.TxOffPower);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_TXDIS;} 
			else		{sprintf (Info, "���Է���Ӳ���ضϹ⹦��=%.2fdBm �ɹ�", data.TxOffPower);}  
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}  
		
		//test sens  �����Ȳ���ʱ�������AOP��ER�����Թ��׸���
		if (bOK && my_struConfig.Sel_R_Sens && !my_struConfig.Sel_R_Sens_Real)  
		{
			strcpy (Info, "�����Ȳ���...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = testFixSens_10G(channel, &data);
			if (Status) {sprintf (Info, "�����Ȳ���=%.2fdBm �ɹ�", data.Sens);}
			else	    {sprintf (Info, "�����Ȳ���=%.2fdBm ʧ��", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;}  
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		if (bOK && my_struConfig.Sel_R_Sens && my_struConfig.Sel_R_Sens_Real) 
		{
			strcpy (Info, "����������ֵ...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			error = testDetailSens_10G (channel, &data);
			if (error) {sprintf (Info, "�����Ȳ���=%.2fdBm ʧ��", data.Sens);bOK=FALSE;data.ErrorCode=ERR_TES_TEST_SENS;} 
			else	   {sprintf (Info, "�����Ȳ���=%.2fdBm �ɹ�", data.Sens);}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}
		
		//cal RxPower  
		if (my_struConfig.Sel_Calibrate_Test && bOK)
		{
			strcpy (Info, "�ն�У׼����...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
			Status = caltest_RxPower10G (panMain, channel, &data);
			if (Status) {sprintf (Info, "�ն�У׼���ԣ��ɹ�");}
			else	    
			{
				sprintf (Info, "�ն�У׼���ԣ�ʧ��");
				bOK=FALSE;
				if (data.ErrorCode==0) 
				{
					data.ErrorCode=ERR_TES_CAL_RXPOWER;
				}
			}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info); 
		}  	   
	
		/****************��������*************************/
		
		//���Գɹ������ֵ
		strcpy (prolog.PN, my_struConfig.PN); 	//PN
		strcpy (prolog.SN, data.OpticsSN);		//SN			
		strcpy (prolog.Operator, g_Operator);//Operator		 
		prolog.ResultsID=0;    						//ResultsID
		//Log_Action  �ڲ��Ժ�����ʼλ�ø�ֵ   
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
			//-------------------��Log_Action��ֵ--------------------//  
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
		
		//�����������
		if (SaveData)
		{
			strcpy (Info, "�����������...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
			Status = SaveDataBase(data, prolog);
			if (Status) {strcpy (Info, "����������ݣ��ɹ�");}
			else	    {strcpy (Info, "����������ݣ�ʧ��");bOK=FALSE;data.ErrorCode=ERR_TUN_SAVE_DATA;}       
			Insert_Info(panMain, gCtrl_BOX[channel], Info);  
		}
		/**************************************************/
		
		//��ʾ���н��
		prolog.RunTime=Timer()-timestart; 
		if (bOK)	//�����Գɹ�����ʾ�������
		{
			sprintf (Info, "��������ʱ��=%ds��\n������ʹ�ô���=%d��", prolog.RunTime, data.FiberTime);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_ON_COLOR, VAL_GREEN);
			SetCtrlVal (panMain, gCtrl_LED[channel], 1);
		}
		else
		{
			sprintf (Info, "��������ʱ��=%ds��\n������ʹ�ô���=%d�Σ�\n�������=%d", prolog.RunTime, data.FiberTime, data.ErrorCode);
			SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_RED);
			SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		} 
		Insert_Info(panMain, gCtrl_BOX[channel], Info);

#if DEBUG
		Display_PASS_Fail_Rate (DB_Get_Handle(), my_struConfig.batch, prolog.PN, my_struProcessLOG.StationID, prolog.Log_Action, panMain, PAN_MAIN_NUMERIC_PASS, PAN_MAIN_NUMERIC_FAIL, PAN_MAIN_NUMERIC_TOTAL, PAN_MAIN_NUMERIC_PASS_RATE);
#endif 
		
		//���ģ���Ƿ��ڲ��԰���
		if (bMOD0)
		{
			strcpy (Info, "�뽫����ģ��Ӳ���ϵͳȡ��...");
			Insert_Info(panMain, gCtrl_BOX[channel], Info);
			do 
			{
				error = checktestbed(channel, &bMOD0);
				if (error) 
				{
					strcpy (Info, "�����԰��Ƿ���ģ������쳣");
					Insert_Info(panMain, gCtrl_BOX[channel], Info);  
				}
				else if (!bMOD0)	  	
				{
					break;
				} 
				else
				{
					strcpy (Info, "�뽫����ģ��Ӳ���ϵͳȡ��...");
					Update_RESULTSInfo(panMain, gCtrl_BOX[channel], Info, FALSE);
				}
			} while (bMOD0 && gFlag);
			
			my_struInstrument[channel].ChannelStart = FALSE;
		}	 
	}
	
Error:
	
	my_struInstrument[channel].ChannelStart = FALSE;
	
	strcpy (Info, "��ͨ��������ֹͣ");
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
		MessagePopup("��ʾ","δ�ҵ���Ӧ�ؼ�Value!");
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
//	// Ĭ��������0ͨ��
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
//	if(/*�ж�temp�ķ���ֵ*/)
//	{
//		// AOP��ǰ��ֱͨ״̬����򿪹��ʱջ�����ϵͳ
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
//		MessagePopup ("ERROR", "�����������!     "); 
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
//		// �·�Ĭ���¶�
//		
//	}
	return 0;
}

int AdjustAop(int handle, int setDac, int *aopVlue)
{
//	assert(aopVlue != NULL);
//	// ������һ��Ĭ��ֵ
//	SettingingAop(0,0xD0);
//	do
//	{
//		ReadingAop(0, aopVlue);
//		if(/*AopVlue�ںϷ���Χ��*/)
//		{
//			break;
//		}
//		/*����DACֵ*/
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
//	// ʹ��string.hͷ�ļ�
//	bzero(&dataTmp,sizeof(dataTmp));
//	// ��ѯAPC����״̬
//	// Ĭ��������0ͨ��
//	SearchApcState(0);

//	// �����⹦��AOP
//	AdjustAop(0, setDac, &AopVlue);

//	// ����ER
//	Status = tuningER10G(panMain, 0, dataTmp);

//	// ���ڲ���
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
	// ��ʾ����
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
	
	progresshandle = CreateProgressDialog ("�豸����״̬", "�豸���ý���", 1, VAL_NO_INNER_MARKERS, "");
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
			
				//�л���·
			my_struInstrument[channel].SW_TYPE = 4;
			if (SW_TYPE_NONE != my_struInstrument[channel].SW_TYPE)
			{
				//�л���һͨ��
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
	
	progresshandle = CreateProgressDialog ("�豸����״̬", "�豸���ý���", 1, VAL_NO_INNER_MARKERS, "");
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
			
				//�л���·
			my_struInstrument[channel].SW_TYPE = 4;
			if (SW_TYPE_NONE != my_struInstrument[channel].SW_TYPE)
			{
				//�л���һͨ��
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
