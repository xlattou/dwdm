#include "CVIDLL.h" 
   
#include "TxCalibration.h"
#include "global.h"
#include "toolbox.h"
#include "function.h" 

//#include "XGSPON_OLT.h"  

int CVICALLBACK On_CaliT_OK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int i, err, channel;
	float temp_cal, temp_st, temp, temp_delta=999.;
	char buf[1024];
	
	switch (event)
	{
		case EVENT_COMMIT:

			for (channel=0; channel<CHANNEL_MAX; channel++)
			{
				if (my_struInstrument[channel].ChannelEn)
				{
					temp_delta=999.;
					for (i=0; i<3; i++)
					{
						GetTableCellVal (phCaliT, PAN_CALI_T_TABLE, MakePoint (2, channel*3+i+1), &temp_st); 
						GetTableCellVal (phCaliT, PAN_CALI_T_TABLE, MakePoint (3, channel*3+i+1), &temp_cal); 
				
						//获得最小差值
						temp = temp_st-temp_cal;
						if (temp<temp_delta)
						{
							temp_delta = temp;	
				
							if(temp_delta>my_struTxCal.power_max[channel] || temp_delta<my_struTxCal.power_min[channel])
							{
								sprintf (buf, "通道%d的校准光功率超出设置范围    ", channel);
								MessagePopup("提示", buf);
								return -1;
							}
						}
					}
					
					my_struTxCal.power[channel] = temp_delta;
					my_struTxCal.power_st[channel] = temp_st;
	
					my_struTxCal.power_array[channel*4+3] = temp_delta;
					my_struTxCal.power_array[channel*4+0] = temp_st;
	
					//保存校准结果到数据库
					DB_save_txcali(channel); 
				
					//update Flag
					my_struTxCal.flag[channel]=TRUE;
			
					//Power off
					err = SET_EVB_SHDN(channel, 0); 
					if (err) return -1;
					Delay(0.8);
					err = SET_EVB_SHDN(channel, 1); 
					if (err) return -1;
				}
			}
			
			//将校准参数保存到文件
			err = SetCaliConfigFile ();
			if (err) return -1;
			
			HidePanel (phCaliT); 
			
			break;
	}
	return 0;
}

int CVICALLBACK On_CaliT_Set (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			MyDLL_License_Load ();
			showphID = License_Flag_panCALI;  
			
			break;
	}
	return 0;
}

int CVICALLBACK On_CaliT_Quit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int channel;
	
	switch (event)
	{
		case EVENT_COMMIT:

			HidePanel (phCaliT);
			
			for (channel=0; channel<CHANNEL_MAX; channel++)
			{
				if (my_struInstrument[channel].ChannelEn)
				{
					//Power off 
					if (SET_EVB_SHDN(channel, 0)<0) 
					{MessagePopup("Error", "Set Power OFF error!");}
				}
			}
			
			break;
	}
	return 0;
}

int CVICALLBACK On_CaliT_Table (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	Point focus; 
	double Power;
	int channel;
	int error;
	
	switch (event)
	{
		case EVENT_COMMIT:

			GetActiveTableCell(panel, control, &focus);
			
            if (focus.x == 4)
			{
				//计算通道
				channel = (focus.y-1)/3;
				
				if (GET_Power(channel, &Power)<0) 
				{
					MessagePopup ("ERROR", "Read power meter error !"); 
					return -1;
				} 
				
				SetTableCellVal (panel, PAN_CALI_T_TABLE, MakePoint (3, focus.y), Power);
			}

			break;
	}
	return 0;
}

int CVICALLBACK On_Cali_Limit_OK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int err;
	int channel;
	
	switch (event)
	{
		case EVENT_COMMIT:

			for (channel=0; channel<CHANNEL_MAX; channel++)
			{
				GetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (4, channel*3+1), &my_struTxCal.power_min[channel]); 
				GetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (5, channel*3+1), &my_struTxCal.power_max[channel]); 
				my_struTxCal.power_array[channel*4+1] = my_struTxCal.power_min[channel]; 
				my_struTxCal.power_array[channel*4+2] = my_struTxCal.power_max[channel]; 
	
				GetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (3, channel*3+2), &sRxCal_Lock.power_in[channel]); 
				GetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (4, channel*3+2), &sRxCal_Lock.power_min[channel]); 
				GetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (5, channel*3+2), &sRxCal_Lock.power_max[channel]); 
				sRxCal_Lock.power_array[channel*4+0] = sRxCal_Lock.power_in[channel]; 
				sRxCal_Lock.power_array[channel*4+1] = sRxCal_Lock.power_min[channel];
				sRxCal_Lock.power_array[channel*4+2] = sRxCal_Lock.power_max[channel];
				
				GetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (3, channel*3+3), &sRxCal_Free.power_in[channel]); 
				GetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (4, channel*3+3), &sRxCal_Free.power_min[channel]); 
				GetTableCellVal (phCaliLimit, PAN_CALI_L_TABLE, MakePoint (5, channel*3+3), &sRxCal_Free.power_max[channel]); 
				sRxCal_Free.power_array[channel*4+0] = sRxCal_Free.power_in[channel]; 
				sRxCal_Free.power_array[channel*4+1] = sRxCal_Free.power_min[channel];
				sRxCal_Free.power_array[channel*4+2] = sRxCal_Free.power_max[channel];
			}
			
			err = SetCaliConfigFile ();
			if (err) return -1;
			
			HidePanel (phCaliLimit); 
			
			break;
	}
	return 0;
}

