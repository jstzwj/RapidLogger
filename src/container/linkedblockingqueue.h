#ifndef LINKEDBLOCKINGQUEUE_H
#define LINKEDBLOCKINGQUEUE_H
#include<queue>

#include<thread>
#include<mutex>
#include<condition_variable>

namespace rapidlogger
{
    template<class T>
    class LinkedBlockingQueue
    {
    public:
        LinkedBlockingQueue()
            :running(false),max_size(0){}
        LinkedBlockingQueue(int size)
            :running(false),max_size(size)
        {
            assert(running==false);
            running=true;
        }
        void put(const T& obj)
        {
            std::unique_lock<std::mutex> lock(mtx);
            while(isFull())
            {
                notFull.wait(lock);
            }
            data.push(obj);
            notEmpty.notify_one();
        }
        T take()
        {
            std::unique_lock<std::mutex> lock(mtx);
            while(isEmpty())
            {
                notEmpty.wait(lock);
            }
            T result=data.front();
            data.pop();
            notFull.notify_one();
            return result;
        }
		bool offer(const T& obj)
		{
			std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
			if (lock.try_lock()==true)
			{
				while (isFull())
				{
					notFull.wait(lock);
				}
				data.push(obj);
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
				obj = data.front();
				data.pop();
				notFull.notify_one();
				return true;
			}
			else
			{
				return false;
			}
		}
		int size()
		{
			std::unique_lock<std::mutex> lock(mtx);
			return data.size();
		}
    protected:
        std::mutex mtx;
        std::condition_variable notEmpty;
		std::condition_variable notFull;
        std::queue<T> data;
        bool running;
        int max_size;

        bool isEmpty()
        {
            return data.size()==0;
        }
        bool isFull()
        {
            if(max_size==0)
            {
                return false;
            }
            if((int)data.size()>=max_size)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    };
}
#endif // LINKEDBLOCKINGQUEUE_H
