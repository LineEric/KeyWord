// KEY_WORD.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "sqlite3.h"
#include "iostream"
#include "vector"
#include "string"
#include "string.h"
#include "time.h"
#include "Windows.h"
#include "fstream"
#include "stdafx.h"
#include "KEY_WORD.h"//DLL相关头文件
#pragma comment(lib, "sqlite3.lib")
using namespace std;

char* UTF8ToGBK(const char* strUTF8) {   //进行UTF8转至GBK
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);

	if (wszGBK) delete[] wszGBK;

	return szGBK;
}

char * GBKToUTF8(const char* strGBK) {  //GBK转至UTF8
	int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);

	if (wstr) delete[] wstr;
	return str;
}

char * Key_Word(char *msg)//DLL主体
{
	sqlite3 *conn = NULL;
	sqlite3_stmt *order;
	char sql[500] = { "SELECT answer FROM qa WHERE keyword GLOB '" };
	char *SQL;
	strcat(sql, msg);
	strcat(sql, "';");
	//上述进行SQL指令编写
	vector<string> strs;
	SQL = GBKToUTF8(sql); //在语言为简体中文下的VS2015以下进行编译采用为GBK2312
	if (SQLITE_OK != sqlite3_open("qav2.db", &conn))//其中sqlite_open()的使用将数据库内部进行UTF8编码
		return "Open_ERROR";
	int result = sqlite3_prepare_v2(conn, SQL, -1, &order, NULL);
	if (SQLITE_OK != result)
		return "prepare_error";

	while (101 != sqlite3_step(order))//sqlite3_step()在对sqlite操作结束后会返回101
	{
		string str = string(UTF8ToGBK((const char *)sqlite3_column_text(order, 0)));
		wcout << str.c_str() << endl;
		strs.push_back(str);
	}
	 sqlite3_finalize(order);
	 sqlite3_close(conn);
	//断开连通与清除指令
	if (strs.empty())
		return "NULL";//
	else
	{
		int r;
		char * str;
		srand(time(NULL));
		if (strs.size() > 1)
			r = rand() % strs.size();
		else
			r = 0;
		str = (char *)malloc(strs[r].size() * sizeof(char));
		strcpy(str, strs[r].c_str());//为使对应e语言文本型兼容
		return str;
	}
}
