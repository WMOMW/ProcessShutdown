// ProcessShutdown.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup")

using namespace std;


// 强杀定期删除进程

HANDLE GetProcessHandle(int);
deque<HANDLE> GetProcessHandle(LPCWSTR);
LPCWSTR _lpName;
vector<LPCWSTR> ProcessName;
unsigned int delay_time = 10000;
const unsigned int temp_time = delay_time;
bool delay_flag = true;

bool Quit(LPCWSTR lpName) {

	// 结束强杀定期删除进程
	deque<HANDLE> hProcess = GetProcessHandle(lpName);
	if (hProcess.begin() == hProcess.end())
	{
		return false;
	}
	while (hProcess.begin() != hProcess.end())
	{
		if ((*hProcess.begin()) != NULL)
		{
			TerminateProcess((*hProcess.begin()), 0);
			CloseHandle((*hProcess.begin()));
			hProcess.pop_front();
		}
	}
	/*if (hProcess != NULL)
	{
		TerminateProcess(hProcess, 0);
		CloseHandle(hProcess);
		return true;
	}
	CloseHandle(hProcess);*/
	return true;
}



//通过进程ID获取进程句柄

HANDLE GetProcessHandle(int nID) {
	return OpenProcess(PROCESS_TERMINATE, FALSE, nID);
}



//通过进程名获取进程句柄

deque<HANDLE> GetProcessHandle(LPCWSTR lpName) {
	deque<HANDLE> handle;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		return handle;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL fOk;
	for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe))
	{
		if (!_tcscmp(pe.szExeFile, lpName))
		{
			handle.push_back(GetProcessHandle(pe.th32ProcessID));
		}
	}
	CloseHandle(hSnapshot);
	return handle;
}

int main(int argc, char **argv)
{
	fstream P_name;
	P_name.open("P_name.txt", ios::in);
	if (!P_name)
	{
		P_name.open("P_name.txt", ios::out);
		exit(1);
	}
	string s;
	LPWSTR ws;
	while (P_name >> s)
	{
		const char *pCStrKey = s.c_str();
		int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);
		ws = new wchar_t[pSize];
		MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, ws, pSize);
		ProcessName.push_back(ws);
	}
	P_name.close();
	while (true)
	{
		bool flag = false;
		unsigned int p = 1;
		do {
			for (auto i : ProcessName)
			{
				if (Quit(i))
				{
					flag = true;
				}
			}
			if (!flag)
			{
				++p;
				delay_flag = false;
			}
			else
			{
				if (delay_flag)
				{
					delay_time = static_cast<unsigned int>(delay_time*0.5);
					if (delay_time < 1000)
					{
						delay_time = 1000;
					}
				}
				else
				{
					delay_time = (temp_time * p + 2 * delay_time) / 2;
					if (delay_time > 600000)
					{
						delay_time = 600000;
					}
				}
				delay_flag = true;
				break;
			}
			Sleep(temp_time);
		} while (true);
		Sleep(delay_time);
	}
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
