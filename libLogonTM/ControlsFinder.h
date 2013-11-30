#include <windows.h>
#include <stdlib.h>        //CHAR转换为WCHAR需要用到此文件  
#include <locale.h>  //CHAR转换为WCHAR时，要本地化，否则转换中文字符乱码  
#include <vector>
#include <tchar.h>
#include <iostream>
#include <Oleacc.h>
#pragma comment(lib,"Oleacc.lib")
#include <process.h>
using std::vector;
using std::string;
using std::pair;

#define CHROME_WINDOW_CLASS_NAME TEXT("Chrome_WidgetWin_1")
#define MAX_LEN_OF_QQ 16

class BrowserData{
public:
	BrowserData(LPCTSTR mainClsName = TEXT("") ,LPCTSTR wndName = TEXT(""),LPCTSTR roleName = TEXT(""), LPCTSTR clsName = TEXT(""))
	{
		m_MainClsName = mainClsName;
		m_WndName = wndName;
		m_RoleName = roleName;
		m_ClsName = clsName;

		m_MainClsName = (NULL == m_MainClsName) ? TEXT("") : m_MainClsName;
		m_WndName = (NULL == m_WndName) ? TEXT("") : m_WndName;
		m_RoleName = (NULL == m_RoleName) ? TEXT("") : m_RoleName;
		m_ClsName = (NULL == m_ClsName || TEXT("") == m_ClsName) ? m_MainClsName : m_ClsName;
		m_hMainWnd = NULL;
		m_hAddressWnd = NULL;
		m_ptCenterAddress.x = 0;
		m_ptCenterAddress.y = 0;
	}

	~BrowserData(){}

	LPCTSTR m_MainClsName;	//浏览器主窗口类名
	LPCTSTR m_ClsName;		//地址栏所在窗口类名
	LPCTSTR m_WndName;		//地址栏对象名称
	LPCTSTR m_RoleName;		//地址栏role name
	HWND m_hMainWnd;			//浏览器主窗口句柄
	HWND m_hAddressWnd;		//地址栏所在窗口的句柄
	POINT m_ptCenterAddress;	//地址栏中心位置（屏幕坐标）

	void GetBrowserMainWnd();
	void GetAddressCenter();
private:
	void GetAddressHwnd();
};

enum LaunchType{
	QQ,
	TM
};

typedef struct tagPanelNames{
	LPCTSTR mainCls,mainWndName,
		accountCls,accountWndName,
		pwdCls, pwdWndName;
} PanelNames;

class QQAccountData{
public:
	string account;
	string pwd;
	HWND h_MainHwnd;
	long persistTime;
	LaunchType ltype;
	PanelNames panels;
	QQAccountData(string account, string pwd, long time = 0, LaunchType ltype = TM)
		:account(account),pwd(pwd),
		persistTime(time),ltype(ltype),
		h_MainHwnd(NULL)
	{
		switch(ltype)
		{//尚未实现用QQ登录
		case QQ:
		case TM:
		default:
			panels.mainCls = TEXT("TXGuiFoundation");
			panels.accountCls = TEXT("ATL:30A4D1D8");
			panels.pwdCls = TEXT("Edit");
			panels.mainWndName = TEXT("TM2009");
			panels.accountWndName = NULL;
			panels.pwdWndName = TEXT("R");
			break;
		}
	}
	~QQAccountData(){}
};

namespace WindowElementFinder{
	typedef struct tagEnumWindowStruct{
		std::vector<HWND> hwnd_vec;
		LPCTSTR wndClass;
		LPCTSTR wndName;
	} EnumWndStruct;
	
	typedef struct tagEnumWindowPidStruct{
		DWORD pid;
		std::vector<HWND> hwnd_vec;
	} EnumWndPidStruct;

	BOOL CALLBACK  EnumWindowProc(HWND hwnd, LPARAM lParam)
	{
		EnumWndStruct *  struc = (EnumWndStruct *)lParam;
		TCHAR ClassName[MAX_PATH];
		GetClassName(hwnd, ClassName, MAX_PATH);
		TCHAR WndName[MAX_PATH];
		GetWindowText(hwnd, WndName, MAX_PATH);
		if (NULL == struc->wndClass && NULL == struc->wndName)
		{
			return FALSE;
		}
		if (NULL == struc->wndClass || _tcscmp(struc->wndClass, TEXT("")) == 0 )
		{
			if(_tcscmp(WndName, struc->wndName) == 0)
			{
				struc->hwnd_vec.push_back(hwnd);
			}
			return TRUE;
		}
		else if (NULL == struc->wndName || _tcscmp(struc->wndName, TEXT("")) == 0 )
		{
			if( _tcscmp(ClassName, struc->wndClass) == 0)
			{
				struc->hwnd_vec.push_back(hwnd);
			}
			return TRUE;
		}
		else if (_tcscmp(ClassName, struc->wndClass) == 0 && _tcscmp(WndName, struc->wndName) == 0)
		{
			struc->hwnd_vec.push_back(hwnd);
		}
		return TRUE;
	}

