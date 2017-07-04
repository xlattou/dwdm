/****************************************************************************
 *
 * File:                B2BERT.h
 *
 * Author:              SUPERXON INC.(Roger Li)
 *
 * Description:         B2BERT.c 头文件
 *
 * Time:                2010-03-29
 *
 * version:				V2.0.0.1
 * 
 * Update Description:  new document
V2.0.0.1   
1. 删除函数B2BERT_GET_BURST_SDChk_Status_noClear ()
2. 删除函数B2BERT_SET_BURST_ClearSD ()
 
V2.0.0.0
B版本B2设备适用
****************************************************************************/

#ifndef _B2BERT_H_
#define _B2BERT_H_

#include "ch341a_dll.h"

//B2BERT寄存器定义 
#pragma pack (1) /* set maximum alignment to 1 按一个字节对齐*/  

struct sB2BERT_GLOBAL_Rst
{
INT8U bit0:1;				//为1复位除寄存器外的全部电路，为0正常工作，同时要置位Norm和Burst部分的复位寄存器
INT8U reserved:7;			//reserved
};

struct sB2BERT_GLOBAL_REG06
{
INT8U Sel_Rx_ONU:1;			//Norm Burst接收选择，为0选择第0个ONU的接收，为1选择第1个ONU的接收
INT8U isGPONorEPON:1;		//为1表示测试EPON,上下行速率皆为1.25Ghz；为0表示测试GPON，上行为1.244Ghz，下行为2.488Ghz
INT8U reserved:6;			//reserved
};

struct sB2BERT_GLOBAL_REG07
{
INT8U Period_Trig_Sel:1;	//发送方式选择，为0表示周期发送，为1表示单次发送，由本寄存器的bit1触发
INT8U Period_Trig:1;		//写此bit为0--1--0就触发一次Burst发送，只有当本寄存器的bit0为1时才有效
INT8U reserved:6;			//reserved
};

struct sB2BERT_GLOBAL_REG08 //1、实际发送周期要加1。2、时间以发送频率的8分频为1个周期。3、发送周期必须大于两个Burst Bert的各个参数寄存器之和
{
INT8U Burst_Period7_0;		//20位Burst Bert的发送周期寄存器的低8位
INT8U Burst_Period15_8;		//20位Burst Bert的发送周期寄存器的中8位
INT8U Burst_Period19_16:4;	//20位Burst Bert的发送周期寄存器的高4位
INT8U reserved:4;			//reserved 
};

struct sB2BERT_GLOBAL_OLT_Rst  
{
INT8U OLT_Rst_Level:1;		//OLT的复位信号有效电平选择，为0表示低电平复位OLT，为1表示高电平复位OLT 
INT8U OLT_Rst_Out:1;		//为1表示关闭OLT_Rst的输出，为0表示打开OLT_Rst的输出。注意CPU写的OLT的复位不受此位的控制 
INT8U reserved:6;			//reserved 
};

struct sB2BERT_GLOBAL_REG0C
{
INT8U bit0:1;				//两个Burst Bert部分公用的输入延时控制的复位信号，从0到1的跳变触发复位
INT8U reserved:6;			//reserved 
};				

struct sB2BERT_GLOBAL_REG0D
{
INT8U reserved0:1;
INT8U Burst_Ctrl_OK:1;		//为1表示Burst Bert部分中接口延时控制部件复位完毕，为0正在复位
INT8U Burst_Ctrl_Err:1;		//为1表示Burst Bert部分中延时控制部件工作错误，为0正常
INT8U reserved7_3:5;		//reserved 
};

struct sB2BERT_GLOBAL_REG10
{
INT8U Rst_Delay_Control:5;  //控制复位OLT的Rst信号的延时值，一般设为0x05，要根据实际情况调整设置
INT8U reserved7_5:3;		//reserved 
};

struct sB2BERT_GLOBAL_REG11
{
INT8U En_Delay_Control:5;  	//控制Burst En、Trig、Gate和Period等输出的延时值，一般设为0x0E
INT8U reserved7_5:3;		//reserved 
};

struct sB2BERT_GLOBAL_REG12
{
INT8U BTx_LOS:1;  			//为1表示ADN2812有LOS告警，为0表示正常工作
INT8U BTx_LOL:1;  			//为1表示ADN2812有LOL告警，为0表示正常工作
INT8U reserved7_2:6;		//reserved 
};

struct sB2BERT_GLOBAL_REG31
{
INT8U reserved3_0:4;		//reserved 
INT8U TxDis:1;				//Tx_DisableEn，对应Bit4
INT8U reserved7_5:3;		//reserved 
};

struct sB2BERT_GLOBAL_REG40
{
INT8U Trig_Sel:1;			//采样信号有效电平选择，为0表示低电平有效，为1表示高电平有效
INT8U Trig_Off:1;			//为1表示关闭采样信号的输出，为1表示打开的输出。为0表示关闭
INT8U reserved:6;			//reserved 
};

struct sB2BERT_GLOBAL_REG41 //1、实际位置要加1。2、时间以发送频率的8分频为1单位
{
INT8U Trig_Pos7_0;			//20位采样位置信号寄存器的8位
INT8U Trig_Pos15_8;			//20位采样位置信号寄存器的8位
INT8U Trig_Pos19_16:4;		//20位采样位置信号寄存器的高4位
INT8U reserved:4;			//reserved 
};

struct sB2BERT_GLOBAL_REG44  //1、实际位置要加1。2、时间以发送频率的8分频为1单位
{
INT8U Trig_Width7_0;		//20位采样宽度信号寄存器的8位
INT8U Trig_Width15_8;		//20位采样宽度信号寄存器的8位
INT8U Trig_Width19_16:4;	//20位采样宽度信号寄存器的高4位
INT8U reserved:4;			//reserved 
};

struct sB2BERT_GLOBAL_REG63  
{
INT8U Burst_RST_Dly:1;		//用于AD2855和FPGA之间接口延时调整，当写0--1--0时，将延时值设为初始值
INT8U Burst_INC:1;			//用于AD2855和FPGA之间接口延时调整，一般设为0
INT8U reserved7_2:6;		//reserved 
};

