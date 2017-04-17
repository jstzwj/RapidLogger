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
Use RapidLogger in three lines.

~~~cpp
#include"Logger.h"
#include"LogStream.h"
using namespace rapidlogger;
int main()
{
  //Init a singleton logger
	Logger & errlog = initRapidLogger(argv[0]);
  //Set log target
	errlog.setAppender(FileAppender("mylog.log"));
  //Put a log
	info_out(errlog) << "Hello 0123456789 abcdefghijklmnopqrstuvwxyz";
	return 0;
}
~~~
