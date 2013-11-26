#include "logonTM.h"

void logonTM(const char* QQAccount,const char* QQPwd)
{
	WindowElementFinder::Logon(TEXT("C:\\Program Files (x86)\\Tencent\\TM2008\\Bin\\TM.exe"),QQAccount, QQPwd);
}