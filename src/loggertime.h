#ifndef RAPIDLOGGER_TIME_H
#define RAPIDLOGGER_TIME_H

#include<string>
#include<sstream>
#include<algorithm>
#include<chrono>
#include<iomanip>
#include<cstdint>
#include<ctime>


#include"platform.h"

#ifdef RAPIDLOGGER_OS_WINDOWS
#include <windows.h>
#endif
#ifdef RAPIDLOGGER_OS_LINUX
#include <time.h>
#endif

namespace rapidlogger
{
    class Time
    {
    public:
        static int getYear()
		{
			std::time_t timep;
			std::tm *p;
			std::time(&timep);
			p = std::gmtime(&timep);
			return (1900 + (p->tm_year));
		}
        static int getMonth()
		{
			std::time_t timep;
			std::tm *p;
			std::time(&timep);
			p = std::gmtime(&timep);
			return (1 + (p->tm_mon));
		}
        static int getDay()
		{
			std::time_t timep;
			std::tm *p;
			std::time(&timep);
			p = std::gmtime(&timep);
			return (p->tm_mday);
		}
        static int getHour()
		{
			std::time_t timep;
			std::tm *p;
			std::time(&timep);
			p = std::gmtime(&timep);
			return (p->tm_hour);
		}
        static int getMinute()
		{
			std::time_t timep;
			std::tm *p;
			std::time(&timep);
			p = std::gmtime(&timep);
			return (p->tm_min);
		}
        static int getSecond()
		{
			std::time_t timep;
			std::tm *p;
			std::time(&timep);
			p = std::gmtime(&timep);
			return (p->tm_sec);
		}
        static std::string getTime(const std::string& format)
		{
			std::string result = format;
			std::time_t timep = std::time(nullptr);
			std::tm *p = std::gmtime(&timep);
			std::stringstream stream;

			while (result.find("%SSS") != std::string::npos)
				result.replace(result.find("%SSS"), 4, std::to_string(Time::GetMicros()));
			stream << std::put_time(p, result.c_str());
			return stream.str();
		}
        static std::string getTime()
		{
			std::time_t timep = std::time(nullptr);
			std::tm *p = std::gmtime(&timep);
			thread_local int last_sec = p->tm_sec;
			thread_local std::string time_buf = "";
			int micros = Time::GetMicros();
			if (time_buf.empty() || last_sec != p->tm_sec)
			{
				std::stringstream stream;
				stream << std::put_time(p, ("%F %T."));
				time_buf = stream.str();
			}
			return time_buf + std::to_string(micros);
		}
        static int64_t GetSysTimeMicros()
		{
#ifdef _WIN32
			// 从1601年1月1日0:0:0:000到1970年1月1日0:0:0:000的时间(单位100ns)
#define EPOCHFILETIME   (116444736000000000UL)
			FILETIME ft;
			LARGE_INTEGER li;
			int64_t tt = 0;
			GetSystemTimeAsFileTime(&ft);
			li.LowPart = ft.dwLowDateTime;
			li.HighPart = ft.dwHighDateTime;
			// 从1970年1月1日0:0:0:000到现在的微秒数(UTC时间)
			tt = (li.QuadPart - EPOCHFILETIME) / 10;
			return tt;
#endif
#ifdef RAPIDLOGGER_OS_LINUX
			timeval tv;
			gettimeofday(&tv, 0);
			return (int64_t)tv.tv_sec * 1000000 + (int64_t)tv.tv_usec;
#endif
			return 0;
		}
		static int64_t Time::GetMicros()
		{
			return GetSysTimeMicros() % 1000000;
		}
    };
}

#endif // RAPIDLOGGER_TIME_H
