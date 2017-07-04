/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2017. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PAN_ADVA                         1
#define  PAN_ADVA_BUT_QUIT                2       /* callback function: On_Advance_Quit */
#define  PAN_ADVA_BUT_APP                 3       /* callback function: On_Advance_App */
#define  PAN_ADVA_CHE_NPI                 4

#define  PAN_CONF                         2
#define  PAN_CONF_RING_PN                 2       /* callback function: On_Config_PN */
#define  PAN_CONF_BUT_OK                  3       /* callback function: On_Config_OK */
#define  PAN_CONF_RING_EED                4
#define  PAN_CONF_RING_BOM                5
#define  PAN_CONF_RAD_HIGH                6       /* callback function: On_Config_Temper */
#define  PAN_CONF_RAD_ROOM                7       /* callback function: On_Config_Temper */
#define  PAN_CONF_RAD_LOW                 8       /* callback function: On_Config_Temper */
#define  PAN_CONF_DECORATION_2            9
#define  PAN_CONF_DECORATION              10
#define  PAN_CONF_TEXTMSG                 11
#define  PAN_CONF_TEXTMSG_2               12
#define  PAN_CONF_CHE_BOX_R_SDHYS         13
#define  PAN_CONF_CHE_BOX_T_MASK          14
#define  PAN_CONF_CHE_BOX_T_EYE           15
#define  PAN_CONF_CHE_BOX_T_DIS_SOFT      16
#define  PAN_CONF_CHE_BOX_T_DIS           17
#define  PAN_CONF_CHE_BOX_T_ER            18
#define  PAN_CONF_CHE_BOX_T_AOP           19
#define  PAN_CONF_CHE_BOX_T_CROSS         20
#define  PAN_CONF_CHE_BOX_EE_PROTECT      21
#define  PAN_CONF_CHE_BOX_CALI_TEST       22
#define  PAN_CONF_CHE_BOX_CALI            23
#define  PAN_CONF_CHE_BOX_R_OVER          24
#define  PAN_CONF_CHE_BOX_R_SENS          25
#define  PAN_CONF_CHE_BOX_T_SPECTRUM      26
#define  PAN_CONF_CHE_BOX_R_SENS_REAL     27
#define  PAN_CONF_RAD_BUT_TUN_DWDM        28      /* callback function: On_Config_Item */
#define  PAN_CONF_RAD_BUT_TEST_DWDM       29      /* callback function: On_Config_Item */

#define  PAN_CTR                          3
#define  PAN_CTR_BUT_SAVE                 2       /* callback function: On_Ctr_Save */
#define  PAN_CTR_BUT_SAVE_2               3       /* callback function: Quit */
#define  PAN_CTR_TAB                      4

#define  PAN_INIT                         4
#define  PAN_INIT_TEXTMSG                 2

