#include <utility.h>
#include <userint.h>
#include "XGSPON OLT Parallel ATE.h" 

//==============================================================================
//
// Title:       Measure Data.c
// Purpose:     A short description of the implementation.
//
// Created on:  2017-06-13 at 15:55:48 by Sky123.Org.
// Copyright:   Sky123.Org. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

//#include "Measure Data.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?
int Define_Your_Functions_Here (int x)
{
    return x;
}
int Chan0_Handle = -1;;
int Initial_Measure(int currentHandle)
{
	Chan0_Handle = currentHandle;
	return 0;
}
int CVICALLBACK Callback_SetItemTest1 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	Point activeCell1;  
	double  itemVlue;
	switch (event)
	{
		case EVENT_COMMIT:
			// 获取当前焦点单元格
			GetActiveTableCell(Chan0_Handle, PAN_SEAR0_CHAN0_DATA,&activeCell1);
			// 获取焦点单元格内容
			GetTableCellVal (Chan0_Handle, PAN_SEAR0_CHAN0_DATA, activeCell1, &itemVlue);
			// 单元格设置选中边框颜色
			//SetTableCellAttribute (Chan0_Handle, PAN_SEAR0_CHAN0_DATA, MakePoint(2,2), ATTR_TEXT_BGCOLOR, VAL_RED);
			
			
			break;
	}
	return 0;
}

int CVICALLBACK CallbackTest2 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	Point activeCell;
	
	double vlueTemp = 711;
	int aaa,bbb;
	switch (event)
	{
		case EVENT_COMMIT:
			// 获取当前焦点单元格
			//*pCell = MakePoint(1,2);
			//SetActiveTableCell (Chan0_Handle, PAN_SEAR0_CHAN0_DATA,*pCell);
			//GetActiveTableCell(Chan0_Handle, PAN_SEAR0_CHAN0_DATA,&activeCell);
			// 焦点单元格填充内容
			activeCell.x = 1;
			activeCell.y = 2;
			SetTableCellVal (Chan0_Handle, PAN_SEAR0_CHAN0_DATA, activeCell, vlueTemp);
			aaa = activeCell.x;
			bbb = activeCell.y;
			break;
	}
	return 0;
}

int CVICALLBACK Callback_Chan0_Hide (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			// 隐藏通道0测试面板			   

			HidePanel(Chan0_Handle);
			break;
	}
	return 0;
}
// 测试面板添加30行25列数据
int AddItemData(int rowNum, int colNum)
{
	// 行列计数
	int i;
	int j;
	double numTmp = 0;
	Point cellAddr;
	for(j = 1; j <= rowNum; j++)
	{
		// Table添加一行
		InsertTableRows (Chan0_Handle, PAN_SEAR0_CHAN0_DATA, j, 1, VAL_CELL_NUMERIC);
		for(i = 1; i <= colNum; i++)
		{
			cellAddr.x = i;
			cellAddr.y = j;
			SetTableCellVal (Chan0_Handle, PAN_SEAR0_CHAN0_DATA, cellAddr, numTmp++);
			Delay(0.5);
		}
	}
	return 0;	
}
int CVICALLBACK Callback_AddData (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int RowCount = 0;
	int rowDel = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			if(RowCount != 0)
			{
			    // 获取当前有多少行
				GetNumTableRows (Chan0_Handle, PAN_SEAR0_CHAN0_DATA, &rowDel);
				if(rowDel > 0)
				{
					// 从0行开始全部删除
					DeleteTableRows (Chan0_Handle, PAN_SEAR0_CHAN0_DATA, 1, rowDel);
				}
			}
			else
			{
				// 如果列表为空，添加一些数据
				AddItemData(30,25);
				RowCount++;
			}
			break;
	}
	return 0;
}
