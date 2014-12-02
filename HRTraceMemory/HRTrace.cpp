#include <iostream>
#include "HRTrace.h"

#ifndef NDEBUG
bool HRTrace::bReady = false;

HRTrace::HRTrace()
{
	bReady = true;
}


HRTrace::~HRTrace()
{
	bReady = false;
	Dump();
}

void HRTrace::Add(void* p, const char* file, long line)
{
	mapTraceInfo[p] = TraceInfo(file, line);
}

void HRTrace::Remove(void* p)
{
	std::map<void*, TraceInfo>::iterator iter;
	iter = mapTraceInfo.find(p);
	if (iter != mapTraceInfo.end())
	{
		mapTraceInfo.erase(iter);
	}
}

//打印泄露的信息
void HRTrace::Dump()
{
	if (mapTraceInfo.size() > 0)
	{
		std::cout << "*** Memory Leak ***" << std::endl;
		std::map<void*, TraceInfo>::iterator iter;
		for (iter = mapTraceInfo.begin(); iter != mapTraceInfo.end(); ++iter)
		{
			const char* file = iter->second.File();
			long line = iter->second.Line();
			int address = reinterpret_cast<int>(iter->first);
			std::cout << "Memory Leak Address:" << "0x" << std::hex << address << std::endl;
			std::cout << "Leak file path:" << file << std::endl;
			std::cout<<"Leak line:" << std::dec << line << std::endl;
		}
		std::cout << std::endl;
	}
}

HRTrace trace;

//重载operator new/delete new[]/delete[]
void* operator new(size_t size,const char* file,long line)
{
	void* p = malloc(size);
	if (HRTrace::bReady)
	{
		trace.Add(p, file, line);
	}
	return p;
}

void operator delete(void*p)
{
	if (HRTrace::bReady)
	{
		trace.Remove(p);
	}
	free(p);
}

void* operator new[](size_t size, const char* file, long line)
{
	void* p = malloc(size);
	if (HRTrace::bReady)
	{
		trace.Add(p, file, line);
	}
	return p;
}

void operator delete[](void* p)
{
	if (HRTrace::bReady)
	{
		trace.Remove(p);
	}
	free(p);
}

#endif