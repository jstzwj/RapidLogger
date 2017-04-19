#pragma once
#ifndef RAPIDLOGGER_LOGSTREAM
#define RAPIDLOGGER_LOGSTREAM

#include<ostream>
#include<string>
#include<cstring>

#include"LogLevel.h"
#include"Logger.h"

namespace rapidlogger
{
#define off_out(logger) LoggerStream(logger,__FILE__,__FUNCTION__,__LINE__,::rapidlogger::OffLevel())
#define fatal_out(logger) LoggerStream(logger,__FILE__,__FUNCTION__,__LINE__,::rapidlogger::FatalLevel())
#define error_out(logger) LoggerStream(logger,__FILE__,__FUNCTION__,__LINE__,::rapidlogger::ErrorLevel())
#define warn_out(logger) LoggerStream(logger,__FILE__,__FUNCTION__,__LINE__,::rapidlogger::WarnLevel())
#define info_out(logger) LoggerStream(logger,__FILE__,__FUNCTION__,__LINE__,::rapidlogger::InfoLevel())
#define debug_out(logger) LoggerStream(logger,__FILE__,__FUNCTION__,__LINE__,::rapidlogger::DebugLevel())
#define all_out(logger) LoggerStream(logger,__FILE__,__FUNCTION__,__LINE__,::rapidlogger::AllLevel())
#define custom_out(logger,level) LoggerStream(logger,__FILE__,__FUNCTION__,__LINE__,level)

#define off_out_d LoggerStream(::rapidlogger::getLogger(),__FILE__,__FUNCTION__,__LINE__,::rapidlogger::OffLevel())
#define fatal_out_d LoggerStream(::rapidlogger::getLogger(),__FILE__,__FUNCTION__,__LINE__,::rapidlogger::FatalLevel())
#define error_out_d LoggerStream(::rapidlogger::getLogger(),__FILE__,__FUNCTION__,__LINE__,::rapidlogger::ErrorLevel())
#define warn_out_d LoggerStream(::rapidlogger::getLogger(),__FILE__,__FUNCTION__,__LINE__,::rapidlogger::WarnLevel())
#define info_out_d LoggerStream(::rapidlogger::getLogger(),__FILE__,__FUNCTION__,__LINE__,::rapidlogger::InfoLevel())
#define debug_out_d LoggerStream(::rapidlogger::getLogger(),__FILE__,__FUNCTION__,__LINE__,::rapidlogger::DebugLevel())
#define all_out_d LoggerStream(::rapidlogger::getLogger(),__FILE__,__FUNCTION__,__LINE__,::rapidlogger::AllLevel())
#define custom_out_d(level) LoggerStream(::rapidlogger::getLogger(),__FILE__,__FUNCTION__,__LINE__,level)


	class LoggerStream
	{
	public:
		LoggerStream(Logger& _logger,const char * _filename, const char * _function, int _line, const LogLevel& _level,bool cond=true)
			:logger(_logger),file_name(_filename),function(_function),line(_line),level(_level),condition(cond)
		{
		}
		~LoggerStream()
		{
			if (condition)
				logger.customer(buffer,file_name,function,line,level);
		}
		LoggerStream& operator<<(bool v)
		{
			if(condition)
				buffer.append(v ? "true" : "false");
			return *this;
		}
		LoggerStream& operator<<(short n)
		{
			if (condition)
				buffer.append(std::to_string(n));
			return *this;
		}
		LoggerStream& operator<<(unsigned short n)
		{
			if (condition)
				buffer.append(std::to_string(n));
			return *this;
		}
		LoggerStream& operator<<(int n)
		{
			if (condition)
				buffer.append(std::to_string(n));
			return *this;
		}
		LoggerStream& operator<<(unsigned int n)
		{
			if (condition)
				buffer.append(std::to_string(n));
			return *this;
		}
		LoggerStream& operator<<(long n)
		{
			if (condition)
				buffer.append(std::to_string(n));
			return *this;
		}
		LoggerStream& operator<<(unsigned long n)
		{
			if (condition)
				buffer.append(std::to_string(n));
			return *this;
		}
		LoggerStream& operator<<(long long n)
		{
			if (condition)
				buffer.append(std::to_string(n));
			return *this;
		}
		LoggerStream& operator<<(unsigned long long n)
		{
			if (condition)
				buffer.append(std::to_string(n));
			return *this;
		}
		LoggerStream& operator<<(float n)
		{
			if (condition)
				buffer.append(std::to_string(n));
			return *this;
		}
		LoggerStream& operator<<(double n)
		{
			if (condition)
				buffer.append(std::to_string(n));
			return *this;
		}

		LoggerStream& operator<<(char c)
		{
			if (condition)
				buffer.push_back(c);
			return *this;
		}

		LoggerStream& operator<<(const char* str)
		{
			if (str)
			{
				if (condition)
					buffer.append(str, std::strlen(str));
			}
			else
			{
				if (condition)
					buffer.append("(null)", 6);
			}
			return *this;
		}

		LoggerStream& operator<<(const unsigned char* str)
		{
			return operator<<(reinterpret_cast<const char*>(str));
		}

		LoggerStream& operator<<(const std::string& str)
		{
			if (condition)
				buffer.append(str.c_str(), str.size());
			return *this;
		}

		std::string & str()
		{
			return buffer;
		}
	protected:
		const char * file_name;
		const char * function;
		int line;
		bool condition;
		const LogLevel& level;
		std::string buffer;
		Logger &logger;
	};

}


#endif // !RAPIDLOGGER_LOGSTREAM
