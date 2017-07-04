/*****************************************************************************/
/*  ag8614x.h                                                               */
/*  Copyright (C) 2000 Agilent Technologies Inc.                             */
/*---------------------------------------------------------------------------*/
/*  Driver for ag8614x, Optical Spectrum Analyzer                           */
/*  Driver Version: A.02.00                                                  */
/*****************************************************************************/

/*****************************************************************************/
/* $Workfile: ag8614x.h $ */
/* Copyright (C) 2001 Agilent Technologies */
/*****************************************************************************/
/* Microsoft Visual SourceSafe Header */
/* $Logfile: /Connectivity/Source/ag8614x_Visa/ag8614x/ag8614x.h $ */
/* $Date: 5/11/01 5:27p $ */
/* $Revision: 3 $ */
/* $Author: Mikefitz $ */
/*****************************************************************************/


#ifndef ag8614x_INCLUDE
#define ag8614x_INCLUDE
#include "vpptype.h"

/* Used for "C" externs in C++ */
#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/*****************************************************************************/
/*  STANDARD SECTION                                                         */
/*  Constants and function prototypes for HP standard functions.             */
/*****************************************************************************/

        /***************************************************/
        /*  Standard constant error conditions returned    */
        /*  by driver functions.                           */
        /*    HP Common Error numbers start at BFFC0D00    */
        /*    The parameter errors extend the number of    */
        /*      errors over the eight defined in VPP 3.4   */
        /***************************************************/

#define ag8614x_INSTR_ERROR_NULL_PTR      (_VI_ERROR+0x3FFC0D02L) /* 0xBFFC0D02 */
#define ag8614x_INSTR_ERROR_RESET_FAILED  (_VI_ERROR+0x3FFC0D03L) /* 0xBFFC0D03 */
#define ag8614x_INSTR_ERROR_UNEXPECTED    (_VI_ERROR+0x3FFC0D04L) /* 0xBFFC0D04 */
#define ag8614x_INSTR_ERROR_INV_SESSION   (_VI_ERROR+0x3FFC0D05L) /* 0xBFFC0D05 */
#define ag8614x_INSTR_ERROR_LOOKUP        (_VI_ERROR+0x3FFC0D06L) /* 0xBFFC0D06 */
#define ag8614x_INSTR_ERROR_DETECTED      (_VI_ERROR+0x3FFC0D07L) /* 0xBFFC0D07 */
#define ag8614x_INSTR_NO_LAST_COMMA       (_VI_ERROR+0x3FFC0D08L) /* 0xBFFC0D08 */
#define ag8614x_VI_ERROR_SPAN			   (_VI_ERROR+0x3FFC0D09L) /* 0xBFFC0D09 */

#define ag8614x_INSTR_ERROR_PARAMETER9    (_VI_ERROR+0x3FFC0D20L) /* 0xBFFC0D20 */
#define ag8614x_INSTR_ERROR_PARAMETER10   (_VI_ERROR+0x3FFC0D21L) /* 0xBFFC0D21 */
#define ag8614x_INSTR_ERROR_PARAMETER11   (_VI_ERROR+0x3FFC0D22L) /* 0xBFFC0D22 */
#define ag8614x_INSTR_ERROR_PARAMETER12   (_VI_ERROR+0x3FFC0D23L) /* 0xBFFC0D23 */
#define ag8614x_INSTR_ERROR_PARAMETER13   (_VI_ERROR+0x3FFC0D24L) /* 0xBFFC0D24 */
#define ag8614x_INSTR_ERROR_PARAMETER14   (_VI_ERROR+0x3FFC0D25L) /* 0xBFFC0D25 */
#define ag8614x_INSTR_ERROR_PARAMETER15   (_VI_ERROR+0x3FFC0D26L) /* 0xBFFC0D26 */
#define ag8614x_INSTR_ERROR_PARAMETER16   (_VI_ERROR+0x3FFC0D27L) /* 0xBFFC0D27 */
#define ag8614x_INSTR_ERROR_PARAMETER17   (_VI_ERROR+0x3FFC0D28L) /* 0xBFFC0D28 */
#define ag8614x_INSTR_ERROR_PARAMETER18   (_VI_ERROR+0x3FFC0D29L) /* 0xBFFC0D29 */

#define ag8614x_FILE_OPEN_ERROR			(_VI_ERROR+0x3FFC0D30L) /* 0xBFFC0D30 */
#define ag8614x_FILE_READ_ERROR    		(_VI_ERROR+0x3FFC0D31L) /* 0xBFFC0D31 */
#define ag8614x_FILE_WRITE_ERROR    	(_VI_ERROR+0x3FFC0D32L) /* 0xBFFC0D32 */
#define ag8614x_CAL_POW_ERROR  	  		(_VI_ERROR+0x3FFC0D33L) /* 0xBFFC0D33 */
#define ag8614x_CAL_WAV_ERROR	    	(_VI_ERROR+0x3FFC0D34L) /* 0xBFFC0D34 */
#define ag8614x_OUT_OF_MEMORY			(_VI_ERROR+0x3FFC0D35L) /* 0xBFFC0D35 */
#define ag8614x_CMD_NOT_SUPPORTED		(_VI_ERROR+0x3FFC0D36L)

        /***************************************************/
        /*  Constants used by system status functions      */
        /*    These defines are bit numbers which define   */
        /*    the operation and questionable registers.    */
        /*    They are instrument specific.                */
        /***************************************************/

#define ag8614x_QUES_BIT0          1
#define ag8614x_QUES_BIT1          2
#define ag8614x_QUES_BIT2          4
#define ag8614x_QUES_BIT3          8
#define ag8614x_QUES_BIT4         16
#define ag8614x_QUES_BIT5         32
#define ag8614x_QUES_BIT6         64
#define ag8614x_QUES_BIT7        128
#define ag8614x_QUES_BIT8        256
#define ag8614x_QUES_BIT9        512
#define ag8614x_QUES_BIT10      1024
#define ag8614x_QUES_BIT11      2048
#define ag8614x_QUES_BIT12      4096
#define ag8614x_QUES_BIT13      8192
#define ag8614x_QUES_BIT14     16384
#define ag8614x_QUES_BIT15     32768

#define ag8614x_OPER_BIT0          1
#define ag8614x_OPER_BIT1          2
#define ag8614x_OPER_BIT2          4
#define ag8614x_OPER_BIT3          8
#define ag8614x_OPER_BIT4         16
#define ag8614x_OPER_BIT5         32
#define ag8614x_OPER_BIT6         64
#define ag8614x_OPER_BIT7        128
#define ag8614x_OPER_BIT8        256
#define ag8614x_OPER_BIT9        512
#define ag8614x_OPER_BIT10      1024
#define ag8614x_OPER_BIT11      2048
#define ag8614x_OPER_BIT12      4096
#define ag8614x_OPER_BIT13      8192
#define ag8614x_OPER_BIT14     16384
#define ag8614x_OPER_BIT15     32768

        /***************************************************/
        /*  Constants used by function ag8614x_timeOut    */
        /***************************************************/

