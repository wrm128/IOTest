#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <string>
#include <stdexcept>
#include <time.h>

#include "BasicTools.h"

using namespace std;

/*
**	class Clog
*/
/*
Clog::Clog()
{
	m_filename = "pTopLog.txt";         //默认路径为当前目录下
	m_strLogInfo.push_back(GetTimeStr()+"\n");
}
Clog::~Clog()
{
	_Save();
}
//获取当前时间，格式如：Thu May 22 10:31:30 2014
string Clog::GetTimeStr()
{
	time_t rawtime;
	struct tm timeinfo;
	time( &rawtime );
	localtime_s(&timeinfo, &rawtime);
	char chtime[128];
	errno_t errNum = asctime_s(chtime, 128, &timeinfo);
	if (errNum)
	{
		printf("Error code: %d", (int)errNum);
		return "";
	 }
	chtime[strlen(chtime) - 1] = '\0';
	string strtime(chtime);
	return strtime;
}

//在Clog类外修改m_strLogInfo的窗口
void Clog::AppendInfo(string info)
{
	m_strLogInfo.push_back(info);
}
//将记录下的log信息输出，在析构函数中调用，可能不安全 CHECK luolan
void Clog::_Save()
{
	errno_t err;
	err = fopen_s( &m_pfile, m_filename.c_str(),"a+" );
	if (err != 0)
	{
		cout << "[Error] Failed to create log, "<< m_filename <<  " not exists!" <<endl;
	}
	for (int i = 0; i < (int)m_strLogInfo.size(); i++)
	{
		fprintf(m_pfile, "%s", m_strLogInfo[i].c_str());
	}
	fclose(m_pfile);
}
//按照一定的格式输出提示信息
//并将log信息都记录在字符串数组m_strLogInfo中
void Clog::PrintInfoTo(ostream &output, string infoType, string Info)
{
	string strtime = GetTimeStr();
	output << "[" << infoType << "] <" << strtime << "> " << Info << endl;
	m_strLogInfo.push_back(Info);
	m_strLogInfo.push_back("\n");
}
*/


/**
* COptionTool
**/

const int COptionTool::BUFFER_SIZE = 81920;

COptionTool::COptionTool(const string szFile)
		:m_strFile(szFile)
{
	
};

COptionTool::~COptionTool()
{

}

void COptionTool::_GetPrivateProfileString(const string & strSection, const char * strKey,const char * szDefault,
				char szValue[], int nBufSize)
{
	szValue[0] = 0;

	FILE *fp = fopen(m_strFile.c_str(), "r");
	if(!fp)
	{
		strcpy(szValue, szDefault);
		//CErrInfo info("COptionTool", "_GetPrivateProfileString", "cannot find the file.");
		throw runtime_error("COptionTool _GetPrivateProfileString cannot find the file." + m_strFile);
	}

	char szBuf[BUFFER_SIZE] = {0};
	size_t lenKey = strlen(strKey);
	bool bRange = false;
	if(strSection.length() == 0)
	{
		bRange = true;
	} else {
		string str("[");
		str.append(strSection);
		str.append("]");
		size_t lenApp = str.length();
		while(1)
		{
			if(0 == fgets(szBuf, BUFFER_SIZE - 1, fp)) // NULL，文件结束
			{
				break;
			}

			szBuf[lenApp] = 0;

			if(strcmp(str.c_str(), szBuf) == 0)
			{
				bRange = true;
				break;
			}
		}
	}
	if(bRange)
	{
		while(1)
		{
			if(0 == fgets(szBuf, BUFFER_SIZE - 1, fp)) // NULL
			{
				break;
			}
			size_t tCurrLen = strlen(szBuf) - 1;
            if(szBuf[lenKey] != '=')  // 此处若'='前多一个空格都不行
            {
                continue;
            }
			szBuf[lenKey] = 0;
						
			if(0 == strcmp(szBuf, strKey))
			{
				while(tCurrLen >= 0 && (szBuf[tCurrLen] == 0xa || szBuf[tCurrLen] == 0xd))
				{   // 0xa 换行，0xd回车
					szBuf[tCurrLen--] = 0;
				}
				strcpy(szValue, szBuf + lenKey + 1);
				fclose(fp);
				return;
			}
		}
		strcpy(szValue, szDefault);
		fclose(fp);
		return;
	} else {
		strcpy(szValue, szDefault);
		fclose(fp);
		return;
	}			
}