struct strB2BERT_GLOBAL		//定义全局控制寄存器
{
INT8U	FPGA_Ver; 									//[0x00h]
INT8U   FPGA_TYPE1;									//[0x01h]
INT8U   FPGA_TYPE2;									//[0x02h]
INT8U   FPGA_TYPE3;									//[0x03h]
INT8U   CPU_Haddr;									//[0x04h]
struct  sB2BERT_GLOBAL_Rst 	 		GLOBALRst;		//[0x05h]
struct	sB2BERT_GLOBAL_REG06 		REG06;			//[0x06h]
struct	sB2BERT_GLOBAL_REG07 		Trigger;		//[0x07h] 
struct  sB2BERT_GLOBAL_REG08 		Burst_Period;  	//[0x08h~0x0Ah], 3byte
struct	sB2BERT_GLOBAL_OLT_Rst 		OLTRst;			//[0x0Bh]
struct	sB2BERT_GLOBAL_REG0C 		BurstCtrlRst;	//[0x0Ch]
struct	sB2BERT_GLOBAL_REG0D 		BurstCtrlStatus;//[0x0Dh]
INT8U	Reserved14to15[2];							//[0x0Eh~0x0Fh], 2byte
struct	sB2BERT_GLOBAL_REG10		OLTRst_Dly;		//[0x10h] 控制复位OLT的Rst信号的延时值，一般设为0x05，要根据实际情况调整设置
struct	sB2BERT_GLOBAL_REG11		GLOBAL_Dly;		//[0x11h] 控制Burst En、Trig、Gate和Period等输出的延时值，一般设为0x0E 
struct	sB2BERT_GLOBAL_REG12		ADN2817Status;  //[0x12h] ADN2812状态
INT8U	Reserved19to48[30];							//[0x13h~0x30h], 30byte 
struct 	sB2BERT_GLOBAL_REG31	    OLT_TxDis;		//[0x31h]
INT8U	Reserved50to63[14];							//[0x32h~0x3Fh], 14byte 
struct	sB2BERT_GLOBAL_REG40 		Trig_Ctrl;		//[0x40h]
struct	sB2BERT_GLOBAL_REG41 		Trig_Pos;		//[0x41h~0x43h], 3byte
struct	sB2BERT_GLOBAL_REG44 		Trig_Width;		//[0x44h~0x46h], 3byte
INT8U	Reserved71to95[25];							//[0x47h~0x5Fh], 25byte  
INT8U   REG60;										//[0x60h] 两个Burst部分当发送为Tx Train(寄存器0x30为2时)为TX_TRAIN_BIT时发送的Byte，主要用于硬件调试，一般不用
INT8U	Reserved61;									//[0x61h]
INT8U   REG62; 										//[0x62h] AD2855和FPGA的接口数据直接读回值，主要用于硬件调试，一般不用
struct  sB2BERT_GLOBAL_REG63		AD2855_Dly;		//[0x63h]
INT8U	Reserved100to127[28];						//[0x64h~0x7Fh], 28byte  
};

union uB2BERT_GLOBAL										  
{ 
struct strB2BERT_GLOBAL  sStrB2BERT_GLOBAL;
INT8U  pStrB2BERT_GLOBAL[128]; 
};

struct sB2BERT_NORM_Rst
{
INT8U bit0:1;			//为1复位除寄存器外的全部Norm Bert部分电路，为0正常工作，当全局寄存器的Reset为1时置位为1
INT8U reserved:7;		//reserved
};

struct sB2BERT_NORM_REG06 //dynamic
{
INT8U Rx_LOS:1;			//为1表示LOS告警，为0表示正常，动态告警
INT8U RX_Lock_Err:1;	//为1表示接收PLL告警，为0表示正常，动态告警
INT8U Tx_FIFO_Err:1;	//为1表示发送FIFO告警，为0表示正常，动态告警
INT8U Tx_Lock_Err:1;  	//为1表示发送PLL告警，为0表示正常，动态告警
INT8U reserved:4;		//reserved
};

struct sB2BERT_NORM_REG07 //static
{
INT8U Rx_LOS:1;			//为1表示LOS告警，为0表示正常，静态态告警        
INT8U RX_Lock_Err:1;	//为1表示接收PLL告警，为0表示正常，静态告警    
INT8U Tx_FIFO_Err:1;	//为1表示发送FIFO告警，为0表示正常，静态告警   
INT8U Tx_Lock_Err:1;	//为1表示发送PLL告警，为0表示正常，静态告警    
INT8U reserved:4;		//reserved
};

struct sB2BERT_NORM_Loop 
{
INT8U Rx2Tx_Loop:1;		//为1控制Norm Bert在FPGA内部从接收输入环回到发送输出，为0正常工作       
INT8U Tx2Rx_Loop:1;		//为1控制Norm Bert在FPGA内部从发送输出环回到接收输入，为0正常工作  
INT8U VSC_Rx2Tx_Loop:1;	//为1控制Norm Bert在VSC8145内部从接收输入环回到发送输出，为0正常工作 
INT8U VSC_Tx2Rx_Loop:1;	//为1控制Norm Bert在VSC8145内部从发送输出环回到接收输入，为0正常工作   
INT8U reserved:4;		//reserved
};

struct sB2BERT_NORM_Align
{
INT8U Align_ReStr:1;	//从0到1的跳变启动SFI41的接口调整电路
INT8U reserved:7;		//reserved
};

struct sB2BERT_NORM_AlignStatus
{
INT8U Align_OK:1;	//为1表示SFI41接口中延时控制状态机调整完毕，为0正在调整
INT8U Ctrl_OK:1;	//为1表示SFI41接口中延时控制部件复位完毕，为0正在复位
INT8U Ctrl_Err:1;	//为1表示SFI41接口中延时控制部件工作错误，为0正常
INT8U reserved:5;	//reserved
};

struct sB2BERT_NORM_Pattern
{
INT8U isUserorPRBS:1;	//为1表示测试序列为用户自定义序列，为0表示是m序列
INT8U reserved:7;		//reserved
};

struct sB2BERT_NORM_UserLen  //当选择用户定义序列时才有效，最长512bit，最短4个bit。实际个数要加1
{
INT8U UserLen7_0;		//用户定义序列长度的低8位
INT8U UserLen8:1;		//用户定义序列长度的最高位
INT8U reserved:7;		//reserved  
};

struct sB2BERT_NORM_UserStatus  
{	 
INT8U User_Start:1;		//当测试序列为用户自定义序列时，为1启动测试，为0复位测试。当测试序列为m序列时设为0
INT8U User_Rdy:1;		//当测试序列为用户自定义序列时，当写User_Start为1启动测试时，为1表示发送还没准备好，为0表示准备好
INT8U reserved:6;		//reserved  
};

struct sB2BERT_NORM_REG17
{
INT8U InsertErrBit:1;	//写0 -- 1 -- 0在发送码流中插入一个bit误码，此动作不受寄存器0x19的Bert_InsErr_En的影响
INT8U reserved:7;		//reserved
};

struct sB2BERT_NORM_REG18
{
INT8U Out_Inv:1;		//为1控制发送输出反相，为0不反相
INT8U In_Inv:1;			//为1控制接收输入反相，为0不反相
INT8U reserved:6;		//reserved  
};

struct sB2BERT_NORM_REG19
{
INT8U InsErrEnbit0:1;	//为1表示允许在发送输出测试序列中插入误码，为0不插入误码
INT8U reserved:7;		//reserved
};

struct sB2BERT_NORM_BertInsErrTime	//以发送频率的16分频为单位(插入1个bit误码)，实际时间要加1
{
INT8U Time7_0;			//插入误码时的时间间隔的最低8位
INT8U Time15_8;	
INT8U Time23_16;
INT8U Time31_24;  		//插入误码时的时间间隔的最高8位
};