#define  PAN_INST                         5
#define  PAN_INST_TREE                    2       /* callback function: On_Inst_Tree */
#define  PAN_INST_DECORATION              3
#define  PAN_INST_STR_FIBER               4
#define  PAN_INST_STR_SPECTRUM            5
#define  PAN_INST_STR_DCA                 6
#define  PAN_INST_RING_ATT_MAI_TYPE_REF   7
#define  PAN_INST_RING_ATT_MAI_TYPE       8
#define  PAN_INST_RING_CLOCK_TYPE         9
#define  PAN_INST_RING_SPECTRUM_TYPE      10
#define  PAN_INST_RING_SW_TYPE            11      /* callback function: On_Sel_FSW */
#define  PAN_INST_RING_DCA_TYPE           12
#define  PAN_INST_RING_SEVB_TYPE          13
#define  PAN_INST_RING_SEVB_LIGHT_TYPE    14      /* callback function: On_Sel_SEVB_Light */
#define  PAN_INST_RING_10GBERT_TYPE       15
#define  PAN_INST_RING_BERT_TYPE          16
#define  PAN_INST_RING_PM_TYPE            17
#define  PAN_INST_RING_CLOCK              18
#define  PAN_INST_RING_GVPM_REF           19
#define  PAN_INST_RING_EVB_LIGHT          20
#define  PAN_INST_RING_GVPM_MAI           21
#define  PAN_INST_RING_GBERT10G           22
#define  PAN_INST_RING_FSW                23
#define  PAN_INST_STR_SN_SPECTRUM         24
#define  PAN_INST_RING_BERT               25
#define  PAN_INST_BUT_GET_EVB_LIGHT_SN    26      /* callback function: On_Get_EVB_Light_SN */
#define  PAN_INST_RING_SEVB               27
#define  PAN_INST_STR_SN_DCA              28
#define  PAN_INST_BUT_10GBERT             29      /* callback function: On_Get_10GBERT_SN */
#define  PAN_INST_STR_SN_PM               30
#define  PAN_INST_BUT_CLOCK               31      /* callback function: On_Search_Clock */
#define  PAN_INST_BUT_ATT_REF             32      /* callback function: On_Search_ATT_REF */
#define  PAN_INST_BUT_BERT                33      /* callback function: On_Search_BERT */
#define  PAN_INST_BUT_SEVB                34      /* callback function: On_Search_SEVB */
#define  PAN_INST_BUT_ATT_MAI             35      /* callback function: On_Search_ATT */
#define  PAN_INST_BUT_FSW                 36      /* callback function: On_Search_FSW */
#define  PAN_INST_BUT_DCA                 37      /* callback function: On_Search_DCA */
#define  PAN_INST_NUM_CHANNEL             38
#define  PAN_INST_NUM_CLOCK_CHAN          39
#define  PAN_INST_NUM_SW_CHAN             40
#define  PAN_INST_NUM_SW                  41
#define  PAN_INST_NUM_PM                  42
#define  PAN_INST_BUT_ADD                 43      /* callback function: On_Inst_AddTree */
#define  PAN_INST_BUT_CHECK               44      /* callback function: On_Inst_Check */
#define  PAN_INST_NUM_ATT_REF             45
#define  PAN_INST_BUT_OK                  46      /* callback function: On_Inst_OK */
#define  PAN_INST_BIN_CHAN_EN             47
#define  PAN_INST_BUT_SAVE                48      /* callback function: On_Inst_Save */
#define  PAN_INST_NUM_ATT                 49

#define  PAN_MAIN                         6       /* callback function: panelCB */
#define  PAN_MAIN_CANVAS                  2
#define  PAN_MAIN_STATUSBAR               3
#define  PAN_MAIN_TREE                    4
#define  PAN_MAIN_BUT_RUN                 5       /* callback function: On_Run */
#define  PAN_MAIN_BUT_STOP                6       /* callback function: On_Stop */
#define  PAN_MAIN_LED                     7
#define  PAN_MAIN_TEXTMSG_INFO            8
#define  PAN_MAIN_TEXTBOX_CHAN7           9
#define  PAN_MAIN_TEXTBOX_CHAN6           10
#define  PAN_MAIN_TEXTBOX_CHAN5           11
#define  PAN_MAIN_TEXTBOX_CHAN4           12
#define  PAN_MAIN_TEXTBOX_CHAN3           13
#define  PAN_MAIN_TEXTBOX_CHAN2           14
#define  PAN_MAIN_TEXTBOX_CHAN1           15
#define  PAN_MAIN_TEXTBOX_CHAN0           16
#define  PAN_MAIN_STR_PN                  17
#define  PAN_MAIN_STR_OPERATOR            18
#define  PAN_MAIN_STR_BOM                 19
#define  PAN_MAIN_STR_BATCH               20
#define  PAN_MAIN_STR_SN                  21
#define  PAN_MAIN_LED_CHAN1               22
#define  PAN_MAIN_BIN_CHAN7               23      /* callback function: On_P */
#define  PAN_MAIN_BIN_CHAN5               24      /* callback function: On_P */
#define  PAN_MAIN_BIN_CHAN4               25      /* callback function: On_P */
#define  PAN_MAIN_BIN_CHAN1               26      /* callback function: On_P */
#define  PAN_MAIN_BIN_CHAN2               27      /* callback function: On_P */
#define  PAN_MAIN_BIN_CHAN0               28      /* callback function: On_P */
#define  PAN_MAIN_BIN_CHAN6               29      /* callback function: On_P */
#define  PAN_MAIN_BIN_CHAN3               30      /* callback function: On_P */
#define  PAN_MAIN_LED_CHAN4               31
#define  PAN_MAIN_LED_CHAN7               32
#define  PAN_MAIN_LED_CHAN6               33
#define  PAN_MAIN_LED_CHAN5               34
#define  PAN_MAIN_LED_CHAN3               35
#define  PAN_MAIN_LED_CHAN2               36
#define  PAN_MAIN_LED_CHAN0               37
#define  PAN_MAIN_TIMER                   38      /* callback function: On_Timer */
#define  PAN_MAIN_NUMERIC_PASS_RATE       39
#define  PAN_MAIN_NUMERIC_TOTAL           40
#define  PAN_MAIN_NUMERIC_FAIL            41
#define  PAN_MAIN_NUMERIC_PASS            42
#define  PAN_MAIN_DECORATION              43

