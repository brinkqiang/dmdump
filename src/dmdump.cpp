
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

#ifdef WIN32
#define popen _popen
#define pclose _pclose

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

#endif

#define BUFFER_SIZE 256

std::string DMExecute(const char* cmd)
{
    std::array<char, BUFFER_SIZE> buffer;
    std::string result;

    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);

    if (!pipe) return result;

    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), BUFFER_SIZE, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

#ifdef WIN32

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
        printf("PID: %d (%s) \n", procEntry.th32ProcessID, procEntry.szExeFile);
        bRet = Process32Next(procSnap, &procEntry);
    }
    CloseHandle(procSnap);
    return vecList;
}

bool DMGenDumpFile(const std::string& strName)
{
    std::vector<uint64_t> vecList = DMGetProcessList(strName);

    for (int i=0; i < vecList.size(); ++i)
    {
        char cmd[256] = { 0 };
        sprintf(cmd, "procdump.exe -ma -w %d %s_%d.dmp", (int)vecList[i], strName.c_str(), (int)vecList[i]);
        std::string strData = DMExecute(cmd);
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

bool DMGenDumpFile(const std::string& strName)
{
    std::vector<uint64_t> vecList = DMGetProcessList(strName);

    for (int i = 0; i < vecList.size(); ++i)
    {

        char cmd[256] = { 0 };
        sprintf(cmd, "pstree -p %d", strName.c_str(), (int)vecList[i]);
        std::string data = DMExecute(cmd);
        std::cout << data << std::endl;

        char cmd2[256] = { 0 };
        sprintf(cmd2, "gcore -o %s.core.%d %d", strName.c_str(), (int)vecList[i], (int)vecList[i]);
        std::string data2 = DMExecute(cmd2);

        char cmd3[256] = { 0 };
        sprintf(cmd3, "gen %s.core.%d", strName.c_str(), (int)vecList[i]);
        std::cout << cmd3 << std::endl;
    }

    return true;
}

#endif
