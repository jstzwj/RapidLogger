#pragma once
#ifndef RAPIDLOGGER_LOGFILTER
#define RAPIDLOGGER_LOGFILTER

#include"LogLevel.h"

namespace rapidlogger
{

	class LogFilter
	{
	public:
		LogFilter() :start(MINLEVEL), finish(MAXLEVEL) {}
		LogFilter(int _start):start(_start),finish(MAXLEVEL) {}
		LogFilter(int _start,int _finish) :start(_start),finish(_finish) {}
		bool isInRange(const LogLevel& level)
		{
			if (level.getLevel() >= start&&level.getLevel() <= finish)
				return true;
			else
				return false;
		}
	protected:
		int start;
		int finish;
	};
}
#endif // !RAPIDLOGGER_LOGFILTER
