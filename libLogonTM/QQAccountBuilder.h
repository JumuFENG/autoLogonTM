#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "base32.h"

using std::string;
using std::vector;
using std::pair;
using std::make_pair;
using std::fstream;
using std::ofstream;
using std::ifstream;

class QQAccountBuilder{
	bool loaded,dirty;
	vector<pair<string, string> > QContainer;
	string filename;
	string qpath;
	void prepare();
	void unprepare();

	void getQQpath(vector<string> & vec_path);

public:
	QQAccountBuilder(string filename = "accs.dat");
	~QQAccountBuilder();

	string getPath();
	string getLastQpaht(){return qpath;}
	void setPath(const char* qpath);
	void addaRec(const char* QQAccount, const char* QQPwd);
	vector<string> getAll();
	string getPwdByAccounts(const char* QQAccount);

};

QQAccountBuilder::QQAccountBuilder(string filename):dirty(false),loaded(false),filename(filename)
	,qpath("")
{

}

QQAccountBuilder::~QQAccountBuilder()
{
	if (dirty)
	{
		unprepare();
	}
}

void QQAccountBuilder::prepare()
{
	if (loaded)
	{
		return;
	}
	ifstream file(this->filename,ifstream::in|std::ios::binary);
	size_t pathlen = 0;
	file.read((char*)&pathlen, sizeof(size_t));
	char* chPath = new char[pathlen+1];
	file.read(chPath, pathlen);
	chPath[pathlen] = '\0';
	this->setPath(chPath);
	size_t nPair = 0;
	file.read((char*)&nPair,sizeof(size_t));
	for (size_t i = 0; i < nPair; ++i)
	{
		size_t nLen = 0;
		file.read((char*)&nLen, sizeof(size_t));
		char* pAcc = new char[nLen];
		file.read(pAcc, nLen);
		size_t nPlen = 0;
		file.read((char*)&nPlen, sizeof(size_t));
		char* pPwd = new char[nPlen];
		file.read(pPwd, nPlen);
		QContainer.push_back(make_pair(string(pAcc,nLen),string(pPwd, nPlen)));
		if (pAcc != NULL)
		{
			delete[] pAcc;
			pAcc = NULL;
		}
		if (pPwd != NULL)
		{
			delete[] pPwd;
			pPwd = NULL;
		}
	}
	file.close();
	loaded = true;
}

void QQAccountBuilder::unprepare()
{
	if (!dirty)
	{
		return;
	}
	size_t pairs = QContainer.size();
	ofstream file(this->filename, ofstream::out|std::ios::binary|std::ios::trunc);

	size_t pathlen = this->qpath.length();
	file.write((char*)&pathlen, sizeof(size_t));
	file.write(this->qpath.c_str(),pathlen);

	file.write((char*)&pairs, sizeof(size_t));
	for (vector<pair<string, string> >::iterator it = QContainer.begin();
		it != QContainer.end(); ++it)
	{
		size_t len = (*it).first.length();
		file.write((char*)&len,sizeof(size_t));
		file.write((*it).first.c_str(),(*it).first.length());
		len = (*it).second.length();
		file.write((char*)&len,sizeof(size_t));
		file.write((*it).second.c_str(),(*it).second.length());
	}
	file.close();
}

void QQAccountBuilder::addaRec(const char* QQAccount, const char* QQPwd)
{
	if (QQAccount == NULL || QQPwd == NULL)
	{
		return;
	}
	if (!loaded)
	{
		prepare();
	}
	bool bexists = false;
	for (vector<pair<string,string> >::iterator it = QContainer.begin();it!=QContainer.end();++it)
	{
		if ((*it).first.compare(QQAccount) == 0)
		{
			if ((*it).first.compare(QQAccount) != 0)
			{
				(*it).second = string(QQPwd);
				dirty = true;
			}
			bexists = true;
		}
	}
	if (!bexists)
	{
		QContainer.push_back(make_pair(string(QQAccount),string(QQPwd)));
		dirty = true;
	}
}

