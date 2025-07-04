
// Copyright (c) 2018 brinkqiang (brink.qiang@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "dmdump.h"
#include <memory>
#include <array>
#include <iostream>
#include <stdio.h>

#include "dmstrtk.hpp"
#include "dmutil.h"

#ifdef _WIN32
#define popen _popen
#define pclose _pclose

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

#endif

#define BUFFER_SIZE 256

std::string DMExecute(const char* cmd)
{
    std::string result;

    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);

    if (!pipe) return result;

    while (!feof(pipe.get())) {
        char szBuf[BUFFER_SIZE+1] = {0};
        if (fgets(szBuf, BUFFER_SIZE, pipe.get()) != nullptr)
        {
            result += szBuf;
        }
    }
    return result;
}

#ifdef _WIN32

std::vector<uint64_t> DMGetProcessList(const std::string& strName)
{
    std::vector<uint64_t> vecList;
    HANDLE procSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (procSnap == INVALID_HANDLE_VALUE)
    {
        return vecList;
    }

    PROCESSENTRY32 procEntry = { 0 };
    procEntry.dwSize = sizeof(PROCESSENTRY32);
    BOOL bRet = Process32First(procSnap, &procEntry);
    while (bRet)
    {
        std::string strExeFile = procEntry.szExeFile;
        if (strExeFile == strName)
        {
            vecList.push_back(procEntry.th32ProcessID);
        }
        //printf("PID: %d (%s) \n", procEntry.th32ProcessID, procEntry.szExeFile);
        bRet = Process32Next(procSnap, &procEntry);
    }
    CloseHandle(procSnap);
    return vecList;
}

bool DMGenDumpFile(const std::string& strName, bool bWin32)
{
    std::vector<uint64_t> vecList = DMGetProcessList(strName);
    std::string strTime = DMFormatDateTime(time(0), "%Y-%m-%d-%H-%M-%S");
    for (int i=0; i < vecList.size(); ++i)
    {
        char cmd[256] = { 0 };

        if (bWin32)
        {
            sprintf(cmd, "procdump.exe -ma %d %s_%d_%s.dmp", (int)vecList[i], strName.c_str(), (int)vecList[i], strTime.c_str());
        }
        else
        {
            sprintf(cmd, "procdump.exe -64 -ma %d %s_%d_%s.dmp", (int)vecList[i], strName.c_str(), (int)vecList[i], strTime.c_str());
        }
        std::cout << cmd << std::endl;
        std::string strData = DMExecute(cmd);
        std::cout << strData << std::endl;;
    }

    return true;
}

#else
std::vector<uint64_t> DMGetProcessList(const std::string& strName)
{
    std::vector<uint64_t> vecList;

    char cmd[256] = { 0 };
    sprintf(cmd, "pidof %s", strName.c_str());
    std::string data = DMExecute(cmd);

    strtk::std_string::token_list_type token_list;

    std::vector<std::string> vecData;
    const std::string delimiters("\t\r\n ");

    strtk::parse(data, delimiters, vecData);

    for (int i=0; i < static_cast<int>(vecData.size()); ++i)
    {
        if (vecData[i].empty())
        {
            continue;
        }
        vecList.push_back(atoll(vecData[i].c_str()));
    }

    return vecList;
}

bool DMGenDumpFile(const std::string& strName, bool bWin32)
{
    std::vector<uint64_t> vecList = DMGetProcessList(strName);

    for (int i = 0; i < vecList.size(); ++i)
    {
        char cmd[256] = { 0 };
        sprintf(cmd, "pstree -p %d", (int)vecList[i]);
        std::string data = DMExecute(cmd);
        std::cout << data;

        char cmd2[256] = { 0 };
        sprintf(cmd2, "gcore -o %s.core %d", strName.c_str(), (int)vecList[i]);
        std::string data2 = DMExecute(cmd2);
        std::cout << data2;

        char cmd3[256] = { 0 };
        sprintf(cmd3, "%s.core.%d", strName.c_str(), (int)vecList[i]);
        std::cout << cmd3 << std::endl;
    }

    return true;
}

#endif
