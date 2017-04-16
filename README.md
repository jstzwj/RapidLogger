# RapidLogger
A fast logger for C++ 

# Introduction
The library was inspired by RapidJSON and log4j.
RapidLogger is small and fast.

# Compatibility
These compilers which have been tested are shown in the following list.

* Visual C++ 2015 on Windows (32/64-bit)

# How to use
The RapidLogger library is a header-only library.Just copy the folder in your project.

# Usage
Use RapidLogger in five lines.

~~~cpp
#include"Logger.h"
#include"Singleton.h"
int main()
{
  //Create a singleton logger
	rapidlogger::Logger * errlog = &rapidlogger::Singleton<rapidlogger::Logger>::getInstance();
  //Set logger name
	errlog->setName("errlog");
  //Set log target
	errlog->setAppender(rapidlogger::FileAppender("mylog.log"));
  //Start async logger
	errlog->start();
  //Put a log
	errlog->logInfo("Hello world 0123456789 abcdefghijklmnopqrstuvwxyz");
	return 0;
}
~~~
