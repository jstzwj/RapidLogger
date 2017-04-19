#pragma once
#ifndef RAPIDLOGGER_LOGCONDITION
#define RAPIDLOGGER_LOGCONDITION

#include"Logger.h"
#include"LogStream.h"

#define LOG_IF(logger,level,cond) LoggerStream((logger),__FILE__,__FUNCTION__,__LINE__,(level),(cond))
#define LOG_IF_D(level,cond) LoggerStream(::rapidlogger::getLogger(),__FILE__,__FUNCTION__,__LINE__,(level),(cond))

namespace rapidlogger
{
	
}


#endif // !RAPIDLOGGER_LOGCONDITION