struct sB2BERT_NORM_Sync_Err_Max	//每次判断检查了14784个bit，当误码个数小于设置的误码门限时就认为同步。当为PRBS码时，一般设置为0x5C6，而当为自定义数据时，一般设为0x004
{
INT8U Err_Max7_0;		//收同步时判断同步的误码个数低8位
INT8U Err_Max13_8:6;	//收同步时判断同步的误码个数高6位
INT8U reserved:2;		//reserved  
};

struct sB2BERT_NORM_Sync_Status
{
INT8U Status:1;			//为1表示接收处于同步状态，为0表示仍在搜索状态
INT8U reserved:7;		//reserved
};

struct sB2BERT_NORM_Sync_ReSync
{
INT8U ReSync:1;			//从0到1的跳变启动接收重新搜索同步
INT8U reserved:7;		//reserved
};

struct sB2BERT_NORM_ErrTime_Cnt //当写ErrTime7_0寄存器任意值，就锁定以前测试的误码累加个数和测试时间，同时启动下次测试 （两次读写间隔不能超出27s）
{
INT8U ErrTime7_0;		//误码测试的时间最低8位，以发送频率的16分频为单位，实际数字要加1，当超过32位表达时间时，读出值为全1。
INT8U ErrTime15_8;		
INT8U ErrTime23_16;		
INT8U ErrTime31_24;		//误码测试的时间的最高8位
};

struct sB2BERT_NORM_ErrNum_Cnt 
{
INT8U ErrNum7_0;		//误码测试的误码累加个数的最低8位，当超过40位表达个数时，读出值为全1。
INT8U ErrNum15_8;		
INT8U ErrNum23_16;		
INT8U ErrNum31_24;		//误码测试的误码累加个数的最高8位
};

struct strB2BERT_NORM	//定义NormBERT寄存器
{
INT8U	FirstByte; 									//[0x00h] 
INT8U	Reserved1to3[3];							//[0x01h~0x03h], 3byte 
INT8U   CPU_Haddr;									//[0x04h] 
struct  sB2BERT_NORM_Rst 			NormRst;		//[0x05h]
struct	sB2BERT_NORM_REG06 			Status_Dynamic;	//[0x06h]
struct	sB2BERT_NORM_REG07 			Status_Static;	//[0x07h]
struct 	sB2BERT_NORM_Loop   		Loop;			//[0x08h]
struct 	sB2BERT_NORM_Align  		Align;			//[0x09h]
struct 	sB2BERT_NORM_AlignStatus 	AlignStatus;	//[0x0Ah] 
INT8U	Reserved11to15[5];							//[0x0Bh~0x0Fh], 5byte 
struct	sB2BERT_NORM_Pattern 		Pattern;		//[0x10h]
INT8U 	PRBS;										//[0x11h]
struct 	sB2BERT_NORM_UserLen		UserLen;		//[0x12h~0x13h], 2byte 
struct 	sB2BERT_NORM_UserStatus 	UserStatus;		//[0x14h]
INT8U	Reserved21to22[2];							//[0x15h~0x16h], 2byte  
struct	sB2BERT_NORM_REG17			REG17;			//[0x17h]
struct	sB2BERT_NORM_REG18			INV;			//[0x18h] 
struct	sB2BERT_NORM_REG19			InsErrEn;		//[0x19h] 
INT8U	Reserved26to27[2];							//[0x1Ah~0x1Bh], 2byte  
struct	sB2BERT_NORM_BertInsErrTime InsErrTime; 	//[0x1Ch~0x1Fh], 4byte 
struct	sB2BERT_NORM_Sync_Err_Max	SyncErrMax;   	//[0x20h~0x21h], 2byte
struct	sB2BERT_NORM_Sync_Status	SyncStatus;		//[0x22h] 
struct	sB2BERT_NORM_Sync_ReSync	SyncReStar;		//[0x23h]
struct	sB2BERT_NORM_ErrTime_Cnt	ErrTime;		//[0x24h~0x27h], 4byte 
struct	sB2BERT_NORM_ErrNum_Cnt		ErrNum;			//[0x28h~0x2Bh], 4byte  
INT8U	Reserved44to63[20];							//[0x2Ch~0x3Fh], 20byte 
INT8U	UserPattern[64];							//[0x40h~0x7Fh], 64byte  存放用户自定义序列的存储器，最长64个byte(512bit)，最短4个bit，数据发送从低位地址开始，每一个Byte是从高位D7开始到D0的顺序发送
};

union uB2BERT_NORM										  
{ 
struct strB2BERT_NORM  sStrB2BERT_NORM;
INT8U  pStrB2BERT_NORM[128]; 
};

struct sB2BERT_BURST_Rst
{
INT8U bit0:1;			//为1复位除寄存器外的相应部分的Burst Bert部分电路，为0正常工作，当全局寄存器的Reset为1时置位为1
INT8U reserved:7;		//reserved
};

struct sB2BERT_BURST_OLTRst
{
INT8U bit0:1;			//为1表示复位OLT光模块，为0正常工作，此位代替对应的OLT Pin寄存器位的功能。在正常工作时应置此位为0。两个Burst部分的复位是或地关系，即任意一个复位寄存器为1，就会复位OLT。此位不受全局控制寄存器0xb的bit1位影响。
INT8U reserved:7;		//reserved
};

struct sB2BERT_BURST_REG08
{
INT8U Framer_Not:1;		//为1表示Burst时曾经有过没有找到帧头的情况，为0正常。读后自动清掉
INT8U reserved7_1:7;	//reserved
};

struct sB2BERT_BURST_Pattern
{
INT8U isUserorPRBS:1;	//为1表示相应Burst Bert的测试序列为用户自定义序列，为0表示是m序列
INT8U reserved:7;		//reserved
};

struct sB2BERT_BURST_UserLen  //当选择用户定义序列时才有效，最长4096bit，最短4个bit。实际个数要加1
{
INT8U UserLen7_0;		//用户定义序列长度的低8位
INT8U UserLen11_8:4;	//用户定义序列长度的高4位
INT8U reserved:4;		//reserved  
};

struct sB2BERT_BURST_UserStatus  
{	 
INT8U User_Start:1;		//当测试序列为用户自定义序列时，为1启动测试，为0复位测试。当测试序列为m序列时设为0
INT8U User_Rdy:1;		//当测试序列为用户自定义序列时，当写User_Start为1启动测试时，为1表示发送还没准备好，为0表示准备好
INT8U reserved:6;		//reserved  
};

struct sB2BERT_BURST_REG17
{
INT8U InsertErrBit:1;	//写0 -- 1 -- 0在发送码流中插入一个bit误码，此动作不受寄存器0x19的Bert_InsErr_En的影响
INT8U reserved:7;		//reserved
};

struct sB2BERT_BURST_REG18
{
INT8U Out_Inv:1;		//为1控制发送输出反相，为0不反相
INT8U In_Inv:1;			//为1控制接收输入反相，为0不反相
INT8U reserved:6;		//reserved  
};

