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
Use RapidLogger in two lines.

~~~cpp
#include"Logger.h"
#include"LogStream.h"
using namespace rapidlogger;
int main(int argc,char **argv)
{
  //Init a singleton logger
  	rapidlogger::initRapidLogger(argv[0], rapidlogger::RollingFileAppender("mylog.log", 1000000));
  //Put a log
	info_out_d<< "Hello 0123456789 abcdefghijklmnopqrstuvwxyz"<<i;
	return 0;
}
~~~
