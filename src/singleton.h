#ifndef RAPIDLOGGER_SINGLETON_H
#define RAPIDLOGGER_SINGLETON_H

#include<atomic>
#include<functional>
#include<thread>
#include<mutex>

namespace rapidlogger
{
    template<class T>
    class Singleton
    {
    public:
        Singleton()=delete;
        ~Singleton();
        Singleton(const Singleton &other)=delete;
        Singleton & operator =(const Singleton &other)=delete;
        static T& getInstance()
        {
			static Gc4Instance instance_gc;
            static std::once_flag oc;
            std::call_once(oc,[&]{value=new T();});
            return *value;
        }
		//used to release singleton
		class Gc4Instance
		{
		public:
			Gc4Instance() = default;
			~Gc4Instance()
			{
				if (Singleton<T>::value!=nullptr)
					delete Singleton<T>::value;
			}
		};
     private:
        static T* value;
    };
    template<class T>
    T * Singleton<T>::value(nullptr);
}


#endif // RAPIDLOGGER_SINGLETON_H
