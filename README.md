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

## Output logs

~~~cpp
 logOff_d(msg)
 logFatal_d(msg)
 logError_d(msg)
 logWarn_d(msg)
 logInfo_d(msg)
 logDebug_d(msg)
 logAll_d(msg)
 logCustomer_d(msg,level)
~~~


## C++ stream style logs output

~~~cpp
 off_out_d
 fatal_out_d
 error_out_d
 warn_out_d
 info_out_d
 debug_out_d
 all_out_d
 custom_out_d(level)
~~~

Take warning for example.
~~~cpp
 warn_out_d<<"This is a warning.";
~~~

## About log layout

The LogLayout class
%m Output the message.
%p Output the log level, such as WARN, FATAL.
%r Put the microsecond from the application initialization.
%t Put the thread id.
%n Print a newline character. "\r\n" under Windows. "\n" under Unix.
%d Print the log time. Default format is "%Y-%m-%d %H:%M:%S.%SSS". The format can be defined by user. %d{%Y %m %d %H:%M:%S,%SSS}. It will output "2002 10 18 22:10:28,921".
%l Print the log location in the source including function name, thread id and line number. For example, main(TestLog.cpp:10).

## About appenders
* ConsoleAppender
* FileAppender
* DailyRollingFileAppender
* RollingFileAppender



## About log level
Built-in level
~~~cpp
#define MAXLEVEL 2147483647
#define MINLEVEL -2147483647
enum LevelID
{
	OFF=2147483647,
	FATAL=50000,
	ERROR=40000,
	WARN=30000,
	INFO=20000,
	DEBUG=10000,
	ALL=-2147483647,
	CUSTOMER=20050
};

#define LEVEL_OFF OffLevel()
#define LEVEL_FATAL FatalLevel()
#define LEVEL_ERROR ErrorLevel()
#define LEVEL_WARN WarnLevel()
#define LEVEL_INFO InfoLevel()
#define LEVEL_DEBUG DebugLevel()
#define LEVEL_ALL AllLevel()
#define LEVEL_CUSTOM CustomerLevel()

~~~



## About filter

You can set a level range for the logs you want.
~~~cpp

LogFilter();	//MINLEVEL to MAXLEVEL
LogFilter(int _start);	//start to MAXLEVEL including start
LogFilter(int _start,int _finish);	//start to finish including start and finish

~~~


## Conditional logging macros
~~~cpp
/*
Logger will print a log when condition is true
*/
LOG_IF(errlog, LEVEL_INFO,condition) << "Hello 0123456789 abcdefghijklmnopqrstuvwxyz" << i;


~~~


# Advanced usage

## Multiple loggers
~~~cpp
//Init a singleton logger which instance id is 1.
  	initRapidLogger<1>(argv[0], FileAppender("mylog.log"));
//Put a log by logger 1.
	getLogger<1>().info_out<< "Hello world 0123456789 abcdefghijklmnopqrstuvwxyz"<<i;

~~~
The default logger id is 0;


## Multiple appenders
~~~cpp
template<class T>
void addAppender(const T& app);
void removeLastAppender();
void clearAppender();

~~~



## Custom data type
~~~cpp

LoggerStream& operator<<(LoggerStream& stream,T n);

~~~

## Custom log level
~~~cpp
class MyCustomerLevel :public LogLevel
{
public:
	MyCustomerLevel() :LogLevel("MYCUSTOMER", 20051) {}
};

~~~

## Custom appender
Override all of the virtual member function in LogAppender.
~~~cpp

class LogAppender
{
public:
        LogAppender()=default;
        /*inner function*/
        virtual int open()=0;
        /*inner function*/
        virtual void close()=0;
        virtual void append(const std::string & str)=0;
        virtual void flush()=0;
};

~~~



# Performance