	BOOL CALLBACK  EnumWindowProcPid(HWND hwnd, LPARAM lParam)
	{
		EnumWndPidStruct *  pidStruct = (EnumWndPidStruct *)lParam;
		DWORD wndPid;
		GetWindowThreadProcessId(hwnd, &wndPid);
		if (wndPid == pidStruct->pid)
		{
			TCHAR ClassName[MAX_PATH];
			GetClassName(hwnd, ClassName, MAX_PATH);
			if (  _tcscmp(ClassName, TEXT("TXGuiFoundation")) == 0 && FindWindowEx(hwnd,NULL,NULL,NULL) != NULL )
			{
				pidStruct->hwnd_vec.push_back(hwnd);
				return TRUE;
			}
		}
		return TRUE;
	}

	//根据窗口名或类名，查找顶级窗口
	bool FindMainWnds(std::vector<HWND> & h_vec, LPCTSTR clsName, LPCTSTR wndName = NULL)
	{
		h_vec.clear();
		std::vector<HWND> wnds;
		EnumWndStruct wndStruct;
		wndStruct.hwnd_vec = wnds;
		wndStruct.wndClass = clsName;
		wndStruct.wndName = wndName;
		EnumWindows(EnumWindowProc,(LPARAM)(&wndStruct));
		wnds = wndStruct.hwnd_vec;
		if (wnds.empty())
		{
			return false;
		}

		for (std::vector<HWND>::iterator it = wnds.begin(); it != wnds.end(); ++it)
		{
			if (IsWindowVisible(*it))//FindWindowEx(*it,NULL,NULL,NULL) != NULL
			{
				h_vec.push_back(*it);
			}
		}
		if (h_vec.empty())
		{
			return false;
		}
		return true;
	}

	//根据窗口名或类名，查找某顶级窗口的子窗口控件
	void FindSubWindows(HWND hParent, std::vector<HWND> & h_vec, LPCTSTR clsName, LPCTSTR wndName = NULL)
	{
		h_vec.clear();
		std::vector<HWND> wnds;
		EnumWndStruct wndStruct;
		wndStruct.hwnd_vec = wnds;
		wndStruct.wndClass = clsName;
		wndStruct.wndName = wndName;
		EnumChildWindows(hParent, EnumWindowProc, (LPARAM)(&wndStruct));
		h_vec = wndStruct.hwnd_vec;
	}

	// 获取进行修改的浏览器主窗口，若有多个窗口，则寻找活动窗口
	// 有的浏览器如：搜狗、世界之窗。浏览器主窗口与内容窗口没有父子关系，
	// 浏览器在最顶层时获取到的活动窗口是内容窗口不是主窗口。这里尚需改进
	HWND GetHwndToModify(std::vector<HWND> h_vec)
	{
		if (h_vec.empty())
		{
			return NULL;
		}

		HWND frontWnd = GetForegroundWindow();
		if (h_vec.size() == 1)
		{
			return *(h_vec.begin());
		}

		for (std::vector<HWND>::iterator it = h_vec.begin(); it != h_vec.end(); ++it)
		{
			if (*it == frontWnd)
			{
				return *it;
			}
		}
		return NULL;
	}

	// UI元素的状态也表示成整型形式。因为一个状态可以有多个值，
	//例如可选的、可做焦点的，该整数是反映这些值的位的或操作结果。
	//将这些或数转换成相应的用逗号分割的状态字符串。
	UINT GetObjectState(IAccessible* pacc, 
		VARIANT* pvarChild, 
		LPTSTR lpszState, 
		UINT cchState)
	{
		HRESULT hr;
		VARIANT varRetVal;

		*lpszState = 0;

		VariantInit(&varRetVal);

		hr = pacc->get_accState(*pvarChild, &varRetVal);

		if (!SUCCEEDED(hr))
			return(0);

		DWORD dwStateBit;
		int cChars = 0;
		if (varRetVal.vt == VT_I4)
		{
			// 根据返回的状态值生成以逗号连接的字符串。
			for (dwStateBit = STATE_SYSTEM_UNAVAILABLE; 
				dwStateBit < STATE_SYSTEM_ALERT_HIGH; 
				dwStateBit <<= 1)
			{
				if (varRetVal.lVal & dwStateBit)
				{
					cChars += GetStateText(dwStateBit, 
						lpszState + cChars, 
						cchState - cChars);
					*(lpszState + cChars++) = ',';
				}
			}
			if(cChars > 1)
				*(lpszState + cChars - 1) = '\0';
		}
		else if (varRetVal.vt == VT_BSTR)
		{
			WideCharToMultiByte(CP_ACP, 
				0, 
				varRetVal.bstrVal, 
				-1, 
				(LPSTR)lpszState,
				cchState, 
				NULL, 
				NULL);
		}

		VariantClear(&varRetVal);

		return(lstrlen(lpszState));
	}

