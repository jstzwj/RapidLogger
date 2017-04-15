#pragma once
#ifndef RAPIDLOGGER_TIMESTAMP
#define RAPIDLOGGER_TIMESTAMP

#include<chrono>

#include<cstdint>

namespace rapidlogger
{
	class Timestamp
	{
	public:
		static Timestamp invalid() { return Timestamp(); }
		static Timestamp now()
		{
			Timestamp result;
			std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::milliseconds> tp
				= std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
			auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
			result.time_stamp=tmp.count();
			return result;
		}
		Timestamp() :time_stamp(0) {}
		Timestamp(uint64_t time) :time_stamp(time) {}
		Timestamp operator +(uint64_t timepart)
		{
			Timestamp result;
			result.time_stamp = time_stamp + timepart;
			return result;
		}
		Timestamp operator -(uint64_t timepart)
		{
			Timestamp result;
			result.time_stamp = time_stamp - timepart;
			return result;
		}
		Timestamp& operator +=(uint64_t timepart)
		{
			this->time_stamp += timepart;
			return *this;
		}
		Timestamp& operator -=(uint64_t timepart)
		{
			this->time_stamp -= timepart;
			return *this;
		}
		bool operator >(const Timestamp &other)
		{
			return this->time_stamp>other.time_stamp;
		}
		bool operator <(const Timestamp &other)
		{
			return this->time_stamp<other.time_stamp;
		}
		bool operator >=(const Timestamp &other)
		{
			return this->time_stamp>=other.time_stamp;
		}
		bool operator <=(const Timestamp &other)
		{
			return this->time_stamp<=other.time_stamp;
		}
		bool operator ==(const Timestamp &other)
		{
			return this->time_stamp==other.time_stamp;
		}
		bool operator !=(const Timestamp &other)
		{
			return this->time_stamp!=other.time_stamp;
		}
		bool isValid() const { return time_stamp > 0; }
	private:
		uint64_t time_stamp;
	};
}
#endif // !RAPIDLOGGER_TIMESTAMP