struct sB2BERT_BURST_InsErrEn
{
INT8U InsErrEnbit0:1;	//为1表示允许在发送输出测试序列中插入误码，为0不插入误码
INT8U reserved:7;		//reserved
};

struct sB2BERT_BURST_InsErrTime	//以发送频率的16分频为单位(插入1个bit误码)，实际时间要加1
{
INT8U Time7_0;			//插入误码时的时间间隔的最低8位
INT8U Time15_8;	
INT8U Time23_16;
INT8U Time31_24;  		//插入误码时的时间间隔的最高8位
};

struct sB2BERT_BURST_Sync_Err_Max	//每次判断检查了14784个bit，当误码个数小于设置的误码门限时就认为同步。当为PRBS码时，一般设置为0x5C6，而当为自定义数据时，一般设为0x004
{
INT8U Err_Max7_0;		//收同步时判断同步的误码个数低8位
INT8U Err_Max13_8:6;	//收同步时判断同步的误码个数高6位
INT8U reserved:2;		//reserved  
};

struct sB2BERT_BURST_Sync_Status
{
INT8U Status:1;			//为1表示接收处于同步状态，为0表示仍在搜索状态
INT8U reserved:7;		//reserved
};

struct sB2BERT_BURST_Sync_ReSync
{
INT8U ReSync:1;			//从0到1的跳变启动接收重新搜索同步
INT8U reserved:7;		//reserved
};

struct sB2BERT_BURST_ErrTime_Cnt //当写ErrTime7_0寄存器任意值，就锁定以前测试的误码累加个数和测试时间，同时启动下次测试 （两次读写间隔不能超出27s）
{
INT8U ErrTime7_0;		//误码测试的时间最低8位，以发送频率的16分频为单位，实际数字要加1，当超过32位表达时间时，读出值为全1。
INT8U ErrTime15_8;		
INT8U ErrTime23_16;		
INT8U ErrTime31_24;		//误码测试的时间的最高8位
};

struct sB2BERT_BURST_ErrNum_Cnt 
{
INT8U ErrNum7_0;		//误码测试的误码累加个数的最低8位，当超过40位表达个数时，读出值为全1。
INT8U ErrNum15_8;		
INT8U ErrNum23_16;		
INT8U ErrNum31_24;		//误码测试的误码累加个数的最高8位
};

struct sB2BERT_BURST_Preamble_Len 	//前导引导位长度，单位为字节(8个发送时钟宽度)，实际字节个数要加1，一般为5到6个字节
{
INT8U Len7_0;			//前导引导位长度的低8位
INT8U Len11_8:4;		//前导引导位长度的高4位
INT8U reserved:4;		//reserved  
};

struct sB2BERT_BURST_Data_Len 		//BURST发送数据宽度，1、单位为字节(8个发送时钟宽度)，2、实际发送字节数要加1。
{
INT8U Len7_0;			//Burst Bert发送数据宽度的低8位
INT8U Len15_8;			//Burst Bert发送数据宽度的中8位  
INT8U Len18_16:3;		//Burst Bert发送数据宽度的高3位
INT8U reserved:5;		//reserved  
};

struct sB2BERT_BURST_TxDly_Len  	//Burst Bert的突发发送数据的延时宽度，1、单位为字节(8个发送时钟宽度)，2、实际发送字节数要加1。
{
INT8U Len7_0;			//Burst Bert的突发发送数据的延时宽度寄存器的低8位
INT8U Len15_8;			//Burst Bert的突发发送数据的延时宽度寄存器的中8位  
INT8U Len18_16:3;		//Burst Bert的突发发送数据的延时宽度寄存器的高3位
INT8U reserved:5;		//reserved  
};

struct sB2BERT_BURST_RxDly_Len  	//测试出的发送线路延时，1、单位为字节(8个发送时钟宽度)，2、实际发送字节数要加1。
{
INT8U Len7_0;			//测试出的发送线路延时的低8位
INT8U Len15_8;			//测试出的发送线路延时的高8位 
};

struct sB2BERT_BURST_Train_Status  	
{
INT8U Bit_OK:1;			//为1表示接收调整Bit相位完毕，为0表示还在调整。当Burst_Train_State为2时才有效
INT8U Dly_OK:1;			//为1表示测试延时完毕，为0表示还在测试。当Burst_Train_State为3时才有效
INT8U reserved:6;		//reserved  
};

struct sB2BERT_BURST_OLTRst_Pos_Sel  	
{
INT8U bit0:1;			//为1表示对应ONU的OLT的复位信号由自定义位置产生的可编程宽度的复位信号代替，为0表示由内部电路自动计算宽度为两个发送频率的8分频时钟周期的复位信号
INT8U reserved:7;		//reserved  
};

struct sB2BERT_BURST_REG41
{
INT8U Pos7_0;			//12位对应ONU的OLT复位的自定义位置信号寄存器的低8位
INT8U Pos11_8:4;		//12位对应ONU的OLT复位的自定义位置信号寄存器的高4位
INT8U reserved:4;		//reserved  
};

struct sB2BERT_BURST_SDChk_Width
{
INT8U width7_0;			//19位检查SDD宽度寄存器的低8位
INT8U width15_8;			//20位对应ONU的OLT复位的自定义位置信号寄存器的中8位  
INT8U width18_16:3;		//20位对应ONU的OLT复位的自定义位置信号寄存器的高4位
INT8U reserved:5;		//reserved  
};

struct sB2BERT_BURST_SDChk_Status  	//当写此寄存器任意值，就清除SDA和SDD的检测结果 
{									
INT8U SDD:1;			//为1表示在对应的OLT_Rst上升沿后延时(SDA_ChkPos + 1)*8个发送时钟周期后宽度(SDDChk_Width + 1)*8个发送时钟周期内为不该出现SD为1的地方出现了SD为1，为0表示正常
INT8U SDA:1;			//为1表示在对应的OLT_Rst上升沿后延时(SDA_ChkPos + 1)*8个发送时钟周期后宽度(SDDChk_Width + 1)*8个发送时钟周期内为不该出现SD为0的地方出现了SD为0，为0表示正常
INT8U reserved:6;		//reserved  
};

struct sB2BERT_BURST_UserPattern  
{									
INT8U Table:2;			//存放用户自定义序列的存储器的高两位扩展地址
INT8U reserved:6;		//reserved
INT8U Data[128]; //存放用户自定义序列的存储器的读写窗口，每个窗口为128byte。用户自定义数据最长512个byte(4096bit)，最短4个bit，发送信号规则 Table0 byte0 D7......Table3 byte127 D0
};
	