#define  PAN_ORDER                        7
#define  PAN_ORDER_COMBO_ORDER            2
#define  PAN_ORDER_BUT_ORDER              3       /* callback function: On_Order */
#define  PAN_ORDER_TEXTMSG                4

#define  PAN_POPU                         8
#define  PAN_POPU_BUT_CANCEL              2       /* callback function: On_Pup_Cancel */
#define  PAN_POPU_BUT_USER                3       /* callback function: On_Pup_User */
#define  PAN_POPU_BUT_EXIT                4       /* callback function: On_Pup_Exit */
#define  PAN_POPU_TEXTMSG                 5

#define  PAN_RCAL_F                       9
#define  PAN_RCAL_F_BUT_SET               2       /* callback function: On_CaliT_Set */
#define  PAN_RCAL_F_TABLE                 3
#define  PAN_RCAL_F_BUT_OK                4       /* callback function: On_Calibrate_R_Free_OK */
#define  PAN_RCAL_F_BUT_QUIT              5       /* callback function: On_CalRx_Quit */
#define  PAN_RCAL_F_BUT_OK_TDP            6       /* callback function: On_Calibrate_R_Free_OK */

#define  PAN_RCALL                        10
#define  PAN_RCALL_BUT_SET                2       /* callback function: On_CaliT_Set */
#define  PAN_RCALL_TABLE                  3
#define  PAN_RCALL_BUT_OK                 4       /* callback function: On_Calibrate_R_Lock_OK */
#define  PAN_RCALL_BUT_QUIT               5       /* callback function: On_CalRx_Quit */
#define  PAN_RCALL_BUT_OK_TDP             6       /* callback function: On_Calibrate_R_Lock_OK */

#define  PAN_SEAR0                        11
#define  PAN_SEAR0_CHAN0_DATA             2

#define  PAN_SN                           12
#define  PAN_SN_STRING_MODULESN           2
#define  PAN_SN_CMD_OK                    3
#define  PAN_SN_CHECKBOX                  4

     /* tab page panel controls */
#define  GetBosaSN_RING_Table             2
#define  GetBosaSN_RING_I2cAddr           3
#define  GetBosaSN_DECORATION             4

     /* tab page panel controls */
#define  InitModule_RING_I2cAddr          2
#define  InitModule_RING_PassWord         3
#define  InitModule_DECORATION            4
#define  InitModule_TEXTMSG               5
#define  InitModule_STR_GetVersion        6
#define  InitModule_DECORATION_2          7

     /* tab page panel controls */
#define  RxCal_STR_SAVE                   2
#define  RxCal_STR_ADC_GET                3
#define  RxCal_STR_RSSI_TEMP_POINT        4
#define  RxCal_STR_SET_RSSI_KL            5
#define  RxCal_STR_SET_RSSI_KH            6
#define  RxCal_STR_MODE_T                 7
#define  RxCal_STR_MODE_F                 8
#define  RxCal_STR_MODE_E                 9
#define  RxCal_NUM_OFFSET_AMP             10
#define  RxCal_NUM_RSSI_K_AMP             11
#define  RxCal_NUM_SLOPE_AMP              12
#define  RxCal_NUM_LUK_INDEX              13
#define  RxCal_RING_LUT_NAME              14

     /* tab page panel controls */
#define  TestTemper_STR_GetTempCase       2
#define  TestTemper_STR_GetTempCore       3
#define  TestTemper_NUM_GetTempOffset     4

     /* tab page panel controls */
