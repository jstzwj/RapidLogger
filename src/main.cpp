#include"logger.h"
#include"singleton.h"

int main()
{
	//开启错误日志
	rapidlogger::Logger * errlog = &rapidlogger::Singleton<rapidlogger::Logger>::getInstance();
	errlog->configure();
	errlog->setName("errlog");
	errlog->setAppender(rapidlogger::FileAppender("mylog.log"));
	errlog->setLayout(rapidlogger::LogLayout("%t\t[%p]\t%m%n"));
	errlog->start();

	//日志输出测试
	clock_t start = clock();
	errlog->info("LogQuickTest:id=1 time:" + std::to_string(clock() - start));
	std::cout << "Time used:" << clock() - start << std::endl;
	return 0;
}