#define ag8614x_TIMEOUT_MAX         2147483647L
#define ag8614x_TIMEOUT_MIN         0L

#define ag8614x_CMDINT16ARR_Q_MIN   1L
#define ag8614x_CMDINT16ARR_Q_MAX   2147483647L

#define ag8614x_CMDINT32ARR_Q_MIN   1L
#define ag8614x_CMDINT32ARR_Q_MAX   2147483647L

#define ag8614x_CMDREAL64ARR_Q_MIN  1L
#define ag8614x_CMDREAL64ARR_Q_MAX  2147483647L


        /***************************************************/
        /*  Required plug and play functions from VPP-3.1  */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_init
                        (ViRsrc     resourceName,
                         ViBoolean  IDQuery,
                         ViBoolean  resetDevice,
                         ViPSession instrumentHandle ) ;

ViStatus _VI_FUNC ag8614x_close
                        (ViSession  instrumentHandle);

ViStatus _VI_FUNC ag8614x_reset
                        (ViSession  instrumentHandle);

ViStatus _VI_FUNC ag8614x_self_test
                        (ViSession  instrumentHandle,
                         ViPInt16   selfTestResult,
                         ViChar _VI_FAR  selfTestMessage[]);

ViStatus _VI_FUNC ag8614x_error_query
                        (ViSession  instrumentHandle,
                         ViPInt32   errorCode,
                         ViChar _VI_FAR  errorMessage[]);

ViStatus _VI_FUNC ag8614x_error_message
                        (ViSession  instrumentHandle,
                         ViStatus   statusCode,
                         ViChar _VI_FAR  message[]);

ViStatus _VI_FUNC ag8614x_revision_query
                        (ViSession  instrumentHandle,
                         ViChar _VI_FAR  instrumentDriverRevision[],
                         ViChar _VI_FAR  firmwareRevision[]);


        /***************************************************/
        /*  HP standard utility functions                  */
        /***************************************************/

#define ag8614x_ERR_Q_DISABLE  0
#define ag8614x_ERR_Q_ENABLE   1

ViStatus _VI_FUNC ag8614x_timeOut
                        (ViSession  instrumentHandle,
                         ViInt32    setTimeOut);

ViStatus _VI_FUNC ag8614x_timeOut_Q
                        (ViSession  instrumentHandle,
                         ViPInt32   timeOut);

ViStatus _VI_FUNC ag8614x_errorQueryDetect
                        (ViSession  instrumentHandle,
                         ViBoolean  setErrorQueryDetect);

ViStatus _VI_FUNC ag8614x_errorQueryDetect_Q
                        (ViSession  instrumentHandle,
                         ViPBoolean errorQueryDetect);

ViStatus _VI_FUNC ag8614x_dcl
                        (ViSession  instrumentHandle);

ViStatus _VI_FUNC ag8614x_opc_Q
                        (ViSession  instrumentHandle,
                         ViPBoolean instrumentReady);


        /***************************************************/
        /*  HP standard status functions                   */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_readStatusByte_Q
                        (ViSession  instrumentHandle,
                         ViPInt16   statusByte);

ViStatus _VI_FUNC ag8614x_operEvent_Q
                        (ViSession  instrumentHandle,
                         ViPInt32   operationEventRegister);

ViStatus _VI_FUNC ag8614x_operCond_Q
                        (ViSession  instrumentHandle,
                         ViPInt32   operationConditionRegister );

ViStatus _VI_FUNC ag8614x_quesEvent_Q
                        (ViSession  instrumentHandle,
                         ViPInt32   questionableEventRegister );

ViStatus _VI_FUNC ag8614x_quesCond_Q
                        (ViSession  instrumentHandle,
                         ViPInt32   questionableConditionRegister );


        /***************************************************/
        /*  HP standard command passthrough functions      */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_cmd
                        (ViSession  instrumentHandle,
                         ViString   sendStringCommand);

ViStatus _VI_FUNC ag8614x_cmdString_Q
                        (ViSession  instrumentHandle,
                         ViString   queryStringCommand,
	                     ViInt32    stringSize,
                         ViChar _VI_FAR  stringResult[]);

ViStatus _VI_FUNC ag8614x_cmdInt
                        (ViSession  instrumentHandle,
                         ViString   sendIntegerCommand,
                         ViInt32    sendInteger);

ViStatus _VI_FUNC ag8614x_cmdInt16_Q
                        (ViSession  instrumentHandle,
                         ViString   queryI16Command,
                         ViPInt16   i16Result);

ViStatus _VI_FUNC ag8614x_cmdInt32_Q
                        (ViSession  instrumentHandle,
                         ViString   queryI32Command,
                         ViPInt32   i32Result);

ViStatus _VI_FUNC ag8614x_cmdInt16Arr_Q
                        (ViSession  instrumentHandle,
                         ViString   queryI16ArrayCommand,
                         ViInt32    i16ArraySize,
                         ViInt16 _VI_FAR i16ArrayResult[],
                         ViPInt32   i16ArrayCount);

ViStatus _VI_FUNC ag8614x_cmdInt32Arr_Q
                        (ViSession  instrumentHandle,
                         ViString   queryI32ArrayCommand,
                         ViInt32    i32ArraySize,
                         ViInt32 _VI_FAR i32ArrayResult[],
                         ViPInt32   i32ArrayCount);

ViStatus _VI_FUNC ag8614x_cmdReal
                        (ViSession  instrumentHandle,
                         ViString   sendRealCommand,
                         ViReal64   sendReal);

ViStatus _VI_FUNC ag8614x_cmdReal64_Q
                        (ViSession  instrumentHandle,
                         ViString   queryRealCommand,
                         ViPReal64  realResult);

ViStatus _VI_FUNC ag8614x_cmdReal64Arr_Q
                        (ViSession  instrumentHandle,
                         ViString   realArrayCommand,
                         ViInt32    realArraySize,
                         ViReal64 _VI_FAR realArrayResult[],
                         ViPInt32   realArrayCount);


/* End of HP standard declarations */
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
/*  INSTRUMENT SPECIFIC SECTION                                              */
/*  Constants and function prototypes for instrument specific functions.     */
/*****************************************************************************/

        /***************************************************/
        /*  Instrument specific constants                  */
        /***************************************************/

