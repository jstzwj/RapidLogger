#include"Logger.h"
#include"LogStream.h"
#include"Singleton.h"

int main()
{
	//开启错误日志
	rapidlogger::Logger & errlog = rapidlogger::Singleton<rapidlogger::Logger>::getInstance();
	//errlog->configure();
	errlog.setName("errlog");
	//errlog->setFilter(rapidlogger::LogFilter(rapidlogger::LevelID::WARN));
	errlog.setAppender(rapidlogger::FileAppender("mylog.log"));
	//errlog->setLayout(rapidlogger::LogLayout("[%p]\t%m%l%n"));
	errlog.start();

	//日志输出测试
	long long i = 1e6;
	clock_t start = clock();
	while (i-->0)
	{
		info_out(errlog) << "Hello 0123456789 abcdefghijklmnopqrstuvwxyz"<<i;
	}
	std::cout << "Time used:" << clock() - start << std::endl;
	system("pause");
	return 0;
}