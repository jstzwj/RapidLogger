#pragma once
#ifndef RAPIDLOGGER_LOGSTREAM
#define RAPIDLOGGER_LOGSTREAM

#include<ostream>
#include<string>
#include<cstring>

namespace rapidlogger
{
	class endlog{};

	class LoggerStream
	{
	public:
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
		std::string buffer;
	};
}


#endif // !RAPIDLOGGER_LOGSTREAM