#define ag8614x_MASK_MIN				0
#define ag8614x_MASK_MAX				255
#define ag8614x_WAV_NM_MIN				600.0
#define ag8614x_WAV_NM_MAX				1700.0
#define ag8614x_WAV_SPAN_NM_MIN 		0.0
#define ag8614x_WAV_SPAN_NM_MAX 		1100.0
#define ag8614x_BWID_RES_NM_MIN_6 		.06
#define ag8614x_BWID_RES_NM_MIN_7 		.07
#define ag8614x_BWID_RES_NM_MAX 		10.0
#define ag8614x_BWID_VID_HZ_MIN 		.1
#define ag8614x_BWID_VID_HZ_MAX 		3000.0
#define ag8614x_POW_DBM_MIN 			-300.0
#define ag8614x_POW_DBM_MAX	 		300.0
#define ag8614x_RLEV_DBM_MIN 			-300.0
#define ag8614x_RLEV_DBM_MAX 			300.0
#define ag8614x_X_TIME_S_MIN			0.0
#define ag8614x_X_TIME_S_MAX 			2048.0
#define ag8614x_SWE_TIME_S_MIN			2E-4
#define ag8614x_SWE_TIME_S_MAX 		2048.0
#define ag8614x_MARK_PEXC_DB_MIN 		0.0
#define ag8614x_MARK_PEXC_DB_MAX 		60.
#define ag8614x_BWID_NDB_DB_MIN 		-200.0
#define ag8614x_BWID_NDB_DB_MAX 		200.0
#define ag8614x_NOIS_BWID_01NM 		0
#define ag8614x_NOIS_BWID_1NM  		1
#define ag8614x_YSCAL_RPOS_MIN 		0.0
#define ag8614x_YSCAL_RPOS_MAX 		10.0
#define ag8614x_SCAL_PDIV_DB_MIN 		.01
#define ag8614x_SCAL_PDIV_DB_MAX 		20.0
#define ag8614x_TRAC_POIN_MIN 			3
#define ag8614x_TRAC_POIN_MAX 			10001
#define ag8614x_AVER_COUNT_MIN 		1
#define ag8614x_AVER_COUNT_MAX 		1000
#define ag8614x_TRIG_DELAY_S_MIN 		2E-6
#define ag8614x_TRIG_DELAY_S_MAX 		6.5536E-3
#define ag8614x_RCL 					0
#define ag8614x_SAV 					1
#define ag8614x_MANUAL 				0
#define ag8614x_AUTO 					1

#define ag8614x_ENABLE 				1
#define ag8614x_DISABLE 					0

#define ag8614x_SINGLE              0
#define ag8614x_REPEAT                      1
#define ag8614x_LINEAR              0
#define ag8614x_LOG                      1
#define ag8614x_REGISTER_MIN 			0
#define ag8614x_REGISTER_MAX 			9
#define ag8614x_REF_LEVEL 				0
#define ag8614x_CENTER_WAV 			1
#define ag8614x_CAL_HIGH 				0
#define ag8614x_CAL_MARKER 			1
#define ag8614x_CAL_BOTH 				0
#define ag8614x_CAL_POWER 				1
#define ag8614x_CAL_WAVELENGTH 		2
#define ag8614x_MARKER_AUTO 			0
#define ag8614x_MARKER_1 				1
#define ag8614x_MARKER_2 				2
#define ag8614x_MARKER_3 				3
#define ag8614x_MARKER_4 				4
#define ag8614x_SRAN_OFF				0
#define ag8614x_SRAN_ON				1
#define ag8614x_SOURCE_IMM 			0
#define ag8614x_SOURCE_EXT 			1
#define ag8614x_SOURCE_INT 			2
#define ag8614x_SOURCE_POS 			0
#define ag8614x_SOURCE_NEG 			1
#define ag8614x_SOURCE_EITHER 			2
#define ag8614x_SWEEP_SINGLE 			0
#define ag8614x_SWEEP_REPEAT 			1

#define ag8614x_STATE_OFF              0
#define ag8614x_STATE_ON               1
#define ag8614x_FORMAT_CGM             0
#define ag8614x_FORMAT_PCL             1
#define ag8614x_RANGE_FULL             0
#define ag8614x_RANGE_TELE             1
#define ag8614x_CORR_MIN			   1
#define ag8614x_CORR_MAX			   4

#define ag8614x_MARKER_OFF				0
#define ag8614x_MARKER_ON 				1
#define ag8614x_PEAK_HIGH 				0
#define ag8614x_PEAK_MARKER 			1
#define ag8614x_YAXIS_LEFT 			    0
#define ag8614x_YAXIS_RIGHT 			1
#define ag8614x_YSCALE_LOG 			0
#define ag8614x_YSCALE_LIN 			1
#define ag8614x_TRA 					1
#define ag8614x_TRB 					2
#define ag8614x_TRC 					3
#define ag8614x_TRD 					4
#define ag8614x_TRE 					5
#define ag8614x_TRF 					6
#define ag8614x_TRACE_ON 				1
#define ag8614x_TRACE_OFF 				0
#define ag8614x_TRACE_FEED_ALWAYS 		1
#define ag8614x_TRACE_FEED_NEVER 		0
#define ag8614x_HOLD_MAX 				1
#define ag8614x_HOLD_MIN 				0
#define ag8614x_HOLD_CLEAR 			2
#define ag8614x_HOLD_ON 				1
#define ag8614x_HOLD_OFF 				0
#define ag8614x_CALC_MARK_MAX 			1
#define ag8614x_CALC_MARK_MAX_LEFT 	2
#define ag8614x_CALC_MARK_MAX_RIGH 	3
#define ag8614x_CALC_MARK_MAX_NEXT 	4
#define ag8614x_CALC_MARK_MIN 			5
#define ag8614x_CALC_MARK_MIN_LEFT 	6
#define ag8614x_CALC_MARK_MIN_RIGH 	7
#define ag8614x_CALC_MARK_MIN_NEXT 	8
#define ag8614x_BWIDTH_1NM 			1
#define ag8614x_BWIDTH_0_1NM 			0
#define ag8614x_UNIT_POW_DBM 			0
#define ag8614x_UNIT_POW_W 			1
#define ag8614x_UNIT_POW_AUTO 			2
#define ag8614x_TRIG_OUTPUT_OFF		0
#define ag8614x_TRIG_OUTPUT_ON			1
#define ag8614x_TRIG_OUTPUT_AUTO 		2
#define ag8614x_PRINTER_INTERNAL 		0
#define ag8614x_PRINTER_EXTERNAL 		1
#define ag8614x_CANCEL 				-1
#define ag8614x_MAX_SIZE				(100*1024)

#define ag8614x_NOISE_PIT               0
#define ag8614x_NOISE_AUTO              1
#define ag8614x_NOISE_MANUAL			2