struct strB2BERT_BURST		//定义BurstBERT寄存器
{
INT8U	FirstByte; 									//[0x00h] 
INT8U	Reserved1to3[3];							//[0x01h~0x03h], 3byte 
INT8U   CPU_Haddr;									//[0x04h] 
struct  sB2BERT_BURST_Rst 			BurstRst;		//[0x05h] 
struct  sB2BERT_BURST_OLTRst 		OLTRst;			//[0x06h]  	
INT8U	Reserved7;									//[0x07h]
struct  sB2BERT_BURST_REG08 		Framer;			//[0x08h]   
INT8U	Reserved9to15[7];							//[0x09h~0x0Fh], 7byte  
struct	sB2BERT_BURST_Pattern 		Pattern;		//[0x10h]
INT8U 	PRBS;										//[0x11h]
struct 	sB2BERT_BURST_UserLen		UserLen;		//[0x12h~0x13h], 2byte 
struct 	sB2BERT_BURST_UserStatus 	UserStatus;		//[0x14h] 
INT8U	Reserved21to22[2];							//[0x15h~0x16h], 2byte      
struct	sB2BERT_BURST_REG17			REG17;			//[0x17h]
struct	sB2BERT_BURST_REG18			INV;			//[0x18h] 
struct	sB2BERT_BURST_InsErrEn		InsErrEn;		//[0x19h] 
INT8U	Reserved26to27[2];							//[0x1Ah~0x1Bh], 2byte  
struct	sB2BERT_BURST_InsErrTime 	InsErrTime;		//[0x1Ch~0x1Fh], 4byte 
struct	sB2BERT_BURST_Sync_Err_Max	SyncErrMax;   	//[0x20h~0x21h], 2byte
struct	sB2BERT_BURST_Sync_Status	SyncStatus;		//[0x22h] 
struct	sB2BERT_BURST_Sync_ReSync	SyncReStar;		//[0x23h]
struct	sB2BERT_BURST_ErrTime_Cnt	ErrTime;		//[0x24h~0x27h], 4byte 
struct	sB2BERT_BURST_ErrNum_Cnt	ErrNum;			//[0x28h~0x2Bh], 4byte  
INT8U	Reserved44to47[4];							//[0x2Ch~0x2Fh], 4byte  
INT8U   Burst_State;								//[0x30h] 
INT8U   PreGuideLen;								//[0x31h] 前导保护位长度，单位为字节(8个发送时钟宽度)，实际字节个数要加1，一般为4个字节。
struct 	sB2BERT_BURST_Preamble_Len	PreambleLen;	//[0x32h~0x33h], 2byte  
struct 	sB2BERT_BURST_Data_Len		DataLen;		//[0x34h~0x36h], 3byte 
INT8U	Reserved55;									//[0x37h] 
struct 	sB2BERT_BURST_TxDly_Len		TxDelayLen;		//[0x38h~0x3Ah], 3byte 
INT8U	Reserved59;									//[0x3Bh] 
struct 	sB2BERT_BURST_RxDly_Len		RxDelayLen;		//[0x3Ch~0x3Dh], 2byte  
struct	sB2BERT_BURST_Train_Status	Train_Status;	//[0x3Eh]  
INT8U	Reserved63to64[2];							//[0x3Fh~0x40h], 2byte  
struct 	sB2BERT_BURST_REG41			OLTRst_Pos;		//[0x41h~0x42h], 2byte 
INT8U	Reserved67;									//[0x43h]
INT8U   OLTRst_Width;								//[0x44h] 
INT8U	Reserved69to71[3];							//[0x45h~0x47h], 3byte 
INT8U   SD_Level;									//[0x48h] OLT的SD信号有效电平选择，为0表示低电平有效，为1表示高电平有效  
INT8U   SDChk_Pos;									//[0x49h] 8位检查SD位置，对应本路OLT_Rst下降沿。注意：1、实际位置要加1。2、时间以发送频率的8分频为1单位。3、位置是相对于本路OLT复位信号的
struct 	sB2BERT_BURST_SDChk_Width	SDChk_Width;	//[0x4Ah~0x4Ch], 3byte 
struct	sB2BERT_BURST_SDChk_Status 	SDChk_Status;	//[0x4Dh] 
INT8U	Reserved78to126[49];						//[0x4Eh~0x7Eh], 49byte  
struct 	sB2BERT_BURST_UserPattern	UserPattern;	//[0x7Fh, 0x80h~0xFFh], 1byte + 128*4byte
};
										
union uB2BERT_BURST										  
{ 
struct strB2BERT_BURST  sStrB2BERT_BURST;
INT8U  pStrB2BERT_BURST[256]; 
};

struct strB2BERT
{                       
INT8U	FirstByte;				//[0], 1byte
INT8U	FirmwareVersionSuffix;	//[1], 1byte, 
INT8U	FirmwareVersion;		//[2], 1byte 										
INT8U	CPLDVersion;			//[3], 1byte,
INT16U	Core_Voltage;			//[4~5], 2byte 							
INT16S	Core_Temperature;		//[6~7], 2byte
INT8U   reserved8to9[2];		//[8~9], 2byte                                                  
INT16U  ADC0_V33_MON;			//[10~11], 2byte，
INT16U  ADC1_VRX_MON;			//[12~13], 2byte，
INT16U  ADC2_V25_MON;			//[14~15], 2byte，
INT16U  ADC3_V10_MON;			//[16~17], 2byte，
INT16U  ADC4_LD_MON;			//[18~19], 2byte，TOSA-LD前向电压
INT16U  DAC0_SET_VPD;			//[20~21], 2byte，3482或3571的boost-up电压调节
INT16U  DAC1_SET_VTR;			//[22~23], 2byte，VTR电压调节
INT16U  ADC14_PD_MON;			//[24~25], 2byte，PD管电流电压
INT16U  ADC15_MON;				//[26~27], 2byte，未用
INT16U  ADC_V33_MON;			//[28~29], 2byte
INT16U  ADC_VTR_MON;			//[30~31], 2byte
INT16U  ADC_V25_MON;			//[32~33], 2byte
INT16U  ADC_V10_MON;			//[34~35], 2byte
INT16U  ADC_VPDM_MON;			//[36~37], 2byte
INT16U  ADC_VTRx_MON;			//[38~39], 2byte
INT16U  ADC_VBUS_MON;			//[40~41], 2byte，实际是+5V的量
INT16U  ADC_LD_MON;				//[42~43], 2byte
INT16U  ADC_PD_MON;				//[44~45], 2byte
INT16U  ADC0_VPDM_MON;			//[46~47], 2byte
INT16U  DAC_ADN8810_SET;	    //[48~49], 2byte
INT16U  ADC15_SC_MON;			//[50~51], 2byte
INT16U  ADC_SC_MON;				//[52~53], 2byte
INT8U   reserved54;				//[54],    1byte
INT8U   Vapd_3571LT3482_sel;	//[55], 1byte; =0, 3571, Vapd正比于DAC
INT8U   reserved56to73[18];		//[56~73], 18byte
INT8U	BEN_mode;				//[74],    1byte 
INT8U   RSSI_mode;				//[75],    1byte 
INT16U  Tune_VPD_MON;			//[76~77], 2byte
INT8U   VapdAdj_loop;			//[78],    1byte 
INT8U   VapdAdj_en;				//[79],    1byte
float	Vapd_expect;			//[80~83], 4byte 
INT16U  Tune_IPD_MON;			//[84~85], 2byte
INT8U   IapdAdj_loop;			//[86],    1byte 
INT8U   IapdAdj_en;				//[87],    1byte
float	Iapd_expectMIN;			//[88~91], 4byte 
float	Iapd_expectMAX;			//[92~95], 4byte 
INT16U	Vapd20V_loop;			//[96~97], 2byte 
INT16U	Vapd20V_DAC;			//[98~99], 2byte 
INT8U   Vapd20V_en;				//[100],   1byte
INT8U	Vbr_3V4V_sel;			//[101],   1byte, =0 Vbr-3V; =1 Vbr-4V;
INT8U	Vbr_3V_en;				//[102],   1byte
INT8U	Vbr_3V_status;			//[103],   1byte
INT16U	ADC_Vbr_MON;			//[104~105],2byte
INT8U   Vapd_30V_en;			//[106],   1byte
INT8U   Vapd_30V_status;		//[107],   1byte
INT16U	ADC_V30V_MON;			//[108~109],2byte
INT16U	myPWMDAT0;				//[110~111],2byte
INT16U  Tune_VTR_MON;			//[112~113],2byte
INT8U   VTRAdj_loop;			//[114],    1byte 
INT8U   VTRAdj_en;				//[115],    1byte
float	VTR_expect;				//[116~119],4byte 
INT8U   delay1;					//[120],    1byte
INT8U   delay2;					//[121],    1byte
INT8U   delay3;					//[122],    1byte
INT32U  Password;  				//[123~126]   										
INT8U   LUTIndex;				//[127] 
INT8U	I2CSlaveAdd2;			//[128], 1byte
INT8U	I2CSlaveAdd0;			//[129], 1byte
INT8U	I2CSlaveAdd1;			//[130], 1byte
INT8U	CheckSumPNSN;			//[131], 1byte
INT8U   vendorPN[16];			//[132~147], 16byte
INT8U   vendorSN[16];			//[148~163], 16byte
INT8U	reserved164to227[64];	//[188~227], 40byte           
INT16U	RSSI_PW;				//[228~229], 2byte 
INT8U	RSSI_polarity;			//[230], 1byte 
INT8U	Reserved231;			//[231], 1byte 
float	Vapd_expectMIN;			//[232~235], 4byte 
float	Vapd_expectMAX;			//[236~239], 4byte 
float	Vapd_DeltaMAX;			//[240~243], 4byte,
INT16U	Vapd20V_loop_ini;		//[244~245], 2byte 
INT16U	Vapd20V_DAC_ini;		//[246~247], 2byte 
INT8U   Vapd20V_en_ini;			//[248], 1byte
INT8U	BEN_polarity;			//[249], 1byte 
INT16U	BEN_PW;					//[250~251], 2byte 
INT8U	Reserved252to255[4];	//[252~255], 4byte 
};