int COptionTool::_GetPrivateProfileInt(const string & strSection, const char * strKey, const int & nDefault)
{
	char szValue[BUFFER_SIZE] = {0};
	_GetPrivateProfileString(strSection, strKey, "", szValue, BUFFER_SIZE);
	if(0 == strlen(szValue))
	{
		return nDefault;
	} else {
		return atoi(szValue);
	}
}
		
string COptionTool::GetString(const char * strSection, const char * strKey, const char * szDefault)
{
	char szValue[BUFFER_SIZE] = {0};
	_GetPrivateProfileString(strSection, strKey, szDefault, szValue, BUFFER_SIZE);
	return string(szValue);
};

double COptionTool::GetDouble(const char * strSection, const char * strKey, const double lfDefault)
{
	char szValue[BUFFER_SIZE] = {0};
	_GetPrivateProfileString(strSection, strKey, "", szValue, BUFFER_SIZE);
	if(strlen(szValue) == 0)
		return lfDefault;
	else return atof(szValue);
}
		
int COptionTool::GetInteger(const char * strSection, const char * strKey, const int nDefault)
{
	return _GetPrivateProfileInt(strSection, strKey, nDefault);
}
		
size_t COptionTool::GetSizeT(const char * strSection, const char * strKey, const size_t tDefault)
{
	return (size_t)GetInteger(strSection, strKey, (int)tDefault);	
};

bool COptionTool::GetBool(const char * strSection, const char * strKey, const bool bDefault)
{
	char szValue[BUFFER_SIZE] = {0};
	_GetPrivateProfileString(strSection, strKey, "", szValue, BUFFER_SIZE);
	if(strcmp(szValue, "true") == 0 || strcmp(szValue, "TRUE") == 0)
	{
		return true;
	} else if(strcmp(szValue, "false") == 0 || strcmp(szValue, "FALSE") == 0) {
		return false;
	} else {
		return bDefault;
	}
};	




/**
*	CErrInfo
**/
CErrInfo::CErrInfo(const string &strClass, const string &strMethod,
		const string &strDetail) 
{
	m_strClass = strClass;
	m_strMethod = strMethod;
	m_strDetail = strDetail;
}

CErrInfo::CErrInfo(const string &strClass, const string &strMethod,
		const string &strDetail, const exception & e) 
{
	m_strClass = strClass;
	m_strMethod = strMethod;
	m_strDetail = strDetail;
	SetException(e);
}
void CErrInfo::Append(const string &strInfo) 
{
	if (strInfo.empty())
		return;
	else
	{
		m_strInfo += "\t\t  " + strInfo+"\n";
	}
}

string CErrInfo::Get() const 
{
	string strError = m_strException;
	strError += "\t  at " + m_strClass + "::" + m_strMethod + "() "
			+ m_strDetail + "\n";
	strError += m_strInfo;
	return strError;
}

string CErrInfo::Get(const exception& e) 
{
	SetException(e);
	return Get();
}
void CErrInfo::SetException(const exception & e) 
{
	m_strException = e.what();
}

ofstream& operator<< ( ofstream& os, const CErrInfo& info)
{
	os << endl << "==========================" << endl;
	time_t current_time;
	time(&current_time);
	os << ctime(&current_time) << endl;
	os << info.Get() << endl;
	return os;
}
ostream& operator<< ( ostream& os, const CErrInfo& info)
{
	os << endl << "==========================" << endl;
	time_t current_time;
	time(&current_time);
	os << ctime(&current_time) << endl;
	os << info.Get() << endl;
	return os;
}

/**
*	CStringProcess
**/

void CStringProcess::Trim(string& str) {
	int i;
	for (i = 0; i < (int)str.length() && bIsNoUse(str.at(i)); ++i)
		;
	if (i == (int)str.length()) {
		str.erase(0, i - 1);
		return;
	}
	str.erase(0, i);
	for (i = str.length() - 1; i >= 0 && bIsNoUse(str.at(i)); --i)
		;
	str.erase(i + 1, str.length() - 1 - i);
	return;
}