string QQAccountBuilder::getPwdByAccounts(const char* QQAccount)
{
	if (!loaded)
	{
		prepare();
	}
	for(vector<pair<string, string> >::iterator it = QContainer.begin();
		it != QContainer.end(); ++it)
	{
		if ((*it).first.compare(string(QQAccount)) == 0)
		{
			return (*it).second;
		}
	}
	return "";
}

vector<string> QQAccountBuilder::getAll()
{
	if (!loaded)
	{
		prepare();
	}
	vector<string> vecAccounts;
	for(vector<pair<string, string> >::iterator it = QContainer.begin();
		it != QContainer.end(); ++it)
	{
		vecAccounts.push_back((*it).first);
	}
	return vecAccounts;
}

string QQAccountBuilder::getPath()
{
	if (!qpath.empty())
	{
		return qpath;
	}
	vector<string> vec_path;
	getQQpath(vec_path);
	if (!vec_path.empty())
	{
		this->qpath = *(vec_path.begin());
		dirty = true;
		return this->qpath;// TEXT("C:\\Program Files (x86)\\Tencent\\TM2008\\Bin\\TM.exe");
	}
	return "";
}

void find(LPCTSTR szPath, vector<string> & vec_path)
{
	TCHAR szFind[MAX_PATH]={0};
	WIN32_FIND_DATA FindFileData;
	TCHAR lpPath[MAX_PATH]={0};
	_tcscpy_s(lpPath,szPath);
	if (lpPath[_tcslen(lpPath)-1]!=TEXT('\\'))
	{
		_tcscat_s(lpPath, TEXT("\\"));
	}
	_tcscat_s(szFind,lpPath);
	_tcscat_s(szFind,L"*.*");
	HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)
		return ;
	while(TRUE)
	{
		if(FindFileData.dwFileAttributes
			&FILE_ATTRIBUTE_DIRECTORY)
		{
			if(FindFileData.cFileName[0]!='.')
			{
				_tcscpy_s(szFind, lpPath);
				_tcscat_s(szFind, FindFileData.cFileName);
				if (_tcsstr(szFind,TEXT("$Recycle.Bin")) == NULL)
				{
					find(szFind, vec_path);
				}
			}
		}
		else
		{
			if ( _tcsstr(FindFileData.cFileName,TEXT(".exe")) != NULL )
			{
				TCHAR filename[MAX_PATH] = {9};
				_tcscpy_s(filename, lpPath);
				_tcscat_s(filename, FindFileData.cFileName);
				if (_tcsstr(filename, TEXT("Tencent")) && (_tcsstr(filename, TEXT("QQ.exe")) || _tcsstr(filename, TEXT("TM.exe"))))
				{
#if defined(UNICODE)|| defined(_UNICODE)
					int wslen = wcslen(filename);
					int len= WideCharToMultiByte(CP_ACP,0,filename,wslen,NULL,0,NULL,NULL);  
					char* m_char=new char[len+1];  
					WideCharToMultiByte(CP_ACP,0,filename,wslen,m_char,len,NULL,NULL);  
					m_char[len]='\0'; 
					std::string rStr = std::string(m_char);
					delete[] m_char;
					vec_path.push_back(rStr);
#else
					vec_path.push_back(string(filename));
#endif
				}
			}
		}
		if(!FindNextFile(hFind,&FindFileData))
			break;
	}
	FindClose(hFind);
}

void QQAccountBuilder::getQQpath(vector<string> & vec_path)
{
	TCHAR Drive[MAX_PATH] = {0};
	int i = 0;
	GetLogicalDriveStrings(MAX_PATH,Drive);
	TCHAR logic[MAX_PATH] = {0};
	LPTSTR p = Drive;
	while (_tcslen(p)!=0)
	{
		_tcscpy_s(logic, p);
		find(logic, vec_path);
		p += _tcslen(p) + 1;
	}
}

void QQAccountBuilder::setPath(const char* qpath)
{
	this->qpath = string(qpath);
	dirty = true;
}
