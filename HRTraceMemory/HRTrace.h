#pragma once

#include <map>

#ifndef NDEBUG

void *operator new(size_t size, const char* file, long line);
void operator delete(void* p);

void *operator new[](size_t size, const char* file, long line);
void operator delete[](void* p);

class HRTrace
{
private:
	class TraceInfo
	{
	public:
		TraceInfo(const char* file = 0, long line = 0) :_file(file), _line(line){}
		const char* File() const
		{
			return _file;
		}
		long Line() const
		{
			return _line;
		}
		
	private:
		const char* _file;
		long _line;
	};

public:
	HRTrace();
	~HRTrace();

	void Add(void* p, const char* file, long line);
	void Remove(void* p);
	void Dump();
	static bool bReady;
private:
	std::map<void*, TraceInfo> mapTraceInfo;
};

#endif