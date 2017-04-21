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

## Log 'hello world'
Use RapidLogger in two lines.

~~~cpp
#include"rapidlogger.h"
using namespace rapidlogger;
int main(int argc,char **argv)
{
  //Init a singleton logger
  	initRapidLogger(argv[0], FileAppender("mylog.log"));
  //Put a log
	info_out_d<< "Hello world 0123456789 abcdefghijklmnopqrstuvwxyz"<<i;
	return 0;
}
~~~

## Init logger
Before using a logger, it shall be inited by function 'initRapidLogger'.
~~~cpp
 initRapidLogger(argv[0]);
~~~
The first paramater of 'initRapidLogger' is the name of logger. The function will call 'setName' and 'start'.
Another thread will be created to get logs from message queue, which will be written to appenders.


## Get a logger 

To get the logger use rapidlogger::getLogger() function:
~~~cpp
 Logger & getLogger()
~~~


## Close a logger
If a logger will not be used, you can close it by call 'closeRapidLogger'
~~~cpp
 void closeRapidLogger()
~~~

## Some settings of logger

Most settings can only modified when the logger is closed. However, I am considering to make it possible to change settings when logger is running in the future versions.

### Set name

~~~cpp
 void setName(const char * name)
~~~
The name can be used in log layout.

### Set appender

~~~cpp
 template<class T> void addAppender(const T& app);

 void removeLastAppender();

 void clearAppender();
~~~
### Set layout
~~~cpp
 void setLayout(const LogLayout& _layout);
~~~

### Set filter
~~~cpp
 void setFilter(const LogFilter& _filter);
~~~


# Advanced usage



