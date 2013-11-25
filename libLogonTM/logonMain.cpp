#include "logonTM.h"

void logonTM(LPCTSTR path, string QQAccount, string QQPwd)
{
	WindowElementFinder::Logon(path, QQAccount, QQPwd);
}