#define  TunAOP10G_STR_SAVE               2
#define  TunAOP10G_STR_DAC_SET            3
#define  TunAOP10G_STR_MODE_A             4
#define  TunAOP10G_STR_MODE_M             5
#define  TunAOP10G_NUM_OFFSET_AMP         6
#define  TunAOP10G_NUM_SLOPE_AMP          7
#define  TunAOP10G_NUM_LUK_INDEX          8
#define  TunAOP10G_RING_LUT_NAME          9

     /* tab page panel controls */
#define  TunAOP1G_STR_SAVE                2
#define  TunAOP1G_STR_DAC_SET             3
#define  TunAOP1G_STR_MODE_A              4
#define  TunAOP1G_STR_MODE_M              5
#define  TunAOP1G_NUM_OFFSET_AMP          6
#define  TunAOP1G_NUM_SLOPE_AMP           7
#define  TunAOP1G_NUM_LUK_INDEX           8
#define  TunAOP1G_RING_LUT_NAME           9

     /* tab page panel controls */
#define  TunAPD_STR_SAVE                  2
#define  TunAPD_STR_DAC_SET_RANGE         3
#define  TunAPD_STR_DAC_SET               4
#define  TunAPD_STR_MODE_A                5
#define  TunAPD_STR_MODE_M                6
#define  TunAPD_NUM_OFFSET_AMP            7
#define  TunAPD_NUM_SLOPE_AMP             8
#define  TunAPD_NUM_LUK_INDEX             9
#define  TunAPD_RING_LUT_NAME             10

     /* tab page panel controls */
#define  TunCros10G_STR_SAVE              2
#define  TunCros10G_STR_DAC_SET           3
#define  TunCros10G_STR_MODE_A            4
#define  TunCros10G_STR_MODE_M            5
#define  TunCros10G_NUM_OFFSET_AMP        6
#define  TunCros10G_NUM_SLOPE_AMP         7
#define  TunCros10G_NUM_LUK_INDEX         8
#define  TunCros10G_RING_LUT_NAME         9

     /* tab page panel controls */
#define  TunCross1G_STR_SAVE              2
#define  TunCross1G_STR_DAC_SET           3
#define  TunCross1G_STR_MODE_A            4
#define  TunCross1G_STR_MODE_M            5
#define  TunCross1G_NUM_OFFSET_AMP        6
#define  TunCross1G_NUM_SLOPE_AMP         7
#define  TunCross1G_NUM_LUK_INDEX         8
#define  TunCross1G_RING_LUT_NAME         9

     /* tab page panel controls */
#define  TunER10G_STR_SAVE                2
#define  TunER10G_STR_DAC_SET             3
#define  TunER10G_STR_MODE_A              4
#define  TunER10G_STR_MODE_M              5
#define  TunER10G_NUM_OFFSET_AMP          6
#define  TunER10G_NUM_SLOPE_AMP           7
#define  TunER10G_NUM_LUK_INDEX           8
#define  TunER10G_RING_LUT_NAME           9

     /* tab page panel controls */
#define  TunER1G_STR_SAVE                 2
#define  TunER1G_STR_DAC_SET              3
#define  TunER1G_STR_MODE_A               4
#define  TunER1G_STR_MODE_M               5
#define  TunER1G_NUM_OFFSET_AMP           6
#define  TunER1G_NUM_SLOPE_AMP            7
#define  TunER1G_NUM_LUK_INDEX            8
#define  TunER1G_RING_LUT_NAME            9

     /* tab page panel controls */
#define  TunSD_STR_SAVE                   2
#define  TunSD_STR_FUN_ON                 3
#define  TunSD_STR_FUN_OFF                4
#define  TunSD_STR_DAC_SET                5
#define  TunSD_STR_MODE_A                 6
#define  TunSD_STR_MODE_M                 7
#define  TunSD_NUM_OFFSET_AMP             8
#define  TunSD_NUM_SLOPE_AMP              9
#define  TunSD_NUM_LUK_INDEX              10
#define  TunSD_RING_LUT_NAME              11

     /* tab page panel controls */
#define  TxCal10G_STR_Slope_Addr          2
#define  TxCal10G_STR_ThreAddr_Min        3
#define  TxCal10G_STR_ThreAddr_Max        4
#define  TxCal10G_STR_DAC_GET             5
#define  TxCal10G_STR_MODE_F              6
#define  TxCal10G_STR_MODE_E              7
#define  TxCal10G_NUM_OFFSET_AMP          8
#define  TxCal10G_NUM_SLOPE_AMP           9

     /* tab page panel controls */
