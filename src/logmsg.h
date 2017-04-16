#ifndef LOGMSG_H
#define LOGMSG_H

#include<string>
#include<sstream>
#include<thread>


#include"platform.h"
#include"loglevel.h"

#include"loggertime.h"



namespace rapidlogger
{
    class LogMsg
    {
    public:
        LogMsg():thread_id(),level(),msg(){}
        LogMsg(const LogLevel& _level,const std::string & _msg,std::string _id)
            :thread_id(_id),level(_level),msg(_msg),file_name(nullptr),function_name(nullptr),line(0){}
		LogMsg(const LogLevel& _level,
			const std::string & _msg, 
			std::string _id,
			const char * file, 
			const char * function,
			int linenum)
			:thread_id(_id), 
			level(_level),
			msg(_msg),
			file_name(file),
			function_name(function),
			line(linenum) {}

        std::string getLevel()const {return level.to_string();}
        std::string getMsg()const {return msg;}
        std::string getThreadId()const
        {
            std::stringstream stream;
            stream<<thread_id;
            return stream.str();
        }
        std::string getNewLine()const
        {
#ifdef RAPIDLOGGER_OS_WINDOWS
            return "\r\n";
#endif
#ifdef RAPIDLOGGER_OS_LINUX
            return "\n";
#endif
        }
        std::string getTime(const std::string & format)const
        {
            return Time::getTime(format);
        }
		std::string getTime()const
		{
			return Time::getTime();
		}
        std::string getTimeFromStart(const std::chrono::time_point<std::chrono::system_clock,std::chrono::nanoseconds> start_time)const
        {
            auto now_time=std::chrono::system_clock::now();
            auto diff=now_time-start_time;
            std::stringstream stream;
            stream<<diff.count();
            return stream.str();
        }
		const char * getFileName()const { return file_name; }
		const char * getFunctionName()const { return function_name; }
		std::string getLineNum()const { return std::to_string(line); }
    protected:
        std::string thread_id;
        LogLevel level;
        std::string msg;
		const char * file_name;
		const char * function_name;
		int line;
    };
}
#endif // LOGMSG_H
