#include "supermaster.h" 
#include <utility.h>
#include <ansi_c.h>
#include <userint.h>

char strInpt[256], strOupt[256]; 

int USB_Handle = 0;   

static void GetLock (int tmpthreadLock, int *obtainedLock)
{
	double timeout = 300;  //timeout 300s, 5 min
    double startTime = Timer();
	
    do
    {
        // NOTE - If needed, you can process messages, etc, here.
        CmtTryToGetLock(tmpthreadLock, obtainedLock);
    } while (!*obtainedLock && Timer() - startTime < timeout);
}

int I2C_Write(BYTE nDev, BYTE nReg, BYTE nLen, BYTE* pbyDat)
{
    int i;   
	BYTE pbyVal[256];   
	
	if (-1 == USB_Handle)
	{
		return -1;
	}
    
    memset(pbyVal,0,256);

    for (i = 0; i < nLen; i++)
    {
        pbyVal[(nReg + i)%256] = pbyDat [i];
    }

    return   I2C_BYTEs_WRITE_DLL(USB_Handle, nDev, nReg, nLen, pbyVal, 0);
}

int I2C_Read(BYTE nDev,BYTE nReg,BYTE nLen, BYTE* pbyBuf)
{
    int i, wRes;
	BYTE pbyVal[256]; 
	
	if (-1 == USB_Handle)
	{
		return -1;
	}
	  
    memset(pbyVal,0,256);

    wRes = I2C_BYTEs_READ_DLL (USB_Handle, nDev,nReg,nLen, pbyVal);
    if (0 == wRes)
    {
        for (i = 0; i < nLen; i++)
        {
            pbyBuf[i] = pbyVal[nReg + i];
        }
    }
    return wRes;
}

int RegisterI2cCallBackFunction(int handle)
{
    char strCmdR [] = "I2C_READ";
	char strCmdW [] = "I2C_WRITE";	
    int wRes = 0; 
	
	USB_Handle = handle;
    wRes += RegistCallBackFunciton(strCmdR,&I2C_Read);
    
    wRes += RegistCallBackFunciton(strCmdW,&I2C_Write);
	
    return wRes;
}

int SetI2cUSBHandle(int handle)
{
    char strCmdR [] = "I2C_READ";
	char strCmdW [] = "I2C_WRITE";	
    int wRes = 0; 
	
	USB_Handle = handle;

    return wRes;
}

/*
int SetCommand(char *strCmd, char *strOupt)
{
	int wRes = 0;
	int wValue = 0;
	
	wRes = SuperCmdSer(strCmd,strOupt);
    if (ERR_SUCCESS == wRes)
    {
		;
    }
	else
	{
		GetErroInfo(wRes,strOupt);
	}
	return wRes;
}
*/

int SetCommand(int handle, char *strCmd, char *strOupt)
{
	int wRes = 0;
	int wValue = 0;
	
	int obtainedLock = 0; 
	int index = 0;
	int MAXINDEXCOUNT = 5;
	
	GetLock(ThreadLock_SuperCMD, &obtainedLock); 
	if (obtainedLock)
	{
	//	Delay(0.1);
		SetI2cUSBHandle (handle);   
	//	Delay(0.2);
		for (index=0;index<MAXINDEXCOUNT;index++)
		{
			wRes = SuperCmdSer(strCmd,strOupt);
		    if (ERR_SUCCESS == wRes)
		    {
				break;
		    }
			else
			{
				GetErroInfo(wRes,strOupt);
				Delay(0.2);
			}
		}
		
		CmtReleaseLock(ThreadLock_SuperCMD);
		
		return wRes;
	}
	else
	{
		return -1;     
	}
	
	return 0;
}	

