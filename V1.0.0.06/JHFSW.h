

/****************************************************************************
 *
 * File:                FSW.h
 *
 * Author:              SUPERXON INC.(Roger Li)
 *
 * Description:         ��������⿪�ؿ��ƴ���
 *
 * Time:                2013-07-17
 *
 * version:				v1.0.0.0
 * 
 * Update Description:  
�˰汾ֻ֧���豸��ַΪ01�Ĺ⿪�� 
****************************************************************************/

#ifndef _JHFSW_H_
#define _JHFSW_H_

int JHFSW_Init(int COMIndex); //��ʼ�����������ܴ��ڶ˿ڱ�ռ�õ����������ͨ���ٴε��ô˺������  
int JHFSW_Close(int COMIndex); 
int JHFSW_SetChannel(int COMIndex, int channel);

#endif  
