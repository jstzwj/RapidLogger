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
#include<vector>
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
#include"Singleton.h"
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

#define logOff_d(msg) getLogger().off(msg,__FILE__,__FUNCTION__,__LINE__)
#define logFatal_d(msg) getLogger().fatal(msg,__FILE__,__FUNCTION__,__LINE__)
#define logError_d(msg) getLogger().error(msg,__FILE__,__FUNCTION__,__LINE__)
#define logWarn_d(msg) getLogger().warn(msg,__FILE__,__FUNCTION__,__LINE__)
#define logInfo_d(msg) getLogger().info(msg,__FILE__,__FUNCTION__,__LINE__)
#define logDebug_d(msg) getLogger().debug(msg,__FILE__,__FUNCTION__,__LINE__)
#define logAll_d(msg) getLogger().all(msg,__FILE__,__FUNCTION__,__LINE__)
#define logCustomer_d(msg,level) getLogger().costumer(msg,__FILE__,__FUNCTION__,__LINE__,level)

#define LOGGER_BUFFER_SIZE 4*1024*1024
	/*!
	 * \brief The Logger class
	 */
	class Logger
	{
	public:
		using TimePoint = std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>;
		using AppenderPtr = std::shared_ptr<LogAppender>;
		Logger()
			:log_queue(1 * 1000000), running(false), log_thread(nullptr)
		{
			appender.push_back(std::shared_ptr<LogAppender>(new ConsoleAppender));
		}
		virtual ~Logger()
		{
			end();
		}
		Logger(const Logger &other) = delete;
		Logger operator =(const Logger &other) = delete;
		void setName(const char * name)
		{
			assert(running == false);
			logger_name = name;
		}
		void setLayout(const LogLayout& _layout)
		{
			assert(running == false);
			layout = _layout;
		}
		void setFilter(const LogFilter& _filter)
		{
			assert(running == false);
			filter = _filter;
		}
		bool configure()
		{
			assert(running == false);
			return true;
		}
		bool configureFromMem(const char *)
		{
			assert(running == false);
			return true;
		}
		bool configureFromFile(const char *)
		{
			assert(running == false);
			return true;
		}
		void log(const ::std::string & msg, const char * filename, const char * function, int line, const LogLevel& level)
		{
			thread_local std::string thread_id = "";
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
		void off(const ::std::string & msg, const char * filename, const char * function, int line)
		{
			log(msg, filename, function, line, OffLevel());
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
		void customer(const ::std::string & msg, const char * filename, const char * function, int line, const LogLevel& level)
		{
			log(msg, filename, function, line, level);
		}

		void start()
		{
			assert(running == false);
			running = true;
			start_time = std::chrono::system_clock::now();
			layout.setStartTime(start_time);
			for (auto &each : appender)
				each->open();
			log_thread = new std::thread(std::bind(&Logger::run, this));
		}
		void end()
		{
			{
				std::unique_lock<std::mutex> lock(queue_mtx);
				running = false;
			}
			assert(log_thread != nullptr);
			log_thread->join();
			delete log_thread;
			log_thread = nullptr;
			//flush buffer
			for (auto &each : appender)
			{
				each->append(buffer);
				each->flush();
				each->close();
			}
			buffer.clear();
		}
		/*!
		 * \brief setAppender
		 * \param app
		 * \note We will delete appender. The Appender must be created in heap.
		 */
		template<class T>
		void addAppender(const T& app)
		{
			assert(running == false);
			appender.push_back(std::make_shared<T>(app));
		}
		void removeLastAppender()
		{
			assert(running == false);
			appender.pop_back();
		}
		void clearAppender()
		{
			assert(running == false);
			appender.clear();
		}
	protected:
		void run()
		{
			try
			{
				buffer.reserve(LOGGER_BUFFER_SIZE);
				clock_t last_time = clock();
				while (running || log_queue.size() != 0)
				{
					clock_t now_time = clock();
					if (now_time - last_time >= 3 * 1000)
					{
						for (auto &each : appender)
						{
							each->append(buffer);
							each->flush();
						}
						buffer.clear();
						last_time = now_time;
					}
					std::string eachmsg;
					if (log_queue.poll(eachmsg) == true)
					{
						if (buffer.length() > LOGGER_BUFFER_SIZE*0.8)
						{
							for (auto &each : appender)
							{
								each->append(buffer);
								each->flush();
							}
							buffer.clear();
						}
						buffer += eachmsg;
					}
					std::this_thread::sleep_for(std::chrono::nanoseconds(0));
				}
			}
			catch (const std::exception& ex)
			{
				std::cerr << "exception caught in Logger %s\n" << logger_name << std::endl;
				std::cerr << "reason: %s\n" << ex.what() << std::endl;
				abort();
			}
			catch (...)
			{
				std::cerr << "unknown exception caught in Logger %s\n" << logger_name << std::endl;
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
		std::vector<AppenderPtr> appender;
		LogFilter filter;
	};
	template<int num>
	class InstanceNum
	{
	public:
		constexpr static int value = num;
		using type = InstanceNum<num>;
	};

	Logger & initRapidLogger(char *argv)
	{
		Logger& result = Singleton<std::pair<InstanceNum<0>, rapidlogger::Logger> >::getInstance().second;
		result.setName(argv);
		result.start();
		return result;
	}
	template<class AppenderType>
	Logger & initRapidLogger(char *argv, const AppenderType &appender)
	{
		Logger& result = Singleton<std::pair<InstanceNum<0>, rapidlogger::Logger> >::getInstance().second;
		result.setName(argv);
		result.clearAppender();
		result.addAppender(appender);
		result.start();
		return result;
	}
	template<class AppenderType>
	Logger & initRapidLogger(char *argv, const AppenderType &appender, const LogLayout & layout)
	{
		Logger& result = Singleton<std::pair<InstanceNum<0>, rapidlogger::Logger> >::getInstance().second;
		result.setName(argv);
		result.clearAppender();
		result.addAppender(appender);
		result.setLayout(layout);
		result.start();
		return result;
	}
	template<class AppenderType>
	Logger & initRapidLogger(char *argv, const AppenderType &appender, const LogLayout & layout,const LogFilter & filter)
	{
		Logger& result = Singleton<std::pair<InstanceNum<0>, rapidlogger::Logger> >::getInstance().second;
		result.setName(argv);
		result.clearAppender();
		result.addAppender(appender);
		result.setLayout(layout);
		result.setFilter(filter);
		result.start();
		return result;
	}

	template<int instance>
	Logger & initRapidLogger(char *argv)
	{
		Logger& result = Singleton<std::pair<InstanceNum<instance>, rapidlogger::Logger> >::getInstance().second;
		result.setName(argv);
		result.start();
		return result;
	}
	template<class AppenderType, int instance>
	Logger & initRapidLogger(char *argv, const AppenderType &appender)
	{
		Logger& result = Singleton<std::pair<InstanceNum<instance>, rapidlogger::Logger> >::getInstance().second;
		result.setName(argv);
		result.clearAppender();
		result.addAppender(appender);
		result.start();
		return result;
	}
	template<class AppenderType, int instance>
	Logger & initRapidLogger(char *argv, const AppenderType &appender, const LogLayout & layout)
	{
		Logger& result = Singleton<std::pair<InstanceNum<instance>, rapidlogger::Logger> >::getInstance().second;
		result.setName(argv);
		result.clearAppender();
		result.addAppender(appender);
		result.setLayout(layout);
		result.start();
		return result;
	}
	template<class AppenderType, int instance>
	Logger & initRapidLogger(char *argv, const AppenderType &appender, const LogLayout & layout,const LogFilter & filter)
	{
		Logger& result = Singleton<std::pair<InstanceNum<instance>, rapidlogger::Logger> >::getInstance().second;
		result.setName(argv);
		result.clearAppender();
		result.addAppender(appender);
		result.setLayout(layout);
		result.setFilter(filter);
		result.start();
		return result;
	}

	void closeRapidLogger()
	{
		Logger& result = Singleton<rapidlogger::Logger>::getInstance();
		result.end();
	}

	Logger & getLogger()
	{
		Logger& result = Singleton<std::pair<InstanceNum<0>, rapidlogger::Logger> >::getInstance().second;
		return result;
	}
	template<int instance>
	Logger & getLogger()
	{
		Logger& result = Singleton<std::pair<InstanceNum<instance>, rapidlogger::Logger> >::getInstance().second;
		return result;
	}
}

#endif // RAPIDLOGGER_LOGGER_H
