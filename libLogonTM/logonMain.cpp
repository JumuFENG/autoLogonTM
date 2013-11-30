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
	TCHAR szPath[MAX_PATH] = {0};
	string qpath = qqbuilder.getPath();
#if defined(UNICODE)||defined(_UNICODE)
	int slen = strlen(qpath.c_str());
	int len = MultiByteToWideChar(CP_ACP,0,qpath.c_str(),slen,NULL,0);
	wchar_t* m_wchar=new wchar_t[len+1];  
	MultiByteToWideChar(CP_ACP,0,qpath.c_str(),slen,m_wchar,len);  
	m_wchar[len]=L'\0';
	_tcscpy_s(szPath, m_wchar);
	delete[] m_wchar;
#else
	_tcscpy_s(szPath, qpath.c_str());
#endif
	WindowElementFinder::Logon(szPath, QQAccount, QQPwd);
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

void setQPath(const char* qpath)
{
	qqbuilder.setPath(qpath);
}

int getLastPath(char* rlt, int buflen)
{
	string spath = qqbuilder.getLastQpaht();
	if (spath.length() < buflen)
	{
		strcpy(rlt, spath.c_str());
	}
	return spath.length();
}