#pragma once
#include "framework.h"
#include <ShlObj_core.h>
#include <sstream>




class Timer
{
public:
    void start()
    {
        m_StartTime = std::chrono::system_clock::now();
        m_bRunning = true;
    }

    void stop()
    {
        m_EndTime = std::chrono::system_clock::now();
        m_bRunning = false;
    }

    double elapsedMilliseconds()
    {
        std::chrono::time_point<std::chrono::system_clock> endTime;

        if (m_bRunning)
        {
            endTime = std::chrono::system_clock::now();
        }
        else
        {
            endTime = m_EndTime;
        }

        return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
    }

    double elapsedSeconds()
    {
        return elapsedMilliseconds() / 1000.0;
    }

private:
    std::chrono::time_point<std::chrono::system_clock> m_StartTime;
    std::chrono::time_point<std::chrono::system_clock> m_EndTime;
    bool                                               m_bRunning = false;
};


class Log
{
private:
    std::string getCurrentDateTime()
    {
        time_t now = time(0);
        struct tm  tstruct;
        CHAR buffer[MAX_PATH] = { 0 };
        localtime_s(&tstruct, &now);
        strftime(buffer, MAX_PATH, "%Y-%m-%d %X", &tstruct);
        std::string result;
        return result = buffer;
    };

 public:
    CHAR docsPath[MAX_PATH] = { 0 };

    VOID myDocs()
    {
        SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, docsPath);
    }

    VOID Logger(std::string logMsg)
    {

        std::string filePath = docsPath;
        filePath.append("\\__Log.txt");

        std::string now = getCurrentDateTime();
        std::ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);
        ofs << now << '\t' << logMsg << '\n';
        ofs.close();
    }
};

inline Log Logger;



template <typename... Args>
inline std::string Format(const char* pFormat, Args...args)
{
	int Length = std::snprintf(nullptr, 0, pFormat, args...);
	if (Length <= 0)
		return "";
	char* Str = new char[Length + 1];
	std::string Result;
	std::snprintf(Str, Length + 1, pFormat, args...);
	Result = std::string(Str);
	delete[] Str;
	return Result;
}