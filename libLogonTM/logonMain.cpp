#include "ControlsFinder.h"
#include "logonTM.h"
#include "QQAccountBuilder.h"

QQAccountBuilder qqbuilder;

void logonTM(const char* QQAccount)
{
	if (NULL == QQAccount)
	{
		return;
	}
	string QQPwd = qqbuilder.getPwdByAccounts(QQAccount);
	if (QQPwd.empty())
	{
		return;
	}
	WindowElementFinder::Logon(qqbuilder.getPath(), QQAccount, QQPwd);
}

void addAQQRec(const char* QQAccount, const char* QQPwd)
{
	qqbuilder.addaRec(QQAccount, QQPwd);
}

int getAllAccounts(char* rlt, int buflen)
{
	vector<string> qqs = qqbuilder.getAll();
	if (qqs.empty())
	{
		return 0;
	}
	vector<string>::iterator it = qqs.begin();
	string strQq = *it;
	for ( ++it;it != qqs.end(); ++it)
	{
		strQq += "-" + (*it);
	}
	if (buflen > strQq.length())
	{
		strcpy(rlt, strQq.c_str());
	}
	return strQq.length();
}