int CVICALLBACK On_Cali_Limit_Quit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			HidePanel (phCaliLimit);
			
			break;
	}
	return 0;
}

int CVICALLBACK On_CheckT_Quit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			HidePanel (phCheckT); 
			
			break;
	}
	return 0;
}

void CVICALLBACK On_CheckT (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	int progresshandle;
	int channel;
	int error;
	
	DisplayPanel (phCheckT); 
	
	progresshandle = CreateProgressDialog ("设备配置状态", "设备配置进度", 1, VAL_NO_INNER_MARKERS, "");
	
	//for BERT
	for (channel=0;channel<CHANNEL_MAX;channel++)
	{
		UpdateProgressDialog (progresshandle, (int)(100.*channel/CHANNEL_MAX), 0);
	
		if (my_struInstrument[channel].ChannelEn)
		{
			SetTableCellRangeAttribute (phCheckT, PAN_CHE_T_TABLE, MakeRect (channel+1, 2, 1, 3), ATTR_CELL_DIMMED, 0);
			
			//Power on
			errChk(SET_EVB_SHDN(channel, 1));
	
			//清除控件显示信息
			SetTableCellVal (phCheckT, PAN_CHE_T_TABLE, MakePoint (2, channel+1), ""); 
			SetTableCellVal (phCheckT, PAN_CHE_T_TABLE, MakePoint (3, channel+1), 0.00); 
		}
	//	else
		{
			SetTableCellRangeAttribute (phCheckT, PAN_CHE_T_TABLE, MakeRect (channel+1, 2, 1, 3), ATTR_CELL_DIMMED, 1); 	
		}
	}
	
Error:
		
	DiscardProgressDialog (progresshandle); 
	
}

int CVICALLBACK On_CheckT_OK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int error=0;
	int channel;
	char buf[1024];
	
	switch (event)
	{
		case EVENT_COMMIT:

			for (channel=0; channel<CHANNEL_MAX; channel++)
			{
			//	if (my_struInstrument[channel].ChannelEn)
				{
					GetTableCellVal (phCheckT, PAN_CHE_T_TABLE, MakePoint (2, channel+1), sTxCheck.sn_array[channel]);
					GetTableCellVal (phCheckT, PAN_CHE_T_TABLE, MakePoint (3, channel+1), &sTxCheck.power_sys[channel]); 
					
					if (0==strlen(sTxCheck.sn_array[channel]))
					{
						sprintf (buf, "请先读取通道%d的标准模块序列号    ", channel);
						MessagePopup ("提示", buf); 
						return -1;
					}
			
					//读取点检门限
				//	error = DB_get_txchecklimit(channel);
					if (error)
					{
						return -1;
					}
			
					if (sTxCheck.power_sys[channel]>sTxCheck.power_max[channel] || sTxCheck.power_sys[channel]<sTxCheck.power_min[channel])
					{
						sprintf (buf, "通道%d点检模块功率读数超出设定范围，点检失败    \n请重新校准发端光路", channel);
						MessagePopup ("提示", buf); 
						return -1;
					}
			
				//	error = DB_save_txcheckup (channel, sTxCheck.sn_array[channel], sTxCheck.power_sys[channel]);
					if (error)
					{
						MessagePopup ("提示", "保存点检数据错误   ");        
						return -1;
					}
				}
			}
			
			HidePanel (phCheckT); 
			
			break;
	}
	return 0;
}

int CVICALLBACK On_Check_Table (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char sn[30];
	double val;  
	int channel;
	Point focus; 
	
	switch (event)
	{
		case EVENT_COMMIT:

			GetActiveTableCell(panel, control, &focus);
			
			if (focus.x == 4) 
			{
				channel = focus.y-1;
				
				MyDLLReadModuleSN (INST_EVB[channel], sn);
			
				MyDLLCheckSN (sn);
			
			//	if (Get_Power(channel, &val)<0) 
				{
					MessagePopup ("ERROR", "Read power meter error !"); 
					return -1;
				} 
			
				val += my_struTxCal.power[channel]; 
			
				SetTableCellVal (panel, PAN_CHE_T_TABLE, MakePoint (2, focus.y), sn); 
				SetTableCellVal (panel, PAN_CHE_T_TABLE, MakePoint (3, focus.y), val); 
			}
			
			break;
	}
	return 0;
}