#define  TxCal1G_STR_Slope_Addr           2
#define  TxCal1G_STR_ThreAddr_Min         3
#define  TxCal1G_STR_ThreAddr_Max         4
#define  TxCal1G_STR_DAC_GET              5
#define  TxCal1G_STR_MODE_F               6
#define  TxCal1G_STR_MODE_E               7
#define  TxCal1G_NUM_OFFSET_AMP           8
#define  TxCal1G_NUM_SLOPE_AMP            9


     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_FILE                     2
#define  MENUBAR_FILE_SGD                 3       /* callback function: On_SGD */
#define  MENUBAR_FILE_USER                4       /* callback function: On_User */
#define  MENUBAR_FILE_EXIT                5       /* callback function: On_Quit */
#define  MENUBAR_SET                      6
#define  MENUBAR_SET_INIT                 7       /* callback function: On_Init */
#define  MENUBAR_SET_CONFIG               8       /* callback function: On_Config */
#define  MENUBAR_SET_INSTRUMENT           9       /* callback function: On_Inst */
#define  MENUBAR_SET_FUN_ADVANCE          10      /* callback function: On_Advance */
#define  MENUBAR_SET_FUN_CONTROL          11      /* callback function: On_Control */
#define  MENUBAR_CALI                     12
#define  MENUBAR_CALI_RX_CALI_LOCK        13      /* callback function: On_Calibrate_R_Lock */
#define  MENUBAR_CALI_RX_CALI_FREE        14      /* callback function: On_Calibrate_R_Free */
#define  MENUBAR_CALI_RX_CALI_LOCK_LINE   15      /* callback function: On_Calibrate_R_Lock_Line */
#define  MENUBAR_CALI_RX_CALI_FREE_LINE   16      /* callback function: On_Calibrate_R_Free_Line */
#define  MENUBAR_CALI_TX_CALI             17      /* callback function: On_Calibrate_T */
#define  MENUBAR_CALI_TX_CHECK            18      /* callback function: On_CheckT */
#define  MENUBAR_SEARCH_DATA              19
#define  MENUBAR_SEARCH_DATA_SEARCH_DATA_CHAN0 20 /* callback function: Callback_SearchData_Chan0 */
#define  MENUBAR_SEARCH_DATA_SEARCH_DATA_CHAN1 21 /* callback function: Callback_SearchData_Chan1 */
#define  MENUBAR_HELP                     22
#define  MENUBAR_HELP_TEST1_0612          23      /* callback function: Callback_Test1_170612 */
#define  MENUBAR_HELP_HIS                 24      /* callback function: On_His */
#define  MENUBAR_HELP_ABOUT               25      /* callback function: On_About */


     /* Callback Prototypes: */ 

void CVICALLBACK Callback_SearchData_Chan0(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK Callback_SearchData_Chan1(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK Callback_Test1_170612(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK On_About(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK On_Advance(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK On_Advance_App(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Advance_Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK On_Calibrate_R_Free(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK On_Calibrate_R_Free_Line(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK On_Calibrate_R_Free_OK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK On_Calibrate_R_Lock(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK On_Calibrate_R_Lock_Line(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK On_Calibrate_R_Lock_OK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK On_Calibrate_T(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK On_CaliT_Set(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_CalRx_Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK On_CheckT(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK On_Config(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK On_Config_Item(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Config_OK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Config_PN(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Config_Temper(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK On_Control(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK On_Ctr_Save(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Get_10GBERT_SN(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Get_EVB_Light_SN(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK On_His(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK On_Init(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK On_Inst(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK On_Inst_AddTree(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Inst_Check(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Inst_OK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Inst_Save(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Inst_Tree(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Order(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_P(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Pup_Cancel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Pup_Exit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Pup_User(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK On_Quit(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK On_Run(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Search_ATT(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Search_ATT_REF(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Search_BERT(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Search_Clock(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Search_DCA(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Search_FSW(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Search_SEVB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Sel_FSW(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Sel_SEVB_Light(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK On_SGD(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK On_Stop(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Timer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK On_User(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
