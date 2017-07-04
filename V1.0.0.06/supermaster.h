#ifndef _SUPERMASTER_H_
#define _SUPERMASTER_H_ 

#include <windows.h>  
#include "SuperCommand.h"
#include "CH341A_DLL.h"
#include "global.h"

extern int I2C_Write(BYTE nDev, BYTE nReg, BYTE nLen, BYTE* pbyDat);
extern int I2C_Read(BYTE nDev,BYTE nReg,BYTE nLen, BYTE* pbyBuf);
extern int RegisterI2cCallBackFunction(int handle);
extern int SetCommand(int handle, char *strCmd, char *strOupt);

extern int SetI2cUSBHandle(int handle);

#endif 
