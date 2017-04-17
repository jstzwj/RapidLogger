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

	class LoggerStream
	{
	public:
		LoggerStream(Logger& _logger,const char * _filename, const char * _function, int _line, const LogLevel& _level)
			:logger(_logger),file_name(_filename),function(_function),line(_line),level(_level)
		{
		}
		~LoggerStream()
		{
			logger.customer(buffer,file_name,function,line,level);
		}
		LoggerStream& operator<<(bool v)
		{
			buffer.append(v ? "true" : "false");
			return *this;
		}
		LoggerStream& operator<<(short n)
		{
			buffer.append(std::to_string(n));
			return *this;
		}
		LoggerStream& operator<<(unsigned short n)
		{
			buffer.append(std::to_string(n));
			return *this;
		}
		LoggerStream& operator<<(int n)
		{
			buffer.append(std::to_string(n));
			return *this;
		}
		LoggerStream& operator<<(unsigned int n)
		{
			buffer.append(std::to_string(n));
			return *this;
		}
		LoggerStream& operator<<(long n)
		{
			buffer.append(std::to_string(n));
			return *this;
		}
		LoggerStream& operator<<(unsigned long n)
		{
			buffer.append(std::to_string(n));
			return *this;
		}
		LoggerStream& operator<<(long long n)
		{
			buffer.append(std::to_string(n));
			return *this;
		}
		LoggerStream& operator<<(unsigned long long n)
		{
			buffer.append(std::to_string(n));
			return *this;
		}
		LoggerStream& operator<<(float n)
		{
			buffer.append(std::to_string(n));
			return *this;
		}
		LoggerStream& operator<<(double n)
		{
			buffer.append(std::to_string(n));
			return *this;
		}

		LoggerStream& operator<<(char c)
		{
			buffer.push_back(c);
			return *this;
		}

		LoggerStream& operator<<(const char* str)
		{
			if (str)
			{
				buffer.append(str, std::strlen(str));
			}
			else
			{
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
		const LogLevel& level;
		std::string buffer;
		Logger &logger;
	};

	class LoggerStreamFake
	{
		LoggerStreamFake& operator<<(bool v){return *this;}
		LoggerStreamFake& operator<<(short n) { return *this; }
		LoggerStreamFake& operator<<(unsigned short n) { return *this; }
		LoggerStreamFake& operator<<(int n) { return *this; }
		LoggerStreamFake& operator<<(unsigned int n) { return *this; }
		LoggerStreamFake& operator<<(long n) { return *this; }
		LoggerStreamFake& operator<<(unsigned long n) { return *this; }
		LoggerStreamFake& operator<<(long long n) { return *this; }
		LoggerStreamFake& operator<<(unsigned long long n) { return *this; }
		LoggerStreamFake& operator<<(float n) { return *this; }
		LoggerStreamFake& operator<<(double n) { return *this; }
		LoggerStreamFake& operator<<(char c) { return *this; }
		LoggerStreamFake& operator<<(const char* str) { return *this; }
		LoggerStreamFake& operator<<(const unsigned char* str) { return *this; }
		LoggerStreamFake& operator<<(const std::string& str) { return *this; }

	};
}


#endif // !RAPIDLOGGER_LOGSTREAM