union uB2BERT										  
{ struct strB2BERT  sStrB2BERT;
  INT8U  pStrB2BERT[256];
};

#pragma pack ()  /*取消自定义字节对齐方式*/

//BERT pattern
enum eB2BERT_PATTERN   //PRBS7、PRBS9、PRBS11、PRBS15、PRBS20、PRBS21、PRBS23、PRBS29、PRBS31
{   
	B2BERT_PATT_PRBS,
	B2BERT_PATT_USER
};

//BERT PRBS
enum eB2BERT_PRBS   //PRBS7、PRBS9、PRBS11、PRBS15、PRBS20、PRBS21、PRBS23、PRBS29、PRBS31
{   
	B2BERT_PATT_PRBS7=7,
	B2BERT_PATT_PRBS9=9,
	B2BERT_PATT_PRBS11=11,
	B2BERT_PATT_PRBS15=15,
	B2BERT_PATT_PRBS20=20,
	B2BERT_PATT_PRBS21=21,
	B2BERT_PATT_PRBS23=23,
	B2BERT_PATT_PRBS29=29,
	B2BERT_PATT_PRBS31=31
};	

//BERT rate
enum eB2BERT_RATE
{   
	B2BERT_RATE_GPON,
	B2BERT_RATE_EPON
};	

//BERT Norm工作模式下选择接收的ONU
enum eB2BERT_NORM_ONU
{   
	B2BERT_NORM_OLTtoONU0,
	B2BERT_NORM_OLTtoONU1
};	

//BERT Norm工作模式下选择是否需要全局复位
enum eB2BERT_NORM_GLOBALRst
{   
	B2BERT_NORM_neverGLOBALRst,
	B2BERT_NORM_needGLOBALRst
};	

//BERT mode
enum eB2BERT_MODE
{   
	B2BERT_MODE_OLTtoONU0,
	B2BERT_MODE_OLTtoONU1,
	B2BERT_MODE_ONU0toOLT,
	B2BERT_MODE_ONU1toOLT,
	B2BERT_MODE_ONU0andONU1toOLT,
	B2BERT_MODE_ONU0andONU1toOLTtoONU0,
	B2BERT_MODE_ONU0andONU1toOLTtoONU1,
	B2BERT_MODE_ONU0toOLTtoONU0,
	B2BERT_MODE_ONU1toOLTtoONU1
};	

//BERT Burst Trigger
enum eB2BERT_BURST_TRIGGER
{   
	B2BERT_BURST_TRIG_CYCLE,
	B2BERT_BURST_TRIG_PULSE
};

//BERT Burst Trigger out
enum eB2BERT_TRIGGER_OUT
{   
	B2BERT_TRIG_OUT_CLOSE,
	B2BERT_TRIG_OUT_OPEN
};

//BERT Burst Trigger level
enum eB2BERT_TRIGGER_LEVEL
{   
	B2BERT_TRIG_LEVEL_LOW,
	B2BERT_TRIG_LEVEL_HIGH
};

//BERT OLT Rst Out
enum eB2BERT_OLTRst_OUT
{   
	B2BERT_OLTRst_OUT_OPEN,
	B2BERT_OLTRst_OUT_CLOSE
};

//BERT OLT Rst level
enum eB2BERT_OLTRst_LEVEL
{   
	B2BERT_OLTRst_LEVEL_LOW,
	B2BERT_OLTRst_LEVEL_HIGH
};

//BERT OLT Rst Pos mode
enum eB2BERT_OLTRst_POS_MODE
{   
	B2BERT_OLTRst_POS_AUTO,
	B2BERT_OLTRst_POS_USER
} eB2BERT_OLTRst_POS_MODE;

//BERT SD Level
enum eB2BERT_SD_LEVEL
{   
	B2BERT_SD_LOW,
	B2BERT_SD_HIGH
};

//extern union uB2BERT_GLOBAL B2BERT_GLOBAL;
//extern union uB2BERT_NORM 	B2BERT_NORM; 
//extern union uB2BERT_BURST 	B2BERT_BURST; 
//extern union uB2BERT 		B2BERT; 

int	B2BERT_Addr;  

