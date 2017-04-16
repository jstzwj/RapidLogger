#include"Logger.h"
#include"Singleton.h"

int main()
{
	//����������־
	rapidlogger::Logger * errlog = &rapidlogger::Singleton<rapidlogger::Logger>::getInstance();
	//errlog->configure();
	errlog->setName("errlog");
	//errlog->setFilter(rapidlogger::LogFilter(rapidlogger::LevelID::WARN));
	errlog->setAppender(rapidlogger::FileAppender("mylog.log"));
	//errlog->setLayout(rapidlogger::LogLayout("[%p]\t%m%l%n"));
	errlog->start();

	//��־�������
	long long i = 1e6;
	clock_t start = clock();
	while (i-->0)
	{
		errlog->logInfo("Hello 0123456789 abcdefghijklmnopqrstuvwxyz"+std::to_string(i));
	}
	std::cout << "Time used:" << clock() - start << std::endl;
	system("pause");
	return 0;
}