#define ag8614x_MANUAL_OFFSET_MIN		0.
#define ag8614x_MANUAL_OFFSET_MAX		15.00
#define ag8614x_CHAN_POIN_MAX			4
#define ag8614x_STAT_POIN_MAX			11
#define ag8614x_PAIR_MIN                1
#define	ag8614x_PAIR_MAX                10001
#define ag8614x_CORRECTION_MIN			1
#define	ag8614x_CORRECTION_MAX			4
/***********  ag8614x_WDM_wdmSetup *******************/
#define ag8614x_WDM_WAV_NM_MIN  600.0
#define ag8614x_WDM_WAV_NM_MAX  1700.0
#define ag8614x_WDM_MARK_PEXC_DB_MIN  0.01
#define ag8614x_WDM_MARK_PEXC_DB_MAX  60.0
#define ag8614x_WDM_NOISE_PIT  0
#define ag8614x_WDM_NOISE_OFFSET  1
#define ag8614x_WDM_NOISE_BETW_CHAN 2
#define ag8614x_WDM_NOISE_FIXED  1
#define ag8614x_WDM_SPACING_MIN  50.0
#define ag8614x_WDM_SPACING_MAX  500.0
#define ag8614x_WDM_NOISE_HD  2
#define ag8614x_WDM_POW_DBM_MIN  -300.0
#define ag8614x_WDM_POW_DBM_MAX  300.0
#define ag8614x_WDM_NOISE_RES_MIN_A  0.07
#define ag8614x_WDM_NOISE_RES_MAX_A  10.0
#define ag8614x_WDM_PEAKS_RES_MIN_A  0.07
#define ag8614x_WDM_PEAKS_RES_MAX_A  10.0
#define ag8614x_WDM_NOISE_RES_MIN_B  0.06
#define ag8614x_WDM_NOISE_RES_MAX_B  10.0
#define ag8614x_WDM_PEAKS_RES_MIN_B  0.06
#define ag8614x_WDM_PEAKS_RES_MAX_B  10.0
#define ag8614x_WDM_THRESHOLD_MIN -300.0
#define ag8614x_WDM_THRESHOLD_MAX  300.0

/********** ag8614x_WDM_referenceLevel **************/
#define ag8614x_WDM_RLEV_DBM_MIN  -300.0
#define ag8614x_WDM_RLEV_DBM_MAX  300.0
#define ag8614x_AMP_ENABLE 				1
#define ag8614x_AMP_DISABLE 			0

/********** ag8614x_AMP_manualNoiseOffset ******************/
#define ag8614x_NOISE_NM_MIN   0.0
#define ag8614x_NOISE_NM_MAX  15.0

/********** ag8614x_AMP_peakExcursionThreshold *************/
#define ag8614x_AMP_MARK_PEXC_DB_MIN  0.0
#define ag8614x_AMP_MARK_PEXC_DB_MAX  60.0
#define ag8614x_AMP_THRESHOLD_MIN -300.0
#define ag8614x_AMP_THRESHOLD_MAX  300.0

/**************** ag8614x_AMP_traceOffsets *****************/
#define ag8614x_AMP_STRACE_OFFS_DB_MIN -10.0
#define ag8614x_AMP_STRACE_OFFS_DB_MAX 100.0
#define ag8614x_AMP_ATRACE_OFFS_DB_MIN -10.0
#define ag8614x_AMP_ATRACE_OFFS_DB_MAX 100.0


/****************** ag8614x_SRC_bandwidthDFB ***************/
#define ag8614x_BAND_DFB_DBM_MIN        -200.0
#define ag8614x_BAND_DFB_DBM_MAX         200.0

/****************** ag8614x_SRC ****************************/
#define ag8614x_POW_LOW_DBM_MIN         -300.0
#define ag8614x_POW_LOW_DBM_MAX			 300.0

        /***************************************************/
        /*  Instrument specific functions                  */
        /***************************************************/

        /***************************************************/
        /*  Wavelength Class functions                     */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_wavelengthStartStop
						(ViSession instrumentHandle,
						ViReal64 startWav,
						ViReal64 stopWav);

ViStatus _VI_FUNC ag8614x_wavelengthStart
						(ViSession instrumentHandle,
                        ViReal64 startWav);

ViStatus _VI_FUNC ag8614x_wavelengthStop
                        (ViSession instrumentHandle,
                         ViReal64 stopWav);

ViStatus _VI_FUNC ag8614x_wavelengthStartStop_Q
						(ViSession instrumentHandle,
                         ViPReal64 startWav,
                         ViPReal64 stopWav);

ViStatus _VI_FUNC ag8614x_wavelengthStart_Q
						(ViSession instrumentHandle,
                         ViPReal64 startWav);

ViStatus _VI_FUNC ag8614x_wavelengthStop_Q
						(ViSession instrumentHandle,
                         ViPReal64 stopWav);

ViStatus _VI_FUNC ag8614x_wavelengthCenterSpan
						(ViSession instrumentHandle,
						ViReal64 center,
						ViReal64 span);

ViStatus _VI_FUNC ag8614x_wavelengthCenter
						(ViSession instrumentHandle,
                         ViReal64 center);

ViStatus _VI_FUNC ag8614x_wavelengthSpan
						(ViSession instrumentHandle,
                         ViReal64 span);

ViStatus _VI_FUNC ag8614x_wavelengthCenterSpan_Q
						(ViSession instrumentHandle,
                         ViPReal64 center,
                         ViPReal64 span);

ViStatus _VI_FUNC ag8614x_wavelengthCenter_Q
						(ViSession instrumentHandle,
                         ViPReal64 center);

ViStatus _VI_FUNC ag8614x_wavelengthSpan_Q
						(ViSession instrumentHandle,
                        ViPReal64 span);

        /***************************************************/
        /*  Bandwidth/Sweep Class functions                */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_sweepTime
						(ViSession instrumentHandle,
						ViReal64 time);

ViStatus _VI_FUNC ag8614x_sweepTime_Q
						(ViSession instrumentHandle,
						 ViPReal64 time);

ViStatus _VI_FUNC ag8614x_startSweep
						(ViSession instrumentHandle,
						ViInt16 sweep);

ViStatus _VI_FUNC ag8614x_resolutionBandwidth
						(ViSession instrumentHandle,
						ViInt16 autoSet,
                        ViReal64 resolution);

ViStatus _VI_FUNC ag8614x_resolutionBandwidth_Q
						(ViSession instrumentHandle,
                         ViPInt16 autoSet,
                         ViPReal64 resolution);

ViStatus _VI_FUNC ag8614x_videoBandwidth
						(ViSession instrumentHandle,
						ViInt16 autoSet,
						ViReal64 bandwidth);

ViStatus _VI_FUNC ag8614x_videoBandwidth_Q
						(ViSession instrumentHandle,
                         ViPInt16 autoSet,
                         ViPReal64 bandwidth);

        /***************************************************/
        /*  Trigger Class functions                        */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_externalSynchronization
						(ViSession instrumentHandle,
						ViInt16 source,
						ViInt16 slope,
						ViReal64 delay,
						ViInt16 output);

        /***************************************************/
        /*  Amplitude Class functions                      */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_referenceLevel
						(ViSession instrumentHandle,
						ViReal64 level,
						ViInt16 axis);

