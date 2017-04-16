#ifndef MOCORES_TIME_H
#define MOCORES_TIME_H

#include<string>
#include<sstream>
#include<algorithm>
#include<chrono>
#include<iomanip>
#include<cstdint>
#include<ctime>


#include"platform.h"

namespace rapidlogger
{
    class Time
    {
    public:
        static int getYear();
        static int getMonth();
        static int getDay();
        static int getHour();
        static int getMinute();
        static int getSecond();
        static std::string getTime(const std::string& format);
        static std::string getTime();
        static int64_t GetSysTimeMicros();
		static int64_t Time::GetMicros();
    };
}

#endif // MOCORES_TIME_H