	//通过get_accName得到Name
	UINT GetObjectName(IAccessible* pacc, VARIANT* pvarChild, LPTSTR lpszName, UINT cchName)
	{
		HRESULT hr;
		BSTR bstrName;

		*lpszName = 0;
		bstrName = NULL;

		hr = pacc->get_accName(*pvarChild, &bstrName);

		if (SUCCEEDED(hr) && bstrName)
		{
#if defined(UNICODE) || defined (_UNICODE)
			UINT len = ::SysStringLen(bstrName);
			_tcsncpy(lpszName, bstrName, len);
			lpszName[len] = L'\0';
#else
			WideCharToMultiByte(CP_ACP, 0, bstrName, -1, lpszName, cchName, NULL, NULL);
#endif
			SysFreeString(bstrName);
		}

		return(lstrlen(lpszName));
	}

	//通过get_accRole得到Role
	UINT GetObjectRole(IAccessible* pacc, VARIANT* pvarChild, LPTSTR lpszRole, UINT cchRole)
	{
		HRESULT hr;
		VARIANT varRetVal;

		*lpszRole = 0;

		VariantInit(&varRetVal);

		hr = pacc->get_accRole(*pvarChild, &varRetVal);

		if (!SUCCEEDED(hr))
			return(0);

		if (varRetVal.vt == VT_I4)
		{
			//函数GetRoleText用来将整型的角色表示转换为字符串表示
			GetRoleText(varRetVal.lVal, lpszRole, cchRole);
		}
		else if (varRetVal.vt == VT_BSTR)
		{//将BSTR转换为lpszRole的类型（LPSTR）。
#if defined(UNICODE) || defined (_UNICODE)
			//将BSTR转换为LPWSTR。
			UINT len = ::SysStringLen(varRetVal.bstrVal);
			_tcsncpy(lpszRole, varRetVal.bstrVal, len);
 #else
			//将BSTR转换为LPSTR。
			WideCharToMultiByte(CP_ACP, 0, varRetVal.bstrVal, -1, lpszRole,
				cchRole, NULL, NULL);
 #endif
			//SysFreeString(varRetVal.bstrVal);
		}

		VariantClear(&varRetVal);

		return(lstrlen(lpszRole));
	}

	//通过WindowFromAccessibleObject和GetClassName得到Class
	UINT GetObjectClass(IAccessible* pacc, LPTSTR lpszClass, UINT cchClass)
	{
		HWND hWnd;
		if(S_OK == WindowFromAccessibleObject(pacc,  &hWnd))
		{
			if(hWnd)
				GetClassName(hWnd, lpszClass, cchClass);
			else
				_tcscpy(lpszClass, TEXT("No window"));
		}

		return 1;
	}
	
