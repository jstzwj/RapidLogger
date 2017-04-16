


/*
    © 2016-2017 mocores
    This file is distributed under the MIT liscense.
    See LICENSE for details.
*/
#ifndef RAPIDLOGGER_LOGGER_H
#define RAPIDLOGGER_LOGGER_H

#include<string>
#include<sstream>
#include<queue>
#include<chrono>
#include<exception>
#include<ostream>
#include<memory>

#include<thread>
#include<mutex>


#include<cassert>
#include<ctime>

#include"LogMsg.h"
#include"LogLayout.h"
#include"LogAppender.h"
#include"LogFilter.h"
#include"Timer.h"
#include"container/linkedblockingqueue.h"

namespace rapidlogger
{

#define logOff(msg) off(msg,__FILE__,__FUNCTION__,__LINE__)
#define logFatal(msg) fatal(msg,__FILE__,__FUNCTION__,__LINE__)
#define logError(msg) error(msg,__FILE__,__FUNCTION__,__LINE__)
#define logWarn(msg) warn(msg,__FILE__,__FUNCTION__,__LINE__)
#define logInfo(msg) info(msg,__FILE__,__FUNCTION__,__LINE__)
#define logDebug(msg) debug(msg,__FILE__,__FUNCTION__,__LINE__)
#define logAll(msg) all(msg,__FILE__,__FUNCTION__,__LINE__)
#define logCustomer(msg,level) costumer(msg,__FILE__,__FUNCTION__,__LINE__,level)

    /*!
     * \brief The Logger class
     */
    class Logger
    {
    public:
        using TimePoint=std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>;
        using AppenderPtr=std::shared_ptr<LogAppender>;
        Logger()
            :log_queue(1*1000000),running(false),log_thread(nullptr),appender(new ConsoleAppender){}
        virtual ~Logger()
        {
			end();
        }
        Logger(const Logger &other)=delete;
        Logger operator =(const Logger &other)=delete;
        void setName(const char * name)
        {
            assert(running==false);
            logger_name=name;
        }
        void setLayout(const LogLayout& _layout)
        {
            assert(running==false);
            layout=_layout;
        }
		void setFilter(const LogFilter& _filter)
		{
			assert(running == false);
			filter = _filter;
		}
        bool configure()
        {
            assert(running==false);
            return true;
        }
        bool configureFromMem(const char *)
        {
            assert(running==false);
            return true;
        }
        bool configureFromFile(const char *)
        {
            assert(running==false);
            return true;
        }
		void log(const ::std::string & msg, const char * filename, const char * function, int line, const LogLevel& level)
		{
			thread_local std::string thread_id="";
			if (thread_id.empty())
			{
				std::stringstream stream;
				stream << ::std::this_thread::get_id();
				thread_id = stream.str();
			}
			if (filter.isInRange(level))
			{
				LogMsg logmsg(level, msg, thread_id, filename, function, line);
				log_queue.put(layout.getMsgText(logmsg));
			}
		}
        void off(const ::std::string & msg,const char * filename, const char * function,int line)
        {
			log(msg,filename,function,line,OffLevel());
        }

        void fatal(const ::std::string & msg, const char * filename, const char * function, int line)
        {
			log(msg, filename, function, line, FatalLevel());
        }
        void error(const ::std::string & msg, const char * filename, const char * function, int line)
        {
			log(msg, filename, function, line, ErrorLevel());
        }
        void warn(const ::std::string & msg, const char * filename, const char * function, int line)
        {
			log(msg, filename, function, line, WarnLevel());
        }
        void info(const ::std::string & msg, const char * filename, const char * function, int line)
        {
			log(msg, filename, function, line, InfoLevel());
        }
        void debug(const ::std::string & msg, const char * filename, const char * function, int line)
        {
			log(msg, filename, function, line, DebugLevel());
        }
        void all(const ::std::string & msg, const char * filename, const char * function, int line)
        {
			log(msg, filename, function, line, AllLevel());
        }
		void customer(const ::std::string & msg, const char * filename, const char * function, int line,const LogLevel& level)
		{
			log(msg, filename, function, line, level);
		}

        void start()
        {
            assert(running==false);
            running=true;
            start_time=std::chrono::system_clock::now();
            layout.setStartTime(start_time);
            appender->open();

            log_thread=new std::thread(std::bind(&Logger::run, this));
        }
        void end()
        {
            {
              std::unique_lock<std::mutex> lock(queue_mtx);
              running = false;
            }
            assert(log_thread!=nullptr);
            log_thread->join();
            delete log_thread;
            log_thread=nullptr;
			//flush buffer
			appender->append(buffer);
			appender->flush();
			buffer.clear();
			appender->close();
        }
        /*!
         * \brief setAppender
         * \param app
         * \note We will delete appender. The Appender must be created in heap.
         */
        template<class T>
        void setAppender(const T& app)
        {
            assert(running==false);
            appender=std::make_shared<T>(app);
        }
    protected:
        void run()
        {
            try
            {
              buffer.reserve(1024*1024*4);
			  clock_t last_time = clock();
              while (running||log_queue.size()!=0)
              {
				  clock_t now_time = clock();
				  if (now_time - last_time >= 3 * 1000)
				  {
					  appender->append(buffer);
					  appender->flush();
					  buffer.clear();
					  last_time = now_time;
				  }
				  std::string eachmsg;
				  if(log_queue.poll(eachmsg)==true)
				  {
					  if (buffer.length()>1024 * 1024 * 3)
					  {
						  appender->append(buffer);
						  appender->flush();
						  buffer.clear();
					  }
					  buffer += eachmsg;
				  }
                  std::this_thread::sleep_for(std::chrono::nanoseconds(0));
              }
            }
            catch (const std::exception& ex)
            {
                std::cerr<<"exception caught in Logger %s\n"<<logger_name<<std::endl;
                std::cerr<<"reason: %s\n"<<ex.what()<<std::endl;
                abort();
            }
            catch (...)
            {
                std::cerr<<"unknown exception caught in Logger %s\n"<<logger_name<<std::endl;
                throw; // rethrow
            }
        }

    protected:
        std::string logger_name;
        LinkedBlockingQueue<std::string> log_queue;
        std::mutex queue_mtx;
        bool running;
		std::string buffer;
        TimePoint start_time;
        std::thread * log_thread;
        LogLayout layout;
        AppenderPtr appender;
		LogFilter filter;
    };
}


#endif // RAPIDLOGGER_LOGGER_H