ViStatus _VI_FUNC ag8614x_referenceLevel_Q
						(ViSession instrumentHandle,
						ViPReal64 level,
						ViInt16 axis);

ViStatus _VI_FUNC ag8614x_amplitudeCorrection
						(ViSession instrumentHandle,
                        ViInt16 ampCoEnable,
                        ViInt16 correctionSet,
                        ViInt16 interpolationMethod);

ViStatus _VI_FUNC ag8614x_amplitudeCorrectionState_Q
						(ViSession instrumentHandle,
                        ViPInt16 ampCoEnable,
                        ViPInt16 correctionSet,
                        ViPInt16 interpolationMethod);

ViStatus _VI_FUNC ag8614x_sensitivity
						(ViSession instrumentHandle,
						ViInt16 autoSet,
						ViReal64 sensitivity);

ViStatus _VI_FUNC ag8614x_sensitivity_Q
						(ViSession instrumentHandle,
                        ViPInt16 autoSet,
                        ViPReal64 sensitivity);

ViStatus _VI_FUNC ag8614x_amplitudeCorrectionSet
						(ViSession instrumentHandle,
                        ViInt16 correctionSet,
                        ViReal64 _VI_FAR correctionFactorPairs[],
                        ViInt32 pairs);

ViStatus _VI_FUNC ag8614x_amplitudeCorrectionSet_Q
						(ViSession instrumentHandle,
                        ViInt16 correctionSet,
                        ViReal64 _VI_FAR correctionFactorPairs[],
                        ViPInt32 pairs);


        /***************************************************/
        /*  Integrate Class functions                      */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_integrateTrace
						(ViSession instrumentHandle,
						ViInt16 trace,
						ViPReal64 data);

ViStatus _VI_FUNC ag8614x_integrateTraceRange
						(ViSession instrumentHandle,
						ViInt16 trace,
						ViReal64 lower,
						ViReal64 upper,
						ViPReal64 data);

        /***************************************************/
        /*  Marker Class functions                         */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_markerToWavelength
						(ViSession instrumentHandle,
						ViInt16 marker,
						ViInt16 trace,
						ViReal64 level);

ViStatus _VI_FUNC ag8614x_markerToWavelength_Q
						(ViSession instrumentHandle,
						 ViInt16 marker,
						 ViPInt16 trace);

ViStatus _VI_FUNC ag8614x_markerToReferenceLevel
						(ViSession instrumentHandle,
						ViInt16 marker);

ViStatus _VI_FUNC ag8614x_markerToCenterWavelength
						(ViSession instrumentHandle,
						ViInt16 marker);

ViStatus _VI_FUNC ag8614x_markerSearchRange
						(ViSession instrumentHandle,
						ViReal64 lower,
						ViReal64 upper,
						ViInt16 state);

ViStatus _VI_FUNC ag8614x_markerPosition
						(ViSession instrumentHandle,
						ViInt16 marker,
						ViInt16 markSetting);

ViStatus _VI_FUNC ag8614x_markerRead
						(ViSession instrumentHandle,
						ViInt16 marker,
						ViPReal64 markX,
						ViPReal64 markY);

ViStatus _VI_FUNC ag8614x_markerExcursionCriteria
						(ViSession instrumentHandle,
						ViReal64 peak,
						ViReal64 pit);

ViStatus _VI_FUNC ag8614x_deltaMarkerState
						(ViSession instrumentHandle,
						ViInt16 marker,
						ViInt16 status);

ViStatus _VI_FUNC ag8614x_deltaMarkerState_Q
						(ViSession instrumentHandle,
						ViInt16 marker,
						ViPInt16 status);

ViStatus _VI_FUNC ag8614xb_deltaMarkerOffset
						(ViSession vi,
						 ViInt16 marker,
						 ViPReal64 xOffset,
						 ViPReal64 yOffset,
						 ViPReal64 xRef,
						 ViPReal64 yRef);

ViStatus _VI_FUNC ag8614x_deltaMarkerOffset
						(ViSession instrumentHandle,
						ViInt16 marker,
						ViPReal64 xOffset,
						ViPReal64 yOffset,
						ViPReal64 xRef,
						ViPReal64 yRef);

ViStatus _VI_FUNC ag8614x_bandwidthMarker
						(ViSession instrumentHandle,
						ViInt16 marker,
						ViReal64 offset);

ViStatus _VI_FUNC ag8614x_bandwidthMarker_Q
						(ViSession instrumentHandle,
						ViInt16 marker,
						ViPReal64 result,
						ViPReal64 centerWavelength,
						ViPReal64 ndb);

ViStatus _VI_FUNC ag8614x_noiseMarker
						(ViSession instrumentHandle,
						ViInt16 marker,
						ViInt16 bwidth);

ViStatus _VI_FUNC ag8614x_noiseMarker_Q
						(ViSession instrumentHandle,
						ViInt16 marker,
						ViPReal64 result,
						ViPReal64 bwidth);

ViStatus _VI_FUNC ag8614x_OSNRMarker
						(ViSession instrumentHandle,
                        ViInt16 marker,
                        ViInt16 noise,
                        ViReal64 manualOffset);

ViStatus _VI_FUNC ag8614x_OSNRMarker_Q
						(ViSession instrumentHandle,
                        ViInt16 marker,
                        ViPReal64 osnr,
                        ViPReal64 centerWavelength,
                        ViPReal64 centerPower);

ViStatus _VI_FUNC ag8614x_noiseReferenceBandwidth
						(ViSession instrumentHandle,
                        ViInt16 noiseRefBW);

ViStatus _VI_FUNC ag8614x_noiseReferenceBandwidth_Q
						(ViSession instrumentHandle,
                        ViPInt16 noiseResBW);

        /***************************************************/
        /*  Trace Class functions                          */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_verticalScale
						(ViSession instrumentHandle,
						ViReal64 referenceLevel,
						ViReal64 position,
						ViInt16 axis,
						ViInt16 units,
						ViReal64 db,
						ViInt16 yscale);

ViStatus _VI_FUNC ag8614x_verticalScale_Q
						(ViSession instrumentHandle,
						 ViPReal64 referenceLevel,
						 ViPReal64 position,
						 ViInt16 axis,
						 ViPInt16 units,
						 ViPReal64 db,
						 ViPInt16 yscale);

ViStatus _VI_FUNC ag8614x_numberTracePoints
						(ViSession instrumentHandle,
						ViInt16 points);

ViStatus _VI_FUNC ag8614x_numberTracePoints_Q
						(ViSession instrumentHandle,
						ViPInt16 points);

ViStatus _VI_FUNC ag8614x_traceUpdate
						(ViSession instrumentHandle,
						ViInt16 trace,
						ViInt16 feed);