	BOOL FindChild (IAccessible* paccParent, 
		LPCTSTR szName, LPCTSTR szRole, 
		LPCTSTR szClass, 
		IAccessible** paccChild, 
		VARIANT* pvarChild)
	{
		HRESULT hr;
		long numChildren;
		unsigned long numFetched;
		VARIANT varChild;
		int index;
		IAccessible* pCAcc = NULL;
		IEnumVARIANT* pEnum = NULL;
		IDispatch* pDisp = NULL;
		BOOL found = false;
		TCHAR szObjName[256], szObjRole[256], szObjClass[256], szObjState[256];

		//得到父亲支持的IEnumVARIANT接口
		hr = paccParent -> QueryInterface(IID_IEnumVARIANT, (PVOID*) & pEnum);

		if(pEnum)
			pEnum -> Reset(); //Resets the enumeration sequence to the beginning.

		//取得父亲拥有的可访问的子的数目
		paccParent -> get_accChildCount(&numChildren);

		//搜索并比较每一个子ID，找到名字、角色、类与输入相一致的。
		for(index = 1; index <= numChildren && !found; index++)
		{
			pCAcc = NULL;		
			// 如果支持IEnumVARIANT接口，得到下一个子ID
			// 以及其对应的 IDispatch 接口
			if (pEnum)
			{
				hr = pEnum -> Next(1, &varChild, &numFetched);	
			}
			else
			{
				//如果一个父亲不支持IEnumVARIANT接口，子ID就是它的序号
				varChild.vt = VT_I4;
				varChild.lVal = index;
			}

			// 找到此子ID对应的 IDispatch 接口
			if (varChild.vt == VT_I4)
			{
				//通过子ID序号得到对应的 IDispatch 接口
				pDisp = NULL;
				hr = paccParent -> get_accChild(varChild, &pDisp);
			}
			else
			{
				//如果父支持IEnumVARIANT接口可以直接得到子IDispatch 接口
				pDisp = varChild.pdispVal;
			}

			// 通过 IDispatch 接口得到子的 IAccessible 接口 pCAcc
			if (pDisp)
			{
				hr = pDisp->QueryInterface(IID_IAccessible, (void**)&pCAcc);
				hr = pDisp->Release();
			}

			long numChilds = 0;
			// Get information about the child
			if(pCAcc)
			{
				//如果子支持IAccessible 接口，那么子ID就是CHILDID_SELF
				VariantInit(&varChild);
				varChild.vt = VT_I4;
				varChild.lVal = CHILDID_SELF;

				*paccChild = pCAcc;
				pCAcc-> get_accChildCount(&numChilds);
			}
			else
			{
				//如果子不支持IAccessible 接口
				*paccChild = paccParent;
				numChilds = 0;
				//			continue;
			}

			//跳过了有不可访问状态的元素
			GetObjectState(*paccChild, 
				&varChild, 
				(LPTSTR)szObjState, 
				sizeof(szObjState));
			if(NULL != _tcsstr(szObjState, TEXT("unavailable")))
			{
				if(pCAcc)
					pCAcc->Release();
				continue;
			}
			//通过get_accName得到Name
			GetObjectName(*paccChild, &varChild, szObjName, sizeof(szObjName));
			//通过get_accRole得到Role
			GetObjectRole(*paccChild, &varChild, szObjRole, sizeof(szObjRole));
			//通过WindowFromAccessibleObject和GetClassName得到Class
			GetObjectClass(*paccChild, szObjClass, sizeof(szObjClass));

//			std::cout<<index<<": name="<<szObjName<<"  role="<<szObjRole<<" has "<<numChilds<<" child"<<std::endl;
			//如果这些参数与输入相符或输入为NULL
			if ((!szName || 
				!_tcscmp(szName, szObjName)) && 
				(!szRole || 
				!_tcscmp(szRole, szObjRole)) && 
				(!szClass || 
				!_tcscmp(szClass, szObjClass)))
			{
				found = true;
				*pvarChild = varChild;
				break;
			}
			if(!found && pCAcc)
			{
				// 以这次得到的子接口为父递归调用
				found = FindChild(pCAcc, 
					szName, 
					szRole, 
					szClass, 
					paccChild, 
					pvarChild);
				if(*paccChild != pCAcc)
					pCAcc->Release();
			}
		}//End for

		// Clean up
		if(pEnum)
			pEnum -> Release();

		return found;
	}

	void FindAccObjLocWithNameOrClass(HWND parent,LPCTSTR clsName, LPCTSTR wndName, LPCTSTR szRole, RECT& rect)
	{
		if (NULL == parent)
		{
			return;
		}

// 		if (NULL == clsName && NULL == wndName)
// 		{
// 			return NULL;
// 		}
		// 			//通过窗口句柄得到窗口的 IAccessible 接口指针。
		IAccessible *paccMainWindow = NULL;
		HRESULT hr;
		if(S_OK == (hr = AccessibleObjectFromWindow(parent, 
			OBJID_WINDOW, 
			IID_IAccessible,
			(void**)&paccMainWindow)))
		{
			IAccessible *paccObj = NULL;
			VARIANT var;
			bool isFind = FindChild(paccMainWindow, wndName, szRole, clsName, &paccObj, &var);
			long pxleft=0,pxtop=0,pxwidth=0,pxheight=0;
			if (isFind)
			{
				VARIANT posVar;
				VariantInit(&posVar);
 				posVar.vt = VT_I4;
 				posVar.lVal = CHILDID_SELF;
				paccObj->accLocation(&pxleft, &pxtop, &pxwidth, &pxheight, posVar);
//				std::cout<<pxleft<<" "<<pxwidth<<" "<<pxtop<<" "<<pxheight<<std::endl;
				paccMainWindow->Release();
			}
			rect.left = pxleft;
			rect.top = pxtop;
			rect.right = pxwidth;
			rect.bottom = pxheight;
			std::cout<<__FUNCTION__<<" "<<rect.left<<" "<<rect.top<<" "<<rect.top<<" "<<rect.bottom<<std::endl;
		}
	}

