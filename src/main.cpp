#include"Logger.h"
#include"LogStream.h"


int main(int argc,char **argv)
{
	//����������־
	//rapidlogger::Logger & errlog = rapidlogger::Singleton<rapidlogger::Logger>::getInstance();
	//errlog->configure();
	//��ʼ��д��
	rapidlogger::Logger & errlog = rapidlogger::initRapidLogger(argv[0]);
	//errlog.setName("errlog");
	//errlog->setFilter(rapidlogger::LogFilter(rapidlogger::LevelID::WARN));
	errlog.setAppender(rapidlogger::RollingFileAppender("mylog.log",1000000));
	//errlog->setLayout(rapidlogger::LogLayout("[%p]\t%m%l%n"));
	//errlog.start();

	//��־�������
	long long i = 1e6;
	clock_t start = clock();
	while (i-->0)
	{
		rapidlogger::info_out(errlog) << "Hello 0123456789 abcdefghijklmnopqrstuvwxyz"<<i;
	}
	std::cout << "Time used:" << clock() - start << std::endl;
	system("pause");
	return 0;
}