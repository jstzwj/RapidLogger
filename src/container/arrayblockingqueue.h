#ifndef ARRAYBLOCKINGQUEUE_H
#define ARRAYBLOCKINGQUEUE_H

#include<vector>
#include<thread>
#include<mutex>
#include<condition_variable>

namespace rapidlogger
{
    template<class T>
    class ArrayBlockingQueue
    {
    public:
        ArrayBlockingQueue()
            :start(0),finish(0),data_size(1024),running(false){}
        ArrayBlockingQueue(int size)
            :start(0),finish(0),data_size(size),running(false)
        {
            assert(running==false);
            data.resize(size);
            running=true;
        }

        void put(const T& obj)
        {
            std::unique_lock<std::mutex> lock(mtx);
            while(isFull())
            {
                notFull.wait(lock);
            }
            data[(finish+1)%data_size]=obj;
            finish=(finish+1)%data_size;
            notEmpty.notify_one();
        }
        T take()
        {
            std::unique_lock<std::mutex> lock(mtx);
            T result;
            while(isEmpty())
            {
                notEmpty.wait(lock);
            }
            result=data[start];
            start=(start+1)%data_size;
            notFull.notify_one();
            return result;
        }
		bool offer(const T& obj)
		{
			std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
			if (lock.try_lock() == true)
			{
				while (isFull())
				{
					notFull.wait(lock);
				}
				data[(finish + 1) % data_size] = obj;
				finish = (finish + 1) % data_size;
				notEmpty.notify_one();
				return true;
			}
			else
			{
				return false;
			}
		}
		bool poll(T& obj)
		{
			std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
			if (lock.try_lock() == true)
			{
				while (isEmpty())
				{
					notEmpty.wait(lock);
				}
				obj = data[start];
				start = (start + 1) % data_size;
				notFull.notify_one();
				return true;
			}
			else
			{
				return false;
			}
		}

    protected:
        std::mutex mtx;
		std::condition_variable notEmpty;
        std::condition_variable notFull;
        std::vector<T> data;
        bool running;
        int start;
        int finish;
        int data_size;

        bool isEmpty()
        {
            return start==finish;
        }
        bool isFull()
        {
            return (start+1)%data_size==finish;
        }
    };
}


#endif // ARRAYBLOCKINGQUEUE_H