void CStringProcess::Trim(const char *&fptr, const char *&lptr) {
	while (fptr < lptr && bIsNoUse(*fptr))
		++fptr;
	while (fptr < lptr && bIsNoUse(*(lptr - 1)))
		--lptr;
	return;
}

bool CStringProcess::bIsNoUse(const char ch) {
	if (' ' == ch || '\r' == ch || '\t' == ch) {
		return true;
	}
	return false;
}

bool CStringProcess::bIsNumber(const char ch) {
	if ('0' <= ch && '9' >= ch) {
		return true;
	}
	return false;
}

void CStringProcess::ToLower(string& str) {
	for (size_t i = 1; i < str.size(); i++) {
		str[i] = tolower(str[i]);
	}
}

void CStringProcess::Split(const string& strFullString, const string& strSign,
		string& strPrior, string& strLatter) {
	size_t i = strFullString.find(strSign);
	if (i != string::npos) {
		strPrior = strFullString.substr(0, i);
		strLatter = strFullString.substr(i + strSign.length(),
				strFullString.length() - i - strSign.length());
	} else {
		strPrior = strFullString;
		strLatter.clear();
	}
}

void CStringProcess::Split(const string str, const string &delim, vector<string> &ret)
{
	size_t last = 0;  
    size_t index = str.find_first_of(delim, last);  
    while(index != string::npos)  
    {  
        ret.push_back(str.substr(last, index - last));  
		last = index + delim.length();  
        index = str.find_first_of(delim, last);  
    }  
	if (str.length() > last && index - last > 0)  
    {  
        ret.push_back(str.substr(last, index - last));  
    }  
}

bool CStringProcess::bMatchingFix(string strFullString, string strFix,
		bool bSuffix, bool bCaseSensitive) {
	size_t tLength = strFix.size();
	if (tLength >= strFullString.size()) {
		return false;
	}
	if (bSuffix) {
		strFullString.erase(strFullString.begin(), strFullString.end()
				- tLength);
	} else {
		strFullString.erase(strFullString.begin() + tLength,
				strFullString.end());
	}
	if (!bCaseSensitive) {
		CStringProcess::ToLower(strFullString);
		CStringProcess::ToLower(strFix);
	}
	return strFullString == strFix;
}

bool CStringProcess::isInSet(const string arr[], int n, string &key)
{
	for(int i = 0; i < n && arr[i] != ""; ++i)
	{
		if(arr[i].compare(key) == 0) return true;
	}
	return false;
}

// Generate the decoy sequence
void CStringProcess::Reverse(string &str)
{

/*
//---------Shuffle---------
   srand(time(NULL));
    size_t len = sequence.length();
    for(size_t i = 0; i < len; ++i)
    {
        int idx = rand() % len;
        swap(sequence[i], sequence[idx]);
    }
*/
	
//----------Reverse--------
 //   size_t len = sequence.length();
	//for(size_t i = 0; i < len / 2; ++i)
 //   {
 //       swap(sequence[i], sequence[len - 1 - i]);
 //   }
//------------------------


    //cout<<"target-"<<sequence<<endl;
    if(str.length() <= 1)
	{
        return;
	} else if(str.length() == 2) {
        swap(str[0], str[1]);
        return;
    }
    size_t len = str.length();
    size_t idx = len / 2;
    string subStr = str.substr(idx);
    str.erase(str.begin()+idx, str.end());
    for(size_t i = 0; i < idx/2; ++i)
    {
        swap(str[i], str[idx - 1 - i]);
    }
    for(size_t i = 0; i < subStr.length()/2; ++i)
    {
        swap(subStr[i], subStr[subStr.length() - 1 - i]);
    }
    str.append(subStr);
    //cout<<"decoy-"<<sequence<<endl;
	
//------------------------------------
/*	
	size_t len = sequence.length();
    char lastCh = sequence[len - 1];
    sequence.erase(sequence.end() - 1, sequence.end());
	sequence.insert(sequence.begin(), lastCh);
*/

}