ViStatus _VI_FUNC ag8614x_traceVisible
						(ViSession instrumentHandle,
						ViInt16 trace,
						ViInt16 status);

ViStatus _VI_FUNC ag8614x_maxTraceHold
						(ViSession instrumentHandle,
						ViInt16 trace,
						ViInt16 holdStatus);

ViStatus _VI_FUNC ag8614x_minTraceHold
						(ViSession instrumentHandle,
						ViInt16 trace,
						ViInt16 holdStatus);

ViStatus _VI_FUNC ag8614x_averageTrace
						(ViSession instrumentHandle,
						ViInt16 trace,
						ViInt16 count);

ViStatus _VI_FUNC ag8614x_averageTrace_Q
						(ViSession instrumentHandle,
						 ViInt16 trace,
						 ViPInt16 count,
                         ViPInt16 state);

ViStatus _VI_FUNC ag8614x_traceMath
						(ViSession instrumentHandle,
						ViInt16 trace,
						ViString expression);

ViStatus _VI_FUNC ag8614x_traceMeanValue_Q
						(ViSession instrumentHandle,
						ViInt16 trace,
						ViPReal64 data);

ViStatus _VI_FUNC ag8614x_wavelengthRangeMeanValue_Q
						(ViSession instrumentHandle,
						ViInt16 trace,
						ViReal64 lower,
						ViReal64 upper,
						ViPReal64 data);

        /***************************************************/
        /*  System Class functions                         */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_loadState
						(ViSession instrumentHandle,
						ViInt16 regNumber);
ViStatus _VI_FUNC ag8614x_saveState
						(ViSession instrumentHandle,
						ViInt16 regNumber);

        /***************************************************/
        /*  System Trace Class functions                   */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_downloadTraces
						(ViSession instrumentHandle,
						ViInt16 trace,
						ViReal64 _VI_FAR data[],
						ViPInt32 points);
ViStatus _VI_FUNC ag8614x_uploadTrace
						(ViSession instrumentHandle,
						ViInt16 trace,
						ViInt16 points,
						ViReal64 _VI_FAR values[]);
ViStatus _VI_FUNC ag8614x_saveTraceToFile
						(ViSession instrumentHandle,
						ViInt16 trace,
						ViString filename);
ViStatus _VI_FUNC ag8614x_loadTraceFromFile
						(ViSession instrumentHandle,
						ViInt16 trace,
						ViString filename);

        /***************************************************/
        /*  System Calibration Class functions             */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_autoAlign
						(ViSession instrumentHandle,
						ViInt16 marker);
ViStatus _VI_FUNC ag8614x_autoMeasure
						(ViSession instrumentHandle,
						ViInt16 peak);
ViStatus _VI_FUNC ag8614x_calibrationAtWavelength
						(ViSession instrumentHandle,
						ViReal64 power,
						ViReal64 wavelength);
ViStatus _VI_FUNC ag8614x_calibrationAtMarker
						(ViSession instrumentHandle,
						ViReal64 wavelength,
						ViInt16 marker);
ViStatus _VI_FUNC ag8614x_extMultipointCalWavelength
						(ViSession instrumentHandle,
                        ViReal64 wavelength);
ViStatus _VI_FUNC ag8614x_extMultipointCalAtMarker
						(ViSession instrumentHandle,
                        ViReal64 wavelength,ViInt16 marker);
ViStatus _VI_FUNC ag8614x_internalNormalCalibration
						(ViSession instrumentHandle);
ViStatus _VI_FUNC ag8614x_internalMultipointCalibration
						(ViSession instrumentHandle);

ViStatus _VI_FUNC ag8614x_calibrationMultipointData
						(ViSession instrumentHandle,
                        ViString data);

ViStatus _VI_FUNC ag8614x_calibrationMultipointData_Q
						(ViSession instrumentHandle,
                        ViChar _VI_FAR  data[]);
//*******************************************************
ViStatus _VI_FUNC ag8614x_calibrationMultDataArray
						(ViSession instrumentHandle,
						ViInt32 pairs,
                        ViReal64 _VI_FAR data[]);

ViStatus _VI_FUNC ag8614x_calibrationMultDataArray_Q
                        (ViSession instrumentHandle,
                         ViPInt32 pairs,
                         ViReal64 _VI_FAR  data[]);
//*******************************************************
ViStatus _VI_FUNC ag8614x_deleteMultipointData
						(ViSession instrumentHandle);
ViStatus _VI_FUNC ag8614x_calibrationRange
						(ViSession instrumentHandle,
                        ViInt16 range);
ViStatus _VI_FUNC ag8614x_calibrationRange_Q
						(ViSession instrumentHandle,
                        ViInt16 *range);

        /***************************************************/
        /*  System Print/Display Class functions           */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_printResults
						(ViSession instrumentHandle,
						ViInt16 printer);
ViStatus _VI_FUNC ag8614x_displayText
						(ViSession instrumentHandle,
						ViString text);
ViStatus _VI_FUNC ag8614x_downloadScreenFile
						(ViSession instrumentHandle,
						ViString filename);
ViStatus _VI_FUNC ag8614x_printFormat
						(ViSession instrumentHandle,
                        ViInt16 printer);
ViStatus _VI_FUNC ag8614x_printFormat_Q
						(ViSession instrumentHandle,
                        ViChar _VI_FAR format[]);

        /***************************************************/
        /*  System Register Class functions                */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_clearEventsReg
						(ViSession instrumentHandle);
ViStatus _VI_FUNC ag8614x_eventEnableReg
						(ViSession instrumentHandle,
						ViInt16 mask);
ViStatus _VI_FUNC ag8614x_eventEnableReg_Q
						(ViSession instrumentHandle,
						ViPInt16 mask);
ViStatus _VI_FUNC ag8614x_eventStatusReg_Q
						(ViSession instrumentHandle,
						ViPInt16 mask);
ViStatus _VI_FUNC ag8614x_serviceEnableReg
						(ViSession instrumentHandle,
						ViInt16 mask);
ViStatus _VI_FUNC ag8614x_serviceEnableReg_Q
						(ViSession instrumentHandle,
						ViPInt16 mask);

        /***************************************************/
        /*  System Input/Output Class functions            */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_sourceCatalog_Q
						(ViSession instrumentHandle,
                        ViChar _VI_FAR sourceList[]);
ViStatus _VI_FUNC ag8614x_sourceSelect
						(ViSession instrumentHandle,
                        ViString source,
                        ViInt16 state);
ViStatus _VI_FUNC ag8614x_sourceSelect_Q
						(ViSession instrumentHandle,
                        ViString source,
                        ViInt16 *state);
ViStatus _VI_FUNC ag8614x_pulseMode
						(ViSession instrumentHandle,
						ViInt16 state);
ViStatus _VI_FUNC ag8614x_pulseMode_Q
						(ViSession instrumentHandle,
                        ViInt16 *state);
