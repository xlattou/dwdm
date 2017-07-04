/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2016. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PAN_CALI_L                       1
#define  PAN_CALI_L_BUT_OK                2       /* callback function: On_Cali_Limit_OK */
#define  PAN_CALI_L_BUT_QUIT              3       /* callback function: On_Cali_Limit_Quit */
#define  PAN_CALI_L_TABLE                 4

#define  PAN_CALI_T                       2
#define  PAN_CALI_T_BUT_SET               2       /* callback function: On_CaliT_Set */
#define  PAN_CALI_T_BUT_OK                3       /* callback function: On_CaliT_OK */
#define  PAN_CALI_T_BUT_QUIT              4       /* callback function: On_CaliT_Quit */
#define  PAN_CALI_T_TABLE                 5       /* callback function: On_CaliT_Table */

#define  PAN_CHE_T                        3
#define  PAN_CHE_T_BUT_QUIT               2       /* callback function: On_CheckT_Quit */
#define  PAN_CHE_T_BUT_OK                 3       /* callback function: On_CheckT_OK */
#define  PAN_CHE_T_TABLE                  4       /* callback function: On_Check_Table */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK On_Cali_Limit_OK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Cali_Limit_Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_CaliT_OK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_CaliT_Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_CaliT_Set(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_CaliT_Table(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Check_Table(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_CheckT_OK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_CheckT_Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
