#pragma once
#ifndef RAPIDLOGGER_STRINGBUFFER
#define RAPIDLOGGER_STRINGBUFFER
#include<string>
#include<mutex>
namespace rapidlogger
{
	class StringBuffer
	{
	public:
		void append(const std::string &str)
		{
			std::unique_lock<std::mutex> lock(mtx);
			buffer += str;
		}
		void clear()
		{
			std::unique_lock<std::mutex> lock(mtx);
			buffer.clear();
		}
		/*std::string& getBuffer()
		{
			return buffer;
		}*/
	private:
		std::string buffer;
		std::mutex mtx;
	};

}

#endif // !RAPIDLOGGER_STRINGBUFFER
