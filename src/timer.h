#pragma once
#ifndef RAPIDLOGGER_TIMER
#define RAPIDLOGGER_TIMER

#include<queue>
#include<vector>
#include<map>
#include<list>
#include<functional>
#include<memory>
#include<mutex>
#include<chrono>

#include<cassert>

#include"Timestamp.h"

namespace rapidlogger
{
	class SequenceId
	{
	public:
		SequenceId()
			:id(0){}
		uint64_t incrementAndGet()
		{
			std::unique_lock<std::mutex> lock(mtx);
			uint64_t tmp = id;
			id++;
			return tmp;
		}
		uint64_t get()
		{
			std::unique_lock<std::mutex> lock(mtx);
			return id;
		}
	private:
		uint64_t id;
		std::mutex mtx;
	};

	class Timer
	{
	public:
		using TimerCallback = std::function<void()>;

		Timer(const TimerCallback& cb, Timestamp when, uint64_t interval=0)
			: callback(cb),
			stop_time(when),
			interval(interval),
			repeat(interval > 0),
			sequence(seq_id.incrementAndGet())
		{ }
		
		void run() const
		{
			callback();
		}
		Timestamp getStopTime() const { return stop_time; }
		bool getRepeat() const { return repeat; }
		int64_t getSequence() const { return sequence; }
		void restart(Timestamp now)
		{
			if (repeat)
			{
				stop_time = now+ interval;
			}
			else
			{
				stop_time = Timestamp::invalid();
			}
		}
	private:
		const TimerCallback callback;
		Timestamp stop_time;
		const uint64_t interval;
		const bool repeat;
		const uint64_t sequence;
		static SequenceId seq_id;
	};
	SequenceId Timer::seq_id;
	class TimerTask
	{
	public:
		TimerTask() :running(false), interval(1000){}
		~TimerTask() 
		{
			delete loop;
		}
		void add(const Timer & timer) 
		{
			std::unique_lock<std::mutex> lock(mtx);
			regTimer(timer);
		}
		void remove(uint64_t id)
		{
			std::unique_lock<std::mutex> lock(mtx);
			delTimer(id);
		}
		void start()
		{
			assert(running==false);
			running = true;
			loop=new std::thread(std::bind(&TimerTask::timerLoop, this));
		}
		void timerLoop()
		{
			while (running==true)
			{
				std::unique_lock<std::mutex> lock(mtx);
				if (timer_list.front().getStopTime()>Timestamp::now())
				{
					Timer t = timer_list.front();
					t.run();
					t.restart(Timestamp::now());
					timer_list.erase(timer_list.begin());
					if (t.getStopTime().isValid()==true)
					{
						regTimer(t);
					}
				}
				std::this_thread::sleep_for(std::chrono::microseconds(interval));
			}
		}

	private:
		std::list<Timer> timer_list;
		std::mutex mtx;
		std::thread *loop;
		bool running;
		uint64_t interval;

		void regTimer(const Timer & timer)
		{
			std::list<Timer>::iterator i;
			for ( i= timer_list.begin(); i != timer_list.end(); ++i)
			{
				if (i->getStopTime()>=timer.getStopTime())
				{
					timer_list.insert(i,timer);
					break;
				}
			}
		}
		void delTimer(uint64_t id)
		{
			std::list<Timer>::iterator i;
			for (i = timer_list.begin(); i != timer_list.end(); ++i)
			{
				if (i->getSequence()==id)
				{
					timer_list.erase(i);
					break;
				}
			}
		}
	};
}

#endif // !RAPIDLOGGER_TIMER
