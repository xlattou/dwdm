#include "cvi_db.h"

#include "FPY.h"	 

int Display_PASS_Fail_Rate (int hdbc, char *batch, char *PN, char *StationID, char *Log_Action, int panel, int control_pass, int control_fail, int control_total, int control_passrate)
{
	int error;
	int pass;
	int fail;
	int total;
	float passrate;
	
	memset (&pass, 0, sizeof(pass));
	memset (&fail, 0, sizeof(fail));  
	memset (&total, 0, sizeof(total));  
	memset (&passrate, 0, sizeof(passrate));  
	
	error = DB_GET_Satation_Count(hdbc, batch, PN, StationID, Log_Action, &pass, &fail);
	if (error)
	{
		return -1;
	}
	
	total = pass + fail;
	passrate = (((float) (pass))/ total) * 100.;
	
	SetCtrlVal (panel, control_pass, 		pass);
	SetCtrlVal (panel, control_fail, 		fail);   
	SetCtrlVal (panel, control_total, 		total);      
	SetCtrlVal (panel, control_passrate, 	passrate);   
	
	return 0;
}  

int DB_GET_Satation_Count(int hdbc, char *batch, char *PN, char *StationID, char *Log_Action, int *pass, int *fail)
{
	int error;
	
	int resCode = 0;   /* Result code                      */ 
	int hstmt = 0;	   /* Handle to SQL statement          */ 
	int DBConfigStat;  /* Status code                      */
	
	int num;
	char buf[1024];
	
	int tmp_Pass;
	int tmp_Fail;   
	
	num=0;
	memset (buf, 0, sizeof(buf)); 
	tmp_Pass = 0;
	tmp_Fail = 0;
	
	//***查询PASS数量**Eric.Yao**20140828***
	Fmt (buf, "select count(*)  as cnt from autodt_results_ate where id in (select resultsID from autodt_process_log where sn in (select bosa_sn from autodt_tracking where manufacture_group = '%s' and partnumber = '%s') and log_action = '%s' and stationID = '%s') and errorcode = 0", batch, PN, Log_Action, StationID);
	
	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return -1;}  
    
    resCode = DBBindColInt (hstmt, 1, &tmp_Pass, &DBConfigStat);
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
	
	if (num!=1) 
	{
		return -1;      
	}   
	      
	//***查询Fail数量**Eric.Yao**20140828***
	num=0; 
	Fmt (buf, "select count(*) as cnt from autodt_results_ate where id in (select resultsID from autodt_process_log where sn in (select bosa_sn from autodt_tracking where manufacture_group = '%s' and partnumber = '%s') and log_action = '%s' and stationID = '%s') and errorcode != 0", batch, PN, Log_Action, StationID);
	
	hstmt = DBActivateSQL (hdbc, buf);
    if (hstmt <= 0) {ShowDataBaseError(); return -1;}  
    
    resCode = DBBindColInt (hstmt, 1, &tmp_Fail, &DBConfigStat);
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
	
	if (num!=1) 
	{
		return -1;      
	}  
	
	*pass = tmp_Pass;
	*fail = tmp_Fail;

	return 0;
}

