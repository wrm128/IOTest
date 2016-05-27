#ifndef BASICTOOLS_H_
#define BASICTOOLS_H_

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <string>
#include <limits>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <time.h>

using namespace std;

class Clog
{
public:
	Clog();
	~Clog();
	string GetTimeStr();					// 获取当前时间，格式如：Thu May 22 10:31:30 2014
	void AppendInfo(string info);			// 记录信息，在Clog类外修改m_strLogInfo的窗口
	void PrintInfoTo(ostream &output, string infoType, string Info); 
											// 按照一定的格式输出提示信息并记录
private:
	vector<string> m_strLogInfo;			// 存储每条日志信息
	string m_filename;						// 日志文件名（含路径）
	FILE *m_pfile;							// 日志文件

	void _Save();							// 将记录下的log信息输出
};


class COptionTool
{
private:
	static const int BUFFER_SIZE;
	string m_strFile;

	void _GetPrivateProfileString(const string & strSection, const char * strKey, 
			const char * szDefault, char szValue[], int nBufSize);
		
	int _GetPrivateProfileInt(const string & strSection, const char * strKey, const int & nDefault);
					
public:
	COptionTool(const string szFile);
	~COptionTool();

	string GetString(const char * strSection, const char * strKey, const char * szDefault);
	double GetDouble(const char * strSection, const char * strKey, const double lfDefault);
	int GetInteger(const char * strSection, const char * strKey, const int nDefault);		
	size_t GetSizeT(const char * strSection, const char * strKey, const size_t tDefault);
	bool GetBool(const char * strSection, const char * strKey, const bool bDefault);
};

class CErrInfo {
	string m_strClass;
	string m_strMethod;
	string m_strDetail;
	string m_strInfo;
	string m_strException;
public:
	CErrInfo(const string &strClass, const string &strMethod,
			const string &strDetail="");

	CErrInfo(const string &strClass, const string &strMethod,
			const string &strDetail, const exception & e);
	
	void Append(const string &strInfo);
	void SetException(const exception & e);
	string Get() const;
	string Get(const exception& e);

	friend ofstream& operator<<(ofstream& os, const CErrInfo& info);
	friend ostream& operator<<(ostream& os, const CErrInfo& info);
};


class CStringProcess {
public:
	static void Trim(string& str);
	static void Trim(const char *&fptr, const char *&lptr);
	static bool bIsNoUse(const char ch);
	static bool bIsNumber(const char ch);
	static void ToLower(string& str);
	static void Split(const string& strFullString, const string& strSign,
			string& strPrior, string& strLatter);
	static void Split(const string str, const string &delim, vector<string> &ret);
	static bool bMatchingFix(string strFullString, string strFix, bool bSuffix,bool bCaseSensitive);
	static bool isInSet(const string arr[], int n, string &key);
	static void Reverse(string &str);
};


#endif