	void SendClick(long x = 0, long y = 0)
	{
		POINT point;
		GetCursorPos(&point);
		
		double fScreenWidth    = ::GetSystemMetrics( SM_CXSCREEN )-1; 
		double fScreenHeight  = ::GetSystemMetrics( SM_CYSCREEN )-1; 
		double fx = x * (65535.0f/fScreenWidth);
		double fy = y * (65535.0f/fScreenHeight);

		INPUT input = {0};
		input.type = INPUT_MOUSE;
		BOOL bMouseBtnChanged = SwapMouseButton(false);
		if (x == 0 && y == 0)
		{// 默认在鼠标当前位置单击
			fx = point.x * (65535.0f/fScreenWidth);
			fy = point.y * (65535.0f/fScreenHeight);

			input.mi.dx = fx;
			input.mi.dy = fy;
			input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			SendInput(1, &input, sizeof(input));
			input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
			SendInput(1, &input, sizeof(input));
			Sleep(100);

			return;
		}

		input.mi.dx = fx;
		input.mi.dy = fy;
		input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_MOVE;
		SendInput(1, &input, sizeof(input));
		Sleep(100);

		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &input, sizeof(input));
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput(1, &input, sizeof(input));
		Sleep(100);
		if (bMouseBtnChanged)
		{
			SwapMouseButton(true);
		}
		SetCursorPos(point.x, point.y);
	}

	void HideCursor(bool bHide = true)
	{
		if (bHide)
		{
			ShowCursor(false);
		}
		CONSOLE_CURSOR_INFO cursor_info = {1, 0};
		if (!bHide)
		{
			cursor_info.bVisible = TRUE;
		}
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
		if (!bHide)
		{
			ShowCursor(true);
		}
	}

	void SendCtrlChar(char ch)
	{
		INPUT kinput[3] = {0};
		kinput[0].type = INPUT_KEYBOARD;
		kinput[0].ki.wVk = VK_CONTROL;

		kinput[1].type = INPUT_KEYBOARD;
		kinput[1].ki.wVk = ch;

		kinput[2].type = INPUT_KEYBOARD;
		kinput[2].ki.wVk = VK_CONTROL;
		kinput[2].ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(3, kinput, sizeof(INPUT));
		//Sleep(10);
	}

	void SendVirtualKey(DWORD vkey)
	{
		INPUT vkeyinput[2] = {0};
		vkeyinput[0].type = INPUT_KEYBOARD;
		vkeyinput[0].ki.wVk = vkey;
		vkeyinput[1].type = INPUT_KEYBOARD;
		vkeyinput[1].ki.wVk = vkey;
		vkeyinput[1].ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(2, vkeyinput, sizeof(INPUT));
		Sleep(200);
	}

	void InputNoRepeatString(std::wstring url)
	{
		if (url.empty())
		{
			return;
		}
		size_t nCount = 0;
		std::vector<INPUT>  vInput;

// 		//准备好要填充的分隔按键KEYBDINPUT结构  
// 		KEYBDINPUT KEYdivision[4] = {0};  
// 		KEYdivision[0].wVk = VK_LEFT;  
// 		KEYdivision[1].wVk = VK_LEFT;  
// 		KEYdivision[1].dwFlags = KEYEVENTF_KEYUP;  
// 		KEYdivision[2].wVk = VK_RIGHT;  
// 		KEYdivision[3].wVk = VK_RIGHT;  
// 		KEYdivision[3].dwFlags = KEYEVENTF_KEYUP;  

		for (size_t i = 0; i < url.length(); ++i)
		{
			INPUT input = {0};
// 			if (i > 0 && url[i] == url[i-1])
// 			{
// 				for (size_t j = 0; j < 4; ++j)
// 				{
// 					input.type = INPUT_KEYBOARD;
// 					input.ki = KEYdivision[j];
// 					vInput.push_back(input);
// 				}
// 			}
			input.type = INPUT_KEYBOARD;
			input.ki.dwFlags = KEYEVENTF_UNICODE;
			input.ki.wScan = (WORD)url[i];
			vInput.push_back(input);
		}
		size_t len = vInput.size();
		INPUT* pInput = new INPUT[len];
		memset(pInput, 0, len * sizeof(INPUT)); 
		std::vector<INPUT>::iterator it = vInput.begin();
		for (size_t idx = 0; idx < len; ++it,++idx)
		{
			pInput[idx].type = (*it).type;
			pInput[idx].ki.wScan = (*it).ki.wScan;
			pInput[idx].ki.wVk = (*it).ki.wVk;
			pInput[idx].ki.dwFlags = (*it).ki.dwFlags;
		}

		SendInput(len, pInput, sizeof(INPUT));

		if (pInput != NULL)
		{
			delete[] pInput;
			pInput = NULL;
		}
	}

	void InputString(std::wstring url)
	{
		std::vector<std::wstring> subVec;
		size_t posflag = 0;
		size_t i = 0;
		for ( ; i < url.length(); ++i)
		{
			if (i > 0 && url[i] == url[i-1])
			{
				subVec.push_back(url.substr(posflag, i - posflag));
				posflag = i;
			}
		}
		subVec.push_back(url.substr(posflag, i - posflag));
		for (std::vector<std::wstring>::iterator it = subVec.begin(); it != subVec.end(); ++it)
		{
			if(*((*it).begin()) < L'9' && *((*it).begin()) > L'0')
			{
				Sleep(500);
			}
			else
			{
				Sleep(60);
			}
			InputNoRepeatString(*it);
		}
	}

	void InputString(std::string url)
	{
		//CHAR型字符串转换为wstring  
		std::string curLocale = setlocale(LC_ALL, NULL);  
		setlocale(LC_ALL, "chs");   
		size_t _Dsize = strlen(url.c_str()) + 1;  
		WCHAR *wszText = new WCHAR[_Dsize];  
		wmemset(wszText,0,_Dsize);  
		size_t nValue;  
		mbstowcs_s(&nValue,wszText,_Dsize,url.c_str(),_TRUNCATE);  
		std::wstring wstrText = wszText;  
		delete [] wszText;  
		setlocale(LC_ALL, curLocale.c_str());
		InputString(wstrText);
	}

	void lowerChar(char mch)
	{
		char ch = (char)((int)mch - (int)'a' + (int)'A');
		bool bCapsLock = (GetKeyState(VK_CAPITAL) & 0xffff) != 0;
		if (bCapsLock)
		{
			keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
			keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			Sleep(10);
			keybd_event((byte)ch, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
			keybd_event((byte)ch, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			Sleep(10);
			keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
			keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			Sleep(10);
		}
		else
		{
			keybd_event((byte)ch, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
			keybd_event((byte)ch, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		}
	}

	void upperChar(char ch)
	{
		bool bCapsLock = ((GetKeyState(VK_CAPITAL)) & 0xffff) != 0;
		if (!bCapsLock)
		{
			keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
			keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			Sleep(10);
			keybd_event((byte)ch, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
			keybd_event((byte)ch, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			Sleep(10);
			keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
			keybd_event(VK_CAPITAL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			Sleep(10);
		}
		else
		{
			keybd_event((byte)ch, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
			keybd_event((byte)ch, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			Sleep(10);
		}
	}

	void InputPwdString(std::string url)
	{
// 		std::string alpha;
// 		alpha.clear();
		for (std::string::iterator it = url.begin(); it != url.end(); ++it)
		{
			if (*it >= 'a' && *it <= 'z')
			{
				lowerChar(*it);
			}
			else if (*it >= 'A' && *it <= 'Z')
			{
				upperChar(*it);
			}
			else if (*it >= '0' && *it <= '9')
			{
				keybd_event((byte)(*it), 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
				keybd_event((byte)(*it), 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
				Sleep(10);
			}
			else
			{//其他字符
				InputString(string(&(*it), 1));
				Sleep(10);
			}
// 			if (*it <= '9' && *it >= '0')
// 			{
// 				if (!alpha.empty())
// 				{
// 					InputString(alpha);
// 					alpha.clear();
// 				}
// 				SendVirtualKey(*it);
// 			}
// 			else
// 			{
// 				alpha += *it;
// 			}
		}
// 		if (!alpha.empty())
// 		{
// 			InputString(alpha);
// 			alpha.clear();
// 		}
	}

	void SetChromeAdressBlank()
	{
		std::vector<HWND> wnds;
		FindMainWnds(wnds, CHROME_WINDOW_CLASS_NAME);
		HWND hBrowMain = * wnds.begin();

		RECT rect;
		GetWindowRect(hBrowMain, &rect);

		POINT pt;
		pt.x = rect.left + 250;
		pt.y = IsZoomed(hBrowMain) ? (rect.top + 40) : (rect.top + 60);	      //全屏时+40就够了
		HideCursor();
		SendClick(pt.x, pt.y);   // 发送点击事件，点击的位置获取焦点
		SendCtrlChar('A');       // ctrl+'A'
		SendVirtualKey(VK_SPACE); // 空格
		SendClick();
		HideCursor(false);
	}

	void ReplaceChromeAdressBar(std::string text)
	{
		std::vector<HWND> wnds;
		FindMainWnds(wnds, CHROME_WINDOW_CLASS_NAME);
		HWND hBrowMain = * wnds.begin();

		RECT rect;
		GetWindowRect(hBrowMain, &rect);

		POINT pt;
		pt.x = rect.left + 250;
		pt.y = IsZoomed(hBrowMain) ? (rect.top + 40) : (rect.top + 60);	      //全屏时+40就够了
		HideCursor();
		SendClick(pt.x, pt.y);   // 发送点击事件，点击的位置获取焦点
		SendCtrlChar('A');       // ctrl+'A'
		InputString(text);
		SendClick();
		HideCursor(false);
	}

	void PushTextsToClipboard(std::string text)
	{
		if(OpenClipboard(NULL))
		{
			HGLOBAL clipbuffer;
			char * buffer;
			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, text.length()+1);
			buffer = (char*)GlobalLock(clipbuffer);
			strcpy_s(buffer,text.length() + 1 , text.c_str());
			GlobalUnlock(clipbuffer);
			SetClipboardData(CF_TEXT,clipbuffer);
			CloseClipboard();
		}
	}

	void PasteToAddressBar(BrowserData* browData, std::string text = "")
	{
		if (NULL == browData)
		{
			return;
		}

		if (browData->m_ptCenterAddress.x == 0 || browData->m_ptCenterAddress.y == 0)
		{
			browData->GetAddressCenter();
		}

		RECT rect;
		GetWindowRect(browData->m_hMainWnd, &rect);

		if (!text.empty())
		{
			PushTextsToClipboard(text);
		}
	
		HideCursor();
//		SendClick(pt.x, pt.y);   // 发送点击事件，点击的位置获取焦点
		SendClick(browData->m_ptCenterAddress.x, browData->m_ptCenterAddress.y);
		SendCtrlChar('A');       // ctrl+'A'
		SendCtrlChar('V');
		//SendClick();
		Sleep(3);

		// 使在地址栏输入内容时显示的下拉框消失，一般的只需要movewindow即可，世界之窗需要点击一下标题栏。
		MoveWindow(browData->m_hMainWnd, rect.left - 1, rect.top - 1, rect.right - rect.left, rect.bottom - rect.top, true);
		MoveWindow(browData->m_hMainWnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);
		SendClick(browData->m_ptCenterAddress.x, rect.top + 3);
		HideCursor(false);
// 		SetWindowLong(hBrowMain, GWL_STYLE, wndlong);
// 		SetWindowPos(hBrowMain, HWND_BOTTOM,  0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
// 		SendMessage(hBrowMain, WM_SYSCOMMAND, SC_MINIMIZE, 0);
// 		if (isMinimum)
// 		{
// 			SendMessage(hBrowMain, WM_SYSCOMMAND, SC_MINIMIZE, 0);
// 		}
//  		SetForegroundWindow(topWnd);
 //		SetWindowPos(hBrowMain, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
//		SetActiveWindow(hBrowMain);
	}

	DWORD run_execute(LPCTSTR app, LPCTSTR cmd)//std::string path,
	{
		//一些必备参数设置   
		STARTUPINFO si;
		memset(&si,0,sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOW;
		PROCESS_INFORMATION pi;
		//必备参数设置结束   
		TCHAR wcmd[MAX_PATH] = {0};
		_tcscpy_s(wcmd, MAX_PATH, cmd);
		TCHAR wapp[MAX_PATH] = {0};
		_tcscpy_s(wapp, MAX_PATH, app);
		if(!CreateProcess(
			wapp, wcmd, 
			NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))  
		{
			return 0;
		}
		// 不使用的句柄最好关掉   
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return pi.dwProcessId;
	}


	HWND GetWindowHandleByPID(DWORD dwProcessID)
	{
		EnumWndPidStruct pstruct={0};
		pstruct.pid = dwProcessID;
		EnumWindows(EnumWindowProcPid, (LPARAM)&pstruct);
// 		for (std::vector<HWND>::iterator it = pstruct.hwnd_vec.begin(); it != pstruct.hwnd_vec.end(); ++it)
// 		{
// 			std::cout<<*it<<std::endl;
// 		}
		return pstruct.hwnd_vec.empty() ? NULL : *(pstruct.hwnd_vec.begin());
	}

	void FindLogonBtnAndClick(HWND hMain)
	{
		SetForegroundWindow(hMain);
		WindowElementFinder::SendVirtualKey(VK_RETURN);
		RECT mainRect;
		GetWindowRect(hMain,&mainRect);
		WindowElementFinder::SendClick(mainRect.right - 80, mainRect.top+5);
	}

	void startupQQClient(LPCTSTR szPath, QQAccountData* data, LaunchType ltype = TM)
	{
		DWORD pid = WindowElementFinder::run_execute(szPath,TEXT(""));
		HWND tmMain = NULL;
		while(tmMain == NULL)
		{
			tmMain = WindowElementFinder::GetWindowHandleByPID(pid);
			Sleep(100);
		}
		data->h_MainHwnd = tmMain;
	}

	void Logon(LPCTSTR szPath, string qqAccout, string qqPwd )
	{
		if (szPath == NULL || _tcscmp(szPath,TEXT("")) == 0)
		{
			return;
		}
		QQAccountData qqData(qqAccout, qqPwd);
		startupQQClient(szPath, &qqData, TM);
		vector<HWND> h_TMs;
		while(h_TMs.empty())
		{
			WindowElementFinder::FindSubWindows(qqData.h_MainHwnd,h_TMs,TEXT("ATL:30A4D1D8"));
			Sleep(100);
		}
		HWND tmAccountWnd = *h_TMs.begin();
		WindowElementFinder::FindSubWindows(qqData.h_MainHwnd,h_TMs,TEXT("Edit"));
		HWND tmPwdhWnd = *h_TMs.begin();

		RECT accountRect;
		GetWindowRect(tmAccountWnd,&accountRect);
		SetForegroundWindow(qqData.h_MainHwnd);
		SetFocus(qqData.h_MainHwnd);
		WindowElementFinder::SendClick(accountRect.left/2+accountRect.right/2,accountRect.top/2+accountRect.bottom/2);
		WindowElementFinder::SendCtrlChar('A');
		WindowElementFinder::InputPwdString(qqAccout);

		RECT pwdRect;
		GetWindowRect(tmPwdhWnd,&pwdRect);
		SetForegroundWindow(qqData.h_MainHwnd);
		WindowElementFinder::SendClick(pwdRect.right-10,pwdRect.top/2+pwdRect.bottom/2);
		for (int i = 0; i<MAX_LEN_OF_QQ; ++i)
		{
			WindowElementFinder::SendVirtualKey(VK_BACK);
		}
		SetForegroundWindow(qqData.h_MainHwnd);
		WindowElementFinder::InputPwdString(qqPwd);

		FindLogonBtnAndClick(qqData.h_MainHwnd);
		//	WindowElementFinder::SendClick(accountRect.left/2 + accountRect.right/2,accountRect.top/2 + accountRect.bottom/2+190);
		// 	Sleep(1000);
		// 	RECT mainRect;
		// 	GetWindowRect(tmMain,&mainRect);
		// 	WindowElementFinder::SendClick(mainRect.right - 80, mainRect.top+5);
	}
}

void BrowserData::GetBrowserMainWnd()
{
	std::vector<HWND> wnds;

	WindowElementFinder::FindMainWnds(wnds, m_MainClsName);

	if (wnds.empty())
	{
		m_hMainWnd = NULL;
		return;
	}
#ifdef ADDRESS_TEST
	m_hMainWnd = *(wnds.begin());
#else
//	m_hMainWnd = *(wnds.begin());
	m_hMainWnd = WindowElementFinder::GetHwndToModify(wnds);
#endif
	// 窗口必须是最前，否则为NULL，扩展中用
}

void BrowserData::GetAddressHwnd()
{
	if (NULL == m_hMainWnd)
	{
		GetBrowserMainWnd();
	}
	if ( _tcscmp(m_MainClsName, m_ClsName) == 0)
	{
		m_hAddressWnd = m_hMainWnd;
		return;
	}
	std::vector<HWND> hControls;
	WindowElementFinder::FindSubWindows(m_hMainWnd, hControls, m_ClsName, m_WndName);
	if (hControls.empty())
	{
		m_hAddressWnd = NULL;
		return;
	}
	m_hAddressWnd = *(hControls.begin());
}

void BrowserData::GetAddressCenter()
{
	if (NULL == m_hAddressWnd)
	{
		GetAddressHwnd();
	}

	RECT adressRect = {0, 0, 0, 0};
	if (NULL == m_hAddressWnd)
	{
		return;
	}

	if (m_hAddressWnd == m_hMainWnd)
	{
		WindowElementFinder::FindAccObjLocWithNameOrClass(m_hAddressWnd, m_ClsName, m_WndName, m_RoleName, adressRect);
		m_ptCenterAddress.x = adressRect.left + adressRect.right / 2;
		m_ptCenterAddress.y = adressRect.top + adressRect.bottom / 2;
	}
	else
	{
		GetWindowRect(m_hAddressWnd, &adressRect);
		m_ptCenterAddress.x = adressRect.left / 2 + adressRect.right / 2;
		m_ptCenterAddress.y = adressRect.top / 2 + adressRect.bottom / 2;
	}
}
