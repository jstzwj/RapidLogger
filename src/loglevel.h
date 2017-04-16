#ifndef LOGLEVEL_H
#define LOGLEVEL_H

#include<string>

namespace rapidlogger
{
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
		

    class LogLevel
    {
    public:
        LogLevel():level_str("ALL"){}
        LogLevel(const std::string& str,int id):level_str(str),level_id(id){}
        std::string to_string()const{return level_str;}
		int getLevel()const { return level_id; }
    protected:
        std::string level_str;
        int level_id;
    };
	class OffLevel:public LogLevel
	{
	public:
		OffLevel() :LogLevel("OFF", 2147483647) {}
	};
	class FatalLevel :public LogLevel
	{
	public:
		FatalLevel() :LogLevel("FATAL", 50000) {}
	};
	class ErrorLevel :public LogLevel
	{
	public:
		ErrorLevel() :LogLevel("ERROR", 40000) {}
	};
	class WarnLevel :public LogLevel
	{
	public:
		WarnLevel() :LogLevel("WARN", 30000) {}
	};
	class InfoLevel :public LogLevel
	{
	public:
		InfoLevel() :LogLevel("INFO", 20000) {}
	};
	class DebugLevel :public LogLevel
	{
	public:
		DebugLevel() :LogLevel("DEBUG", 10000) {}
	};
	class AllLevel :public LogLevel
	{
	public:
		AllLevel() :LogLevel("ALL", -2147483647) {}
	};
	class CustomerLevel :public LogLevel
	{
	public:
		CustomerLevel() :LogLevel("CUSTOMER", 20050) {}
	};

}


#endif // LOGLEVEL_H