#define B2_USERPATT_MAX_BYTENUMBER   512 
#define B2_USERPATT_MAX_ROWNUMBER    32 
#define B2_USERPATT_MAX_PATTFILESIZE 1024

char B2BERT_PATTFILE[MAX_PATHNAME_LEN];
unsigned char B2BERT_PATT[B2_USERPATT_MAX_BYTENUMBER]; 

extern int B2BERT_Init (int *USB_Handle, char *SN); 				//根据sn获取设备端口号
extern int B2BERT_SET_CPU_Haddr (int USB_Handle, int Haddr);		//虚拟高位地址，= 0选择全局寄存器部分，= 1选择Norm寄存器部分，= 2选择Burst1寄存器部分，= 2选择Burst2寄存器部分
extern int B2BERT_SET_RATE (int USB_Handle, int rate);				//设置速率， 0=GPON， 1=EPON
extern int B2BERT_SET_Sel_Rx_ONU (int USB_Handle, int sel_rx_onu);  //选择Norm工作状态下的ONU接收
extern int B2BERT_SET_GLOBAL_Rst (int USB_Handle, int Rst); 		//设置全局复位寄存器
extern int B2BERT_SET_GLOBAL_OLT_TxDis (int USB_Handle, int TxDisEn); 
extern int B2BERT_SET_GLOBAL_PeriodTrigSel (int USB_Handle, int TrigSel);//Burst Bert的发送方式选择，为0表示周期发送，为1表示单次发送，由本寄存器的bit1触发
extern int B2BERT_SET_GLOBAL_PeriodTrig (int USB_Handle, int Trig); 	//此bit为0--1--0就触发一次Burst发送，只有当本寄存器的bit0为1时才有效
extern int B2BERT_SET_GLOBAL_BurstPeriod (int USB_Handle, unsigned int BurstPeriod);
extern int B2BERT_SET_GLOBAL_TrigOut (int USB_Handle, int TrigOut);		//为0表示关闭采样信号的输出，为1表示打开的输出 
extern int B2BERT_SET_GLOBAL_TrigLevel (int USB_Handle, int TrigLevel);	//采样信号有效电平选择，为0表示低电平有效，为1表示高电平有效
extern int B2BERT_SET_GLOBAL_TrigPos (int USB_Handle, unsigned int TrigPos);
extern int B2BERT_SET_GLOBAL_TrigWidth (int USB_Handle, unsigned int TrigWidth);
extern int B2BERT_SET_GLOBAL_OLTRst_Out (int USB_Handle, int OLTRstOut);//为1表示关闭OLTRst信号的输出，为0表示打开的输出
extern int B2BERT_SET_GLOBAL_OLTRst_Level (int USB_Handle, int OLTRstLevel);//OLTRst信号有效电平选择，为0表示低电平有效，为1表示高电平有效
extern int B2BERT_GET_GLOBAL_AD2817_Status (int inst_B2, int *LOS, int *LOL);
extern int B2BERT_SET_GLOBAL_FPGA_Dly (int USB_Handle, int Rst);
extern int B2BERT_SET_GLOBAL_OLTRst_Dly (int USB_Handle, unsigned char Dly);
extern int B2BERT_SET_GLOBAL_GLOBAL_Dly (int USB_Handle, unsigned char Dly);

extern int B2BERT_SET_NORM_Rst (int USB_Handle, int Rst);
extern int B2BERT_SET_NORM_Loop (int USB_Handle, int Rx2Tx_Loop, int Tx2Rx_Loop, int VSC_Rx2Tx_Loop, int VSC_Tx2Rx_Loop); 
extern int B2BERT_SET_NORM_Pattern (int USB_Handle, int Pattern);		//设置码型为自定义或m序列，0=m序列，1=自定义序列  
extern int B2BERT_SET_NORM_PRBS (int USB_Handle, int PRBS);				//设置PRBS序列码型 
extern int B2BERT_SET_NORM_User (int USB_Handle, int UserStart);		//设置自定义码型状态 
extern int B2BERT_SET_NORM_INV (int USB_Handle, int InINV, int OutINV);	//设置输入输出反相功能  
extern int B2BERT_SET_NORM_InsErrEn (int USB_Handle, int InsErrEn);		//设置是否插入误码
extern int B2BERT_SET_NORM_InsErrTime (int USB_Handle, unsigned int InsErrTime);		//设置插入误码时间
extern int B2BERT_SET_NORM_SyncErrMax (int USB_Handle, unsigned short int SyncErrMax);	//设置是否插入误码 
extern int B2BERT_SET_NORM_AlignStar (int USB_Handle, int Align);
extern int B2BERT_GET_NORM_AlignStatus (int USB_Handle, int *AlignOK, int *Ctrl_OK, int *Ctrl_Err); 
/*
AlignOK：为1表示SFI41接口中延时控制状态机调整完毕，为0正在调整
Ctrl_OK：为1表示SFI41接口中延时控制部件复位完毕，为0正在复位
Ctrl_Err：为1表示SFI41接口中延时控制部件工作错误，为0正常
*/
extern int B2BERT_SET_NORM_SyncReStar (int USB_Handle, int SyncReStar);
extern int B2BERT_GET_NORM_SyncStatus (int USB_Handle, int *Sync);		//为1表示接收处于同步状态，为0表示仍在搜索状态 