ViStatus _VI_FUNC ag8614x_powerMeterDisplay
						(ViSession instrumentHandle,
                        ViInt16 state);
ViStatus _VI_FUNC ag8614x_powerMeterReading_Q
						(ViSession instrumentHandle,
                        ViInt16 units,
                        ViReal64 *power);
ViStatus _VI_FUNC ag8614x_synchronousOutput
						(ViSession instrumentHandle,
                        ViInt16 state);

        /***************************************************/
        /*                Application                      */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_applicationCatalog_Q
						(ViSession instrumentHandle,
                        ViChar _VI_FAR applicationList[]);
ViStatus _VI_FUNC ag8614x_applicationCatalogFull_Q
						(ViSession instrumentHandle,
                        ViChar _VI_FAR applicationList[]);
ViStatus _VI_FUNC ag8614x_applicationSelect
						(ViSession instrumentHandle,
                        ViString application);
ViStatus _VI_FUNC ag8614x_applicationSelectNumeric
						(ViSession instrumentHandle,
                        ViString application);
ViStatus _VI_FUNC ag8614x_WDM_load
						(ViSession instrumentHandle);

		/***************************************************/
        /*                WDM Subsystem                    */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_WDM_exit
						(ViSession instrumentHandle);

ViStatus _VI_FUNC ag8614x_WDM_wdmSetup
						(ViSession instrumentHandle,
                        ViReal64 startWav, ViReal64 stopWav,
                        ViReal64 peak, ViReal64 threshold,
                        ViInt16 noise, ViReal64 spacing,
                        ViReal64 sensitivity,
                        ViReal64 noiseResolution,
                        ViReal64 peaksResolution);
ViStatus _VI_FUNC ag8614x_WDM_wdmSetup_Q
						(ViSession instrumentHandle,
                        ViReal64 *startWav, ViReal64 *stopWav,
                        ViReal64 *peak, ViReal64 *threshold,
                        ViInt16 *noise, ViReal64 *spacing,
                        ViReal64 *sensitivity,
                        ViReal64 *noiseResolution,
                        ViReal64 *peaksResolution);
ViStatus _VI_FUNC ag8614x_WDM_channelSpacing
						(ViSession instrumentHandle,
                        ViReal64 spacing);
ViStatus _VI_FUNC ag8614x_WDM_channelSpacing_Q
						(ViSession instrumentHandle,
                        ViReal64 *spacing);
ViStatus _VI_FUNC ag8614x_WDM_noiseMethod
						(ViSession instrumentHandle,
                        ViInt16 noise);
ViStatus _VI_FUNC ag8614x_WDM_noiseMethod_Q
						(ViSession instrumentHandle,
                        ViInt16 *noise);
ViStatus _VI_FUNC ag8614x_WDM_resolutionBandwidth
						(ViSession instrumentHandle,
                        ViReal64 noiseResolution,
                        ViReal64 peaksResolution);
ViStatus _VI_FUNC ag8614x_WDM_resolutionBandwidth_Q
						(ViSession instrumentHandle,
                        ViReal64 *noiseResolution,
                        ViReal64 *peaksResolution);
ViStatus _VI_FUNC ag8614x_WDM_peakExcursionThreshold
						(ViSession instrumentHandle,
                        ViReal64 peak,
                        ViReal64 threshold);
ViStatus _VI_FUNC ag8614x_WDM_peakExcursionThreshold_Q
						(ViSession instrumentHandle,
                        ViReal64 *peak,
                        ViReal64 *threshold);
ViStatus _VI_FUNC ag8614x_WDM_wavelengthStartStop
						(ViSession instrumentHandle,
                        ViReal64 startWav,
                        ViReal64 stopWav);
ViStatus _VI_FUNC ag8614x_WDM_wavelengthStartStop_Q
						(ViSession instrumentHandle,
                        ViReal64 *startWav,
                        ViReal64 *stopWav);
ViStatus _VI_FUNC ag8614x_WDM_startSweep
						(ViSession instrumentHandle);
ViStatus _VI_FUNC ag8614x_WDM_referenceLevel
						(ViSession instrumentHandle,
                        ViReal64 level);
ViStatus _VI_FUNC ag8614x_WDM_referenceLevel_Q
						(ViSession instrumentHandle,
                        ViReal64 *level);
ViStatus _VI_FUNC ag8614x_WDM_sensitivity
						(ViSession instrumentHandle,
                        ViReal64 sensitivity);
ViStatus _VI_FUNC ag8614x_WDM_sensitivity_Q
						(ViSession instrumentHandle,
                        ViReal64 *sensitivity);
ViStatus _VI_FUNC ag8614x_WDM_numberLines_Q
						(ViSession instrumentHandle,
                        ViInt16 *channels);
ViStatus _VI_FUNC ag8614x_WDM_powerList_Q
						(ViSession instrumentHandle,
                        ViReal64 _VI_FAR powers []);
ViStatus _VI_FUNC ag8614x_WDM_signalToNoise_Q
						(ViSession instrumentHandle,
                        ViReal64 _VI_FAR values []);
ViStatus _VI_FUNC ag8614x_WDM_wavelengthList_Q
						(ViSession instrumentHandle,
                        ViReal64 _VI_FAR wavelengths []);
ViStatus _VI_FUNC ag8614x_WDM_frequencyList_Q
						(ViSession instrumentHandle,
                        ViReal64 _VI_FAR frequencies []);
ViStatus _VI_FUNC ag8614x_WDM_channelsStatistics_Q
						(ViSession instrumentHandle,
                        ViReal64 _VI_FAR statistics [],
              			ViReal64 *Span_Tilt_dB_nm,
              			ViReal64 *Span_Tilt_dB,
              			ViReal64 *Peak_to_Peak);


		/***************************************************/
        /*                AMP Subsystem                    */
        /***************************************************/

ViStatus _VI_FUNC ag8614x_AMP_loadAmplifierApplication
						(ViSession instrumentHandle);

ViStatus _VI_FUNC ag8614x_AMP_noiseMethod
						(ViSession instrumentHandle,
                        ViInt16 noiseMethod);

ViStatus _VI_FUNC ag8614x_AMP_noiseMethod_Q
						(ViSession instrumentHandle,
                        ViInt16 *noiseMethod);

ViStatus _VI_FUNC ag8614x_AMP_manualNoiseOffset
						(ViSession instrumentHandle,
                        ViReal64 noiseOffset);

ViStatus _VI_FUNC ag8614x_AMP_manualNoiseOffset_Q
						(ViSession instrumentHandle,
                        ViReal64 *noiseOffset);

ViStatus _VI_FUNC ag8614x_AMP_peakExcursionThreshold
						(ViSession instrumentHandle,
                        ViReal64 excursion,
                        ViReal64 threshold);

