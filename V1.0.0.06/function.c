#include "cvidll.h" 
#include "CSA8000.h"
#include "supermaster.h" 
#include "function.h" 
#include "inifile.h"
#include <utility.h>
#include <visa.h>
#include "cvi_db.h"  
#include "Ag86100.h"
#include "HP8156A.h"
#include "GBERT.h"
//#include "DS1856.h"
#include "labdll.h"
#include "license.h"	
#include "global.h" 
#include "PMSII.h" 
#include "OLA55M.h"  
#include "CH341A_DLL.h" 
#include "Ag86142.h"
#include "AQ633x.h"
#include "AQ637x.h"
#include "EVB16.h" 
#include "EVB5.h" 
#include <analysis.h>
#include "combobox.h"
//#include "XGSPON_OLT.h" 
#include "XGSPON OLT Parallel ATE.h"
#include "FPY.h"
#include "JW8504.h"
#include "pss.h"
#include "TG1b1a.h"	


//for database
int hdbc = 0;      /* Handle to database connection    */        
//int hstmt = 0;	   /* Handle to SQL statement          */    
int hmap = 0;      /* Handle to map definition         */           
//int resCode = 0;   /* Result code                      */
int DBConfigStat;  /* Status code                      */ 

//int hamp_monitor=0;
int hamp_E2=0; 

BOOL Status;
int mySTOPID;

void ShowDataBaseError(void)
{
    MessagePopup("Database Error",DBErrorMessage());
}

BOOL GetConfig(void)
{
IniText g_myInifile = 0;
int 	status;

	//创建ini文件句柄
	g_myInifile = Ini_New (0); 
	
	//指向ini文件并读取ini文件信息
	Ini_ReadFromFile (g_myInifile, g_ConfigFileName); 
	
	//获取指定Section和Item的ItemValue字符串
	status = Ini_GetInt (g_myInifile, "Configuration Set", "PNIndex", &my_struConfig.PNIndex);
	if(!status) { MessagePopup("Inifile", "Error reading item from section"); goto  Error;}
	status = Ini_GetStringIntoBuffer (g_myInifile, "Configuration Set", "PN", my_struConfig.PN, 50);
	if(!status) { MessagePopup("Inifile", "Error reading item from section"); goto  Error;}
	
	status = Ini_GetBoolean (g_myInifile, "Configuration Set", "Fun_Tun_10G", &my_struConfig.DT_Tun_10G);
	if(!status) { MessagePopup("Inifile", "Error reading item from section"); goto  Error;}
	status = Ini_GetBoolean (g_myInifile, "Configuration Set", "Fun_Test_10G", &my_struConfig.DT_Test_10G);
	if(!status) { MessagePopup("Inifile", "Error reading item from section"); goto  Error;}
	status = Ini_GetBoolean (g_myInifile, "Configuration Set", "Fun_QATest_10G", &my_struConfig.DT_QATest_10G);
	if(!status) { MessagePopup("Inifile", "Error reading item from section"); goto  Error;}
	
	status = Ini_GetBoolean (g_myInifile, "Configuration Set", "Fun_Test_1G", &my_struConfig.DT_Test_1G);
	if(!status) { MessagePopup("Inifile", "Error reading item from section"); goto  Error;}
	status = Ini_GetBoolean (g_myInifile, "Configuration Set", "Fun_QATest_1G", &my_struConfig.DT_QATest_1G);
	if(!status) { MessagePopup("Inifile", "Error reading item from section"); goto  Error;}

	status = Ini_GetBoolean (g_myInifile, "Configuration Set", "Temper_Low", &my_struConfig.Temper_Low);
	if(!status) { MessagePopup("Inifile", "Error reading item from section"); goto  Error;}
	status = Ini_GetBoolean (g_myInifile, "Configuration Set", "Temper_Room", &my_struConfig.Temper_Room);
	if(!status) { MessagePopup("Inifile", "Error reading item from section"); goto  Error;}
	status = Ini_GetBoolean (g_myInifile, "Configuration Set", "Temper_High", &my_struConfig.Temper_High);
	if(!status) { MessagePopup("Inifile", "Error reading item from section"); goto  Error;}

	status = Ini_GetBoolean (g_myInifile, "Configuration Set", "Sens_Real", &my_struConfig.Sel_R_Sens_Real);
	if(!status) { MessagePopup("Inifile", "Error reading item from section"); goto  Error;}
	status = Ini_GetBoolean (g_myInifile, "Configuration Set", "APD_Aging", &my_struConfig.Sel_APD_Aging);
	if(!status) { MessagePopup("Inifile", "Error reading item from section"); goto  Error;}

//释放ini文件句柄,执行结束后所有获取的值将被释放
	Ini_Dispose (g_myInifile);
	
	return TRUE;
Error:
//释放ini文件句柄
	Ini_Dispose (g_myInifile);
	return FALSE;
}

BOOL SetConfig(void)
{
//char    g_fileName[MAX_PATHNAME_LEN];
IniText g_myInifile = 0;
int 	status;

	//创建ini文件句柄
	g_myInifile = Ini_New (0); 
	
	//写入指定Section和Item的ItemValue字符串
	status = Ini_PutInt (g_myInifile, "Configuration Set", "PNIndex", my_struConfig.PNIndex);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "Configuration Set", "PN", my_struConfig.PN);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	
	status = Ini_PutBoolean (g_myInifile, "Configuration Set", "Fun_Tun_10G", my_struConfig.DT_Tun_10G);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutBoolean (g_myInifile, "Configuration Set", "Fun_Test_10G", my_struConfig.DT_Test_10G);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}  
	status = Ini_PutBoolean (g_myInifile, "Configuration Set", "Fun_QATest_10G", my_struConfig.DT_QATest_10G);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	
	status = Ini_PutBoolean (g_myInifile, "Configuration Set", "Fun_Test_1G", my_struConfig.DT_Test_1G);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}  
	status = Ini_PutBoolean (g_myInifile, "Configuration Set", "Fun_QATest_1G", my_struConfig.DT_QATest_1G);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	
	status = Ini_PutBoolean (g_myInifile, "Configuration Set", "Temper_Low", my_struConfig.Temper_Low);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutBoolean (g_myInifile, "Configuration Set", "Temper_Room", my_struConfig.Temper_Room);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutBoolean (g_myInifile, "Configuration Set", "Temper_High", my_struConfig.Temper_High);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	
	status = Ini_PutBoolean (g_myInifile, "Configuration Set", "Sens_Real", my_struConfig.Sel_R_Sens_Real);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutBoolean (g_myInifile, "Configuration Set", "APD_Aging", my_struConfig.Sel_APD_Aging);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}

	status = Ini_WriteToFile (g_myInifile, g_ConfigFileName);
	if(status){MessagePopup("Inifile", "Error writing item from section"); goto  Error;}  
//释放ini文件句柄,执行结束后所有获取的值将被释放
	Ini_Dispose (g_myInifile);
	
	return TRUE;
Error:
//释放ini文件句柄
	Ini_Dispose (g_myInifile);
	return FALSE;
}

BOOL GetConfig_Inst(void)
{
	IniText g_myInifile = 0;
	int 	status;
	int     index;
	char Section_Name[256];   
	int 	error = 0;             

	//创建ini文件句柄
	g_myInifile = Ini_New (0); 
	
	//指向ini文件并读取ini文件信息
	Ini_ReadFromFile (g_myInifile, g_InstFileName); 
	
	for (index=0; index<CHANNEL_MAX; index++)
	{
		sprintf (Section_Name, "Channel %d", index);
	
		errChk(Ini_GetInt (g_myInifile, Section_Name, "CHANNEL_EN", &my_struInstrument[index].ChannelEn)); 
		errChk(Ini_GetInt (g_myInifile, Section_Name, "CHANNEL", &my_struInstrument[index].Channel));
	
		//获取指定Section和Item的ItemValue字符串
		//for power meter
		errChk(Ini_GetInt (g_myInifile, Section_Name, "PowerType", &my_struInstrument[index].Power_TYPE));   
		errChk(Ini_GetInt (g_myInifile, Section_Name, "PMSII_COM", &my_struInstrument[index].PMSII));
		errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "SN_PM", my_struInstrument[index].SN_PM, 30)); 
	
		//for DCA
		errChk(Ini_GetInt (g_myInifile, Section_Name, "DCAType", &my_struInstrument[index].DCA_TYPE));
		errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "DCA_Addr", my_struInstrument[index].DCA_Addr, 30)); 
		errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "SN_DCA", my_struInstrument[index].SN_DCA, 30)); 
	
		//for BERT
		errChk(Ini_GetInt (g_myInifile, Section_Name, "BERTType", &my_struInstrument[index].BERT_TYPE));  
		errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "SN_BERT", my_struInstrument[index].SN_BERT, 30));  
	
		//for spectrum
		errChk(Ini_GetInt (g_myInifile, Section_Name, "SPECTRUM_TYPE", &my_struInstrument[index].SPECTRUM_TYPE));
		errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "Spectrum_Addr", my_struInstrument[index].Spectrum_Addr, 30));   
		errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "SN_Spectrum", my_struInstrument[index].SN_Spectrum, 30));	 

		//for att
		errChk(Ini_GetInt (g_myInifile, Section_Name, "ATT_Type", &my_struInstrument[index].ATT_TYPE_MAIN));	
		errChk(Ini_GetInt (g_myInifile, Section_Name, "ATT_COM", &my_struInstrument[index].ATT_MAIN_COM));   
		errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "SN_ATT", my_struInstrument[index].SN_ATT_MAIN, 30));
		
		//for att REF
		errChk(Ini_GetInt (g_myInifile, Section_Name, "ATT_REF_Type", &my_struInstrument[index].ATT_REF_TYPE));	
		errChk(Ini_GetInt (g_myInifile, Section_Name, "ATT_REF_COM", &my_struInstrument[index].ATT_REF_COM));   
		errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "SN_ATT_REF", my_struInstrument[index].SN_ATT_REF, 30));
	
		//for evb
		errChk(Ini_GetInt (g_myInifile, Section_Name, "EVB_TYPE", &my_struInstrument[index].SEVB_TYPE));	
		errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "SEVB", my_struInstrument[index].SEVB, 30));
	
		//for evb light
		errChk(Ini_GetInt (g_myInifile, Section_Name, "EVB_Light_TYPE", &my_struInstrument[index].EVB_Light_TYPE));  
		errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "EVB_Light", my_struInstrument[index].EVB_Light, 30)); 
	
		//for bert
		errChk(Ini_GetInt (g_myInifile, Section_Name, "10GBERT_TYPE", &my_struInstrument[index].BERT_TYPE_10G));	
		errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "SN_10GBERT", my_struInstrument[index].SN_10GBERT, 30));	  

		//for fiber
		errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "Fiber", my_struInstrument[index].Fiber, 30));
		
		// for sw
		errChk(Ini_GetInt (g_myInifile, Section_Name, "SW_TYPE", &my_struInstrument[index].SW_TYPE));
		errChk(Ini_GetInt (g_myInifile, Section_Name, "SW_COM", &my_struInstrument[index].SW));
		errChk(Ini_GetInt (g_myInifile, Section_Name, "SW_CHAN", &my_struInstrument[index].SW_CHAN));
		errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "SW_SN", my_struInstrument[index].SW_SN, 30));    
		
		// for clock
		errChk(Ini_GetInt (g_myInifile, Section_Name, "CLOCK_TYPE", &my_struInstrument[index].CLOCK_TYPE));
		errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "CLOCK", my_struInstrument[index].CLOCK, 30));
		errChk(Ini_GetInt (g_myInifile, Section_Name, "CLOCK_CHAN", &my_struInstrument[index].CLOCK_CHAN));
	}
	
	errChk(Ini_GetBoolean (g_myInifile, "Configuration Set", "NPI", &my_struConfig.isNPI_flag)); 

Error:
//释放ini文件句柄
	//释放ini文件句柄,执行结束后所有获取的值将被释放
	Ini_Dispose (g_myInifile);
	return error;
}

BOOL SetConfig_Inst(void)
{
	IniText g_myInifile = 0;
	int 	error = 0;
	char Section_Name[256];   
	int index;

	//创建ini文件句柄
	g_myInifile = Ini_New (0); 
	
	//写入指定Section和Item的ItemValue字符串
	for (index=0; index<CHANNEL_MAX; index++)
	{
		sprintf (Section_Name, "Channel %d", index);
		
		errChk(Ini_PutInt (g_myInifile, Section_Name, "CHANNEL_EN", my_struInstrument[index].ChannelEn));  
		errChk(Ini_PutInt (g_myInifile, Section_Name, "CHANNEL", my_struInstrument[index].Channel));
		
		//写入指定Section和Item的ItemValue字符串
		//for power meter
		errChk(Ini_PutInt (g_myInifile, Section_Name, "PowerType", my_struInstrument[index].Power_TYPE));   
		errChk(Ini_PutInt (g_myInifile, Section_Name, "PMSII_COM", my_struInstrument[index].PMSII));   
		errChk(Ini_PutString (g_myInifile, Section_Name, "SN_PM", my_struInstrument[index].SN_PM)); 
		
		//for DCA
		errChk(Ini_PutInt (g_myInifile, Section_Name, "DCAType", my_struInstrument[index].DCA_TYPE));
		errChk(Ini_PutString (g_myInifile, Section_Name, "DCA_Addr", my_struInstrument[index].DCA_Addr));
		errChk(Ini_PutString (g_myInifile, Section_Name, "SN_DCA", my_struInstrument[index].SN_DCA));
	
		//for BERT
		errChk(Ini_PutInt (g_myInifile, Section_Name, "BERTType", my_struInstrument[index].BERT_TYPE));  
		errChk(Ini_PutString (g_myInifile, Section_Name, "SN_BERT", my_struInstrument[index].SN_BERT));	
	
		//for spectrum
		errChk(Ini_PutInt (g_myInifile, Section_Name, "SPECTRUM_TYPE", my_struInstrument[index].SPECTRUM_TYPE));
		errChk(Ini_PutString (g_myInifile, Section_Name, "Spectrum_Addr", my_struInstrument[index].Spectrum_Addr));         
		errChk(Ini_PutString (g_myInifile, Section_Name, "SN_Spectrum", my_struInstrument[index].SN_Spectrum));

		// for ATT
		errChk(Ini_PutInt (g_myInifile, Section_Name, "ATT_Type", my_struInstrument[index].ATT_TYPE_MAIN));   
		errChk(Ini_PutString (g_myInifile, Section_Name, "SN_ATT", my_struInstrument[index].SN_ATT_MAIN));
		errChk(Ini_PutInt (g_myInifile, Section_Name, "ATT_COM", my_struInstrument[index].ATT_MAIN_COM));   
		
		// for ATT REF
		errChk(Ini_PutInt (g_myInifile, Section_Name, "ATT_REF_Type", my_struInstrument[index].ATT_REF_TYPE));   
		errChk(Ini_PutString (g_myInifile, Section_Name, "SN_ATT_REF", my_struInstrument[index].SN_ATT_REF));
		errChk(Ini_PutInt (g_myInifile, Section_Name, "ATT_REF_COM", my_struInstrument[index].ATT_REF_COM)); 
	
		//for evb
		errChk(Ini_PutInt (g_myInifile, Section_Name, "EVB_TYPE", my_struInstrument[index].SEVB_TYPE));
		errChk(Ini_PutString (g_myInifile, Section_Name, "SEVB", my_struInstrument[index].SEVB));
	
		//for evb light
		errChk(Ini_PutInt (g_myInifile, Section_Name, "EVB_Light_TYPE", my_struInstrument[index].EVB_Light_TYPE));
		errChk(Ini_PutString (g_myInifile, Section_Name, "EVB_Light", my_struInstrument[index].EVB_Light));
	
		//for 10Gbert
		errChk(Ini_PutInt (g_myInifile, Section_Name, "10GBERT_TYPE", my_struInstrument[index].BERT_TYPE_10G));
		errChk(Ini_PutString (g_myInifile, Section_Name, "SN_10GBERT", my_struInstrument[index].SN_10GBERT));

		//for fiber
		errChk(Ini_PutString (g_myInifile, Section_Name, "Fiber", my_struInstrument[index].Fiber));
		
		// for sw
		errChk(Ini_PutInt (g_myInifile, Section_Name, "SW_TYPE", my_struInstrument[index].SW_TYPE));
		errChk(Ini_PutInt (g_myInifile, Section_Name, "SW_COM", my_struInstrument[index].SW));
		errChk(Ini_PutInt (g_myInifile, Section_Name, "SW_CHAN", my_struInstrument[index].SW_CHAN));
		errChk(Ini_PutString (g_myInifile, Section_Name, "SW_SN", my_struInstrument[index].SW_SN)); 
		
		// for clock
		errChk(Ini_PutInt (g_myInifile, Section_Name, "CLOCK_TYPE", my_struInstrument[index].CLOCK_TYPE));
		errChk(Ini_PutString (g_myInifile, Section_Name, "CLOCK", my_struInstrument[index].CLOCK));
		errChk(Ini_PutInt (g_myInifile, Section_Name, "CLOCK_CHAN", my_struInstrument[index].CLOCK_CHAN));
	}
	
		//NPI Flag
	errChk(Ini_PutBoolean (g_myInifile, "Configuration Set", "NPI", my_struConfig.isNPI_flag)); 
	errChk(Ini_WriteToFile (g_myInifile, g_InstFileName));	  

//释放ini文件句柄,执行结束后所有获取的值将被释放
	Ini_Dispose (g_myInifile);
	
	return TRUE;
Error:
//释放ini文件句柄
	Ini_Dispose (g_myInifile);
	return FALSE;
}

BOOL SetControl(void)
{
	IniText g_myInifile = 0;
	int 	status;

	//创建ini文件句柄
	g_myInifile = Ini_New (0); 
	
	//InitModule
	status = Ini_PutInt (g_myInifile, "InitModule", "Init_I2cAddr", my_struControl.Init_I2cAddr);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "InitModule", "Init_PowerWord_Index", my_struControl.Init_PowerWord_Index);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "InitModule", "Init_GetVersion", my_struControl.Init_GetVersion);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	
	//Get SN
	status = Ini_PutInt (g_myInifile, "GetSN", "GetSN_I2cAddr", my_struControl.GetSN_I2cAddr);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "GetSN", "GetSN_Table", my_struControl.GetSN_Table);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}

	//Get Temper
	status = Ini_PutString (g_myInifile, "GetTemper", "GetTemper_CaseCommd", my_struControl.GetTemper_CaseCommd);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "GetTemper", "GetTemper_CoreCommd", my_struControl.GetTemper_CoreCommd);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "GetTemper", "GetTemper_IndexOffset", my_struControl.GetTemper_IndexOffset);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	
	//TunAOP 1G
	status = Ini_PutString (g_myInifile, "TunAOP1G", "TunAOP1G_MODE_M", my_struControl.TunAOP1G_MODE_M);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
    status = Ini_PutString (g_myInifile, "TunAOP1G", "TunAOP1G_MODE_A", my_struControl.TunAOP1G_MODE_A);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TunAOP1G", "TunAOP1G_DAC_SET", my_struControl.TunAOP1G_DAC_SET);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunAOP1G", "TunAOP1G_LUT_NAME_INDEX", my_struControl.TunAOP1G_LUT_NAME_INDEX);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunAOP1G", "TunAOP1G_LUK_INDEX", my_struControl.TunAOP1G_LUK_INDEX);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunAOP1G", "TunAOP1G_LUK_SLOPE_AMP", my_struControl.TunAOP1G_LUK_SLOPE_AMP);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunAOP1G", "TunAOP1G_LUK_OFFSET_AMP", my_struControl.TunAOP1G_LUK_OFFSET_AMP);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}  
	status = Ini_PutString (g_myInifile, "TunAOP1G", "TunAOP1G_SAVE", my_struControl.TunAOP1G_SAVE);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	
	//TunAOP 10G
	status = Ini_PutString (g_myInifile, "TunAOP10G", "TunAOP10G_MODE_M", my_struControl.TunAOP10G_MODE_M);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
    status = Ini_PutString (g_myInifile, "TunAOP10G", "TunAOP10G_MODE_A", my_struControl.TunAOP10G_MODE_A);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TunAOP10G", "TunAOP10G_DAC_SET", my_struControl.TunAOP10G_DAC_SET);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunAOP10G", "TunAOP10G_LUT_NAME_INDEX", my_struControl.TunAOP10G_LUT_NAME_INDEX);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunAOP10G", "TunAOP10G_LUK_INDEX", my_struControl.TunAOP10G_LUK_INDEX);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunAOP10G", "TunAOP10G_LUK_SLOPE_AMP", my_struControl.TunAOP10G_LUK_SLOPE_AMP);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunAOP10G", "TunAOP10G_LUK_OFFSET_AMP", my_struControl.TunAOP10G_LUK_OFFSET_AMP);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;} 
	status = Ini_PutString (g_myInifile, "TunAOP10G", "TunAOP10G_SAVE", my_struControl.TunAOP10G_SAVE);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	
	//TunER 1G
	status = Ini_PutString (g_myInifile, "TunER1G", "TunER1G_MODE_M", my_struControl.TunER1G_MODE_M);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
    status = Ini_PutString (g_myInifile, "TunER1G", "TunER1G_MODE_A", my_struControl.TunER1G_MODE_A);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TunER1G", "TunER1G_DAC_SET", my_struControl.TunER1G_DAC_SET);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunER1G", "TunER1G_LUT_NAME_INDEX", my_struControl.TunER1G_LUT_NAME_INDEX);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunER1G", "TunER1G_LUK_INDEX", my_struControl.TunER1G_LUK_INDEX);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunER1G", "TunER1G_LUK_SLOPE_AMP", my_struControl.TunER1G_LUK_SLOPE_AMP);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunER1G", "TunER1G_LUK_OFFSET_AMP", my_struControl.TunER1G_LUK_OFFSET_AMP);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;} 
	status = Ini_PutString (g_myInifile, "TunER1G", "TunER1G_SAVE", my_struControl.TunER1G_SAVE);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}

	//TunER 10G
	status = Ini_PutString (g_myInifile, "TunER10G", "TunER10G_MODE_M", my_struControl.TunER10G_MODE_M);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
    status = Ini_PutString (g_myInifile, "TunER10G", "TunER10G_MODE_A", my_struControl.TunER10G_MODE_A);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TunER10G", "TunER10G_DAC_SET", my_struControl.TunER10G_DAC_SET);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunER10G", "TunER10G_LUT_NAME_INDEX", my_struControl.TunER10G_LUT_NAME_INDEX);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunER10G", "TunER10G_LUK_INDEX", my_struControl.TunER10G_LUK_INDEX);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunER10G", "TunER10G_LUK_SLOPE_AMP", my_struControl.TunER10G_LUK_SLOPE_AMP);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunER10G", "TunER10G_LUK_OFFSET_AMP", my_struControl.TunER10G_LUK_OFFSET_AMP);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;} 
	status = Ini_PutString (g_myInifile, "TunER10G", "TunER10G_SAVE", my_struControl.TunER10G_SAVE);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	
	//TunCross 1G
	status = Ini_PutString (g_myInifile, "TunCross1G", "TunCross1G_MODE_M", my_struControl.TunCross1G_MODE_M);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
    status = Ini_PutString (g_myInifile, "TunCross1G", "TunCross1G_MODE_A", my_struControl.TunCross1G_MODE_A);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TunCross1G", "TunCross1G_DAC_SET", my_struControl.TunCross1G_DAC_SET);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunCross1G", "TunCross1G_LUT_NAME_INDEX", my_struControl.TunCross1G_LUT_NAME_INDEX);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunCross1G", "TunCross1G_LUK_INDEX", my_struControl.TunCross1G_LUK_INDEX);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunCross1G", "TunCross1G_LUK_SLOPE_AMP", my_struControl.TunCross1G_LUK_SLOPE_AMP);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunCross1G", "TunCross1G_LUK_OFFSET_AMP", my_struControl.TunCross1G_LUK_OFFSET_AMP);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;} 
	status = Ini_PutString (g_myInifile, "TunCross1G", "TunCross1G_SAVE", my_struControl.TunCross1G_SAVE);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}

	//TunCross 10G
	status = Ini_PutString (g_myInifile, "TunCross10G", "TunCross10G_MODE_M", my_struControl.TunCross10G_MODE_M);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
    status = Ini_PutString (g_myInifile, "TunCross10G", "TunCross10G_MODE_A", my_struControl.TunCross10G_MODE_A);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TunCross10G", "TunCross10G_DAC_SET", my_struControl.TunCross10G_DAC_SET);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunCross10G", "TunCross10G_LUT_NAME_INDEX", my_struControl.TunCross10G_LUT_NAME_INDEX);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunCross10G", "TunCross10G_LUK_INDEX", my_struControl.TunCross10G_LUK_INDEX);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunCross10G", "TunCross10G_LUK_SLOPE_AMP", my_struControl.TunCross10G_LUK_SLOPE_AMP);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunCross10G", "TunCross10G_LUK_OFFSET_AMP", my_struControl.TunCross10G_LUK_OFFSET_AMP);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;} 
	status = Ini_PutString (g_myInifile, "TunCross10G", "TunCross10G_SAVE", my_struControl.TunCross10G_SAVE);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	
	//TxCal 1G
	status = Ini_PutString (g_myInifile, "TxCal1G", "TxCal1G_MODE_E", my_struControl.TxCal1G_MODE_E);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TxCal1G", "TxCal1G_MODE_F", my_struControl.TxCal1G_MODE_F);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TxCal1G", "TxCal1G_ADD_MIN", my_struControl.TxCal1G_ADD_MIN);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TxCal1G", "TxCal1G_ADD_MAX", my_struControl.TxCal1G_ADD_MAX);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TxCal1G", "TxCal1G_ADC_GET", my_struControl.TxCal1G_ADC_GET);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TxCal1G", "TxCal1G_SLOPE_ADD", my_struControl.TxCal1G_SLOPE_ADD);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TxCal1G", "TxCal1G_SLOPE_AMP", my_struControl.TxCal1G_SLOPE_AMP);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TxCal1G", "TxCal1G_OFFSET_AMP", my_struControl.TxCal1G_OFFSET_AMP);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	
	//TxCal 10G
	status = Ini_PutString (g_myInifile, "TxCal10G", "TxCal10G_MODE_E", my_struControl.TxCal10G_MODE_E);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TxCal10G", "TxCal10G_MODE_F", my_struControl.TxCal10G_MODE_F);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TxCal10G", "TxCal10G_ADD_MIN", my_struControl.TxCal10G_ADD_MIN);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TxCal10G", "TxCal10G_ADD_MAX", my_struControl.TxCal10G_ADD_MAX);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TxCal10G", "TxCal10G_ADC_GET", my_struControl.TxCal10G_ADC_GET);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TxCal10G", "TxCal10G_SLOPE_ADD", my_struControl.TxCal10G_SLOPE_ADD);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TxCal10G", "TxCal10G_SLOPE_AMP", my_struControl.TxCal10G_SLOPE_AMP);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TxCal10G", "TxCal10G_OFFSET_AMP", my_struControl.TxCal10G_OFFSET_AMP);
	if(status) { MessagePopup("Inifile", "Error writing item from section"); goto  Error;}
	
	//TunAPD
	status = Ini_PutString (g_myInifile, "TunAPD", "TunAPD_MODE_M", my_struControl.TunAPD_MODE_M);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
    status = Ini_PutString (g_myInifile, "TunAPD", "TunAPD_MODE_A", my_struControl.TunAPD_MODE_A);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TunAPD", "TunAPD_DAC_SET", my_struControl.TunAPD_DAC_SET);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TunAPD", "TunAPD_DAC_SET_RANGE", my_struControl.TunAPD_DAC_SET_RANGE);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunAPD", "TunAPD_LUT_NAME_INDEX", my_struControl.TunAPD_LUT_NAME_INDEX);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunAPD", "TunAPD_LUK_INDEX", my_struControl.TunAPD_LUK_INDEX);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunAPD", "TunAPD_LUK_SLOPE_AMP", my_struControl.TunAPD_LUK_SLOPE_AMP);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunAPD", "TunAPD_LUK_OFFSET_AMP", my_struControl.TunAPD_LUK_OFFSET_AMP);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;} 
	status = Ini_PutString (g_myInifile, "TunAPD", "TunAPD_SAVE", my_struControl.TunAPD_SAVE);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	
	//TunSD
	status = Ini_PutString (g_myInifile, "TunSD", "TunSD_MODE_M", my_struControl.TunSD_MODE_M);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
    status = Ini_PutString (g_myInifile, "TunSD", "TunSD_MODE_A", my_struControl.TunSD_MODE_A);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TunSD", "TunSD_DAC_SET", my_struControl.TunSD_DAC_SET);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TunSD", "TunSD_FUN_OFF", my_struControl.TunSD_FUN_OFF);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "TunSD", "TunSD_FUN_ON", my_struControl.TunSD_FUN_ON);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunSD", "TunSD_LUT_NAME_INDEX", my_struControl.TunSD_LUT_NAME_INDEX);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunSD", "TunSD_LUK_INDEX", my_struControl.TunSD_LUK_INDEX);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunSD", "TunSD_LUK_SLOPE_AMP", my_struControl.TunSD_LUK_SLOPE_AMP);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "TunSD", "TunSD_LUK_OFFSET_AMP", my_struControl.TunSD_LUK_OFFSET_AMP);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;} 
	status = Ini_PutString (g_myInifile, "TunSD", "TunSD_SAVE", my_struControl.TunSD_SAVE);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}

	//RxCal
	status = Ini_PutString (g_myInifile, "RxCal", "RxCal_MODE_E", my_struControl.RxCal_MODE_E);
	if(status) { MessagePopup("Inifile", "APDror writing item from section"); goto  Error;}
    status = Ini_PutString (g_myInifile, "RxCal", "RxCal_MODE_F", my_struControl.RxCal_MODE_F);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "RxCal", "RxCal_MODE_T", my_struControl.RxCal_MODE_T);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "RxCal", "RxCal_ADC_GET", my_struControl.RxCal_ADC_GET);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "RxCal", "RxCal_LUT_NAME_INDEX", my_struControl.RxCal_LUT_NAME_INDEX);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "RxCal", "RxCal_LUK_INDEX", my_struControl.RxCal_LUK_INDEX);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "RxCal", "RxCal_LUK_SLOPE_AMP", my_struControl.RxCal_LUK_SLOPE_AMP);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "RxCal", "RxCal_LUK_OFFSET_AMP", my_struControl.RxCal_LUK_OFFSET_AMP);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;} 
	status = Ini_PutString (g_myInifile, "RxCal", "RxCal_SAVE", my_struControl.RxCal_SAVE);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "RxCal", "RxCal_SET_KL", my_struControl.RxCal_SET_KL);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "RxCal", "RxCal_SET_KH", my_struControl.RxCal_SET_KH);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutString (g_myInifile, "RxCal", "RxCal_SET_TEMP_PIONT", my_struControl.RxCal_SET_TEMP_PIONT);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;}
	status = Ini_PutInt (g_myInifile, "RxCal", "RxCal_RSSI_K_AMP", my_struControl.RxCal_RSSI_K_AMP);
	if(status) { MessagePopup("Inifile", "error writing item from section"); goto  Error;} 
	
	status = Ini_WriteToFile (g_myInifile, g_CtrFileName);
	if(status){MessagePopup("Inifile", "Error writing item from section"); goto  Error;}  
//释放ini文件句柄,执行结束后所有获取的值将被释放
	Ini_Dispose (g_myInifile);
	
	return TRUE;
Error:
//释放ini文件句柄
	Ini_Dispose (g_myInifile);
	return FALSE;
}

BOOL GetControl(void)
{
	IniText g_myInifile = 0;
	int 	status;
	int     index;
	char Section_Name[256];   
	int 	error = 0;             

	//创建ini文件句柄
	g_myInifile = Ini_New (0); 
	
	//指向ini文件并读取ini文件信息
	Ini_ReadFromFile (g_myInifile, g_CtrFileName); 
	
	//InitModule
	sprintf (Section_Name, "InitModule");  
	errChk(Ini_GetInt (g_myInifile, Section_Name, "Init_I2cAddr", &my_struControl.Init_I2cAddr));   
	errChk(Ini_GetInt (g_myInifile, Section_Name, "Init_PowerWord_Index", &my_struControl.Init_PowerWord_Index));  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "Init_GetVersion", my_struControl.Init_GetVersion, 50));  
	
	//GetBosaSN
	sprintf (Section_Name, "GetSN");  
	errChk(Ini_GetInt (g_myInifile, Section_Name, "GetSN_I2cAddr", &my_struControl.GetSN_I2cAddr));  
	errChk(Ini_GetInt (g_myInifile, Section_Name, "GetSN_Table", &my_struControl.GetSN_Table));  
	
	//GetTemper
	sprintf (Section_Name, "GetTemper");  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "GetTemper_CaseCommd", my_struControl.GetTemper_CaseCommd ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "GetTemper_CoreCommd", my_struControl.GetTemper_CoreCommd ,  50)); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "GetTemper_IndexOffset ", &my_struControl.GetTemper_IndexOffset ));  

	//TunAOP1G
	sprintf (Section_Name, "TunAOP1G");  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunAOP1G_MODE_M", my_struControl.TunAOP1G_MODE_M ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunAOP1G_MODE_A", my_struControl.TunAOP1G_MODE_A ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunAOP1G_DAC_SET", my_struControl.TunAOP1G_DAC_SET ,50)); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunAOP1G_LUT_NAME_INDEX", &my_struControl.TunAOP1G_LUT_NAME_INDEX));  
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunAOP1G_LUK_INDEX", &my_struControl.TunAOP1G_LUK_INDEX ));  
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunAOP1G_LUK_SLOPE_AMP", &my_struControl.TunAOP1G_LUK_SLOPE_AMP )); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunAOP1G_LUK_OFFSET_AMP", &my_struControl.TunAOP1G_LUK_OFFSET_AMP ));  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunAOP1G_SAVE", my_struControl.TunAOP1G_SAVE ,  50));  
	
	//TunAOP10G
	sprintf (Section_Name, "TunAOP10G");  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunAOP10G_MODE_M", my_struControl.TunAOP10G_MODE_M ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunAOP10G_MODE_A", my_struControl.TunAOP10G_MODE_A ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunAOP10G_DAC_SET", my_struControl.TunAOP10G_DAC_SET ,  50)); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunAOP10G_LUT_NAME_INDEX", &my_struControl.TunAOP10G_LUT_NAME_INDEX));
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunAOP10G_LUK_INDEX", &my_struControl.TunAOP10G_LUK_INDEX ));  
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunAOP10G_LUK_SLOPE_AMP", &my_struControl.TunAOP10G_LUK_SLOPE_AMP )); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunAOP10G_LUK_OFFSET_AMP", &my_struControl.TunAOP10G_LUK_OFFSET_AMP ));
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunAOP10G_SAVE", my_struControl.TunAOP10G_SAVE ,  50));  
	
	//TunER1G
	sprintf (Section_Name, "TunER1G");  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunER1G_MODE_M", my_struControl.TunER1G_MODE_M ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunER1G_MODE_A", my_struControl.TunER1G_MODE_A ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunER1G_DAC_SET", my_struControl.TunER1G_DAC_SET ,  50)); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunER1G_LUT_NAME_INDEX", &my_struControl.TunER1G_LUT_NAME_INDEX));
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunER1G_LUK_INDEX", &my_struControl.TunER1G_LUK_INDEX ));  
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunER1G_LUK_SLOPE_AMP", &my_struControl.TunER1G_LUK_SLOPE_AMP )); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunER1G_LUK_OFFSET_AMP", &my_struControl.TunER1G_LUK_OFFSET_AMP ));
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunER1G_SAVE", my_struControl.TunER1G_SAVE ,  50));  
	
	
	//TunER10G
	sprintf (Section_Name, "TunER10G");  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunER10G_MODE_M", my_struControl.TunER10G_MODE_M ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunER10G_MODE_A", my_struControl.TunER10G_MODE_A ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunER10G_DAC_SET", my_struControl.TunER10G_DAC_SET ,  50)); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunER10G_LUT_NAME_INDEX", &my_struControl.TunER10G_LUT_NAME_INDEX));
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunER10G_LUK_INDEX", &my_struControl.TunER10G_LUK_INDEX ));
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunER10G_LUK_SLOPE_AMP", &my_struControl.TunER10G_LUK_SLOPE_AMP )); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunER10G_LUK_OFFSET_AMP", &my_struControl.TunER10G_LUK_OFFSET_AMP ));
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunER10G_LUK_SLOPE_AMP", &my_struControl.TunER10G_LUK_SLOPE_AMP )); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunER10G_LUK_OFFSET_AMP", &my_struControl.TunER10G_LUK_OFFSET_AMP ));
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunER10G_SAVE", my_struControl.TunER10G_SAVE ,  50));
	
	//TunCross1G
	sprintf (Section_Name, "TunCross1G");  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunCross1G_MODE_M", my_struControl.TunCross1G_MODE_M ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunCross1G_MODE_A", my_struControl.TunCross1G_MODE_A ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunCross1G_DAC_SET", my_struControl.TunCross1G_DAC_SET ,  50)); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunCross1G_LUT_NAME_INDEX", &my_struControl.TunCross1G_LUT_NAME_INDEX));
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunCross1G_LUK_INDEX", &my_struControl.TunCross1G_LUK_INDEX ));  
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunCross1G_LUK_SLOPE_AMP", &my_struControl.TunCross1G_LUK_SLOPE_AMP )); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunCross1G_LUK_OFFSET_AMP", &my_struControl.TunCross1G_LUK_OFFSET_AMP ));
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunCross1G_SAVE", my_struControl.TunCross1G_SAVE ,  50));  
	
	
	//TunCross10G
	sprintf (Section_Name, "TunCross10G");  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunCross10G_MODE_M", my_struControl.TunCross10G_MODE_M ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunCross10G_MODE_A", my_struControl.TunCross10G_MODE_A ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunCross10G_DAC_SET", my_struControl.TunCross10G_DAC_SET ,  50)); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunCross10G_LUT_NAME_INDEX", &my_struControl.TunCross10G_LUT_NAME_INDEX));
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunCross10G_LUK_INDEX", &my_struControl.TunCross10G_LUK_INDEX ));
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunCross10G_LUK_SLOPE_AMP", &my_struControl.TunCross10G_LUK_SLOPE_AMP )); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunCross10G_LUK_OFFSET_AMP", &my_struControl.TunCross10G_LUK_OFFSET_AMP ));
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunCross10G_LUK_SLOPE_AMP", &my_struControl.TunCross10G_LUK_SLOPE_AMP )); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunCross10G_LUK_OFFSET_AMP", &my_struControl.TunCross10G_LUK_OFFSET_AMP ));
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunCross10G_SAVE", my_struControl.TunCross10G_SAVE ,  50));
	
	//TxCal1G
	sprintf (Section_Name, "TxCal1G");  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TxCal1G_MODE_E", my_struControl.TxCal1G_MODE_E ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TxCal1G_MODE_F", my_struControl.TxCal1G_MODE_F ,  50));  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TxCal1G_ADD_MIN", my_struControl.TxCal1G_ADD_MIN,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TxCal1G_ADD_MAX", my_struControl.TxCal1G_ADD_MAX,  50));   
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TxCal1G_ADC_GET", my_struControl.TxCal1G_ADC_GET,  50));
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TxCal1G_SLOPE_ADD", my_struControl.TxCal1G_SLOPE_ADD,  50));  
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TxCal1G_SLOPE_AMP", &my_struControl.TxCal1G_SLOPE_AMP )); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TxCal1G_OFFSET_AMP", &my_struControl.TxCal1G_OFFSET_AMP ));

	//TxCal10G
	sprintf (Section_Name, "TxCal10G");  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TxCal10G_MODE_E", my_struControl.TxCal10G_MODE_E ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TxCal10G_MODE_F", my_struControl.TxCal10G_MODE_F ,  50));  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TxCal10G_ADD_MIN", my_struControl.TxCal10G_ADD_MIN,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TxCal10G_ADD_MAX", my_struControl.TxCal10G_ADD_MAX,  50));   
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TxCal10G_ADC_GET", my_struControl.TxCal10G_ADC_GET,  50));  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TxCal10G_SLOPE_ADD", my_struControl.TxCal10G_SLOPE_ADD,  50));    
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TxCal10G_SLOPE_AMP", &my_struControl.TxCal10G_SLOPE_AMP )); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TxCal10G_OFFSET_AMP", &my_struControl.TxCal10G_OFFSET_AMP ));
	
	
	//TunAPD
	sprintf (Section_Name, "TunAPD");  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunAPD_MODE_M"			, my_struControl.TunAPD_MODE_M ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunAPD_MODE_A"			, my_struControl.TunAPD_MODE_A ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunAPD_DAC_SET"			, my_struControl.TunAPD_DAC_SET , 50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunAPD_DAC_SET_RANGE"	, my_struControl.TunAPD_DAC_SET_RANGE,50));  
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunAPD_LUT_NAME_INDEX"				,&my_struControl.TunAPD_LUT_NAME_INDEX));
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunAPD_LUK_INDEX"					,&my_struControl.TunAPD_LUK_INDEX ));
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunAPD_LUK_SLOPE_AMP"				,&my_struControl.TunAPD_LUK_SLOPE_AMP )); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunAPD_LUK_OFFSET_AMP"				,&my_struControl.TunAPD_LUK_OFFSET_AMP ));
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunAPD_SAVE"			, my_struControl.TunAPD_SAVE ,50));
	
	//TunSD
	sprintf (Section_Name, "TunSD");  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunSD_MODE_M"			, my_struControl.TunSD_MODE_M ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunSD_MODE_A"			, my_struControl.TunSD_MODE_A ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunSD_DAC_SET"			, my_struControl.TunSD_DAC_SET , 50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunSD_FUN_OFF"		, my_struControl.TunSD_FUN_OFF,50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunSD_FUN_ON"	, my_struControl.TunSD_FUN_ON,50));  
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunSD_LUT_NAME_INDEX"				,&my_struControl.TunSD_LUT_NAME_INDEX));
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunSD_LUK_INDEX"						,&my_struControl.TunSD_LUK_INDEX ));
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunSD_LUK_SLOPE_AMP"					,&my_struControl.TunSD_LUK_SLOPE_AMP )); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "TunSD_LUK_OFFSET_AMP"				,&my_struControl.TunSD_LUK_OFFSET_AMP ));
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "TunSD_SAVE"				, my_struControl.TunSD_SAVE ,50));
	
	//RxCal
	sprintf (Section_Name, "RxCal");  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "RxCal_MODE_E"			, my_struControl.RxCal_MODE_E ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "RxCal_MODE_F"			, my_struControl.RxCal_MODE_F ,  50)); 
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "RxCal_MODE_T"			, my_struControl.RxCal_MODE_T ,  50)); 	
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "RxCal_ADC_GET"			, my_struControl.RxCal_ADC_GET , 50)); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "RxCal_LUT_NAME_INDEX"				,&my_struControl.RxCal_LUT_NAME_INDEX));
	errChk(Ini_GetInt (g_myInifile, Section_Name, "RxCal_LUK_INDEX"						,&my_struControl.RxCal_LUK_INDEX ));
	errChk(Ini_GetInt (g_myInifile, Section_Name, "RxCal_LUK_SLOPE_AMP"					,&my_struControl.RxCal_LUK_SLOPE_AMP )); 
	errChk(Ini_GetInt (g_myInifile, Section_Name, "RxCal_LUK_OFFSET_AMP"				,&my_struControl.RxCal_LUK_OFFSET_AMP ));
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "RxCal_SAVE"				, my_struControl.RxCal_SAVE ,  50));
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "RxCal_SET_KL"			, my_struControl.RxCal_SET_KL ,50));  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "RxCal_SET_KH"			, my_struControl.RxCal_SET_KH ,50));  
	errChk(Ini_GetStringIntoBuffer (g_myInifile, Section_Name, "RxCal_SET_TEMP_PIONT"	, my_struControl.RxCal_SET_TEMP_PIONT ,  50));  
	errChk(Ini_GetInt (g_myInifile, Section_Name, "RxCal_RSSI_K_AMP"					,&my_struControl.RxCal_RSSI_K_AMP )); 
Error:
//释放ini文件句柄
	//释放ini文件句柄,执行结束后所有获取的值将被释放
	Ini_Dispose (g_myInifile);
	return error;
}

BOOL DB_Initial(void)
{
	char buf[256];
	int  i, error; 
	int resCode = 0;   /* Result code                      */ 

	error = MyDLL_DB_Init (&hdbc);
    
    /************for ate spec new table***********************************/
    //添加数据库读写部分操作
	hmap = DBBeginMap (hdbc);   
	if(hmap <= 0) {ShowDataBaseError();  goto Error;}
	
	resCode = DBMapColumnToChar (hmap, "PartNumber", 50, my_struDBConfig.PartNumber, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;}
    
   	resCode = DBMapColumnToChar (hmap, "Version", 50, my_struDBConfig.Version, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
    resCode = DBMapColumnToChar (hmap, "DT_Flag", 50, my_struDBConfig.DT_Flag, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;}
    
   	resCode = DBMapColumnToChar (hmap, "Temper_Flag", 50, my_struDBConfig.Temper_Flag, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
	resCode = DBMapColumnToDouble (hmap, "TemperMin", &my_struDBConfig.TemperMin, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
   	resCode = DBMapColumnToDouble (hmap, "TemperMax", &my_struDBConfig.TemperMax, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 

	resCode = DBMapColumnToDouble (hmap, "TxPowMin", &my_struDBConfig.TxPowMin, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
   	resCode = DBMapColumnToDouble (hmap, "TxPowMax", &my_struDBConfig.TxPowMax, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
	
	resCode = DBMapColumnToDouble (hmap, "TxBiasMin", &my_struDBConfig.TxBiasMin, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
   	resCode = DBMapColumnToDouble (hmap, "TxBiasMax", &my_struDBConfig.TxBiasMax, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
	resCode = DBMapColumnToDouble (hmap, "TxErMin", &my_struDBConfig.TxErMin, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
   	resCode = DBMapColumnToDouble (hmap, "TxErMax", &my_struDBConfig.TxErMax, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
	
	resCode = DBMapColumnToDouble (hmap, "TxCrossMin", &my_struDBConfig.TxCrossMin, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 

	resCode = DBMapColumnToDouble (hmap, "TxCrossMax", &my_struDBConfig.TxCrossMax, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
	
	resCode = DBMapColumnToDouble (hmap, "TxMargin", &my_struDBConfig.TxMargin, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
	
	resCode = DBMapColumnToDouble (hmap, "TxTjMax", &my_struDBConfig.TxTjMax, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;}
    
   	resCode = DBMapColumnToDouble (hmap, "TxPPjMax", &my_struDBConfig.TxPPjMax, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
	resCode = DBMapColumnToDouble (hmap, "TxRiseMax", &my_struDBConfig.TxRiseMax, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
   	resCode = DBMapColumnToDouble (hmap, "TxFallMax", &my_struDBConfig.TxFallMax, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 

	resCode = DBMapColumnToDouble (hmap, "TxDisPowMax", &my_struDBConfig.TxDisPowMax, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
   	resCode = DBMapColumnToDouble (hmap, "TrackingErrorMax", &my_struDBConfig.TrackingErrorMax, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 

	resCode = DBMapColumnToDouble (hmap, "PathPenaltyMax", &my_struDBConfig.PathPenaltyMax, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
   	resCode = DBMapColumnToDouble (hmap, "PathPenaltyTime", &my_struDBConfig.PathPenaltyTime, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;}   
 
	resCode = DBMapColumnToDouble (hmap, "OverLoad", &my_struDBConfig.Over, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
   	resCode = DBMapColumnToDouble (hmap, "OverLoadTime", &my_struDBConfig.OverTime, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
	resCode = DBMapColumnToDouble (hmap, "Sens", &my_struDBConfig.Sens, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
   	resCode = DBMapColumnToDouble (hmap, "SensTime", &my_struDBConfig.SensTime, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
	
	resCode = DBMapColumnToDouble (hmap, "Sens_Center", &my_struDBConfig.Sens_Center, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
   	resCode = DBMapColumnToDouble (hmap, "SensTime_Center", &my_struDBConfig.SensTime_Center, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;}

   	resCode = DBMapColumnToDouble (hmap, "SDAssert", &my_struDBConfig.SDA, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
	resCode = DBMapColumnToDouble (hmap, "SDDeassert", &my_struDBConfig.SDD, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
   	resCode = DBMapColumnToDouble (hmap, "SDHysMin", &my_struDBConfig.SDHysMin, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;}
    
    resCode = DBMapColumnToDouble (hmap, "SDHysMax", &my_struDBConfig.SDHysMax, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;}
	
	resCode = DBMapColumnToDouble (hmap, "Sigma", &my_struDBConfig.Sigma, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
   	resCode = DBMapColumnToDouble (hmap, "BandWidth", &my_struDBConfig.BandWidth, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;}
    
    resCode = DBMapColumnToDouble (hmap, "SMSR", &my_struDBConfig.SMSR, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;}   

   	resCode = DBMapColumnToDouble (hmap, "TxVMax", &my_struDBConfig.TxVMax, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
	resCode = DBMapColumnToDouble (hmap, "TxIMax", &my_struDBConfig.TxIMax, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
   	resCode = DBMapColumnToDouble (hmap, "RxVMax", &my_struDBConfig.RxVMax, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;}
    
    resCode = DBMapColumnToDouble (hmap, "RxIMax", &my_struDBConfig.RxIMax, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
	
	resCode = DBMapColumnToDouble (hmap, "PeakWavelengthMin", &my_struDBConfig.PeakWavelengthMin, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;}  
   	resCode = DBMapColumnToDouble (hmap, "PeakWavelengthMax", &my_struDBConfig.PeakWavelengthMax, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
 
   	resCode = DBMapColumnToDouble (hmap, "PeakWavelengthMin01", &my_struDBConfig.PeakWavelengthMin01, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
   	resCode = DBMapColumnToDouble (hmap, "PeakWavelengthMax01", &my_struDBConfig.PeakWavelengthMax01, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
	resCode = DBMapColumnToDouble (hmap, "PeakWavelengthMin02", &my_struDBConfig.PeakWavelengthMin02, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;}  
   	resCode = DBMapColumnToDouble (hmap, "PeakWavelengthMax02", &my_struDBConfig.PeakWavelengthMax02, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
	
	resCode = DBMapColumnToDouble (hmap, "PeakWavelengthMin03", &my_struDBConfig.PeakWavelengthMin03, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
   	resCode = DBMapColumnToDouble (hmap, "PeakWavelengthMax03", &my_struDBConfig.PeakWavelengthMax03, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
	
	resCode = DBMapColumnToDouble (hmap, "PeakWavelengthMin04", &my_struDBConfig.PeakWavelengthMin04, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;}  
   	resCode = DBMapColumnToDouble (hmap, "PeakWavelengthMax04", &my_struDBConfig.PeakWavelengthMax04, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 

    /************end for ate spec_new table***********************************/
    
    //----------------------------for EEPROM-----------------------------//
    hamp_E2 = DBBeginMap (hdbc);   
	if(hamp_E2 <= 0) {ShowDataBaseError();  goto Error;}
    
    resCode = DBMapColumnToChar (hamp_E2, "PartNumber", 50, my_struDBConfig_E2.PN, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;}
    
   	resCode = DBMapColumnToChar (hamp_E2, "Version", 10, my_struDBConfig_E2.Ver, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
    resCode = DBMapColumnToInt (hamp_E2, "A2", &my_struDBConfig_E2.E2_Flag, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;}
	
	resCode = DBMapColumnToChar (hamp_E2, "table_flag", 50, my_struDBConfig_E2.table_flag, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;} 
    
    for (i=0;i <128; i++)
    {
    	sprintf (buf, "Address%d", i);
    	resCode = DBMapColumnToChar (hamp_E2, buf, 5, my_struDBConfig_E2.E2_str[i], &DBConfigStat, "");
    	if (resCode != DB_SUCCESS) {ShowDataBaseError();  goto Error;}
    }
    //----------------------------for EEPROM-----------------------------// 
	return TRUE;
Error:
	return FALSE;
}

BOOL DB_GET_Spec_BarCode_Ver(char Ver[][10])
{
char buf[256];
char myVer[50];
int	num;

	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */ 

	num=0;
	memset (myVer, 0, 50);
	
	Fmt (buf, "SELECT Version from AutoDT_Spec_BarCode where PN='%s' ORDER BY Version ASC", my_struConfig.PN);

	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return FALSE;}  
    
    resCode = DBBindColChar (hstmt, 1, 50, myVer, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
    
    while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
    {strcpy (Ver[num], myVer);num++;}      
    
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) {ShowDataBaseError();  return FALSE;}
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	if (num==0) {MessagePopup("提示","数据库AutoDT_Spec_BarCode中不能找到相应的数据！"); return FALSE;} 
	
	return TRUE;
}

BOOL DB_GET_EED_Ver(char Ver[][10])
{
char buf[256];
char myVer[10];
int	num;

	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */ 

	Fmt (buf, "SELECT Version from AutoDT_Spec_E2_Data where PartNumber='%s' and A2=0 ORDER BY Version ASC", my_struConfig.PN);

	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return FALSE;}  
    
    resCode = DBBindColChar (hstmt, 1, 10, myVer, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
    
    num=0;
    while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
    {
    	strcpy (Ver[num],myVer); 
    	num++;
    }      
    
    if (resCode != DB_SUCCESS && resCode != DB_EOF) {ShowDataBaseError();  return FALSE;}
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	if (num==0) {MessagePopup("提示","数据库AutoDT_Spec_E2_Data中不能找到相应的数据！"); return FALSE;} 

	return TRUE;
}

BOOL DB_GET_BarCode(void)
{
int num, mySNLength;
char buf[256];
char myBarCode[50];

	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */ 

	num=0;
	memset (my_struDBConfig.BarCode, 0, 10);
	memset (myBarCode, 0, 50); 
	my_struDBConfig.SNLength=0;
	
	Fmt (buf, "SELECT PN,BarCode,SNLength from AutoDT_Spec_BarCode where PN='%s' and Version='%s'", my_struConfig.PN, my_struConfig.Ver);
	
	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return FALSE;}  
    
    resCode = DBBindColChar (hstmt, 2, 50, myBarCode, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
    resCode = DBBindColInt (hstmt, 3, &mySNLength, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
    
    while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) {num++;}      
    
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) {ShowDataBaseError();  return FALSE;}
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	if (num!=1) {MessagePopup("Error","Get Barcode fail  !"); return FALSE;} 
	
 //	myBarCode[3]=0; 
	MyDLLCheckSN (myBarCode);   
	
	strcpy (my_struDBConfig.BarCode, myBarCode);
	my_struDBConfig.SNLength=mySNLength;
	
	return TRUE;
}

BOOL DB_GET_EEPROM(void)
{
int count;  
struct  struDBConfig_E2 temp_struDBConfig_E2; 

	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */ 

	//for A0 Lower	
	hstmt = DBActivateMap (hamp_E2, "AutoDT_Spec_E2_Data");
    if(hstmt<=0) {ShowDataBaseError();  return FALSE;} 
    
    count=0;
    while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
    {
		if(	(strcmp(my_struDBConfig_E2.PN, my_struConfig.PN)==0) && (strcmp(my_struDBConfig_E2.Ver, my_struConfig.Ver_E2)==0) && my_struDBConfig_E2.E2_Flag==0 && (stricmp(my_struDBConfig_E2.table_flag, "LOWER")==0))
			{
				count++;
				temp_struDBConfig_E2=my_struDBConfig_E2;
			}
    }      
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) {ShowDataBaseError();  return FALSE;}  
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	

	if (count!=1) {MessagePopup("提示","数据库Spec_E2_Data中不能找到相应的A0 Lower数据或者数据记录多于一条！"); return FALSE;}
	
	my_struDBConfig_E2 = temp_struDBConfig_E2; 
	for (count=0; count<128; count++)
  	{
		sscanf (my_struDBConfig_E2.E2_str[count], "%x", &my_struDBConfig_E2.A0_Lower[count]);
	}  
	
	//for A0 Table1	
	hstmt = DBActivateMap (hamp_E2, "AutoDT_Spec_E2_Data");
    if(hstmt<=0) {ShowDataBaseError();  return FALSE;} 
    
    count=0;
    while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
    {
		if(	(strcmp(my_struDBConfig_E2.PN, my_struConfig.PN)==0) && (strcmp(my_struDBConfig_E2.Ver, my_struConfig.Ver_E2)==0) && my_struDBConfig_E2.E2_Flag==0 && (stricmp(my_struDBConfig_E2.table_flag, "TABLE1")==0))
			{count++;temp_struDBConfig_E2=my_struDBConfig_E2;}
    }      
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) {ShowDataBaseError();  return FALSE;}  
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	

	if (count!=1) {MessagePopup("提示","数据库Spec_E2_Data中不能找到相应的A0 Table1数据或者数据记录多于一条！"); return FALSE;}
	
	my_struDBConfig_E2 = temp_struDBConfig_E2; 
	for (count=0; count<128; count++)
  	{sscanf (my_struDBConfig_E2.E2_str[count], "%x", &my_struDBConfig_E2.A0_Table1[count]);}  
	
	return TRUE;
}

BOOL DB_GET_EEPROM_Ex(void)
{
int count, A2_flag, i=0;
//struct  struDBConfig_E2	temp_struDBConfig_E2;
char buf[1024], table_flag[50];
int hstmt;
int resCode;
    
	count=0;
	A2_flag = 0;
	memset(table_flag, 0, sizeof(table_flag));
	my_struConfig.A0_low = FALSE;
    my_struConfig.A0_tab1 = FALSE;
    my_struConfig.A0_tab0 = FALSE;
    my_struConfig.A0_tab2 = FALSE;
    my_struConfig.A2_low = FALSE;
    my_struConfig.A2_tab0 = FALSE;
	my_struConfig.A2_tab1 = FALSE;
	for(i=0; i<128; i++)
	{
		my_struDBConfig_E2.A0[i] = 0;
		my_struDBConfig_E2.A0_Table0[i] = 0;
		my_struDBConfig_E2.A0_Table1[i] = 0;
		my_struDBConfig_E2.A0_Table2[i] = 0;
		my_struDBConfig_E2.A2[i] = 0;
		my_struDBConfig_E2.A2_Table0[i] = 0;
		my_struDBConfig_E2.A2_Table1[i] = 0; 
	}
	
	//预先查询是否有A2地址的数据来决定是否要检查A2地址
	sprintf (buf, "SELECT A2, table_flag FROM AutoDT_Spec_E2_Data WHERE PartNumber='%s' and Version='%s'", my_struConfig.PN, my_struConfig.Ver_E2);
	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return FALSE;}     
	
	count=DBNumberOfRecords(hstmt);
	
	resCode = DBBindColInt (hstmt, 1, &A2_flag, &DBConfigStat);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColChar (hstmt, 2, 50, table_flag, &DBConfigStat, "");
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
	{
		if (0 == A2_flag)
		{
			if (0 == strcmp(table_flag, ""))
			{
				my_struConfig.A0_low = TRUE;   
			}
			if (0 == stricmp(table_flag, "LOWER"))
			{
				my_struConfig.A0_low = TRUE; 
			}
			if (0 == stricmp(table_flag, "TABLE1"))     
			{
				my_struConfig.A0_tab1 = TRUE;   
			}
			if (0 == stricmp(table_flag, "TABLE2"))
			{
				my_struConfig.A0_tab2 = TRUE; 
			}
			if (0 == stricmp(table_flag, "TABLE0")) 
			{
				my_struConfig.A0_tab0 = TRUE;    
			}
		}
		if (1 == A2_flag)
		{
			if (0 == strcmp(table_flag, ""))
			{
				my_struConfig.A2_low = TRUE;  
				//my_struDBConfig_E2.A2OK = TRUE;  
			}
			if (0 == stricmp(table_flag, "LOWER"))
			{
				my_struConfig.A2_low = TRUE;    
				//my_struDBConfig_E2.A2OK = TRUE;  
			}
			if (0 == stricmp(table_flag, "TABLE0")) 
			{
				my_struConfig.A2_tab0 = TRUE; 
			}
			if (0 == stricmp(table_flag, "TABLE1")) 
			{
				my_struConfig.A2_tab1 = TRUE;    
			}
		}
	}
	if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) {ShowDataBaseError(); return FALSE;}   
	
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	if (count==0) {MessagePopup("提示","数据库AutoDT_Spec_E2_Data中不能找到数据！      "); return FALSE;}
	
	Delay(0.5);

	if (my_struConfig.A0_low)
	{
		sprintf (buf, "SELECT * FROM AutoDT_Spec_E2_Data WHERE PartNumber='%s' and Version='%s' and A2=0 and (table_flag is NULL or table_flag='LOWER')",  my_struConfig.PN, my_struConfig.Ver_E2);
		hstmt = DBActivateSQL (hdbc, buf);
	    if (hstmt <= 0) {ShowDataBaseError(); return FALSE;}    
		
		for (i=0;i <128; i++)
	    {
	    	resCode = DBBindColChar (hstmt, i+7, 5, my_struDBConfig_E2.E2_str[i], &DBConfigStat, "");
	    	if (resCode != DB_SUCCESS) {ShowDataBaseError();  return FALSE;}
	    }
		resCode = DBFetchNext (hstmt); 
		if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	   
		
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
		
		for (count=0; count<128; count++)
  		{sscanf (my_struDBConfig_E2.E2_str[count], "%x", &my_struDBConfig_E2.A0_Lower[count]);}  
	}
	if (my_struConfig.A0_tab1)
	{
		sprintf (buf, "SELECT * FROM AutoDT_Spec_E2_Data WHERE PartNumber='%s' and Version='%s' and A2=0 and table_flag='TABLE1'",  my_struConfig.PN, my_struConfig.Ver_E2);
		hstmt = DBActivateSQL (hdbc, buf);
	    if (hstmt <= 0) {ShowDataBaseError(); return FALSE;}    
		
		for (i=0;i <128; i++)
	    {
	    	resCode = DBBindColChar (hstmt, i+7, 5, my_struDBConfig_E2.E2_str[i], &DBConfigStat, "");
	    	if (resCode != DB_SUCCESS) {ShowDataBaseError();  return FALSE;}
	    }
		resCode = DBFetchNext (hstmt); 
		if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	 
		
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
		
		for (count=0; count<128; count++)
  		{sscanf (my_struDBConfig_E2.E2_str[count], "%x", &my_struDBConfig_E2.A0_Table1[count]);}  
	}
	if (my_struConfig.A0_tab2)
	{
		sprintf (buf, "SELECT * FROM AutoDT_Spec_E2_Data WHERE PartNumber='%s' and Version='%s' and A2=0 and table_flag='TABLE2'",  my_struConfig.PN, my_struConfig.Ver_E2);
		hstmt = DBActivateSQL (hdbc, buf);
	    if (hstmt <= 0) {ShowDataBaseError(); return FALSE;}    
		
		for (i=0;i <128; i++)
	    {
	    	resCode = DBBindColChar (hstmt, i+7, 5, my_struDBConfig_E2.E2_str[i], &DBConfigStat, "");
	    	if (resCode != DB_SUCCESS) {ShowDataBaseError();  return FALSE;}
	    }
		resCode = DBFetchNext (hstmt); 
		if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	 
		
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
		
		for (count=0; count<128; count++)
  		{sscanf (my_struDBConfig_E2.E2_str[count], "%x", &my_struDBConfig_E2.A0_Table2[count]);}  
	}
	if (my_struConfig.A0_tab0)
	{
		sprintf (buf, "SELECT * FROM AutoDT_Spec_E2_Data WHERE PartNumber='%s' and Version='%s' and A2=0 and table_flag='TABLE0'",  my_struConfig.PN, my_struConfig.Ver_E2);
		hstmt = DBActivateSQL (hdbc, buf);
	    if (hstmt <= 0) {ShowDataBaseError(); return FALSE;}    
		
		for (i=0;i <128; i++)
	    {
	    	resCode = DBBindColChar (hstmt, i+7, 5, my_struDBConfig_E2.E2_str[i], &DBConfigStat, "");
	    	if (resCode != DB_SUCCESS) {ShowDataBaseError();  return FALSE;}
	    }
		resCode = DBFetchNext (hstmt); 
		if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	 
		
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
		
		for (count=0; count<128; count++)
  		{sscanf (my_struDBConfig_E2.E2_str[count], "%x", &my_struDBConfig_E2.A0_Table0[count]);}  
	}
	if (my_struConfig.A2_low)
	{
		sprintf (buf, "SELECT * FROM AutoDT_Spec_E2_Data WHERE PartNumber='%s' and Version='%s' and A2=1 and (table_flag is NULL or table_flag='LOWER')",  my_struConfig.PN, my_struConfig.Ver_E2);
		hstmt = DBActivateSQL (hdbc, buf);
	    if (hstmt <= 0) {ShowDataBaseError(); return FALSE;}    
		
		for (i=0;i <128; i++)
	    {
	    	resCode = DBBindColChar (hstmt, i+7, 5, my_struDBConfig_E2.E2_str[i], &DBConfigStat, "");
	    	if (resCode != DB_SUCCESS) {ShowDataBaseError();  return FALSE;}
	    }
		resCode = DBFetchNext (hstmt); 
		if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	 
		
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
		
		for (count=0; count<128; count++)
  		{sscanf (my_struDBConfig_E2.E2_str[count], "%x", &my_struDBConfig_E2.A2[count]);}  
	}
	if (my_struConfig.A2_tab0)
	{
		sprintf (buf, "SELECT * FROM AutoDT_Spec_E2_Data WHERE PartNumber='%s' and Version='%s' and A2=1 and table_flag='TABLE0' ",  my_struConfig.PN, my_struConfig.Ver_E2);
		hstmt = DBActivateSQL (hdbc, buf);
	    if (hstmt <= 0) {ShowDataBaseError(); return FALSE;}    
		
		for (i=0;i <128; i++)
	    {
	    	resCode = DBBindColChar (hstmt, i+7, 5, my_struDBConfig_E2.E2_str[i], &DBConfigStat, "");
	    	if (resCode != DB_SUCCESS) {ShowDataBaseError();  return FALSE;}
	    }
		resCode = DBFetchNext (hstmt); 
		if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	 
		
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
		
		for (count=0; count<128; count++)
  		{sscanf (my_struDBConfig_E2.E2_str[count], "%x", &my_struDBConfig_E2.A2_Table0[count]);}  
	}
	if (my_struConfig.A2_tab1)
	{
		sprintf (buf, "SELECT * FROM AutoDT_Spec_E2_Data WHERE PartNumber='%s' and Version='%s' and A2=1 and table_flag='TABLE1' ",  my_struConfig.PN, my_struConfig.Ver_E2);
		hstmt = DBActivateSQL (hdbc, buf);
	    if (hstmt <= 0) {ShowDataBaseError(); return FALSE;}    
		
		for (i=0;i <128; i++)
	    {
	    	resCode = DBBindColChar (hstmt, i+7, 5, my_struDBConfig_E2.E2_str[i], &DBConfigStat, "");
	    	if (resCode != DB_SUCCESS) {ShowDataBaseError();  return FALSE;}
	    }
		resCode = DBFetchNext (hstmt); 
		if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	 
		
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
		
		for (count=0; count<128; count++)
  		{sscanf (my_struDBConfig_E2.E2_str[count], "%x", &my_struDBConfig_E2.A2_Table1[count]);}  
	}
    
	return TRUE;
}
// 获取数据库常温核温
BOOL GetDBNormalCoreTemper(double *CoreTemper, char *opticssn)
{
	char buf[2048] = {0};
	int hstmt = 0;
	char testflag[] = "TUN_10G";
	char statustmp[] = "PASS";
	
	int resCode = 0;
	int count = 0;
	/******************for autodt_results_ate_new1****************************/
	//select * from autodt_results_ate_new1 where id = (select max(ID) from autodt_results_ate_new1 where PartNumber='SO04D199-PLGA-220-HXT' and Version='BOM000895'and TESTFLAG='TUN_10G' and STATUS='PASS')
	//Fmt (buf, "Select A2_TEMPERATRUE 
	//From  autodt_results_ate_new1 Where PartNumber='%s' and Version='%s' and MODULESN='%s' ", my_struDBConfig.PartNumber, my_struDBConfig.Version, modesn);
	Fmt (buf, "Select A2_TEMPERATRUE \
	From  autodt_results_ate_new1 Where id = (select max(ID) from autodt_results_ate_new1 where PartNumber='%s' and Version='%s' and TESTFLAG='%s' and STATUS='%s' and OPTICSSN='%s') ", my_struDBConfig.PartNumber, my_struDBConfig.Version, testflag, statustmp, opticssn);																																												
	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return -1;}   

	resCode = DBBindColDouble (hstmt,1, CoreTemper, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}

	
    while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) {count++;} 
	
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) {ShowDataBaseError(); return FALSE;}
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	if (count!=1) 
	{
		MessagePopup("提示","数据库AutoDT_Spec_ERCompens_New中不能找到相应的数据或者数据记录多于一条！"); 
		return FALSE;
	}
	return TRUE;
}


BOOL DB_GET_ConfigInfo(void) 
{
int count;
struct	struDBConfig temp_struDBConfig;	   //定义一个临时结构体变量
struct  struDBConfig_E2 temp_struDBConfig_E2; 
char buf[2048];
char myRSSITime[50];  

char tmp_DriverChip[512] = "";

	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */ 

	/******************for AutoDT_Spec_ATE_NEW****************************/
	hstmt = DBActivateMap (hmap, "AutoDT_Spec_ATE_NEW");
    if(hstmt<=0) {ShowDataBaseError();  return FALSE;}   

    count=0;
    while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
    	{
    		if(	(strcmp(my_struDBConfig.PartNumber, my_struConfig.PN)==0) && (strcmp(my_struDBConfig.DT_Flag, my_struConfig.DT_Flag)==0) && 
    			(strcmp(my_struDBConfig.Temper_Flag, my_struConfig.Temper_Flag)==0) && (strcmp(my_struDBConfig.Version, my_struConfig.Ver)==0))
    			{
					count++; temp_struDBConfig=my_struDBConfig;
				} //将查询得到的数据存入临时变量,注意临时变量的数据只能保存最新的数据
    	}      
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) {ShowDataBaseError();  return FALSE;}  
    
   	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
	
	if (count!=1) {MessagePopup("提示","数据库AutoDT_Spec_ATE中不能找到相应的数据或者数据记录多于一条！"); return FALSE;}
	
	my_struDBConfig=temp_struDBConfig;   //将临时变量的数据保存到my_struDBConfig变量 		
	/******************for AutoDT_Spec_ATE****************************/
																																																																																							
	/******************for AutoDT_Spec_ERCompens_NEW****************************/
	Fmt (buf, "Select DRIVERCHIP,AMPLIFIERCHIP,ERCOMPENSCHIP,RATE,PRO_TYPE,MCU_CMD_TYPE,A0_PROTECT,RSSIMODE,RSSI_N1,RSSI_N2,RSSI_N3,RSSI_TRIG_LEVEL,RSSI_TIME,LTEMP_TARGET,EAVOL_TARGET,A2TEMPER_S,A2TEMPER_KL,A2TEMPER_BL,A2TEMPER_KH,A2TEMPER_BH, \
	RB_ADD_1G,RM_ADD_1G,  IB030_10_1G, IB10_33_1G, IB33_60_1G, IB60_82_1G ,IB82_105_1G, IM030_10_1G, IM10_33_1G,IM33_60_1G,  IM60_82_1G, IM105_82_1G, APDDAC_MIN_L_1G, APDDAC_MAX_L_1G, APDDAC_MIN_R_1G, APDDAC_MAX_R_1G, APDDAC_MIN_H_1G, APDDAC_MAX_H_1G, VAPD_KL_1G, VAPD_KH_1G, VBR_CLASS_1G, RSSI_KL_1G, RSSI_KH_1G, ISLOSORSD_1G, LOS_KL_1G, LOS_BL_1G, LOS_KH_1G, LOS_BH_1G, LOS_TEMPER_S_1G, \
  	RB_ADD_10G,RM_ADD_10G,IB030_10_10G,IB10_33_10G,IB33_60_10G,IB60_82_10G,IB82_105_10G,IM030_10_10G,IM10_33_10G,IM33_60_10G,IM60_82_10G,IM82_105_10G,APDDAC_MIN_L_10G,APDDAC_MAX_L_10G,APDDAC_MIN_R_10G,APDDAC_MAX_R_10G,APDDAC_MIN_H_10G,APDDAC_MAX_H_10G,VAPD_KL_10G,VAPD_KH_10G,VBR_CLASS_10G,RSSI_KL_10G,RSSI_KH_10G,ISLOSORSD_10G,LOS_KL_10G,LOS_BL_10G,LOS_KH_10G,LOS_BH_10G,LOS_TEMPER_S_10G \
	From  AutoDT_Spec_ERCompens_NEW Where PartNumber='%s' and Version='%s'", my_struDBConfig.PartNumber, my_struDBConfig.Version);
																																													
	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return -1;}   
	
    resCode = DBBindColChar (hstmt, 1, 50, my_struDBConfig_ERCompens.DriverChip, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColChar (hstmt, 2, 50, my_struDBConfig_ERCompens.AmplifierChip, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
    resCode = DBBindColChar (hstmt, 3, 50, my_struDBConfig_ERCompens.ERCompensChip, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt,4, &my_struDBConfig_ERCompens.Rate, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColChar (hstmt, 5, 50, my_struDBConfig_ERCompens.Pro_type, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
	resCode = DBBindColChar (hstmt, 6, 50, my_struDBConfig_ERCompens.MCU_CMD_TYPE, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
	resCode = DBBindColInt (hstmt, 7, &my_struDBConfig_ERCompens.A0_PROTECT, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}

	//RSSI 配置
	resCode = DBBindColInt (hstmt, 8, &my_struDBConfig_ERCompens.RSSIMODE, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColInt (hstmt, 9, &my_struDBConfig_ERCompens.RSSI_N1, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColInt (hstmt, 10, &my_struDBConfig_ERCompens.RSSI_N2, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColInt (hstmt, 11, &my_struDBConfig_ERCompens.RSSI_N3, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColInt (hstmt, 12, &my_struDBConfig_ERCompens.RSSI_TRIG_LEVEL, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColChar (hstmt, 13, 50, myRSSITime, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
  
	resCode = DBBindColChar (hstmt, 14, 50, my_struDBConfig_ERCompens.LTEMP_TARGET, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColChar (hstmt, 15, 50, my_struDBConfig_ERCompens.EAVOL_TARGET, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	//A2温度校准
	resCode = DBBindColDouble (hstmt, 16, &my_struDBConfig_ERCompens.A2TEMPER_S, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 17, &my_struDBConfig_ERCompens.A2TEMPER_KL, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 18, &my_struDBConfig_ERCompens.A2TEMPER_BL, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 19, &my_struDBConfig_ERCompens.A2TEMPER_KH, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 20, &my_struDBConfig_ERCompens.A2TEMPER_BH, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	//1G APC MOD DAC范围
	resCode = DBBindColChar (hstmt, 21, 50, my_struDBConfig_ERCompens.RB_ADD_1G, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColChar (hstmt, 22, 50, my_struDBConfig_ERCompens.RM_ADD_1G, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
	
	//1G APC系数  
	resCode = DBBindColDouble (hstmt, 23, &my_struDBConfig_ERCompens.IB030_10_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 24, &my_struDBConfig_ERCompens.IB10_33_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 25, &my_struDBConfig_ERCompens.IB33_60_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 26, &my_struDBConfig_ERCompens.IB60_82_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 27, &my_struDBConfig_ERCompens.IB82_105_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	//1G MOD系数  
	resCode = DBBindColDouble (hstmt, 28, &my_struDBConfig_ERCompens.IM030_10_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 29, &my_struDBConfig_ERCompens.IM10_33_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 30, &my_struDBConfig_ERCompens.IM33_60_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 31, &my_struDBConfig_ERCompens.IM60_82_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 32, &my_struDBConfig_ERCompens.IM82_105_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	//1G VAPD DAC三温度的范围；
	resCode = DBBindColInt (hstmt, 33, &my_struDBConfig_ERCompens.APDDAC_MIN_L_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColInt (hstmt, 34, &my_struDBConfig_ERCompens.APDDAC_MAX_L_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColInt (hstmt, 35, &my_struDBConfig_ERCompens.APDDAC_MIN_R_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColInt (hstmt, 36, &my_struDBConfig_ERCompens.APDDAC_MAX_R_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColInt (hstmt, 37, &my_struDBConfig_ERCompens.APDDAC_MIN_H_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColInt (hstmt, 38, &my_struDBConfig_ERCompens.APDDAC_MAX_H_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	//1G Vapd温度补偿系数 
	resCode = DBBindColDouble (hstmt, 39, &my_struDBConfig_ERCompens.VAPD_KL_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 40, &my_struDBConfig_ERCompens.VAPD_KH_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 41, &my_struDBConfig_ERCompens.VBR_CLASS_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	//1G RSSI补偿系数   
	resCode = DBBindColDouble (hstmt, 42, &my_struDBConfig_ERCompens.RSSI_KL_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 43, &my_struDBConfig_ERCompens.RSSI_KH_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	//1G LOS  系数 
	resCode = DBBindColInt (hstmt, 44, &my_struDBConfig_ERCompens.ISLOSORSD_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 45, &my_struDBConfig_ERCompens.LOS_KL_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 46, &my_struDBConfig_ERCompens.LOS_BL_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 47, &my_struDBConfig_ERCompens.LOS_KH_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 48, &my_struDBConfig_ERCompens.LOS_BH_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 49, &my_struDBConfig_ERCompens.LOS_TEMPER_S_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	 //10G APC MOD DAC范围
	resCode = DBBindColChar (hstmt, 50, 50, my_struDBConfig_ERCompens.RB_ADD_10G, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColChar (hstmt, 51, 50, my_struDBConfig_ERCompens.RM_ADD_10G, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
	
	//10G APC系数  
	resCode = DBBindColDouble (hstmt, 52, &my_struDBConfig_ERCompens.IB030_10_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 53, &my_struDBConfig_ERCompens.IB10_33_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 54, &my_struDBConfig_ERCompens.IB33_60_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 55, &my_struDBConfig_ERCompens.IB60_82_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 56, &my_struDBConfig_ERCompens.IB82_105_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	//10G MOD系数  
	resCode = DBBindColDouble (hstmt, 57, &my_struDBConfig_ERCompens.IM030_10_1G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 58, &my_struDBConfig_ERCompens.IM10_33_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 59, &my_struDBConfig_ERCompens.IM33_60_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 60, &my_struDBConfig_ERCompens.IM60_82_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 61, &my_struDBConfig_ERCompens.IM82_105_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	//10G VAPD DAC三温度的范围；
	resCode = DBBindColInt (hstmt, 62, &my_struDBConfig_ERCompens.APDDAC_MIN_L_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColInt (hstmt, 63, &my_struDBConfig_ERCompens.APDDAC_MAX_L_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColInt (hstmt, 64, &my_struDBConfig_ERCompens.APDDAC_MIN_R_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColInt (hstmt, 65, &my_struDBConfig_ERCompens.APDDAC_MAX_R_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColInt (hstmt, 66, &my_struDBConfig_ERCompens.APDDAC_MIN_H_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColInt (hstmt, 67, &my_struDBConfig_ERCompens.APDDAC_MAX_H_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	//10G Vapd温度补偿系数 
	resCode = DBBindColDouble (hstmt, 68, &my_struDBConfig_ERCompens.VAPD_KL_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 69, &my_struDBConfig_ERCompens.VAPD_KH_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 70, &my_struDBConfig_ERCompens.VBR_CLASS_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	//10G RSSI补偿系数   
	resCode = DBBindColDouble (hstmt, 71, &my_struDBConfig_ERCompens.RSSI_KL_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 72, &my_struDBConfig_ERCompens.RSSI_KH_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	//10G LOS  系数 
	resCode = DBBindColInt (hstmt, 73, &my_struDBConfig_ERCompens.ISLOSORSD_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 74, &my_struDBConfig_ERCompens.LOS_KL_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 75, &my_struDBConfig_ERCompens.LOS_BL_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 76, &my_struDBConfig_ERCompens.LOS_KH_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 77, &my_struDBConfig_ERCompens.LOS_BH_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 78, &my_struDBConfig_ERCompens.LOS_TEMPER_S_10G, &DBConfigStat); 
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
															  
	count=0;
    while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) {count++;} 
	
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) {ShowDataBaseError(); return FALSE;}
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	if (count!=1) 
	{
		MessagePopup("提示","数据库AutoDT_Spec_ERCompens_New中不能找到相应的数据或者数据记录多于一条！"); 
		return FALSE;
	}
	
	my_struDBConfig.isSDorLOS = my_struDBConfig_ERCompens.ISLOSORSD_10G;
	
	/*
	if (my_struDBConfig_ERCompens.RSSIMODE > 0)
	{
		//识别RSSI时序类型
		if      (strcmp (myRSSITime, "N1N3")==0) {my_struDBConfig_ERCompens.RSSI_TIME=RSSI_TIME_N1N3;}
		else if (strcmp (myRSSITime, "N2")==0)   {my_struDBConfig_ERCompens.RSSI_TIME=RSSI_TIME_N2;} 
		else if (strcmp (myRSSITime, "L2L3")==0) {my_struDBConfig_ERCompens.RSSI_TIME=RSSI_TIME_L2L3;} 
		else if (strcmp (myRSSITime, "L1L2")==0) {my_struDBConfig_ERCompens.RSSI_TIME=RSSI_TIME_L1L2;} 
		else {MessagePopup ("提示", "程序不支持AutoDT_Spec_Tracking_Ex表中RSSI_TIME的设置"); return -1;}	
	
		//根据时序内型检查数据库的配置是否不等于0，防止时序设置错误
		if ((my_struDBConfig_ERCompens.RSSI_N1==0 || my_struDBConfig_ERCompens.RSSI_N3==0) && my_struDBConfig_ERCompens.RSSI_TIME==RSSI_TIME_N1N3) 
		{MessagePopup ("提示", "请检查AutoDT_Spec_Tracking_Ex表中的RSSI_N1和RSSI_N3配置是否正确"); return -1;}
		
		if (my_struDBConfig_ERCompens.RSSI_N2==0 && my_struDBConfig_ERCompens.RSSI_TIME==RSSI_TIME_N2) 
		{MessagePopup ("提示", "请检查AutoDT_Spec_Tracking_Ex表中的RSSI_N2配置是否正确"); return -1;}
		
		if ((my_struDBConfig_ERCompens.RSSI_N2==0 || my_struDBConfig_ERCompens.RSSI_N3==0) && my_struDBConfig_ERCompens.RSSI_TIME==RSSI_TIME_L2L3) 
		{MessagePopup ("提示", "请检查AutoDT_Spec_Tracking_Ex表中的RSSI_N2和RSSI_N3配置是否正确"); return -1;}
		
		if ((my_struDBConfig_ERCompens.RSSI_N1==0 || my_struDBConfig_ERCompens.RSSI_N2==0) && my_struDBConfig_ERCompens.RSSI_TIME==RSSI_TIME_L1L2) 
		{MessagePopup ("提示", "请检查AutoDT_Spec_Tracking_Ex表中的RSSI_N1和RSSI_N2配置是否正确"); return -1;}
	}	 */
	
	//预先识别Driver芯片类型，比较字符串忽略大小写
/*	if (0 == stricmp (my_struDBConfig_ERCompens.ERCompensChip, "DS4830"))	
	{
		my_struConfig.ChiefChip=CHIEFCHIP_DS4830;
	}	   
	else if (0 == stricmp (my_struDBConfig_ERCompens.ERCompensChip, "DS4830A"))
	{
		my_struConfig.ChiefChip=CHIEFCHIP_DS4830;
	} 
	else 
	{
		MessagePopup ("提示", "数据库AutoDT_Spec_ERCompens表中定义的ERCompensChip不能识别"); 
		return FALSE;
	} 

	//驱动芯片
	if (0 == stricmp (my_struDBConfig_ERCompens.DriverChip, "MAX3738"))	
	{
		my_struConfig.DriverChip = DRIVERCHIP_MAX3738;
	} 
	else if (0 == stricmp (my_struDBConfig_ERCompens.DriverChip, "MAX3656"))
	{
		my_struConfig.DriverChip = DRIVERCHIP_MAX3656;
	} 
	else if (0 == stricmp (my_struDBConfig_ERCompens.DriverChip, "VSC7965"))
	{
		my_struConfig.DriverChip = DRIVERCHIP_VSC7965;
	} 
	else if (0 == stricmp (my_struDBConfig_ERCompens.DriverChip, "VSC7967"))
	{
		my_struConfig.DriverChip = DRIVERCHIP_VSC7967;
	} 
	else if (0 == stricmp (my_struDBConfig_ERCompens.DriverChip, "UX2222")) 
	{
		my_struConfig.DriverChip = DRIVERCHIP_UX2222;
	}
	else if (0 == stricmp (my_struDBConfig_ERCompens.DriverChip, "UX2228")) 
	{
		my_struConfig.DriverChip = DRIVERCHIP_UX2228;
	}  
	else if (0 == stricmp (my_struDBConfig_ERCompens.DriverChip, "GN2010")) 
	{
		my_struConfig.DriverChip = DRIVERCHIP_GN2010;
	}  
	else if (0 == stricmp (my_struDBConfig_ERCompens.DriverChip, "NT25L90")) 
	{
		my_struConfig.DriverChip = DRIVERCHIP_NT25L90;
	}
	else if (0 == stricmp (my_struDBConfig_ERCompens.DriverChip, "")) 
	{
		my_struConfig.DriverChip = -1;
	}    			
	else 
	{
		MessagePopup ("提示", "数据库AutoDT_Spec_ERCompens表中定义的DriverChip不能识别"); 
		return FALSE;
	} 
			
	//预先识别Amplifier芯片类型，比较字符串忽略大小写
	if (0 == stricmp (my_struDBConfig_ERCompens.AmplifierChip, "Default")) 
	{
		my_struConfig.AmplifierChip = AMPLIFIERCHIP_DEFAULT;
	}
	else if (0 == stricmp (my_struDBConfig_ERCompens.AmplifierChip, "SY88903AL"))
	{
		my_struConfig.AmplifierChip = AMPLIFIERCHIP_SY88903AL; 
	}
	else if (0 == stricmp (my_struDBConfig_ERCompens.AmplifierChip, "SY88933V")) 
	{
		my_struConfig.AmplifierChip = AMPLIFIERCHIP_SY88933V;
	}
	else if (0 == stricmp (my_struDBConfig_ERCompens.AmplifierChip, "SY88923"))
	{
		my_struConfig.AmplifierChip = AMPLIFIERCHIP_SY88923;  
	}
	else   
	{
		MessagePopup ("提示", "数据表AutoDT_Spec_ERCompens中AmplifierChip列定义的类型不能被识别"); 
		return FALSE;
	}
 */ 	
	return TRUE;
}

int DB_GET_Config_Monitor_Info (void) 
{
	int  num;
	char buf[1024];
	
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */ 
	
	Fmt (buf, "SELECT Temper_Prec,Vcc_Prec,I_Bias_Prec,Tx_Pow_Prec,Rx_Pow_Prec FROM AutoDT_Spec_Monitor WHERE PartNumber='%s' AND Version='%s' AND DT_Flag='%s' AND Temper_Flag='%s'", my_struConfig.PN, my_struConfig.Ver, my_struConfig.DT_Flag, my_struConfig.Temper_Flag);

	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return -1;}  
	
	resCode = DBBindColFloat (hstmt, 1, &my_struDBConfig_Monitor.Temper_Prec, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColFloat (hstmt, 2, &my_struDBConfig_Monitor.Vcc_Prec, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColFloat (hstmt, 3, &my_struDBConfig_Monitor.I_Bias_Prec, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColFloat (hstmt, 4, &my_struDBConfig_Monitor.Tx_Pow_Prec, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColFloat (hstmt, 5, &my_struDBConfig_Monitor.Rx_Pow_Prec, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	
    num=0;
    while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) {num++;} 
	
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) {ShowDataBaseError(); return -1;}
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	
	Fmt (buf, "数据库AutoDT_Spec_Monitor中不能找到PartNumber='%s' AND Version='%s' AND DT_Flag='%s' AND Temper_Flag='%s'的数据或者数据记录多于一条", my_struDBConfig.PartNumber, my_struDBConfig.Version, my_struDBConfig.DT_Flag, my_struDBConfig.Temper_Flag);
	if (num!=1) {MessagePopup("提示", buf); return -1;} 
	
	return 0;
}
#if 0
int DB_Save_Results_ATE_New(struct struTestData data, struct struProcessLOG prolog)
{
	char 	buf[5000];
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */
    strcpy(data.TestFlag,prolog.Log_Action);
	if (SERVER_ORACLE == my_struConfig.servertype)
	{
		sprintf (buf, "begin sp_add_results_ate_new ('%s','%s',to_date('%s','yyyy-mm-dd hh24:mi:ss'),'%s','%s',%f,%f,'%s',%d,'%s',%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%d,%d,'%s','%s','%s','%s','%s',%f,%f,%f,%f,%f,'%s','%s','%s','%s',to_date('%s','yyyy-mm-dd hh24:mi:ss'),'%s','%s','%s','%s','%s',%d );end;", 
				
			       data.PN, 
				   data.Ver, 
				   data.TestDate, 
				   data.ModuleSN, 
				   data.OpticsSN,
				   data.Temperatrue,
				   data.AmbientTemperatrue,
				   data.TestFlag,
				   data.ErrorCode,
				   data.Status, 
				   
				   data.AOP,
				   data.ER,
				   data.EA,
				   data.TxTj, 
				   data.TxPPj, 
				   data.TxRise,
				   data.TxFall,
				   data.MaskMargin,
				   data.TxCross,
				   data.TxOffPower,
				   
				   data.RxAmp, 
				   data.RxTj,
				   data.RxPPj,
				   data.RxTr,
				   data.RxTf,
				   data.Over, 
				   data.OverTime,
				   data.Sens,
				   data.SensTime,				   
				   data.SDA,
				   
				   data.SDD, 
				   data.SDHys,
				   data.RsdStep,
				   data.Pathpenalty, 
				   (float)data.PathpenaltyTime,  
				   data.PeakWL, 
				   data.Sigma,
				   data.Bandwidth,
				   data.SMSR,				   
				   data.TxI, 
				   
				   data.RxI, 
				   data.SE,
				   data.TE, 
				   data.DACApc, 
				   data.DACMod, 
				   data.DACCross,
				   data.DACApd,
				   data.DACLos, 
				   data.RSSI_Compensation, 
				   data.CoreTempera_Apc,
				   
				   data.CoreTempera_Mod,
				   data.CoreTempera_Cross,
				   data.CoreTempera_Apd,
				   data.CoreTempera_Los,
				   data.TxCalPower0,
				   data.TxCalPower1, 
				   
				   data.TxCalAdc0, 
				   data.TxCalAdc1,
				   
				   data.Info_APC,
				   data.Info_MOD,
				   data.Info_CROSS,
				   data.Info_APD,
				   data.Info_LOS,
				   
				   data.A2_Temper,
				   data.A2_Vcc,
				   data.A2_Ibias,
				   data.A2_TxMon, 
				   data.CaseTemper,  
				   
				   
				   prolog.PN, prolog.SN, prolog.Operator, prolog.Log_Action, prolog.Action_Time, 
				   prolog.Parameter, prolog.P_Value,prolog.Comments, prolog.SoftVersion, prolog.StationID, prolog.RunTime); 
			
	}
	else
	{
		MessagePopup("提示", " 不支持该数据库类型");
	}			   
	
	resCode = DBImmediateSQL (hdbc, buf); 
	if (resCode != DB_SUCCESS) 
	{
		ShowDataBaseError(); 
		return -1;
	}
	
	return 0;   
}
#endif


int DB_Save_Results_ATE_New(struct struTestData data, struct struProcessLOG prolog)
{
	char 	buf[10000];
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */
    strcpy(data.TestFlag,prolog.Log_Action);
	if (SERVER_ORACLE == my_struConfig.servertype)
	{
		sprintf (buf, "begin sp_add_results_ate_new1 ('%s','%s',to_date('%s','yyyy-mm-dd hh24:mi:ss'),'%s','%s',%f,%f,'%s',%d,'%s',%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%d,%d,'%s','%s','%s','%s','%s','%s',%f,%f,%f,%f,%f,%f,%f,%f,'%s','%s','%s','%s',to_date('%s','yyyy-mm-dd hh24:mi:ss'),'%s','%s','%s','%s','%s',%d );end;", 
				
			       data.PN, 
				   data.Ver, 
				   data.TestDate, 
				   data.ModuleSN, 
				   data.OpticsSN,
				   data.Temperatrue,
				   data.AmbientTemperatrue,
				   data.TestFlag,
				   data.ErrorCode,
				   data.Status, 
				   
				   data.AOP,
				   data.ER,
				   data.EA,
				   data.TxTj, 
				   data.TxPPj, 
				   data.TxRise,
				   data.TxFall,
				   data.MaskMargin,
				   data.TxPCTCROss,
				   data.TxOffPower,
				   
				   data.RxAmp, 
				   data.RxTj,
				   data.RxPPj,
				   data.RxTr,
				   data.RxTf,
				   data.Over, 
				   data.OverTime,
				   data.Sens,
				   data.SensTime,				   
				   data.SDA,
				   
				   data.SDD, 
				   data.SDHys,
				   data.RsdStep,
				   data.Pathpenalty, 
				   (float)data.PathpenaltyTime,  
				   data.PeakWL, 
				   data.Sigma,
				   data.Bandwidth,
				   data.SMSR,				   
				   data.TxI, 
				   
				   data.RxI, 
				   data.SE,
				   data.TE, 
				   data.DACApc, 
				   data.DACMod, 
				   data.DACCross,
				   data.DACApd,
				   data.DACLos, 
				   data.DACEa,  				   
				   data.RSSI_Compensation, 
				   data.CoreTempera_Apc,
				   
				   data.CoreTempera_Mod,
				   data.CoreTempera_Cross,
				   data.CoreTempera_Apd,
				   data.CoreTempera_Los,
				   data.TxCalPower0,
				   data.TxCalPower1, 
				   
				   data.TxCalAdc0, 
				   data.TxCalAdc1,
				   
				   data.Info_APC,
				   data.Info_MOD,
				   data.Info_CROSS,
				   data.Info_APD,
				   data.Info_LOS,
				   data.Info_EA,
				   
				   data.CoreTemper,
				   data.A2_Vcc,
				   data.A2_Ibias,
				   data.A2_TxMon, 
				   data.A2_Temper, 
				   
				   data.TecCurrent,
				   data.TecTemper,
				   data.EaAbsorb,

				   
				   
				   prolog.PN, prolog.SN, prolog.Operator, prolog.Log_Action, prolog.Action_Time, 
				   prolog.Parameter, prolog.P_Value,prolog.Comments, prolog.SoftVersion, prolog.StationID, prolog.RunTime); 
			
	}
	else
	{
		MessagePopup("提示", " 不支持该数据库类型");
	}			   
	
	resCode = DBImmediateSQL (hdbc, buf); 
	if (resCode != DB_SUCCESS) 
	{
		ShowDataBaseError(); 
		return -1;
	}
	
	return 0;   
}


BOOL DB_GET_ProType(void)
{
	char buf[1024]; 
	int	num;
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */
					 
	sprintf (buf, "SELECT Pro_Type FROM AutoDT_Spec_10G_ATE WHERE  PartNumber='%s' and Version='%s' and DT_Flag='TUN_10G' and Temper_Flag='ROOM' ", my_struConfig.PN, my_struConfig.Ver);  
	
	hstmt = DBActivateSQL (hdbc, buf);																												  
    if (hstmt <= 0) 
	{
		ShowDataBaseError(); 
		return FALSE;
	}  	

	resCode = DBBindColChar (hstmt, 1, 50, my_struDBConfig.Pro_Type, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) 
	{
		ShowDataBaseError(); 
		return FALSE;
	}
	
    num=0;
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)   
	{
		num++;
	}      
    
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) 
	{
		ShowDataBaseError();  
		return FALSE;
	}
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	if (1 != num)	
	{
		MessagePopup("Error", "数据表AutoDT_Spec_10G_ATE没有找到该产品类型的测试参数！   ");
		return FALSE;
	}	 

	return TRUE;    
}

BOOL checksn(char *s)
{
char tempstr[30];
int	i, j;

	memset (tempstr, 0, 30);   //
	j=0;
	for (i=0; i<StringLength (s); i++)
 	{
 		if (isalnum (s[i])) {tempstr[j]=s[i];j++;}  //to see if a character is alphanumeric (a-z, A-Z, 0-9). 
 	} 
 	memset (s, 0, StringLength (s)); 
 	strcpy (s, tempstr);
 	
	return TRUE;
}

BOOL DB_Save_Process_LOG(void)
{
	char 	buf[1024];
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */
	
/*	Fmt (buf, "EXEC sp_add_autodt_process_log_ate '%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%d", 
		       my_struProcessLOG.PN, my_struProcessLOG.SN, my_struProcessLOG.Operator, my_struProcessLOG.Log_Action, my_struProcessLOG.Action_Time, 
			   my_struProcessLOG.Parameter, my_struProcessLOG.P_Value,my_struProcessLOG.Comments, my_struProcessLOG.SoftVersion, my_struProcessLOG.StationID, my_struProcessLOG.RunTime); 
	  */
	if (SERVER_ORACLE == my_struConfig.servertype)
	{
		Fmt (buf, "begin sp_add_autodt_log ('%s','%s','%s','%s',to_date('%s','yyyy-mm-dd hh24:mi:ss'),'%s','%s','%s','%s','%s',%d, '%s', '%s', '%s', '%s', %d );end;", 
			       my_struProcessLOG.PN, my_struProcessLOG.SN, my_struProcessLOG.Operator, my_struProcessLOG.Log_Action, my_struProcessLOG.Action_Time, 
				   my_struProcessLOG.Parameter, my_struProcessLOG.P_Value,my_struProcessLOG.Comments, my_struProcessLOG.SoftVersion, my_struProcessLOG.StationID, my_struProcessLOG.RunTime,
				   "AutoDT_Results_10G", "ID", "PartNumber", "OpticsSN", 1); 
		
	}
	else
	{
		Fmt (buf, "EXEC sp_add_autodt_process_log '%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%d, '%s', '%s', '%s', '%s', %d",
		       my_struProcessLOG.PN, my_struProcessLOG.SN, my_struProcessLOG.Operator, my_struProcessLOG.Log_Action, my_struProcessLOG.Action_Time, 
			   my_struProcessLOG.Parameter, my_struProcessLOG.P_Value,my_struProcessLOG.Comments, my_struProcessLOG.SoftVersion, my_struProcessLOG.StationID, my_struProcessLOG.RunTime,
			   "AutoDT_Results_10G", "ID", "PartNumber", "OpticsSN", 1); 
	}
	resCode = DBImmediateSQL (hdbc, buf); 
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	return TRUE;
}

int DB_Search_Process_LOG_Ex(struct struTestData data, struct struProcessLOG prolog)
{
	int  retParam, errcode;
	char *errmsg;
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */
	
	/* Prepare a statement which calls the stored procedure */
    resCode = DBSetAttributeDefault(hdbc, ATTR_DB_COMMAND_TYPE, DB_COMMAND_STORED_PROC);
	 if (resCode < 0) {ShowDataBaseError();  return -1;} 
	 
    hstmt = DBPrepareSQL (hdbc, "sp_workstation_check");
    if (hstmt <= 0) {ShowDataBaseError();  return -1;} 
	 
	resCode = DBSetAttributeDefault(hdbc, ATTR_DB_COMMAND_TYPE, DB_COMMAND_UNKNOWN);
	if (resCode < 0) {ShowDataBaseError();  return -1;} 
	 
	if (SERVER_ORACLE == my_struConfig.servertype)
	{
    /* Set the input parameter */
		resCode = DBCreateParamInt (hstmt, "V_return", DB_PARAM_OUTPUT, -1);
		if (resCode < 0) {ShowDataBaseError();  return -1;}
	    resCode = DBCreateParamChar (hstmt, "V_pn", DB_PARAM_INPUT, my_struConfig.PN, strlen(my_struConfig.PN)+1);   //必须要获取实际的字符串大小，否则定义过长的字符空间，不足的程序会在字符串后以空格补齐
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "V_bom", DB_PARAM_INPUT, my_struConfig.Ver, strlen(my_struConfig.Ver)+1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
		resCode = DBCreateParamChar (hstmt, "V_station", DB_PARAM_INPUT, prolog.Log_Action, strlen(prolog.Log_Action)+1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "V_sn", DB_PARAM_INPUT, data.OpticsSN, strlen(data.OpticsSN)+1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "V_errmsg ", DB_PARAM_OUTPUT, "", 500); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamInt (hstmt, "V_errcode", DB_PARAM_OUTPUT, -1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;}   
	}
	else
	{
	    /* Set the input parameter */
		resCode = DBCreateParamInt (hstmt, "", DB_PARAM_RETURN_VALUE, -1);
		if (resCode < 0) {ShowDataBaseError();  return -1;}
	    resCode = DBCreateParamChar (hstmt, "@pn", DB_PARAM_INPUT, my_struConfig.PN, strlen(my_struConfig.PN)+1);   //必须要获取实际的字符串大小，否则定义过长的字符空间，不足的程序会在字符串后以空格补齐
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "@bom", DB_PARAM_INPUT, my_struConfig.Ver, strlen(my_struConfig.Ver)+1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
		resCode = DBCreateParamChar (hstmt, "@station", DB_PARAM_INPUT, prolog.Log_Action, strlen(prolog.Log_Action)+1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "@sn", DB_PARAM_INPUT, data.OpticsSN, strlen(data.OpticsSN)+1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "@errmsg ", DB_PARAM_OUTPUT, "", 500); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamInt (hstmt, "@errcode", DB_PARAM_OUTPUT, -1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	}
	
    /* Execute the statement */
    resCode = DBExecutePreparedSQL(hstmt);
	if (resCode < 0) {ShowDataBaseError();  return -1;}  
	
    /* Close the statement.  Output parameters are invalid until the statement is closed */
    resCode = DBClosePreparedSQL(hstmt);
	if (resCode < 0) {ShowDataBaseError();  return -1;} 
	
    /* Examine the parameter values */
	resCode = DBGetParamInt (hstmt, 1, &retParam); 
	if (resCode < 0) {ShowDataBaseError();  return -1;}
    resCode = DBGetParamChar(hstmt, 6, &errmsg, "");		//此处需要对返回的字符串空格做处理
	if (resCode < 0) {ShowDataBaseError();  return -1;} 
    resCode = DBGetParamInt(hstmt, 7, &errcode);
	if (resCode < 0) {ShowDataBaseError();  return -1;} 

    /* Discard the statement */
    resCode = DBDiscardSQLStatement(hstmt);
	if (resCode < 0) {ShowDataBaseError();  return -1;} 
	
	if (errcode)
	{
		//删除错误信息中的空格
		MyDLL_TRIM (errmsg);
		MessagePopup("提示", errmsg); 
		return -1;
	}
	
	return 0;
}

int DB_Search_Tracking_BareBOM(char *SN, char *bom, char batch[])
{
	int  errcode;
	char *errmsg, *mybom, *pn, *mybatch;
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */
	
	/* Prepare a statement which calls the stored procedure */
    resCode = DBSetAttributeDefault(hdbc, ATTR_DB_COMMAND_TYPE, DB_COMMAND_STORED_PROC);
	 if (resCode < 0) {ShowDataBaseError();  return -1;} 
	 
    hstmt = DBPrepareSQL (hdbc, "sp_get_tracking_bom");
    if (hstmt <= 0) {ShowDataBaseError();  return -1;} 
	 
	resCode = DBSetAttributeDefault(hdbc, ATTR_DB_COMMAND_TYPE, DB_COMMAND_UNKNOWN);
	if (resCode < 0) {ShowDataBaseError();  return -1;} 
	 
    /* Set the input parameter */
	if (SERVER_ORACLE == my_struConfig.servertype)
	{ 
	    resCode = DBCreateParamInt (hstmt, "V_return", DB_PARAM_OUTPUT, -1); 
	    if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "V_sn", DB_PARAM_INPUT, SN, strlen(SN)+1);   //必须要获取实际的字符串大小，否则定义过长的字符空间，不足的程序会在字符串后以空格补齐
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "V_bom", DB_PARAM_OUTPUT, "", 50); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "V_errmsg ", DB_PARAM_OUTPUT, "", 500); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamInt (hstmt, "V_errcode", DB_PARAM_OUTPUT, -1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "V_pn", DB_PARAM_OUTPUT, "", 50); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "V_batch", DB_PARAM_OUTPUT, "", 50); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	}
	else
	{
		resCode = DBCreateParamInt (hstmt, "", DB_PARAM_RETURN_VALUE, -1); 
	    if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "@sn", DB_PARAM_INPUT, SN, strlen(SN)+1);   //必须要获取实际的字符串大小，否则定义过长的字符空间，不足的程序会在字符串后以空格补齐
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "@bom", DB_PARAM_OUTPUT, "", 50); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "@errmsg ", DB_PARAM_OUTPUT, "", 500); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamInt (hstmt, "@errcode", DB_PARAM_OUTPUT, -1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "@pn", DB_PARAM_OUTPUT, "", 50); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "@batch", DB_PARAM_OUTPUT, "", 50); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	}	
    /* Execute the statement */
    resCode = DBExecutePreparedSQL(hstmt);
	if (resCode < 0) {ShowDataBaseError();  return -1;}  
	
    /* Close the statement.  Output parameters are invalid until the statement is closed */
    resCode = DBClosePreparedSQL(hstmt);
	if (resCode < 0) {ShowDataBaseError();  return -1;} 
	
    /* Examine the parameter values */
	resCode = DBGetParamChar(hstmt, 3, &mybom, "");			
	if (resCode < 0) {ShowDataBaseError();  return -1;} 
	resCode = DBGetParamChar(hstmt, 4, &errmsg, "");		//此处需要对返回的字符串空格做处理
	if (resCode < 0) {ShowDataBaseError();  return -1;} 
	resCode = DBGetParamInt(hstmt, 5, &errcode);
	if (resCode < 0) {ShowDataBaseError();  return -1;} 
	resCode = DBGetParamChar(hstmt, 6, &pn, "");			
	if (resCode < 0) {ShowDataBaseError();  return -1;} 
	resCode = DBGetParamChar(hstmt, 7, &mybatch, "");			
	if (resCode < 0) {ShowDataBaseError();  return -1;} 

    /* Discard the statement */
    resCode = DBDiscardSQLStatement(hstmt);
	if (resCode < 0) {ShowDataBaseError();  return -1;} 
	
	if (errcode)
	{
		//删除错误信息中的空格
		MyDLL_TRIM (errmsg);
		MessagePopup("提示", errmsg); 
		return -1;
	}
	else
	{
		MyDLL_TRIM (mybom); 
		strcpy (bom, mybom);
		
		MyDLL_TRIM (pn); 
		MyDLL_TRIM (mybatch); 
		strcpy (batch, mybatch); 
	}
	
	return 0;
}

int DB_READ_Spec_Monitor_Ex(int Flag)  
{
	char buf[1024]; 
	int  num, i;
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */

	//查询校准点设置
	Fmt (buf, "SELECT CalibrateNumber,CaliPoint1,CaliPoint2,CaliPoint3,CaliPoint4,CaliPoint5,CaliPoint6,CaliPoint7,CaliPoint8,CaliPoint9,CaliPoint10,CaliPoint11,CaliPoint12,CaliPoint13,CaliPoint14,CaliPoint15,CaliPoint16,CaliPoint17,CaliPoint18,CaliPoint19,CaliPoint20,CaliPoint21,CaliPoint22,CaliPoint23,CaliPoint24,CaliPoint25,CaliPoint26,CaliPoint27,CaliPoint28,CaliPoint29,CaliPoint30 FROM AutoDT_Spec_Monitor_Ex WHERE PartNumber='%s' AND BarBOM='%s' AND CalibrateFlag=0", my_struConfig.PN, my_struConfig.Ver);

	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return -1;}  
	
	resCode = DBBindColInt (hstmt, 1, &my_struDBConfig_Monitor.CaliNumber, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	for (i=0; i<30; i++)
	{
		resCode = DBBindColDouble (hstmt, i+2, &my_struDBConfig_Monitor.CaliPoint[i], &DBConfigStat);
	    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	}
	
	num=0; 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) {num++;}
	
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
    
    if (num!=1)
	{MessagePopup ("ERROR", "读取数据库AutoDT_Spec_Monitor_Ex中的记录失败"); return -1;}  
	
	//查询校准测试点设置
	Fmt (buf, "SELECT CalibrateNumber,CaliPoint1,CaliPoint2,CaliPoint3,CaliPoint4,CaliPoint5,CaliPoint6,CaliPoint7,CaliPoint8,CaliPoint9,CaliPoint10,CaliPoint11,CaliPoint12,CaliPoint13,CaliPoint14,CaliPoint15,CaliPoint16,CaliPoint17,CaliPoint18,CaliPoint19,CaliPoint20,CaliPoint21,CaliPoint22,CaliPoint23,CaliPoint24,CaliPoint25,CaliPoint26,CaliPoint27,CaliPoint28,CaliPoint29,CaliPoint30 FROM AutoDT_Spec_Monitor_Ex WHERE PartNumber='%s' AND BarBOM='%s' AND CalibrateFlag=%d", my_struConfig.PN, my_struConfig.Ver, Flag);

	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return -1;}  
	
	resCode = DBBindColInt (hstmt, 1, &my_struDBConfig_Monitor.CaliNumber_Test, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	for (i=0; i<30; i++)
	{
		resCode = DBBindColDouble (hstmt, i+2, &my_struDBConfig_Monitor.CaliPoint_Test[i], &DBConfigStat);
	    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	}
	
	num=0; 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) {num++;}
	
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
    
    if (num!=1)
	{MessagePopup ("ERROR", "读取数据库AutoDT_Spec_Monitor_Ex中的记录失败"); return -1;}  
	
	return 0;
}

int DB_Close(void)
{
	int resCode = 0;   /* Result code                      */ 
//	int hstmt = 0;	   /* Handle to SQL statement          */
	
	if (hdbc>0)
	{
		resCode = DBDisconnect (hdbc);
		if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
		
		hdbc=0;
	}
	return 0;
}

void GetPartNo (int panel, int control)
{
char	FileName_PN[MAX_PATHNAME_LEN];    
char 	*cBuffer;
int 	iCount;
int 	iPNIndex;
char 	*PN;
int 	FileHandle_PN;  
int 	status;

	//***************获取PN*****************/
	GetProjectDir (FileName_PN);
	strcat(FileName_PN,"\\data\\PartNumber.txt");
	
	FileHandle_PN = OpenFile (FileName_PN, VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);
	
	status = GetFileSize (FileName_PN, &iCount);
	if(status != 0) {MessagePopup("提示", "打开PN存储文件出错！");   } 
	
	cBuffer = malloc (iCount+1);
	memset (cBuffer, 0, iCount+1);
	status = ReadFile (FileHandle_PN, cBuffer, iCount);	   //从文件读出的cBuffer后必须添加一个字符'00'
	if(!status) {MessagePopup("提示", "读取PN存储文件出错！");   }  
	
	PN=strtok(cBuffer,";");
	if (PN) 
	{   	
		ClearListCtrl (panel, control); 
		iPNIndex=0;
		InsertListItem (panel, control, iPNIndex, PN, iPNIndex);  
	}
	while (PN)
	{
		PN=strtok(NULL,";"); 
		if (PN)
		{
			iPNIndex++;
			InsertListItem (panel, control, iPNIndex, PN, iPNIndex);
		}
	}
	if(cBuffer != NULL)
		free(cBuffer);
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
void GetPNVersion(int panel, int control_Ver)
{
int 	VerIndex; 
char    Ver[100][10]={""};

	DB_GET_Spec_BarCode_Ver(Ver);
	
	ClearListCtrl (panel, control_Ver); 
	VerIndex=0;
	while (strcmp(Ver[VerIndex],"")!=0)
	{
		InsertListItem (panel, control_Ver, VerIndex, Ver[VerIndex], VerIndex); 
		VerIndex++;
	}
}

void Get_EED_Ver(int panel, int control)
{
int 	VerIndex; 
char    Ver[100][10]={""};

	DB_GET_EED_Ver(Ver);
	
	ClearListCtrl (panel, control); 
	VerIndex=0;
	while (strcmp(Ver[VerIndex],"")!=0)
	{
		InsertListItem (panel, control, VerIndex, Ver[VerIndex], VerIndex); 
		VerIndex++;
	}
}

BOOL fit_LUT_Line(BYTE fitNumber, int *Temper_in_arr, BYTE *LUT_in_arr, int TemperLimit_H, int TemperLimit_L, int *Temper_out_arr, BYTE *LUT_out_arr)
{
int 	i, j;
double 	slope, offset, *slope_arr, *offset_arr, temper; 

	slope_arr  = malloc ((fitNumber-1)*8); //开辟内存空间
	offset_arr = malloc ((fitNumber-1)*8);
  
	for (i=0; i<fitNumber-1; i++)  //计算slope和offset
	{
		slope_arr[i]  =(double)(LUT_in_arr[i+1]-LUT_in_arr[i])/(double)(Temper_in_arr[i+1]-Temper_in_arr[i]);
		offset_arr[i] =(double)(LUT_in_arr[i+1]-slope_arr[i]*Temper_in_arr[i+1]);
	}
	//计算查找表
	for (i=0; i<72; i++)
	{
		temper = i*2-40;
		
		for (j=0; j<fitNumber-1; j++) //得到对应温度的slope和offset
		{
			if (temper>Temper_in_arr[fitNumber-1])
			{
				slope  = slope_arr[fitNumber-2];
				offset = offset_arr[fitNumber-2];
				break;
			}
			if (temper<=Temper_in_arr[j+1])
			{
				slope  = slope_arr[j];
				offset = offset_arr[j];
				break;
			}
		}//for (j=0; j<arraylenth-1; j++)
		Temper_out_arr[i] = temper;
		if (temper>TemperLimit_H) temper=TemperLimit_H;
		if (temper<TemperLimit_L) temper=TemperLimit_L;
		
		//对LUT计算值超出0和255范围的值处理
		if ((temper*slope + offset)<0)        LUT_out_arr[i] = 0;
		else if ((temper*slope + offset)>255) LUT_out_arr[i] = 255;
		else                                  LUT_out_arr[i] = temper*slope + offset;
	}
		
	if (slope_arr != NULL) free(slope_arr);
	if (offset_arr != NULL) free(offset_arr); 

	return TRUE;
}

BOOL fit_LUT_Line_SFP(BYTE fitNumber, int *Temper_in_arr, INT16U *LUT_in_arr, int TemperLimit_H, int TemperLimit_L, int *Temper_out_arr, INT16U *LUT_out_arr)
{
int 	i, j;
double 	slope, offset, *slope_arr, *offset_arr, temper; 

	slope_arr  = malloc ((fitNumber-1)*8); //开辟内存空间
	offset_arr = malloc ((fitNumber-1)*8);
  
	for (i=0; i<fitNumber-1; i++)  //计算slope和offset
	{
		slope_arr[i]  =(double)(LUT_in_arr[i+1]-LUT_in_arr[i])/(double)(Temper_in_arr[i+1]-Temper_in_arr[i]);
		offset_arr[i] =(double)(LUT_in_arr[i+1]-slope_arr[i]*Temper_in_arr[i+1]);
	}
	//计算查找表
	for (i=0; i<166; i++)
	{
		temper = i-40;
		for (j=0; j<fitNumber-1; j++) //得到对应温度的slope和offset
		{
			if (temper>Temper_in_arr[fitNumber-1])
			{
				slope  = slope_arr[fitNumber-2];
				offset = offset_arr[fitNumber-2];
				break;
			}
			if (temper<=Temper_in_arr[j+1])
			{
				slope  = slope_arr[j];
				offset = offset_arr[j];
				break;
			}
		}//for (j=0; j<arraylenth-1; j++)
		Temper_out_arr[i] = temper;
		if (temper>TemperLimit_H) temper=TemperLimit_H;
		if (temper<TemperLimit_L) temper=TemperLimit_L;
		
		//对LUT计算值超出0和4095范围的值处理
		if ((temper*slope + offset)<0)        LUT_out_arr[i] = 0;
		else if ((temper*slope + offset)>0x800) LUT_out_arr[i] = 0x800;
		else                                  LUT_out_arr[i] = temper*slope + offset;
	}
	
	//释放内存
	if (slope_arr != NULL) free(slope_arr);
	if (offset_arr != NULL) free(offset_arr); 	
	
	return TRUE;
}
	
int Set_Att(int channel, int AttType, double AttValue)
{  
	int obtainedLock=0;
	
	switch (AttType)
		{
		case ATT_TYPE_HP8156A:
			//添加功率值判断
			AttValue = 0-AttValue;
			if (AttValue<0) 
			{
				MessagePopup ("ERROR", "HP8156A Set Att error!"); 
				return -1;
			}
			if (!HP8156_SET_ATT(INST_ATT_MAIN[channel], AttValue))
			{
				return -1;    
			}
			Delay(0.3); 
			break;
		case ATT_TYPE_OLA55M:
			AttValue = 0-AttValue;
			if (AttValue<0) 
			{
				MessagePopup ("ERROR", "OLA55M Set Att error!"); 
				return -1;
			}
			if (!OLA55M_Set_ATTValue(INST_ATT_MAIN[channel], AttValue)) 
			{
				MessagePopup ("ERROR", "OLA55M Set Att error!"); 
				return -1;
			}   
			break;
		case ATT_TYPE_NONE:
			//没有衰减器
			break;
		case ATT_TYPE_GVPM:
			AttValue = 0-AttValue; 
			if (AttValue<0)  
			{
				MessagePopup ("ERROR", "GVPM Set Value error!"); 
				return -1;
			} 
			if (AttValue>50) 
			{
				if(GVPM_SET_OutputEnable(INST_ATT[channel], 0)<0) 
				{
					return -1;
				}
			}//考虑GVPM设置光功率不能完全关断（由于输入光较强的原因）的影响     
			else  		
			{
				if(GVPM_SET_Attenuate (INST_ATT[channel], AttValue, 1550)<0) 
				{
					return -1;
				}
			}  
			break; 	
		case ATT_TYPE_JW8504:
			GetLock(ThreadLock_JW8504, &obtainedLock);
			if(JW8504_Set (INST_ATT[channel], channel, 1310, 0-AttValue)<0)
			{
				return -1;
			}
	  
			CmtReleaseLock(ThreadLock_JW8504);
			Delay(0.5);
			break;
		default:
			{
				MessagePopup ("提示", "未定义的衰减器类型"); 
				return -1;
			} 
			break;			
		}
	return 0;
}

int Set_Att_Lock (int channel, int AttType, double AttValue)
{   
	int obtainedLock = 0;   
	int error;

	switch (AttType)
		{
		case ATT_TYPE_GVPM:
			AttValue = 0-AttValue; 
			if (AttValue<0)  {MessagePopup ("ERROR", "GVPM Set Value error!"); return -1;} 
			if (AttValue>50) {if(GVPM_SET_OutputEnable(INST_ATT[channel], 0)<0) return -1;}			//考虑GVPM设置光功率不能完全关断（由于输入光较强的原因）的影响     
			else  			 
			{	 
				if(GVPM_SET_LockEnable (INST_ATT[channel], 1)<0) 
				{
					return -1;
				}
					
				if(GVPM_SET_LockAttenuate (INST_ATT[channel], 0-AttValue, 1550, 1)<0) 
				{
					return -1;
				}
			}  	
			break; 	
		case ATT_TYPE_JW8504:
			GetLock(ThreadLock_JW8504, &obtainedLock); 
			AttValue = 0-AttValue; 
			if (AttValue<0)  
			{
				MessagePopup ("ERROR", "JW8504 Set Value error!"); 
				return -1;
			} 
			if(JW8504_set_lock (INST_ATT[channel],channel, 0-AttValue))  
			{
				return -1;  
			}
			Delay(0.1);
			CmtReleaseLock(ThreadLock_JW8504);   
			break; 
		default:
			return -1;
			break;			
		}
	
	return 0;
}

int Set_Att_Lock_Enable (int channel)
{   
	int obtainedLock = 0;   			  
	int error;

	switch (my_struInstrument[channel].ATT_TYPE_MAIN)
		{
		case ATT_TYPE_GVPM:
			
			error = GVPM_SET_LockEnable (INST_ATT[channel], 1);	
			if (error) 
			{
				MessagePopup ("提示", "打开GVPM锁定模式异常");
				return -1;  
			}  
		  	
			break; 	
			
		case ATT_TYPE_JW8504:
		
			GetLock(ThreadLock_JW8504, &obtainedLock); 
			error = JW8504_enable (INST_ATT[channel],channel, 1); 
			if (error) 
			{
				MessagePopup ("提示", "打开佳慧衰减器锁定模式异常");
				return -1;  
			}  
			CmtReleaseLock(ThreadLock_JW8504);   
			break; 
		default:
			return -1;
			break;			
		}
	
	return 0;
}

int Set_Att_Lock_Disable (int channel)
{   
	int obtainedLock = 0;   			  
	int error;

	switch (my_struInstrument[channel].ATT_TYPE_MAIN)
		{
		case ATT_TYPE_GVPM:
			
			error = GVPM_SET_LockEnable (INST_ATT[channel], 0);	
			if (error) 
			{
				MessagePopup ("提示", "打开GVPM锁定模式异常");
				return -1;  
			}  
		  	
			break; 	
			
		case ATT_TYPE_JW8504:
		
			GetLock(ThreadLock_JW8504, &obtainedLock); 
			error = JW8504_enable (INST_ATT[channel],channel, 0); 
			if (error) 
			{
				MessagePopup ("提示", "打开佳慧衰减器锁定模式异常");
				return -1;  
			}  
			CmtReleaseLock(ThreadLock_JW8504);   
			break; 
		default:
			return -1;
			break;			
		}
	
	return 0;
}

BOOL Set_Power_Wavelength(int channel, int Wavelength)
{    int obtainedLock = 0;   
	switch (my_struInstrument[channel].Power_TYPE)  
		{
		case PowMeter_TYPE_PMSII:
			if (Wavelength==1490) Wavelength=1480;
			if (PMSII_Set_Wavlength(INST_PM[channel], Wavelength)<0) return FALSE; 
			break; 
			
		case PowMeter_TYPE_NONE:
			;
			break; 
					
		case PowerMeter_TYPE_PSS_OPM:
			  GetLock(Thread_PSS_PowerPeter,&obtainedLock);   
				if (Wavelength==1480) Wavelength=1490;
			if (PSS_Set_WaveLength(INST_PM[channel],channel+1, Wavelength)<0) return FALSE; 
			CmtReleaseLock(Thread_PSS_PowerPeter);       
			break; 
		
		default:
			return FALSE;
			break;
		}
	return TRUE;
}

BOOL GET_Power(int channel, double *Power)
{    
	int error ,obtainedLock = 0;  
	switch (my_struInstrument[channel].Power_TYPE)
	{
		case PowMeter_TYPE_PMSII:
			
			if (PMSII_Get_Power(INST_PM[channel], Power)<0) 
			{
				return FALSE; 
			}
			
			break;
			
  	    case PowerMeter_TYPE_PSS_OPM:
			
		   GetLock(Thread_PSS_PowerPeter,&obtainedLock);
		   
		   error=PSS_Get_Power(INST_PM[channel],channel+1,Power);
		   
		   CmtReleaseLock(Thread_PSS_PowerPeter);
		   
		   if (error)
		   {
			   return FALSE;
		   }
		   
		   break;
		   
		default:
			
			return FALSE;
		
			break;
		}
	return TRUE;
}

BOOL GET_Er(int DCAType, double *Er)
{
	switch (DCAType)
		{
		case DCA_TYPE_CSA8000:
			if(!CSA8000_Set_Clear(inst_CSA8000)) return FALSE; 
			Delay(2);
			if (!CSA8000_GET_Er(inst_CSA8000, Er)) return FALSE;
			break;
		case DCA_TYPE_Ag86100:
			if(!Ag86100_Set_Clear(inst_Ag86100)) return FALSE; 
			Delay(2);
			if (!Ag86100_GET_Er(inst_Ag86100, Er)) return FALSE;
			break;
		default:
			return FALSE;
			break;
		}
	return TRUE;
}

BOOL GET_Cross(int DCAType, double *Cross)
{
	switch (DCAType)
		{
		case DCA_TYPE_CSA8000:
			if(!CSA8000_Set_Clear(inst_CSA8000)) return FALSE; 
			Delay(2);
			if (!CSA8000_GET_O_PCTCROss(inst_CSA8000, Cross)) return FALSE;
			break;
		case DCA_TYPE_Ag86100:
			if(!Ag86100_Set_Clear(inst_Ag86100)) return FALSE; 
			Delay(2);
			if (!Ag86100_GET_O_CROSsing(inst_Ag86100, Cross)) return FALSE;
			break;
		default:
			return FALSE;
			break;
		}
	return TRUE;
}

BOOL READ_DCA_PJ(int DCAType, double *PJ)
{
	switch (DCAType)
		{
		case DCA_TYPE_CSA8000:
			if (!CSA8000_GET_O_PPj(inst_CSA8000, PJ)) return FALSE;
			break;
		case DCA_TYPE_Ag86100:
			if (!Ag86100_GET_O_PPj(inst_Ag86100, PJ)) return FALSE;
			break;
		default:
			return FALSE;
			break;
		}
	return TRUE;
}

BOOL READ_DCA_RJ(int DCAType, double *RJ)
{
	switch (DCAType)
		{
		case DCA_TYPE_CSA8000:
			if (!CSA8000_GET_O_RSj(inst_CSA8000, RJ)) return FALSE;
			break;
		case DCA_TYPE_Ag86100:
			if (!Ag86100_GET_O_RSj(inst_Ag86100, RJ)) return FALSE;
			break;
		default:
			return FALSE;
			break;
		}
	return TRUE;
}

BOOL READ_DCA_Rise(int DCAType, double *Rise)
{
	switch (DCAType)
		{
		case DCA_TYPE_CSA8000:
			if (!CSA8000_GET_O_Rise(inst_CSA8000, Rise)) return FALSE;
			break;
		case DCA_TYPE_Ag86100:
			if (!Ag86100_GET_O_Rise(inst_Ag86100, Rise)) return FALSE;
			break;
		default:
			return FALSE;
			break;
		}
	return TRUE;
}

BOOL READ_DCA_Fall(int DCAType, double *Fall)
{
	switch (DCAType)
		{
		case DCA_TYPE_CSA8000:
			if (!CSA8000_GET_O_Fall(inst_CSA8000, Fall)) return FALSE;
			break;
		case DCA_TYPE_Ag86100:
			if (!Ag86100_GET_O_Fall(inst_Ag86100, Fall)) return FALSE;
			break;
		default:
			return FALSE;
			break;
		}
	return TRUE;
}

BOOL READ_DCA_PCTCROss(int DCAType, double *PCTCROss)
{
	switch (DCAType)
		{
		case DCA_TYPE_CSA8000:
			if (!CSA8000_GET_O_PCTCROss(inst_CSA8000, PCTCROss)) return FALSE;
			break;
		case DCA_TYPE_Ag86100:
			if (!Ag86100_GET_O_CROSsing(inst_Ag86100, PCTCROss)) return FALSE;
			break;
		default:
			return FALSE;
			break;
		}
	return TRUE;
}

BOOL GET_MaskHits(int DCAType, int WaveForms, double *MaskMargin)
{
	switch (DCAType)
		{
		case DCA_TYPE_CSA8000:
			if (!CSA8000_SET_MaskMargin(inst_CSA8000, 20, "ENET1250")) return FALSE;
			if (!CSA8000_GET_MaskCount(inst_CSA8000, 1000)) return FALSE;
			*MaskMargin = 20;
			break;
		case DCA_TYPE_Ag86100:
			if (!Ag86100_SET_MaskMargin(inst_Ag86100, 20, "\"01xGbEthernet.msk\"")) return FALSE;
			
			if (!Ag86100_GET_MaskHits(inst_Ag86100, 200)) return FALSE;
			*MaskMargin = 20;      
			break;
		default:
			return FALSE;
			break;
		}
	return TRUE;
}

BOOL GET10G_MaskHits(int DCAType, int WaveForms, double *MaskMargin)
{
	double tmp_MaskMargin;

	switch (DCAType)
	{
		case DCA_TYPE_CSA8000:						  
			if (!CSA8000_SET_MaskMargin(inst_CSA8000, my_struDBConfig.TxMargin, my_struCSA8000.MaskName)) return FALSE;
			if (!CSA8000_GET_MaskCount(inst_CSA8000, 1000)) return FALSE;
			*MaskMargin = my_struDBConfig.TxMargin;
			break;
		case DCA_TYPE_Ag86100:
			if (!Ag86100_Set_AutoScale (inst_Ag86100)) return FALSE;
			if (!Ag86100_SET_MaskMargin(inst_Ag86100, my_struDBConfig.TxMargin, my_struAg86100.MaskName )) return FALSE;     
			if (!Ag86100_GET_MaskHits(inst_Ag86100, 200)) return FALSE;
			*MaskMargin = my_struDBConfig.TxMargin;
			break;
		default:
			return FALSE;
			break;
	}
	return TRUE;
}

BOOL Set_DCA_Clear(int DCAType)
{
	switch (DCAType)
		{
		case DCA_TYPE_CSA8000:
			if(!CSA8000_Set_Clear(inst_CSA8000)) return FALSE; 
			break;
		case DCA_TYPE_Ag86100:
			if(!Ag86100_Set_Clear(inst_Ag86100)) return FALSE; 
			break;
		default:
			return FALSE;
			break;
		}
	return TRUE;
}

  /*
BOOL Set_DAC(INT16U R_NEW, INT16U R_OLD, int DAC_Sel)
{
int i, stepmin, stepmax, step, error;
BOOL bOK;
	
	bOK=TRUE;
	i=0; stepmin=50; stepmax=256;
	do
	{
		step=(i==0?stepmin:stepmax);
		if (fabs(R_NEW-R_OLD)>=step)
		{R_OLD=(R_NEW-R_OLD)>0?(R_OLD+step):(R_OLD-step);}
		else
		{R_OLD=R_NEW;bOK=FALSE;}
			
		if (DAC_Sel==0) 
		{
			if (my_struConfig.PN_TYPE==PN_TYPE_SFF)
			{
				if (!SFF_SET_DAC_APC(USB_Handle, R_OLD)) return FALSE;  	
			}
			else if (my_struConfig.PN_TYPE==PN_TYPE_SFP)
			{
				if (!SFP_SET_DAC_APC(USB_Handle, R_OLD)) return FALSE;
			}
			else 
			{
				MessagePopup ("提示", "Set_DAC函数识别产品类型异常"); 
				return FALSE;	
			}
		}
		else
		{
			if (my_struConfig.PN_TYPE==PN_TYPE_SFF)
			{
				if (!SFF_SET_DAC_MOD(USB_Handle, R_OLD)) return FALSE;  	
			}
			else if (my_struConfig.PN_TYPE==PN_TYPE_SFP)
			{
				if (!SFP_SET_DAC_MOD(USB_Handle, R_OLD)) return FALSE;
			}
			else 
			{
				MessagePopup ("提示", "Set_DAC函数识别产品类型异常"); 
				return FALSE;	
			}
		}
		
		i++;
		
	} while (bOK);
	
	return TRUE;
}		 
	  */
BOOL SaveEEPROM(int channel, struct struTestData data)
{
int  error, count, rom_StartAddress;  
BYTE device_addr, rom_value;
BYTE rom_value_arr_A0_Low[256], rom_value_arr_A0_table0[256], rom_value_arr_A0_table1[256], rom_value_arr_A0_table2[256];  
BYTE rom_value_arr_A2_Low[256], rom_value_arr_A2_table0[256], rom_value_arr_A2_table1[256];
BYTE CC_BASE, CC_EXT; 
BYTE rom_value_arr_A0_Low_r[256], rom_value_arr_A0_table0_r[256], rom_value_arr_A0_table1_r[256], rom_value_arr_A0_table2_r[256]; 
BYTE rom_value_arr_A2_Low_r[256], rom_value_arr_A2_table0_r[256], rom_value_arr_A2_table1_r[256];   
BYTE rom_value_arr_F2[256];
char data_str[20];
BOOL A0OK, A2OK;
  

	//Write A0h LOWER
	if(my_struConfig.A0_low)
	{
		for(count=0; count<128; count++)
		{
			rom_value_arr_A0_Low[count]=my_struDBConfig_E2.A0_Lower[count]; 
		}	 

		device_addr=0xA0;
		rom_StartAddress=0;
	
		error = I2C_BYTEs_WRITE_DLL (INST_EVB[channel], device_addr, rom_StartAddress, 128, rom_value_arr_A0_Low, 0.2);
		if (error<0) {MessagePopup ("ERROR", "NO Acknowledge from target !");return FALSE;}
	}
	
	//Write A0h Table 0
	if(my_struConfig.A0_tab0)
	{
		for(count=0; count<128; count++)
		{
			rom_value_arr_A0_table0[count +128]=my_struDBConfig_E2.A0_Table0[128]; 
		}
		
		device_addr=0xA0;
		rom_StartAddress=128;
	
		error = I2C_4BYTEs_WRITE_DLL (INST_EVB[channel], 0xA0, 0x7B, 0xD3, 0x4F, 0x45, 0x42, 0.1);//写0级密码 
		error = I2C_BYTE_WRITE_DLL (INST_EVB[channel], 0xA0, 127, 0, 0.1);   
			
		error = I2C_BYTEs_WRITE_DLL (INST_EVB[channel], device_addr, rom_StartAddress, 128, rom_value_arr_A0_table0, 0.2);
		if (error<0) {MessagePopup ("ERROR", "NO Acknowledge from target !");return FALSE;}
	}
	
	//Write A0h Table 1
	if(my_struConfig.A0_tab1)
	{
		for(count=0; count<128; count++)
		{
			rom_value_arr_A0_table1[128+count]=my_struDBConfig_E2.A0_Table1[count];
		}
	
		//set Vendor SN 
		for (count=0; count<16; count++)  
		{
	  		if (data.ModuleSN[count] != 0 ) 
			{
				rom_value_arr_A0_table1[128+68+count]=data.ModuleSN[count]; 
			}
	 		else 
			{
				rom_value_arr_A0_table1[128+68+count]=0x20 ;  //遇到0结束符，用空格代替
			}
		} 
	
		//set Vendor DATE 
		MyDLLGetSystemDate(data_str);  
		rom_value_arr_A0_table1[128+84]=data_str[2];   //year MSB   //locate from A0h[84]
		rom_value_arr_A0_table1[128+85]=data_str[3];  //year LSB
		rom_value_arr_A0_table1[128+86]=data_str[5];   //month MSB
		rom_value_arr_A0_table1[128+87]=data_str[6];   //month LSB
		rom_value_arr_A0_table1[128+88]=data_str[8];  //date MSB
		rom_value_arr_A0_table1[128+89]=data_str[9];  //date LSB
		rom_value_arr_A0_table1[128+90]=' ';  //space key, 0x20
		rom_value_arr_A0_table1[128+91]=' ';  //space key, 0x20
			
		//re-calculate check-sum, A0h[0~62]
		CC_BASE=0;
		for (count=0; count<63; count++)
		{ 
			CC_BASE += rom_value_arr_A0_table1[128+count]; 
		}
		rom_value_arr_A0_table1[128+63] = CC_BASE;

		//re-calculate check-sum, A0h[64~94]
		CC_EXT=0;
		for (count=64; count<95; count++)
		{ 
			CC_EXT += rom_value_arr_A0_table1[128+count]; 
		}
		rom_value_arr_A0_table1[128+95] = CC_EXT;   
	
		device_addr=0xA0;
		rom_StartAddress=128;
	
		error = I2C_4BYTEs_WRITE_DLL (INST_EVB[channel], 0xA0, 0x7B, 0xD3, 0x4F, 0x45, 0x42, 0.1);//写0级密码 
		error = I2C_BYTE_WRITE_DLL (INST_EVB[channel], 0xA0, 127, 1, 0.1);   
			
		error = I2C_BYTEs_WRITE_DLL (INST_EVB[channel], device_addr, rom_StartAddress, 128, rom_value_arr_A0_table1, 0.2);
		if (error<0) {MessagePopup ("ERROR", "NO Acknowledge from target !");return FALSE;}
	}
	
	//Write A0h Table 2
	if(my_struConfig.A0_tab2)
	{
		for(count=0; count<128; count++)
		{
			rom_value_arr_A0_table2[count +128]=my_struDBConfig_E2.A0_Table2[count]; 
		}
		
		device_addr=0xA0;
		rom_StartAddress=128;
	
		error = I2C_4BYTEs_WRITE_DLL (INST_EVB[channel], 0xA0, 0x7B, 0xD3, 0x4F, 0x45, 0x42, 0.1);//写0级密码 
		error = I2C_BYTE_WRITE_DLL (INST_EVB[channel], 0xA0, 127, 2, 0.1);   
			
		error = I2C_BYTEs_WRITE_DLL (INST_EVB[channel], device_addr, rom_StartAddress, 128, rom_value_arr_A0_table2, 0.2);
		if (error<0) {MessagePopup ("ERROR", "NO Acknowledge from target !");return FALSE;}
	}
	
	//Write A2h LOWER
	if(my_struConfig.A2_low)
	{
		for(count=0; count<128; count++)
		{
			rom_value_arr_A2_Low[count]=my_struDBConfig_E2.A2[count]; 
		}
		
		device_addr=0xA2;
		rom_StartAddress=0;
	
		error = I2C_4BYTEs_WRITE_DLL (INST_EVB[channel], 0xA0, 0x7B, 0xD3, 0x4F, 0x45, 0x42, 0.1);//写0级密码 

		error = I2C_BYTEs_WRITE_DLL (INST_EVB[channel], device_addr, rom_StartAddress, 128, rom_value_arr_A2_Low, 0.2);
		if (error<0) {MessagePopup ("ERROR", "NO Acknowledge from target !");return FALSE;}
	}
	
	//Write A2h Table 0
	if(my_struConfig.A2_tab0)
	{
		for(count=0; count<128; count++)
		{
			rom_value_arr_A2_table0[count + 128]=my_struDBConfig_E2.A2_Table0[count]; 
		}
		
		device_addr=0xA2;
		rom_StartAddress=128;
	
		error = I2C_4BYTEs_WRITE_DLL (INST_EVB[channel], 0xA0, 0x7B, 0xD3, 0x4F, 0x45, 0x42, 0.1);//写0级密码 
		error = I2C_BYTE_WRITE_DLL (INST_EVB[channel], 0xA2, 127, 0, 0.1);   
			
		error = I2C_BYTEs_WRITE_DLL (INST_EVB[channel], device_addr, rom_StartAddress, 128, rom_value_arr_A2_table0, 0.2);
		if (error<0) {MessagePopup ("ERROR", "NO Acknowledge from target !");return FALSE;}
	}
	
	//Write A2h Table 1
	if(my_struConfig.A2_tab1)
	{
		for(count=0; count<128; count++)
		{
			rom_value_arr_A2_table1[count + 128]=my_struDBConfig_E2.A2_Table1[count]; 
		}
		
		device_addr=0xA2;
		rom_StartAddress=128;
	
		error = I2C_4BYTEs_WRITE_DLL (INST_EVB[channel], 0xA0, 0x7B, 0xD3, 0x4F, 0x45, 0x42, 0.1);//写0级密码 
		error = I2C_BYTE_WRITE_DLL (INST_EVB[channel], 0xA2, 127, 1, 0.1);   
			
		error = I2C_BYTEs_WRITE_DLL (INST_EVB[channel], device_addr, rom_StartAddress, 128, rom_value_arr_A2_table1, 0.2);
		if (error<0) {MessagePopup ("ERROR", "NO Acknowledge from target !");return FALSE;}
	}

	
	//添加检查代码
//	Power off
	if (SET_EVB_SHDN(channel, 0)<0) return FALSE;
	Delay(0.1);
	//Power on
	if (SET_EVB_SHDN(channel, 1)<0) return FALSE;     
	Delay(1.5);
	
	//Check A0 LOWER
	if(my_struConfig.A0_low)
	{
		error = I2C_4BYTEs_WRITE_DLL (INST_EVB[channel], 0xA0, 0x7B, 0xD3, 0x4F, 0x45, 0x42, 0.1);//写0级密码 
		error = I2C_BYTE_WRITE_DLL (INST_EVB[channel], 0xA0, 127, 1, 0.1); 
	
		device_addr=0xA0;
		rom_StartAddress=0;
		
		error = I2C_BYTEs_READ_DLL (INST_EVB[channel], device_addr, rom_StartAddress, 128, rom_value_arr_A0_Low_r);  
		if (error<0) {MessagePopup ("ERROR", "NO Acknowledge from target !");return FALSE;} 
	
		A0OK=TRUE;
		for (count=0; count<42; count++)
		{
			if (rom_value_arr_A0_Low[count] != rom_value_arr_A0_Low_r[count]) 
			{
				A0OK=FALSE;
				break;
			}
		}
	}
	
	//Check A0 Table 0
	if(my_struConfig.A0_tab0)
	{
		error = I2C_4BYTEs_WRITE_DLL (INST_EVB[channel], 0xA0, 0x7B, 0xD3, 0x4F, 0x45, 0x42, 0.1);//写0级密码 
		error = I2C_BYTE_WRITE_DLL (INST_EVB[channel], 0xA0, 127, 0, 0.1); 
		
		device_addr=0xA0;
		rom_StartAddress=128;
		
		error = I2C_BYTEs_READ_DLL (INST_EVB[channel], device_addr, rom_StartAddress, 128, rom_value_arr_A0_table0_r);  
		if (error<0) {MessagePopup ("ERROR", "NO Acknowledge from target !");return FALSE;} 
	

		for (count=0; count<128; count++)
		{
			if (rom_value_arr_A0_table0[count +128] != rom_value_arr_A0_table0_r[count+128]) 
			{
				A0OK=FALSE;
				break;
			}
		}
	}
	
	//Check A0 Table 1
	if(my_struConfig.A0_tab1)
	{
		error = I2C_4BYTEs_WRITE_DLL (INST_EVB[channel], 0xA0, 0x7B, 0xD3, 0x4F, 0x45, 0x42, 0.1);//写0级密码 
		error = I2C_BYTE_WRITE_DLL (INST_EVB[channel], 0xA0, 127, 1, 0.1); 
		
		device_addr=0xA0;
		rom_StartAddress=128;
		
		error = I2C_BYTEs_READ_DLL (INST_EVB[channel], device_addr, rom_StartAddress, 128, rom_value_arr_A0_table1_r);  
		if (error<0) {MessagePopup ("ERROR", "NO Acknowledge from target !");return FALSE;} 
	

		for (count=0; count<128; count++)
		{
			if (rom_value_arr_A0_table1[count +128] != rom_value_arr_A0_table1_r[count +128]) 
			{
				A0OK=FALSE;
				break;
			}
		}
	}
	
	//Check A0 Table 2
	if(my_struConfig.A0_tab2)
	{
		error = I2C_4BYTEs_WRITE_DLL (INST_EVB[channel], 0xA0, 0x7B, 0xD3, 0x4F, 0x45, 0x42, 0.1);//写0级密码 
		error = I2C_BYTE_WRITE_DLL (INST_EVB[channel], 0xA0, 127, 2, 0.1); 
		
		device_addr=0xA0;
		rom_StartAddress=128;
		
		error = I2C_BYTEs_READ_DLL (INST_EVB[channel], device_addr, rom_StartAddress, 128, rom_value_arr_A0_table2_r);  
		if (error<0) {MessagePopup ("ERROR", "NO Acknowledge from target !");return FALSE;} 
	

		for (count=0; count<128; count++)
		{
			if (rom_value_arr_A0_table2[count +128] != rom_value_arr_A0_table2_r[count+128]) 
			{
				A0OK=FALSE;
				break;
			}
		}
	}
	
	//Check A2 LOWER
	if(my_struConfig.A2_low)
	{
		error = I2C_4BYTEs_WRITE_DLL (INST_EVB[channel], 0xA0, 0x7B, 0xD3, 0x4F, 0x45, 0x42, 0.1);//写0级密码 

		device_addr=0xA2;
		rom_StartAddress=0;
		
		error = I2C_BYTEs_READ_DLL (INST_EVB[channel], device_addr, rom_StartAddress, 128, rom_value_arr_A2_Low_r);  
		if (error<0) {MessagePopup ("ERROR", "NO Acknowledge from target !");return FALSE;} 
	

		for (count=0; count<128; count++)
		{
			if (rom_value_arr_A2_Low[count] != rom_value_arr_A2_Low_r[count +128]) 
			{
				A0OK=FALSE;
				break;
			}
		}
	}
	
	//Check A2 Table 0
	if(my_struConfig.A2_tab0)
	{
		error = I2C_4BYTEs_WRITE_DLL (INST_EVB[channel], 0xA0, 0x7B, 0xD3, 0x4F, 0x45, 0x42, 0.1);//写0级密码 
		error = I2C_BYTE_WRITE_DLL (INST_EVB[channel], 0xA2, 127, 0, 0.1); 
		
		device_addr=0xA2;
		rom_StartAddress=128;
		
		error = I2C_BYTEs_READ_DLL (INST_EVB[channel], device_addr, rom_StartAddress, 128, rom_value_arr_A2_table0_r);  
		if (error<0) {MessagePopup ("ERROR", "NO Acknowledge from target !");return FALSE;} 
	

		for (count=0; count<128; count++)
		{
			if (rom_value_arr_A2_table0[count +128] != rom_value_arr_A2_table0_r[count+128]) 
			{
				A0OK=FALSE;
				break;
			}
		}
	}
	
	//Check A2 Table 1
	if(my_struConfig.A2_tab1)
	{
		error = I2C_4BYTEs_WRITE_DLL (INST_EVB[channel], 0xA0, 0x7B, 0xD3, 0x4F, 0x45, 0x42, 0.1);//写0级密码 
		error = I2C_BYTE_WRITE_DLL (INST_EVB[channel], 0xA2, 127, 1, 0.1); 
		
		device_addr=0xA2;
		rom_StartAddress=128;
		
		error = I2C_BYTEs_READ_DLL (INST_EVB[channel], device_addr, rom_StartAddress, 128, rom_value_arr_A2_table1_r);  
		if (error<0) {MessagePopup ("ERROR", "NO Acknowledge from target !");return FALSE;} 
	

		for (count=0; count<128; count++)
		{
			if (rom_value_arr_A2_table1_r[count +128] != rom_value_arr_A2_table1_r[count+128]) 
			{
				A0OK=FALSE;
				break;
			}
		}
	}
	

	if (!A0OK)
	{
	return FALSE;
	}
	return TRUE;
}		  

BOOL DB_GET_Firmware_Ver(void)
{
int  num;
char buf[256];
char myFirmwareVer[50];
char mySFRM_Number[50];
char myFEP_Number[50];

	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */

	num=0;
	memset (my_struDBConfig.FirmwareVer, 0, 50);
	memset (myFirmwareVer, 0, 50); 
	
	memset (my_struDBConfig.SFRM_Number, 0, 50);
	memset (mySFRM_Number, 0, 50); 
	
	memset (my_struDBConfig.FEP_Number, 0, 50);
	memset (myFEP_Number, 0, 50); 
	
	Fmt (buf, "SELECT Firmware_Ver, SFRM_Number, FEP_Number FROM AutoDT_Spec_Image where PartNumber='%s' and Version='%s'", my_struConfig.PN, my_struConfig.Ver);
	
	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return FALSE;}  
    
    resCode = DBBindColChar (hstmt, 1, 50, myFirmwareVer, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColChar (hstmt, 2, 50, mySFRM_Number, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColChar (hstmt, 3, 50, myFEP_Number, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
    
    while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) {num++;}      
    
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) {ShowDataBaseError();  return FALSE;}
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	if (num!=1) {MessagePopup("Error","读取数据库AutoDT_Spec_Image表的Firmware Version失败！"); return FALSE;} 
	
	strcpy (my_struDBConfig.FirmwareVer, myFirmwareVer);
	strcpy (my_struDBConfig.SFRM_Number, mySFRM_Number);  
	strcpy (my_struDBConfig.FEP_Number,  myFEP_Number);
	
	return TRUE;
}
/*
BOOL SET_EVB6_MODE_MANUAL(void)
{
	sTriggerMode.OLT_Trigger=1;
	sTriggerMode.OLT_RxRst=1;
	sTriggerMode.ONU1_BEN =1;
	sTriggerMode.ONU2_BEN =1;
	
	if (!EVB6_SET_OLT_Trigger_RxRst_Mode (EVB6_Handle, OLT_TRIGGER_RXRST_MODE_MANUAL)) return FALSE; 
	if (!EVB6_SET_ONU_BEN_Mode (EVB6_Handle, ONU_BEN_MODE_MANUAL)) return FALSE; 
	if (!EVB6_SET_OLT_Trigger (EVB6_Handle, sTriggerMode.OLT_Trigger)) return FALSE; 
	if (!EVB6_SET_OLT_RxRst (EVB6_Handle, sTriggerMode.OLT_RxRst)) return FALSE; 
	
	//EPON 只需要一个ONU，ONU1可不用控制	
	if (!EVB6_SET_ONU1_BEN (EVB6_Handle, sTriggerMode.ONU1_BEN)) return FALSE;  
	
	if (!EVB6_SET_ONU2_BEN (EVB6_Handle, sTriggerMode.ONU2_BEN)) return FALSE; 
					
	return TRUE;
}

BOOL SET_EVB6_MODE_SEMIAUTO(void)
{
	if (strcmp (my_struConfig.PN, "PHHW6388-POLT-SI-B")==0) 
	{
		sTriggerMode.OLT_Trigger_PulseWidth =2000;	//2    us
		sTriggerMode.OLT_Trigger_Delay =500;		//500  ns  
		sTriggerMode.ONU_BEN_PulseWidth=2540; 		//2.54 us 
	}
	else if (strcmp (my_struConfig.PN, "PHHW6388-POLT-SI-C")==0) 
	{
		sTriggerMode.OLT_Trigger_PulseWidth =600;	//600  ns
		sTriggerMode.OLT_Trigger_Delay =512;		//512  ns  
		sTriggerMode.ONU_BEN_PulseWidth=1200; 		//1.2  us 
	}
	else
	{MessagePopup ("Error", "收端光功率监控模式选择有误，不支持当前测试PN");return FALSE;}     	
	
	if (!EVB6_SET_OLT_Trigger_PulseWidth (EVB6_Handle, sTriggerMode.OLT_Trigger_PulseWidth)) return FALSE; 
	if (!EVB6_SET_OLT_Trigger_Delay (EVB6_Handle, sTriggerMode.OLT_Trigger_Delay)) return FALSE; 
	
	if (!EVB6_SET_ONU_BEN_PulseWidth (EVB6_Handle, sTriggerMode.ONU_BEN_PulseWidth)) return FALSE; 
	if (!EVB6_SET_OLT_Trigger_RxRst_Mode (EVB6_Handle, OLT_TRIGGER_RXRST_MODE_SEMIAUTO)) return FALSE; 
	if (!EVB6_SET_ONU_BEN_Mode (EVB6_Handle, ONU_BEN_MODE_SEMIAUTO)) return FALSE; 
	if (!EVB6_SET_ONU_BEN_EN (EVB6_Handle)) return FALSE; 
	
	return TRUE;
}		 */

BOOL DB_GET_SPEC_CSA8000(void)
{   
	char buf[256];
	int	 num;
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */

	Fmt (buf, "SELECT X_Scale,X_Position,Y_Scale,Y_Position,Y_Offset,Filter,Wavelength,MaskMargin,MaskName,Channel_O,ER_Slope,ER_Offset from AutoDT_Spec_CSA8000 where PN='%s' AND MainSN='%s'", my_struConfig.PN, my_struCSA8000.MainSN); 
	
	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return FALSE;}  
    
    resCode = DBBindColFloat (hstmt, 1, &my_struCSA8000.X_Scale, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
    resCode = DBBindColFloat (hstmt, 2, &my_struCSA8000.X_Position, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
    resCode = DBBindColFloat (hstmt, 3, &my_struCSA8000.Y_Scale, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
    resCode = DBBindColFloat (hstmt, 4, &my_struCSA8000.Y_Position, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
    resCode = DBBindColFloat (hstmt, 5, &my_struCSA8000.Y_Offset, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
    resCode = DBBindColChar (hstmt, 6, 50, my_struCSA8000.Filter, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
    resCode = DBBindColInt (hstmt, 7, &my_struCSA8000.Wavelength, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
    resCode = DBBindColInt (hstmt, 8, &my_struCSA8000.MaskMargin, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
    resCode = DBBindColChar (hstmt, 9, 50, my_struCSA8000.MaskName, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
    resCode = DBBindColInt (hstmt, 10, &my_struCSA8000.Channel_O, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
    resCode = DBBindColFloat (hstmt, 11, &my_struTxCal.TxCal_Er_Slope, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
    resCode = DBBindColFloat (hstmt, 12, &my_struTxCal.TxCal_Er_Offset, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
	
	num=0; 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) {num++;}      
    
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) {ShowDataBaseError();  return FALSE;}
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	if (num!=1) {MessagePopup("提示","数据库AutoDT_Spec_CSA8000中不能找到数据或者数据记录多于一条！"); return FALSE;} 
	
	return TRUE;
}

BOOL DB_GET_SPEC_Ag86100(void)
{   
	char buf[256];
	int	 num;
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */

	Fmt (buf, "SELECT X_Scale,Y_Scale,Y_Offset,ErFactor,Filter,Wavelength,MaskMargin,MaskName,ER_Slope,ER_Offset,X_Position,channel_o from AutoDT_Spec_Ag86100 where PN='%s' AND MainSN='%s'", my_struConfig.PN, my_struAg86100.MainSN); 
	
	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return FALSE;}  
    
    resCode = DBBindColFloat (hstmt, 1, &my_struAg86100.X_Scale, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
    resCode = DBBindColFloat (hstmt, 2, &my_struAg86100.Y_Scale, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
    resCode = DBBindColFloat (hstmt, 3, &my_struAg86100.Y_Offset, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
	resCode = DBBindColFloat (hstmt, 4, &my_struAg86100.ERFactor, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
    resCode = DBBindColInt (hstmt, 5, &my_struAg86100.Filter, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
    resCode = DBBindColInt (hstmt, 6, &my_struAg86100.Wavelength, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
    resCode = DBBindColInt (hstmt, 7, &my_struAg86100.MaskMargin, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
    resCode = DBBindColChar (hstmt, 8, 50, my_struAg86100.MaskName, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
    resCode = DBBindColFloat (hstmt, 9, &my_struTxCal.TxCal_Er_Slope, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
    resCode = DBBindColFloat (hstmt, 10, &my_struTxCal.TxCal_Er_Offset, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
    resCode = DBBindColFloat (hstmt, 11, &my_struAg86100.X_Position, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
	resCode = DBBindColInt (hstmt, 12, &my_struAg86100.Channel_O, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	
	
	num=0; 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) {num++;}      
    
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) {ShowDataBaseError();  return FALSE;}
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	if (num!=1) {MessagePopup("提示","数据库AutoDT_Spec_Ag86100中不能找到数据或者数据记录多于一条！"); return FALSE;} 
	
	return TRUE;
}

int DB_READ_Spec_Tracking_Ex(int *RSSIMode)  
{
	char buf[1024], myRSSITime[50]; 
	int  num, myRSSIMode;
	char losflag[30] = "";
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */
	
	Fmt (buf, "SELECT RSSIMode,RSSI_N1,RSSI_N3,RSSI_N2,RSSI_TIME,RSSI_TRIG_Level,RapdtoGND,Rapd,Kh,Kl,Vapd_K1,Vapd_K2,APD_Protect,A0_Protect, isLOSorSD, APD_DAC_MIN, APD_DAC_MAX, APD_DAC_MIN_H, APD_DAC_MAX_H, APD_DAC_MIN_L, APD_DAC_MAX_L from AutoDT_Spec_Tracking_Ex WHERE PartNumber='%s' AND BarBOM='%s'", my_struConfig.PN, my_struConfig.Ver); 

	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return -1;}  
	
	resCode = DBBindColInt (hstmt, 1, &myRSSIMode, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColInt (hstmt, 2, &my_struConfig.RSSI_N1, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}	
	resCode = DBBindColInt (hstmt, 3, &my_struConfig.RSSI_N3, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}	
	resCode = DBBindColInt (hstmt, 4, &my_struConfig.RSSI_N2, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}	
    resCode = DBBindColChar(hstmt, 5, 50, myRSSITime, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColInt (hstmt, 6, &my_struConfig.RSSI_Trig_Level, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}	
	resCode = DBBindColFloat (hstmt, 7, &my_struConfig.RapdtoGND, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}	
	resCode = DBBindColFloat (hstmt, 8, &my_struConfig.Rapd, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}	
	resCode = DBBindColFloat (hstmt, 9, &my_struConfig.Kh, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}	
	resCode = DBBindColFloat (hstmt, 10, &my_struConfig.Kl, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}	
	resCode = DBBindColFloat (hstmt, 11, &my_struConfig.Vapd_K1, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}	
	resCode = DBBindColFloat (hstmt, 12, &my_struConfig.Vapd_K2, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}	
	resCode = DBBindColInt (hstmt, 13, &my_struConfig.APD_Protect, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}	
	resCode = DBBindColInt (hstmt, 14, &my_struConfig.A0_Protect, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColChar (hstmt, 15, 30, losflag, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	resCode = DBBindColInt (hstmt, 16, &my_struConfig.APD_DAC_MIN, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColInt (hstmt, 17, &my_struConfig.APD_DAC_MAX, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	
	resCode = DBBindColInt (hstmt, 18, &my_struConfig.APD_DAC_MIN_H, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColInt (hstmt, 19, &my_struConfig.APD_DAC_MAX_H, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	
	resCode = DBBindColInt (hstmt, 20, &my_struConfig.APD_DAC_MIN_L, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColInt (hstmt, 21, &my_struConfig.APD_DAC_MAX_L, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	
	num=0; 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
	{num++;*RSSIMode=myRSSIMode;}
	
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
    
    if (num!=1)
	{MessagePopup ("ERROR", "读取数据库AutoDT_Spec_Tracking_Ex中的记录失败！"); return -1;}  
	
	MyDLL_TRIM (losflag);
	if  	(stricmp (losflag, "LOS") ==0) {my_struDBConfig.isSDorLOS=FALSE;}
	else if (stricmp (losflag, "SD") ==0)  {my_struDBConfig.isSDorLOS=TRUE;} 
	else {MessagePopup ("提示", "数据库AutoDT_Spec_Tracking_Ex中定义的isLOSorSD有错误！"); return -1;} 

	//当RSSIMode>0才做以下判断
	if (myRSSIMode>0)
	{
		//识别RSSI时序类型
		if      (strcmp (myRSSITime, "N1N3")==0) {my_struDBConfig_ERCompens.RSSI_TIME=RSSI_TIME_N1N3;}
		else if (strcmp (myRSSITime, "N2")==0)   {my_struDBConfig_ERCompens.RSSI_TIME=RSSI_TIME_N2;} 
		else if (strcmp (myRSSITime, "L2L3")==0) {my_struDBConfig_ERCompens.RSSI_TIME=RSSI_TIME_L2L3;} 
		else if (strcmp (myRSSITime, "L1L2")==0) {my_struDBConfig_ERCompens.RSSI_TIME=RSSI_TIME_L1L2;} 
		else {MessagePopup ("提示", "程序不支持AutoDT_Spec_Tracking_Ex表中RSSI_TIME的设置"); return -1;}	
	
		//根据时序内型检查数据库的配置是否不等于0，防止时序设置错误
		if ((my_struConfig.RSSI_N1==0 || my_struConfig.RSSI_N3==0) && my_struDBConfig_ERCompens.RSSI_TIME==RSSI_TIME_N1N3) 
		{MessagePopup ("提示", "请检查AutoDT_Spec_Tracking_Ex表中的RSSI_N1和RSSI_N3配置是否正确"); return -1;}
		
		if (my_struConfig.RSSI_N2==0 && my_struDBConfig_ERCompens.RSSI_TIME==RSSI_TIME_N2) 
		{MessagePopup ("提示", "请检查AutoDT_Spec_Tracking_Ex表中的RSSI_N2配置是否正确"); return -1;}
		
		if ((my_struConfig.RSSI_N2==0 || my_struConfig.RSSI_N3==0) && my_struDBConfig_ERCompens.RSSI_TIME==RSSI_TIME_L2L3) 
		{MessagePopup ("提示", "请检查AutoDT_Spec_Tracking_Ex表中的RSSI_N2和RSSI_N3配置是否正确"); return -1;}
		
		if ((my_struConfig.RSSI_N1==0 || my_struConfig.RSSI_N2==0) && my_struDBConfig_ERCompens.RSSI_TIME==RSSI_TIME_L1L2) 
		{MessagePopup ("提示", "请检查AutoDT_Spec_Tracking_Ex表中的RSSI_N1和RSSI_N2配置是否正确"); return -1;}
	}
	
	Fmt (buf, "SELECT A2Temper_K, A2Temper_B, A2Temper_Kl, A2Temper_Bl, Los_Kh, Los_Kl FROM AutoDT_Spec_Tracking_Ex1 WHERE PartNumber='%s' AND BOM='%s'", my_struConfig.PN, my_struConfig.Ver);

	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return -1;}  
	
	resCode = DBBindColFloat (hstmt, 1, &my_struConfig.Temper_K, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}	
	resCode = DBBindColFloat (hstmt, 2, &my_struConfig.Temper_B, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}	
	resCode = DBBindColFloat (hstmt, 3, &my_struConfig.Temper_Kl, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColFloat (hstmt, 4, &my_struConfig.Temper_Bl, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColFloat (hstmt, 5, &my_struConfig.Los_Kh, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColFloat (hstmt, 6, &my_struConfig.Los_Kl, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	
	num=0; 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) {num++;}
	
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
    
    if (num!=1)
	{MessagePopup ("提示", "读取数据库AutoDT_Spec_Tracking_Ex1中的记录失败！"); return -1;}  	 

	return 0;
}

int SET_EVB_SHDN(int channel, unsigned char SHUTValue)
{
	switch (my_struInstrument[channel].SEVB_TYPE)
	{
		case SEVB_TYPE_SEVB5:
		case SEVB_TYPE_SEVB27:
			if (EVB5_SET_SHDN_VTR(INST_EVB[channel], SHUTValue)<0)
			{
				return -1;
			}
			break; 	
		default:
			{
				MessagePopup("Error", "Can not find this EVB type");
			} 
			return -1;
			break;
	}
	return 0;
}

BOOL SET_EVB_TxDis(int channel, unsigned char TxDis)
{
	switch (my_struInstrument[channel].SEVB_TYPE)
	{
		case SEVB_TYPE_SEVB5:
		case SEVB_TYPE_SEVB27: 
			if (EVB5_SET_OLT_TxDIS(INST_EVB[channel], TxDis)<0) return FALSE;
			break; 	
		default:
			{MessagePopup("Error", "Can not find this EVB type");} 
			return FALSE;
			break;
	}
	return TRUE;	
}

int SET_EVB_Light_MODE_RSSI(int channel)
{
	int error;  

	if (my_struInstrument[channel].ATT_TYPE_MAIN == ATT_TYPE_GVPM)
	{
		//关闭锁定输出模式
		error = GVPM_SET_LockEnable (INST_ATT[channel], 0);	
		if (error) {MessagePopup ("Error", "关闭GVPM锁定模式异常"); return -1;}    
	}
	
	switch (my_struInstrument[channel].EVB_Light_TYPE)
	{
		case SEVB_TYPE_SEVB5:
		case SEVB_TYPE_SEVB27:
			if      (my_struDBConfig_ERCompens.RSSI_TIME==RSSI_TIME_N1N3)  {if (EVB5_SET_BEN_Mode (INST_EVB_Light[channel], EVB5_BEN_MODE_N1N3)<0) return -1;}
			else if (my_struDBConfig_ERCompens.RSSI_TIME==RSSI_TIME_N2)    {if (EVB5_SET_BEN_Mode (INST_EVB_Light[channel], EVB5_BEN_MODE_N2)<0) return -1;} 
			else if (my_struDBConfig_ERCompens.RSSI_TIME==RSSI_TIME_L2L3)  {if (EVB5_SET_BEN_Mode (INST_EVB_Light[channel], EVB5_BEN_MODE_L2L3)<0) return -1;}
			else if (my_struDBConfig_ERCompens.RSSI_TIME==RSSI_TIME_L1L2)  {if (EVB5_SET_BEN_Mode (INST_EVB_Light[channel], EVB5_BEN_MODE_L1L2)<0) return -1;}  
			else {MessagePopup ("提示", "程序不支持AutoDT_Spec_Tracking_Ex表中RSSI_TIME的设置"); return -1;}
			
			if (EVB5_SET_BEN_DELAY(INST_EVB_Light[channel], my_struDBConfig_ERCompens.RSSI_N1, my_struDBConfig_ERCompens.RSSI_N2, my_struDBConfig_ERCompens.RSSI_N3)<0) return -1;
			break;
			
		case SEVB_TYPE_SEVB6:
			//evb6没有添加参数设置功能
			break; 
			
		default:											 
			{MessagePopup("Error", "Can not find this EVB light type");} 
			return -1;
			break;
	}
	
	return 0;
}

int SET_EVB_Light_BEN_Trig_Gen(int channel)
{
	switch (my_struInstrument[channel].EVB_Light_TYPE)
	{
		case SEVB_TYPE_SEVB5:
		case SEVB_TYPE_SEVB27:
			if (EVB5_SET_BEN_Level(INST_EVB_Light[channel], 1)<0) return -1;
			break; 	
		default:
			{MessagePopup("Error", "Can not find this EVB light type");} 
			return -1;
			break;
	}
	return 0;
}

int SET_EVB_Light_MODE_Level(int channel)
{
	int error;
		
	switch (my_struInstrument[channel].EVB_Light_TYPE)
	{
		case SEVB_TYPE_SEVB5:
		case SEVB_TYPE_SEVB27:
			if (EVB5_SET_BEN_Mode(INST_EVB_Light[channel], EVB5_BEN_MODE_LEVEL)<0) return -1; 
			//设置BEN_level, 高电平发光
			if (EVB5_SET_BEN_Level(INST_EVB_Light[channel], 1)<0) return -1;  
			break; 	
		default:
			{MessagePopup("Error", "Can not find this EVB light type");} 
			return -1;
			break;
	}
	
	if (my_struInstrument[channel].ATT_TYPE_MAIN == ATT_TYPE_GVPM)
	{
		//开启锁定输出模式																																							 -
		error = GVPM_SET_LockEnable (INST_ATT[channel], 1);	
		if (error) {MessagePopup ("Error", "开启GVPM锁定模式异常"); return -1;}    
	}	 
	
	return 0;
}

int DB_READ_AutoDT_Spec_ATE_Item(void)  
{
	char buf[1024];
	int	 num;
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */
	
	Fmt (buf, "SELECT ProductLine,T_AOP,T_ER,T_Dis,T_Eye,T_Mask,T_Spectrum,R_Sens,R_OverLoad,R_SDHys,Calibrate,Calibrate_Test, EEPROM_Protect,T_SoftDis, T_CROSS FROM AutoDT_Spec_ATE_Item WHERE PN='%s' AND DT_Flag='%s' AND Temper_Flag='%s'", my_struConfig.PN, my_struConfig.DT_Flag, my_struConfig.Temper_Flag);

	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return -1;}  
	
	resCode = DBBindColInt (hstmt, 1, &my_struConfig.ProductLine, &DBConfigStat); 
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;} 
	resCode = DBBindColInt (hstmt, 2, &my_struConfig.Sel_T_AOP, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColInt (hstmt, 3, &my_struConfig.Sel_T_ER, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColInt (hstmt, 4, &my_struConfig.Sel_T_TxDis, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColInt (hstmt, 5, &my_struConfig.Sel_T_Eye, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColInt (hstmt, 6, &my_struConfig.Sel_T_Mask, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColInt (hstmt, 7, &my_struConfig.Sel_T_Spectrum, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColInt (hstmt, 8, &my_struConfig.Sel_R_Sens, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColInt (hstmt, 9, &my_struConfig.Sel_R_Over, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColInt (hstmt, 10, &my_struConfig.Sel_R_SDHys, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColInt (hstmt, 11, &my_struConfig.Sel_Calibrate, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColInt (hstmt, 12, &my_struConfig.Sel_Calibrate_Test, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColInt (hstmt, 13, &my_struConfig.Sel_EE_P, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColInt (hstmt, 14, &my_struConfig.Sel_T_TxDis_soft, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;} 
	resCode = DBBindColInt (hstmt, 15, &my_struConfig.Sel_T_Cross, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	
	num=0; 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) {num++;}
	
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
    
    if (num<1)
	{MessagePopup ("ERROR", "读取数据库AutoDT_Spec_ATE_Item中的记录失败"); return -1;}  
	
	return 0;
}

BOOL resetdata(int channel, struct struTestData *data, struct struProcessLOG *prolog)
{
//	int i;
	int error;
	
	union utestdata
	{
	struct struTestData sStr;
	unsigned char pStr[sizeof(struct struTestData)];
	} uTestData;

	//通过创建本地union变量，赋值0后，再传给测试结果存储结构体
	memset (uTestData.pStr, 0, sizeof (uTestData.pStr));
	*data = uTestData.sStr;  
	
	//检查光纤使用情况
	if (my_struConfig.ProductLine==PRODUCTLINE_CD && !my_struConfig.isNPI_flag)
	{
		error = MyDLL_FiberManage_Run (hdbc, my_struInstrument[channel].Fiber, &data->FiberTime);
		if (error) {return FALSE;}
	}
	else
	{
		data->FiberTime=0;	
	}
		
	return TRUE;
}

int Initial(int panel, int panMain, int *error_channel, int ini_Flag)
{
	int	iIndex, error; 
	int channel;   
	char Info[256] = "";

	struct struInstrument localInst; 
	int progresshandle, dca_no_init, spectrum_no_init;  
	int sw_no_init, clock_no_init;
	int mydev;
	char tempFirmwareVer[20];

	channel=0;
	DisplayPanel (panel);   //display initial panel
	//get software version
	strcpy (my_struProcessLOG.SoftVersion, SOFTVER); 
	//get computername
	Status = MyDLLGetComputerName(my_struProcessLOG.StationID);
	if(!Status) 
	{ 
		MessagePopup ("Error", "Read Computer Name error !"); 
		goto Error;
	} 

	//获取配置文件路径
	readconfigfilename();
	
	//初始化数据库配置
	Status = DB_Initial();
	if(!Status) 
	{ 
		MessagePopup ("Error", "Initial database error !"); 
		goto Error; 
	}

	//读取功能配置文件
	errChk(GetConfig()); 

	//读取设备配置文件
	errChk(GetConfig_Inst());	 

	errChk(GetCaliConfigFile ());
	
	//程序主界面控件配置
	for (channel=0; channel<CHANNEL_MAX; channel++)
	{
		localInst = my_struInstrument[channel]; 
		InsertTree (panMain, PAN_MAIN_TREE, &localInst, 1); 
		
		//测试状态栏属性配置
		SetCtrlAttribute (panMain, gCtrl_BOX[channel], ATTR_DIMMED, !my_struInstrument[channel].ChannelEn);
		//设置颜色
		SetCtrlAttribute (panMain, gCtrl_BOX[channel], ATTR_TEXT_BGCOLOR, COLOR_CHAN[channel]);
		
		//开关控件
		SetCtrlAttribute (panMain, gCtrl_BIN[channel], ATTR_DIMMED, !my_struInstrument[channel].ChannelEn);  
		
		//LED灯
		SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_DIMMED, !my_struInstrument[channel].ChannelEn);  
		SetCtrlAttribute (panMain, gCtrl_LED[channel], ATTR_OFF_COLOR, VAL_GRAY);
		SetCtrlVal (panMain, gCtrl_LED[channel], 0);
		
		ResetTextBox (panMain, gCtrl_BOX[channel], "");  
	}

	m_WLDac_normal = 0;             // 常温DAC和核温
	m_WLTemper_normal = 0.1;
	m_WLDac_High = 0;               //高温DAC和核温
	m_WLTemper_High = 0.1;
	m_Value_K = 0;
	m_Value_B = 0;
	if (ini_Flag)
	{
		progresshandle = CreateProgressDialog ("设备配置状态", "设备配置进度", 1, VAL_NO_INNER_MARKERS, ""); 
		
		dca_no_init = 1 ;
		spectrum_no_init = 1;
		sw_no_init=1;
		clock_no_init=1;
	
		for (channel=0;channel<CHANNEL_MAX;channel++) 
		{
			UpdateProgressDialog (progresshandle, (int)(100.*channel/CHANNEL_MAX), 0);
			
			if (my_struInstrument[channel].ChannelEn)   
			{
				//检查光纤使用情况
//				if (my_struConfig.ProductLine==PRODUCTLINE_CD && !my_struConfig.isNPI_flag)   
//				{
//					error = MyDLL_FiberManage_Init (hdbc, my_struInstrument[channel].Fiber);
//					if (error) 
//					{
//						goto Error;
//					}
//				}

				/******************initial USBtoI2C***********************************/ 
				if (my_struInstrument[channel].SEVB_TYPE==SEVB_TYPE_SEVB5 || my_struInstrument[channel].SEVB_TYPE==SEVB_TYPE_SEVB27)
				{
					error = EVB5_Init(&INST_EVB[channel], my_struInstrument[channel].SEVB);
					if(error<0) 
					{
						MessagePopup ("Error", "EVB5初始化失败！"); 
						goto Error;
					}
	
					//2009-02-18,添加SBERT初始化代码
					//2010-01-17,改为查询SBERT地址0x80是否存在，有SERT就初始化
					error = I2C_SLAVE_SEARCH_DLL(INST_EVB[channel], SBERT_Addr); 
					if (error==0)
					{
						error = EVB5_SBERT_Init(INST_EVB[channel], SBERT_R_R2488Mbps, SBERT_P_PRBS23);
						if (error)
						{
							MessagePopup("提示","EVB5设置BERT工作模式失败！");
							goto Error;
						} 
					}
		
					//设置BEN_mode
					if (EVB5_SET_BEN_Mode (INST_EVB[channel], EVB5_BEN_MODE_LEVEL)<0) 
					{
						MessagePopup ("Error", "EVB5 Set level mode error !");
						goto Error; 
					}   
	
					//设置BEN_level, 低电平发光
					if (EVB5_SET_BEN_Level(INST_EVB[channel], 0)<0)
					{
						MessagePopup ("Error", "EVB5 Set low level mode error !");
						goto Error;
					} 		
				}
				else 
				{
					MessagePopup("Error", "Can not find this EVB type");
				} 
				
			
				RegisterI2cCallBackFunction(INST_EVB[channel]);   
				mydev=my_struControl.Init_I2cAddr;
				SetSuperDev(mydev);
				mydev = GetSuperDev();

				//for evb light 
				if (my_struDBConfig_ERCompens.RSSIMODE>0 && my_struInstrument[channel].EVB_Light_TYPE == SEVB_TYPE_NONE) 
				{
			//		MessagePopup ("Error", "RSSI系列产品需要选择光源装置"); 
			//		goto Error;
					;
				} 
				else
				{
					if (my_struDBConfig_ERCompens.RSSIMODE>0)
					{
						if (my_struInstrument[channel].EVB_Light_TYPE==SEVB_TYPE_SEVB5) 
						{
							error = EVB5_Init(&INST_EVB_Light[channel], my_struInstrument[channel].EVB_Light); 
							if(error)
							{
								MessagePopup ("Error", "Initial EVB5 error !"); 
								goto Error;
							}   
			
							//设置BEN_mode
							if (EVB5_SET_BEN_Mode (INST_EVB_Light[channel], EVB5_BEN_MODE_LEVEL)<0) 
							{
								MessagePopup ("Error", "EVB5 Set level mode error !");
								goto Error; 
							}   
	
							//设置BEN_level, 低电平发光
							if (EVB5_SET_BEN_Level(INST_EVB_Light[channel], 1)<0)
							{
								MessagePopup ("Error", "EVB5 Set high level mode error !");
								goto Error; 
							} 	
				
							//2010-01-17 查询SBERT地址0x80是否存在，有SERT就初始化
							error = I2C_SLAVE_SEARCH_DLL(INST_EVB_Light[channel], SBERT_Addr); 
							if (error==0)
							{
								error = EVB5_SBERT_Init(INST_EVB_Light[channel], SBERT_R_R2488Mbps, SBERT_P_PRBS23);
								if (error)
								{
									MessagePopup("提示","EVB5_Light设置BERT工作模式失败！");
									goto Error;
								} 
							}
				
							//光源板默认开电
							if (EVB5_SET_SHDN_VTR(INST_EVB_Light[channel], 1)<0)
							{
								goto Error;  
							}
						}
						else if (my_struInstrument[channel].EVB_Light_TYPE==SEVB_TYPE_B2BERT)
						{
							;//使用B2BERT可以不用初始化，B2BERT的初始化在Init_BERT函数完成  
						}	
						else if(my_struInstrument[channel].EVB_Light_TYPE==SEVB_TYPE_NONE)
						{
							;
						}	
						else 
						{
							MessagePopup("Error", "Can not find this EVB light type");
						}  
					}
				}
				
				//Power off
				// 初始化断电，屏蔽掉***lwy——20170623***
				//if (SET_EVB_SHDN(channel, 0)<0)
				//{
				//	goto Error;
				//}
				
				/******************initial USBtoI2C***********************************/		 
				//for BERT
				//error = Init_BERT (channel, -1);
				//if (error) 
				//{
				//	goto Error;
				//}
				
				if(BERT_TYPE_NONE != my_struInstrument[channel].BERT_TYPE_10G)
				{
					//for 10GBERT
					error = Init_10GBERT (channel);
					if (error)
					{
						goto Error;
					}
				}
		
				//仪器初始化函数
				/*****************Optics power meter********************/
				if(my_struInstrument[channel].Power_TYPE == PowMeter_TYPE_PMSII)
				{
					Status = PMSII_Init_Port(&INST_PM[channel], my_struInstrument[channel].PMSII, 1550);
					if (Status<0) 
					{
						MessagePopup("Error", "Initial PMSII error!"); 
						goto Error;
					} 
				}
				else if(my_struInstrument[channel].Power_TYPE==PowerMeter_TYPE_PSS_OPM)
				{
				   error=PSS_init_port(&INST_PM[channel], my_struInstrument[channel].PMSII);
				   if(error<0)
				   { 
					   MessagePopup("Error","光功率计初始化异常！") ;
						goto Error;
				   }
				  
				   my_struDBConfig.TxWavelength=1550;   
				   error =PSS_Set_WaveLength(INST_PM[channel],channel+1, my_struDBConfig.TxWavelength);
				   if(error<0)
				   {
				     MessagePopup("Error","波长设置失败！");
					 goto Error;
				   }
				}
				else if(my_struInstrument[channel].Power_TYPE == PowMeter_TYPE_NONE)
				{
					;	
				}	
				else 
				{
					MessagePopup("Error", "Can not find this optics power meter  !");  
					goto Error;
				}
				/*****************Optics power meter********************/
				/**************************************ATT*******************************/ 
				//for ATT main
				if (my_struInstrument[channel].ATT_TYPE_MAIN == ATT_TYPE_HP8156A)  
				{
					Status = HP8156_Init(&INST_ATT[channel],  my_struInstrument[channel].SN_ATT_MAIN, 1310, 10);
					if (!Status)
					{
						MessagePopup("Error", "Initial HP8156A error!");  
						goto Error;
					} 
				}
				else if(my_struInstrument[channel].ATT_TYPE_MAIN == ATT_TYPE_OLA55M)
				{
					Status = OLA55M_Init(&INST_ATT[channel], my_struInstrument[channel].ATT_MAIN_COM, 1310, OLA55M_MODE_ATT);
					if (!Status) 
					{
						MessagePopup("Error", "Initial OLA55M error!"); 
						goto Error;
					} 
	
					Status = OLA55M_Set_ATTValue(INST_ATT_MAIN[channel], 10);
					if (!Status) 
					{
						MessagePopup("Error", "OLA55M set ATT value error!");  
						goto Error;
					}  
				}
				else if(my_struInstrument[channel].ATT_TYPE_MAIN==ATT_TYPE_JW8504)
				{  
					my_struDBConfig.RxWavelength=1310;  
					error=JW8504_Init_Port(&INST_ATT[channel],my_struInstrument[channel].ATT_MAIN_COM);
					if (error) {MessagePopup("Error", "Initial JW8504 error!");goto Error;} 
					error=JW8504_Set(INST_ATT[channel],channel,my_struDBConfig.RxWavelength,0);
					if (error) {MessagePopup("Error", "Initial JW8504 error!");goto Error;}
			   
					if(JW8504_enable(INST_ATT[channel],channel, 1)<0) return -1; 
				}
				else if(my_struInstrument[channel].ATT_TYPE_MAIN == ATT_TYPE_NONE)
				{
					;
				}
				else if (my_struInstrument[channel].ATT_TYPE_MAIN == ATT_TYPE_GVPM)  
				{
					error = GVPM_Init(&INST_ATT[channel], my_struInstrument[channel].SN_ATT_MAIN, 1550, 10);
					if (error) 
					{
						MessagePopup("Error", "Initial GVPM error!"); 
						goto Error;
					} 
				}	
				else 
				{
					MessagePopup("Error", "Can not find this Attenuator !"); 
					goto Error;
				}  
	
				//for ATT Ref
				if (my_struInstrument[channel].ATT_REF_TYPE == ATT_TYPE_HP8156A)  
				{
					Status = HP8156_Init(&INST_ATT_REF[channel],  my_struInstrument[channel].SN_ATT_REF, 1310, 10);
					if (!Status)
					{
						MessagePopup("Error", "Initial HP8156A error!"); 
						goto Error;
					} 
				}
				else if(my_struInstrument[channel].ATT_REF_TYPE == ATT_TYPE_OLA55M)
				{
					Status = OLA55M_Init(&INST_ATT_REF[channel], my_struInstrument[channel].ATT_REF_COM, 1310, OLA55M_MODE_ATT);
					if (!Status) 
					{
						MessagePopup("Error", "Initial OLA55M error!");  
						goto Error;
					} 
	
					Status = OLA55M_Set_ATTValue(INST_ATT_REF[channel], 10);
					if (!Status) 
					{
						MessagePopup("Error", "OLA55M set ATT value error!"); 
						goto Error;
					}  
				}
				else if(my_struInstrument[channel].ATT_REF_TYPE == ATT_TYPE_NONE)
				{
					;
				}
				else if (my_struInstrument[channel].ATT_REF_TYPE == ATT_TYPE_GVPM)  
				{
					error = GVPM_Init(&INST_ATT_REF[channel], my_struInstrument[channel].SN_ATT_REF, 1310, 10);
					if (error)
					{
						MessagePopup("Error", "Initial GVPM error!"); 
						goto Error;
					} 
			  	}	
				else 
				{
					MessagePopup("Error", "Can not find this Attenuator !"); 
					goto Error;
				}  
				/**************************************ATT*******************************/
				
				/**************************************DCA*******************************/ 
				if (my_struConfig.Sel_T_ER || my_struConfig.Sel_T_Eye) 
				{
					if ((my_struInstrument[channel].DCA_TYPE == DCA_TYPE_CSA8000) && dca_no_init) 
					{
						if (!CSA8000_Init(&inst_CSA8000, my_struInstrument[channel].DCA_Addr)) 
						{
							MessagePopup("Error", "Initial CSA8000 error!");
							goto Error;
						}  
				
						if (!CSA8000_GET_MainSN(inst_CSA8000, my_struCSA8000.MainSN))
						{
							MessagePopup("Error", "读取CSA8000主机序列号错误！");
							goto Error;
						} 
				
						if (!DB_GET_SPEC_CSA8000()) 
						{
							goto Error;
						}  
		
						if (!CSA8000_Set_O(inst_CSA8000, my_struCSA8000.Channel_O, my_struCSA8000.X_Scale, my_struCSA8000.X_Position, my_struCSA8000.Y_Scale, my_struCSA8000.Y_Position, my_struCSA8000.Y_Offset, my_struCSA8000.Wavelength, my_struCSA8000.Filter, my_struCSA8000.MaskName)) 
						{
							MessagePopup("Error", "CSA8000 Config for Optics Channel error!");
							goto Error;
						} 
						dca_no_init = 0;  
					}
					else if ((my_struInstrument[channel].DCA_TYPE == DCA_TYPE_Ag86100) && dca_no_init) 
					{
						if (!Ag86100_Init(&inst_Ag86100, my_struInstrument[channel].DCA_Addr,my_struAg86100.Channel_O)) 
						{
							MessagePopup("Error", "Initial Ag86100 error!");
							goto Error;
						}
				
						if (!Ag86100_GET_MainSN(inst_Ag86100, my_struAg86100.MainSN)) 
						{
							MessagePopup("Error", "读取Ag86100主机序列号错误！");
							goto Error;
						} 
		
						if (!DB_GET_SPEC_Ag86100())
						{
							goto Error;
						}  
			
						if (!Ag86100_SET_O(inst_Ag86100, my_struAg86100.ERFactor, my_struAg86100.X_Scale, my_struAg86100.X_Position, my_struAg86100.Y_Scale, my_struAg86100.Y_Offset, my_struAg86100.Filter, my_struAg86100.Wavelength, my_struAg86100.MaskName, my_struAg86100.MaskMargin, my_struAg86100.Channel_O)) 
						{
							MessagePopup("Error", "Initial Ag86100 error!");
							goto Error;
						}
			
						if (0 == strncmp (my_struAg86100.MainSN, "US", 2))
						{	
							if (!Ag86100_SET_FILTER_OFF(inst_Ag86100,my_struAg86100.Channel_O))
							{
								goto Error;
							}
						}
						dca_no_init = 0;  
					}
					else if ((my_struInstrument[channel].DCA_TYPE == DCA_TYPE_NONE) && dca_no_init) 
					{
						;
					}	
					else if(dca_no_init)
					{
						MessagePopup("Error", "Can not find this DAC!");
						goto Error;
					}
				}
				/**************************************DCA*******************************/
					
				//--------------------光谱-------------------//
				if ((my_struInstrument[channel].SPECTRUM_TYPE == SPECTRUM_TYPE_AG8614X) && spectrum_no_init)
				{
					if (!Ag86142_Init(&inst_Ag86142, my_struInstrument[channel].SN_Spectrum)) 
					{
						MessagePopup("Error", "Initial Spectrum error!");
						goto Error;
					} 
	
					if (!Ag86142_Config(LASER_TYPE_DFB, inst_Ag86142, my_struConfig.wave_start, my_struConfig.wave_stop, 0.1)) 
					{
						MessagePopup("Error", "Config Spectrum error!");
						goto Error;
					} 
					spectrum_no_init=0;  
				}
				else if ((my_struInstrument[channel].SPECTRUM_TYPE == SPECTRUM_TYPE_AQ633X) && spectrum_no_init)
				{
					error = AQ633X_Init (&Inst_AQ633X, my_struInstrument[channel].SN_Spectrum); 
					if (error) 
					{
						MessagePopup("Error", "Initial Spectrum aq8663x error!"); 
						goto Error;
					}
	
					error = AQ633X_Config (Inst_AQ633X, LASER_TYPE_DFB, my_struConfig.wave_center, my_struConfig.wave_span, 0.05); 
					if (error)
					{
						MessagePopup("Error", "Config Spectrum aq8663x error!");
						goto Error;
					} 
					spectrum_no_init=0;  
				}
				else if (my_struInstrument[channel].SPECTRUM_TYPE == SPECTRUM_TYPE_AQ637X && spectrum_no_init)
				{
					//GetCtrlVal (panel, PAN_INST_STR_SN_SPECTRUM, my_struInstrument[channel].SN_Spectrum);
					//char aaa[] = "SPECTRUM";  
					//my_struInstrument[channel].SN_Spectrum = aaa;
					error = AQ637X_Init (&Inst_AQ637X, my_struInstrument[channel].Spectrum_Addr); 
					//error = AQ637X_Init (&Inst_AQ637X, aaa); 
					if (error) 
					{
						MessagePopup("Error", "Initial Spectrum AQ637X error!"); 
						goto Error;
					}
//					if (my_struConfig.DT_OSA_WL_Test)
//					{
//						my_struDBConfig.PeakWavelengthMin[0]=struDBConfig_OSATEST.WaveLenth_Min;
//						my_struDBConfig.PeakWavelengthMax[0]=struDBConfig_OSATEST.WaveLenth_Min;
//					}
					error = AQ637X_Config (Inst_AQ637X, LASER_TYPE_DFB, my_struConfig.wave_center, 2, 0.03); 	  // my_struConfig.wave_span
					if (error)
					{
						MessagePopup("Error", "Config Spectrum AQ637X error!");
						goto Error;
					} 

					// (my_struDBConfig.PeakWavelengthMin[0]+my_struDBConfig.PeakWavelengthMax[0])/2.0 
					spectrum_no_init=0;
				}
				else if((my_struInstrument[channel].SPECTRUM_TYPE == SPECTRUM_TYPE_NONE) && spectrum_no_init)
				{
					;
				}
				else if(spectrum_no_init) 
				{
					MessagePopup("Error", "Can not find this spectrum type");
					goto Error;
				}  
				//--------------------光谱-------------------//
		
				// for sw
				if (((my_struInstrument[channel].SW_TYPE == SW_TYPE_FSW) || (SW_TYPE_COFFSW == my_struInstrument[channel].SW_TYPE) || (SW_TYPE_10G == my_struInstrument[channel].SW_TYPE) || (SW_TYPE_JHFSW == my_struInstrument[channel].SW_TYPE) || (SW_TYPE_COFFSW02 == my_struInstrument[channel].SW_TYPE)) && sw_no_init)
				{
					errChk(Fiber_SW_Init(my_struInstrument[channel].SW_TYPE, my_struInstrument[channel].SW, &my_struInstrument[channel].SW_Handle, my_struInstrument[channel].SW_SN));
				
					sw_no_init=0;
				}
				else if ((SW_TYPE_10G == my_struInstrument[channel].SW_TYPE) || (0 == sw_no_init))
				{
					my_struInstrument[channel].SW_Handle = my_struInstrument[channel-1].SW_Handle;
				}
				else if ((SW_TYPE_NONE == my_struInstrument[channel].SW_TYPE) || (0==sw_no_init))
				{
					;
				}
				else 
				{
					sprintf (Info, "通道%d 光开关型号配置错误    ", channel); 
					MessagePopup("Error", Info);
					error=-1;
					goto Error;
				} 
		
				// for clock 目前只支持一个clock测试板，可选8通道
				if (my_struInstrument[channel].CLOCK_TYPE == CLOCK_TYPE_SEVB0027_4001 && clock_no_init)
				{
					errChk(SEVB0027_4001_Init(&inst_SEVB0027_4001, my_struInstrument[channel].CLOCK));
				
					clock_no_init=0;
				}
				else if(CLOCK_TYPE_NONE == my_struInstrument[channel].CLOCK_TYPE || 0==clock_no_init)   /***没有时钟板，则不初始化**Eric.Yao**20130904***/
				{
					;
				}
				else 
				{
					sprintf (Info, "通道%d 时钟切换板型号配置错误    ", channel); 
					MessagePopup("Error", Info);
					error=-1;
					goto Error;
				} 
			
				// for fiber
				if (!my_struConfig.isNPI_flag)
				{
					error = MyDLL_FiberManage_Init (hdbc, my_struInstrument[channel].Fiber);
				    if (error) goto Error;
				}
				/**************************************BERT******************************/
			}
		}
	}
	HidePanel (panel);
	*error_channel = 0;
	channel = 0;
	if (ini_Flag) 
	{
		DiscardProgressDialog (progresshandle);
	}
	   
	strcpy (Info, "系统初始化：成功");
	Insert_Info(panMain, gCtrl_BOX[channel], Info); 
	
	return 0;
Error:
	
	if (ini_Flag) 
	{
		DiscardProgressDialog (progresshandle);
	}
		
	HidePanel (panel); 
	
	*error_channel = channel;
	
	sprintf (Info, "系统初始化：失败 通道%d出现异常", channel);
	Insert_Info(panMain, gCtrl_BOX[channel], Info); 

	return -1;
}

int	Init_BERT (int channel, int mode_B2BERT)
{
	int error;
	unsigned int 	BurstPeriod, DataLen, TxDelay, Trig_Pos, Trig_Width, SDChk_Width;
	unsigned short 	UserPatternLen, PreAmbleLen, OLTRst_Pos;
	unsigned char 	PreGuideLen, OLTRst_Width, SDChk_Pos;
	
	if (my_struConfig.Sel_R_Over || my_struConfig.Sel_R_Sens || my_struConfig.Sel_R_SDHys ||my_struConfig.Sel_T_AOP || my_struConfig.Sel_T_ER || my_struConfig.Sel_T_Eye)
	{
		if (my_struInstrument[channel].BERT_TYPE == BERT_TYPE_GBERT) 
		{
			if (!GBERT_Init_SFPTxDisable(&INST_BERT[channel], my_struInstrument[channel].SN_BERT, 0)) 
			{
				MessagePopup("Error", "Initial GBERT error!");
				goto Error;
			} 
		}
		else if (my_struInstrument[channel].BERT_TYPE == BERT_TYPE_SBERT) 
		{
			//注意返回值是int型，0表示OK
			error = EVB5_Init(&INST_BERT[channel], my_struInstrument[channel].SN_BERT); 
			if (error)
			{
				MessagePopup("Error", "SBERT初始化失败！");
				goto Error;
			} 
				
			if (strcmp(my_struInstrument[channel].SN_BERT, my_struInstrument[channel].SEVB)!=0) //SBERT和EVB5使用相同的测试板，则不用再次初始化
			{
				error = EVB5_SBERT_Init(INST_BERT[channel], SBERT_R_R2488Mbps, SBERT_P_PRBS23);
				if (error)
				{
					MessagePopup("提示","SBERT设置失败！");
					goto Error;
				} 
	
				//设置BEN_mode
				if (EVB5_SET_BEN_Mode (INST_BERT[channel], EVB5_BEN_MODE_LEVEL)<0) 
				{
					MessagePopup ("Error", "EVB5 Set level mode error !"); 
					goto Error;
				}   
	
				//设置BEN_level, 低电平发光
				if (EVB5_SET_BEN_Level(INST_BERT[channel], 1)<0)
				{
					MessagePopup ("Error", "EVB5 Set low level mode error !"); 
					goto Error; 
				} 
	
				//Power on
				if (EVB5_SET_SHDN_VTR(INST_BERT[channel], 1)<0)
				{
					goto Error;
				}
			}
		}
		else if(my_struInstrument[channel].BERT_TYPE == BERT_TYPE_NONE)
		{
			;
		}	
		else 
		{
			MessagePopup("Error", "Can not find this BERT  !");
			goto Error;
		} 
	}
	
	return 0;
Error:
	return -1;	
}

int	Init_10GBERT (int channel)
{
	int error;
	double rate;
	
	if (my_struConfig.Sel_R_Over || my_struConfig.Sel_R_Sens || my_struConfig.Sel_T_AOP || my_struConfig.Sel_T_ER || my_struConfig.Sel_T_Eye)
	{
		if (my_struInstrument[channel].BERT_TYPE_10G == BERT_TYPE_SBERT10000) 
		{
			//注意返回值是int型，0表示OK
		/*	error = evb16_init(my_struInstrument.SBERT_10G, &INST_BERT_10G[channel]); 
			if (error){MessagePopup("Error", "SBERT10000初始化失败！"); goto Error;} 
	
			error = evb16_SetBERTInit(INST_BERT_10G[channel], 0, 0, 0, 0, SBERT10G_PRBS31, SBERT10G_R103125Mbps, SBERT10G_INTCLK, SBERT10G_SignalIn_SMA);
			if (error) {MessagePopup("提示","SBERT10000设置失败！"); goto Error;}   */
			//注意返回值是int型，0表示OK
			error = evb16_init(my_struInstrument[channel].SN_10GBERT, &INST_BERT_10G[channel]); 
			if (error)
			{
				MessagePopup("Error", "SBERT10000初始化失败！"); 
				goto Error;
			} 
	
			rate = SEVB16_R9953Mbps * 1000.0;//Gbs 换算为Mbs
			rate /=16.0;
			error = evb16_SetRate(INST_BERT_10G[channel], rate);
			if (error)
			{
				MessagePopup("Error", "SBERT10000设置速率失败！"); 
				goto Error;
			}  
	
			error = evb16_SetBERTInit(INST_BERT_10G[channel], 0, 0, 0, 0, SBERT10G_PRBS31, SBERT10G_INTCLK, SBERT10G_SignalIn_SMA);        
		//	error = evb16_SetBERTInit(INST_BERT_10G[channel], 0, 0, 0, 0, SBERT10G_PRBS31, SBERT10G_R103125Mbps, SBERT10G_INTCLK, SBERT10G_SignalIn_SMA);
			if (error) 
			{
				MessagePopup("提示","SBERT10000设置失败！"); 
				goto Error;
			} 
		}	
		else if (my_struInstrument[channel].BERT_TYPE_10G == BERT_TYPE_GBERT10000) 
		{
			error = GBERT10000_Init (&INST_BERT_10G[channel], my_struInstrument[channel].SN_10GBERT, GBERT_R10312Mbps, GBERT_PRBS31);
			if (error) 
			{
				MessagePopup("Error", "Initial GBERT error!"); 
				goto Error;
			} 
		}
		else
		{
			MessagePopup("Error", "Can not find this BERT  !");
			goto Error;
		}
	}
	
	return 0;
Error:
	return -1;	
}

int	Init_10GBERT_Start (int channel)
{
	int error;
	double rate;
	
	if (my_struConfig.Sel_R_Over || my_struConfig.Sel_R_Sens || my_struConfig.Sel_T_AOP || my_struConfig.Sel_T_ER || my_struConfig.Sel_T_Eye)
	{
		if (my_struInstrument[channel].BERT_TYPE_10G == BERT_TYPE_SBERT10000) 
		{
			//注意返回值是int型，0表示OK
		/*	error = evb16_init(my_struInstrument.SBERT_10G, &INST_BERT_10G[channel]); 
			if (error){MessagePopup("Error", "SBERT10000初始化失败！"); goto Error;} 
	
			error = evb16_SetBERTInit(INST_BERT_10G[channel], 0, 0, 0, 0, SBERT10G_PRBS31, SBERT10G_R103125Mbps, SBERT10G_INTCLK, SBERT10G_SignalIn_SMA);
			if (error) {MessagePopup("提示","SBERT10000设置失败！"); goto Error;}   */
			//注意返回值是int型，0表示OK
			
			rate = SEVB16_R9953Mbps * 1000.0;//Gbs 换算为Mbs
			rate /=16.0;
			error = evb16_SetRate(INST_BERT_10G[channel], rate);
			if (error)
			{
				MessagePopup("Error", "SBERT10000设置速率失败！"); 
				goto Error;
			}  
	
			error = evb16_SetBERTInit(INST_BERT_10G[channel], 0, 0, 0, 0, SBERT10G_PRBS31, SBERT10G_INTCLK, SBERT10G_SignalIn_SMA);        
			if (error) 
			{
				MessagePopup("提示","SBERT10000设置失败！"); 
				goto Error;
			} 
		}	
		else if (my_struInstrument[channel].BERT_TYPE_10G == BERT_TYPE_GBERT10000) 
		{
			error = GBERT10000_Init (&INST_BERT_10G[channel], my_struInstrument[channel].SN_10GBERT, GBERT_R10312Mbps, GBERT_PRBS31);
			if (error) 
			{
				MessagePopup("Error", "Initial GBERT error!"); 
				goto Error;
			} 
		}
		else
		{
			MessagePopup("Error", "Can not find this BERT  !");
			goto Error;
		}
	}
	
	return 0;
Error:
	return -1;	
}

BOOL quittest(void)
{
	int error;
	int channel; 
	
	//恢复光纤使用情况， 必须在关闭数据库函数前使用
	if (hdbc>0 && my_struConfig.ProductLine==PRODUCTLINE_CD && !my_struConfig.isNPI_flag) 
	{
		for (channel=0; channel<CHANNEL_MAX;channel++)
		{
			if (my_struInstrument[channel].ChannelEn)
			{
				errChk(MyDLL_FiberManage_End (hdbc, my_struInstrument[channel].Fiber));
			}
		}
	}

	error = DB_Close();
	if(error) {MessagePopup ("Error", "Close database!"); goto Error;} 
	
	return TRUE;
Error:
	return FALSE;
}

BOOL InitModule(int channel, struct struTestData *data, struct struProcessLOG *prolog)
{
BYTE	device_addr, Rb_Addr, Rm_Addr, Rb, Rm, Fail; 	
int		error, count, slope, offset, eaDAC, eaDACmax, eaDACmin;
INT16U  DAC;
unsigned char mark;
char    FirmwareVer[50], str1[20], str2[20];
int 	TecDAC, TecDACmax, TecDACmin;    

	
	//等待开电后稳定
	Delay(1.0);
	
	//TxDis
	if (!SET_EVB_TxDis(channel, 0))
	{
		return -1;
	}

	//调测参数赋初值
	resetdata(channel, data, prolog);

	error = ENTRY_Password(INST_EVB[channel]);
	if (error)
	{
		return FALSE;     	
	}

	Delay(0.5); 
	
	//读取模块的firmware版本信息																	  
	if (GET_FirmVersion (INST_EVB[channel], FirmwareVer)<0) 			
	{
		MessagePopup ("提示", "Read FirmwareVer error !"); 
		return FALSE;
	}  
	
	Scan(FirmwareVer, "%s>%s[xt95]%s[xt95]%s", str1, FirmwareVer, str2);  
	
	if (strcmp (FirmwareVer, my_struDBConfig.FirmwareVer)!=0) 	
	{
		MessagePopup ("提示", "Check FirmwareVer error!"); 
		return FALSE;
	} 
	
	sscanf(str1, "SFRM%d", &data->ProjectNumber);  /***获取固件项目号**Eric.Yao**20131012***/
	
	//写入Biase校准系数(区分2uA/4uA)
//	error = XGPON_OLT_SET_BIAS_Unit_Ex(INST_EVB[channel],my_struConfig.Bias_unit);									  //需改到配对程序
//	if (error)
//	{
//		return FALSE;
//	}
	
//	error = Update_BASE (INST_EVB[channel], 0);
//	error = XGPON_OLT_SET_LOS_DAC(INST_EVB[channel]);
	if (error)
	{
		return FALSE;
	}   
//------------------模块写密码------------------------//

	/*********************Resultstable变量赋值**************************/
	strcpy (data->PN, my_struConfig.PN);     	//PN 
	strcpy (data->Ver, my_struConfig.Ver);  	//Version
	MyDLL_GetDateTime (data->TestDate);		//testdate 
	//ModuleSN， 在测试函数赋值
	//OpticsSN, 在测试函数赋值
	//测试模块温度，在testteperature后赋值
	//环境温度
	if     (my_struConfig.Temper_Low) {data->AmbientTemperatrue=0;}
	else if(my_struConfig.Temper_Room){data->AmbientTemperatrue=25;} 
	else if(my_struConfig.Temper_High){data->AmbientTemperatrue=70;} 
	else							  {data->AmbientTemperatrue=999;}
	/*********************Resultstable变量赋值*********************/
	
	/*********************Process table 变量**************************/ 
	strcpy (prolog->PN, my_struConfig.PN);     		//PN 
	strcpy (prolog->SN, data->ModuleSN);	//SN
	//Operator  在 run函数已经赋值
	//ResultsID 在 SaveDB_Results_ATE函数后确定
	//Log_Action在 调测功能函数已经赋值
	MyDLL_GetDateTime (my_struProcessLOG.Action_Time);			//Action_time  
	strcpy (my_struProcessLOG.Parameter, "Status");				//Parameter
	//P_Value  在调测结束后赋值
	/*********************结束 Process table 变量*********************/ 

	//set att
	Status = Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, -10);
	if (Status) 
	{
		MessagePopup("提示", "Set Att error!");
	}

	//2010-01-17 光源板添加查询SBERT地址0x80是否存在，有SERT就初始化，用于光源板和测试板不是同一个设备时，用光源板做信号源的情况  
	if (my_struInstrument[channel].EVB_Light_TYPE==SEVB_TYPE_SEVB5) 
	{
		error = I2C_SLAVE_SEARCH_DLL(INST_EVB_Light[channel], SBERT_Addr); 
		if (error==0)
		{
			error = EVB5_SBERT_Init(INST_EVB_Light[channel], SBERT_R_R1244Mbps, SBERT_P_PRBS23);
			if (error)
			{
				MessagePopup("提示","EVB5_Light设置BERT工作模式失败！"); 
				return FALSE;
			} 
		}
	}
	
	return TRUE;
}

BOOL testTemperature(int panel, int channel, struct struTestData *data)
{
	int    count;
	float  temp;
	char   Info[500];
	int    maxCnt = 30;
	
	count = 0;
	if (!Read_Temperature(channel, &temp)) 
	{
		return FALSE; 
	}

	if (!my_struConfig.Temper_Low)  
	{
		do
		{
			if(temp>my_struDBConfig.TemperMax)   /***温度超过上限，则直接退出**Eric.Yao**20131119***/
			{
				MessagePopup("Error", "温度超过上限");
				return FALSE;
			}
			else if (temp > my_struDBConfig.TemperMin && temp < my_struDBConfig.TemperMax)
			{
				break;
			}
			else	/***温度低于下限，则循环等待升温到指标范围或者超时**Eric.Yao**20131119***/
			{
				Delay(1);  
				count++;
			}
	
			if (!Read_Temperature(channel, &temp)) 
			{
				return FALSE;   
			}
		
			sprintf (Info, "测试模块温度=%.2f℃ ", temp);     
			Insert_Info(panel, gCtrl_BOX[channel], Info);
		
		//	Delay(1.0);
		//	count++;
			ProcessDrawEvents ();
		}while (count < maxCnt);		
 
	}
	
	if (!Read_Temperature(channel, &data->Temperatrue)) 
	{
		return FALSE;
	}
	
	//判断范围
	if (data->Temperatrue>my_struDBConfig.TemperMax || data->Temperatrue<my_struDBConfig.TemperMin)
	{
		return FALSE;
	}
	
	return TRUE;
}
BOOL testSpectrum1(int channel, struct struTestData *data)
{
	int count, error;
	
	count=0;
	do
	{
		if (my_struInstrument[channel].SPECTRUM_TYPE == SPECTRUM_TYPE_AG8614X) 
		{
			if (!Ag86142_Read(LASER_TYPE_DFB, inst_Ag86142, my_struConfig.wave_span, &data->PeakWL, &data->Sigma, &data->Bandwidth, &data->SMSR)) return FALSE;
		}
		else if (my_struInstrument[channel].SPECTRUM_TYPE == SPECTRUM_TYPE_AQ633X)
		{
			error = AQ633X_Read (Inst_AQ633X, LASER_TYPE_DFB, my_struConfig.wave_span, &data->PeakWL, &data->Sigma, &data->Bandwidth, &data->SMSR);
			if (error) {MessagePopup("Error", "Read spectrum aq633x error");return FALSE;}
		}
		else {MessagePopup("Error", "Can not find this spectrum type");return FALSE;} 
		
		//------------判断范围---------------//
		//forDFB	
	//	if (my_struTestData.PeakWL<=my_struDBConfig.PeakWavelengthMax && my_struTestData.PeakWL>=my_struDBConfig.PeakWavelengthMin && my_struTestData.Bandwidth<=my_struDBConfig.BandWidth && my_struTestData.SMSR>=my_struDBConfig.SMSR) break;
		if (data->PeakWL<=my_struDBConfig.PeakWavelengthMax01 && data->PeakWL>=my_struDBConfig.PeakWavelengthMin01)	break;
		//------------判断范围---------------// 
		count++;
	} while(count<3);	
	
	if(count>=3) return FALSE; 	
	
	return TRUE;
}
int SaveFlash_Dwdm(int channel)
{
	char strOupt[256];
	char strInpt[256];
	// 保存查找表
	memset(strInpt, 0, 256);
	memset(strOupt, 0, 256);
	sprintf(strInpt, "gn2010_save_reg()");  
	SetCommand(INST_EVB[channel],strInpt, strOupt);
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "保存失败!     "); 
		return -1;
	}
	memset(strInpt, 0, 256);
	memset(strOupt, 0, 256);
	sprintf(strInpt, "mcu_update_flash(driver,0)");  
	SetCommand(INST_EVB[channel],strInpt, strOupt);
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "保存失败!     "); 
		return -1;
	}
	memset(strInpt, 0, 256);
	memset(strOupt, 0, 256);
	sprintf(strInpt, "mcu_update_flash(base,0)");  
	SetCommand(INST_EVB[channel],strInpt, strOupt);
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "保存失败!     "); 
		return -1;
	}
	memset(strInpt, 0, 256);
	memset(strOupt, 0, 256);
	sprintf(strInpt, "mcu_update_flash(base,1)");  
	SetCommand(INST_EVB[channel],strInpt, strOupt);
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "保存失败!     "); 
		return -1;
	}
	memset(strInpt, 0, 256);
	memset(strOupt, 0, 256);
	sprintf(strInpt, "mcu_update_flash(luk,0)");  
	SetCommand(INST_EVB[channel],strInpt, strOupt);
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "保存失败!     "); 
		return -1;
	}
	memset(strInpt, 0, 256);
	memset(strOupt, 0, 256);
	sprintf(strInpt, "mcu_update_flash(luk,1)");  
	SetCommand(INST_EVB[channel],strInpt, strOupt);
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "保存失败!     "); 
		return -1;
	}
	return 0;
}
int Get_normal_DAC_DWDM(int channel)
{
	char strOupt[256];
	char strInpt[256];
	char strModel[10]; 
	int paraTmp2 = 0;
	int DACtemp = 0;
	// 读取常温DAC 值
	memset(strInpt, 0, 256);
	memset(strOupt, 0, 256);
	memset(strModel, 0, 10);
	sprintf(strInpt, "mcu_get_adjust(0)");  
	SetCommand(INST_EVB[channel],strInpt, strOupt);
	sscanf(strOupt,"%c, 0x%x, 0x%x", strModel, &paraTmp2, &DACtemp);
	m_WLDac_normal =  DACtemp;
	
	return DACtemp;
}
int TestWL_TemperHigh(int channel, struct struTestData *data, int panel, int control, char *Info, char *ERInfo)
{
	int error = 0;
	float CurrentTemper = 0;
	double CoreNormalTemper = 0;
	int valueK = 0;
	int valueB = 0;
	int getDac = 0;
	int paraTmp1 = 0;
	int paraTmp2 = 0;
	char strOupt[256];
	char strInpt[256];
	char strModel[10];
	BOOL Status_WL = FALSE;
	float HighTemper;
	// 检查是否到达指定温度
	error = MyDLL_8472_GetTemperatrue (INST_EVB[channel], &HighTemper);
	while (HighTemper < 65)
	{
		MessagePopup("提示", "温度条件不满足!");
		Delay(30);
	}

	// 调试高温波长和获取高温DAC 和核温
	Status_WL = testSpectrum(channel, data);
	
	Status_WL = tunSpectrum_WL_High_DWDM(channel,data,m_WLDac_normal);
	//获取当前高温
	ReadNormalTemper(channel, data);
	// 用高温常温两点数据计算k和b
	// 获取常温温度(从数据库读)
	GetDBNormalCoreTemper(&CoreNormalTemper, data->OpticsSN);
	
	valueK = ((m_WLDac_High - m_WLDac_normal)/ (m_WLTemper_High - CoreNormalTemper) * 512);
	valueB = ((m_WLDac_normal * m_WLTemper_High - m_WLDac_High * CoreNormalTemper) / (m_WLTemper_High - CoreNormalTemper));
	// 写入查找表数据
	memset(strInpt, 0, 256);
	memset(strOupt, 0, 256);
	sprintf(strInpt, "mcu_set_table(LUK,1,0x00,0xFF,0x00, 0x%x, 0x%x, 0x%x, 0x%x)", (valueK & 0xFF), ((valueK>>8) & 0xFF),(valueB & 0xFF), ((valueB>>8) & 0xFF));  
	SetCommand(INST_EVB[channel],strInpt, strOupt);
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "写TEC查找表失败!     "); 
		return -1;
	}
	// 保存查找表
	memset(strInpt, 0, 256);
	memset(strOupt, 0, 256);
	sprintf(strInpt, "mcu_update_flash(luk,1)");  
	SetCommand(INST_EVB[channel],strInpt, strOupt);
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "保存查找表失败!     "); 
		return -1;
	}
	// 切回查找表模式
	memset(strInpt, 0, 256);
	memset(strOupt, 0, 256);
	// 核对密码
	error = I2C_4BYTEs_WRITE_DLL (INST_EVB[channel], 0xA2, 0x7B, 0x53, 0x4F, 0x45, 0x42, 0.1);//写0级密码
	sprintf(strInpt, "mcu_set_adjust(0,a,0x00,0xF900,0x1F00,0x850)");  
	SetCommand(INST_EVB[channel],strInpt, strOupt);
	if (error<0) {MessagePopup ("ERROR", "NO Acknowledge from target !");return FALSE;}
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "切回查找表模式失败!     "); 
		return FALSE;
	}
	return 0;
	
}

// 获取常高 温下的核温
BOOL ReadNormalTemper(int channel, struct struTestData *data)
{   
	int error = 0;
	int value_K = 0;
	int value_B = 0;
	char valueTmp[10];
	char strOupt[256];
	char strInpt[256];
	float NorTemper;
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	memset(valueTmp, 0, 10);
	sprintf(strInpt, "mcu_get_mon_info(temperature)");  
	SetCommand(INST_EVB[channel],strInpt, strOupt);
	sscanf(strOupt,"%c, 0x%x,0x%x", valueTmp, &value_K, &value_B); 
	m_Value_K = value_K;
	m_Value_B = value_B;
	// 设置T模式
	memset(strInpt, 0, 256);
	memset(strOupt, 0, 256);
	sprintf(strInpt, "mcu_set_mon_info(temperature,T,0x8000,0x0)");  
	SetCommand(INST_EVB[channel],strInpt, strOupt);
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "T模式设置失败!     "); 
		return FALSE;
	}
	// 读常温核温
	error = MyDLL_8472_GetTemperatrue (INST_EVB[channel], &NorTemper); 
	if (error)
	{
		return FALSE;
	} 
	
	if (my_struConfig.Temper_Room)
	{
		m_WLTemper_normal = NorTemper;
	}
	if (my_struConfig.Temper_High)
	{
		m_WLTemper_High = NorTemper;
	}
	data->CoreTemper = NorTemper;
	// 设置回F模式
	memset(strInpt, 0, 256);
	memset(strOupt, 0, 256);
	sprintf(strInpt, "mcu_set_mon_info(temperature,F,%d,%d)", m_Value_K, m_Value_B);  
	SetCommand(INST_EVB[channel],strInpt, strOupt);
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "T模式设置失败!     "); 
		return FALSE;
	}
	
	return TRUE;
}

BOOL testSpectrum(int channel, struct struTestData *data)
{
int count, error;
	
	count=0;
	do
	{
		if (my_struInstrument[channel].SPECTRUM_TYPE == SPECTRUM_TYPE_AG8614X) 
		{
			if (!Ag86142_Read(LASER_TYPE_DFB, inst_Ag86142, my_struConfig.wave_span, &data->PeakWL, &data->Sigma, &data->Bandwidth, &data->SMSR)) return FALSE;
		}
		else if (my_struInstrument[channel].SPECTRUM_TYPE == SPECTRUM_TYPE_AQ633X)
		{
			error = AQ633X_Read (Inst_AQ633X, LASER_TYPE_DFB, my_struConfig.wave_span, &data->PeakWL, &data->Sigma, &data->Bandwidth, &data->SMSR);
			if (error) {MessagePopup("Error", "Read spectrum aq633x error");return FALSE;}
		}
		else if (my_struInstrument[channel].SPECTRUM_TYPE == SPECTRUM_TYPE_AQ637X)
		{
			error = AQ637X_Read(Inst_AQ637X, LASER_TYPE_DFB, my_struConfig.wave_span, &data->PeakWL, &data->Sigma, &data->Bandwidth, &data->SMSR);
			if (error) {MessagePopup("Error", "Read spectrum aq637x error");return FALSE;}
		}
		else {MessagePopup("Error", "Can not find this spectrum type");return FALSE;} 
		
		//------------判断范围---------------//
		//forDFB	
	//	if (my_struTestData.PeakWL<=my_struDBConfig.PeakWavelengthMax && my_struTestData.PeakWL>=my_struDBConfig.PeakWavelengthMin && my_struTestData.Bandwidth<=my_struDBConfig.BandWidth && my_struTestData.SMSR>=my_struDBConfig.SMSR) break;
		if (data->PeakWL<=my_struDBConfig.PeakWavelengthMax && data->PeakWL>=my_struDBConfig.PeakWavelengthMin)	break;
		//------------判断范围---------------// 
		count++;
	} while(count<3);	
	
	if(count>=3) return FALSE; 	
	
	return TRUE;
}

BOOL tunSpectrum_WL_High_DWDM(int channel, struct struTestData *data, int tekVlue)
{
	BOOL Status_WL = FALSE;
	int count = 0;
	BOOL StatusTmp = FALSE;
	while (data->PeakWL >= my_struDBConfig.PeakWavelengthMax || data->PeakWL <= my_struDBConfig.PeakWavelengthMin)
	{
		if (data->PeakWL >= my_struDBConfig.PeakWavelengthMax)
		{
			tekVlue += 0x08;
		}
		if (data->PeakWL <= my_struDBConfig.PeakWavelengthMin)
		{
			tekVlue -= 0x08;
		}
		SET_10G_DWDM(INST_EVB[channel], tekVlue);
		Delay(0.5);
		Status_WL = testSpectrum(channel, data);
		count++;
		// 加一个循环次数限制，防止死循环
		if (Status_WL == TRUE || count > 70) break;
	}
	StatusTmp = (data->PeakWL <= my_struDBConfig.PeakWavelengthMax && data->PeakWL >= my_struDBConfig.PeakWavelengthMin);
	if (StatusTmp && my_struConfig.Temper_Room)
	{
		m_WLDac_normal = tekVlue;
		Status_WL = TRUE;	
	}
	if (StatusTmp && my_struConfig.Temper_High)
	{
		m_WLDac_High = tekVlue;
		Status_WL = TRUE;
	}
	return Status_WL;
}

int DacAdjustSpeed(double targetWL)
{
	int targetDac = 0;
	double valueK = 0;
	double valueB = 0;
	valueK = (m_adjustTemper[1] - m_adjustTemper[0])/(m_adjustDac[1] - m_adjustDac[0]);
	valueB = m_adjustTemper[1] - valueK * m_adjustDac[1];
	targetDac = (targetWL - valueB)/valueK;
	return targetDac;
}
BOOL tunSpectrum_WL_DWDM(int channel, struct struTestData *data, int *tekVlue)
{
	BOOL Status_WL = FALSE;
	int count = 0;
	BOOL StatusTmp = FALSE;
	double TargetWL = 0;
	while (data->PeakWL >= my_struDBConfig.PeakWavelengthMax || data->PeakWL <= my_struDBConfig.PeakWavelengthMin)
	{
		if (data->PeakWL >= my_struDBConfig.PeakWavelengthMax)
		{
			*tekVlue += 0x08;
		}
		if (data->PeakWL <= my_struDBConfig.PeakWavelengthMin)
		{
			*tekVlue -= 0x08;
		}
		SET_10G_DWDM(INST_EVB[channel], *tekVlue);
		Delay(0.5);
		Status_WL = testSpectrum(channel, data);
		if (count < 2 && m_adjustSpeed == FALSE)
		{
			m_adjustDac[count] = (*tekVlue);
			m_adjustTemper[count] = data->PeakWL;
		}
		if (count == 1 && m_adjustSpeed == FALSE)
		{
			TargetWL = (my_struDBConfig.PeakWavelengthMax + my_struDBConfig.PeakWavelengthMin)/2;
			*tekVlue = DacAdjustSpeed(TargetWL);
			SET_10G_DWDM(INST_EVB[channel], *tekVlue);
			Delay(0.5);
			Status_WL = testSpectrum(channel, data);
			m_adjustSpeed = TRUE;
		}
		count++;
		// 加一个循环次数限制，防止死循环
		if (Status_WL == TRUE || count > 70) break;
	}
	StatusTmp = (data->PeakWL <= my_struDBConfig.PeakWavelengthMax && data->PeakWL >= my_struDBConfig.PeakWavelengthMin);
	if (StatusTmp && my_struConfig.Temper_Room)
	{
		m_WLDac_normal = *tekVlue;
		Status_WL = TRUE;	
		SET_10G_DWDM(INST_EVB[channel], *tekVlue);
	}
	//if (StatusTmp && my_struConfig.Temper_High)
	//{
	//	m_WLDac_High = tekVlue;
	//	Status_WL = TRUE;
	//}
	return Status_WL;
}
// 从EVB5的内存中读取温度值
BOOL GetTemper_DWDM(short *temperVlue, int channel)
{
	BYTE bigend = 0;
	BYTE littleend = 0;
	int error = 0;
	short temperTmp = 0;
	BYTE Temp_arr[256];

	error = I2C_BYTE_WRITE_DLL (INST_EVB[channel], my_struControl.GetSN_I2cAddr, 0x7F, my_struControl.GetSN_Table, 0.1); 
    if (error<0) 
	{
		return FALSE; 
	}
	
	error = I2C_BYTEs_READ_DLL (INST_EVB[channel], my_struControl.GetSN_I2cAddr, 128, 128, Temp_arr);
	if (error<0)
	{
		return FALSE;
	}

	bigend = Temp_arr[240];
	littleend = Temp_arr[241];
	temperTmp = (littleend & 0xFFFF);
	temperTmp = ((bigend<<8) & temperTmp);
	*temperVlue = temperTmp;
	return TRUE;
}
BOOL GetBosaSN(int channel, struct struTestData *data)
{
	int	i, error;
	BYTE Temp_arr[256];
	char substr, str[25]="", myBareBOM[50];
	char batch[50] = "";
	

	error = I2C_BYTE_WRITE_DLL (INST_EVB[channel], my_struControl.GetSN_I2cAddr, 0x7F, my_struControl.GetSN_Table, 0.1); 
    if (error<0) 
	{
		return FALSE; 
	}
	
	error = I2C_BYTEs_READ_DLL (INST_EVB[channel], my_struControl.GetSN_I2cAddr, 128, 128, Temp_arr);
	if (error<0)
	{
		return FALSE;
	}

	for(i=0; i<16; i++)
	{
		substr=(Temp_arr[240+i]);
		if(((substr>='A') && (substr<='Z')) || ((substr>='0') && (substr<='9')))
		{
			str[i]=substr;
		}
	}
	
	//必须先赋值, 避免my_struTestData的数据问题
	strcpy (data->OpticsSN, str);

	if ( strlen(str)==0)
	{
		return FALSE;
	}
	
	if(!my_struConfig.isNPI_flag)
	{
		//090312 QA测试情况下部分模块没有通过前面的工序，不能进行检查
		if (DB_Search_Tracking_BareBOM (data->OpticsSN, myBareBOM, batch)<0) 
		{
			return FALSE;
		}
	
		if (strcmp (myBareBOM, my_struConfig.Ver) != 0)
		{
			MessagePopup ("提示", "产品料号检查失败");
			return FALSE;
		}  

		if ((0 != stricmp (batch, my_struConfig.batch)) && !my_struConfig.isNPI_flag)
		{
			MessagePopup ("提示", "调测批次检查失败"); 
			return FALSE;
		}  
	}

	return TRUE;
}

int get_module_sn (int channel, struct struTestData *data)
{
	BYTE Temp_arr[256]; 
	char tempsn[30];
	int  index, error;
	
	error = I2C_BYTEs_READ_DLL (INST_EVB[channel], 0xA0, 0, 256, Temp_arr);
	if (error<0) return -1;
	
	memset (tempsn, 0x20, 30);
	for (index=0; index<16; index++)
	{tempsn[index]=(Temp_arr[index+68]==0x20) ? 0:(Temp_arr[index+68]);}
	tempsn[16]=0;	//最后一个字节添加0，用于结束字符串
	
	strcpy (data->ModuleSN, tempsn);

	return 0;
}

BOOL CheckModuleSN(int panel, int channel, struct struTestData *data)
{
int index, status;

char prestr[30], preserial[20], laststr[30], lastserial[20], str[30], serial[20]; 
int  prenum, lastnum, num; 
	
	//080420更新，先读取界面上的SN，如果界面上的SN为空则弹出对话框
	//080419 更新，通过对话框输入SN 
	GetCtrlVal (panel, PAN_MAIN_STR_SN, data->ModuleSN);
	
	if (strcmp(data->ModuleSN, "")==0)
	{
		status = PromptPopup ("提示", "请输入模块序列码", data->ModuleSN, 30);
		if (status<0)
		{
			return FALSE;
		}
	}
/*
	if (MyDLLReadModuleSN (INST_EVB[channel], data->ModuleSN)<0)
	{
		return FALSE;      	
	}
*/	
	//将SN转换为大写，并将不合理的字符去掉
	MyDLLCheckSN (data->ModuleSN);
	
	index=strlen (my_struDBConfig.BarCode);
	for (index=0;index<strlen (my_struDBConfig.BarCode); index++)
	{
		if (my_struDBConfig.BarCode[index]!=data->ModuleSN[index]) 
		{
			return FALSE;
		}
	}

	if (my_struDBConfig.SNLength != strlen (data->ModuleSN))
	{
		MessagePopup ("提示", "ModuleSN长度检查失败");
		return FALSE;
	} 
	
	if (my_struConfig.check_sn && !my_struConfig.isNPI_flag) 	  /***增加根据批次检查模块SN范围**Eric.Yao**20130819***/
	{
		Scan(my_struConfig.firstsn, "%s>%s[w9]%s", prestr, preserial);     
		Scan(preserial, "%s>%i", &prenum);
	
		Scan(my_struConfig.lastsn, "%s>%s[w9]%s", laststr, lastserial);     
		Scan(lastserial, "%s>%i", &lastnum);
	
		Scan(data->ModuleSN, "%s>%s[w9]%s", str, serial);     
		Scan(serial, "%s>%i", &num);
				
		if (0!= strcmp(prestr, str) || 0!= strcmp(laststr, str))
		{
			MessagePopup ("提示", "序列号前9位与sgd_scdd_trx表里的不一致"); 
			return FALSE; 
		}  	
	
		if (num < prenum || num > lastnum)
		{
			MessagePopup ("提示", "序列号不在sgd_scdd_trx表里的序列号范围内"); 
			return FALSE; 
		} 
	}
	
	return TRUE;
}

int ReadModuleSN (int channel, struct struTestData *data)
{
int  index, error, i;
BYTE Temp_arr[256];
char substr, str[25]="";

	error = I2C_BYTEs_READ_DLL (INST_EVB[channel], 0xA0, 0, 256, Temp_arr);
	if (error<0) return -1;
 
	for(i=0; i<16; i++)
	{
		substr=(Temp_arr[128+68+i]);
		if(((substr>='A') && (substr<='Z')) || ((substr>='0') && (substr<='9'))) 
		{
			str[i]=substr;
		}
	}
	//必须先赋值, 避免my_struTestData的数据问题
	strcpy (data->ModuleSN, str);
	//将SN转换为大写，并将不合理的字符去掉
	MyDLLCheckSN (data->ModuleSN);
	
	index=strlen (my_struDBConfig.BarCode);
	for (index=0;index<strlen (my_struDBConfig.BarCode); index++)
	{
		if (my_struDBConfig.BarCode[index]!=data->ModuleSN[index]) 
		{
			return -1;
		}
	}

	if (my_struDBConfig.SNLength != strlen (data->ModuleSN))
	{
		MessagePopup ("提示", "ModuleSN长度检查失败");
		return -1;
	} 
	
	return 0;
}

BOOL testAOP10G(int channel, struct struTestData *data)
{
	double Aop_target, aop_min, aop_max;
	int count , forward, backward, DACMax, DACMin, setDAC, DAC_target, DAC, temp_DAC, temp_DACmax, temp_DACmin;
	int ltDAC, ltDACmax, ltDACmin, eaDAC, eaDACmax, eaDACmin, step;
	int	ModDAC,ModDACmin,ModDACmax;
	BOOL tun2;
	char 	buf[256]; 
	double	biasMax = 0;  
	float	ldv_coretemper[3]={6,45,98};
	float 	ldv_10G_APC_DAC[3]={0}; 
	int		error;
	float   temper;
	float   txbias;
	
	int 		tempture_arry[3];
	float		temp_slop[3],temp_offset[3];

	biasMax=my_struDBConfig.TxBiasMax; 
	tun2 = FALSE;
	count=0;
	do
	{
		if (!Read_AOP(channel, &data->AOP)) 
		{
			return FALSE;
		}
		
		//判断光功率
		if (data->AOP<=my_struDBConfig.TxPowMax && data->AOP>=my_struDBConfig.TxPowMin) 
		{
			break;
		}
		count++;
		Delay(1);
	}while(count<3);
	
	//记录Biase
	error = GET_10G_Bias(INST_EVB[channel], &txbias, my_struConfig.Bias_unit);
	if (error)
	{
		return FALSE;   
	}
	data->A2_Ibias=txbias;
	
	//判断光功率
	if (data->AOP>my_struDBConfig.TxPowMax || data->AOP<my_struDBConfig.TxPowMin || txbias > biasMax)
	{
		if((my_struConfig.Temper_High || my_struConfig.Temper_Low) && my_struConfig.DT_Test_DWDM)     
		{
			if(txbias > biasMax)
			{
				
				error = GET_10G_APC(INST_EVB[channel], &setDAC);			 //Get APC_DAC
				if (error)
				{
					return FALSE; 
				}	
				
				error = SET_10G_APC_Mode(INST_EVB[channel], LUT_MODE_MANUAL);
				if (error)
				{
					return FALSE;
				}
				do
				{
					//读Ibias
					error = GET_10G_Bias(INST_EVB[channel], &txbias, my_struConfig.Bias_unit);
					if (error)
					{
						return FALSE;   
					}
				
					data->A2_Ibias=txbias;  
				
					if (txbias > biasMax)  
					{   
						setDAC = setDAC -5;
					} 
			
					error = SET_10G_APC(INST_EVB[channel], setDAC);			 //写APC_DAC
					if (error)
					{
						return FALSE; 
					}

				}while(txbias > biasMax);  
			
		 	
		 
				//读取光功率
				if (!Read_AOP(channel, &data->AOP))
				{
					return FALSE;  
				} 
				
				if (data->AOP>my_struDBConfig.TxPowMax || data->AOP<my_struDBConfig.TxPowMin)
				{
					return FALSE;  	
				}
			}
			else
			{
				//高低温微调AOP
				sscanf (my_struDBConfig_ERCompens.RB_ADD_10G, "(0x%03x)(0x%03x)(0x%03x)", &temp_DAC, &temp_DACmin, &temp_DACmax);
	
		//		Aop_target = (my_struDBConfig.TxPowMax + my_struDBConfig.TxPowMin) / 2;  
				Aop_target = (my_struDBConfig.TxPowMin + 0.5);
				aop_min = Aop_target - 0.2;
				aop_max = Aop_target + 0.2;

				//切换模式---手动模式
				error = SET_10G_APC_Mode(INST_EVB[channel], LUT_MODE_MANUAL);
				if (error)
				{
					return FALSE;
				}
	
	//			sscanf (my_struDBConfig_ERCompens.RB_ADD_10G, "(0x%03x)(0x%03x)(0x%03x)", &ModDAC, &ModDACmin, &ModDACmax);   
	//			if(SET_10G_MOD(INST_EVB[channel], ModDAC)<0)			 //写MOD_DAC初值
	//			{
	//				return FALSE; 
	//			}
			
				DACMax=temp_DACmax;  
				DACMin=temp_DACmin;     
				setDAC=temp_DAC;
		  
				error = SET_10G_APC(INST_EVB[channel], setDAC);			 //写APC_DAC
				if (error)
				{
					return FALSE; 
				}
		
				Delay(2);
	
				Status = Read_AOP(channel, &data->AOP);
				if (!Status)
				{
					return FALSE;  
				}
			
				count=0;     
				//set AOP
				while(!tun2)		                
				{	 
					if ((data->AOP>=aop_min) && (data->AOP<=aop_max))	
					{
						break;
					}
					else if((DACMax-DACMin)<2) 				  //减到不能减即退出
					{
						tun2 = TRUE;
						break;
					} 
					else
					{
						if(data->AOP>Aop_target) 
						{
							DACMax=setDAC;
							setDAC=(DACMax+DACMin)/2;
						}
						else					 
						{
							DACMin=setDAC;
							setDAC=(DACMax+DACMin)/2;
						}
					}
		
					error = SET_10G_APC(INST_EVB[channel], setDAC);			 //写APC_DAC
					if (error)
					{
						return FALSE; 
					}
		
					Delay(1.5);   //1.5s
					Status = Read_AOP(channel, &data->AOP);
					if (!Status)
					{
						return FALSE;  
					}
				}
	
				/**再次调试**/
				if (tun2)
				{ 
					Status = Read_AOP(channel, &data->AOP);
					if (!Status)
					{
						return FALSE;  
					}
		
					step = 0x10;
					count = 0;
					setDAC = temp_DAC;
					do
					{
						if (data->AOP < aop_min)
						{
							setDAC = setDAC + step;	
							if(setDAC>=0xFF)
							{
								if (data->AOP>my_struDBConfig.TxPowMax || data->AOP<my_struDBConfig.TxPowMin)
								{
									return FALSE;  	
								}
								else
								{
									break;
								}
							}
						}
						else if (data->AOP > aop_max)
						{
							setDAC = setDAC - (step>>1) ;
							if (setDAC<0)
							{
								return FALSE;
							}
						}
						else
						{
							break;
						}
			
						error = SET_10G_APC(INST_EVB[channel], setDAC);			 //写APC_DAC
						if (error)
						{
							return FALSE; 
						}
			
						Delay(2);
			
						if (!Read_AOP(channel, &data->AOP))
						{
							return FALSE;    
						}
						count++; 
					}while(count < 10); 
				
					if (data->AOP>my_struDBConfig.TxPowMax || data->AOP<my_struDBConfig.TxPowMin)
					{
						return FALSE;  	
					}
				}
			}

			/***增加10G发端光功率DAC值记录**Eric.Yao**20130716***/
			data->DACApc = setDAC;
			
			//写LUT 
			error = GET_CORE_Temper(INST_EVB[channel], &temper);		//核温索引改为Case索引
			if (error)
			{
				return FALSE;	
			}
			
			if(my_struConfig.Temper_High)
			{
				 if(temper<60.)
				 {
					 return FALSE;   
				 }
			 
				 error = GET_TEMPE_LUT(INST_EVB[channel],LUK_NAME[my_struControl.TunAOP10G_LUT_NAME_INDEX], my_struControl.TunAOP10G_LUK_INDEX, my_struControl.TunAOP10G_LUK_SLOPE_AMP, my_struControl.TunAOP10G_LUK_OFFSET_AMP, tempture_arry, temp_slop, temp_offset );
				 if (error)
				 {
				 	return FALSE;         
				 }
				 
				 if(temper>(tempture_arry[0]))
				 {
					ldv_10G_APC_DAC[1]=temp_slop[1]*(tempture_arry[0]+ my_struControl.GetTemper_IndexOffset)+temp_offset[1];
					ldv_10G_APC_DAC[2]=(double)setDAC ; 
			
				    ldv_coretemper[1]=tempture_arry[0]; 
					ldv_coretemper[2]=temper;  
			
					error = SET_TEMPE_LUT_High(INST_EVB[channel],LUK_NAME[my_struControl.TunAOP10G_LUT_NAME_INDEX], my_struControl.TunAOP10G_LUK_INDEX, my_struControl.TunAOP10G_LUK_SLOPE_AMP, my_struControl.TunAOP10G_LUK_OFFSET_AMP, ldv_10G_APC_DAC, ldv_coretemper);
					if(error)
					{
						return FALSE;  
					} 
				 }
				 else
				 {
				 	return FALSE;
				 }
			}
			else if(my_struConfig.Temper_Low)
			{
				 if(temper>25.) 
				 {
					 return FALSE;   
				 }
			 
				 error = GET_TEMPE_LUT(INST_EVB[channel],LUK_NAME[my_struControl.TunAOP10G_LUT_NAME_INDEX], my_struControl.TunAOP10G_LUK_INDEX, my_struControl.TunAOP10G_LUK_SLOPE_AMP, my_struControl.TunAOP10G_LUK_OFFSET_AMP, tempture_arry, temp_slop, temp_offset );
				 if (error)
				 {
				 	return FALSE;         
				 }
			   
				 if(temper<(tempture_arry[0]))
				 {
					ldv_10G_APC_DAC[1]=temp_slop[1]*(tempture_arry[0]+ my_struControl.GetTemper_IndexOffset)+temp_offset[1];
					ldv_10G_APC_DAC[0]=(double)setDAC ; 
			
					ldv_coretemper[0]=temper;
				    ldv_coretemper[1]=tempture_arry[0]; 
			
					error = SET_TEMPE_LUT_Low(INST_EVB[channel],LUK_NAME[my_struControl.TunAOP10G_LUT_NAME_INDEX], my_struControl.TunAOP10G_LUK_INDEX, my_struControl.TunAOP10G_LUK_SLOPE_AMP, my_struControl.TunAOP10G_LUK_OFFSET_AMP, ldv_10G_APC_DAC, ldv_coretemper);   
					if (error)
					{  
						return FALSE;  
					} 
				}
				else
				{
					return FALSE;
				}
			}
			
			//切换模式---自动模式
			error = SET_10G_APC_Mode(INST_EVB[channel], LUT_MODE_AUTO);
			if (error)
			{
				return FALSE;
			}
			
			//读取光功率
			if (!Read_AOP(channel, &data->AOP))
			{
				return FALSE;  
			} 
				
			if (data->AOP>my_struDBConfig.TxPowMax || data->AOP<my_struDBConfig.TxPowMin)
			{
				return FALSE;  	
			}
			
			//保存
			error = Save_Flash(INST_EVB[channel],my_struControl.TunAOP10G_SAVE);
			if (error)
			{
				return FALSE;    
			}
			
			error = Update_BASE(INST_EVB[channel], 0);
			if (error)
			{
				return FALSE;  
			} 
		}
		else
		{
			return FALSE;
		}
	}

	//记录Biase
	error = GET_10G_Bias(INST_EVB[channel], &txbias, my_struConfig.Bias_unit);
	if (error)
	{
		return FALSE;   
	}
	data->A2_Ibias=txbias;
	
	if (txbias > biasMax)  
	{
		sprintf(buf,"Bias电流:%f > %f mA",txbias ,biasMax);
		MessagePopup ("Error", buf);
		return FALSE; 
	}  
	return TRUE;
}

BOOL testSD(int channel)
{
	BYTE SD;
	int count;  

	count=0;
	do
	{
		//检测SD信号   0：模块los, 1:没有los 
		if (!Read_RX_SD(channel, 1, &SD))
		{
			return FALSE;  
		}
		if(SD == 1)
		{
			break;
		}
		Delay(1);
		count++;
	} while (count<3);
	if (count>=3) 
	{
		return FALSE;
	}
	
	return TRUE;
}

int testTxDis_10G(int channel, struct struTestData *data)
{
	int count,error;

//	Set_Att(my_struInstrument.ATT_TYPE, -60);
	Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, -60); 
	//TxDis
	if (EVB5_SET_ONU_TxDIS(INST_EVB[channel], 1))	   
	{
		return -1;
	}
	
	Delay(1.0);
	
	//判断光功率 
	count=0;
	do
	{
		if (!Read_AOP(channel, &data->TxOffPower)) 
		{
			return -1;  
		}
		
		if (data->TxOffPower>my_struDBConfig.TxDisPowMax)
		{
			Delay (0.5);
			count++;
		}
		else 
		{
			break;
		}
	} while (count<3);

	if (data->TxOffPower>my_struDBConfig.TxDisPowMax) 
	{
		return -1;
	}
	
	//将TxOffPower设置到-70dBm, 使显示正常
	data->TxOffPower=-70;
	
	//TxEn  for AOP test
	if (EVB5_SET_ONU_TxDIS(INST_EVB[channel], 0)) 
	{
		return -1;
	}
	
	return 0;
}

BOOL testFixSD(int channel, struct struTestData *data)
{
BYTE 	SD;
double 	temp_sda, temp_sdd, timer_star, timer, testTime=3.; 
int count, error, err_count;
int cycle = 6;
	
	Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, -51);//先设置光功率到-50dBm，确定到SDD状态
	
	temp_sda = my_struDBConfig.SDA+sRxCal_Lock.power[channel];
	Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sda);	  	//设置SDA 
	//测试SDA
	count=0;
	do
	{
		//检测SD信号   0：模块los, 1:没有los 
		if (!Read_RX_SD(channel, 1, &SD)) 
		{
			return FALSE;     /***Read_RX_SD内部将信号转换为SD，其他函数调用时，直接按SD处理**Eric.Yao**20130713***/
		}
		if(SD == 1) 
		{
			break;
		}
		Delay(0.1);
		count++;
	} while (count<cycle);
	if (count>=cycle)
	{
		return FALSE;
	}
	
	temp_sdd = my_struDBConfig.SDD+sRxCal_Lock.power[channel];
	Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sdd);	  	//设置SDD   
	//测试SDD
	count=0;
	do
	{
	
		//检测SD信号   0：模块los, 1:没有los 
		if (!Read_RX_SD(channel, 0, &SD)) 
		{
			return FALSE; 					  /***Read_RX_SD内部将信号转换为SD，其他函数调用时，直接按SD处理**Eric.Yao**20130713***/      
		}
		if(SD == 0) 
		{
			break;
		}
		Delay(0.1);  
		count++;
	} while (count<cycle);
	if (count>=cycle) 
	{
		return FALSE;
	}

	data->SDA=my_struDBConfig.SDA;
	data->SDD=my_struDBConfig.SDD;
	data->SDHys=my_struDBConfig.SDA-my_struDBConfig.SDD;
	
	Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, -20);//先设置光功率到-20dBm，确定到SDA状态
	
	return TRUE;
}

BOOL testCurrent(int channel, struct struTestData *data)
{
	if (EVB5_READ_TxI(INST_EVB[channel], &data->TxI)<0) 
	{
		return FALSE; 
	}
	
	if (EVB5_READ_RxI(INST_EVB[channel], &data->RxI)<0)
	{
		return FALSE;
	}
	
	//当光源板与测试板的序列号相同时，ONU光源的电流要扣除
	if (strcmp (my_struInstrument[channel].EVB_Light, my_struInstrument[channel].SEVB)==0 && my_struInstrument[channel].EVB_Light_TYPE != SEVB_TYPE_NONE)
	{
		data->TxI = (data->TxI-140)<0 ? 0:(data->TxI-140);
		data->RxI = (data->RxI-100)<0 ? 0:(data->RxI-100);	
	}
	
	data->RxI = (data->RxI-28)<0 ? 0:(data->RxI-28);
	
	if (data->TxI>my_struDBConfig.TxIMax || data->RxI>my_struDBConfig.RxIMax)
	{
		return FALSE;   
	}
	
	return TRUE;
}

BOOL testOEye(int channel, struct struTestData *data)
{
int count;  
	

	//配置眼图测试，适用ag86100，只能添加四个测试量
	if (my_struInstrument[channel].DCA_TYPE == DCA_TYPE_Ag86100)
	{
	   if (!Ag86100_SET_O_EYE(inst_Ag86100)) 
	   {
		   return FALSE;
	   }
	   Delay(2);
	}
	
	if (!READ_DCA_PJ(my_struInstrument[channel].DCA_TYPE, &data->TxPPj)) 
	{
		return FALSE; 
	}
	if (!READ_DCA_RJ(my_struInstrument[channel].DCA_TYPE, &data->TxRSj))
	{
		return FALSE; 
	}
	if (!READ_DCA_Rise(my_struInstrument[channel].DCA_TYPE, &data->TxRise)) 
	{
		return FALSE; 
	}
	if (!READ_DCA_Fall(my_struInstrument[channel].DCA_TYPE, &data->TxFall))
	{
		return FALSE; 
	}
	
	if (my_struInstrument[channel].DCA_TYPE == DCA_TYPE_Ag86100)
	{
	   if (!Ag86100_SET_O_ER(inst_Ag86100, my_struAg86100.X_Scale, my_struAg86100.X_Position, my_struAg86100.Y_Scale, my_struAg86100.Y_Offset,my_struAg86100.Channel_O)) 
	   {
		   return FALSE;  
	   }
	}	
	
	//考虑到86100只能在Ag86100_SET_O_ER函数后测试，因为Ag86100只能添加四个测试量
	if (!READ_DCA_PCTCROss(my_struInstrument[channel].DCA_TYPE, &data->TxCross))
	{
		return FALSE;  
	}
	
	data->TxPPj =data->TxPPj>1000  ? 1000:data->TxPPj;
	data->TxTj  =data->TxTj>1000   ? 1000:data->TxTj;
	data->TxRise=data->TxRise>1000 ? 1000:data->TxRise;
	data->TxFall=data->TxFall>1000 ? 1000:data->TxFall;
	data->TxCross=data->TxCross>1000 ? 1000:data->TxCross;    

	return TRUE;   
}

BOOL testOEye_Mask(int channel, struct struTestData *data)
{
	if (!GET_MaskHits(my_struInstrument[channel].DCA_TYPE, 200, &data->MaskMargin)) 
	{
		return FALSE;
	}

	return TRUE;   
}

BOOL testOEye10G_Mask(int channel, struct struTestData *data)
{
	if (!GET10G_MaskHits(my_struInstrument[channel].DCA_TYPE, 200, &data->MaskMargin)) 
	{
		return FALSE;
	}

	return TRUE;   
}

BOOL tuningCross_10G(int channel, struct struTestData *data)
{
	BOOL status;
	int error, count, voldac, DACmax, DACmin, biasvol, dac108, dac109, dac110;
	int dac, dac_target, step, forward, backward, aop_max, aop_min, dac_er, dacer_target;
	double cross, target, er, rsj, Aop, Aop_target, Aop_read, er_target, ex0, ex1;
	char strInpt[256], strOupt[256];   
	
	if (!Read_Cross(channel,&cross)) return FALSE;  
	dac=0;
	
	if(cross>my_struDBConfig.TxCrossMax || cross<my_struDBConfig.TxCrossMin)
	{  
		GET_TXCross_DAC_10G(INST_EVB[channel],&dac);
		
		dac_target = dac;
	//	target = 50.0;   //47-53
		target = (my_struDBConfig.TxCrossMax + my_struDBConfig.TxCrossMin) / 2;
		count=0; 
		step = 4;
		forward=0;
		backward=0;
		do // (cross < 50)
		{
			if (0 == dac_target)
			{
				data->TxCross = cross;
				return FALSE; 
			}
			if((cross<=target+3) && (cross>=target-3)) break;
		
			if (step < 1) break;
		
			if(cross < target)
			{
				forward = 1;
				if (1 == backward)
				{
					step = step/2;
					backward = 0;
				}
				dac_target = dac + step; 
			}
			else
			{
				backward = 1;
				if (1 == forward)
				{
					step = step/2;
					forward = 0; 
				}
				dac_target = dac - step; 
			}	  
		
			if (dac_target<0)
				dac_target = 0;
	
			if (!SET_TXCross_DAC_10G(INST_EVB[channel],dac_target))	return FALSE;  
 
			//if(!CSA8000_Set_Clear(inst_CSA8000)) return FALSE; 
			Delay(2);
			if (!Read_Cross(channel,&cross)) return FALSE;     
		
			dac = dac_target;
			ProcessDrawEvents (); 
			count++;
		
		} while (count<15);
		data->DACCross =  dac;
		
		if(count>=15)
		{
			return FALSE; 
		} 

		//保存数据  
		error = Save_Flash(INST_EVB[channel], my_struControl.TunCross10G_SAVE);
		if (error)
		{
			return FALSE;    
		}
	}

	GET_TXCross_DAC_10G(INST_EVB[channel],&dac); 
	data->DACCross =  dac;
	data->TxCross = cross;  
	
	return TRUE;
}

BOOL caltestTemper(int channel)
{
BYTE	MSB, LSB;
int		error;
unsigned short	temp;
double	temper;

	error = I2C_2BYTEs_READ_DLL(INST_EVB[channel], 0xA2, 0x60, &MSB, &LSB);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	temp = (MSB <<8) | LSB;
	if ((temp & 0x8000) == 0) 
	{temper = temp/256.0;}
	else 
	{temper = temp/256.0 - 256.0;}
	
	if(temper>60 || temper<20)  return FALSE;
	
	return TRUE;
}

BOOL calVcc(int channel)
{
unsigned short 	temp;
BYTE	MSB, LSB;
int		error;
double	Vcc;

	error = I2C_2BYTEs_READ_DLL  (INST_EVB[channel], 0xA2, 0x62, &MSB, &LSB);
	if (error<0) { MessagePopup ("ERROR", "NO Acknowledge from target !");  return FALSE;} 
	
	temp = (MSB <<8) | LSB;
	Vcc = temp/10000.0;
	
	if(Vcc>3.5 || Vcc<3.1)  return FALSE; 
	
	return TRUE;
}

BOOL calIbias(void)
{
	return TRUE;
}
	   
BOOL calTxPower10G(int channel, struct struTestData *data)
{
	float TxPower[2];
	float TxPow, TxPow_mon,delta; //mW
	unsigned short   ADC[2], DAC_r;
	unsigned short   APC, SetAPC;
	const float  Max_Tx_Pow_Prec = 1.;
	char buf[256] = "";
	
	int  index;
	int error;
	BOOL Stauts;

	if (!Read_AOP(channel, &TxPower[1]))
	{
		return FALSE;  
	}
	
	error = GET_10G_TxPOWER_ADC(INST_EVB[channel], &ADC[1]);
	if (error)
	{
		return FALSE;     
	}

	/***打到TxDisable**Eric.Yao**20130713***/
	error = Set_10G_TX_OFF (INST_EVB[channel]);
	if (error)
	{
		MessagePopup ("Error","关断10G发端错误  ");
		return FALSE;             
	}
	
	Delay (3);
	
	Stauts =Read_AOP(channel, &TxPower[0]);   /***关断后读取光功率**Eric.Yao**20130715***/
	if (!Stauts)
	{
		return FALSE;
	}
	
	if (TxPower[0] > -40.)
	{
		MessagePopup ("Error", "发端关断后光功率读数为大于-40dBm  ");
		return FALSE;
	}
	else
	{
		TxPower[0] = -40.;             	
	}  
	
	error = GET_10G_TxPOWER_ADC(INST_EVB[channel], &ADC[0]);
	if(error)
	{
		return FALSE;      
	}
	
	error = Set_10G_TX_ON (INST_EVB[channel]);
	if (error)
	{
		MessagePopup ("Error","打开10G发端错误  ");
		return FALSE;             
	}
	
	Delay (6);    /***延时6秒，可能是光功率计或者10G发端响应有问题，这个需要PE确认后再修改**Eric.Yao**20130715***/
	
 	//对过程数据赋值   
	data->TxCalPower0 = TxPower[0];
	data->TxCalPower1 = TxPower[1];  
	data->TxCalAdc0   = ADC[0];      
	data->TxCalAdc1   = ADC[1];  
	
	error = SET_10G_TXPOW_CAL(INST_EVB[channel], ADC, TxPower, my_struConfig.Tx_unit);
	if (error)
	{
		return FALSE;    
	} 

	//添加测试流程
	if (!Read_AOP(channel, &TxPow))
	{
		return FALSE; 
	}
	
	if (GET_10G_TxMon(INST_EVB[channel], &TxPow_mon, my_struConfig.Tx_unit) < 0)
	{
		return FALSE;
	}
	
	data->A2_TxMon = TxPow_mon;  
	
	delta = fabs(TxPow - TxPow_mon);

	if (delta > Max_Tx_Pow_Prec) 
	{
		sprintf(buf, "发端光功率监控值%.2fdBm与实际光功率值%.2fdBm之差大于%.2fdB  ", TxPow_mon, TxPow, Max_Tx_Pow_Prec);
		MessagePopup ("Error", buf);
		return FALSE;      
	}
	
	/***增加发端监控光功率范围判断，其范围必须在数据库配置的光功率范围内**Eric.Yao**20130715***/          
	if ((TxPow_mon < my_struDBConfig.TxCalMin) || (TxPow_mon > my_struDBConfig.TxCalMax))
	{
		return FALSE;   
	}

	return TRUE;
}

BOOL calTxPower10G_Ex(int channel, struct struTestData *data)
{
	float TxPower[2], TxPow, TxPow_mon,delta; //mW
	unsigned short   ADC[2], DAC_r;
	const float  Max_Tx_Pow_Prec = 1.;
	char buf[256] = "";
	double dAOPavg;
	
	int  index;
	int  error;
	BOOL Status;
	
	//关闭收端，避免反射
	error = Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, -60.); 
	if(error<0)
	{
		return FALSE;  
	}
	


	dAOPavg = (my_struDBConfig.TxPowMin + my_struDBConfig.TxPowMax) / 2.; 

	Status = Read_AOP(channel, &TxPower[1]);
	if (!Status)
	{
		return FALSE;  
	}
	// 读取背光ADC	   
	error = GET_10G_TxPOWER_ADC(INST_EVB[channel], &ADC[1]);
	if (error)
	{
		return FALSE;     
	}
	// 关断发端读取数值
	//Set TxDisable
	error = EVB5_SET_ONU_TxDIS (INST_EVB[channel], 1);
	if (error)
	{
		return FALSE;     
	}
	
	Delay(0.5);
	Status = Read_AOP(channel, &TxPower[0]);
	if (!Status)
	{
		return FALSE;  
	}
	// 读取背光ADC	   
	error = GET_10G_TxPOWER_ADC(INST_EVB[channel], &ADC[0]);
	if (error)
	{
		return FALSE;     
	}

	//对过程数据赋值   
	data->TxCalPower0 = TxPower[0];
	data->TxCalPower1 = TxPower[1];  
	data->TxCalAdc0   = ADC[0];      
	data->TxCalAdc1   = ADC[1]; 
	
	if(SET_10G_TXPOW_CAL(INST_EVB[channel], ADC, TxPower, my_struConfig.Tx_unit)<0)
	{
		return FALSE;    
	} 
	
	/***该固件版本增加发端监控阈值写**Eric.Yao**20150514***/
//	error = SET_10G_TxMon(INST_EVB[channel],my_struDBConfig.TxCalMin, my_struDBConfig.TxCalMax, my_struConfig.Tx_unit);
//	if (error < 0)
//	{
//		return FALSE;	
//	} 
	
	error = Update_BASE(INST_EVB[channel], 0);
	if (error < 0)
	{
		return FALSE;	
	}
	
	//Set TxDisable
	error = EVB5_SET_ONU_TxDIS (INST_EVB[channel], 0);
	if (error)
	{
		return FALSE;     
	}
	Delay(1.0);

	//添加测试流程
	Status = Read_AOP(channel, &TxPow);
	if (!Status)
	{
		return FALSE; 
	}
	
//	if (GET_10G_TxMon(INST_EVB[channel], &TxPow_mon, my_struConfig.Tx_unit) < 0)
	if (MyDLL_8472_GetTxPWR(INST_EVB[channel], &TxPow_mon) < 0)   
	{
		return FALSE;
	}
	data->A2_TxMon = TxPow_mon;
	
	delta = fabs(TxPow - TxPow_mon);
	if (delta > Max_Tx_Pow_Prec) 
	{
		sprintf(buf, "发端光功率监控值%.2fdBm与实际光功率值%.2fdBm之差大于%.2fdB  ", TxPow_mon, TxPow, Max_Tx_Pow_Prec);
		MessagePopup ("Error", buf);
		return FALSE;      
	}
	
	/***增加发端监控光功率范围判断，其范围必须在数据库配置的光功率范围内**Eric.Yao**20130715***/  
	my_struDBConfig.TxCalMin = 0;
	my_struDBConfig.TxCalMax = 1.0;
	if ((TxPow_mon < my_struDBConfig.TxCalMin) || (TxPow_mon > my_struDBConfig.TxCalMax))
	{
		return FALSE;   
	}

	return TRUE;
}	
BOOL WriteDateTables(int panSearchData_chan0, struct struTestData data)
{
	Point cellAddr;
	// ID ...
	// PN
	cellAddr.x = 2;
	cellAddr.y = 1;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(2), ATTR_CELL_TYPE,VAL_CELL_STRING);
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.PN);
	// VERSION
	cellAddr.x = 3;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(3), ATTR_CELL_TYPE,VAL_CELL_STRING);
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.Ver);
	// TESTDATE
	cellAddr.x = 4;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(4), ATTR_CELL_TYPE,VAL_CELL_STRING);
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.TestDate);
	// OPTICSSN
	cellAddr.x = 5;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(5), ATTR_CELL_TYPE,VAL_CELL_STRING);
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.OpticsSN);
	// TESTFLAG
	cellAddr.x = 6;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(6), ATTR_CELL_TYPE,VAL_CELL_STRING);
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.TestFlag);
	// STATUS
	cellAddr.x = 7;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(7), ATTR_CELL_TYPE,VAL_CELL_STRING);
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.Status);
	// TXAOP
	cellAddr.x = 8;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(8), ATTR_CELL_TYPE,VAL_CELL_NUMERIC);
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.AOP);
	// TXER
	cellAddr.x = 9;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(9), ATTR_CELL_TYPE,VAL_CELL_NUMERIC);
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.ER);
	// OVERLOAD
	cellAddr.x = 10;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(10), ATTR_CELL_TYPE,VAL_CELL_NUMERIC); 
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.Over);
	// OVERTIME
	cellAddr.x = 11;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(11), ATTR_CELL_TYPE,VAL_CELL_NUMERIC); 
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.OverTime);
	// SENSITIVITY
	cellAddr.x = 12;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(12), ATTR_CELL_TYPE,VAL_CELL_NUMERIC); 
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.Sens);
	// PATHPENALTY
	cellAddr.x = 13;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(13), ATTR_CELL_TYPE,VAL_CELL_NUMERIC); 
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.Pathpenalty);
	// TXI
	cellAddr.x = 14;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(14), ATTR_CELL_TYPE,VAL_CELL_NUMERIC); 
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.TxI);
	// RXI
	cellAddr.x = 15;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(15), ATTR_CELL_TYPE,VAL_CELL_NUMERIC); 
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.RxI);
	// DAC_APC
	cellAddr.x = 16;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(16), ATTR_CELL_TYPE,VAL_CELL_NUMERIC); 
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, (double)(data.DACApc));
	// DAC_MOD
	cellAddr.x = 17;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(17), ATTR_CELL_TYPE,VAL_CELL_NUMERIC); 
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, (double)(data.DACMod));
	// DAC_CROSS
	cellAddr.x = 18;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(18), ATTR_CELL_TYPE,VAL_CELL_NUMERIC); 
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, (double)(data.DACCross));
	// DAC_APD
	cellAddr.x = 19;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(19), ATTR_CELL_TYPE,VAL_CELL_NUMERIC); 
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, (double)(data.DACApd));
	// A2_TEMPERATRUE
	cellAddr.x = 20;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(20), ATTR_CELL_TYPE,VAL_CELL_NUMERIC); 
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.A2_Temper);
	// A2_VCC
	cellAddr.x = 21;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(21), ATTR_CELL_TYPE,VAL_CELL_NUMERIC); 
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.A2_Vcc);
	// A2_IBIAS
	cellAddr.x = 22;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(22), ATTR_CELL_TYPE,VAL_CELL_NUMERIC); 
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.A2_Ibias);
	// A2_TXMON
	cellAddr.x = 23;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(23), ATTR_CELL_TYPE,VAL_CELL_NUMERIC); 
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.A2_TxMon);
	// TEC_TEMPERATURE
	cellAddr.x = 24;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(24), ATTR_CELL_TYPE,VAL_CELL_NUMERIC); 
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.CaseTemper);
	// TEC_CURRENT
	cellAddr.x = 25;
	SetTableCellRangeAttribute(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA,VAL_TABLE_COLUMN_RANGE(25), ATTR_CELL_TYPE,VAL_CELL_NUMERIC); 
	SetTableCellVal(panSearchData_chan0, PAN_SEAR0_CHAN0_DATA, cellAddr, data.Temperatrue);

	return TRUE;
}

/*
BOOL SaveDataBase(struct struTestData data, struct struProcessLOG prolog)
{
	int error, status;
	
	error = DB_Save_Results_ATE_New (data, prolog);    //***使用存储过程完成Results_ATE、Results_10G和Process_Log三张表的数据存储和关联**Eric.Yao**20130722**
	if (error) 
	{
		return FALSE;
	}

	//只有当校准点>0时，才记录校准测试结果，每次开始测试时校准点将被恢复到0，
	//如果在收端校准测试开始前出错，校准点将不被赋值，数据库中就不会有校准记录
	if (data.RxPow_num>0)
	{
		error = DB_Save_Results_Monitor (data);
		if (error)
		{
			return FALSE;
		}
	}
	
	if (data.curvesize)
	{
		error = DB_Save_Results_Image_for_Oracle(data, prolog);
		if (error) 
		{
			return FALSE;  
		}
	}
	
	return TRUE;
}
 */


BOOL SaveDataBase(struct struTestData data, struct struProcessLOG prolog)
{
	int error, status;
	/*
	error = DB_Save_Results_10G_Ex (data, prolog);    //***使用存储过程完成Results_ATE、Results_10G和Process_Log三张表的数据存储和关联**Eric.Yao**20130722**
	if (error) 
	{
		return FALSE;
	}	 */
	
	error = DB_Save_Results_ATE_New (data, prolog);
	if (error) 
	{
		return FALSE;
	}

	
	//只有当校准点>0时，才记录校准测试结果，每次开始测试时校准点将被恢复到0，
	//如果在收端校准测试开始前出错，校准点将不被赋值，数据库中就不会有校准记录
	if (data.RxPow_num>0)
	{
		error = DB_Save_Results_Monitor (data);
		if (error)
		{
			return FALSE;
		}
	}
	
	if (data.curvesize)
	{
		error = DB_Save_Results_Image_for_Oracle(data, prolog);
		if (error) 
		{
			return FALSE;  
		}
	}
	
	return TRUE;
}



BOOL testFixOver_1G(int channel, struct struTestData *data)
{
BYTE 	SD;
double 	temp_Over;
int	timer_star, timer, err_count, error;
int OptoBERT_Lock, OptoBERT_ErrorCount;

	temp_Over = my_struDBConfig.Over+sRxCal_Lock.power[channel]; 
	
	Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_Over);	  	//设置灵敏度功率 
	Read_RX_SD(channel, 0, &SD);   				//检测SD信号   0：模块los, 1:没有los  
	if(SD == 1) //0：模块los		//如果有LOS，增大光功率，直到没有LOS信号，step＝2dB   
	{
		do
		{
			temp_Over=temp_Over+2.0;     

			Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_Over);     
			Read_RX_SD(channel, 0, &SD);
		} while ((SD==1)&&(temp_Over<0));
	
		temp_Over = my_struDBConfig.Over+sRxCal_Lock.power[channel]; 
		
		//重新设置灵敏度功率
		Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_Over);   
		
		Read_RX_SD(channel, 0, &SD); 
		if(SD == 1)	
		{
			return FALSE; 							//如果还是有LOS信号，报错
		}
	}
	
	error = BERT_Start (channel, my_struDBConfig.OverTime, 0, 0);
	if (error)
	{
		return FALSE; 
	}
	
	timer_star=Timer(); 
	do
	{
		//读取误码
		Delay(1); 
		Read_Error_Bit(channel, &err_count, 1, 0);
		timer = Timer()- timer_star; 
	} while (timer<my_struDBConfig.OverTime && err_count==0);
	
	error = BERT_Stop (channel); 
	if (error) 
	{
		return FALSE;
	}

	//判断测试结果,并赋值 
	if ((timer < my_struDBConfig.OverTime) || (err_count>0)) 
	{
		data->Over=0; 
		return FALSE;
	}

	data->Over=my_struDBConfig.Over;
	data->Over=timer;
	
	return TRUE;
}

BOOL testFixSens_1G(int channel, struct struTestData *data)
{
	BYTE 	SD;
	double 	temp_sens;
	int	timer_star, timer, err_count, error;
	int OptoBERT_Lock, OptoBERT_ErrorCount;
	
	signed  int ErroCode;
	double	ERR_Ratio;

	temp_sens = my_struDBConfig.Sens+sRxCal_Lock.power[channel]; 
	
	Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sens);	  	//设置灵敏度功率 
	Read_RX_SD(channel, 0, &SD);   				//检测SD信号   0：模块los, 1:没有los  
/*	if(SD == 1) //0：模块los		//如果有LOS，增大光功率，直到没有LOS信号，step＝2dB   
	{
		do
		{
			temp_sens=temp_sens+2.0;     

			Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sens);     
			Read_RX_SD(channel, 0, &SD);
		} while ((SD==1)&&(temp_sens<0));
	
		temp_sens = my_struDBConfig.Sens+sRxCal_Lock.power[channel]; 
		
		//重新设置灵敏度功率
		Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sens);   
		
		Read_RX_SD(channel, 0, &SD); 
		if(SD == 1)	
		{
			return FALSE; 							//如果还是有LOS信号，报错
		}
	}
*/	
	error = BERT_Start (channel, my_struDBConfig.SensTime, 0, 0);
	if (error)
	{
		return FALSE; 
	}
	
	timer_star=Timer(); 
	do
	{
		//读取误码
		Delay(1); 
	//	Read_Error_Bit(channel, &err_count, 1, 0);
		Read_Bert_ERR_Ratio_1G(channel, &ErroCode,&ERR_Ratio); 
		timer = Timer()- timer_star; 
	} while (timer<my_struDBConfig.SensTime && ERR_Ratio<1.0E-4);
	
	error = BERT_Stop (channel); 
	if (error) 
	{
		return FALSE;
	}

	//判断测试结果,并赋值 
	if ((timer < my_struDBConfig.SensTime) || (ERR_Ratio>1.0E-4)) 
	{
		data->Sens=0; 
		return FALSE;
	}

	data->Sens=my_struDBConfig.Sens;
	data->SensTime=timer;
	
	return TRUE;
}

BOOL testFixSens_Start_1G(int channel, struct struTestData *data, int *StartTime)
{
	double 	temp_sens, temp_ref;
	int	err_count, error;

	temp_sens = my_struDBConfig.Sens+sRxCal_Lock.power[channel]; 
	
	Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sens);	  	//设置灵敏度功率  
	
    error = BERT_Start (channel, my_struDBConfig.SensTime, 1, 0);
	if(error<0)
	{
		err_count=0 ;
		do
		{   Delay(0.5);
			error= BERT_Start (channel, my_struDBConfig.SensTime, 1, 0);
			err_count++;
		}while(error<0 || err_count<3);
		if(err_count>=3 && error<0)
		{
			goto Error; 
		}
	}
	
	Delay(1); 
	
	error = BERT_Read(channel, &err_count, 0);
	if ((error<0) || (0!=err_count)) 
	{
		MessagePopup("Error", "Sens test Start fail!"); 
		goto Error;
	}

	*StartTime=Timer();  

	return TRUE;
	
Error:
	
	BERT_Stop (channel);
	
	return FALSE;    
}

BOOL testFixSens_End_1G(int channel, struct struTestData *data, int StartTime)
{
	double 	temp_sens;
	int	timer_star, timer, err_count, error;

	timer_star=StartTime; 
	do
	{
		//读取误码
		Delay(1); 
		//Read_Error_Bit(&err_count, 1, 0);
		BERT_Read(channel, &err_count, 1);
		timer = Timer()- timer_star; 
	} while ((timer<my_struDBConfig.SensTime) && (0 == err_count));
	
	error = BERT_Stop (channel); 
	if (error) 
	{
		return FALSE;
	}

	//判断测试结果,并赋值 
	if ((timer<my_struDBConfig.SensTime) || (err_count>0)) 
	{
		data->Sens=0; 
		return FALSE;
	}

	data->Sens=my_struDBConfig.Sens;
	data->SensTime=timer;
	
	return TRUE;
}

BOOL testFixSens_10G(int channel, struct struTestData *data)
{
BYTE 	SD;
double 	temp_sens, err_rate;
int		error,timer_star,timer;
unsigned long long 	err_count;

	temp_sens = -20. + sRxCal_Lock.power[channel];
	Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sens);	  	//设置灵敏度功率  

	Delay(0.5);	
	
	error = BERT_10G_Start (channel, my_struDBConfig.SensTime);
	if (error) 
	{
		return FALSE; 
	}
	

	temp_sens = my_struDBConfig.Sens+sRxCal_Lock.power[channel]; 
	
	Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sens);	  	//设置灵敏度功率       
	Read_RX_SD(channel, 1, &SD);   				//检测SD信号   0：模块los, 1:没有los  
	if(SD == 0) //0：模块los		//如果有LOS，增大光功率，直到没有LOS信号，step＝2dB   
	{
		do
		{
			temp_sens=temp_sens+2.0;     
			Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sens); 
			Read_RX_SD(channel, 1, &SD);
		} while ((SD==0)&&(temp_sens<0));
	
		temp_sens = my_struDBConfig.Sens + sRxCal_Lock.power[channel];
		
		Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sens);		//重新设置灵敏度功率
		
		Read_RX_SD(channel, 1, &SD); 
		if(SD == 0)	
		{
			return FALSE; 							//如果还是有LOS信号，报错
		}
	}
	
	timer_star=Timer();
	
	error = BERT_10G_Start (channel, my_struDBConfig.SensTime);
	if (error) 
	{
		return FALSE; 
	}
	
	timer_star=Timer(); 
	if (my_struInstrument[channel].BERT_TYPE_10G == BERT_TYPE_GBERT10000) 
	{
		Delay(1); 
		if (!Read_Error_Bit_10G(channel, &err_count, &err_rate, my_struDBConfig.SensTime)) 
		{
			MessagePopup("Error", "Sens test Start fail!"); 
			return FALSE;
		}
	}
	else
	{
		do
		{
			//读取误码
			Delay(1); 
			Read_Error_Bit_10G(channel, &err_count, &err_rate, my_struDBConfig.SensTime);
	
			timer = Timer()- timer_star; 
		} while (timer<my_struDBConfig.SensTime && err_rate < 1.0E-3);		// while (timer<my_struDBConfig.SensTime);
	}
	
	timer = Timer()- timer_star;  
	
	//判断测试结果,并赋值 
	if (err_rate > 1.0E-3) 
	{
		data->Sens=0; 
		return FALSE;
	}

	data->Sens=my_struDBConfig.Sens;
	data->SensTime=timer;  

	return TRUE;
}

BOOL testFixOver_10G(int channel, struct struTestData *data)					//测试10G过载 
{
	double 	temp_sens=0;
	double  ErrorRate=0; 
	BYTE 	SD=0;
	int 	error=0;
	
	//设置衰减模式
	if(GVPM_SET_LockEnable(INST_ATT[channel],1)<0)
	{
		return FALSE;
	}
	//	Set_Att(my_struInstrument.ATT_TYPE, temp_sens);	  	          //设置灵敏度功率    
	temp_sens = my_struDBConfig.Over+sRxCal_Lock.power[channel]; //my_struDBConfig.Over+my_struRxCal.RxCal_Power; 

	error = Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sens);   		//wenyao.xi 20101030
	if (error<0)
	{
		MessagePopup("Error", "Set Att error!");
		return FALSE;         
	}
	Read_RX_SD(channel, 1, &SD);						          //检测SD信号   0：模块los, 1:没有los  
	if(SD == 0) 										          //如果有LOS信号，报错   
	{
		return FALSE; 						                
	}
	
	Delay(2);
	error = Init_10GBERT (channel);   
	if (error<0) 
	{
		return FALSE; 
	}
	
	error = evb16_SaveSi570NVM(INST_BERT_10G[channel]);  
	if(error<0)
	{
		MessagePopup("提示", "RECALL Si570 NVM 失败 ");  
		return FALSE;  
	}
	
	error = BERT_10G_Start (channel, my_struDBConfig.OverTime);
	if (error<0) 
	{
		return FALSE; 
	}
	Delay(1);
	error = Read_BER(channel, my_struDBConfig.OverTime, &ErrorRate,1);
	if (error<0) 
	{
		return FALSE;	
	}

	if (ErrorRate>9E-4) 		                    	//判断测试结果,并赋值      
	{
		data->Over=0; 
		return FALSE;
	}

	data->Over=my_struDBConfig.Over;
	data->OverTime=my_struDBConfig.OverTime;
	
	return TRUE;
}

BOOL tuningSD(int channel, struct struTestData *data)
{
	INT16U 	DAC0, DAC, DACmax, DACmin, DACstep;

	INT16U  DACstep_Arr[3]={0x400,0xA0,0x10}; 
	
	double 	temp_sd;
	BYTE	SD;
	int	    i,j, error;
	
	Delay(1);  

	DACmax=0xFFF;
	DACmin=0x000;

	//check sda
	DAC=DACmin;

	if (Set_LOS_DAC(INST_EVB[channel], DAC) < 0)
	{
		return FALSE;           
	}
	temp_sd = my_struDBConfig.SDA+sRxCal_Lock.power[channel];
	Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sd);   
	
	error = Read_RX_SD (channel, 1, &SD);
	if (!error)
	{
		return FALSE;
	}

	if(SD!=1) 
	{
		MessagePopup ("ERROR", "Rsd=min SDA test fail !"); 
		return FALSE;
	} 

	//check sdd
	DAC=DACmax;
	
	if (Set_LOS_DAC(INST_EVB[channel], DAC) < 0)
	{
		return FALSE;           
	}
	temp_sd = my_struDBConfig.SDD+sRxCal_Lock.power[channel];

	Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sd);

	error = Read_RX_SD (channel, 0, &SD);
	if (!error)
	{
		return FALSE;
	}
	
	if(SD!=0) 
	{
		MessagePopup ("ERROR", "Rsd=max SDD test fail !");
		return FALSE;
	} 
	
	// 调试sda
	temp_sd = my_struDBConfig.SDA+sRxCal_Lock.power[channel];
	Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sd);
	
	i=0;
	j=0;
	do
	{
		do
		{
			DACstep=DACstep_Arr[j];
			DAC=DAC-DACstep;
			DAC=DAC<DACmin ? DACmin:DAC;//防止溢出
			
			if (Set_LOS_DAC(INST_EVB[channel], DAC) < 0)
			{
				return FALSE;           
			}	  
			
			Delay(1);

			error = Read_RX_SD (channel, 1, &SD);
			if (!error)
			{
				return FALSE;
			}
		  
			if(SD==1) 
			{
				break;			//得到了SDA点,跳出本次循环     
			}
	
			i++;
		} while (DAC>DACmin);		//如果没有到达Rsd的最大步长继续循环
		
		if (DAC==DACmin)   
		{
			break;  
		}
		if (DACstep==DACstep_Arr[2]) 
		{
			break;   //得到了最小的step 
		}
		
		DAC=DAC+DACstep;
		
		if (Set_LOS_DAC(INST_EVB[channel], DAC) < 0)
		{
			return FALSE;           
		}

		Delay(0.);
		temp_sd = my_struDBConfig.SDA + sRxCal_Lock.power[channel];
		Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sd);	  

		ProcessDrawEvents ();
		j++;
	}while (DAC>DACmin);

	Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sd - 0.6);	

	error = Read_RX_SD (channel, 1, &SD);
	if (!error)
	{
		return FALSE;
	}
	
	if(SD!=1) 
	{
		MessagePopup ("ERROR", "SDHys<=0.6!");
		return FALSE;
	} 
	
	data->SDA = temp_sd - sRxCal_Lock.power[channel];   
	
	temp_sd = my_struDBConfig.SDD + sRxCal_Lock.power[channel];    
	Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sd);	
	//研发要求增加延时 130605
	Delay(0.1);

	error = Read_RX_SD (channel, 0, &SD);
	if (!error)
	{
		return FALSE;
	}
	
	if(SD!=0) 
	{
		MessagePopup ("ERROR", "tuning SDA SDD fail!");
		return FALSE;
	} 
	
	data->SDD=my_struDBConfig.SDD;    
	
	error = Update_BASE(INST_EVB[channel], 0);
	if (error)
	{
		return FALSE;  
	}

	return TRUE;
}		 

BOOL tuningSD2(int channel, struct struTestData *data)					   //实际为测试SD,未调试
{
	double 	temp_sd;
	BYTE	SD;
	int	    error;

	temp_sd = my_struDBConfig.SDA+sRxCal_Lock.power[channel];
	Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sd);
	Delay(2); 
	error = Read_RX_SD (channel, 1, &SD);
	if (!error)
	{
		return FALSE;
	}
	if(SD==0) 
	{
		MessagePopup ("ERROR", "SDA test fail !"); 
		return FALSE;
	}
	data->SDA=my_struDBConfig.SDA;
	
	//SDD
	temp_sd = my_struDBConfig.SDD+sRxCal_Lock.power[channel];

	Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sd);
	Delay(2);
	error = Read_RX_SD (channel, 0, &SD);
	if (!error)
	{
		return FALSE;
	}
	if(SD==1) 
	{
		MessagePopup ("ERROR", "SDD test fail !"); 
		return FALSE;
	}
	data->SDD=my_struDBConfig.SDD;
	return TRUE;  
}

BOOL Read_AOP(int channel, float *AOP)
{
	double Power;

	if (!GET_Power(channel, &Power)) 
	{
		return FALSE;
	}
	*AOP=Power+my_struTxCal.power[channel];

  	return TRUE;
}

BOOL Read_ER(int channel, double *ER)
{
	double Er;

	if (!GET_Er(my_struInstrument[channel].DCA_TYPE, &Er)) 
	{
		return FALSE; 
	}

	*ER=Er*my_struTxCal.TxCal_Er_Slope+my_struTxCal.TxCal_Er_Offset;  

	return TRUE;
}

BOOL Read_Cross(int channel, double *Cross)
{
double cross;

	if (!GET_Cross(my_struInstrument[channel].DCA_TYPE, &cross))
	{
		return FALSE; 
	}
	*Cross=cross;  

	return TRUE;
}

BOOL Read_Temperature(int channel, float *Temperatrue)
{
	int error;
	
	error = GET_CASE_Temper(INST_EVB[channel], Temperatrue, my_struConfig.Temp_unit);
	if (error)
	{
		return FALSE;     
	}
 
	return TRUE; 	
}

BOOL Read_RX_SD(int channel, const int SD_Target, BYTE *SD)
{
	BYTE temp, tempSD;
	int	i;

	if (SD_Target == -1) //不需要获取稳定的状态，只读一次  
	{
		switch (my_struInstrument[channel].SEVB_TYPE)
		{
			case SEVB_TYPE_SEVB5: 
			case SEVB_TYPE_SEVB27: 
				if (EVB5_READ_OLT_RxLOS(INST_EVB[channel], &temp)<0) 
				{
					return FALSE;
				}
				
				break;	
			default:
				MessagePopup("Error", "Can not find this EVB type"); 
				return FALSE;
		}
		//添加los信号的读取和判断，便于SFP的模块在通常的信号读取中使用  
		//tempSD = temp>0 ? 0:1;  
		temp = temp>0? 1:0;
			
		/***根据SD或者LOS判断，将信号统一为转换为SD**Eric.Yao**20130713***/
		if(my_struDBConfig.isSDorLOS)
		{
			tempSD = temp;  
		}
		else
		{
			tempSD = !temp;
		}
	}	
	else //为了确认SD的测试准确,需要多测几次
	{
		i=0;
		do 
		{
			switch (my_struInstrument[channel].SEVB_TYPE)
			{
				case SEVB_TYPE_SEVB5: 
				case SEVB_TYPE_SEVB27:
					if (EVB5_READ_OLT_RxLOS(INST_EVB[channel], &temp)<0) 
					{
						return FALSE;  
					}
					
					break;	
				default:
					MessagePopup("Error", "Can not find this EVB type"); 
					return FALSE;
			}			
			//添加los信号的读取和判断，便于SFP的模块在通常的信号读取中使用  
			temp = temp>0? 1:0;	   
		//	tempSD = !temp;   
			
			if(my_struDBConfig.isSDorLOS)
			{
				tempSD = temp;  
			}
			else
			{
				tempSD = !temp;
			}
			
			if (SD_Target == tempSD) 
			{
				break;
			}
			i++;
			Delay(0.5);
		}while (i<4);
	}   
		
	*SD=tempSD; 

	return TRUE;
}

BOOL Read_RX_LOS(int channel, const int LOS_Target, BYTE *LOS)
{
int	i;
BYTE temp;
	
	if (LOS_Target==-1)	//不需要获取稳定的状态，只读一次 
	{
		switch (my_struInstrument[channel].SEVB_TYPE)
		{
			case SEVB_TYPE_SEVB5: 
				if (EVB5_READ_OLT_RxLOS(INST_EVB[channel], &temp)<0)
				{
					return FALSE;  
				}
				break;			
			default:
				{
					MessagePopup("Error", "Can not find this EVB type");
					return FALSE;         
				} 
				
				break;			
		}		
	}
	else  //为了确认SD的测试准确,需要多测几次    
	{
		i=0;
		do 
		{
			switch (my_struInstrument[channel].SEVB_TYPE)
			{
				case SEVB_TYPE_SEVB5: 
					if (EVB5_READ_OLT_RxLOS(INST_EVB[channel], &temp)<0) 
					{
						return FALSE;  
					}
				
					break;			
				default:
					{
						MessagePopup("Error", "Can not find this EVB type");
						return FALSE;        
					}  
					break;			
			}			
			if (LOS_Target == temp)
			{
				break;
			}
			i++;
			Delay(0.5);
		}while (i<4);
	}
	
	*LOS=temp;
	
	return TRUE;
}

BOOL Read_Tx_Fail(int channel, BYTE *Fail)
{
	BYTE temp;

	switch (my_struInstrument[channel].SEVB_TYPE)
	{
		case SEVB_TYPE_SEVB5: 
			if (EVB5_READ_OLT_TxFAULT(INST_EVB[channel], &temp)<0) 
			{
				return FALSE;  
			}
					
			break;			
		default:
			{
				MessagePopup("Error", "Can not find this EVB type");
				return FALSE;                   
			} 
			
			break;			
	}
	
	if (my_struConfig.PN_TYPE==PN_SFP)
	{
		*Fail= temp>0 ? 1:0;
	} 
	else 	
	{
		MessagePopup("Error", "PN type select error!"); 
		return FALSE;
	} 
	
	return TRUE;
}

BOOL Read_Error_Bit(int channel, unsigned int *ErrorCount, int TestFlag, double testtime)
{
int 	Lock, error, sBERT_Status_Code, count; 
char 	sBERT_Status_String[500];
double 	sBERT_ERR_Number, sBERT_ERR_Ratio, sBERT_ElapsedTime; 

	 switch (my_struInstrument[channel].BERT_TYPE)
	{
		case BERT_TYPE_GBERT:
			
			if (TestFlag==0)
			{
				if (!GBERT_Check_Start(INST_BERT[channel], ErrorCount)) 
				{
					return FALSE;   
				}
			}
			else
			{
				if (!GBERT_Check(INST_BERT[channel], ErrorCount)) 
				{
					return FALSE;
				}
			}			
			break;
			
		case BERT_TYPE_SBERT: 
			
			error = EVB5_SBERT_Read(INST_BERT[channel], SBERT_R_R2488Mbps, SBERT_P_PRBS7, &sBERT_Status_Code, sBERT_Status_String, &sBERT_ERR_Number, &sBERT_ERR_Ratio, &sBERT_ElapsedTime); 
			if (TestFlag==0 && (error==-2 || error==-4))
			{
				count = 0;
				do
				{
					//开始测试，出现同步不上的问题，增加BERT初始化流程
					if (EVB5_SBERT_Init (INST_BERT[channel], SBERT_R_R2488Mbps, SBERT_P_PRBS23)<0) 
					{
						return FALSE;
					}
				
					if (EVB5_SBERT_Start(INST_BERT[channel], SBERT_R_R2488Mbps, testtime)<0) 
					{
						MessagePopup("Error", "SBERT start test error!");
						return FALSE;
					}

					Delay (0.3);
					error = EVB5_SBERT_Read(INST_BERT[channel], SBERT_R_R2488Mbps, SBERT_P_PRBS7, &sBERT_Status_Code, sBERT_Status_String, &sBERT_ERR_Number, &sBERT_ERR_Ratio, &sBERT_ElapsedTime); 
				} while ((error==-2 || error==-4) && count<5);
			}

			if (error<0 || sBERT_Status_Code<0) 
			{
			//	MessagePopup("Error", sBERT_Status_String);
				*ErrorCount=99;
				return FALSE;
			}  
			*ErrorCount=sBERT_ERR_Number;
			break;
		
		default: 
			{
				MessagePopup("Error", "BERT type error!");
				return FALSE;
			} 
	}
	return TRUE;
}

BOOL Read_Error_Bit_Ex(int channel, unsigned int *ErrorCount, int TestFlag)
{
	int		Lock, error, sBERT_Status_Code; 
	char 	sBERT_Status_String[500];
	double 	sBERT_ERR_Number, sBERT_ERR_Ratio, sBERT_ElapsedTime;  

	switch (my_struInstrument[channel].BERT_TYPE)
	{
		case BERT_TYPE_GBERT:
			if (TestFlag==0)
			{
				if (!GBERT_Check_Start(INST_BERT[channel], ErrorCount))
				{
					return FALSE;  
				}
			}
			else if (TestFlag==1)
			{
				if (!GBERT_Check(INST_BERT[channel], ErrorCount)) 
				{
					return FALSE;
				}
			}
			else
			{
				error= GBERT_READ_Status (INST_BERT[channel], ErrorCount);
				if (error) 
				{
					return FALSE;
				}
			}
			break;
		case BERT_TYPE_SBERT: 
			error = EVB5_SBERT_Read(INST_BERT[channel], SBERT_R_R2488Mbps, SBERT_P_PRBS7, &sBERT_Status_Code, sBERT_Status_String, &sBERT_ERR_Number, &sBERT_ERR_Ratio, &sBERT_ElapsedTime); 
			if (error<0 || sBERT_Status_Code<0)
			{
				*ErrorCount=99;
				return FALSE;
			} 
			*ErrorCount=sBERT_ERR_Number;
			break;
		default:
			{
				MessagePopup("Error", "BERT type error!");
				return FALSE;
			} 
	}
	return TRUE;
}

BOOL Read_Error_Bit_Ex1(int channel, unsigned int *reErroCode,double *ERR_Ratio)
{
	int		Lock, error, sBERT_Status_Code; 
	char 	sBERT_Status_String[500];
	int ErroCode;
	double 	sBERT_ERR_Number, sBERT_ERR_Ratio, sBERT_ElapsedTime;  

	switch (my_struInstrument[channel].BERT_TYPE)
	{
		case BERT_TYPE_GBERT:
			error= GBERT_Check_Ratio(INST_BERT[channel],&sBERT_ERR_Ratio,&ErroCode);
			if (error) 
			{
				return error;
			}
			*reErroCode =  ErroCode;
			break;
		case BERT_TYPE_SBERT: 
			error = EVB5_SBERT_Read(INST_BERT[channel], SBERT_R_R2488Mbps, SBERT_P_PRBS7, &sBERT_Status_Code, sBERT_Status_String, &sBERT_ERR_Number, &sBERT_ERR_Ratio, &sBERT_ElapsedTime); 
			if (error<0 )
			{
			//	*reErroCode =  sBERT_Status_Code; 
			//	*ERR_Ratio = sBERT_ERR_Ratio;
				return FALSE; 
			} 
			*reErroCode =  sBERT_Status_Code;  
			*ERR_Ratio = sBERT_ERR_Ratio;
			break;
		default:
			{
				MessagePopup("Error", "BERT type error!");
				return FALSE;
			} 
	}
	return TRUE;
}

BOOL Read_Bert_ERR_Ratio_1G(int channel, unsigned int *reErroCode,double *ERR_Ratio)
{
	int		Lock, error, sBERT_Status_Code; 
	char 	sBERT_Status_String[500];
	int ErroCode;
	double 	sBERT_ERR_Number, sBERT_ERR_Ratio, sBERT_ElapsedTime;  
	int i,Sync;
	int	InstantSD, InstantSyns; 
	unsigned long long InstantErrNum;
	double	InstantElapsedTime,ErrorBit; 
	char   	BERT_Status_String[500]; 
	double  ErrorRateTemp=0;
	double  ErrorRateAvg=0;  

	switch (my_struInstrument[channel].BERT_TYPE)
	{
		case BERT_TYPE_GBERT:
			{
				error= GBERT_Check_Ratio(INST_BERT[channel],&sBERT_ERR_Ratio,&ErroCode);
				if (!error) 
				{
					return FALSE;
				}
				*reErroCode =  ErroCode;
				break;
			}
		case BERT_TYPE_SBERT: 
			{
				error = EVB5_SBERT_Read(INST_BERT[channel], SBERT_R_R2488Mbps, SBERT_P_PRBS23, &sBERT_Status_Code, sBERT_Status_String, &sBERT_ERR_Number, &sBERT_ERR_Ratio, &sBERT_ElapsedTime); 
				if (error<0 )
				{
					return FALSE; 
				} 
				*reErroCode = sBERT_Status_Code;  
				*ERR_Ratio  = sBERT_ERR_Ratio;
				break;
			}
		default:
			{
				MessagePopup("Error", "BERT type error!");
				return FALSE;
			} 
	}
	return TRUE;
}
BOOL Read_Bert_ERR_Ratio_10G(int channel, unsigned int *reErroCode,double *ERR_Ratio)
{
	int		Lock, error, sBERT_Status_Code; 
	char 	sBERT_Status_String[500];
	int ErroCode;
	double 	sBERT_ERR_Number, sBERT_ERR_Ratio, sBERT_ElapsedTime;  
	int i,Sync;
	int	InstantSD, InstantSyns; 
	unsigned long long InstantErrNum;
	double	InstantElapsedTime,ErrorBit; 
	char   	BERT_Status_String[500]; 
	double  ErrorRateTemp=0;
	double  ErrorRateAvg=0;  

	switch (my_struInstrument[channel].BERT_TYPE_10G)
	{
		case BERT_TYPE_SBERT10000:
			{	
				for(i=0;i<3;i++)
				{
					error = evb16_SetBERTStart(INST_BERT_10G[channel], &Lock, &Sync, 1);
					if (error)
					{
						*ERR_Ratio=1; 
						return FALSE;
					}
					Delay (0.1);
					error = evb16_GetBERTStatus (INST_BERT_10G[channel], SBERT10G_SignalIn_SMA, &InstantSD, &InstantSyns, &InstantErrNum, &InstantElapsedTime, &ErrorRateTemp, BERT_Status_String);
					if (error) return -1;
					ErrorRateAvg=ErrorRateAvg+ErrorRateTemp;

				}
				*ERR_Ratio	= ErrorRateAvg/3;
			
				break;
			}
		case BERT_TYPE_GBERT10000:	
			{	
				for(i=0;i<3;i++)
				{			
					error = GBERT10000_Check_10G_ONU (INST_BERT_10G[channel], 0.1, &ErrorRateTemp, &ErrorBit);
					if (error) return -1;
					ErrorRateAvg=ErrorRateAvg+ErrorRateTemp;   
				}
				*ERR_Ratio=ErrorRateAvg/3;        
				break;
			}
		default:
			{
				MessagePopup("Error", "BERT type error!");
				return FALSE;
			} 
	}
	return TRUE;
}

BOOL testRapd(int channel)
{ 
	double Vapd;
	
	Vapd=10;
	switch (my_struInstrument[channel].SEVB_TYPE) 
	{
		case SEVB_TYPE_SEVB5:
			if (EVB5_READ_VAPDM_MON (INST_EVB[channel], &Vapd)<0)
			{
				return FALSE;
			}
			break;
		default:
			return FALSE;
			break;
	}
	if (Vapd>5) 
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL teststation(struct struTestData data, struct struProcessLOG prolog)
{
	if (DB_Search_Process_LOG_Ex(data, prolog)<0) 
	{
		return FALSE; 
	}

	return TRUE;
}
// 带纤灵敏度
BOOL testTheLoadSens(int channel, struct struTestData *data)
{
	
	double 	temp_sens, ber_temp;
	int		error,timer_star,timer;

	//切换光路
	my_struInstrument[channel].SW_TYPE = SW_TYPE_COFFSW02;
	if (SW_TYPE_NONE != my_struInstrument[channel].SW_TYPE)
	{
		//切换成一通道
		my_struInstrument[channel].SW_CHAN = 1;
		Fiber_SW_SetChannel(my_struInstrument[channel].SW_TYPE, my_struInstrument[channel].SW, my_struInstrument[channel].SW_Handle, my_struInstrument[channel].SW_CHAN);
	}
	
	//设置衰减模式
	if(GVPM_SET_LockEnable(INST_ATT[channel],0)<0)
	{
		return FALSE;
	}
	temp_sens = my_struDBConfig.Sens + 2 + sRxCal_Lock_TDP.power[channel];
	// 锁定过纤灵敏度功率
	error = Set_Att_Lock(channel, ATT_TYPE_GVPM, temp_sens);
	if (error)
	{
		return FALSE;
	}

	timer_star=Timer();
	
	if (!BERT_Start1(channel, my_struInstrument[channel].BERT_TYPE, my_struDBConfig.SensTime))	
	{
		return FALSE; 
	}
	
//	if (!BERT_Get_BER(channel, my_struInstrument[channel].BERT_TYPE, &ber_temp))   	
//	{
//		return FALSE; 
//	}
	error = Read_BER1(channel, my_struInstrument[channel].BERT_TYPE, my_struDBConfig.SensTime, &ber_temp,1);
	if (error<0) 
	{
		return FALSE;
	}   
		
	timer = Timer()- timer_star;  
	
	if(timer<my_struDBConfig.SensTime)
	{
		MessagePopup("Error", "Sen测试时间小于配置！");  
		return FALSE; 
	}
	if (BERT_TYPE_SBERT10000 == my_struInstrument[channel].BERT_TYPE)      
	{
		error = evb16_ModeSet(INST_BERT_10G[channel], REPEAT_MODE, 0);
		if (error)
		{
			MessagePopup("Error", "SBERT10000设置联系模式失败！"); 
			return FALSE; 
		}  
	}
	//判断测试结果,并赋值 
	if (ber_temp>0) 
	{
		data->Sens=0; 
		return FALSE;
	}
	temp_sens = my_struDBConfig.Sens + 2;
	data->Sens=temp_sens;
	data->SensTime=timer;  
	// 切换回二通道
	my_struInstrument[channel].SW_CHAN = 2;
	Fiber_SW_SetChannel(my_struInstrument[channel].SW_TYPE, my_struInstrument[channel].SW, my_struInstrument[channel].SW_Handle, my_struInstrument[channel].SW_CHAN);
	return TRUE;
}

// SFP 拷贝
BOOL testTheSens(int channel, struct struTestData *data)
{
	double 	temp_sens, ber_temp;
	int		error,timer_star,timer;
	
	//测灵敏度设置衰减模式
	if(GVPM_SET_LockEnable(INST_ATT[channel],0)<0)
	{
		return FALSE;
	}
	
	//切换光路
	my_struInstrument[channel].SW_TYPE = 4;
	if (SW_TYPE_NONE != my_struInstrument[channel].SW_TYPE)
	{
		//切换成一通道
		my_struInstrument[channel].SW_CHAN = 2;
		Fiber_SW_SetChannel(my_struInstrument[channel].SW_TYPE, my_struInstrument[channel].SW, my_struInstrument[channel].SW_Handle, my_struInstrument[channel].SW_CHAN);
	}

	//temp_sens = my_struDBConfig.Sens - 1 + my_struRxCal.RxCal_Power;   
	temp_sens = my_struDBConfig.Sens + sRxCal_Free.power[channel];  
	error = Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sens);    //设置灵敏度功率
	if (error)
	{
		return FALSE;
	}

	timer_star=Timer();
	
	if (!BERT_Start1(channel, my_struInstrument[channel].BERT_TYPE, my_struDBConfig.SensTime))	
	{
		return FALSE; 
	}
	
//	if (!BERT_Get_BER(channel, my_struInstrument[channel].BERT_TYPE, &ber_temp))   	
//	{
//		return FALSE; 
//	}
	error = Read_BER1(channel, my_struInstrument[channel].BERT_TYPE, my_struDBConfig.SensTime, &ber_temp,1);
	if (error<0) 
	{
		return FALSE;
	}   
		
	timer = Timer()- timer_star;  
	
	if(timer<my_struDBConfig.SensTime)
	{
		MessagePopup("Error", "Sen测试时间小于配置！");  
		return FALSE; 
	}
	if (BERT_TYPE_SBERT10000 == my_struInstrument[channel].BERT_TYPE)      
	{
		error = evb16_ModeSet(INST_BERT_10G[channel], REPEAT_MODE, 0);
		if (error)
		{
			MessagePopup("Error", "SBERT10000设置联系模式失败！"); 
			return FALSE; 
		}  
	}
	//判断测试结果,并赋值 
	if (ber_temp>0) 
	{
		data->Sens=0; 
		return FALSE;
	}

	data->Sens=my_struDBConfig.Sens;
	data->SensTime=timer;  

	return TRUE;
}

BOOL XGSPON_OLT_SET_RXP_MON_E(int handle)
{
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	strcpy(strInpt, "MCU_SET_MON_INFO(RXPOWER,E,0x0000,0x0000)");  
	SetCommand(handle, strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "输入命令出错!     "); 
		return FALSE;
	} 
	return TRUE;
}

BOOL XGSPON_OLT_SET_RXP_MON_F(int handle, INT16U wSlop, INT16U wOffset)
{
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt, "MCU_SET_MON_INFO(RXPOWER,F,0x%04X,0x%04X)", wSlop, wOffset);    
	SetCommand(handle, strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "输入命令出错!     "); 
		return FALSE;
	} 
	return TRUE;
}

BOOL XGSPON_OLT_MCU_GET_RxPOW(int handle, int *data)
{
	int wValue;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	strcpy(strInpt, "MCU_GET_ADC(5)");  
	SetCommand(handle, strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &wValue); 
	*data = wValue;
	return TRUE; 
}

BOOL XGSPON_OLT_MCU_Update_BASE0(int handle)
{
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	strcpy(strInpt, "MCU_UPDATE_FLASH(BASE,0)"); 	
	SetCommand(handle, strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "输入命令出错!     "); 
		return FALSE;
	} 
	return TRUE; 
}

//SFP 拷贝
BOOL caltestRxPower_Ex(int panel, int channel, struct struTestData *data)
{
	BOOL status;  
	int error;  
	char strInpt[256], strOupt[256], Info[256]; 
	int i, count, wValue;
	unsigned short ADC; 
	double 	power, dRxPow, RxPowMax;
	float temp_RxPow_mon;
	int adc_arrary[30];     
	
	double rx_array[30], adc_arr[30], output[30], k, b, mean; 
	INT16S Slope, Offset;   
	INT16U wSlop, wOffset;
	INT8U  lslop, mslop, loffset, moffset,ladc,madc;  
	

	//strcpy (Info, "序号   校准点（dBm）   采样值（HEX）");
	//Insert_Info(panel, gCtrl_BOX[channel], Info); 
	
	RxPowMax=0;
	data->RxPow_num = my_struDBConfig_Monitor.CaliNumber_Test;
	
	if(my_struDBConfig_ERCompens.RSSIMODE>0)
	{
		if(GVPM_SET_LockEnable(INST_ATT[channel],0)<0)   // 0是衰减模式 sRxCal_Free.power[channel]，1是锁定模式   sRxCal_Lock.power[channel]
		{
			return FALSE;
		}
	}
	
	if (!XGSPON_OLT_SET_RXP_MON_E(INST_EVB[channel]))	
	{
		return FALSE;
	}

	for (i=0; i<my_struDBConfig_Monitor.CaliNumber; i++)
	{
		if (my_struConfig.DT_Test_DWDM) 
		{
			break; 	
		}
		power = my_struDBConfig_Monitor.CaliPoint[i]+sRxCal_Free.power[channel];
		status = Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN, power); 
		if (status)
		{
			MessagePopup("Error", "设置衰减失败!"); 
			return FALSE;
		} 
		
		Delay(0.5);
		


		if (!XGSPON_OLT_MCU_GET_RxPOW(INST_EVB[channel], &adc_arrary[i]))	
		{
			return FALSE;  
		}

	
		rx_array[i] = pow(10, my_struDBConfig_Monitor.CaliPoint[i]/10)*10000;
		
		//	sprintf (Info, "序号	校准点（dBm）	采样值（HEX）", );
		sprintf (Info, "%02d:   %.2f   0x%04X", i+1, my_struDBConfig_Monitor.CaliPoint[i], adc_arrary[i]); 
		Insert_Info(panel, gCtrl_BOX[channel], Info);
		ProcessDrawEvents ();  
	}
	
	//判断ADC采样的单调性
	for (i=0; i<my_struDBConfig_Monitor.CaliNumber-1; i++)
	{
		if (my_struConfig.DT_Test_DWDM) 
		{
			break; 	
		}
		if ((adc_arrary[i]-adc_arrary[i+1]) >=0)	  /***极性与原来代码相反，调试中发现光越小，收端采样值越小，已跟East确认无误**Eric.Yao**20130705***/
		{
			MessagePopup ("ERROR", "Rx采样值单调性检查失败！   "); 
			data->ErrorCode=ERR_TUN_CAL_RX_MONOTONY; 
			return FALSE;
		} 	
	}
	
	for(i=0; i<my_struDBConfig_Monitor.CaliNumber; i++)
	{
		if (my_struConfig.DT_Test_DWDM) 
		{
			break; 	
		}
		adc_arr[i] = (double)adc_arrary[i];
	}

	if (my_struConfig.DT_Test_DWDM != TRUE ) 
	{
		LinFit(adc_arr, rx_array, my_struDBConfig_Monitor.CaliNumber, output, &k, &b, &mean);
		Slope = k * 256 ;
		Offset = b * 1;
		wSlop = Slope;
		wOffset = Offset;

		if (!XGSPON_OLT_SET_RXP_MON_F(INST_EVB[channel], wSlop, wOffset))
		{
			return FALSE;   
		}	


		if (!XGSPON_OLT_MCU_Update_BASE0(INST_EVB[channel]))
		{
			return FALSE; 
		}
			
	}
	
	strcpy (Info, "序号   校准点（dBm）   采样值（dBm）");
	Insert_Info(panel, gCtrl_BOX[channel], Info);
	
	for (i=0; i<my_struDBConfig_Monitor.CaliNumber_Test; i++) 
	{
		power = my_struDBConfig_Monitor.CaliPoint_Test[i]+sRxCal_Lock.power[channel];     
		status = Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN, power);   
		if (status)
		{
			MessagePopup ("提示", "设置校准功率点异常"); 
			return FALSE; 
		}     
		
		Delay(1);  
		
//		GetRxpower(&my_struTestData.RxPow_mon[i]);  
		//error = MyDLL_8472_GetRxPWR_EED (INST_EVB[channel], &temp_RxPow_mon, my_struDBConfig_E2.A2); 
		error = MyDLL_8472_GetRxPWR( INST_EVB[channel], &temp_RxPow_mon);
		if (error) 
		{
			return FALSE; 
		}
		
		if(temp_RxPow_mon<-90.)
		{
			temp_RxPow_mon = -90.;
		}
			
		data->RxPow_mon[i]=(double)temp_RxPow_mon;
		dRxPow = fabs(data->RxPow_mon[i] - my_struDBConfig_Monitor.CaliPoint_Test[i]);         
		
		if (dRxPow>my_struDBConfig_Monitor.Rx_Pow_Prec)  //采样值超出测试指标，重复测试5次，直到通过测试
		{
			count=0;
			do
			{
			//	if (!GetRxpower(INST_EVB[channel], &data->RxPow_mon[i]))
			//	{
			//		return FALSE;    
			//	}
				
				error = MyDLL_8472_GetRxPWR_EED (INST_EVB[channel], &temp_RxPow_mon, my_struDBConfig_E2.A2); 
				if (error) 
				{
					return FALSE; 
				}
				
				if(temp_RxPow_mon<-90.)
				{
					temp_RxPow_mon = -90.;
				}
				
				dRxPow = fabs(data->RxPow_mon[i] - my_struDBConfig_Monitor.CaliPoint_Test[i]);    
				if (dRxPow<=my_struDBConfig_Monitor.Rx_Pow_Prec)
				{
					break;
				}
				
				count++;
				ProcessDrawEvents ();  
				Delay(0.1);
			} while(count<5);
			
			if (count >= 5)
			{ 
				MessagePopup ("Error", "RxCal test Error  !");
				return FALSE;   
			}
		} 
		RxPowMax = dRxPow>RxPowMax ? dRxPow:RxPowMax; 
		
		sprintf (Info, "%02d:   %.2f   %.2f", i+1, my_struDBConfig_Monitor.CaliPoint_Test[i], data->RxPow_mon[i]+sRxCal_Free.power[channel]); 
		Insert_Info(panel, gCtrl_BOX[channel], Info);    
		ProcessDrawEvents ();  
	}
	
	//由数据库配置参数决定测试指标
	if (RxPowMax>my_struDBConfig_Monitor.Rx_Pow_Prec) 
	{
		return FALSE;   
	}
	
	return TRUE; 
}

BOOL cal_RxPower10G(int panel, int channel, struct struTestData *data)
{
	int     i, j, count, error, k;
	float 	tempRxPow, temper;
	int 	RxPow_mon_iW[30], sum_adc; 
	float  PWR_Array[30];   
	INT16S  Slope, Offset;   
	INT16U  wSlop, wOffset;
	float 	RxPow, RxPowMax, RxPow_mon_DB[30];
	INT32U 	Mode=0;
	char    Info[256];
	unsigned short adc, adc_arr[5];
	short	ADC_Array[30],  ADC_Test_Array[30];
	

	
	error = SET_10G_RXP_MON(INST_EVB[channel], LUT_MODE_EXT);
	if (error)
	{
		goto Error;   
	}
	
	strcpy (Info, "序号   校准点（dBm）   采样值（HEX）");
	Insert_Info(panel, gCtrl_BOX[channel], Info);         

	//开始校准
	for (i=0; i<my_struDBConfig_Monitor.CaliNumber; i++) 
	{
		if(i==(my_struDBConfig_Monitor.CaliNumber-1)) 
		{
			my_struDBConfig_Monitor.CaliPoint[i] = -35.;  
		}

		error = Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN , my_struDBConfig_Monitor.CaliPoint[i]+sRxCal_Free.power[channel]); 
		if (error) 
		{
			MessagePopup ("ERROR", "GVPM Set Att error!"); 
			goto Error;
		}
		
		if (-18.0 > my_struDBConfig_Monitor.CaliPoint[i])
		{
			sum_adc = 0;
			for (k = 0; k<3; k++)
			{

				error = GET_10G_RX_ADC (INST_EVB[channel], &adc_arr[k]);
				if(error!=0)
				{
					goto Error;
				}
				
				sum_adc = sum_adc + adc_arr[k];    
			}
			ADC_Array[i] = (int)(sum_adc / 3);
		}
		else
		{
			error = GET_10G_RX_ADC (INST_EVB[channel], &ADC_Array[i]);    
			if(error!=0)
			{
				goto Error;
			}
		}

		sprintf (Info, "%02d:   %.2f   0x%03X", i+1, my_struDBConfig_Monitor.CaliPoint[i], ADC_Array[i]); 
		Insert_Info(panel, gCtrl_BOX[channel], Info);         
	}
	
	//判断ADC采样的单调性
	for (i=0; i<my_struDBConfig_Monitor.CaliNumber-1; i++)
	{
		if ((ADC_Array[i]-ADC_Array[i+1]) >= 0)
		{
			if ((i+1) != (my_struDBConfig_Monitor.CaliNumber-1))
			{
				MessagePopup ("ERROR", "Rx采样值单调性检查失败！   "); 
				data->ErrorCode=ERR_TUN_CAL_RX_MONOTONY; 
				goto Error;
			} 
			else
			{
				if (ADC_Array[i] > ADC_Array[i+1])
				{
					ADC_Array[i+1] = ADC_Array[i];	
				}
			}
		}
	}
	
	error = SET_RXPOW_TABLE(INST_EVB[channel], ADC_Array, my_struDBConfig_Monitor.CaliPoint, my_struDBConfig_Monitor.CaliNumber);
	if (error)
	{
		goto Error;
	}
	
	//添加测试流程 
	strcpy (Info, "收端校准系数写入后测试");
	Insert_Info(panel, gCtrl_BOX[channel], Info);         
	
	strcpy (Info, "序号   校准点（dBm）   采样值（dBm）");
	Insert_Info(panel, gCtrl_BOX[channel], Info);         
	
	data->RxPow_num = my_struDBConfig_Monitor.CaliNumber_Test;
	
	RxPowMax=0; 
	for (i=0; i<my_struDBConfig_Monitor.CaliNumber_Test; i++) 
	{
		error = Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN , my_struDBConfig_Monitor.CaliPoint_Test[i]+sRxCal_Free.power[channel]);
		if (error)
		{
			MessagePopup ("提示", "设置校准功率点异常"); 
			goto Error;
		}
		
	  	//设置trigger  
		error = SET_EVB_Light_BEN_Trig_Gen (channel);
		if (error)
		{
			MessagePopup ("Error", "设置光源RSSI触发失败"); 
			goto Error;
		}   
		
		Delay(0.3);
		
		error = GET_10G_RxMon (INST_EVB[channel], &RxPow_mon_DB[i], my_struConfig.Rx_unit); 
		if (error)
		{
			goto Error;                	
		}
		
		error = GET_10G_RX_ADC(INST_EVB[channel], &ADC_Test_Array[i]);
		if (error)
		{
			goto Error;     
		}
		
		RxPow = fabs(RxPow_mon_DB[i]-my_struDBConfig_Monitor.CaliPoint_Test[i]);
		
		if (RxPow>my_struDBConfig_Monitor.Rx_Pow_Prec)  //采样值超出测试指标，重复测试5次，直到通过测试
		{
			count=0;
			do
			{
				error = Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN , my_struDBConfig_Monitor.CaliPoint_Test[i]+sRxCal_Free.power[channel]);  
				if (error) 
				{
					MessagePopup ("提示", "设置校准功率点异常"); 
					goto Error;
				}
		
			   	//设置trigger  
				error = SET_EVB_Light_BEN_Trig_Gen (channel);
				if (error)
				{
					MessagePopup ("Error", "设置光源RSSI触发失败"); 
					goto Error;
				}   
				
				Delay(0.3);
				error = GET_10G_RxMon (INST_EVB[channel], &RxPow_mon_DB[i], my_struConfig.Rx_unit); 
				if (error)
				{
					goto Error;     
				}
		
				RxPow = fabs(RxPow_mon_DB[i]-my_struDBConfig_Monitor.CaliPoint_Test[i]);
				if (RxPow<=my_struDBConfig_Monitor.Rx_Pow_Prec)
				{
					break;
				}
				
				count++;
				Delay(0.1);
			} while(count<5);
			
			if (count >=5 )
			{
				sprintf (Info, "%02d:   %.2f   %.2f", i+1, my_struDBConfig_Monitor.CaliPoint_Test[i], RxPow_mon_DB[i]); 
				Insert_Info(panel, gCtrl_BOX[channel], Info);  
				MessagePopup ("提示", "模块写入校准系数后光功率监控值检查失败"); 
				goto Error;
			}
		} 
		
		RxPowMax = RxPow>RxPowMax ? RxPow:RxPowMax; //将测试误差最大值更新
		
		sprintf (Info, "%02d:   %.2f   %.2f", i+1, my_struDBConfig_Monitor.CaliPoint_Test[i], RxPow_mon_DB[i]); 
		Insert_Info(panel, gCtrl_BOX[channel], Info);         
	}
	
	//由数据库配置参数决定测试指标
	if (RxPowMax>my_struDBConfig_Monitor.Rx_Pow_Prec) 
	{
		goto Error;  
	}

//set manual mode
	if (my_struDBConfig_ERCompens.RSSIMODE>0) 
	{
		error = SET_EVB_Light_MODE_Level (channel);
		if (error)
		{
			MessagePopup ("Error", "设置光源常规模式失败"); 
			return FALSE;
		}   
		
		error = Set_Att_Lock_Enable(channel);
		if (error)        
		{
			return FALSE;    
		}
	}
	
	return TRUE;
	
Error:
//set manual mode 
	if (my_struDBConfig_ERCompens.RSSIMODE>0) 
	{
		if (SET_EVB_Light_MODE_Level (channel)<0) 																
		{
			MessagePopup ("Error", "设置光源常规模式失败"); 
			return FALSE;
		} 
		
		if (Set_Att_Lock_Enable(channel)<0) 
		{
			return FALSE;    
		}
	}
	
	return FALSE;
}
 
BOOL caltest_RxPower10G(int panel, int channel, struct struTestData *data)
{
	int 	i, count, VAPDEN, error;
	float 	dRxPow, RxPowMax, RxPow_mon_mW[30], RxPow_mon_DB[30], delta, delta_Max, RxPow;
	BYTE 	RxMode;
	char    Info[256];  

	int    RSII_Compensate_Flag = 0;

	float K_Max=5;
	float K_Min=-5;
	float K_temp;
	float RSSI_Power = -30.;
	int RSSI_ADC_COUNT_MAX = 3; 
	int index;  
	BOOL RSSItunFlag=FALSE;
	
	
	//检查模块Rx_Mode
	if (my_struDBConfig_ERCompens.RSSIMODE>0)    
	{
		error = SET_EVB_Light_MODE_RSSI (channel); 
		if (error<0)   
		{
			MessagePopup ("Error", "设置光源到RSSI模式失败"); 
			goto Error;
		} 
		
		error = Set_Att_Lock_Disable (channel);                 
		if (error<0) 
		{
			goto Error;     
		}
	}
	
	strcpy (Info, "序号   校准点（dBm）   采样值（dBm）");
	Insert_Info(panel, gCtrl_BOX[channel], Info);
	
	//RSII校准不准，需要在测试时给出补偿值	
	if((my_struConfig.Temper_Low || my_struConfig.Temper_High) && my_struConfig.DT_Test_10G)
	{  
		error = Set_Att (channel, my_struInstrument[channel].ATT_TYPE_MAIN, RSSI_Power+sRxCal_Free.power[channel]);
		if (error) 
		{
			MessagePopup ("提示", "设置校准功率点异常"); 
			goto Error;
		}
   	
		for(index=0; index<RSSI_ADC_COUNT_MAX; index++)
		{
			if (my_struDBConfig_ERCompens.RSSIMODE>0)
			{   
				//设置trigger  
				error = SET_EVB_Light_BEN_Trig_Gen (channel);      
				if (error<0) 
				{
					 MessagePopup ("Error", "设置光源RSSI触发失败"); 
					goto Error;
				} 
	 
				error = GET_10G_RxMon (INST_EVB[channel], &RxPow_mon_DB[0], my_struConfig.Rx_unit); 
				if (error)
				{
					goto Error;                	
				}
			}
			else
			{	//for DDMI
				Delay(0.3); //DDMI必须添加  
				error = GET_10G_RxMon (INST_EVB[channel], &RxPow_mon_DB[0], my_struConfig.Rx_unit); 
				if (error)
				{
					goto Error;                	
				}
			}
   
			if(fabs(RxPow_mon_DB[0] - RSSI_Power) > 1.0)  
			{
				RSII_Compensate_Flag = 1; 				/***精度超过2dB，需要微调温补系数**Eric.Yao**20150407***/ 
				break;
			} 
		} 
	}

	if(RSII_Compensate_Flag)	 /***微调温补系数**Eric.Yao**20150407***/
	{
		K_temp=(K_Max+K_Min)/2; 
		if(my_struConfig.Temper_Low)
		{
			error = SET_10G_RXKL(INST_EVB[channel], K_temp);
			if(error<0)	
			{
				goto Error;   
			}
		}
		
		if(my_struConfig.Temper_High)
		{
			error = SET_10G_RXKH(INST_EVB[channel], K_temp);
			if(error<0)
			{
				return FALSE;
			}
		}
		
		if (my_struDBConfig_ERCompens.RSSIMODE>0)
		{   
			//设置trigger  
			error = SET_EVB_Light_BEN_Trig_Gen (channel);      
			if (error<0) 
			{
				MessagePopup ("Error", "设置光源RSSI触发失败"); 
				goto Error;
			}
			
			error = GET_10G_RxMon (INST_EVB[channel], &RxPow_mon_DB[0], my_struConfig.Rx_unit); 
			if (error)
			{
				goto Error;                	
			}
		}
		else
		{	
			//for DDMI
			Delay(0.3); //DDMI必须添加  
			error = GET_10G_RxMon (INST_EVB[channel], &RxPow_mon_DB[0], my_struConfig.Rx_unit); 
			if (error)
			{
				goto Error;                	
			}
		}
		do
		{
			if((RxPow_mon_DB[0] - RSSI_Power)> 0.5)
			{
				if(K_Max-K_Min<0.0001)   
				{
					goto Error;
				}
				if(my_struConfig.Temper_Low)
				{
					K_Max=K_temp;
				}
				else  
				{
					K_Min=K_temp; 
				}
				K_temp=(K_Max+K_Min)/2;
			}
			else if((RxPow_mon_DB[0] - RSSI_Power)<-0.5)
			{
				if(K_Max-K_Min<0.0001)  
				{
					goto Error; 
				}
				if(my_struConfig.Temper_Low) 
				{
					K_Min=K_temp;
				}
				else 
				{
					K_Max=K_temp;
				}
				K_temp=(K_Max+K_Min)/2;
			}
			else
			{
				RSSItunFlag=TRUE;
				break;
			}
			
			if(my_struConfig.Temper_Low)
			{
				error = SET_10G_RXKL(INST_EVB[channel], K_temp);
				if(error<0)	
				{
					goto Error;   
				}
			}
			
			if(my_struConfig.Temper_High)
			{
				error = SET_10G_RXKH(INST_EVB[channel], K_temp);
				if(error<0)	
				{
					goto Error;   
				}
			}
			
			if (my_struDBConfig_ERCompens.RSSIMODE>0)
			{   
				//设置trigger  
				error = SET_EVB_Light_BEN_Trig_Gen (channel);      
				if (error<0) 
				{
					MessagePopup ("Error", "设置光源RSSI触发失败"); 
					goto Error;
				}
				
				error = GET_10G_RxMon (INST_EVB[channel], &RxPow_mon_DB[0], my_struConfig.Rx_unit); 
				if (error)
				{
					goto Error;                	
				}
			}
			else
			{
				//for DDMI
				Delay(0.3); //DDMI必须添加  
				error = GET_10G_RxMon (INST_EVB[channel], &RxPow_mon_DB[0], my_struConfig.Rx_unit); 
				if (error)
				{
					goto Error;                	
				}
			}
		}while(!RSSItunFlag);
		
		error = Update_BASE(INST_EVB[channel], 0);
		if (error<0) 	 
		{
			goto Error;   
		}
		data->RSSI_Compensation=K_temp; 		
	}

	RxPowMax=0; 
	data->RxPow_num = my_struDBConfig_Monitor.CaliNumber_Test;   
	
	for (i=0; i<my_struDBConfig_Monitor.CaliNumber_Test; i++) 
	{
		error = Set_Att (channel, my_struInstrument[channel].ATT_TYPE_MAIN, my_struDBConfig_Monitor.CaliPoint_Test[i]+sRxCal_Free.power[channel]);
		if (error) 
		{
			MessagePopup ("提示", "设置校准功率点异常"); 
			goto Error;
		}
		
		if (my_struDBConfig_ERCompens.RSSIMODE>0)
		{   
			//设置trigger  
			error = SET_EVB_Light_BEN_Trig_Gen (channel);      
			if (error<0) 
			{
				MessagePopup ("Error", "设置光源RSSI触发失败"); 
				goto Error;
			}
			
			error = GET_10G_RxMon (INST_EVB[channel], &RxPow_mon_DB[i], my_struConfig.Rx_unit); 
			if (error)
			{
				goto Error;                	
			}
		}
		else
		{	
			//for DDMI
			Delay(0.3); //DDMI必须添加  
			error = GET_10G_RxMon (INST_EVB[channel], &RxPow_mon_DB[i], my_struConfig.Rx_unit); 
			if (error)
			{
				goto Error;                	
			}
		}  
		data->RxPow_mon[i] = RxPow_mon_DB[i];
		
		RxPow = fabs(RxPow_mon_DB[i]-my_struDBConfig_Monitor.CaliPoint_Test[i]);
		
		if (RxPow>my_struDBConfig_Monitor.Rx_Pow_Prec)  //采样值超出测试指标，重复测试5次，直到通过测试
		{
			count=0;
			do
			{
				error = Set_Att (channel, my_struInstrument[channel].ATT_TYPE_MAIN, my_struDBConfig_Monitor.CaliPoint_Test[i]+sRxCal_Free.power[channel]);
				if (error) 
				{
					MessagePopup ("提示", "设置校准功率点异常"); 
					goto Error;
				}
				
				if (my_struDBConfig_ERCompens.RSSIMODE>0)
				{   
					//设置trigger  
					//设置trigger  
					error = SET_EVB_Light_BEN_Trig_Gen (channel);      
					if (error<0) 
					{
						MessagePopup ("Error", "设置光源RSSI触发失败"); 
						goto Error;
					} 
					
					error = GET_10G_RxMon (INST_EVB[channel], &RxPow_mon_DB[i], my_struConfig.Rx_unit); 
					if (error)
					{
						goto Error;                	
					}
				}
				else
				{	
					//for DDMI
					Delay(0.3); //DDMI必须添加 
					error = GET_10G_RxMon (INST_EVB[channel], &RxPow_mon_DB[i], my_struConfig.Rx_unit); 
					if (error)
					{
						goto Error;                	
					}
				}				

				RxPow = fabs(RxPow_mon_DB[i]-my_struDBConfig_Monitor.CaliPoint_Test[i]);
				if (RxPow<=my_struDBConfig_Monitor.Rx_Pow_Prec)
				{
					break;
				}
				
				count++;
				Delay(0.1);
			} while(count<5);
			
			if (count >=5 )
			{
				MessagePopup ("提示", "光功率监控值检查失败");
				goto Error; 
			}
		} 
		RxPowMax = RxPow>RxPowMax ? RxPow:RxPowMax; //将测试误差最大值更新
		
		sprintf (Info, "%02d:   %.2f   %.2f", i+1, my_struDBConfig_Monitor.CaliPoint_Test[i], RxPow_mon_DB[i]); 
		Insert_Info(panel, gCtrl_BOX[channel], Info);   
	}
	
	//由数据库配置参数决定测试指标
	if (RxPowMax>my_struDBConfig_Monitor.Rx_Pow_Prec) 
	{
		goto Error;   
	}
	
	//set manual mode
	if (my_struDBConfig_ERCompens.RSSIMODE>0) 
	{
		error = SET_EVB_Light_MODE_Level (channel);
		if (error<0) 
		{
			MessagePopup ("Error", "设置光源常规模式失败"); 
			return FALSE;
		}   
		
		error = Set_Att_Lock_Enable(channel);
		if (error<0) 
		{
			return FALSE;  
		}
	}
	
	return TRUE;
 
Error:
	//set manual mode 
	if (my_struDBConfig_ERCompens.RSSIMODE>0) 
	{
		error = SET_EVB_Light_MODE_Level (channel);
		if (error<0) 
		{
			MessagePopup ("Error", "设置光源常规模式失败"); 
			return FALSE;
		}  
		
		error = Set_Att_Lock_Enable(channel);
		if (error<0) 
		{
			return FALSE;  
		}
	} 
	
	return FALSE;
}

int short_circuit_check (int channel, struct struTestData *data)
{
	int error, errorcode;
	char buf[50]="";
	
	errorcode=0;
	error = EVB5_GET_R7_SC (INST_EVB[channel], &errorcode, buf);
	if (error)
	{
		return -1;	
	}
	
	switch (errorcode)
	{
		case 0: //no error
			;
			break;
			
		case -1: //gnd
			MessagePopup("提示", buf);
			data->ErrorCode=ERR_EEP_SHORT_GND;  
			return -1;
			
			break;
		
		case -2: //Vapd
			MessagePopup("提示", buf);
			data->ErrorCode=ERR_EEP_SHORT_VAPD;  
			return -1;
			
			break;
			
		case -3: //Vcc
			MessagePopup("提示", buf);
			data->ErrorCode=ERR_EEP_SHORT_VCC; 
			return -1;
			
			break;
			
		default:
			data->ErrorCode=ERR_EEP_NO_DEFINE; 
			return -1; 
			break;
	}
	
	return 0;
}

int	testDetailSens_1G (int channel, struct struTestData *data)
{
	int  error, err_count;
	BYTE SD;
	double 	temp_sens, temp_ref, stepdown=0.5, stepup=0.2, timer_star, timer;
	
    //设置初始光功率, 以数据库中的测试指标为起始点
	temp_sens = my_struDBConfig.Sens+2.5+sRxCal_Lock.power[channel]; 
	
	error = Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sens);	  	//设置灵敏度功率 
	if (error)
	{
		goto Error; 	
	}
	Delay(1);
	//BERT开始计数 
	error = BERT_Start (channel, my_struDBConfig.SensTime, 1, 0);
	if (error)
	{
		goto Error; 	
	}
	
	//读取误码状态，如果出现误码，直接退出 
	Delay(0.3); 
	if (!Read_Error_Bit_Ex(channel, &err_count, 3)) 
	{
		//出现误码后重新在检查一遍
		error = BERT_Stop (channel); 
		if (error)
		{
			goto Error; 	
		}
		
		Delay(0.3); 
	
		error = BERT_Start (channel, my_struDBConfig.SensTime, 1, 0);
		if (error)
		{
			goto Error; 	
		}
		
		Delay(0.5);
		
		if (!Read_Error_Bit_Ex(channel, &err_count, 3))
		{
			MessagePopup("提示", "灵敏度开始测试时出现误码"); 
			goto Error;
		}
	}
	
	//此时的光功率=my_struRxCal_OLT.RxCal_Power_Max，不能出现误码 
	if (err_count>0) 
	{
		goto Error; 	
	}

	//设置光功率到刚好不出现误码 
	do 
	{
		temp_sens = temp_sens - stepdown;  
		//Set_Att(my_struInstrument.ATT_TYPE, temp_sens);//设置灵敏度功率  
		error = Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sens);	  	//设置灵敏度功率 
		if (error)
		{
			goto Error; 	
		}
		
		error = BERT_Stop (channel); 
		if (error)
		{
			goto Error; 	
		}
		
		Delay(0.3); 
	
		error = BERT_Start (channel, my_struDBConfig.SensTime, 1, 0);
		if (error)
		{
			goto Error; 	
		}

		Delay(0.5);
		
		Read_Error_Bit_Ex(channel, &err_count, 3);    

	} while(err_count==0);     
	
	//调整衰减，直到没有误码为止 
	do 
	{
	 	temp_sens = temp_sens + stepup;  
		
		//Set_Att(my_struInstrument.ATT_TYPE, temp_sens);
		error = Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sens);	  	//设置灵敏度功率 
		if (error)
		{
			goto Error; 	
		}
		
		error = BERT_Stop (channel); 
		if (error)
		{
			goto Error; 	
		}
		
		Delay(0.3); 
	
		error = BERT_Start (channel, my_struDBConfig.SensTime, 1, 0);
		if (error)
		{
			goto Error; 	
		}

		Delay(0.5);
		
		Read_Error_Bit_Ex(channel, &err_count, 3);    
	
	} while (err_count != 0);
	
	//测试足够时间没有误码 
	error = BERT_Start (channel, my_struDBConfig.SensTime, 1, 0);
	if (error)
	{
		goto Error; 	
	}
	
	do
	{
		timer_star=Timer(); 
		do
		{
			//读取误码
			Delay(1); 
			Read_Error_Bit_Ex(channel, &err_count, 3);
			timer = Timer()- timer_star; 
		} while (timer<my_struDBConfig.SensTime && err_count==0);
	
		if(err_count != 0)
	    {
		    temp_sens = temp_sens + stepup;  
			//Set_Att(my_struInstrument.ATT_TYPE, temp_sens);
			error = Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sens);	  	//设置灵敏度功率 
			if (error)
			{
				goto Error; 	
			}
			
			Delay(0.5); 
			
			error = BERT_Stop (channel); 
			if (error)
			{
				goto Error; 	
			}
		
			Delay(0.3); 
	
			error = BERT_Start (channel, my_struDBConfig.SensTime, 1, 0);
			if (error)
			{
				goto Error; 	
			}
	   	}
	}while (err_count>0); 
	
	//关闭BRET
	error = BERT_Stop (channel); 
	if (error) 
	{
		return -1;
	}
	
	data->Sens = temp_sens-sRxCal_Lock.power[channel]; 
	data->SensTime=timer;
	
	if (data->Sens > my_struDBConfig.Sens) 
	{
		return -1;
	}
	
	return 0;
	
Error:
	error = BERT_Stop (channel);

	return -1;
}

int	testDetailSens_10G (int channel, struct struTestData *data)
{
	int  error;
	unsigned long long 	err_count;  
	double err_rate;
	BYTE SD;
	double 	temp_sens, temp_ref, stepdown=0.5, stepup=0.2, timer_star, timer;
	
	//设置衰减模式
	if(GVPM_SET_LockEnable(INST_ATT[channel],1)<0)
	{
		return FALSE;
	}
	
    //设置初始光功率, 以数据库中的测试指标为起始点
	temp_sens = my_struDBConfig.Sens+2.5+sRxCal_Lock.power[channel]; 
	
	error = Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sens);	  	//设置灵敏度功率 
	if (error)
	{
		goto Error; 	
	}
	Delay(1);
	//BERT开始计数 
	error = BERT_10G_Start (channel, my_struDBConfig.SensTime);
	if (error)
	{
		goto Error; 	
	}
	
	//读取误码状态，如果出现误码，直接退出 
	Delay(0.3); 
	
	if (!Read_Error_Bit_10G(channel, &err_count, &err_rate, my_struDBConfig.SensTime)) 
	{  
		Delay(0.3); 
	
		error = BERT_10G_Start (channel, my_struDBConfig.SensTime); 
		if (error)
		{
			goto Error; 	
		}
		
		Delay(0.5);
		
		if (!Read_Error_Bit_10G(channel, &err_count, &err_rate, my_struDBConfig.SensTime))  
		{
			MessagePopup("提示", "灵敏度开始测试时出现误码"); 
			goto Error;
		}
	}

	//设置光功率到刚好误码>1.0E-3 
	do 
	{
		temp_sens = temp_sens - stepdown;  
		//Set_Att(my_struInstrument.ATT_TYPE, temp_sens);//设置灵敏度功率  
		error = Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sens);	  	//设置灵敏度功率 
		if (error)
		{
			goto Error; 	
		}
	
		Delay(0.5);
		
		if (!Read_Error_Bit_10G(channel, &err_count, &err_rate, my_struDBConfig.SensTime))     
		if (error)
		{
			goto Error; 	
		}
	} while(err_rate < 1.0E-3);     
	
	//调整衰减，直到误码<1.0E-3 
	do 
	{
	 	temp_sens = temp_sens + stepup;  
		
		//Set_Att(my_struInstrument.ATT_TYPE, temp_sens);
		error = Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sens);	  	//设置灵敏度功率 
		if (error)
		{
			goto Error; 	
		}
		
		Delay(0.5);
		
		if (!Read_Error_Bit_10G(channel, &err_count, &err_rate, my_struDBConfig.SensTime))     
		{
			goto Error; 	
		}
	} while (err_rate > 1.0E-3);
	
	//测试足够时间没有误码 
	error = BERT_10G_Start (channel, my_struDBConfig.SensTime); 
	if (error)
	{
		goto Error; 	
	}

	do
	{
		timer_star=Timer(); 
		do
		{
			//读取误码
			Delay(1); 
			if (!Read_Error_Bit_10G(channel, &err_count, &err_rate, my_struDBConfig.SensTime))     
			{
				goto Error; 	
			}
			timer = Timer()- timer_star; 
		} while (timer<my_struDBConfig.SensTime && err_rate < 1.0E-3);
	
		if( err_rate > 1.0E-3)
	    {
		    temp_sens = temp_sens + stepup;  
			//Set_Att(my_struInstrument.ATT_TYPE, temp_sens);
			error = Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sens);	  	//设置灵敏度功率 
			if (error)
			{
				goto Error; 	
			}
			
			Delay(0.5); 
	
			error = BERT_10G_Start (channel, my_struDBConfig.SensTime);  
			if (error)
			{
				goto Error; 	
			}
	   	}
	}while (err_rate > 1.0E-3); 
	

	data->Sens = temp_sens-sRxCal_Lock.power[channel]; 
	data->SensTime=timer;
	
	if (data->Sens > my_struDBConfig.Sens) 
	{
		return -1;
	}
	
	return 0;
	
Error:
	return -1;
}
// SFP
int Read_BER1(int channel, int BERTType, double GatingTime, double *ErrorRate,int readtype)	  //readtype为1 判断为测误码率
{
	int error;
	int	InstantSD, InstantSyns; 
	unsigned long long InstantErrNum;
	double	InstantElapsedTime, ErrorBit;
	char   	BERT_Status_String[500]; 
	int     Lock, Sync;     
	
	int     i=0;
	double  ErrorRateTemp=0;
	double  ErrorRateAvg=0;  
	BERTType = BERT_TYPE_SBERT10000;						   
	switch (BERTType) 
	{
		case BERT_TYPE_SBERT10000:
			
			for(i=0;i<3;i++)
			{
				error = evb16_SetBERTStart(INST_BERT_10G[channel], &Lock, &Sync,1);
				if (error)
				{
					*ErrorRate=1; 
					return 0;
				}
				Delay (GatingTime);
				error = evb16_GetBERTStatus (INST_BERT_10G[channel], SBERT10G_SignalIn_SMA, &InstantSD, &InstantSyns, &InstantErrNum, &InstantElapsedTime, &ErrorRateTemp, BERT_Status_String);
				if (error) 
				{
					return -1;
				}
				
				ErrorRateAvg=ErrorRateAvg+ErrorRateTemp;
				if(readtype==1)
				{
					*ErrorRate=ErrorRateAvg;
					return 0;
				}
			}
			*ErrorRate=ErrorRateAvg/3;
			
			break;
	
		case BERT_TYPE_GBERT10000:	
			
			for(i=0;i<3;i++)
			{			
				error = GBERT10000_Check_10G_ONU (INST_BERT[channel], GatingTime, &ErrorRateTemp, &ErrorBit);
				if (error)
				{
					return -1;
				}
				
				ErrorRateAvg=ErrorRateAvg+ErrorRateTemp;
				if(readtype==1)
				{
					*ErrorRate=ErrorRateAvg;
					return 0;
				}
			}
			*ErrorRate=ErrorRateAvg/3;        
			break;
			
		default: 
			MessagePopup("提示", "BERT类型调用异常"); 
			return -1;
			break;
	}
	
	return 0;
}


BOOL BERT_Start1(int channel, int BERTType, int settime)
{
	int  Lock, Sync, error;  
	// 暂时使用固定值  lwy
	BERTType = BERT_TYPE_SBERT10000;
	switch (BERTType)
	{
		case BERT_TYPE_TG1B1A:
			if (!TG1b1a_Start_Test(INST_BERT[channel])) 
			{
				MessagePopup ("ERROR", "TG1b1a start error!"); 
				return FALSE;
			}     
			Delay(settime);    
			
			break;
			
		case BERT_TYPE_SBERT10000:
		//	error = evb16_ModeSet(INST_BERT_10G[channel], SINGLE_MODE, settime); 
			error = evb16_ModeSet(INST_BERT_10G[channel], REPEAT_MODE, settime); 
			if (error)
			{
				MessagePopup("Error", "SBERT10000设置连续模式失败！");
				return FALSE;
			}  
			
			error = evb16_SetBERTStart(INST_BERT_10G[channel], &Lock, &Sync, 1);
			if (error) 
			{
				MessagePopup("提示","SBERT10000同步异常");
				return FALSE;
			} 
			break;
			
		default:
			{
				MessagePopup ("ERROR", "BERT Type error!"); 
				return FALSE;
			}
			break;
	}
	return TRUE;      
}

int BERT_Start (int channel, double time_test, int isTestErrbitFlag, int B2_BurstMode)
{
	int error, count, sBERT_Status_Code, Lock;
	int OptoBERT_Lock, OptoBERT_ErrorCount;  
	char 	sBERT_Status_String[500];
	double 	sBERT_ERR_Number, sBERT_ERR_Ratio, sBERT_ElapsedTime; 
	unsigned int InstantErrNum;  
	double  InstantErrTime, InstantErrRate;  
	int Sync;
	int MAXCOUNT = 6; 
	
	switch (my_struInstrument[channel].BERT_TYPE)
	{
		case BERT_TYPE_GBERT:
			{	
				if (!GBERT_Start(INST_BERT[channel], time_test))
				{
					MessagePopup("Error", "Reset GBERT error!");
					return -1;
				} 
			
				break;
			}
		case BERT_TYPE_SBERT: 
			{	
				count = 0;
				do
				{
					error = EVB5_SBERT_Init(INST_BERT[channel], SBERT_R_R2488Mbps, SBERT_P_PRBS23);
					if (error) 
					{
						MessagePopup("提示","sBERT设置工作模式失败"); 
						return -1;
					} 
			
					error = EVB5_SBERT_Start(INST_BERT[channel], SBERT_R_R2488Mbps, time_test);
					if (error)
					{
						MessagePopup("Error", "sBERT start test error"); 
						return -1;
					}
				
					Delay(0.2);
				
					error = EVB5_SBERT_Read(INST_BERT[channel], SBERT_R_R2488Mbps, SBERT_P_PRBS7, &sBERT_Status_Code, sBERT_Status_String, &sBERT_ERR_Number, &sBERT_ERR_Ratio, &sBERT_ElapsedTime); 
					if (error) 
					{
						MessagePopup("Error", "sBERT read data error");
						return -1;
					} 
				
					count++;
					Delay(0.3);
				
				} while (sBERT_Status_Code ==-2 && count<10);				// 不同步的情况下重复启动，其他错误直接返回
				if(sBERT_Status_Code==-2)  
				{
					return -1; 
				}
				break;
			}
		default: 
			{
				MessagePopup("Error", "can not find this BERT Type!"); 
				return -1;
			}
	}
	
	return 0;
}
	
int BERT_Stop (int channel)
{
 	switch (my_struInstrument[channel].BERT_TYPE)
	{
		case BERT_TYPE_GBERT:
			if (!GBERT_Stop(INST_BERT[channel])) 
			{
				MessagePopup("Error", "GBERT Set Stop Error!"); 
				return -1;
			}  			
			break;
			
		case BERT_TYPE_SBERT: 
			if (EVB5_SBERT_End(INST_BERT[channel])<0) 
			{
				MessagePopup("Error", "SBERT Set Stop Error!!"); 
				return -1;
			} 			
			break;
			
		default: 
			{
				MessagePopup("Error", "can not find this BERT Type!");
				return -1;
			}
	}
	
	return 0;
}

int BERT_Read(int channel, unsigned int *ErrorCount, int TestFlag)
{
int 	Lock, error, sBERT_Status_Code; 
char 	sBERT_Status_String[500];
double 	sBERT_ERR_Number, sBERT_ERR_Ratio, sBERT_ElapsedTime; 
unsigned int InstantErrNum; 
double  InstantErrTime, InstantErrRate;
	
	switch (my_struInstrument[channel].BERT_TYPE)
	{
		case BERT_TYPE_GBERT:
			if (TestFlag==0)
			{
				if (!GBERT_Check_Start(INST_BERT[channel], ErrorCount)) 
				{
					return -1;    
				}
			}
			else
			{
				if (!GBERT_Check(INST_BERT[channel], ErrorCount))
				{
					return -1;    
				}
			}			
			break;
			
		case BERT_TYPE_SBERT: 
			error = EVB5_SBERT_Read(INST_BERT[channel], SBERT_R_R2488Mbps, SBERT_P_PRBS7, &sBERT_Status_Code, sBERT_Status_String, &sBERT_ERR_Number, &sBERT_ERR_Ratio, &sBERT_ElapsedTime); 
			if (error<0 || sBERT_Status_Code<0)
			{
				MessagePopup("Error", sBERT_Status_String);
				*ErrorCount=99;
				return -1;
			}  
			
			*ErrorCount=sBERT_ERR_Number;
			break;  
			
		default: 
			{
				MessagePopup("Error", "can not find this BERT Type!");
				return -1;
			}
			break;
	}
	return 0;
}

int BERT_Start_SDD (int channel, double time_test)
{
	int error, count, sBERT_Status_Code;
	int OptoBERT_Lock, OptoBERT_ErrorCount;  
	char 	sBERT_Status_String[500];
	double 	sBERT_ERR_Number, sBERT_ERR_Ratio, sBERT_ElapsedTime; 
	
 	switch (my_struInstrument[channel].BERT_TYPE)
	{
		case BERT_TYPE_GBERT:
			
			if (!GBERT_Start(INST_BERT[channel], time_test))
			{
				MessagePopup("Error", "Reset GBERT error!");
				return -1;
			} 
			
			break;
			
		case BERT_TYPE_SBERT: 
			
			count = 0;
			do
			{
				error = EVB5_SBERT_Init(INST_BERT[channel], SBERT_R_R1244Mbps, SBERT_P_PRBS23);
				if (error)
				{
					MessagePopup("提示","sBERT设置工作模式失败");
					return -1;
				} 
			
				error = EVB5_SBERT_Start(INST_BERT[channel], SBERT_R_R1244Mbps, time_test);
				if (error) 
				{
					MessagePopup("Error", "sBERT start test error"); 
					return -1;
				}
				
				Delay(0.2);
				
				error = EVB5_SBERT_Read(INST_BERT[channel], SBERT_R_R1244Mbps, SBERT_P_PRBS7, &sBERT_Status_Code, sBERT_Status_String, &sBERT_ERR_Number, &sBERT_ERR_Ratio, &sBERT_ElapsedTime); 
				if (error) 
				{
					MessagePopup("Error", "sBERT read data error"); 
					return -1;
				} 
				
				count++;
				Delay(0.3); 
				
			} while (sBERT_Status_Code>=0 && count<3);
			
			if (count>=3) 
			{
				return -1;
			}
			if (-2 != sBERT_Status_Code && -4 != sBERT_Status_Code)	 
			{
				return -1; 
			}
			
			break;
			
		default: 
			{
				MessagePopup("Error", "can not find this BERT Type!");
				return -1;
			}
	}
	
	return 0;
}

int BERT_10G_Start (int channel, double time_test)  
{
	int phase, Lock, Sync;  
	int error;
	
	int count;
	int MAXCOUNT = 6;
	
	switch (my_struInstrument[channel].BERT_TYPE_10G)
	{
		case BERT_TYPE_SBERT10000:
			
			count = 0;
			do
			{
				error = evb16_SetBERTStart(INST_BERT_10G[channel], &Lock, &Sync, 1);
			//	if (error) 
			//	{
			//	//	MessagePopup("提示","SBERT10000同步异常"); 
			//		return -1;
			//	} 
				count++;
			}while((error<0) && count<=MAXCOUNT);
			
			if (count>MAXCOUNT)
			{
				return -1;
			} 
			
			break;
		case BERT_TYPE_GBERT10000:
			
			break;
		default: 
			{
				MessagePopup("Error", "can not find this BERT Type!"); 
				return -1;
			}
	}
	
	return 0;
}

BOOL Read_Error_Bit_10G(int channel, unsigned long long *ErrorCount, double *ErrorRate, double testtime)
{
	int 	Lock, error, count; 
	double 	InstantErrRate, InstantElapsedTime, InstantErrRateArr[1], gBERT_ERR_Number;
	int    	InstantSD, InstantSys, InstantSyns;
	char   	BERT_Status_String[500];
	unsigned int InstantErrNum;

	switch (my_struInstrument[channel].BERT_TYPE_10G)
	{
		case BERT_TYPE_SBERT10000:	
			
			error = evb16_GetBERTStatus (INST_BERT_10G[channel], SBERT10G_SignalIn_SMA, &InstantSD, &InstantSyns, ErrorCount, &InstantElapsedTime, ErrorRate, BERT_Status_String);
			if (error) 
			{
				return FALSE;
			}
		
			break;
			
		case BERT_TYPE_GBERT10000:
			
			error = GBERT10000_Check (INST_BERT_10G[channel], testtime, ErrorRate, &gBERT_ERR_Number);
			if (error) 
			{
				if (gBERT_ERR_Number < 6)
				{
					gBERT_ERR_Number =  0;
				}
				else
				{
					return FALSE;
				}
			}		 
			*ErrorCount = gBERT_ERR_Number;    
			break;
			
		default: 
			{
				MessagePopup("Error", "BERT type error!");
				return FALSE;
			} 
	}
	return TRUE;
}

int DB_Save_Results_Monitor (struct struTestData data)
{
	char buf[5000];
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */

	if (SERVER_ORACLE == my_struConfig.servertype)
	{
		sprintf (buf, "begin sp_add_results_monitor ('%s','%s',to_date('%s','yyyy-mm-dd hh24:mi:ss'),'%s','%s',%f,%d,'%s','%s',%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f); end ;", 
		       	data.PN, data.Ver, data.TestDate, data.ModuleSN, data.OpticsSN,data.AmbientTemperatrue,data.ErrorCode,data.Status,"RxCalibration",data.RxPow_num, 
			   	data.RxPow_mon[0],data.RxPow_mon[1],data.RxPow_mon[2],data.RxPow_mon[3], data.RxPow_mon[4],data.RxPow_mon[5],data.RxPow_mon[6],data.RxPow_mon[7],data.RxPow_mon[8],data.RxPow_mon[9],
				data.RxPow_mon[10],data.RxPow_mon[11],data.RxPow_mon[12],data.RxPow_mon[13],data.RxPow_mon[14], data.RxPow_mon[15],data.RxPow_mon[16],data.RxPow_mon[17],data.RxPow_mon[18],data.RxPow_mon[19],
				data.RxPow_mon[20],data.RxPow_mon[21],data.RxPow_mon[22],data.RxPow_mon[23],data.RxPow_mon[24], data.RxPow_mon[25],data.RxPow_mon[26],data.RxPow_mon[27],data.RxPow_mon[28],data.RxPow_mon[29]); 
		
	}
	else
	{
		sprintf (buf, "EXEC sp_add_autodt_results_monitor '%s','%s','%s','%s','%s',%f,%d,'%s','%s',%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", 
	       	data.PN, data.Ver, data.TestDate, data.ModuleSN, data.OpticsSN,data.AmbientTemperatrue,data.ErrorCode,data.Status,"RxCalibration",data.RxPow_num, 
		   	data.RxPow_mon[0],data.RxPow_mon[1],data.RxPow_mon[2],data.RxPow_mon[3], data.RxPow_mon[4],data.RxPow_mon[5],data.RxPow_mon[6],data.RxPow_mon[7],data.RxPow_mon[8],data.RxPow_mon[9],
			data.RxPow_mon[10],data.RxPow_mon[11],data.RxPow_mon[12],data.RxPow_mon[13],data.RxPow_mon[14], data.RxPow_mon[15],data.RxPow_mon[16],data.RxPow_mon[17],data.RxPow_mon[18],data.RxPow_mon[19],
			data.RxPow_mon[20],data.RxPow_mon[21],data.RxPow_mon[22],data.RxPow_mon[23],data.RxPow_mon[24], data.RxPow_mon[25],data.RxPow_mon[26],data.RxPow_mon[27],data.RxPow_mon[28],data.RxPow_mon[29]); 
	}
	
	resCode = DBImmediateSQL (hdbc, buf); 
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	
	return 0;
}

int tuningAPD_BestSen_10G (int channel, struct struTestData *data) //调试APD 最好灵敏度
{	 
	int i=0,index,index2,APD_DAC,ErroCode; 
	int error,count;
	BOOL Status;
	unsigned short  DACmax=0x450;  
	unsigned short  DACmin=0x300;  
	unsigned short  setDAC=DACmax; 
	unsigned short  DAC_target=0;
	double          dErrorRatio=0;
	double          dErrorRatio_Arry[10]; 
	double          dErrorRatiotemp=1;
	double          dErrorRatiofirst=0;
	unsigned short  DACstep_Arr[2]={0x25, 0x10};     

	int 			tempture_arry[3];
	float			temp_slop[3],temp_offset[3];
	
	float 			temper; 
	float         	ldv_coretemper[3]={-20,42,105};	 
	float         	ldv_APDDAC[3]={0};
	double			K,K1,K2;
	int 			DAC_VAPD30;
	
	int Vapd_1V=110;								//1V Vapd电压=110数字量 
	INT16U tempADC_7;
	INT16U tempADC_9;
	INT16U DACTuningCount=0;
	double tempPower;
	double dErrorRatioSPC[2] ={1.0E-4,1.0E-3};

	//关闭10G光，降低误码率，减少同步不上的比例
	error = Set_10G_TX_OFF(INST_EVB[channel]);
	if (error)
	{
		return -1;        
	}
	
	error = SET_SD_Function_State(INST_EVB[channel],SD_FUN_OFF);
	if (error)
	{
		return -1;        
	}
		
	if (my_struConfig.Temper_Low)
	{
		//低温温DAC范围 
		DACmax=my_struDBConfig_ERCompens.APDDAC_MAX_L_10G;  
		DACmin=my_struDBConfig_ERCompens.APDDAC_MIN_L_10G; 		  
		setDAC=DACmax; 
	}
	else if (my_struConfig.Temper_High)
	{
		//高温DAC范围 
		DACmax=my_struDBConfig_ERCompens.APDDAC_MAX_H_10G;  
		DACmin=my_struDBConfig_ERCompens.APDDAC_MIN_H_10G; 		  
		setDAC=DACmax; 
	}
	else
	{
		//常温DAC范围
		DACmax=my_struDBConfig_ERCompens.APDDAC_MAX_R_10G;  
		DACmin=my_struDBConfig_ERCompens.APDDAC_MIN_R_10G; 	  
		setDAC=DACmax;
	}
	
	K1 = my_struDBConfig_ERCompens.VAPD_KL_10G;
	K2 = my_struDBConfig_ERCompens.VAPD_KH_10G;
		
	//设置光功率  -27dBm
	tempPower = -27;
	error = Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, tempPower + sRxCal_Lock.power[channel]);     
	if (error<0) 
	{
		MessagePopup("Error", "Set Att error!");
		goto Error;        
	}  
	
	//初始化设置 
	//切换模式---手动模式
	error = SET_10G_APD_Mode(INST_EVB[channel], LUT_MODE_MANUAL);
	if (error)
	{
		goto Error;        
	}
	
	error = SET_10G_APD_Range(INST_EVB[channel]);  ////设置APD最大最小值	
	if (error)    
	{	
		goto Error;   
	}
			
	error = SET_10G_APD(INST_EVB[channel], setDAC);  //设置APD
	if (error)    
	{	
		goto Error;        
	}
	
	Delay(0.5);
	
	if(BERT_TYPE_GBERT ==	my_struInstrument[channel].BERT_TYPE)  
	{	
		Status = GBERT_Init_SFPTxDisable(&INST_BERT[channel], my_struInstrument[channel].SN_BERT, 0);
		if (!Status)
		{
			MessagePopup("Error", "Initial GBERT error!");
			goto Error;        
		} 
	}
	
	count=0;
	do
	{
		error = BERT_10G_Start (channel, my_struDBConfig.SensTime);
		if(error<0)
		{
			error = Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, tempPower + sRxCal_Lock.power[channel]);     
			if (error<0) 
			{
				MessagePopup("Error", "Set Att error!");
				goto Error;        
			} 
			tempPower = tempPower +1.0;
		}
	}while(error<0 && count<3);
	
	if (count>=3) 
	{
		goto Error;        
	}
	
	Delay(0.5);

	for(count=0;count<2;count++)
	{
		dErrorRatiotemp=1;
		do
		{	
			tempPower = tempPower -0.3;	 
			error = Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, tempPower + sRxCal_Lock.power[channel]); 
			if (error<0) 
			{
				MessagePopup("Error", "Set Att error!");
				goto Error;
			} 	
		
			Delay(0.5);
		
			Status = Read_Bert_ERR_Ratio_10G(channel, &ErroCode,&dErrorRatio);
			if (!Status)
			{
				goto Error;        
			}
		
		}while(dErrorRatioSPC[count]> dErrorRatio); 
	
		if(1 == dErrorRatio)
		{
			MessagePopup("Error", "Bert Error Ratio =1 ");
			goto Error;	
		}
	
		while(1)
		{		
			index = 0;
			while(1)
			{
				error = SET_10G_APD(INST_EVB[channel], setDAC);  //设置APD
				if (error)    
				{	
					goto Error;   
				}
			
				if(BERT_TYPE_GBERT == my_struInstrument[channel].BERT_TYPE)  
				{
					Status = GBERT_Init_SFPTxDisable(&INST_BERT[channel], my_struInstrument[channel].SN_BERT, 0);
					if (!Status)
					{
						MessagePopup("Error", "Initial GBERT error!");
						goto Error;         
					} 
				}
			
				Delay(1);
				
				Status = Read_Bert_ERR_Ratio_10G(channel, &ErroCode,&dErrorRatio);
				if (!Status)
				{
					goto Error;        
				}
			
				if(dErrorRatiotemp<dErrorRatio&&dErrorRatio<1)
				{
					break;
				}
			
				setDAC=setDAC-DACstep_Arr[i];
				if(setDAC<=(DACmin-DACstep_Arr[0]))
				{
					if(dErrorRatio != 0 )
					{
						setDAC=DACmin;
						i=1;
						break;
					}
					else
					{
						MessagePopup ("Error", "ErrorRatio=0,APD调试超出设置范围!");
						goto Error;         
					}
				}
				dErrorRatiotemp=dErrorRatio;
				index++;
			}
			if(i==1)
			{
				DAC_target=setDAC;
				break;
			}
			setDAC=setDAC+DACstep_Arr[i]*2;
			dErrorRatiotemp = 1;
			i++;  
		}
	}	

	error = SET_SD_Function_State(INST_EVB[channel],SD_FUN_ON);
	if (error)
	{
		return -1;        
	}
	
	setDAC=DAC_target;			
/*	for(Index=0;Index<20;Index++)
	{   
		error = Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN , -7+sRxCal_Free.power[channel]);   
		if (error) 
		{
			MessagePopup ("ERROR", "GVPM Set Att error!"); 
			goto Error;
		} 
		
		error = SET_10G_APD(INST_EVB[channel], setDAC);
		if (error)
		{	
			goto Error;         
		}

		error = SET_EVB_Light_BEN_Trig_Gen (channel);
		if (error)
		{
			MessagePopup ("Error", "设置光源RSSI触发失败"); 
			goto Error;
		}   
		
		error = GET_10G_RX_ADC(INST_EVB[channel], &tempADC_7);
		if(error!=0)
		{
			goto Error;
		}

		error = Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN , -9+sRxCal_Free.power[channel]);   
		if (error) 
		{
			MessagePopup ("ERROR", "GVPM Set Att error!"); 
			goto Error;
		} 
		
		error = SET_EVB_Light_BEN_Trig_Gen (channel);
		if (error)
		{
			MessagePopup ("Error", "设置光源RSSI触发失败"); 
			goto Error;
		}   
		
		error = GET_10G_RX_ADC(INST_EVB[channel], &tempADC_9);
		if(error!=0)
		{
			goto Error;
		}
		
	  	if((tempADC_9-tempADC_7)>0x60)
		{
			break;
		}
		else
		{
			setDAC = setDAC+Vapd_1V/2;	 
		}
	}
	DAC_target = setDAC;  */	
	//-------------以0.5V为步进，微调APD,确认-7dBm与-9dBm差值>0x60,最大降压10V------------------------------wenyao.xi20161010// 
	
	data->DACApd= DAC_target; 		
	//-----------------------------------写查找表-------------------------------------//
	error = GET_CORE_Temper(INST_EVB[channel], &temper);
	if (error)
	{
		goto Error;         
	}   

	K = temper<43 ? K1:K2;   
	if(my_struConfig.Temper_High)
	{
		if(temper<60) 
		{
			goto Error;         
		}
		 
		memset( temp_slop,0,sizeof(temp_slop));
		memset( temp_offset,0,sizeof(temp_offset)); 
		memset( tempture_arry,0,sizeof(tempture_arry)); 
		 
		error = GET_TEMPE_APD_LUT(INST_EVB[channel], LUK_NAME[my_struControl.TunAPD_LUT_NAME_INDEX], my_struControl.TunAPD_LUK_INDEX, my_struControl.TunAPD_LUK_SLOPE_AMP, my_struControl.TunAPD_LUK_OFFSET_AMP, tempture_arry, temp_slop, temp_offset);	//读取查找表    
		if (error<0) 
		{
			goto Error;          
		}
		 
		if(temper>(tempture_arry[0]))
		{
			ldv_APDDAC[1]=temp_slop[1]*(tempture_arry[0]+ my_struControl.GetTemper_IndexOffset)+temp_offset[1];
			ldv_APDDAC[2]=(double)DAC_target ; 
			ldv_coretemper[2]=temper; 
		    ldv_coretemper[1]=tempture_arry[0]; 
			
			error = SET_TEMPE_APD_LUT_High(INST_EVB[channel], LUK_NAME[my_struControl.TunAPD_LUT_NAME_INDEX], my_struControl.TunAPD_LUK_INDEX, my_struControl.TunAPD_LUK_SLOPE_AMP, my_struControl.TunAPD_LUK_OFFSET_AMP, ldv_APDDAC, ldv_coretemper); 	
			if (error<0)       
			{
				goto Error;         
			} 
		}
	}
	else if(my_struConfig.Temper_Low)
	{
		if(temper>25) 
		{
			goto Error;         
		}

		memset( temp_slop,0,sizeof(temp_slop));
		memset( temp_offset,0,sizeof(temp_offset));
		memset( tempture_arry,0,sizeof(tempture_arry));  
		
		error = GET_TEMPE_APD_LUT(INST_EVB[channel], LUK_NAME[my_struControl.TunAPD_LUT_NAME_INDEX], my_struControl.TunAPD_LUK_INDEX,my_struControl.TunAPD_LUK_SLOPE_AMP, my_struControl.TunAPD_LUK_OFFSET_AMP, tempture_arry, temp_slop, temp_offset);	//读取查找表  
		if (error<0) 
		{
			goto Error;          
		}
		
		if(temper<(tempture_arry[0]))
		{
			ldv_APDDAC[1]=temp_slop[1]*(tempture_arry[0]+ my_struControl.GetTemper_IndexOffset)+temp_offset[1];
			ldv_APDDAC[0]=(double)DAC_target ; 
			ldv_coretemper[0]=temper; 
		    ldv_coretemper[1]=tempture_arry[0]; 
			
			error = SET_TEMPE_APD_LUT_Low(INST_EVB[channel], LUK_NAME[my_struControl.TunAPD_LUT_NAME_INDEX], my_struControl.TunAPD_LUK_INDEX, my_struControl.TunAPD_LUK_SLOPE_AMP, my_struControl.TunAPD_LUK_OFFSET_AMP, ldv_APDDAC, ldv_coretemper); 
			if (error)
			{
				goto Error;       
			} 
		}
	}
	else
	{
		DAC_VAPD30=DAC_target - (temper-ldv_coretemper[1])*K;
		ldv_APDDAC[0]=DAC_VAPD30 -(ldv_coretemper[1]-ldv_coretemper[0]) * K1;	 
		ldv_APDDAC[1]=(double)DAC_VAPD30;
		ldv_APDDAC[2]=DAC_VAPD30 -(ldv_coretemper[1]-ldv_coretemper[2]) * K2;
		
		error = SET_TEMPE_APD_LUT(INST_EVB[channel], LUK_NAME[my_struControl.TunAPD_LUT_NAME_INDEX], my_struControl.TunAPD_LUK_INDEX, my_struControl.TunAPD_LUK_SLOPE_AMP, my_struControl.TunAPD_LUK_OFFSET_AMP, ldv_APDDAC, ldv_coretemper);   
		if (error)        
		{
			goto Error;         
		}
	}  
	
	//切换模式---查找表模式
	error = SET_10G_APD_Mode(INST_EVB[channel], LUT_MODE_AUTO);
	if (error)
	{
		goto Error;        
	}
	
	error = Update_BASE(INST_EVB[channel], 0);
	if (error)      
	{
		goto Error;         
	}

	//打开10G光，降低误码率，减少同步不上的比例
	error = Set_10G_TX_ON(INST_EVB[channel]);
	if (error)
	{
		goto Error;        
	}
	
	//set manual mode
/*	if (my_struDBConfig_ERCompens.RSSIMODE>0) 
	{
		error = SET_EVB_Light_MODE_Level (channel);
		if (error)
		{
			MessagePopup ("Error", "设置光源常规模式失败"); 
			goto Error;        
		}   
		
		error = Set_Att_Lock_Enable(channel);
		if (error)
		{
			goto Error;        
		}
	}  */
	return 0;

Error:
	//打开10G光，降低误码率，减少同步不上的比例
	error = Set_10G_TX_ON(INST_EVB[channel]);
	if (error)
	{
		return -1;        
	}
//	error = BERT_Stop (channel);
	
	//set manual mode
	if (my_struDBConfig_ERCompens.RSSIMODE>0) 
	{
		error = SET_EVB_Light_MODE_Level (channel);
		if (error)
		{
			MessagePopup ("Error", "设置光源常规模式失败"); 
			return -1;     
		}   
		
		error = Set_Att_Lock_Enable(channel);
		if (error)
		{
			return -1;     
		}
	}

	return -1;
}  

BOOL DB_Search_Optics_Data_Vbr(char *OpticsSN, double *Vbr)
{
	char buf[1024];
	int  count;
	double temp_Vbr; 
	BOOL SearchOK=TRUE;   
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */

	//查询BosaSN数据库  
	*Vbr=0;
	Fmt (buf, "SELECT APD_T_Vbr from Optics_Data where SN='%s'", OpticsSN);
	
	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return FALSE;} 
	
	resCode = DBBindColDouble (hstmt, 1, &temp_Vbr, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
    
    count=0;
    while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) {count++;}      
    
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) {ShowDataBaseError();return FALSE;} 
    
    if (count != 1)	{SearchOK=FALSE;} 
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	*Vbr=temp_Vbr;
	
	return SearchOK;
}

int Set_Att_Ex (int channel, int RSSI_Mode, double RxP)
{
	int status, error;
	
	//必须是RSSI模式，且使用GVPM才采用此方式，如果不采用GVPM（带锁定模式），函数外部已设置了RRSI模式，不需要切换
	if (RSSI_Mode>0 && my_struInstrument[channel].ATT_TYPE_MAIN == ATT_TYPE_GVPM)  
	{
		if (SET_EVB_Light_MODE_Level (channel)<0) 
		{
			MessagePopup ("Error", "设置光源到Level模式失败"); 
			return -1;
		} 
	}
	else if(RSSI_Mode>0 && my_struInstrument[channel].ATT_TYPE_MAIN == ATT_TYPE_JW8504)
	{
	   	if (SET_EVB_Light_MODE_Level (channel)<0) 
		{
			MessagePopup ("Error", "设置光源到Level模式失败"); 
			return -1;
		} 
	}  
	
	//打开锁定输出模式
	if(RSSI_Mode>0)
	{
		if (Set_Att_Lock_Enable(channel)<0) 
		{
			return -1;
		}
	}
	
	status = Set_Att_Lock(channel, my_struInstrument[channel].ATT_TYPE_MAIN, RxP); 
	if (status) 
	{
		MessagePopup ("提示", "设置校准功率点异常"); 
		return -1;
	}  
	
	//关闭锁定输出模式
	if(RSSI_Mode>0)
	{
		if (Set_Att_Lock_Disable(channel)<0) 
		{
			return -1;
		}
	}
	  
//	Delay (0.3); 
	
	//必须是RSSI模式，且使用GVPM才采用此方式，如果不采用GVPM（带锁定模式），函数外部已设置了RRSI模式，不需要切换 
	if (RSSI_Mode>0 && my_struInstrument[channel].ATT_TYPE_MAIN == ATT_TYPE_GVPM)  
	{
		if (SET_EVB_Light_MODE_RSSI (channel)<0) 
		{
			MessagePopup ("Error", "设置光源到RSSI模式失败"); 
			return -1;
		} 
	}
	else if(RSSI_Mode>0 && my_struInstrument[channel].ATT_TYPE_MAIN == ATT_TYPE_JW8504)
	{
	  	if (SET_EVB_Light_MODE_RSSI (channel)<0) 
		{
			MessagePopup ("Error", "设置光源到RSSI模式失败"); 
			return -1;
		} 
	}   
	
	return 0;
}

BOOL testTemper(int panel, int channel, struct struTestData *data)
{
	int count;
	short temperTmp = 0;
	char   Info[500]; 
	BOOL readTemper = FALSE;
	int error = 0;
	
	int maxCnt = 180;
	
	error = MyDLL_8472_GetTemperatrue (INST_EVB[channel], &data->Temperatrue);  
	//readTemper = GetTemper_DWDM(&temperTmp,channel);
	//data->Temperatrue = temperTmp / 256;
	if (error)
	{
		return FALSE;
	}  

	count = 0;
	do
	{
		if(data->Temperatrue>my_struDBConfig.TemperMax)   /***温度超过上限，则直接退出**Eric.Yao**20131119***/
		{
			MessagePopup("Error", "温度超过上限");
			return FALSE;
		}
		else if (data->Temperatrue > my_struDBConfig.TemperMin && data->Temperatrue < my_struDBConfig.TemperMax)
		{
			break;
		}
		else	/***温度低于下限，则循环等待升温到指标范围或者超时**Eric.Yao**20131119***/
		{
			Delay(1);  
			count++;
		}
		error = MyDLL_8472_GetTemperatrue (INST_EVB[channel], &data->Temperatrue);
		if (error)
		{
			return FALSE;
		} 
	
		sprintf (Info, "测试模块温度=%.2f℃ ", data->Temperatrue);     
		Insert_Info(panel, gCtrl_BOX[channel], Info); 
	
		ProcessDrawEvents ();
	}while (count < maxCnt);		

	//判断范围
	if (data->Temperatrue>my_struDBConfig.TemperMax || data->Temperatrue<my_struDBConfig.TemperMin)
	{
		return FALSE;
	}
	
	return TRUE;
}
BOOL testDWDMAOP10G(int channel, struct struTestData *data)
{
	float Aop_target, aop_min, aop_max, AopTemp;
	BOOL Status = FALSE;
	Aop_target = (my_struDBConfig.TxPowMax + my_struDBConfig.TxPowMin) / 2;    
	aop_min = Aop_target - 0.2;
	aop_max = Aop_target + 0.2;

	Status = Read_AOP(channel, &AopTemp);
	if ((data->AOP>=aop_min) && (data->AOP<=aop_max))	
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL tuningAOP10G(int channel, struct struTestData *data)
{
	int error;
	BOOL Status;
	
	float Aop_target, aop_min, aop_max, txbias;
	int count , forward, backward, DACMax, DACMin, setDAC, DAC_target, DAC, temp_DAC, temp_DACmax, temp_DACmin;
	int ltDAC, ltDACmax, ltDACmin, eaDAC, eaDACmax, eaDACmin, step;
	BOOL tun2;

	char 	buf[256]; 
	float   temper;
	float	ldv_coretemper[3]={6,36,88};
	float 	ldv_10G_APC_DAC[3]={0}; 
	float 	GN2010E_REG_109_SLOPE = 0.14375;
	float 	GN2010E_REG_110_SLOPE = 0.08125;  
	
	tun2 = FALSE;

	Aop_target = (my_struDBConfig.TxPowMax + my_struDBConfig.TxPowMin) / 2;    
	aop_min = Aop_target - 0.2;
	aop_max = Aop_target + 0.2;

	//切换模式---手动模式
	error = SET_10G_APC_Mode(INST_EVB[channel], LUT_MODE_MANUAL);
	if (error)
	{
		return FALSE;
	}
	
	//error = SET_10G_MOD_Mode(INST_EVB[channel], LUT_MODE_MANUAL);
	//if (error)
	//{
	//	return FALSE;
	//}

	//sscanf (my_struDBConfig_ERCompens.RM_ADD_10G, "(0x%03x)(0x%03x)(0x%03x)", &temp_DAC, &temp_DACmin, &temp_DACmax);   
	// 配死
	temp_DAC = 0x40;
	error = SET_10g_DWDM_MOD(INST_EVB[channel], temp_DAC);			 //写MOD_DAC初值
	if (error)
	{
		return FALSE; 
	}
			
	//sscanf (my_struDBConfig_ERCompens.RB_ADD_10G, "(0x%03x)(0x%03x)(0x%03x)", &temp_DAC, &temp_DACmin, &temp_DACmax);  
	temp_DACmax = 0x80;
	temp_DACmin = 0x00;
	DACMax=temp_DACmax;  
	DACMin=temp_DACmin;     
	setDAC=temp_DAC;
		  
	error = SET_10g_DWDM_MOD(INST_EVB[channel], setDAC);			 //写APC_DAC初值
	if (error)
	{
		return FALSE; 
	}
		
	Delay(2);
	
	Status = Read_AOP(channel, &data->AOP);
	if (!Status)
	{
		return FALSE;  
	}
	
	count=0;     
	//set AOP
	while(!tun2)		                
	{	 
		if ((data->AOP>=aop_min) && (data->AOP<=aop_max))	
		{
			break;
		}
		else if((DACMax-DACMin)<2) 				  //减到不能减即退出
		{
			tun2 = TRUE;
			break;
		} 
		else
		{
			if(data->AOP>Aop_target) 
			{
				DACMax=setDAC;
				setDAC=(DACMax+DACMin)/2;
			}
			else					 
			{
				DACMin=setDAC;
				setDAC=(DACMax+DACMin)/2;
			}
		}
		
		error = SET_10g_DWDM_MOD(INST_EVB[channel], setDAC);			 //写APC_DAC
		if (error)
		{
			return FALSE; 
		}
		
		Delay(1.5);   //1.5s
		Status = Read_AOP(channel, &data->AOP);
		if (!Status)
		{
			return FALSE;  
		}
	}
	
	/**再次调试**/
	if (tun2)
	{ 
		Status = Read_AOP(channel, &data->AOP);
		if (!Status)
		{
			return FALSE;  
		}
		
		step = 0x10;
		count = 0;
		setDAC = temp_DAC;
		do
		{
			if (data->AOP < aop_min)
			{
				setDAC = setDAC + step;	
			}
			else if (data->AOP > aop_max)
			{
				setDAC = setDAC - (step>>1) ;
				if (setDAC<0)
				{
					return FALSE;
				}
			}
			else
			{
				break;
			}
			
			error = SET_10g_DWDM_MOD(INST_EVB[channel], setDAC);			 //写APC_DAC
			if (error)
			{
				return FALSE; 
			}
			
			Delay(2);
			
			Status = Read_AOP(channel, &data->AOP);
			if (!Status)
			{
				return FALSE;  
			}
			count++; 
		}while(count < 15);  
		if(count <=15)
		{
			return FALSE;
		}
	}
	
	/***增加10G发端光功率DAC值记录**Eric.Yao**20130716***/
	data->DACApc = setDAC;
	
	
	error = SET_10G_APC_OverShooting(INST_EVB[channel], setDAC);			 //写APC_DAC 过冲
	if (error)
	{
		return FALSE; 
	}
	
	
	error = Save_Flash(INST_EVB[channel], my_struControl.TunAOP10G_SAVE);
	if (error)
	{
		return FALSE;    
	}
	
	error = Update_BASE(INST_EVB[channel],0);
	if (error)
	{
		return FALSE;  
	}
	
	//读取光功率
	Status = Read_AOP(channel, &data->AOP);
	if (!Status)
	{
		return FALSE;  
	}
	
	//记录Biase
	error = MyDLL_8472_GetIbias(INST_EVB[channel], &txbias);      
	if (error)
	{
		return FALSE;   
	}
	data->A2_Ibias = txbias;
	
	if (txbias > my_struDBConfig.TxBiasMax)  
	{
		sprintf(buf,"Bias电流:%f > %f mA",txbias ,my_struDBConfig.TxBiasMax);
		MessagePopup ("Error", buf);
		return FALSE; 
	}	
	
	if (data->AOP>my_struDBConfig.TxPowMax || data->AOP<my_struDBConfig.TxPowMin)
	{
		return FALSE;  	
	} 

	return TRUE;
}

BOOL tuningAOP10G_R(int channel, struct struTestData *data)
{
	int error;
	BOOL Status;
	
	float Aop_target, aop_min, aop_max, txbias;
	int count , forward, backward, DACMax, DACMin, setDAC, DAC_target, DAC, temp_DAC, temp_DACmax, temp_DACmin;
	int ltDAC, ltDACmax, ltDACmin, eaDAC, eaDACmax, eaDACmin, step;
	BOOL tun2;

	char 	buf[256]; 
	float   temper;
	float	ldv_coretemper[3]={6,36,88};
	float 	ldv_10G_APC_DAC[3]={0}; 
	float 	GN2010E_REG_109_SLOPE = 0.14375;
	float 	GN2010E_REG_110_SLOPE = 0.08125;  
	
	tun2 = FALSE;
	data->AOP_CheckFlag = FALSE;
	
	Aop_target = (my_struDBConfig.TxPowMax + my_struDBConfig.TxPowMin) / 2;    
	aop_min = Aop_target - 0.2;
	aop_max = Aop_target + 0.2;

	//切换模式---手动模式
	error = SET_10G_APC_Mode(INST_EVB[channel], LUT_MODE_MANUAL);
	if (error)
	{
		return FALSE;
	}
	
	error = SET_10G_MOD_Mode(INST_EVB[channel], LUT_MODE_MANUAL);
	if (error)
	{
		return FALSE;
	}

//	sscanf (my_struDBConfig_ERCompens.RM_ADD_10G, "(0x%03x)(0x%03x)(0x%03x)", &temp_DAC, &temp_DACmin, &temp_DACmax);     
	
//	error = SET_10G_MOD(INST_EVB[channel], temp_DAC);			 //写MOD_DAC初值
//	if (error)
//	{
//		return FALSE; 
//	}
			
	sscanf (my_struDBConfig_ERCompens.RB_ADD_10G, "(0x%03x)(0x%03x)(0x%03x)", &temp_DAC, &temp_DACmin, &temp_DACmax);     
	DACMax=temp_DACmax;  
	DACMin=temp_DACmin;     
	setDAC=temp_DAC;
		  
	error = SET_10G_APC(INST_EVB[channel], setDAC);			 //写APC_DAC初值
	if (error)
	{
		return FALSE; 
	}
		
	Delay(2);
	
	Status = Read_AOP(channel, &data->AOP);
	if (!Status)
	{
		return FALSE;  
	}
	
	count=0;     
	//set AOP
	while(!tun2)		                
	{	 
		if ((data->AOP>=aop_min) && (data->AOP<=aop_max))	
		{
			break;
		}
		else if((DACMax-DACMin)<2) 				  //减到不能减即退出
		{
			tun2 = TRUE;
			break;
		} 
		else
		{
			if(data->AOP>Aop_target) 
			{
				DACMax=setDAC;
				setDAC=(DACMax+DACMin)/2;
			}
			else					 
			{
				DACMin=setDAC;
				setDAC=(DACMax+DACMin)/2;
			}
		}
		
		error = SET_10G_APC(INST_EVB[channel], setDAC);			 //写APC_DAC
		if (error)
		{
			return FALSE; 
		}
		
		Delay(1.5);   //1.5s
		Status = Read_AOP(channel, &data->AOP);
		if (!Status)
		{
			return FALSE;  
		}
	}
	
	/**再次调试**/
	if (tun2)
	{ 
		Status = Read_AOP(channel, &data->AOP);
		if (!Status)
		{
			return FALSE;  
		}
		
		step = 0x10;
		count = 0;
		setDAC = temp_DAC;
		do
		{
			if (data->AOP < aop_min)
			{
				setDAC = setDAC + step;	
			}
			else if (data->AOP > aop_max)
			{
				setDAC = setDAC - (step>>1) ;
				if (setDAC<0)
				{
					return FALSE;
				}
			}
			else
			{
				break;
			}
			
			error = SET_10G_APC(INST_EVB[channel], setDAC);			 //写APC_DAC
			if (error)
			{
				return FALSE; 
			}
			
			Delay(2);
			
			Status = Read_AOP(channel, &data->AOP);
			if (!Status)
			{
				return FALSE;  
			}
			count++; 
		}while(count < 15);  
		if(count <=15)
		{
			return FALSE;
		}
	}
	
	/***增加10G发端光功率DAC值记录**Eric.Yao**20130716***/
	data->DACApc = setDAC;

	error = Save_Flash(INST_EVB[channel], my_struControl.TunAOP10G_SAVE);
	if (error)
	{
		return FALSE;    
	}
	
	
	//读取光功率
	Status = Read_AOP(channel, &data->AOP);
	if (!Status)
	{
		return FALSE;  
	}
	
	//记录Biase
	error = MyDLL_8472_GetIbias (INST_EVB[channel], &txbias);
	if (error)
	{
		return FALSE;   
	}
	data->A2_Ibias = txbias;
	
	if (txbias > my_struDBConfig.TxBiasMax)  
	{
		sprintf(buf,"Bias电流:%f > %f mA",txbias ,my_struDBConfig.TxBiasMax);
		MessagePopup ("Error", buf);
		return FALSE; 
	}	
	
	if (data->AOP>my_struDBConfig.TxPowMax || data->AOP<my_struDBConfig.TxPowMin)
	{
		data->AOP_CheckFlag = TRUE; 
		return FALSE;  	
	} 

	return TRUE;
}

BOOL tuningRE10G_DWDM(int panel, int channel, struct struTestData *data)
{
	int   error;
	BOOL  Status;
	int   dac_target, count, DACMax, DACMin, setDAC, temp_DACmax, temp_DACmin, temp_DAC;
	float target, er_min, er_max, er, ex0, ex1;
	BOOL  tun_ok=FALSE;	  
	
	float ldv_coretemper[3]={6,45,98},temper;
	float ldv_10G_Er_DAC[3]={0};
	float K,K1,K2;  
	int	  DAC_Er30;
	
	target = (my_struDBConfig.TxErMax + my_struDBConfig.TxErMin) / 2;    
	er_min = target - 0.2;
	er_max = target + 0.2;
	//sscanf (my_struDBConfig_ERCompens.RM_ADD_10G, "(0x%03x)(0x%03x)(0x%03x)", &temp_DAC, &temp_DACmin, &temp_DACmax); 
	count=0;
	temp_DACmax = 0x0300;
	temp_DACmin = 0x0;
	DACMax=temp_DACmax; 
	DACMin=temp_DACmin;
	setDAC=0x00D8; 
	
	//切换模式---手动模式
	error = SET_10G_MOD_Mode(INST_EVB[channel], LUT_MODE_MANUAL);
	if (error)
	{
		return FALSE;
	}
	
	error = SET_10G_MOD(INST_EVB[channel], setDAC);			 //写MOD_DAC初值
	if (error)
	{
		return FALSE; 
	}
	
	Delay(1.5);
	
	Status = Read_ER(channel, &data->ER);
	if (!Status)
	{
		return FALSE;   
	}

	count=0;
	while(!tun_ok)		                
	{	 
		if ((data->ER>=er_min) && (data->ER<=er_max))
		{
			break;
		}	
		else if((DACMax-DACMin)<2) 
		{
			return FALSE;
		} 
		else
		{
			if(data->ER>target) 
			{
				DACMax=setDAC;
				setDAC=(DACMax+DACMin)/2;
			}   
			else					 
			{
				DACMin=setDAC;
				setDAC=(DACMax+DACMin)/2;
			}    
		}
		
		error = SET_10G_MOD(INST_EVB[channel], setDAC);			 //写MOD_DAC
		if (error)
		{
			return FALSE; 
		}
		
		Delay(1.5);
		
		Status = Read_ER(channel, &data->ER);
		if (!Status)
		{
			return FALSE;   
		}   
	}
	/***增加10G消光比DAC值记录**Eric.Yao**20130716***/
	data->DACMod = setDAC;	 
	
	//写LUK
	error = GET_CORE_Temper(INST_EVB[channel], &temper);		//核温索引改为Case索引
	if (error)
	{
		return FALSE;	
	}
			
	ldv_coretemper[1]=temper;
	
	ldv_10G_Er_DAC[0]= (double)setDAC -(ldv_coretemper[1]-ldv_coretemper[0])*my_struDBConfig_ERCompens.IM10_33_10G;
	ldv_10G_Er_DAC[1]= (double)setDAC; 
	ldv_10G_Er_DAC[2]= (double)setDAC -(ldv_coretemper[1]-ldv_coretemper[2])*my_struDBConfig_ERCompens.IM33_60_10G; 
	
	error = SET_TEMPE_LUT(INST_EVB[channel], LUK_NAME[my_struControl.TunER10G_LUT_NAME_INDEX], my_struControl.TunER10G_LUK_INDEX, my_struControl.TunER10G_LUK_SLOPE_AMP, my_struControl.TunER10G_LUK_OFFSET_AMP, ldv_10G_Er_DAC, ldv_coretemper);
	if (error)
	{
		return FALSE;
	}
	
	//切换模式---自动模式
	error = SET_10G_MOD_Mode(INST_EVB[channel], LUT_MODE_AUTO);
	if (error)
	{
		return FALSE;
	}

	error = Save_Flash(INST_EVB[channel],my_struControl.TunER10G_SAVE);
	if (error)
	{
		return FALSE;
	}
	
	error = Update_BASE(INST_EVB[channel], 0);
	if (error)
	{
		return FALSE;
	}
	
	Status = Read_ER(channel, &data->ER);
	if (!Status)
	{
		return FALSE;   
	}

	if ((data->ER>my_struDBConfig.TxErMax) || (data->ER<my_struDBConfig.TxErMin))  
	{
		return FALSE;
	}
	
	Status = Read_AOP(channel, &data->AOP);
	if (!Status)
	{
		return FALSE;  
	}
	
	//判断光功率
	if ((data->AOP>my_struDBConfig.TxPowMax) || (data->AOP<my_struDBConfig.TxPowMin)) 
	{
		data->AOP_CheckFlag = TRUE;
		return FALSE;
	}
	
	return TRUE; 
}
int SET_10G_DWDM_ER(int handle, unsigned short SET_MOD)
{
	int error=0;
	int	Temp_DAC;
	unsigned short SET_MOD_LOW, SET_MOD_HIGH;
	char strOupt[256];
	char strInpt[256];
	
	char buf[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	SET_MOD_LOW = SET_MOD;
	SET_MOD_HIGH = SET_MOD;
	// 低位
	sprintf(strInpt,"gn2010_write(87,0x%X)", (unsigned char)(SET_MOD_LOW&0xFF));   
	error = SetCommand (handle, strInpt, strOupt);
	// 高位
	sprintf(strInpt,"gn2010_write(88,0x%X)", (unsigned char)((SET_MOD_HIGH>>8)&0xFF));   
	error = SetCommand (handle, strInpt, strOupt);

	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0; 
}


BOOL tuningER10G(int panel, int channel, struct struTestData *data)
{
	int   error;
	BOOL  Status;
	int   dac_target, count, DACMax, DACMin, setDAC, temp_DACmax, temp_DACmin, temp_DAC;
	float target, er_min, er_max, er, ex0, ex1;
	BOOL  tun_ok=FALSE;	  
	
	float ldv_coretemper[3]={6,45,98},temper;
	float ldv_10G_Er_DAC[3]={0};
	float K,K1,K2;  
	int	  DAC_Er30;
	
	target = (my_struDBConfig.TxErMax + my_struDBConfig.TxErMin) / 2;    
	er_min = target - 0.2;
	er_max = target + 0.2;
	//sscanf (my_struDBConfig_ERCompens.RM_ADD_10G, "(0x%03x)(0x%03x)(0x%03x)", &temp_DAC, &temp_DACmin, &temp_DACmax); 
	count=0;
	temp_DACmax = 0x0300;
	temp_DACmin = 0x00;
	temp_DAC = 0x168;
	DACMax=temp_DACmax; 
	DACMin=temp_DACmin;
	setDAC=temp_DAC; 
	
	//切换模式---手动模式
	error = SET_10G_MOD_Mode(INST_EVB[channel], LUT_MODE_MANUAL);
	if (error)
	{
		return FALSE;
	}
	
	error = SET_10G_DWDM_ER(INST_EVB[channel], setDAC);			 //写MOD_DAC初值
	if (error)
	{
		return FALSE; 
	}
	
	Delay(1.5);
	
	Status = Read_ER(channel, &data->ER);
	if (!Status)
	{
		return FALSE;   
	}

	count=0;
	while(!tun_ok)		                
	{	 
		if ((data->ER>=er_min) && (data->ER<=er_max))
		{
			break;
		}	
		else if((DACMax-DACMin)<2) 
		{
			return FALSE;
		} 
		else
		{
			if(data->ER>target) 
			{
				DACMax=setDAC;
				setDAC=(DACMax+DACMin)/2;
			}   
			else					 
			{
				DACMin=setDAC;
				setDAC=(DACMax+DACMin)/2;
			}    
		}
		
		error = SET_10G_DWDM_ER(INST_EVB[channel], setDAC);			 //写MOD_DAC
		if (error)
		{
			return FALSE; 
		}
		
		Delay(1.5);
		
		Status = Read_ER(channel, &data->ER);
		if (!Status)
		{
			return FALSE;   
		}   
	}
	/***增加10G消光比DAC值记录**Eric.Yao**20130716***/
	data->DACMod = setDAC;	 
	
	//切换模式---自动模式
	error = SET_10G_MOD_Mode(INST_EVB[channel], LUT_MODE_AUTO);
	if (error)
	{
		return FALSE;
	}

	error = Save_Flash(INST_EVB[channel],my_struControl.TunER10G_SAVE);
	if (error)
	{
		return FALSE;
	}
	
	error = Update_BASE(INST_EVB[channel], 0);
	if (error)
	{
		return FALSE;
	}
	
	Status = Read_ER(channel, &data->ER);
	if (!Status)
	{
		return FALSE;   
	}

	if ((data->ER>my_struDBConfig.TxErMax) || (data->ER<my_struDBConfig.TxErMin))  
	{
		return FALSE;
	}
	
	Status = Read_AOP(channel, &data->AOP);
	if (!Status)
	{
		return FALSE;  
	}
	
	//判断光功率
	if ((data->AOP>my_struDBConfig.TxPowMax) || (data->AOP<my_struDBConfig.TxPowMin)) 
	{
		data->AOP_CheckFlag = TRUE;
		return FALSE;
	}
	
	return TRUE;  
}
BOOL caltestTxPower(int channel, struct struTestData *data)
{
	int error = 0;
	int wValue;
	char strInpt[256], strOupt[256];
	double  TxPow_mon, temp; //mW
	float  TxPow_meter;
	float txbias = 0;
	char buf[256] = {0};
	//测试时，在此处使用了光开关切换通道，从无光变为有光，需要延时
	Delay(3.);
	//添加测试流程
	if (!Read_AOP(channel, &TxPow_meter)) 
	{
		return FALSE;  
	}
	if (TxPow_meter < -1 || TxPow_meter > 1.5) 
	{
		return FALSE;  
	}
	data->AOP = TxPow_meter;
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	strcpy(strInpt, "SFP_GET_MONITOR(TXPOWER)");  
	
	if (SetCommand(INST_EVB[channel], strInpt, strOupt)<0)
	{
		return FALSE;    
	}
	sscanf(strOupt,"0x%x", &wValue); 
	
	TxPow_mon = 10 * log10(wValue * 0.0001);
	
	temp = fabs (TxPow_meter-TxPow_mon); 
	if (temp>my_struDBConfig_Monitor.Tx_Pow_Prec)
	{
		return FALSE; 
	}

	//记录Biase
	error = MyDLL_8472_GetIbias(INST_EVB[channel], &txbias);      
	if (error)
	{
		return FALSE;   
	}
	data->A2_Ibias = txbias;
	
	if (txbias > my_struDBConfig.TxBiasMax)  
	{
		sprintf(buf,"Bias电流:%f > %f mA",txbias ,my_struDBConfig.TxBiasMax);
		MessagePopup ("Error", buf);
		return FALSE; 
	}	
	
	my_struDBConfig.TxCalMin = -1;
	my_struDBConfig.TxCalMax = 1.5;
	if (TxPow_mon < my_struDBConfig.TxCalMin || TxPow_mon > my_struDBConfig.TxCalMax)		/***增加发端监控门限值检查，门限采用QA光功率标准**zhi.xu**20141113***/
	{
		return FALSE;           
	}
	return TRUE;
}


BOOL testER10G(int channel, struct struTestData *data)
{
	int count, error;   

	double 	temp_ER, temp_ERmin, temp_ERmax, temp_ERavg; 
	int temp_DAC, temp_DACmax, temp_DACmin;

	int   setDAC; 
	int   DACMax;
	int   DACMin; 
	float ldv_10G_ER_DAC[3]={0},temp_AOP;
	float ldv_coretemper[3]={-25,43,96}; 

	int			tempture_arry[3];
	float		temp_slop[3],temp_offset[3];

	int ER_DAC;
	char buf[256];
	
	char Info[500];     	
	
	if (!Read_ER(channel, &data->ER))
	{
		return FALSE; 
	}
	
			
	if (data->ER>500.)
	{	  
		//一定是示波器度取出正确的消光比，重新设置示波器刻度
		if (my_struInstrument[channel].DCA_TYPE==DCA_TYPE_CSA8000) 
		{
			if (!CSA8000_Set_O(inst_CSA8000, my_struCSA8000.Channel_O, my_struCSA8000.X_Scale*2, my_struCSA8000.X_Position, my_struCSA8000.Y_Scale*2, my_struCSA8000.Y_Position, my_struCSA8000.Y_Offset, my_struCSA8000.Wavelength, my_struCSA8000.Filter, my_struCSA8000.MaskName)) 
			{
				MessagePopup("Error", "CSA8000 Config for Optics Channel error!");
				error=-1; 
				return FALSE;     
			} 
		}
		else if (my_struInstrument[channel].DCA_TYPE==DCA_TYPE_Ag86100)
		{
			if (!Ag86100_SET_O(inst_Ag86100, my_struAg86100.ERFactor, my_struAg86100.X_Scale*2, my_struAg86100.X_Position, my_struAg86100.Y_Scale*2, my_struAg86100.Y_Offset, my_struAg86100.Filter, my_struAg86100.Wavelength, my_struAg86100.MaskName, my_struAg86100.MaskMargin,my_struAg86100.Channel_O))   
			{
				MessagePopup("Error", "Initial Ag86100 error!");
				error=-1;
				return FALSE;     
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
			return FALSE;     
		}
	
		if (!Read_ER(channel, &data->ER))
		{
			return FALSE;    
		}	  
	}  
	
	//判断消光比
	if (data->ER>my_struDBConfig.TxErMax || data->ER<my_struDBConfig.TxErMin) 
	{

		return FALSE; 
	}
	else
	{
		;
	}
	
	if ((data->AOP>my_struDBConfig.TxPowMax)||(data->AOP<my_struDBConfig.TxPowMin)) 
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL testCross(int channel, struct struTestData *data)
{
int count;
	
	count=0;
	do
	{
		if (!READ_DCA_PCTCROss(my_struInstrument[channel].DCA_TYPE, &data->TxCross)) 
		{
			return FALSE;  
		}

		if (data->TxCross<=my_struDBConfig.TxCrossMax && data->TxCross>=my_struDBConfig.TxCrossMin)	
		{
			break; 
		}
		
		count++;
		Delay(1);
	}
	while(count<3);
	
	//判断光功率
	if (data->TxCross>my_struDBConfig.TxCrossMax || data->TxCross<my_struDBConfig.TxCrossMin)
	{
		return FALSE;
	}
		
	return TRUE;
}

BOOL DB_GET_Spectrum(void)
{
	char buf[1024]; 
	int	num;
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */
	
	sprintf (buf, "SELECT laser_type, Wavelength_start, Wavelength_stop, Wavelength_center, Span FROM AutoDT_Spec_Spectrum WHERE  pn='%s' ", my_struConfig.PN );  
	
	hstmt = DBActivateSQL (hdbc, buf);																												  
    if (hstmt <= 0) {ShowDataBaseError(); return FALSE;}  	

	resCode = DBBindColChar (hstmt, 1, 10, my_struConfig.laser_type, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 2, &my_struConfig.wave_start, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 3, &my_struConfig.wave_stop, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 4, &my_struConfig.wave_center, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColDouble (hstmt, 5, &my_struConfig.wave_span, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
    num=0;
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)   {num++;}      
    
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) {ShowDataBaseError();  return FALSE;}
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	if (1 != num)	{MessagePopup("Error", "数据表AutoDT_Spec_Spectrum没有找到该产品类型的配置参数！   ");return FALSE;}
	
	return TRUE;    
}

int DB_GET_EA(char *OpticsSN, float *ea)
{
int num, mySNLength;
char buf[256];
char myBarCode[50];
float tmpEA;

	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */

	tmpEA = 0;

	num=0;

	sprintf (buf, "select er from xfp_ld_data where originsn = '%s'", OpticsSN);
	
	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return -1;}  
    
    resCode = DBBindColFloat (hstmt, 1, &tmpEA, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
    
    while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
	{
		num++;
	}      
    
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) 
	{
		ShowDataBaseError();  
		return -1;
	}
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) 
	{
		ShowDataBaseError(); 
		return -1;
	}
	
	if (0 == num) 
	{
		MessagePopup("Error","数据库XFP_LD_DATA中无对应的EA值  !"); 
		return -1;
	} 
	else if(1 < num)
	{
		MessagePopup("Error","数据库XFP_LD_DATA中有多条对应的的EA值  !"); 
		return -1;
	}
	
	*ea = tmpEA;  

	return 0;
}

int analize_1G_10G(const char *info, char *info_1G, char *info_10G)
{
	//将输入的数据按照1G和10G解析出来
	
	char tmp_info[1024]; 
	char tmp_1G[512];
	char tmp_10G[512];  
	char tmp[512];
	
	int index;
	int cnt;

	memset(tmp_info, 0, sizeof(tmp_info));
	memset(tmp_1G,   0, sizeof(tmp_1G));        
	memset(tmp_10G,  0, sizeof(tmp_10G));  
	memset(tmp,      0, sizeof(tmp));  
	
	strcpy(tmp_info, info);  
	
	cnt = 0;
	for (index = 0; index < strlen(info); index++)
	{
		if (';' == info[index])
		{
			cnt++;
		}
	}
	
	if ((NULL == strstr(tmp_info, "1G")) && (NULL == strstr(tmp_info, "10G")) && (0 == cnt))
	{
		//没有1G和10G标志，则认为是1G和10G驱动芯片类型一样
		strcpy(tmp_1G, info);
		strcpy(tmp_10G, info);     
	}
	else if ((NULL != strstr(tmp_info, "1G")) && (NULL != strstr(tmp_info, "10G")) && (1 == cnt))           
	{   
		//切分成1G和10G信息
		Scan (tmp_info, "%s>%s[xt59]%s", tmp_1G, tmp_10G);   //按照“;”截断字符串
		Scan (tmp_1G, 	"%s>%s[xt58]%s", tmp, 	 tmp_1G); 	 //取“:”后的字符串
		Scan (tmp_10G, 	"%s>%s[xt58]%s", tmp, 	 tmp_10G); 	 //取“:”后的字符串     
	}
	else
	{
		return -1;
	}  
	
	strcpy(info_1G, tmp_1G);
	strcpy(info_10G, tmp_10G);        
	
	return 0;
}

int SetCaliConfigFile(void)
{
	int Status;
	
	Status = ArrayToFile (my_struTxCal.TxCal_Power_FileName, my_struTxCal.power_array, VAL_FLOAT, 4*CHANNEL_MAX, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_ROWS, VAL_SEP_BY_TAB, 10, VAL_BINARY, VAL_TRUNCATE);
	if (Status != 0) {MessagePopup("提示", "写入发端光功率校准文件出错！");  return -1;} 

	Status = ArrayToFile (sRxCal_Lock.RxCal_FileName, sRxCal_Lock.power_array, VAL_FLOAT, 4*CHANNEL_MAX, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_ROWS, VAL_SEP_BY_TAB, 10, VAL_BINARY, VAL_TRUNCATE);
	if (Status != 0) {MessagePopup("提示", "写入收端校准文件出错！");  return -1;} 
	
	Status = ArrayToFile (sRxCal_Free.RxCal_FileName, sRxCal_Free.power_array, VAL_FLOAT, 4*CHANNEL_MAX, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_ROWS, VAL_SEP_BY_TAB, 10, VAL_BINARY, VAL_TRUNCATE);
	if (Status != 0) {MessagePopup("提示", "写入收端校准文件出错！");  return -1;} 

	return 0;
}

int DB_save_txcali(int channel)
{
	char buf[2048];
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */ 
	char stationid[256];
	
	sprintf (stationid, "%s-%d",my_struProcessLOG.StationID, channel);
	
	if (SERVER_ORACLE == my_struConfig.servertype)  	 
	{ 
		sprintf (buf, "INSERT INTO calibration (id, pcname,loss,lossmin,lossmax,flag) VALUES (s_id.nextval, '%s',%f,%f,%f,'%s')", stationid,my_struTxCal.power[channel],my_struTxCal.power_min[channel],my_struTxCal.power_max[channel],CALI_TYPE[CALI_TYPE_TX]); 
	}
	else
	{
		sprintf (buf, "INSERT INTO calibration (pcname,loss,lossmin,lossmax,flag) VALUES ('%s',%f,%f,%f,'%s')", stationid,my_struTxCal.power[channel],my_struTxCal.power_min[channel],my_struTxCal.power_max[channel],CALI_TYPE[CALI_TYPE_TX]); 
	}
	resCode = DBImmediateSQL (hdbc, buf); 
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;} 
	
	return 0;
}

int DB_get_txchecklimit(void)
{
	char buf[1024];
	int  cnt;
	
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */
	
	sprintf (buf, "select val_min,val_max from calibration_spec where sn='%s'", sTxCheck.sn);

	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return -1;}  
    
    resCode = DBBindColFloat (hstmt, 1, &sTxCheck.Power_Min, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
    resCode = DBBindColFloat (hstmt, 2, &sTxCheck.Power_Max, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	
	cnt=0;
    while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
    {
		cnt++;
    }      
    
    if (resCode != DB_SUCCESS && resCode != DB_EOF) {ShowDataBaseError();  return -1;}
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	
	if (cnt!=1)
	{
		MessagePopup("提示", "数据库中没有存放当前标准模块相关记录    ");
		return -1;
	}
	
	return 0;
}

int DB_save_txcheckup(char sn[], double val)
{
	char buf[2048];
	
	int resCode = 0;   /* Result code                      */ 
//	int hstmt = 0;	   /* Handle to SQL statement          */
	
	if (SERVER_ORACLE == my_struConfig.servertype)      
	{
		sprintf (buf, "INSERT INTO calibration (id, pcname,modulesn,sysval,flag,lossmin,lossmax) VALUES (s_id.nextval, '%s','%s',%f,'%s',%f,%f)", my_struProcessLOG.StationID,sn,val,CALI_TYPE[CALI_TYPE_TX_CHECKUP],sTxCheck.Power_Min,sTxCheck.Power_Max); 
	}
	else
	{
		sprintf (buf, "INSERT INTO calibration (pcname,modulesn,sysval,flag,lossmin,lossmax) VALUES ('%s','%s',%f,'%s',%f,%f)", my_struProcessLOG.StationID,sn,val,CALI_TYPE[CALI_TYPE_TX_CHECKUP],sTxCheck.Power_Min,sTxCheck.Power_Max); 
	}
		
	resCode = DBImmediateSQL (hdbc, buf); 
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;} 
	
	return 0;
}

void readconfigfilename(void)
{
	//读取调测功能配置参数文件名称
	GetProjectDir (g_ConfigFileName);
	strcat (g_ConfigFileName,"\\data\\ATE_OLT_Function.ini");		 
		
	//读取调测功能配置参数文件名称
	GetProjectDir (g_CtrFileName);
	strcat (g_CtrFileName,"\\data\\ATE_OLT_Control.ini");
	
	//get tx power cal file name
	GetProjectDir (my_struTxCal.TxCal_Power_FileName);
	strcat (my_struTxCal.TxCal_Power_FileName,"\\data\\ATE_OLT_TxPowCal.txt");

	//get rx power cal file name
	GetProjectDir (sRxCal_Lock.RxCal_FileName);
	strcat (sRxCal_Lock.RxCal_FileName,"\\data\\ATE_RxPowCal_Main_Lock.txt");  
	
	//get rx power cal file name
	GetProjectDir (sRxCal_Free.RxCal_FileName);
	strcat (sRxCal_Free.RxCal_FileName,"\\data\\ATE_RxPowCal_Main_Free.txt"); 
	
	//get rx power ref cal file name
	GetProjectDir (sRxCal_Ref.RxCal_FileName);
	strcat (sRxCal_Ref.RxCal_FileName,"\\data\\ATE_RxPowCal_Ref.txt");
	
	//读取仪器地址配置文件
	GetProjectDir (g_InstFileName);
	strcat (g_InstFileName,"\\data\\ATE_OLT_Instrument.ini");
	
	//用于配置初始的参数
	Status = GetConfig_Inst();
	if(!Status) { MessagePopup ("Error", "Get Instrument config error !"); } 
	
	return ;
} 

int GetCaliConfigFile(void)
{
	int Status;
	int i;  
	
	Status = FileToArray (my_struTxCal.TxCal_Power_FileName, my_struTxCal.power_array, VAL_FLOAT, 4*CHANNEL_MAX, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_ROWS, VAL_BINARY);
	if (Status != 0)	{MessagePopup("提示", "打开发端校准文件出错！");  goto Error;}  
	for (i=0;i<CHANNEL_MAX;i++)
	{
		my_struTxCal.power_st[i]  = my_struTxCal.power_array[i*4+0];
		my_struTxCal.power_min[i] = my_struTxCal.power_array[i*4+1]; 
		my_struTxCal.power_max[i] = my_struTxCal.power_array[i*4+2]; 
		my_struTxCal.power[i] 	  = my_struTxCal.power_array[i*4+3];
	}

	Status = FileToArray (sRxCal_Lock.RxCal_FileName, sRxCal_Lock.power_array, VAL_FLOAT, 4*CHANNEL_MAX, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_ROWS, VAL_BINARY);
	if (Status != 0)	{MessagePopup("提示", "打开收端校准文件出错！");  goto Error;}  
	for (i=0;i<CHANNEL_MAX;i++)  
	{
		sRxCal_Lock.power_in[i]  = sRxCal_Lock.power_array[i*4+0];
		sRxCal_Lock.power_min[i] = sRxCal_Lock.power_array[i*4+1]; 
		sRxCal_Lock.power_max[i] = sRxCal_Lock.power_array[i*4+2]; 
		sRxCal_Lock.power[i] 	 = sRxCal_Lock.power_array[i*4+3];
	}
	
	Status = FileToArray (sRxCal_Free.RxCal_FileName, sRxCal_Free.power_array, VAL_FLOAT, 4*CHANNEL_MAX, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_ROWS, VAL_BINARY);
	if (Status != 0)	{MessagePopup("提示", "打开收端校准文件出错！");  goto Error;}  
	for (i=0;i<CHANNEL_MAX;i++)  
	{
		sRxCal_Free.power_in[i]  = sRxCal_Free.power_array[i*4+0];
		sRxCal_Free.power_min[i] = sRxCal_Free.power_array[i*4+1]; 
		sRxCal_Free.power_max[i] = sRxCal_Free.power_array[i*4+2]; 
		sRxCal_Free.power[i] 	 = sRxCal_Free.power_array[i*4+3];
	}   
	
	Status = FileToArray (sRxCal_Ref.RxCal_FileName, sRxCal_Ref.power_array, VAL_FLOAT, 4*CHANNEL_MAX, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_ROWS, VAL_BINARY);
	if (Status != 0)	{MessagePopup("提示", "打开收端校准文件出错！");  goto Error;}  
	for (i=0;i<CHANNEL_MAX;i++)  
	{
		sRxCal_Ref.power_in[i]  = sRxCal_Ref.power_array[i*4+0];
		sRxCal_Ref.power_min[i] = sRxCal_Ref.power_array[i*4+1]; 
		sRxCal_Ref.power_max[i] = sRxCal_Ref.power_array[i*4+2]; 
		sRxCal_Ref.power[i] 	= sRxCal_Ref.power_array[i*4+3];
	}  
	
	return 0;

Error:
	
	return -1;	
}


int DB_Read_Spec_TxCalThreshold (void)
{
	char buf[256];
	int  num;
	
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */
    
	Fmt (buf, "select TxCalThreshold_Min,TxCalThreshold_Max from AUTODT_SPEC_ATE_NEW  where dt_flag='%s' and temper_flag='%s' and PartNumber='%s' and version='%s'",my_struConfig.DT_Flag,my_struConfig.Temper_Flag, my_struConfig.PN, my_struConfig.Ver);  
	
	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return -1;}  
	
	resCode = DBBindColFloat (hstmt, 1, &my_struDBConfig.TxCalMin, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}	
	resCode = DBBindColFloat (hstmt, 2, &my_struDBConfig.TxCalMax, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	
	num=0;
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) {num++;}
	
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
    
	if (num!=1) {MessagePopup("提示","数据库AUTODT_SPEC_10G_ATE中不能找到相应的数据！"); return -1;} 	
	
	return 0;
}

int DB_Read_Spec_1GTxCal (void)
{
	char buf[256];
	int  num;
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */
    
	Fmt (buf, "select txpowmin,txpowmax from AUTODT_SPEC_ATE  where dt_flag='QATEST' and temper_flag='ROOM' and PartNumber='%s' and version='%s'",my_struConfig.PN, my_struConfig.Ver);  
	
	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return -1;}  
	
	resCode = DBBindColFloat (hstmt, 1, &my_struDBConfig.TxCalMin, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}	
	resCode = DBBindColFloat (hstmt, 2, &my_struDBConfig.TxCalMax, &DBConfigStat);
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	
	num=0;
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) {num++;}
	
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
    
	if (num!=1) 
	{
		return -1;
	} 	
	
	return 0;
}  

int Set_LOS_DAC (int handle, const INT16U DAC)
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,%s,0x%x,0x%x)", my_struControl.TunSD_DAC_SET, (unsigned char)(DAC&0xFF), (unsigned char)((DAC>>8)&0xFF));    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

BOOL caltestTxPower10G(int channel, struct struTestData *data)
{
	BOOL Status;
	int  error;
	
	float TxPow_meter, TxPow_mon; //mW
	
	//添加测试流程
	Status = Read_AOP(channel, &TxPow_meter);
	if (!Status)
	{
		return FALSE; 
	}
	
	error = GET_10G_TxMon(INST_EVB[channel], &TxPow_mon, my_struConfig.Tx_unit);
	if (error)
	{
		return FALSE; 
	}

	data->A2_TxMon = TxPow_mon;   /***保存发端监控光功率值在AutoDT_Results_ATE的RxTr列**Eric.Yao**20131119***/      
	
	if ((TxPow_meter-10*log10(TxPow_mon))>my_struDBConfig_Monitor.Tx_Pow_Prec)
	{
		return FALSE;
	}
	
	/***修改发端监控光功率范围判断，其范围采用QA测试门限**zhi.xu**20141117***/          
	if (TxPow_mon < my_struDBConfig.TxCalMin || TxPow_mon > my_struDBConfig.TxCalMax)
	{
		return FALSE;   
	}

	return TRUE;
}

int display_FSW(int panel, int fsw_type)
{
	switch(fsw_type)
	{
		case SW_TYPE_NONE:
			
			SetCtrlAttribute (panel, PAN_INST_RING_FSW, ATTR_VISIBLE, FALSE);
			SetCtrlAttribute (panel, PAN_INST_BUT_FSW,  ATTR_VISIBLE, FALSE);
			SetCtrlAttribute (panel, PAN_INST_NUM_SW,   ATTR_VISIBLE, FALSE); 
			break;
					
		case SW_TYPE_FSW:
			
			SetCtrlAttribute (panel, PAN_INST_RING_FSW, ATTR_VISIBLE, FALSE);
			SetCtrlAttribute (panel, PAN_INST_BUT_FSW,  ATTR_VISIBLE, FALSE);
			SetCtrlAttribute (panel, PAN_INST_NUM_SW,   ATTR_VISIBLE, TRUE); 
			break;
					
		case SW_TYPE_COFFSW:
					
			SetCtrlAttribute (panel, PAN_INST_RING_FSW, ATTR_VISIBLE, FALSE);
			SetCtrlAttribute (panel, PAN_INST_BUT_FSW,  ATTR_VISIBLE, FALSE);
			SetCtrlAttribute (panel, PAN_INST_NUM_SW,   ATTR_VISIBLE, TRUE);
			break;
					
		case SW_TYPE_10G:
				
			SetCtrlAttribute (panel, PAN_INST_RING_FSW, ATTR_VISIBLE, TRUE);
			SetCtrlAttribute (panel, PAN_INST_BUT_FSW,  ATTR_VISIBLE, TRUE);
			SetCtrlAttribute (panel, PAN_INST_NUM_SW,   ATTR_VISIBLE, FALSE);
			break;
			
		case SW_TYPE_JHFSW:
					
			SetCtrlAttribute (panel, PAN_INST_RING_FSW, ATTR_VISIBLE, FALSE);
			SetCtrlAttribute (panel, PAN_INST_BUT_FSW,  ATTR_VISIBLE, FALSE);
			SetCtrlAttribute (panel, PAN_INST_NUM_SW,   ATTR_VISIBLE, TRUE);
			break;
			
		case SW_TYPE_COFFSW02:
					
			SetCtrlAttribute (panel, PAN_INST_RING_FSW, ATTR_VISIBLE, FALSE);
			SetCtrlAttribute (panel, PAN_INST_BUT_FSW,  ATTR_VISIBLE, FALSE);
			SetCtrlAttribute (panel, PAN_INST_NUM_SW,   ATTR_VISIBLE, TRUE);
			break;

		default:
			MessagePopup ("提示", "无法支持该光开关!");
			return -1;
			break;	   
	}  
	
	return 0;   
}

void Update_Config_Ver(int panel, BOOL updateVer)
{
	char tempStr[1024];
	int  status, err, index;
	char *ptr;
		
	GetCtrlIndex (panel, PAN_CONF_RING_PN, &my_struConfig.PNIndex);
	GetLabelFromIndex (panel, PAN_CONF_RING_PN, my_struConfig.PNIndex, my_struConfig.PN);
	
	if (updateVer)
	{
		DB_Get_BOM (panel, PAN_CONF_RING_BOM); 
	}
	
	GetCtrlIndex (panel, PAN_CONF_RING_BOM, &index);  
	if(index>=0)
	{
		GetLabelFromIndex (panel, PAN_CONF_RING_BOM, index, my_struConfig.Ver);
	}
	else
	{
		strcpy (my_struConfig.Ver, "");
	}
 
//	GetCtrlVal (panel, PAN_CONF_RAD_BUT_TUN_10G, &my_struConfig.DT_Tun_10G);
//	GetCtrlVal (panel, PAN_CONF_RAD_BUT_TEST_10G, &my_struConfig.DT_Test_10G);
//	GetCtrlVal (panel, PAN_CONF_RAD_BUT_TEST_1G, &my_struConfig.DT_Test_1G);      
//	GetCtrlVal (panel, PAN_CONF_RAD_BUT_QATEST_10G, &my_struConfig.DT_QATest_10G); 
//	GetCtrlVal (panel, PAN_CONF_RAD_BUT_QATEST_1G, &my_struConfig.DT_QATest_1G);  
	GetCtrlVal (panel, PAN_CONF_RAD_BUT_TUN_DWDM, &my_struConfig.DT_Tun_DWDM); 
	GetCtrlVal (panel, PAN_CONF_RAD_BUT_TEST_DWDM, &my_struConfig.DT_Test_DWDM); 
	
	GetCtrlVal (panel, PAN_CONF_RAD_LOW, 	 &my_struConfig.Temper_Low); 
	GetCtrlVal (panel, PAN_CONF_RAD_ROOM, 	 &my_struConfig.Temper_Room); 
	GetCtrlVal (panel, PAN_CONF_RAD_HIGH, 	 &my_struConfig.Temper_High);
	
	/***工序全部改为大写**Eric.Yao***/
	//get version info according PN, temper_sel, ATE_sel
	if(my_struConfig.DT_Tun_10G) 		
	{
		strcpy (my_struConfig.DT_Flag, "TUN_10G");
	} 
	else if(my_struConfig.DT_Test_10G)	
	{
		strcpy (my_struConfig.DT_Flag, "TEST_10G");
	} 
	else if(my_struConfig.DT_Test_1G)	
	{
		strcpy (my_struConfig.DT_Flag, "TEST_1G");
	} 
	else if(my_struConfig.DT_QATest_10G) 
	{
		strcpy (my_struConfig.DT_Flag, "QATEST_10G");
	}   
	else if(my_struConfig.DT_QATest_1G) 
	{
		strcpy (my_struConfig.DT_Flag, "QATEST_1G");
	} 
	else if(my_struConfig.DT_Tun_DWDM) 
	{
		strcpy (my_struConfig.DT_Flag, "TUNING");
	}
	else if(my_struConfig.DT_Test_DWDM) 
	{
		strcpy (my_struConfig.DT_Flag, "TEST");
	}
	else							 	
	{
		strcpy (my_struConfig.DT_Flag, "TUNING");
	}
	
	if (my_struConfig.Temper_Low) 
	{
		strcpy (my_struConfig.Temper_Flag, "LOW");
	}
	else if(my_struConfig.Temper_Room)
	{
		strcpy (my_struConfig.Temper_Flag, "ROOM");
	} 
	else if(my_struConfig.Temper_High)
	{
		strcpy (my_struConfig.Temper_Flag, "HIGH");
	} 
	else							  
	{
		strcpy (my_struConfig.Temper_Flag, "");
	}
	
	//根据PN获取产品类型,i档还是c档
    ptr = strrchr(my_struConfig.PN,'I');
    if ((ptr-my_struConfig.PN)>=8) 
	{
		my_struConfig.PN_CLASS=PN_i;
	}
    else
	{
		my_struConfig.PN_CLASS=PN_c;   
	}
			
	//获取EED版本
	Get_EED_Ver (panel, PAN_CONF_RING_EED);
	SetCtrlAttribute (panel, PAN_CONF_RING_EED, ATTR_DIMMED, 0);
	
	//查询测试项目配置
	err = DB_READ_AutoDT_Spec_ATE_Item ();
	if(err) 
	{   
		//禁用界面的确认按钮，避免获取配置异常后，程序还能使用
		SetCtrlAttribute (panel, PAN_CONF_BUT_OK, ATTR_DIMMED, 1); 
		return ;  
	}
	else
	{
		SetCtrlAttribute (panel, PAN_CONF_BUT_OK, ATTR_DIMMED, 0);	
	}
	
	SetCtrlVal (panel, PAN_CONF_CHE_BOX_T_AOP, my_struConfig.Sel_T_AOP); 
	SetCtrlVal (panel, PAN_CONF_CHE_BOX_T_ER, my_struConfig.Sel_T_ER); 
	SetCtrlVal (panel, PAN_CONF_CHE_BOX_T_DIS, my_struConfig.Sel_T_TxDis); 
	SetCtrlVal (panel, PAN_CONF_CHE_BOX_T_DIS_SOFT, my_struConfig.Sel_T_TxDis_soft);  
	SetCtrlVal (panel, PAN_CONF_CHE_BOX_T_EYE, my_struConfig.Sel_T_Eye); 
	SetCtrlVal (panel, PAN_CONF_CHE_BOX_T_MASK, my_struConfig.Sel_T_Mask); 
	SetCtrlVal (panel, PAN_CONF_CHE_BOX_T_SPECTRUM, my_struConfig.Sel_T_Spectrum);
	SetCtrlVal (panel, PAN_CONF_CHE_BOX_R_SENS, my_struConfig.Sel_R_Sens); 
//	SetCtrlVal (panel, PAN_CONF_CHE_BOX_R_SENS_REAL, my_struConfig.Sel_R_Sens_Real); 
	SetCtrlVal (panel, PAN_CONF_CHE_BOX_R_OVER, my_struConfig.Sel_R_Over);
	SetCtrlVal (panel, PAN_CONF_CHE_BOX_R_SDHYS, my_struConfig.Sel_R_SDHys);
	SetCtrlVal (panel, PAN_CONF_CHE_BOX_CALI, my_struConfig.Sel_Calibrate);
	SetCtrlVal (panel, PAN_CONF_CHE_BOX_CALI_TEST, my_struConfig.Sel_Calibrate_Test);
	SetCtrlVal (panel, PAN_CONF_CHE_BOX_EE_PROTECT, my_struConfig.Sel_EE_P);  
	SetCtrlVal (panel, PAN_CONF_CHE_BOX_T_CROSS, my_struConfig.Sel_T_Cross); 	
	
	ProcessDrawEvents();
}

int DB_Get_BOM(int panel, int control)
{
	int num;
	char buf[1024],bom[50];
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */ 
	
	Fmt (buf, "SELECT Version from AutoDT_Spec_BarCode where PN='%s' ORDER BY Version ASC", my_struConfig.PN);

	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return -1;}  
    
    resCode = DBBindColChar (hstmt, 1, 50, bom, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
    
	ClearListCtrl (panel, control); 
	num=0; 
    while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
    {
		InsertListItem (panel, control, num, bom, num); 
		num++;
	}      
    
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) {ShowDataBaseError();  return -1;}
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	
	if (num==0) {MessagePopup("提示","数据库AutoDT_Spec_BarCode中不能找到相应的数据！"); return -1;}
	
	return 0;
}

//由于BarBOM与PN重复的问题，数据库中相同的记录可能有多条，选取最后一条记录
BOOL DB_GET_PN_TYPE(char *PN_TYPE)
{
	char buf[256];
	char myTYPE[50];
	int	 num;
	
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */

	Fmt (buf, "SELECT PN_TYPE from AutoDT_Spec_Tracking WHERE PartNumber='%s' and BarBOM='%s'", my_struConfig.PN, my_struConfig.Ver); 
	
	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return FALSE;}  
    
    resCode = DBBindColChar (hstmt, 1, 50, myTYPE, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
    
    num=0;
    while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
    {
    	strcpy (PN_TYPE, myTYPE); 
    	num++;
    }      
    
    if (resCode != DB_SUCCESS && resCode != DB_EOF) {ShowDataBaseError();  return FALSE;}
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	
	if (num==0) {MessagePopup("提示","数据库AutoDT_Spec_Tracking中不能找到相应的数据！"); return FALSE;} 
	
	return TRUE;
}

int DB_save_rxcali(int channel, enum enum_CALIBRATION_TYPE cali_type)
{
	char buf[2048];
	float fLoss, fLoss_min, fLoss_max, fVal;
	int resCode;
	char stationid[256];
	
	sprintf (stationid, "%s-%d",my_struProcessLOG.StationID, channel);
	
	switch (cali_type)
	{
		case CALI_TYPE_RX_MAINLOCK:
			
			fLoss     = sRxCal_Lock.power[channel];
			fLoss_min = sRxCal_Lock.power_min[channel];
			fLoss_max = sRxCal_Lock.power_max[channel];
			fVal      = sRxCal_Lock.power_in[channel] - sRxCal_Lock.power[channel];        
			
			break;
			
		case CALI_TYPE_RX_MAINFREE:
			
			fLoss     = sRxCal_Free.power[channel];
			fLoss_min = sRxCal_Free.power_min[channel];
			fLoss_max = sRxCal_Free.power_max[channel];
			fVal      = sRxCal_Free.power_in[channel] - sRxCal_Free.power[channel];        
			
			break;

		default:
			MessagePopup ("提示", "保存收端光路校准值时，不能识别校准类型   "); 
		    return -1;
	}
	
	if (SERVER_ORACLE == my_struConfig.servertype)  	 
	{ 
		sprintf (buf, "INSERT INTO calibration (id, pcname,loss,lossmin,lossmax,sysval,flag) VALUES (s_id.nextval, '%s',%f,%f,%f,%f,'%s')", stationid,fLoss,fLoss_min,fLoss_max,fVal, CALI_TYPE[cali_type]); 
	}
	else
	{
		sprintf (buf, "INSERT INTO calibration (pcname,loss,lossmin,lossmax,sysval,flag) VALUES ('%s',%f,%f,%f,%f,'%s')", stationid,fLoss,fLoss_min,fLoss_max,fVal, CALI_TYPE[cali_type]); 
	}
	resCode = DBImmediateSQL (hdbc, buf); 
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;} 
	
	return 0;
}


void InsertTree(int panel, int control, struct struInstrument *localInst, int collapsed)
{
	char label[256];
	int ParentIndex, cnt, ChildIndex;
	
	//获取通道的index
	sprintf (label, "通道%d", localInst->Channel);
	GetTreeItemFromLabel (panel, control, VAL_ALL, 0, VAL_FIRST, VAL_NEXT_PLUS_SELF, 0, label, &ParentIndex);
	if (ParentIndex>=0)
	{   //删除通道对应的设备后重新添加
		GetNumTreeItems (panel, control, VAL_CHILD, ParentIndex, VAL_FIRST, VAL_NEXT_PLUS_SELF, 0, &cnt);
		DeleteListItem (panel, control, ParentIndex+1, cnt);
	}
	else
	{
		//添加合法的通道
		ParentIndex=InsertTreeItem (panel, control, VAL_SIBLING, 0, VAL_LAST, label, "", NULL, 0); 
	}
	
	//设置通道图标
	if (localInst->ChannelEn)
	{
		SetTreeItemAttribute (panel, control, ParentIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_OK); 
		SetTreeItemAttribute (panel, control, ParentIndex, ATTR_COLLAPSED_IMAGE_INDEX, TREE_IMAGE_OK); 	
	}
	else
	{
		SetTreeItemAttribute (panel, control, ParentIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_ERROR); 
		SetTreeItemAttribute (panel, control, ParentIndex, ATTR_COLLAPSED_IMAGE_INDEX, TREE_IMAGE_ERROR); 
	}
	
	//添加设备
	sprintf (label, LAB_PM,localInst->Power_TYPE,localInst->PMSII,localInst->SN_PM);
	ChildIndex=InsertTreeItem (panel, control, VAL_CHILD, ParentIndex, VAL_LAST, label, "", NULL, 0);
	if (PowMeter_TYPE_NONE == localInst->Power_TYPE)
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_ERROR); 
	}
	else
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_WARING); 
	}
	
	sprintf (label, LAB_ATT_MAI,localInst->ATT_TYPE_MAIN, localInst->SN_ATT_MAIN,localInst->ATT_MAIN_COM);                    
	ChildIndex=InsertTreeItem (panel, control, VAL_CHILD, ParentIndex, VAL_LAST, label, "", NULL, 0);
	if (ATT_TYPE_NONE == localInst->ATT_TYPE_MAIN)
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_ERROR); 
	}
	else
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_WARING); 
	}
	
	sprintf (label, LAB_ATT_REF,localInst->ATT_REF_TYPE, localInst->SN_ATT_REF,localInst->ATT_REF_COM);                    
	ChildIndex=InsertTreeItem (panel, control, VAL_CHILD, ParentIndex, VAL_LAST, label, "", NULL, 0);
	if (ATT_TYPE_NONE == localInst->ATT_REF_TYPE)
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_ERROR); 
	}
	else
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_WARING); 
	}
	
	sprintf (label, LAB_SEVB,localInst->SEVB_TYPE, localInst->SEVB);
	ChildIndex=InsertTreeItem (panel, control, VAL_CHILD, ParentIndex, VAL_LAST, label, "", NULL, 0);
	if (SEVB_TYPE_NONE == localInst->SEVB_TYPE)
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_ERROR); 
	}
	else
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_WARING); 
	}

	sprintf (label, LAB_BERT,localInst->BERT_TYPE, localInst->SN_BERT);
	ChildIndex=InsertTreeItem (panel, control, VAL_CHILD, ParentIndex, VAL_LAST, label, "", NULL, 0);
	if (BERT_TYPE_NONE == localInst->BERT_TYPE)
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_ERROR); 
	}
	else
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_WARING); 
	}
	
	sprintf (label, LAB_BERT_10G,localInst->BERT_TYPE_10G, localInst->SN_10GBERT);
	ChildIndex=InsertTreeItem (panel, control, VAL_CHILD, ParentIndex, VAL_LAST, label, "", NULL, 0);
	if (BERT_TYPE_NONE == localInst->BERT_TYPE_10G)
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_ERROR); 
	}
	else
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_WARING); 
	}
	
	sprintf (label, LAB_SEVB_LIGHT,localInst->EVB_Light_TYPE, localInst->EVB_Light);
	ChildIndex=InsertTreeItem (panel, control, VAL_CHILD, ParentIndex, VAL_LAST, label, "", NULL, 0);
	if (SEVB_TYPE_NONE == localInst->EVB_Light_TYPE)
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_ERROR); 
	}
	else
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_WARING); 
	}
	
	sprintf (label, LAB_DCA,localInst->DCA_TYPE, localInst->DCA_Addr,localInst->SN_DCA);
	ChildIndex=InsertTreeItem (panel, control, VAL_CHILD, ParentIndex, VAL_LAST, label, "", NULL, 0);
	if (DCA_TYPE_NONE == localInst->DCA_TYPE)
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_ERROR); 
	}
	else
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_WARING); 
	}
	
	sprintf (label, LAB_SPECTRUM,localInst->SPECTRUM_TYPE, localInst->Spectrum_Addr,localInst->SN_Spectrum);
	ChildIndex=InsertTreeItem (panel, control, VAL_CHILD, ParentIndex, VAL_LAST, label, "", NULL, 0);
	if (SPECTRUM_TYPE_NONE == localInst->SPECTRUM_TYPE)
	{				
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_ERROR); 
	}
	else
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_WARING); 
	}
	
	sprintf (label, LAB_FIBER,localInst->Fiber);
	ChildIndex=InsertTreeItem (panel, control, VAL_CHILD, ParentIndex, VAL_LAST, label, "", NULL, 0);
	SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_WARING); 
	
	//光开关
	sprintf (label, LAB_SW,localInst->SW_TYPE,localInst->SW,localInst->SW_CHAN, localInst->SW_SN);
	ChildIndex=InsertTreeItem (panel, control, VAL_CHILD, ParentIndex, VAL_LAST, label, "", NULL, 0);
	if (SW_TYPE_NONE == localInst->SW_TYPE)
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_ERROR); 
	}
	else
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_WARING); 
	}
	
	//时钟切换板
	sprintf (label, LAB_CLOCK,localInst->CLOCK_TYPE,localInst->CLOCK,localInst->CLOCK_CHAN);
	ChildIndex=InsertTreeItem (panel, control, VAL_CHILD, ParentIndex, VAL_LAST, label, "", NULL, 0);
	if (CLOCK_TYPE_NONE == localInst->CLOCK_TYPE)
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_ERROR); 
	}
	else
	{
		SetTreeItemAttribute (panel, control, ChildIndex, ATTR_IMAGE_INDEX, TREE_IMAGE_WARING); 
	}
	
	//必须在SortTreeItems前，否则ParentIndex值与控件的实际位置不一致
	SetTreeItemAttribute (panel, control, ParentIndex, ATTR_COLLAPSED, collapsed); 
	
	//必须放在最后
	SortTreeItems (panel, control, ParentIndex, 0, 0, 0, 0, 0);
}

int DB_get_cali(enum enum_CALIBRATION_TYPE cali_type, int channel, BOOL *timeOutFlag)
{
/*
(@pcname nvarchar(50), 
@timein smalldatetime,
@flag nvarchar(50),
@timeout int OUTPUT)
*/
	int  retParam, calitime;
	char timebuf[30], buf[1024];
	int  errcode, resCode, hstmt;
	char stationid[256];
	
	*timeOutFlag = FALSE;
	
	sprintf (stationid, "%s-%d",my_struProcessLOG.StationID, channel);
	
	MyDLL_GetDateTime(timebuf);
	
	/* Prepare a statement which calls the stored procedure */
    resCode = DBSetAttributeDefault(hdbc, ATTR_DB_COMMAND_TYPE, DB_COMMAND_STORED_PROC);
	 if (resCode < 0) {ShowDataBaseError();  return -1;} 
	 
    hstmt = DBPrepareSQL (hdbc, "p_get_calitime");
    if (hstmt <= 0) {ShowDataBaseError();  return -1;} 
	 
	resCode = DBSetAttributeDefault(hdbc, ATTR_DB_COMMAND_TYPE, DB_COMMAND_UNKNOWN);
	if (resCode < 0) {ShowDataBaseError();  return -1;} 
	 
	if (SERVER_ORACLE == my_struConfig.servertype)
	{
	    /* Set the input parameter */
		resCode = DBCreateParamInt (hstmt, "V_return", DB_PARAM_OUTPUT, -1);
		if (resCode < 0) {ShowDataBaseError();  return -1;}
	    resCode = DBCreateParamChar (hstmt, "V_pcname", DB_PARAM_INPUT, stationid, strlen(stationid)+1);   //必须要获取实际的字符串大小，否则定义过长的字符空间，不足的程序会在字符串后以空格补齐
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "V_timein", DB_PARAM_INPUT, timebuf, strlen(timebuf)+1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
		resCode = DBCreateParamChar (hstmt, "V_flag", DB_PARAM_INPUT, CALI_TYPE[cali_type], strlen(CALI_TYPE[cali_type])+1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamInt (hstmt, "V_timeout", DB_PARAM_OUTPUT, -1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	}
	else
	{
	    /* Set the input parameter */
		resCode = DBCreateParamInt (hstmt, "", DB_PARAM_RETURN_VALUE, -1);
		if (resCode < 0) {ShowDataBaseError();  return -1;}
	    resCode = DBCreateParamChar (hstmt, "@pcname", DB_PARAM_INPUT, stationid, strlen(stationid)+1);   //必须要获取实际的字符串大小，否则定义过长的字符空间，不足的程序会在字符串后以空格补齐
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "@timein", DB_PARAM_INPUT, timebuf, strlen(timebuf)+1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
		resCode = DBCreateParamChar (hstmt, "@flag", DB_PARAM_INPUT, CALI_TYPE[cali_type], strlen(CALI_TYPE[cali_type])+1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamInt (hstmt, "@timeout", DB_PARAM_OUTPUT, -1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	}
	
    /* Execute the statement */
    resCode = DBExecutePreparedSQL(hstmt);
	if (resCode < 0) {ShowDataBaseError();  return -1;}  
	
    /* Close the statement.  Output parameters are invalid until the statement is closed */
    resCode = DBClosePreparedSQL(hstmt);
	if (resCode < 0) {ShowDataBaseError();  return -1;} 
	
    /* Examine the parameter values */
	resCode = DBGetParamInt (hstmt, 1, &retParam); 
	if (resCode < 0) {ShowDataBaseError();  return -1;}
	resCode = DBGetParamInt(hstmt, 5, &calitime);
	if (resCode < 0) {ShowDataBaseError();  return -1;} 

    /* Discard the statement */
    resCode = DBDiscardSQLStatement(hstmt);
	if (resCode < 0) {ShowDataBaseError();  return -1;} 

	if (retParam)
	{
		sprintf (buf, "执行p_get_calitime错误代码%d    ",retParam);
		MessagePopup("提示", buf); 
		return -1;
	}
	
	if (calitime>=CALIBRATION_MAX)
	{
		switch (cali_type)
		{
			case CALI_TYPE_TX_CHECKUP:
				strcpy (buf, "本工位发端光路点检时间已超出120分钟，请重新点检    ");
				break;
			case CALI_TYPE_TX:
				strcpy (buf, "本工位发端光路校准时间已超出120分钟，请重新校准    ");
				break;
			case CALI_TYPE_RX:
				strcpy (buf, "本工位收端参考光路校准时间已超出120分钟，请重新校准    ");
				break;
			case CALI_TYPE_RX_MAINLOCK:
				strcpy (buf, "本工位收端主光路锁定模式校准时间已超出120分钟，请重新校准    ");
				break;
			case CALI_TYPE_RX_MAINFREE:
				strcpy (buf, "本工位收端主光路普通模式校准时间已超出120分钟，请重新校准    ");
				break;
			default:
				strcpy (buf, "光路校准类型异常 ");
				break;
		}
		
		*timeOutFlag = TRUE;
		MessagePopup("提示", buf); 
	}
	
	return 0;
}

int DB_get_txcali(enum enum_CALIBRATION_TYPE cali_type)
{
/*
(@pcname nvarchar(50), 
@timein smalldatetime,
@flag nvarchar(50),
@timeout int OUTPUT)
*/
	int  retParam, calitime;
	char timebuf[30], buf[1024];
	
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */
	
	MyDLL_GetDateTime(timebuf);
	
	/* Prepare a statement which calls the stored procedure */
    resCode = DBSetAttributeDefault(hdbc, ATTR_DB_COMMAND_TYPE, DB_COMMAND_STORED_PROC);
	 if (resCode < 0) {ShowDataBaseError();  return -1;} 
	 
    hstmt = DBPrepareSQL (hdbc, "p_get_calitime");
    if (hstmt <= 0) {ShowDataBaseError();  return -1;} 
	 
	resCode = DBSetAttributeDefault(hdbc, ATTR_DB_COMMAND_TYPE, DB_COMMAND_UNKNOWN);
	if (resCode < 0) {ShowDataBaseError();  return -1;} 
	 
	if (SERVER_ORACLE == my_struConfig.servertype)
	{
	    /* Set the input parameter */
		resCode = DBCreateParamInt (hstmt, "V_return", DB_PARAM_OUTPUT, -1);
		if (resCode < 0) {ShowDataBaseError();  return -1;}
	    resCode = DBCreateParamChar (hstmt, "V_pcname", DB_PARAM_INPUT, my_struProcessLOG.StationID, strlen(my_struProcessLOG.StationID)+1);   //必须要获取实际的字符串大小，否则定义过长的字符空间，不足的程序会在字符串后以空格补齐
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "V_timein", DB_PARAM_INPUT, timebuf, strlen(timebuf)+1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
		resCode = DBCreateParamChar (hstmt, "V_flag", DB_PARAM_INPUT, CALI_TYPE[cali_type], strlen(CALI_TYPE[cali_type])+1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamInt (hstmt, "V_timeout", DB_PARAM_OUTPUT, -1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	}
	else
	{
	    /* Set the input parameter */
		resCode = DBCreateParamInt (hstmt, "", DB_PARAM_RETURN_VALUE, -1);
		if (resCode < 0) {ShowDataBaseError();  return -1;}
	    resCode = DBCreateParamChar (hstmt, "@pcname", DB_PARAM_INPUT, my_struProcessLOG.StationID, strlen(my_struProcessLOG.StationID)+1);   //必须要获取实际的字符串大小，否则定义过长的字符空间，不足的程序会在字符串后以空格补齐
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamChar (hstmt, "@timein", DB_PARAM_INPUT, timebuf, strlen(timebuf)+1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
		resCode = DBCreateParamChar (hstmt, "@flag", DB_PARAM_INPUT, CALI_TYPE[cali_type], strlen(CALI_TYPE[cali_type])+1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	    resCode = DBCreateParamInt (hstmt, "@timeout", DB_PARAM_OUTPUT, -1); 
		if (resCode < 0) {ShowDataBaseError();  return -1;} 
	}
	
    /* Execute the statement */
    resCode = DBExecutePreparedSQL(hstmt);
	if (resCode < 0) {ShowDataBaseError();  return -1;}  
	
    /* Close the statement.  Output parameters are invalid until the statement is closed */
    resCode = DBClosePreparedSQL(hstmt);
	if (resCode < 0) {ShowDataBaseError();  return -1;} 
	
    /* Examine the parameter values */
	resCode = DBGetParamInt (hstmt, 1, &retParam); 
	if (resCode < 0) {ShowDataBaseError();  return -1;}
	resCode = DBGetParamInt(hstmt, 5, &calitime);
	if (resCode < 0) {ShowDataBaseError();  return -1;} 

    /* Discard the statement */
    resCode = DBDiscardSQLStatement(hstmt);
	if (resCode < 0) {ShowDataBaseError();  return -1;} 

	if (retParam)
	{
		sprintf (buf, "执行p_get_calitime错误代码%d    ",retParam);
		MessagePopup("提示", buf); 
		return -1;
	}
	
	if (calitime>=CALIBRATION_MAX)
	{
		switch (cali_type)
		{
			case CALI_TYPE_TX_CHECKUP:
				strcpy (buf, "本工位发端光路点检时间已超出120分钟，请重新点检    ");
				break;
			case CALI_TYPE_TX:
				strcpy (buf, "本工位发端光路校准时间已超出120分钟，请重新校准    ");
				break;
			case CALI_TYPE_RX:
				strcpy (buf, "本工位收端光路校准时间已超出120分钟，请重新校准    ");
				break;
			default:
				strcpy (buf, "光路校准类型异常 ");
				break;
		}
		
		MessagePopup("提示", buf); 
	}
	
	return 0;
}

int Fiber_SW_Init(int FSW_TYPE, int COMIndex, int *FSW_10G_Handle, char FSW_SN[30])
{
	int error;
	
	switch (FSW_TYPE)
	{
		case SW_TYPE_FSW: 
			error = FSW_Init (COMIndex);	
			break;
			
		case SW_TYPE_COFFSW:
			error = COFFSW_Init (COMIndex);	
			break; 
			
		case SW_TYPE_10G:
			error = EVB5_Init (FSW_10G_Handle, FSW_SN);
			break; 
			
		case SW_TYPE_JHFSW:
			error = JHFSW_Init (COMIndex);	
			break; 
			
		case SW_TYPE_COFFSW02:
			error = COFFSW_Init (COMIndex);	
			break;
			
		default: 
			return -1;
	}
	
	return error; 
}

int Fiber_SW_Close(int FSW_TYPE, int COMIndex, int FSW_10G_Handle)
{
	int error;
	
	switch (FSW_TYPE)
	{
		case SW_TYPE_FSW: 
			error = FSW_Close (COMIndex);	
			break;
			
		case SW_TYPE_COFFSW:
			error = COFFSW_Close (COMIndex);	
			break;
			
		case SW_TYPE_10G:
				;
			break;
			
		case SW_TYPE_JHFSW: 
			error = JHFSW_Close (COMIndex);	
			break;
			
		case SW_TYPE_COFFSW02:
			error = COFFSW_Close (COMIndex);	
			break;
			
		default: 
			return -1;
	}
	
	return error; 	
}

int Fiber_SW_SetChannel(int FSW_TYPE, int COMIndex, int FSW_10G_Handle, int channel)
{
	int error;
	
	switch (FSW_TYPE)
	{
		case SW_TYPE_FSW: 
			error = FSW_SetChannel (COMIndex, channel);	
			break;
			
		case SW_TYPE_COFFSW:
			error = COFFSW_SetChannel (COMIndex, channel);	
			break; 
		
		case SW_TYPE_10G:  
			error = set10GSwitch(FSW_10G_Handle, channel);
			break;
			
		case SW_TYPE_JHFSW:
			error = JHFSW_SetChannel (COMIndex, channel);	
			break; 
			
		case SW_TYPE_COFFSW02:
			error = COFFSW02_SetChannel (COMIndex, channel);	
			break;
			
		default: 
			return -1;
	}
	
	return error; 
}

int set10GSwitch(int FSW_10G_Handle, int channel)
{
	int error;
	BYTE tmp_channel;
	int count;
	
	channel = channel -1;
	
	if (channel >= 2)	  /***10G光开关只支持0和1通道**Eric.Yao**20131224***/
	{
		return -1;
	}
	
	count = 0;
	do
	{
		error = EVB5_SET_ONU_TxDIS (FSW_10G_Handle, channel);
		if(error<0) { MessagePopup ("Error", "光开关设置失败！"); return error;} 
	
		Delay(1.5);       
		
		error = EVB5_GET_ONU_TxDIS (FSW_10G_Handle, &tmp_channel);
		if(error<0) { MessagePopup ("Error", "光开关设置失败！"); return error;} 
	}while (count<10 && (channel != tmp_channel));
	
	
	if (count >= 5)
	{
		return -1;
	}
	
//	error = EVB5_SET_Switch_Local (EVB5_Handle, 1) ;
//	if(error<0) { MessagePopup ("Error", "光开关设置锁定失败！"); return error;}  
	
	return error;    
}

int checktestbed(int channel, int *val)
{
int	 error; 
int  mod0;

	*val = 0; 

	errChk(EVB5_Get_MOD_DEF0 (INST_EVB[channel], &mod0));
	
	*val = !mod0;

Error:
	
	return error;
}

void Display_PFY(char *batch, char *PN, char *StationID, char *Log_Action, int panel, int control_pass, int control_fail, int control_total, int control_passrate)
{
	Display_PASS_Fail_Rate (hdbc, batch, PN, StationID, Log_Action, panel, control_pass, control_fail, control_total, control_passrate);
	//由于hdbc是function.c内部变量，不是全局变量，为了防止意外，将Display_PASS_Fail_Rate再封装一次，避免将hdbc声明为全局变量；
}

int DB_Get_Handle (void)
{
	return hdbc;
}   

int DB_Read_Order (char order[])
{   
	int  error, num;
	char buf[2056], xlmfw[128], first[50], last[50];
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */ 
	
	sprintf (buf, "SELECT csxlmfw FROM sgd_scdd_trx where pch_tc='%s'", order);
	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return -1;} 
	
	resCode = DBBindColChar (hstmt, 1, 128, xlmfw, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}

    num=0;
    while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) {num++;}      
    
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) 
	{ShowDataBaseError(); return -1;} 
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}

	if (num != 1) {return -1;} 
	
	//获取序列号
	Scan(xlmfw, "%s>%s[xt45]%s[xt45]", first, last);
	
	MyDLL_TRIM(first); 
	MyDLL_TRIM(last);
	
	Scan(first, "%s>%s[w14]", first);
	Scan(last, "%s>%s[w14]", last);     
	
	strcpy(my_struConfig.firstsn, first);
	strcpy(my_struConfig.lastsn, last);

	return 0;
}

int DB_Read_pch (char pn[], char bom[], int panel, int control_pch)
{   
	int  error, num;
	char buf[2056], str[256] = "";
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */ 
	
	char Versin_str[256];
	char materiel_code_str[256];
	
	//获取生产批次
	sprintf (buf, "SELECT distinct pch_tc, version,materiel_code FROM sgd_scdd_trx where (state ='下达' or state ='开工') and partnumber like '%s%%'  order by pch_tc", pn);	 /***批次号增加bom限制条件**Eric.Yao***/
	
	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return -1;} 
	
	resCode = DBBindColChar (hstmt, 1, 256, str, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColChar (hstmt, 2, 256, Versin_str, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	resCode = DBBindColChar (hstmt, 3, 256, materiel_code_str, &DBConfigStat, "");
    if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}
	
    num=0;
	Combo_DeleteComboItem (panel, control_pch, 0, -1);
    while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
	{
		sprintf(str,"%s;%s;%s",str,Versin_str,materiel_code_str);   
		Combo_InsertComboItem (panel, control_pch, num, str);   
		num++;
	}     
	
	SetCtrlVal (panel, control_pch, str);   
    
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) 
	{ShowDataBaseError(); return -1;} 
    
    resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return -1;}

	return 0;
}

int Read_BER(int channel, double GatingTime, double *ErrorRate,int readtype)	  //readtype为1 判断为测误码率
{
	int error;
	int	InstantSD, InstantSyns; 
	unsigned long long InstantErrNum;
	double	InstantElapsedTime, ErrorBit;
	char   	BERT_Status_String[500]; 
	int     Lock, Sync;     
	
	int     i=0;
	double  ErrorRateTemp=0;
	double  ErrorRateAvg=0;  
							   
	switch (my_struInstrument[channel].BERT_TYPE_10G) 
	{
		case BERT_TYPE_SBERT10000:
			
			for(i=0;i<3;i++)
			{
				error = evb16_SetBERTStart(INST_BERT_10G[channel], &Lock, &Sync, 1);
				if (error)
				{
					*ErrorRate=1; 
					return 0;
				}
				Delay (GatingTime);
				error = evb16_GetBERTStatus (INST_BERT_10G[channel], SBERT10G_SignalIn_SMA, &InstantSD, &InstantSyns, &InstantErrNum, &InstantElapsedTime, &ErrorRateTemp, BERT_Status_String);
				if (error) return -1;
				ErrorRateAvg=ErrorRateAvg+ErrorRateTemp;
				if(readtype==1)
				{
					*ErrorRate=ErrorRateAvg;
					return 0;
				}
			}
			*ErrorRate=ErrorRateAvg/3;
			
			break;
	
		case BERT_TYPE_GBERT10000:	
			
			for(i=0;i<3;i++)
			{			
				error = GBERT10000_Check_10G_ONU (INST_BERT_10G[channel], GatingTime, &ErrorRateTemp, &ErrorBit);
				if (error) return -1;
				ErrorRateAvg=ErrorRateAvg+ErrorRateTemp;
				if(readtype==1)
				{
					*ErrorRate=ErrorRateAvg;
					return 0;
				}
			}
			*ErrorRate=ErrorRateAvg/3;        
			break;
			
		default: 
			MessagePopup("提示", "BERT类型调用异常"); 
			return -1;
			break;
	}
	
	return 0;
}


int XFP_Get_Mon_Temperature(int USBHandle, double *Temp)
{
	int error;
	INT8U LowerArr[256]={0}; 
	int i;
	
	union XFPuA0 loacal_XFPONU_uA0; 
	
	memset(loacal_XFPONU_uA0.pStr, 0, sizeof(loacal_XFPONU_uA0));
	
	error = I2C_BYTEs_READ_DLL (USBHandle, 0xA0, 0, 256, LowerArr); 
	if (error<0) 
	{	
		return -1; 
	} 
	
	for (i=0; i<256; i++)
	{
		loacal_XFPONU_uA0.pStr[i] = LowerArr[255-i];
	}
	
	*Temp=loacal_XFPONU_uA0.sStr.temperature * (1/256.);	
	return 0;
}

int DB_Save_Results_Image_for_Oracle(struct struTestData data, struct struProcessLOG prolog)
{
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */
	
	int  count = 0;
	int  index = 0;
	char buf[1000];  
	int  results_id = 0; 
	char tmp_flag[50] = "";
	char tmp_results_image[150000] = {0};
	char tmp_testdate[50] = "";
	char tmp_sn[50] = "";
	int  tmp_image_size = 150000;
	int  tmp_results_id = 0;

	/********查询results id*******************/
	sprintf (buf, "SELECT ID from (select id FROM AutoDT_Results_ATE WHERE OpticsSN = '%s' ORDER BY ID DESC) where rownum=1", data.OpticsSN);
	
	hstmt = DBActivateSQL (hdbc, buf);
	if (hstmt <= 0) {ShowDataBaseError (); return -1; }   

	resCode = DBBindColInt (hstmt, 1, &results_id, &DBConfigStat);
	if (resCode != DB_SUCCESS) {ShowDataBaseError (); return -1;}  
	
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS){count++;}
	
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError (); return -1; }
	
	if (0 >= count)	//数据库中没有相应记录，新建一条记录
	{
		results_id = 0;
	}
	/******************************************/
	
	/**********保存眼图************************/
	sprintf (buf, "SELECT results_id, results_image, image_size, flag, testdate, sn FROM autodt_results_image WHERE sn = '%s' and flag = '%s'", data.OpticsSN, my_struProcessLOG.Log_Action);
	
	hstmt = DBActivateSQL (hdbc, buf);
	if (hstmt <= 0) {ShowDataBaseError (); return -1; }   

	resCode = DBBindColInt (hstmt, 1, &tmp_results_id, &DBConfigStat);
	if (resCode != DB_SUCCESS) {ShowDataBaseError (); return -1;}
	resCode = DBBindColBinary (hstmt, 2, tmp_image_size, tmp_results_image, &DBConfigStat);
	if (resCode != DB_SUCCESS) {ShowDataBaseError (); return -1;}	 
	resCode = DBBindColInt (hstmt, 3, &tmp_image_size, &DBConfigStat);
	if (resCode != DB_SUCCESS) {ShowDataBaseError (); return -1;}	 
	resCode = DBBindColChar (hstmt, 4, 50, tmp_flag, &DBConfigStat, "");
	if (resCode != DB_SUCCESS) {ShowDataBaseError (); return -1;}  
	resCode = DBBindColChar (hstmt, 5, 50, tmp_testdate, &DBConfigStat, "");
	if (resCode != DB_SUCCESS) {ShowDataBaseError (); return -1;}  
	resCode = DBBindColChar (hstmt, 6, 50, tmp_sn, &DBConfigStat, "");
	if (resCode != DB_SUCCESS) {ShowDataBaseError (); return -1;}  
	
	count = 0;
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	 
	{
		count++;
		
		tmp_results_id = results_id;
		tmp_image_size = data.curvesize;  
		strcpy(tmp_flag, prolog.Log_Action);
		strcpy(tmp_testdate, data.TestDate); 
		strcpy(tmp_sn, data.OpticsSN); 
		
		for (index = 0; index < 150000; index++)
		{
			tmp_results_image[index] = data.curve[index];
		}
		
		resCode = DBUpdateBatch (hstmt, DB_AFFECT_ALL);
		if (resCode != DB_SUCCESS) {ShowDataBaseError (); return -1; }  
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) {ShowDataBaseError (); return -1; }
	}
	
	if (0 == count)											 
	{
		tmp_results_id = results_id;
		tmp_image_size = data.curvesize;  
		strcpy(tmp_flag, prolog.Log_Action);
		strcpy(tmp_testdate, data.TestDate); 
		strcpy(tmp_sn, data.OpticsSN); 
		
		for (index = 0; index < 150000; index++)
		{
			tmp_results_image[index] = data.curve[index];
		}
		
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) {ShowDataBaseError (); return -1; }  
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) {ShowDataBaseError (); return -1; }
	}
	
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError (); return -1; }  

	return 0;
}  

int XGPON_OLT_SET_BIAS_Unit_Ex(int handle, int unit)                                               //设置Bias单位
{
	char strOupt[256];
	char strInpt[256];
	
    unsigned int config=0; 																								
	int          error=0;
	unsigned int Slop,Offset,Temper; 
	unsigned int dTemper_L,dTemper_H,dSlop_L,dSlop_H,dOffset_L,dOffset_H;     

	if(4 == unit)
	{
		sprintf(strInpt, "MCU_SET_TABLE(BASE,0,18,0x3C,0x00,0x32,0xCE)");								//10G Bias 4uA  			
		error=SetCommand (handle,strInpt, strOupt);
		if(error!=0) 
		{	 
			return -1; 
		}
	}
	else
	{
		sprintf(strInpt, "MCU_SET_TABLE(BASE,0,18,0x78,0x00,0x64,0x9C)");								//10G Bias 2uA  			
		error=SetCommand (handle,strInpt, strOupt);
		if(error!=0) 
		{	 
			return -1; 
		}
	}

	sprintf(strInpt, "MCU_UPDATE_FLASH(BASE,0)");								//10G Bias 2uA  			
	error=SetCommand (handle,strInpt, strOupt);
	if(error!=0) 
	{	 
		return -1; 
	}
		
	return 0;
} 

BOOL DB_GET_DDMI_Unit(void)
{
	int  count=0;
	char buf[1024];
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */
	
	sprintf (buf, "SELECT Temp_Unit, Vol_Unit, Bias_Unit, Tx_Unit, Rx_Unit FROM AutoDT_Spec_DDMI_Unit WHERE PN ='%s' and BOM='%s'",my_struConfig.PN, my_struConfig.Ver);
		
	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return FALSE;} 
	
	resCode = DBBindColFloat (hstmt, 1, &my_struConfig.Temp_unit, &DBConfigStat);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColFloat (hstmt, 2, &my_struConfig.Vol_unit, &DBConfigStat);   
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColFloat (hstmt, 3, &my_struConfig.Bias_unit, &DBConfigStat);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColFloat (hstmt, 4, &my_struConfig.Tx_unit, &DBConfigStat);   
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
	resCode = DBBindColFloat (hstmt, 5, &my_struConfig.Rx_unit, &DBConfigStat);   
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}
		
	count=0; 
    while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
    {count++;}      
   
    if ((resCode != DB_SUCCESS) && (resCode != DB_EOF)) {ShowDataBaseError();  return FALSE;}
    
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) {ShowDataBaseError(); return FALSE;}	

	if (count == 0)	   {MessagePopup("提示","数据库AutoDT_Spec_DDMI_Unit中不能找到数据！      "); return FALSE;} 

	return TRUE;
}

// ==============================================Supermast 命令配置方式的相关函数=========================================================
int ENTRY_Password (int handle)
{
	int error=0; 
	char strOupt[256];
	char strInpt[256];
	char buf[256];
	
	sprintf(strInpt,"I2C_Write(0x%X,0x7B,%s)",my_struControl.Init_I2cAddr,PassWord[my_struControl.Init_PowerWord_Index]);
	
	error = SetCommand(handle, strInpt, strOupt); 
	if(0 != strcmp(strOupt, "OK")) 
	{
		sprintf(buf,"输入密码命令出错! %s",strInpt);
		MessagePopup ("ERROR", buf);
		return -1; 
	}
	
	return 0;
}

int GET_FirmVersion (int handle,char *str)
{
	int error=0; 
	char strOupt[256];
	char strInpt[256];
	char buf[256];
	
	sprintf(strInpt,my_struControl.Init_GetVersion);
	
	error = SetCommand(handle, strInpt, strOupt); 
	if(error < 0) 
	{
		sprintf(buf,"读取固件版本命令出错! %s",strInpt);
		MessagePopup ("ERROR", buf);
		return -1; 
	}
	
	strcpy(str,strOupt);
	return 0;
}

int GET_CORE_Temper(int handle, float *CoreTemper)	
{
	int error=0; 
	char strOupt[256];
	char strInpt[256];
	int ltemp;
	char buf[256];
	
	sprintf(strInpt,my_struControl.GetTemper_CoreCommd);
	
	error = SetCommand(handle, strInpt, strOupt); 
	if(error != 0) 
	{
		sprintf(buf,"读取核温本命令出错! %s",strInpt);
		MessagePopup ("ERROR", buf);
		return -1; 
	}
	
	sscanf(strOupt, "0x%x", &ltemp);
	*CoreTemper=(double)((short)ltemp)-my_struControl.GetTemper_IndexOffset; 
	return 0;  
}

int GET_CASE_Temper(int handle, float *Temper, float Unit)		
{
	int error;
	INT8U LowerArr[256]={0};
	union XFPuA0 local_XFP_A0_Lower;  
	int index;
	
	error = I2C_BYTE_WRITE_DLL (handle, 0xA0, 127, 1, 0.1);
	if (error<0)
	{
		MessagePopup ("ERROR", "NO Acknowledge from target!");
		return -1;
	}  

	Delay(0.5);
	error = I2C_BYTEs_READ_DLL (handle, 0xA0, 0, 256, LowerArr);
	if (error<0) 
	{
		MessagePopup ("ERROR", "NO Acknowledge from target!");
		return -1;
	}
	
	memset (&local_XFP_A0_Lower, 0, sizeof(local_XFP_A0_Lower));
	for (index =0; index<256; index++)
	{
		local_XFP_A0_Lower.pStr[index] = LowerArr[255-index];   
	}

	*Temper = calculate_temperature_8077(local_XFP_A0_Lower.sStr.temperature, 1, 0, Unit);
	   
	return 0; 
}

/*************************************** APC ************************************/
int SET_1G_APC_Mode (int handle, enum LUT_MODE Mode)	//设置1G APC 模式
{
	int error;
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	switch (Mode)
	{
		case LUT_MODE_MANUAL:
		{
			strcpy (strInpt, my_struControl.TunAOP1G_MODE_M);
			break;
		}
		
		case LUT_MODE_AUTO:
		{
			strcpy (strInpt, my_struControl.TunAOP1G_MODE_A);
			break;
		}
		
		default:
		{
			return -1;
		}
	}	 
		
	error = SetCommand (handle, strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return -1;	
	}
	
	return 0;
}

int SET_10G_APC_Mode (int handle, enum LUT_MODE Mode)	//设置10G APC 模式
{
	int error;
	char strOupt[256];
	char strInpt[256];
	char setmanumode[] = "gn2010_set_cfg(loop, close)";
	char setAuto[] = "gn2010_set_cfg(loop, open)";
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	switch (Mode)
	{
		case LUT_MODE_MANUAL:
		{
			strcpy (strInpt, setmanumode);   // my_struControl.TunAOP10G_MODE_M
			break;
		}
		
		case LUT_MODE_AUTO:
		{
			strcpy (strInpt, setAuto);		        // my_struControl.TunAOP10G_MODE_A
			break;
		}
		
		default:
		{
			return -1;
		}
	}	 
		
	error = SetCommand (handle, strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return -1;	
	}
	
	return 0;
}

/***************************************  APC  ************************************/

/***************************************  MOD  ************************************/
int SET_1G_MOD_Mode (int handle, enum LUT_MODE Mode)	//设置MOD查找表模式   
{
	int error;
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	switch (Mode)
	{
		case LUT_MODE_MANUAL:
		{
			strcpy (strInpt, my_struControl.TunER10G_MODE_M);    
			break;
		}
		
		case LUT_MODE_AUTO:
		{
			strcpy (strInpt, my_struControl.TunER10G_MODE_A);    
			break;
		}
		
		default:
		{
			return -1;
		}
	}	 
		
	error = SetCommand (handle, strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return -1;	
	}
	
	return 0;
}

int SET_1G_APC(int handle, unsigned short SET_APC)	//设置1G APC值  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,%s,0x%X,0x%X)",my_struControl.TunAOP10G_DAC_SET, (unsigned char)(SET_APC&0xFF), (unsigned char)((SET_APC>>8)&0xFF));    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}
int SET_10G_DWDM(int handle, unsigned short SET_Tec)	//设置10G APC值  
{
	char strOupt[256];   
	char strInpt[256];
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	sprintf(strInpt, "mcu_set_adjust(0,m,0x00,0x%04X,0x16a4,0x0850)", SET_Tec);  
	SetCommand(handle,strInpt, strOupt); 
	// 返回字符串还是数值debug
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "输入命令出错!     "); 
		return -1;
	}
	return 0;
}
int SET_10G_DWDM_Cross(int handle)	//设置交叉点 
{
	char strOupt[256];   
	char strInpt[256];
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	sprintf(strInpt, "gn2010_write(138,0x17)");  
	SetCommand(handle,strInpt, strOupt); 
	// 返回字符串还是数值debug
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "输入命令出错!     "); 
		return -1;
	}
	return 0;
}

int SET_10G_DWDM_EA(int handle, unsigned short SET_Ea)	//设置10G APC值  
{
	char strOupt[256];   
	char strInpt[256];
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	sprintf(strInpt, "mcu_set_adjust(6,m,0x00,0x%04X,0xd00,0x000)", SET_Ea);  
	SetCommand(handle,strInpt, strOupt); 
	// 返回字符串还是数值debug
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "输入命令出错!     "); 
		return -1;
	}
	return 0;
}
int SET_10G_APC(int handle, unsigned short SET_APC)	//设置10G APC值  
{
	int error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,%s,0x%X,0x%X)",my_struControl.TunAOP10G_DAC_SET, (unsigned char)(SET_APC&0xFF), (unsigned char)((SET_APC>>8)&0xFF));   
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	} 
	
	//Set 109---DAC=108DAC*1.95mV * 0.9/8mV
	Temp_DAC =(int)(SET_APC * 1.95 * 0.9 / 8);
	sprintf(strInpt,"GN2010_WRITE(109,%d)",Temp_DAC);   
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	} 

	//Set 110---DAC=108DAC*1.95mV * 0.5/8mV
	Temp_DAC =(int)(SET_APC * 1.95 * 0.5 / 8);
	sprintf(strInpt,"GN2010_WRITE(110,%d)",Temp_DAC);   
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	} 
	
	return 0;  
} 

int SET_10G_APC_OverShooting(int handle, unsigned short SET_APC)	//设置10G APC值过冲  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	//Set 109---DAC=108DAC*1.95mV * 0.9/8mV
	Temp_DAC =(int)(SET_APC * 1.95 * 0.9 / 8);
	sprintf(strInpt,"GN2010_WRITE(109,%d)",Temp_DAC);   
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	} 

	//Set 110---DAC=108DAC*1.95mV * 0.5/8mV
	Temp_DAC =(int)(SET_APC * 1.95 * 0.5 / 8);
	sprintf(strInpt,"GN2010_WRITE(110,%d)",Temp_DAC);   
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	} 
	
	return 0;  
} 

int GET_10G_APC(int handle,int *data)
{
	int Dac;
	int error;  
	char strOupt[256];
	char strInpt[256];
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	
	sprintf(strInpt,"gn2010_read(108,1)");   
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	} 
	
	sscanf(strOupt,"0x%x", &Dac); 
	
	*data = Dac;
		
	return 0;  
}

/***************************************  MOD  ************************************/
int SET_10G_MOD_Mode (int handle, enum LUT_MODE Mode)	//设置MOD查找表模式   
{
	int error;
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	switch (Mode)
	{
		case LUT_MODE_MANUAL:
		{
			strcpy (strInpt, my_struControl.TunER10G_MODE_M);    
			break;
		}
		
		case LUT_MODE_AUTO:
		{
			strcpy (strInpt, my_struControl.TunER10G_MODE_A);    
			break;
		}
		
		default:
		{
			return -1;
		}
	}	 
		
	error = SetCommand (handle, strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return -1;	
	}
	
	return 0;
}

int SET_1G_MOD(int handle, unsigned short SET_MOD)	//设置1G APC值  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,%s,0x%X,0x%X)",my_struControl.TunER1G_DAC_SET, (unsigned char)(SET_MOD&0xFF), (unsigned char)((SET_MOD>>8)&0xFF));    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

int SET_10g_DWDM_MOD(int handle, unsigned short SET_MOD)
{
	int error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	char buf[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));

	sprintf(strInpt,"gn2010_write(108,0x%X)", (unsigned char)(SET_MOD&0xFF));   
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

int SET_10G_MOD(int handle, unsigned short SET_MOD)	//设置10G APC值  
{
	int error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	char buf[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));

	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,%s,0x%X,0x%X)",my_struControl.TunER10G_DAC_SET, (unsigned char)(SET_MOD&0xFF), (unsigned char)((SET_MOD>>8)&0xFF));   
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

//LUK
int GET_TEMPE_LUT(int handle, char *LukName, int LukIndex,int Slope_AMP, int Offset_AMP, int *Temper, float *Slope, float *Offset)	//读取查找表      
{
	int  error;   
	int	Temper_Arry[3];
	signed int temp[4]; 
	signed int x;
	int index;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	for(index=0;index<2;index++)
	{
		sprintf(strInpt, "MCU_GET_TABLE(%s,%d,%d,%d)", LukName, LukIndex, index*LUT_BYTE_COUNT, LUT_BYTE_COUNT);
		error=SetCommand (handle, strInpt, strOupt);
		if(error!=0)
		{
			return -1;  
		}
		
		memset(Temper_Arry,0,sizeof(Temper_Arry));
		sscanf(strOupt,"0x%x,0x%x,0x%x,0x%x,0x%x,0x%x",&Temper_Arry[0],&Temper_Arry[1],&temp[0],&temp[1],&temp[2],&temp[3]);
		Temper[index]=(Temper_Arry[0]+(Temper_Arry[1]<<8))-my_struControl.GetTemper_IndexOffset;
		x=(temp[1]<<8)+temp[0];
		if(x>0x7FFF)
		{
			x=x-65536;
		}
		Slope[index]=(float)x/Slope_AMP;
		
		x= ((temp[3]<<8)+temp[2]);  
		if(x>0x7FFF)
		{
			x=x-65536;
		}
		Offset[index]=(float)(x)/Offset_AMP; 
	}
	return 0;
}

int SET_TEMPE_LUT(int handle,char *LukName, int LukIndex,int Slope_AMP, int Offset_AMP, float *DAC, float *Temper)	//写查找表           
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m; 

	int 		   count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算系数和偏移量
	sSlop[0]    = (DAC[1] - DAC[0]) * Slope_AMP / (Temper[1] - Temper[0]);
	sOffset[0]	= ((DAC[0] - sSlop[0] * (Temper[0] + my_struControl.GetTemper_IndexOffset)/Slope_AMP)) * Offset_AMP;
	
			
	sSlop[1]    = (DAC[2] - DAC[1]) * Slope_AMP / (Temper[2] - Temper[1]);
	sOffset[1]	= ((DAC[1] - sSlop[1] * (Temper[1] + my_struControl.GetTemper_IndexOffset)/Slope_AMP)) * Offset_AMP;

			
	offset_m = (120.+ my_struControl.GetTemper_IndexOffset)* sSlop[1]/Slope_AMP +  sOffset[1]/Offset_AMP;   
	
	//写查找表

	//第一段
	count = 0;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", LukName, LukIndex, LUT_BYTE_COUNT*count, \
											 (BYTE)(((BYTE)(Temper[1]+my_struControl.GetTemper_IndexOffset)) & 0xFF),00,\
											 (BYTE)((sSlop[0]) & 0xFF), (BYTE)((sSlop[0] >> 8) & 0xFF), \
											 (BYTE)((sOffset[0]) & 0xFF), (BYTE)((sOffset[0] >> 8) & 0xFF));
	error=SetCommand (handle, strInpt, strOupt);
	if(error!=0)
	{
		return -1;
	}
		
	//第二段
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", LukName, LukIndex, LUT_BYTE_COUNT*count, \
											 (BYTE)(((BYTE)(120+my_struControl.GetTemper_IndexOffset)) & 0xFF),00, 
											 (BYTE)((sSlop[1] ) & 0xFF), (BYTE)((sSlop[1] >> 8)& 0xFF), \
											 (BYTE)((sOffset[1]) & 0xFF), (BYTE)((sOffset[1] >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	//第三段
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", LukName, LukIndex, LUT_BYTE_COUNT*count, \
											 0xFF,00,\
											 00, 00, \
											 (BYTE)((offset_m) & 0xFF), (BYTE)((offset_m >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}

	if(Update_LUK(handle, LukName, LukIndex)<0)
	{
		return -1;	
	}

	return 0;
}

int SET_TEMPE_LUT_High(int handle,char *LukName, int LukIndex,int Slope_AMP, int Offset_AMP, float *DAC, float *Temper)	//写查找表 High      
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m; 
	int            count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算高温系数和偏移量
	sSlop[1]    = (DAC[2] - DAC[1]) * Slope_AMP / (Temper[2] - Temper[1]);
	sOffset[1]	= ((DAC[1] - sSlop[1] * (Temper[1] + my_struControl.GetTemper_IndexOffset)/Slope_AMP)) * Offset_AMP;			

	offset_m = (120 + my_struControl.GetTemper_IndexOffset)* sSlop[1]/Slope_AMP +  sOffset[1]/Offset_AMP; 
//	if(offset_m > my_struControl.TunAOP10G_DAC_MAX)
//	{
		offset_m = my_struControl.TunAOP10G_DAC_MAX;
//	}		  
	
	//写高温段查找表
	count = 1;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", LukName, LukIndex, LUT_BYTE_COUNT*count,\
											 (BYTE)(((BYTE)(120+my_struControl.GetTemper_IndexOffset)) & 0xFF),00, 
											 (BYTE)((sSlop[1] ) & 0xFF), (BYTE)((sSlop[1] >> 8)& 0xFF), \
											 (BYTE)((sOffset[1]) & 0xFF), (BYTE)((sOffset[1] >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	//写高温保护段查找表
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", LukName, LukIndex, LUT_BYTE_COUNT*count,\
											 0xFF,00,\
											 00, 00, \
											 (BYTE)((offset_m) & 0xFF), (BYTE)((offset_m >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	if(Update_LUK(handle, LukName, LukIndex)<0)
	{
		return -1;	
	}

	return 0;
}

int SET_TEMPE_LUT_Low(int handle,char *LukName, int LukIndex,int Slope_AMP, int Offset_AMP, float *DAC, float *Temper)	//写查找表 Low             
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m;    
	int            count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算低温段系数和偏移量
	sSlop[0]    = (DAC[1] - DAC[0]) * Slope_AMP / (Temper[1] - Temper[0]);
	sOffset[0]	= ((DAC[0] - sSlop[0] * (Temper[0] + my_struControl.GetTemper_IndexOffset)/Slope_AMP)) * Offset_AMP;
			
	//写低温段查找表
	count = 0;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", LukName, LukIndex, LUT_BYTE_COUNT*count,\
											 (BYTE)(((BYTE)(Temper[1]+my_struControl.GetTemper_IndexOffset)) & 0xFF),00,\
											 (BYTE)((sSlop[0]) & 0xFF), (BYTE)((sSlop[0] >> 8) & 0xFF), \
											 (BYTE)((sOffset[0]) & 0xFF), (BYTE)((sOffset[0] >> 8) & 0xFF));
	error=SetCommand (handle, strInpt, strOupt);
	if(error!=0)
	{
		return -1;
	}
				
	if(Update_LUK(handle, LukName, LukIndex)<0)
	{
		return -1;	
	}

	return 0;
} 

//APD_LUK
int GET_TEMPE_APD_LUT(int handle, char *LukName, int LukIndex,int Slope_AMP, int Offset_AMP, int *Temper, float *Slope, float *Offset)	//读取查找表      
{
	int  error;   
	int	Temper_Arry[3];
	signed int temp[4]; 
	signed int x;
	int index;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	for(index=1;index<3;index++)
	{
		sprintf(strInpt, "MCU_GET_TABLE(%s,%d,%d,%d)", LukName, LukIndex, index*LUT_BYTE_COUNT, LUT_BYTE_COUNT);
		error=SetCommand (handle, strInpt, strOupt);
		if(error!=0)
		{
			return -1;  
		}
		
		memset(Temper_Arry,0,sizeof(Temper_Arry));
		sscanf(strOupt,"0x%x,0x%x,0x%x,0x%x,0x%x,0x%x",&Temper_Arry[0],&Temper_Arry[1],&temp[0],&temp[1],&temp[2],&temp[3]);
		Temper[index]=(Temper_Arry[0]+(Temper_Arry[1]<<8))-my_struControl.GetTemper_IndexOffset;
		x=(temp[1]<<8)+temp[0];
		if(x>0x7FFF)
		{
			x=x-65536;
		}
		Slope[index]=(float)x/Slope_AMP;
		
		x= ((temp[3]<<8)+temp[2]);  
		if(x>0x7FFF)
		{
			x=x-65536;
		}
		Offset[index]=(float)(x)/Offset_AMP; 
	}
	return 0;
}

int SET_TEMPE_APD_LUT(int handle,char *LukName, int LukIndex,int Slope_AMP, int Offset_AMP, float *DAC, float *Temper)	//写查找表           
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m; 
	
	INT16S			SuperSlope,SuperOffset;
	float			SuperTempe;
	
	int 		   count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算系数和偏移量
	sSlop[0]    = (DAC[1] - DAC[0]) * Slope_AMP / (Temper[1] - Temper[0]);
	sOffset[0]	= ((DAC[0] - sSlop[0] * (Temper[0] + my_struControl.GetTemper_IndexOffset)/Slope_AMP)) * Offset_AMP;
	
			
	sSlop[1]    = (DAC[2] - DAC[1]) * Slope_AMP / (Temper[2] - Temper[1]);
	sOffset[1]	= ((DAC[1] - sSlop[1] * (Temper[1] + my_struControl.GetTemper_IndexOffset)/Slope_AMP)) * Offset_AMP;

			
	offset_m = (120.+ my_struControl.GetTemper_IndexOffset)* sSlop[1]/Slope_AMP +  sOffset[1]/Offset_AMP;   
	
	//写查找表
	
	//超低温段
	 SuperSlope = -7.985 *  Slope_AMP;
	 SuperTempe = -25;
	 SuperOffset = ((DAC[0] - SuperSlope * (Temper[0] + my_struControl.GetTemper_IndexOffset)/Slope_AMP)) * Offset_AMP;  
	 
	count = 0;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", LukName, LukIndex, LUT_BYTE_COUNT*count, \
											 (BYTE)(((BYTE)(SuperTempe + my_struControl.GetTemper_IndexOffset)) & 0xFF),00,\
											 (BYTE)((SuperSlope) & 0xFF),  (BYTE)((SuperSlope >> 8) & 0xFF), \
											 (BYTE)((SuperOffset) & 0xFF), (BYTE)((SuperOffset >> 8) & 0xFF));
	error=SetCommand (handle, strInpt, strOupt);
	if(error!=0)
	{
		return -1;
	}
	
	//第一段
//	count = 0;
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", LukName, LukIndex, LUT_BYTE_COUNT*count, \
											 (BYTE)(((BYTE)(Temper[1]+my_struControl.GetTemper_IndexOffset)) & 0xFF),00,\
											 (BYTE)((sSlop[0]) & 0xFF), (BYTE)((sSlop[0] >> 8) & 0xFF), \
											 (BYTE)((sOffset[0]) & 0xFF), (BYTE)((sOffset[0] >> 8) & 0xFF));
	error=SetCommand (handle, strInpt, strOupt);
	if(error!=0)
	{
		return -1;
	}
		
	//第二段
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", LukName, LukIndex, LUT_BYTE_COUNT*count, \
											 (BYTE)(((BYTE)(120+my_struControl.GetTemper_IndexOffset)) & 0xFF),00, 
											 (BYTE)((sSlop[1] ) & 0xFF), (BYTE)((sSlop[1] >> 8)& 0xFF), \
											 (BYTE)((sOffset[1]) & 0xFF), (BYTE)((sOffset[1] >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	//第三段
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", LukName, LukIndex, LUT_BYTE_COUNT*count, \
											 0xFF,00,\
											 00, 00, \
											 (BYTE)((offset_m) & 0xFF), (BYTE)((offset_m >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}

	if(Update_LUK(handle, LukName, LukIndex)<0)
	{
		return -1;	
	}

	return 0;
}

int SET_TEMPE_APD_LUT_High(int handle,char *LukName, int LukIndex,int Slope_AMP, int Offset_AMP, float *DAC, float *Temper)	//写查找表 High      
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m; 
	int            count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算高温系数和偏移量
	sSlop[1]    = (DAC[2] - DAC[1]) * Slope_AMP / (Temper[2] - Temper[1]);
	sOffset[1]	= ((DAC[1] - sSlop[1] * (Temper[1] + my_struControl.GetTemper_IndexOffset)/Slope_AMP)) * Offset_AMP;			

	offset_m = (120 + my_struControl.GetTemper_IndexOffset)* sSlop[1]/Slope_AMP +  sOffset[1]/Offset_AMP; 
//	if(offset_m > my_struControl.TunAOP10G_DAC_MAX)
//	{
		offset_m = my_struControl.TunAOP10G_DAC_MAX;
//	}		  
	
	//写高温段查找表
	count = 2;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", LukName, LukIndex, LUT_BYTE_COUNT*count,\
											 (BYTE)(((BYTE)(120+my_struControl.GetTemper_IndexOffset)) & 0xFF),00, 
											 (BYTE)((sSlop[1] ) & 0xFF), (BYTE)((sSlop[1] >> 8)& 0xFF), \
											 (BYTE)((sOffset[1]) & 0xFF), (BYTE)((sOffset[1] >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	//写高温保护段查找表
	count++;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", LukName, LukIndex, LUT_BYTE_COUNT*count,\
											 0xFF,00,\
											 00, 00, \
											 (BYTE)((offset_m) & 0xFF), (BYTE)((offset_m >> 8)& 0xFF));
	error=SetCommand (handle, strInpt, strOupt);	
	if(error!=0)
	{
		return -1;
	}
	
	if(Update_LUK(handle, LukName, LukIndex)<0)
	{
		return -1;	
	}

	return 0;
}

int SET_TEMPE_APD_LUT_Low(int handle,char *LukName, int LukIndex,int Slope_AMP, int Offset_AMP, float *DAC, float *Temper)	//写查找表 Low             
{
    int            error;
    INT16S         sSlop[2];
	INT16S         sOffset[2], offset_m;    
	int            count;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
 
	//计算低温段系数和偏移量
	sSlop[0]    = (DAC[1] - DAC[0]) * Slope_AMP / (Temper[1] - Temper[0]);
	sOffset[0]	= ((DAC[0] - sSlop[0] * (Temper[0] + my_struControl.GetTemper_IndexOffset)/Slope_AMP)) * Offset_AMP;
			
	//写低温段查找表
	count = 1;
	sprintf(strInpt, "MCU_SET_TABLE(%s,%d,%d,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x)", LukName, LukIndex, LUT_BYTE_COUNT*count,\
											 (BYTE)(((BYTE)(Temper[1]+my_struControl.GetTemper_IndexOffset)) & 0xFF),00,\
											 (BYTE)((sSlop[0]) & 0xFF), (BYTE)((sSlop[0] >> 8) & 0xFF), \
											 (BYTE)((sOffset[0]) & 0xFF), (BYTE)((sOffset[0] >> 8) & 0xFF));
	error=SetCommand (handle, strInpt, strOupt);
	if(error!=0)
	{
		return -1;
	}
				
	if(Update_LUK(handle, LukName, LukIndex)<0)
	{
		return -1;	
	}

	return 0;
} 

int Update_LUK(int handle, char *LukName, int tableIndex) 			           //更新表LUK
{
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf (strInpt, "MCU_UPDATE_FLASH(%s,%d)", LukName, tableIndex);
	
	SetCommand (handle, strInpt, strOupt); 
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "输入命令出错!"); 
		return -1;
	} 
	return 0; 	
}

int GET_10G_Bias(int handle, float *Bias, float Unit)		//获取10G Bias
{
	int error;
	INT8U LowerArr[256]={0};
	union XFPuA0 local_XFP_A0_Lower;  
	int index;
	
	error = I2C_BYTE_WRITE_DLL (handle, 0xA0, 127, 1, 0.1);
	if (error<0)
	{
		MessagePopup ("ERROR", "NO Acknowledge from target!");
		return -1;
	}  

	Delay(0.5);
	error = I2C_BYTEs_READ_DLL (handle, 0xA0, 0, 256, LowerArr);
	if (error<0) 
	{
		MessagePopup ("ERROR", "NO Acknowledge from target!");
		return -1;
	}
	
	memset (&local_XFP_A0_Lower, 0, sizeof(local_XFP_A0_Lower));
	for (index =0; index<256; index++)
	{
		local_XFP_A0_Lower.pStr[index] = LowerArr[255-index];   
	}

	*Bias = calculate_biascurrent_8077(local_XFP_A0_Lower.sStr.tx_Bias, 1, 0, Unit);
	
	return 0;
		
}

/***************************************DRIVER************************************/
int Save_Flash(int handle ,char *SaveCMD)   			      //更新2010驱动寄存器表  
{
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	
	strcpy(strInpt,SaveCMD);
	
	SetCommand (handle, strInpt, strOupt); 
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "输入命令出错!"); 
		return -1;
	}
	
	SetCommand (handle, "MCU_UPDATE_FLASH(DRIVER, 0)", strOupt);
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "输入命令出错!"); 
		return -1;
	}
	return 0; 		
}

int Update_BASE(int handle, int tableIndex) 			           //更新表BASE
{
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf (strInpt, "MCU_UPDATE_FLASH(BASE,%d)", tableIndex);
	
	SetCommand (handle, strInpt, strOupt); 
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "输入命令出错!"); 
		return -1;
	} 
	return 0; 	
}
/***************************************DRIVER************************************/


/***************************************XGPON TXMON************************************/
int SET_10G_TxMon(int handle, float MON_L, float MON_H, float Unit)  						//设置TxMon 门限    
{
	int error=0;
	
	INT16U TX_MON_L;
	INT16U TX_MON_H;   
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	if(10000* pow(10, MON_L/10.)/(Unit/0.1)>65535) 
    { 
		TX_MON_L = (INT16U)(65535);
	}
	else
	{
    	TX_MON_L = (INT16U)(10000* pow(10, MON_L/10.)/(Unit/0.1));   //dBm转换成0.1uw
	}
	
	if(10000* pow(10, MON_H/10.)/(Unit/0.1)>65535) 
    { 
		TX_MON_H = (INT16U)(65535);
	}
	else
	{
    	TX_MON_H = (INT16U)(10000* pow(10, MON_H/10.)/(Unit/0.1));   //dBm转换成0.1uw
	}
	
	sprintf (strInpt, "MCU_SET_TABLE(BASE,0,%s,0x%x, 0x%x, 0x%x, 0x%x)", my_struControl.TxCal10G_ADD_MIN, (BYTE)(((INT16S)(TX_MON_L)) & 0xFF), (BYTE)(((INT16S)(TX_MON_L)>> 8) & 0xFF), (BYTE)(((INT16S)(TX_MON_H)) & 0xFF), (BYTE)(((INT16S)(TX_MON_H)>> 8) & 0xFF));
	error = SetCommand (handle, strInpt, strOupt);	      //此为设置自动模式
	if(error!=0)
	{	
		return -1;	
	}
	
	return 0;																				  
}

int GET_10G_TxPOWER_ADC(int handle, unsigned short *ADC)   									//读取XGPON TX ADC 
{
	int          error = 0;
    unsigned int temp16U[2];
    char         mode;
	unsigned int liv_adctemp; 
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));

	//Set E Mode
//	error = SetCommand (handle, my_struControl.TxCal10G_MODE_E, strOupt);
//	if (error!=0) 
//	{
//		return -1; 
//	}
	
	error = SetCommand (handle, my_struControl.TxCal10G_ADC_GET, strOupt);
	if (error!=0) 
	{
		return -1; 
	}
	sscanf(strOupt, "0x%x", &liv_adctemp); 
	*ADC=liv_adctemp;
	if(*ADC>3000)
	{
		Delay(0.5);
		error = SetCommand (handle, my_struControl.TxCal10G_ADC_GET, strOupt);
		if (error!=0) 
		{
			return -1; 
		}
		sscanf(strOupt, "0x%x", &liv_adctemp); 
		*ADC=liv_adctemp; 
	}
	
	//Set F Mode
//	error = SetCommand (handle, my_struControl.TxCal10G_MODE_F, strOupt);
//	if (error!=0) 
//	{
//		return -1; 
//	}
	
	return 0;
}

int SET_10G_TXPOW_CAL(int handle, unsigned short *ADC_Array, float *PWR_Array, float Unit) 	//写 Tx校准数据
{
	float slope;
	float offset;
	int   error;
	
	INT16S   nSlope;
	INT16S   nOffset;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	 
	PWR_Array[0] = pow(10, PWR_Array[0] /10.) / (Unit*1E-3); //1LSB=0.1uW 
	PWR_Array[1] = pow(10, PWR_Array[1] /10.) / (Unit*1E-3); //1LSB=0.1uW 
	 
	slope = (PWR_Array[1] - PWR_Array[0]) / (ADC_Array[1] - ADC_Array[0]);
	nSlope = (INT16S)(slope*my_struControl.TxCal10G_SLOPE_AMP);	   				

	offset= PWR_Array[0] - slope * ADC_Array[0];   	//slope四舍五入取整后计算offset**Eric.Yao**20130730
	nOffset = (INT16S)(offset * my_struControl.TxCal10G_OFFSET_AMP);	   										//offset四舍五入取整**Eric.Yao**20130730
	
	sprintf (strInpt, "mcu_set_mon_info(txpower,F,0x%x, 0x%x)", (((INT16S)(nSlope)) & 0xFFFF), (((INT16S)(nOffset)) & 0xFFFF));
	error = SetCommand (handle, strInpt, strOupt);	      
	if (error!=0) 
	{
		return -1; 
	}

	return 0;  
}

int GET_10G_TxMon(int handle, float *TxMon, float Unit)		//计算 TxMon
{
	int error;
	INT8U LowerArr[256]={0};
	union XFPuA0 local_XFP_A0_Lower;  
	int index;
	
	error = I2C_BYTE_WRITE_DLL (handle, 0xA0, 127, 1, 0.1);
	if (error<0)
	{
		MessagePopup ("ERROR", "NO Acknowledge from target!");
		return -1;
	}  

	Delay(0.5);
	error = I2C_BYTEs_READ_DLL (handle, 0xA0, 0, 256, LowerArr);
	if (error<0) 
	{
		MessagePopup ("ERROR", "NO Acknowledge from target!");
		return -1;
	}
	
	memset (&local_XFP_A0_Lower, 0, sizeof(local_XFP_A0_Lower));
	for (index =0; index<256; index++)
	{
		local_XFP_A0_Lower.pStr[index] = LowerArr[255-index];   
	}

	*TxMon = calculate_txpower_8077(local_XFP_A0_Lower.sStr.tx_Power, 1, 0, Unit);
	
	return 0;
		
}

int Set_10G_TX_ON(int handle)                   //打开10G 
{
	int error=0; 
	unsigned char temp;
	unsigned char g_temp;
	
	error = I2C_BYTE_READ_DLL (handle, 0xA0, 110, &temp);
	if (error)
	{
		MessagePopup ("ERROR", "输入命令出错!"); 
		return -1;
	}
	
	g_temp=temp&0xbf;
	error = I2C_BYTE_WRITE_DLL (handle, 0xA0, 110, g_temp, 0.1);
	if (error)
	{
		return -1;
	} 
	
	return 0;	 
}

int Set_10G_TX_OFF(int handle)                  //关闭10G 
{
	int error=0; 
	unsigned char temp;
	unsigned char g_temp;
	
	error = I2C_BYTE_READ_DLL (handle, 0xA0, 110, &temp);
	if (error)
	{
		return -1;
	}
	
	g_temp=temp|0x40;
	
	error = I2C_BYTE_WRITE_DLL (handle, 0xA0, 110, g_temp, 0.1);
	if (error)
	{
		return -1;
	} 

	return 0;
}

int Set_1G_TX_ON(int handle)                    //打开1G 
{
	int error=0; 
	unsigned char temp;
	unsigned char g_temp;
	
	error = I2C_BYTE_READ_DLL (handle, 0xA0, 111, &temp);
	if (error)
	{
		return -1;
	}
	
	g_temp=temp&0xfe;
	error = I2C_BYTE_WRITE_DLL (handle, 0xA0, 111, g_temp, 0.1);
	if (error)
	{
		return -1;
	} 

	return 0;	
}

int Set_1G_TX_OFF(int handle)                   //关闭1G 
{
	int error=0; 
	unsigned char temp;
	unsigned char g_temp;
	
	error = I2C_BYTE_READ_DLL (handle, 0xA0, 111, &temp);
	if (error)
	{
		return -1;
	}
	
	g_temp=temp|0x01;
	error = I2C_BYTE_WRITE_DLL (handle, 0xA0, 111, g_temp, 0.1);
	if (error)
	{
		return -1;
	} 
	return 0;		
}

/***************************************XGPON APD************************************/
int SET_10G_APD_Mode (int handle, enum LUT_MODE Mode)	//设置APD模式
{
	int error;
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	switch (Mode)
	{
		case LUT_MODE_MANUAL:
		{
			sprintf (strInpt, my_struControl.TunAPD_MODE_M);
			break;
		}
		
		case LUT_MODE_AUTO:
		{
			sprintf (strInpt, my_struControl.TunAPD_MODE_A);
			break;
		}
		
		default:
		{
			return -1;
		}
	}	 
		
	error = SetCommand (handle, strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return -1;	
	}
	
	return 0;
}
  	
int GET_10G_APD(int handle, unsigned short* GET_APD)	//读取APD值 
{
	int            error=0;
	int            liv_temp=0; 
	unsigned char  APD_LSB=0;
	unsigned char  APD_MSB=0; 
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf (strInpt, "MCU_GET_TABLE(BASE,0,%s,2)",my_struControl.TunAPD_DAC_SET);      
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}
	
	sscanf(strOupt,"0x%x,0x%x", &APD_LSB, &APD_MSB);                   
	
	*GET_APD=(((unsigned short)APD_MSB)<<8)+APD_LSB;
				
	return 0;	
}

int SET_10G_APD(int handle, unsigned short SET_APD)	//设置APD值  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"MCU_SET_TABLE(BASE,0,%s,0x%x,0x%x)", my_struControl.TunAPD_DAC_SET, (unsigned char)(SET_APD&0xFF), (unsigned char)((SET_APD>>8)&0xFF));    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

int SET_10G_APD_Range(int handle)	//设置APD最大最小值	  
{
	int     error=0;
	int	Temp_DAC;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt,"%s", my_struControl.TunAPD_DAC_SET_RANGE);    
	error = SetCommand (handle, strInpt, strOupt);
	if (error != 0) 
	{
		return -1; 
	}  
	
	return 0;  
}

 //RSSI 
int SET_10G_RSSI_Compensation(int handle, float Kh, float Kl) 				          		//写 Rx的高低温校准参数Kh和Kl 
{
	int error=0;

	INT16U tmp_KH;
	INT16U tmp_KL;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	tmp_KH = Kh * my_struControl.RxCal_LUK_SLOPE_AMP;
	tmp_KL = Kl * my_struControl.RxCal_LUK_SLOPE_AMP;       
	
	sprintf(strInpt, "MCU_SET_TABLE(base,0,%s,0x%02x,0x%02x,0x%02x,0x%02x)",  my_struControl.RxCal_SET_KL, (unsigned char)(tmp_KL&0xFF), (unsigned char)((tmp_KL>>8)&0xFF), (unsigned char)(tmp_KH&0xFF), (unsigned char)((tmp_KH>>8)&0xFF));
	error=SetCommand (handle, strInpt, strOupt); 
	if (error != 0) 
	{
		return -1; 
	}
	
	return 0;	
}

int SET_10G_RXKL(int handle, float Kl) 				                                  		//写 Rx的低温校准参数和Kl     
{
	int error=0;

	INT16U tmp_KH;
	INT16U tmp_KL;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	tmp_KL = Kl * my_struControl.RxCal_LUK_SLOPE_AMP;   

	sprintf(strInpt, "MCU_SET_TABLE(base,0,%s,0x%02x,0x%02x)", my_struControl.RxCal_SET_KL, (unsigned char)(tmp_KL&0xFF), (unsigned char)((tmp_KL>>8)&0xFF));
	error=SetCommand (handle, strInpt, strOupt); 
	if (error != 0) 
	{
		return -1; 
	}
	
	return 0;	
}

int SET_10G_RXKH(int handle, float Kh) 				                                  		//写 Rx的高温校准参数和Kh     
{
	int error=0;

	INT16U tmp_KH;
	INT16U tmp_KL;
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	tmp_KH = Kh * my_struControl.RxCal_LUK_SLOPE_AMP;
	
	sprintf(strInpt, "MCU_SET_TABLE(base,0,%s,0x%02x,0x%02x)", my_struControl.RxCal_SET_KH, (unsigned char)(tmp_KH&0xFF), (unsigned char)((tmp_KH>>8)&0xFF));
	error=SetCommand (handle, strInpt, strOupt); 
	if (error != 0) 
	{
		return -1; 
	}
	
	return 0;	
}

int GET_10G_RX_ADC(int handle, short *ADC)											  		//读取 RSSI ADC    
{
	int error=0;			  
	int liv_adctemp=0; 
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	error = SetCommand (handle, my_struControl.RxCal_ADC_GET, strOupt);
	if (error != 0) 
	{
		return -1; 
	}
	
	sscanf(strOupt, "0x%x", &liv_adctemp);
	*ADC=(short)liv_adctemp;
	
	return 0;
}

int SET_10G_RXP_MON(int handle, enum LUT_MODE Mode)									  		//设置 RSSI模式     
{
	int error;
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	switch (Mode)
	{
		case LUT_MODE_FORMULA:
		{
			sprintf (strInpt, "%s", my_struControl.RxCal_MODE_F);
			break;
		}
		
		case LUT_MODE_EXT:
		{
			sprintf (strInpt, "%s", my_struControl.RxCal_MODE_E);    
			break;
		}
		
		case LUT_MODE_TABLE:
		{
			sprintf (strInpt, "%s", my_struControl.RxCal_MODE_T);    
			break;
		}
		
		default:
		{
			return -1;
		}
	}	 
		
	error = SetCommand (handle, strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return -1;	
	}
	
	return 0;
}

int SET_RXPOW_TABLE(int handle, short *ADC_Array, double *PWR_Array, int CaliNumber)  	//设置RSSI查找表  
{
	int  error=0;
	int  index;
	
	INT8U address;    
	
	INT8U  lslope, mslope, loffset, moffset,lADC,mADC;
	INT16U wADC, wSlope, wOffset;

	char strOupt[256];
	char strInpt[256];
	
	double Slope[30];
	double Offset[30];
	short  Rx_OP_Arr[30]={0};
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	for (index = 0; index < CaliNumber; index++)
	{
		Rx_OP_Arr[index]=(unsigned short)(pow(10, PWR_Array[index] /10.) / 0.1E-3); 
	}
	
	for (index = 0; index < CaliNumber; index++)
	{	 
		Slope[index]=(Rx_OP_Arr[index+1] - Rx_OP_Arr[index]) / ((double)ADC_Array[index+1] - (double)ADC_Array[index]);
		Offset[index]=(Rx_OP_Arr[index] - Slope[index]*(ADC_Array[index]));  
	}
	
	for (index = 0; index < CaliNumber; index++)
	{
		address = index*6;
		
		if (CaliNumber-1 -1 == index)
		{
			wADC = 0x7FFF;
		}
		else
		{
			wADC = ADC_Array[index + 1];   
		}
		
		wSlope = (Slope[index] * my_struControl.RxCal_LUK_SLOPE_AMP)<-65535 ? 65535:(Slope[index] * my_struControl.RxCal_LUK_SLOPE_AMP);   //溢出保护，最大0xFFFF;
		wOffset = Offset[index] * my_struControl.RxCal_LUK_OFFSET_AMP;   
		
		lslope = wSlope&0x00ff;
		mslope = wSlope>>8&0x00ff;
		loffset = wOffset&0x00ff;
		moffset = wOffset>>8&0x00ff;
		lADC = wADC&0x00ff;
		mADC = wADC>>8&0x00ff;  
		
		memset(strInpt, 0, 256);
		memset(strOupt, 0, 256); 
		sprintf(strInpt, "MCU_SET_TABLE(%s,%d,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X)", LUK_NAME[my_struControl.RxCal_LUT_NAME_INDEX], my_struControl.RxCal_LUK_INDEX, address, lADC, mADC, lslope, mslope, loffset, moffset);
		SetCommand(handle, strInpt, strOupt);    
		if(0 != strcmp(strOupt, "OK"))
		{
			MessagePopup ("ERROR", "输入命令出错!     "); 
			return -1;
		} 
	}
	
	if(Update_BASE(handle, my_struControl.RxCal_LUK_INDEX)<0)
	{
		return -1;
	}
	
	error=SET_10G_RXP_MON(handle, LUT_MODE_TABLE);  
	if(error!=0)
	{
		return -1;
	}
	
	if(Update_BASE(handle, 0)<0)
	{
		return -1;
	}
	
	return 0;	
}

int GET_10G_RxMon(int handle, float *RxMon, float Unit)		//计算 RxMon
{
	int error;
	INT8U LowerArr[256]={0};
	union XFPuA0 local_XFP_A0_Lower;  
	int index;
	
	error = I2C_BYTE_WRITE_DLL (handle, 0xA0, 127, 1, 0.1);
	if (error<0)
	{
		MessagePopup ("ERROR", "NO Acknowledge from target!");
		return -1;
	}  

	Delay(0.5);
	error = I2C_BYTEs_READ_DLL (handle, 0xA0, 0, 256, LowerArr);
	if (error<0) 
	{
		MessagePopup ("ERROR", "NO Acknowledge from target!");
		return -1;
	}
	
	memset (&local_XFP_A0_Lower, 0, sizeof(local_XFP_A0_Lower));
	for (index =0; index<256; index++)
	{
		local_XFP_A0_Lower.pStr[index] = LowerArr[255-index];   
	}

	*RxMon = calculate_rxpower_8077(local_XFP_A0_Lower.sStr.rx_Power, 1, 0, Unit);
	
	return 0;
		
}

//TunSD
int SET_SD_Function_State(int handle, enum SD_FUN State)									  		//设置 RSSI模式     
{
	int error;
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	switch (State)
	{
		case SD_FUN_OFF:
		{
			sprintf (strInpt, "%s", my_struControl.TunSD_FUN_OFF);
			break;
		}
		
		case SD_FUN_ON:
		{
			sprintf (strInpt, "%s", my_struControl.TunSD_FUN_ON);    
			break;
		}

		default:
		{
			return -1;
		}
	}	 
		
	error = SetCommand (handle, strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return -1;	
	}
	
	return 0;
}


float calculate_temperature_8077(INT16S data, float slope, float offset, float unit)
{
	float result;
	
	result = (slope * data + offset) * (1/256.) * unit;
	
	return result;
}

float calculate_voltage_8077(INT16U data, float slope, float offset, float unit)
{
	float result; 

	result = (slope * data + offset) * (1.E-6) * unit; 
	
	return result;
}

float calculate_biascurrent_8077(INT16U data, float slope, float offset, float unit)
{
	float result;

	result = (slope * data + offset) * (1.E-3) * unit;    
	
	return result;
}

float calculate_txpower_8077(INT16U data, float slope, float offset, float unit)
{
	float result;

	DisableBreakOnLibraryErrors ();										 
	result = 10.*log10((slope * data + offset) * (1.E-3) * unit);
	EnableBreakOnLibraryErrors ();
	
	return result;
}

float calculate_rxpower_8077(INT16U data, float slope, float offset, float unit)
{
	float result;

	DisableBreakOnLibraryErrors ();										 
	result = 10.*log10((slope * data + offset) * (1.E-3) * unit);
	EnableBreakOnLibraryErrors ();

	return result;
}  

BOOL UpdateTemperPoint(int channel, struct struTestData data)									  		//修改温度校准分界点   
{
	int error;
	char strOupt[256];
	char strInpt[256];
	
	float temper;
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	//更新温度校准 超低温分界点    
	error = GET_CORE_Temper(INST_EVB[channel], &temper);		//核温索引
	if (error)
	{
		return FALSE;	
	}
	
	sprintf (strInpt, "MCU_SET_TABLE(LUK,3,0,0x%X,0x%X)", (BYTE)(((BYTE)(temper + my_struControl.GetTemper_IndexOffset)) & 0xFF),(BYTE)(((BYTE)(temper + my_struControl.GetTemper_IndexOffset)>>8) & 0xFF) );    
	error = SetCommand (INST_EVB[channel], strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return FALSE;	
	}
	
    sprintf (strInpt, "MCU_UPDATE_FLASH(LUK,3)");
	error = SetCommand (INST_EVB[channel], strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return FALSE;	
	}
	
	//更新APD LUK超低温分界点   
	sprintf (strInpt, "MCU_SET_TABLE(LUK,0,0,0x%X,0x%X)", (BYTE)(((BYTE)(temper + my_struControl.GetTemper_IndexOffset)) & 0xFF),(BYTE)(((BYTE)(temper + my_struControl.GetTemper_IndexOffset)>>8) & 0xFF) );    
	error = SetCommand (INST_EVB[channel], strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return FALSE;	
	}
	
    sprintf (strInpt, "MCU_UPDATE_FLASH(LUK,0)");
	error = SetCommand (INST_EVB[channel], strInpt, strOupt);	      //此为设置查找表模式
	if(error != 0)
	{
		return FALSE;	
	}
	
	return TRUE;
}

//Cross 10G

BOOL SET_TXCross_DAC_10G(int handle,int data)
{
	char strOupt[256];
	char strInpt[256];
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	sprintf(strInpt, "gn2010_write(89,0x%02X)", data);  
	SetCommand(handle,strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "输入命令出错!     "); 
		return FALSE;
	}
	return TRUE;
}


BOOL GET_TXCross_DAC_10G(int handle,int *data)
{
	int wValue,error;  
	char strOupt[256];
	char strInpt[256];
	
	memset(strInpt, 0, 256); 
	memset(strOupt, 0, 256); 
	error = SetCommand (handle, "gn2010_read(89,1)", strOupt);
	if (error != 0) 
		return -1;  
	sscanf(strOupt,"0x%x", &wValue); 
	
	*data = wValue;
		
	return TRUE;
}


int XGPON_OLT_SET_LOS_DAC(int handle)                                               //设置LOS DAC
{
	char strOupt[256];
	char strInpt[256];
	
    unsigned int config=0; 																								
	int          error=0;

	sprintf(strInpt, "MCU_SET_ADJUST(5,M,0x150,0xFFF,0x00)");										
	error=SetCommand (handle,strInpt, strOupt);
	if(error!=0) 
	{	 
		return -1; 
	}

	sprintf(strInpt, "MCU_UPDATE_FLASH(BASE,0)");										
	error=SetCommand (handle,strInpt, strOupt);
	if(error!=0) 
	{	 
		return -1; 
	}
		
	return 0;
} 
BOOL XGSPON_OLT_GET_A2_110_BYTE(int handle, int *data)
{
	int wValue; 
	
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	strcpy(strInpt, "I2C_READ(0xA2,110,1)");
	SetCommand(handle, strInpt, strOupt);    
	sscanf(strOupt,"0x%x", &wValue);
	
	*data = wValue;
	return TRUE;
}


BOOL testRxLOS(int channel, struct struTestData *data)
{
	int dac, value;
	BOOL Status;
	double powerlosa, powerlosd;
	char strInpt[256], strOupt[256];    
	
	//设置衰减模式
	if(GVPM_SET_LockEnable(INST_ATT[channel],0)<0)
	{
		return FALSE;
	}
	
	powerlosa = my_struDBConfig.SDD+sRxCal_Lock.power[channel];			  //-27
	Status = Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN, powerlosa);  //los assert     
	if (Status)
	{
		MessagePopup("Error", "设置衰减失败!   ");  
		return FALSE;
	} 
	
	Delay(2);
	
	if (!XGSPON_OLT_GET_A2_110_BYTE (INST_EVB[channel], &dac))
	{
		return FALSE;     
	}   
	value = dac & 0x02;
	if (0x02 != (dac & 0x02))	
	{
		return FALSE; 	
	}
	
	powerlosd = my_struDBConfig.SDA+sRxCal_Lock.power[channel];		      //-17
	Status = Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN, powerlosd);  //los deassert
	if (Status)
	{
		MessagePopup("Error", "设置衰减失败!   ");  
		return FALSE;
	} 
	
	Delay(2);	
	if (!XGSPON_OLT_GET_A2_110_BYTE (INST_EVB[channel], &dac))
	{
		return FALSE;     
	}
	value = dac & 0x02; 
	if ( 0x02 == (dac & 0x02))	
	{
		return FALSE;  
	}
	
	data->SDA = my_struDBConfig.SDA;
	data->SDD = my_struDBConfig.SDD;
	data->SDHys = my_struDBConfig.SDA-my_struDBConfig.SDD;
	
	return TRUE;
}

BOOL XGPON_OLT_SET_LOS_Ex(int handle,int data)			//10Km、20Km调试LOS中使用
{
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	sprintf(strInpt, "gn2010_write(56,0x%02X)",(data & 0xFF));  
	SetCommand(handle, strInpt, strOupt); 
	sprintf(strInpt, "gn2010_write(57,0x%02X)",((data>>8) & 0xFF));  
	SetCommand(handle, strInpt, strOupt); 
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "输入命令出错! "); 
		return FALSE;
	} 
	
	return TRUE;     
}

BOOL tuningSD1(int channel, struct struTestData *data)				  //调SDA,测SDD
{
	INT8U 	DACmax, DACmin, DACstep, DACstep_Arr[3]={20, 10, 1}, SD;
	int     DAC, status, j, error;
	double 	temp_sd;
	char    buf[1024]; 
	INT8U   DACinin=0;	   //用于给DAC重新赋初值   zhi.xu20140423
	double  temper;
	BOOL Status; 
	char strInpt[256], strOupt[256]; 
	int dac, value;   
		
//	设置模式
		
	DACmax=0x3F;
	DACmin=0x0;
	
	DAC=DACmax;
	
	if (!XGPON_OLT_SET_LOS_Ex(INST_EVB[channel],DAC))			 
	{
		return FALSE;
	}
	
	//设置衰减模式
	if(GVPM_SET_LockEnable(INST_ATT[channel],0)<0)
	{
		return FALSE;
	}
		
	temp_sd=my_struDBConfig.SDD+sRxCal_Lock.power[channel]; 
	status = Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sd);   
	if (status)
	{
		return FALSE;
	}
	
	Delay(1);
	
	if (!XGSPON_OLT_GET_A2_110_BYTE (INST_EVB[channel], &dac))
	{
		return FALSE;     
	}
	value = dac & 0x02;
	if ( 0x02 != (dac & 0x02))	
	{
		return FALSE; 
	}
	
	temp_sd=my_struDBConfig.SDA+sRxCal_Lock.power[channel];  
	status = Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sd);   
	if (status)
	{
		return FALSE;
	}
	
	DAC=DACmin;  
	if (!XGPON_OLT_SET_LOS_Ex(INST_EVB[channel],DAC))			 
	{
		return FALSE;
	}
	
	Delay(1);
	if (!XGSPON_OLT_GET_A2_110_BYTE (INST_EVB[channel], &dac))
	{
		return FALSE;     
	}
	value = dac & 0x02;
	if ( 0x02 == (dac & 0x02))	
	{
		return FALSE; 
	}
	
	//开始调试SDA 
	DAC=DACmax;		 
	
	if (!XGPON_OLT_SET_LOS_Ex(INST_EVB[channel],DAC))			 
	{
		return FALSE;
	}
	
	temp_sd=my_struDBConfig.SDA+sRxCal_Lock.power[channel]; 
	status = Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sd);   
	if (status)
	{
		return FALSE;
	}
	
	Delay(1);
	if (!XGSPON_OLT_GET_A2_110_BYTE (INST_EVB[channel], &dac))
	{
		return FALSE;     
	}
	value = dac & 0x02;
	if ( 0x02 == (dac & 0x02))	
	{
		j=0;
		do
		{
			do
			{
				DACstep=DACstep_Arr[j];
				DAC=DAC-DACstep;
				DAC=DAC<DACmin ? DACmin:DAC;//防止溢出
				
				if (!XGPON_OLT_SET_LOS_Ex(INST_EVB[channel],DAC))			 
				{
					return FALSE;
				}
				
				Delay(1);
				if (!XGSPON_OLT_GET_A2_110_BYTE (INST_EVB[channel], &dac))
				{
					return FALSE;     
				}
				value = dac & 0x02;
				if ( 0x02 != (dac & 0x02))
				{
					break;
				}
			} while (DAC>DACmin);		//如果没有到达Rsd的最大步长继续循环
				
			if (DAC==DACmin) 
			{
				break;  
			}
			
			if (DACstep==DACstep_Arr[2])    
			{
				break;   //得到了最小的step
			}
			
			DAC=DAC+DACstep;
			if (!XGPON_OLT_SET_LOS_Ex(INST_EVB[channel],DAC))			 
			{
				return FALSE;
			}
			
			status = Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN, -15+sRxCal_Lock.power[channel]);   
			if (status)
			{
				return FALSE;
			}
	
			Delay(1);

			temp_sd=my_struDBConfig.SDA+sRxCal_Lock.power[channel]; 
			status = Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sd);   
			if (status)
			{
				return FALSE;
			}
			
			j++;
		}while (DAC<DACmax);
	}
	
	//调试结束后处于SDD状态，添加SDD测试
	temp_sd=my_struDBConfig.SDD+sRxCal_Lock.power[channel]; 
	status = Set_Att(channel, my_struInstrument[channel].ATT_TYPE_MAIN, temp_sd);   
	if (status)
	{
		return FALSE;
	}
	
	Delay(1);
	sprintf (buf, "Rsd=%d SDA test fail", DAC); 
	if (!XGSPON_OLT_GET_A2_110_BYTE (INST_EVB[channel], &dac))
	{
		return FALSE;     
	}
	value = dac & 0x02;
	if ( 0x02 != (dac & 0x02))
	{
		MessagePopup ("Error", buf);
		goto Error;
	}
	
	if (!XGSPON_OLT_GN2010_SAVE_FLASH(INST_EVB[channel])) 
	{
		return FALSE;
	}
	
	if (!XGSPON_OLT_MCU_Update_Driver0(INST_EVB[channel])) 
	{
		return FALSE; 
	}
	
	data->SDA = my_struDBConfig.SDA;
	data->SDD = my_struDBConfig.SDD;
	data->SDHys = my_struDBConfig.SDA-my_struDBConfig.SDD;
	data->ucRsd=DAC; 

	return TRUE;
	
Error:
	
	return FALSE; 
}
BOOL XGSPON_OLT_MCU_Update_Driver0(int handle)
{
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	strcpy(strInpt, "MCU_UPDATE_FLASH(DRIVER,0)"); 	
	SetCommand(handle, strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "输入命令出错!     "); 
		return FALSE;
	} 
	return TRUE; 
}
BOOL XGSPON_OLT_GN2010_SAVE_FLASH(int handle)
{
	char strOupt[256];
	char strInpt[256];
	
	memset(strOupt, 0, sizeof(strOupt));
	memset(strInpt, 0, sizeof(strInpt));
	
	strcpy(strInpt, "GN2010_SAVE_REG()"); 	
	SetCommand(handle, strInpt, strOupt);    
	if(0 != strcmp(strOupt, "OK"))
	{
		MessagePopup ("ERROR", "输入命令出错!     "); 
		return FALSE;
	} 
	return TRUE;
}