extern int B2BERT_SET_BURST_Rst (int USB_Handle, int Rst);
//extern int B2BERT_SET_BURST_Loop (int USB_Handle, int Loop); 
extern int B2BERT_SET_BURST_Pattern (int USB_Handle, int Pattern);		//设置码型为自定义或m序列，0=m序列，1=自定义序列  
extern int B2BERT_SET_BURST_PRBS (int USB_Handle, int PRBS);			//设置PRBS序列码型
extern int B2BERT_SET_BURST_UserStart (int USB_Handle, int UserStart);	//设置自定义码型状态
extern int B2BERT_GET_BURST_UserState (int USB_Handle, int *UserState); //读取自定义码型发送状态, 为1表示发送还没准备好，为0表示准备好
extern int B2BERT_SET_BURST_INV (int USB_Handle, int InINV, int OutINV);//设置输入输出反相功能
extern int B2BERT_SET_BURST_InsErrEn (int USB_Handle, int InsErrEn);	//设置是否插入误码
extern int B2BERT_SET_BURST_InsErrTime (int USB_Handle, unsigned int InsErrTime);		//设置插入误码时间
extern int B2BERT_SET_BURST_SyncErrMax (int USB_Handle, unsigned short int SyncErrMax);	//设置是否插入误码 
extern int B2BERT_SET_BURST_State (int USB_Handle, int state);							//设置Burst工作状态
/*
0：关闭相应的ONU发送
1：发送连续的时钟信号，用于接收调整采样延时
2：发送连续的特殊字符，用于接收调整Bit相位
3：发送连续的特殊帧信号，用于接收测量线路延时
4：发送连续的m序列或自定义数据。在此种模式下就是一般的连续Bert。
5：空闲，关闭发送。
6：保留，相当于关闭发送 
7：工作，用于发送突发的m序列或自定义数据，用于正式的Burst Bert测试。
*/
extern int B2BERT_SET_BurstCtrlRst (int USB_Handle);		//设置两个Burst Bert部分公用的输入延时控制的复位信号，从0到1的跳变触发复位
extern int B2BERT_SET_GLOBAL_BurstCtrlRst (int USB_Handle, int Rst);
extern int B2BERT_GET_GLOBAL_BurstCtrlStatus (int USB_Handle, int *BurstCtrlOK, int *BurstCtrlErr);//BurstCtrlOK为1表示Burst Bert部分中接口延时控制部件复位完毕，为0正在复位;BurstCtrlErr为1表示Burst Bert部分中延时控制部件工作错误，为0正常
extern int B2BERT_SET_BURST_PreGuideLen (int USB_Handle, unsigned char PreGuideLen);		
extern int B2BERT_SET_BURST_PreambleLen (int USB_Handle, unsigned short PreambleLen);
extern int B2BERT_SET_BURST_DataLen (int USB_Handle, unsigned int DataLen);
extern int B2BERT_SET_BURST_TxDelayLen (int USB_Handle, unsigned int TxDelayLen);
//extern int B2BERT_SET_BURST_OLTRst_Pos_Sel (int USB_Handle, int OLTRst_Pos_Sel);
extern int B2BERT_SET_BURST_OLTRst_Pos (int USB_Handle, unsigned short OLTRst_Pos);
extern int B2BERT_SET_BURST_OLTRst_Width (int USB_Handle, unsigned char OLTRst_Width);
extern int B2BERT_SET_BURST_OLTRst (int USB_Handle, int Rst);
//extern int B2BERT_SET_BURST_AlignStar (int USB_Handle, int Align);
//extern int B2BERT_GET_BURST_AlignStatus (int USB_Handle, int *AlignOK); 
extern int B2BERT_GET_BURST_TrainStatus (int USB_Handle, int *Bit_OK, int *Dly_OK);	//Bit_OK:为1表示接收调整Bit相位完毕，为0表示还在调整。当Burst_Train_State为2时才有效；Dly_OK:为1表示测试延时完毕，为0表示还在测试。当Burst_Train_State为3时才有效
extern int B2BERT_SET_BURST_SyncReStar (int USB_Handle, int SyncReStar);
extern int B2BERT_GET_BURST_SyncStatus (int USB_Handle, int *Sync);					//为1表示接收处于同步状态，为0表示仍在搜索状态 
extern int B2BERT_SET_BURST_SD_Level (int USB_Handle, int SD_Level);				//为0表示低电平有效，为1表示高电平有效
extern int B2BERT_SET_BURST_SDChk_Pos (int USB_Handle, unsigned char SDChk_Pos);	//设置SDA/SDD检测位置，相对于OLT_Rst下降沿
extern int B2BERT_SET_BURST_SDChk_Width (int USB_Handle, unsigned int SDChk_Width);	//设置SDA/SDD检测框，实际宽度要加1，时间以发送频率的8分频为1单位
extern int B2BERT_SET_BURST_UserPatternLen (int USB_Handle, unsigned short UserPatternLen);		//设置自定义码型长度
extern int B2BERT_SET_BURST_UserPatternData (int USB_Handle, unsigned char *UserPatternData);   //设置自定义码型数据

//应用函数
extern int B2BERT_SET_NORM (int USB_Handle, int RstSel, int rate, int SelONU, int InINV, int OutINV, int Pattern, int PRBS, unsigned short int SyncErrMax); 	//设置NORM配置 
extern int B2BERT_GET_NORM_Align (int USB_Handle, int *align);						//训练
extern int B2BERT_GET_NORM_Sync (int USB_Handle, int *sync_state);					//搜索NORM同步
extern int B2BERT_GET_NORM_ErrCount (int USB_Handle, int rate, unsigned int *ErrNum, double *ErrTime, double *ErrRate);
extern int B2BERT_SET_NORM_ClearErr (int USB_Handle);	//清除误码读数
extern int B2BERT_SET_NORM_Close (int USB_Handle);		//关闭Norm工作状态 

extern int B2BERT_SET_BURST_COMMON (int USB_Handle, int rate, int SelONU, int BurstPeriod_Sel, unsigned int BurstPeriod, 
	                                int Trig_Out, int Trig_Level, unsigned int Trig_Pos, unsigned int Trig_Width, int OLTRstOut, int OLTRstLevel);//设置BURST GPON公共配置 
extern int B2BERT_SET_BURST_ParaConfig (int USB_Handle, int Pattern, unsigned short UserPatternLen, int PRBS, int InINV, int OutINV, unsigned short int SyncErrMax);		//设置Burst参数配置  				
extern int B2BERT_SET_BURST_TransConfig (int USB_Handle, int Pattern, unsigned char PreGuideLen, unsigned short int PreAmbleLen, unsigned int DataLen, unsigned int TxDelay, 
										 int OLTRst_Out, unsigned short OLTRst_Pos, unsigned char OLTRst_Width, int SD_Level, unsigned char SDChk_Pos, unsigned int SDChk_Width);	//设置Burst传输配置
extern int B2BERT_SET_BURST_ClearErr (int USB_Handle);	//清除误码读数
extern int B2BERT_GET_BURST_Align (int USB_Handle, int *align);						//训练  
extern int B2BERT_GET_BURST_Sync (int USB_Handle, int *sync_state);					//搜索BURST同步 返回值-2搜索同步失败，其它<0的返回值表示函数执行异常
//B版本不支持此项功能，读取SD状态后B2自动清除
//extern int B2BERT_SET_BURST_ClearSD (int USB_Handle);	//清除SD结果 
extern int B2BERT_GET_BURST_SDChk_Status (int USB_Handle, int *SDA, int *SDD);		//读取SD结果
//B版本不支持此项功能，读取SD状态后B2自动清除 
//extern int B2BERT_GET_BURST_SDChk_Status_noClear (int USB_Handle, int *SDA, int *SDD);//读取SD结果，不带自动清空状态功能
extern int B2BERT_GET_BURST_ErrCount (int USB_Handle, int rate, unsigned int *ErrNum, double *ErrTime, double *ErrRate);
extern int B2BERT_SET_BURST_Start (int USB_Handle, int ONU0State, int ONU1State);  	//设置开始工作状态
extern int B2BERT_SET_BURST_UserPatternOpen (int USB_Handle);						//设置BERT为自定义码型工作状态，自定义码型由当前寄存器的设置决定
extern int B2BERT_SET_BURST_UserPatternClose (int USB_Handle);						//设置BERT为伪随机码型工作状态，m序列的确定由当前寄存器的设置决定 

extern int B2BERT_CreatPatternFile (const int datalen, unsigned char *data, char *filename);
extern int B2BERT_ReadPatternFile (const char *filename, unsigned short *datalen, unsigned char *data); 
extern int B2BERT_SET_CDR (int USB_Handle);				//配置CDR
extern int BERT_Align(int USB_Handle);
#endif