ViStatus _VI_FUNC ag8614x_AMP_peakExcursionThreshold_Q
						(ViSession instrumentHandle,
                        ViReal64 *excursion,
                        ViReal64 *threshold);

ViStatus _VI_FUNC ag8614x_AMP_ShotNoise
						(ViSession instrumentHandle,
                        ViInt16 shotNoise);

ViStatus _VI_FUNC ag8614x_AMP_ShotNoise_Q
						(ViSession instrumentHandle,
                        ViInt16 *shotNoise);

ViStatus _VI_FUNC ag8614x_AMP_traceOffsets
						(ViSession instrumentHandle,
                        ViReal64 sourceTraceOffset,
                        ViReal64 amplifierTraceOffset);

ViStatus _VI_FUNC ag8614x_AMP_traceOffsets_Q
						(ViSession instrumentHandle,
                        ViReal64 *sourceTraceOffset,
                        ViReal64 *amplifierTraceOffset);

ViStatus _VI_FUNC ag8614x_AMP_wavelengthStartStop
						(ViSession instrumentHandle,
                        ViReal64 startWavelength,
                        ViReal64 stopWavelength);

ViStatus _VI_FUNC ag8614x_AMP_wavelengthStartStop_Q
						(ViSession instrumentHandle,
                        ViReal64 *startWavelength,
                        ViReal64 *stopWavelength);

ViStatus _VI_FUNC ag8614x_AMP_resolutionBandwidth
						(ViSession instrumentHandle,
                        ViReal64 resolutionBandwidth);

ViStatus _VI_FUNC ag8614x_AMP_resolutionBandwidth_Q
						(ViSession instrumentHandle,
                        ViReal64 *resolutionBandwidth);

ViStatus _VI_FUNC ag8614x_AMP_sourceSweepExecute
						(ViSession instrumentHandle);

ViStatus _VI_FUNC ag8614x_AMP_amplifierSweepExecute
						(ViSession instrumentHandle);

ViStatus _VI_FUNC ag8614x_AMP_numberLines_Q
						(ViSession instrumentHandle,
                        ViInt16 *channels);

ViStatus _VI_FUNC ag8614x_AMP_powerList_Q
						(ViSession instrumentHandle,
                        ViInt16 *channelCount,
                        ViReal64 _VI_FAR channelPowers[]);

ViStatus _VI_FUNC ag8614x_AMP_wavelengthList_Q
						(ViSession instrumentHandle,
                        ViInt16 *channelCount,
                        ViReal64 _VI_FAR channelWavelength[]);

ViStatus _VI_FUNC ag8614x_AMP_gainList_Q
						(ViSession instrumentHandle,
                        ViInt16 *channelCount,
                        ViReal64 _VI_FAR channelGainValues[]);

ViStatus _VI_FUNC ag8614x_AMP_noiseFigureList_Q
						(ViSession instrumentHandle,
                        ViInt16 *channelCount,
                        ViReal64 _VI_FAR chNoiseFigureVals[]);

ViStatus _VI_FUNC ag8614x_AMP_traceStatistics_Q
						(ViSession instrumentHandle,
                        ViReal64 *sourceMeanWavs,
                        ViReal64 *totalSrcSignalPow,
                        ViReal64 *ampChWavsMean,
                        ViReal64 *totalAmpSignalPower);

ViStatus _VI_FUNC ag8614x_AMP_exitAmplifierApplication
						(ViSession instrumentHandle);

		/***************************************************/
        /*                SRC Subsystem                    */
        /***************************************************/


ViStatus _VI_FUNC ag8614x_SRC_sweepExecute
						(ViSession instrumentHandle,
                        ViInt16 sweep);

ViStatus _VI_FUNC ag8614x_SRC_loadTestDFB
						(ViSession instrumentHandle);

ViStatus _VI_FUNC ag8614x_SRC_configureTestDFB
						(ViSession instrumentHandle,
                        ViReal64 startWavelength,
                        ViReal64 stopWavelength,
                        ViReal64 resolutionBandwidth,
                        ViInt16 tracePoints,
                        ViReal64 sensitivity,
                        ViReal64 bandwidthDFB);

ViStatus _VI_FUNC ag8614x_SRC_bandwidthDFB
						(ViSession instrumentHandle,
                        ViReal64 bandwidth);

ViStatus _VI_FUNC ag8614x_SRC_bandwidthDFB_Q
						(ViSession instrumentHandle,
                        ViReal64 *bandwidth);

ViStatus _VI_FUNC ag8614x_SRC_resultRecordDFB_Q
						(ViSession instrumentHandle,
                        ViReal64 *peakWavelength,
                        ViReal64 *peakAmplitude,
                        ViReal64 *modeOffset,
                        ViReal64 *stopBand,
                        ViReal64 *SMSR,
                        ViReal64 *centerOffset,
                        ViReal64 *bandwidth,
                        ViReal64 *bandwidthAmp);

ViStatus _VI_FUNC ag8614x_SRC_loadTestFP
						(ViSession instrumentHandle);

ViStatus _VI_FUNC ag8614x_SRC_configureTestFP
						(ViSession instrumentHandle,
                        ViReal64 startWavelength,
                        ViReal64 stopWavelength,
                        ViReal64 resolutionBandwidth,
                        ViInt16 tracePoints,
                        ViReal64 sensitivity);

ViStatus _VI_FUNC ag8614x_SRC_resultRecordFP_Q
						(ViSession instrumentHandle,
                        ViReal64 *meanWavelength,
                        ViReal64 *peakWavelength,
                        ViReal64 *peakAmplitude,
                        ViReal64 *modeSpacing,
                        ViReal64 *FWHM, ViReal64 *sigma,
                        ViReal64 *totalPower);

ViStatus _VI_FUNC ag8614x_SRC_loadTestLED
						(ViSession instrumentHandle);

ViStatus _VI_FUNC ag8614x_SRC_configureTestLED
						(ViSession instrumentHandle,
                        ViReal64 startWavelength,
                        ViReal64 stopWavelength,
                        ViReal64 resolutionBandwidth,
                        ViInt16 tracePoints,
                        ViReal64 sensitivity);

ViStatus _VI_FUNC ag8614x_SRC_resultRecordLED_Q
						(ViSession instrumentHandle,
                        ViReal64 *meanWavelength,
                        ViReal64 *meanWav3dBdown,
                        ViReal64 *peakWavelength,
                        ViReal64 *peakSpecDensity,
                        ViReal64 *FWHM, ViReal64 *sigma,
                        ViReal64 *totalPower,
                        ViReal64 *width3dBdown);

ViStatus _VI_FUNC ag8614x_SRC_exitSourceApplication
						(ViSession instrumentHandle);

/* End of instrument specific declarations */
/*---------------------------------------------------------------------------*/

/* Used for "C" externs in C++ */
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif /* ag8614x_INCLUDE */

/* ================================================================= */
/* End of File: $Workfile: ag8614x.h $ */


