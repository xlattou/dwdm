/****************************************************************************
 *
 * File:                FPY.h
 *
 * Author:              Eric.Yao
 *
 * Description:         FPY´úÂë
 *
 * Time:                2014-08£­28
 *
 * version:				v1.0.0.0
 *
****************************************************************************/
#ifndef _FPY_H_
#define _FPY_H_ 

#include "Function.h" 


int Display_PASS_Fail_Rate (int hdbc, char *batch, char *PN, char *StationID, char *Log_Action, int panel, int control_pass, int control_fail, int control_total, int control_passrate);

int DB_GET_Satation_Count(int hdbc, char *batch, char *PN, char *StationID, char *Log_Action, int *pass, int *fail);

#endif  

