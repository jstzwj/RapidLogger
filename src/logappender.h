#ifndef RAPIDLOGGER_LOGAPPENDER_H
#define RAPIDLOGGER_LOGAPPENDER_H

#include<ostream>
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>

#include"LoggerTime.h"

namespace rapidlogger
{
	std::string getPathSuffix(const std::string& path)
	{
		std::string result;
		size_t dot=path.find_last_of('.');
		if ( dot!= std::string::npos)
		{
			result = path.substr(dot,path.length());
		}
		return result;
	}
	std::string getPathWithoutSuffix(const std::string& path)
	{
		std::string result;
		size_t dot = path.find_last_of('.');
		if (dot != std::string::npos)
		{
			result = path.substr(0, dot);
		}
		return result;
	}

	uint64_t getFileSize(const std::string &path)
	{
		std::ifstream in(path.c_str());
		in.seekg(0, std::ios::end);
		uint64_t size = in.tellg();
		in.close();
		return size;
	}


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

    /*!
     *  \brief default ostream
     *  \details ConsoleAppender（控制台），
     *           FileAppender（文件），
     *           DailyRollingFileAppender（每天产生一个日志文件），
	 *           以下暂时不支持
     *           RollingFileAppender（文件大小到达指定尺寸的时候产生一个新的文件），
     *           WriterAppender（将日志信息以流格式发送到任意指定的地方）
     */
    class ConsoleAppender:public LogAppender
    {
    public:
        ConsoleAppender():out(std::cout){}
        virtual int open(){return 0;}
        virtual void close(){return;}
        virtual void append(const std::string & str)
        {
            out<<str;
        }
        virtual void flush()
        {
            out.flush();
        }
    protected:
        std::ostream & out;
    };

    class FileAppender:public LogAppender
    {
    public:
        FileAppender(const std::string& filepath)
            :fstream(nullptr),path(filepath){}
        ~FileAppender()
        {
            delete fstream;
            fstream=nullptr;
        }
        virtual int open()
        {
            fstream=new std::ofstream(path,std::ios::out|std::ios::app);
            if(!fstream->good())
                return 0;
            else
                return 1;
        }

        virtual void close()
        {
            fstream->close();
        }

        virtual void append(const std::string & str)
        {
            (*fstream)<<str;
        }
        virtual void flush()
        {
            fstream->flush();
        }
    protected:
        std::ofstream *fstream;
        std::string path;
    };
	
	class DailyRollingFileAppender :public LogAppender
	{
	public:
		DailyRollingFileAppender(const std::string& filepath)
			:fstream(nullptr), path(filepath) {}
		~DailyRollingFileAppender()
		{
			delete fstream;
			fstream = nullptr;
		}
		virtual int open()
		{
			assert(fstream == nullptr);
			//record current day
			std::string new_path= getPathWithoutSuffix(path) + Time::getTime() + getPathSuffix(path);
			fstream = new std::ofstream(new_path, std::ios::out | std::ios::app);
			if (fstream->good())
			{
				day = Time::getDay();
				return 0;
			}
			else
			{
				delete fstream;
				fstream = nullptr;
				return 1;
			}
		}

		virtual void close()
		{
			assert(fstream!=nullptr);
			fstream->close();
			delete fstream;
			fstream = nullptr;
		}

		virtual void append(const std::string & str)
		{
			if (Time::getDay() != day)
			{
				close();
				open();//TODO cerr
			}
			assert(fstream!=nullptr);
			(*fstream) << str;
		}
		virtual void flush()
		{
			assert(fstream != nullptr);
			fstream->flush();
		}
	protected:
		std::ofstream *fstream;
		std::string path;
		int day;
	};
	
	class RollingFileAppender :public LogAppender
	{
	public:
		RollingFileAppender(const std::string& filepath,uint64_t size)
			:fstream(nullptr), path(filepath),max_size(size),cur_size(0) {}
		~RollingFileAppender()
		{
			delete fstream;
			fstream = nullptr;
		}
		virtual int open()
		{
			assert(fstream == nullptr);
			//record current day
			cur_path = getPathWithoutSuffix(path) + Time::getTime() + getPathSuffix(path);
			fstream = new std::ofstream(cur_path, std::ios::out | std::ios::app);
			if (fstream->good())
			{
				cur_size = getFileSize(cur_path);
				return 0;
			}
			else
			{
				delete fstream;
				fstream = nullptr;
				return 1;
			}
		}

		virtual void close()
		{
			assert(fstream != nullptr);
			fstream->close();
			delete fstream;
			fstream = nullptr;
		}

		virtual void append(const std::string & str)
		{
			if (getFileSize(cur_path) >= max_size)
			{
				close();
				open();//TODO cerr
			}
			assert(fstream != nullptr);
			(*fstream) << str;
		}
		virtual void flush()
		{
			assert(fstream != nullptr);
			fstream->flush();
		}
	protected:
		std::ofstream *fstream;
		std::string path;
		std::string cur_path;
		uint64_t max_size;
		uint64_t cur_size;
	};
}

#endif // RAPIDLOGGER_LOGAPPENDER_H
