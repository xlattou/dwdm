

/****************************************************************************
 *
 * File:                FSW.h
 *
 * Author:              SUPERXON INC.(Roger Li)
 *
 * Description:         ����34����ģ�⿪�ؿ��ƴ���
 *
 * Time:                2013-07-17
 *
 * version:				v1.0.0.0
 * 
 * Update Description:  
****************************************************************************/

#ifndef _FSW_H_
#define _FSW_H_

int FSW_Init(int COMIndex); //��ʼ�����������ܴ��ڶ˿ڱ�ռ�õ����������ͨ���ٴε��ô˺������  
int FSW_Close(int COMIndex); 
int FSW_SetChannel(int COMIndex, int channel);

#endif  
