#include<iostream>
#include<memory>
#include<fstream>
#include<Windows.h>
#include<string>
#include<time.h>
using namespace std;

#define TEST
#define MUTEX
#ifdef CRITICALSECTION
#define Mutex CRITICAL_SECTION
#define LOCK EnterCriticalSection
#define UNLOCK LeaveCriticalSection
#endif
#ifdef MUTEX
#define Mutex HANDLE
#define LOCK WaitForSingleObject
#define UNLOCK ReleaseMutex
#endif
class Lock
{
private:
	Mutex _mutex;
public:
	static int c,d;
	Lock(Mutex mutex):_mutex(mutex){
#ifdef MUTEX
		LOCK(_mutex,INFINITE);
#endif
#ifdef CRITICALSECTION
		LOCK(&_mutex);
#endif
	c++;
	}
	~Lock(){
#ifdef MUTEX
		UNLOCK(_mutex);
#endif
#ifdef CRITICALSECTION
		UNLOCK(&_mutex);
#endif
	d++;
	}
};
int Lock::c = 0;
int Lock::d = 0;
class Log
{
private:
	static Mutex m_mutex;
	static ofstream m_log;
	static Log* instance;
	Log& operator = (Log&);
	Log(Log&);
	Log(){
#ifdef CRITICALSECTION
		InitializeCriticalSection(&m_mutex);
#endif
	};
public:
	static Log& getinstance()
	{
		Lock m_lock(m_mutex);
		if(!m_log.is_open())
				m_log.open("server.log",ios::app);
		return *instance;
	}
	Log& operator << (string s)
	{
		Lock m_lock(m_mutex);
		SYSTEMTIME t;
		GetLocalTime(&t);
		char time[30];
		sprintf(time,"%d/%d/%d %d:%d:%d:%d ",t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds);
		m_log<<time<<s.c_str()<<endl;
		return *this;
	}
};
#ifdef CRITICALSECTION
Mutex Log::m_mutex;
#endif
#ifdef MUTEX
Mutex Log::m_mutex = CreateMutex(NULL,false,(LPCWSTR)"LOG");
#endif
Log* Log::instance = new Log();
ofstream Log::m_log;

#ifdef TEST
DWORD WINAPI thread_1()
{
	for(int i = 0;i < 20000;i++){
		Log::getinstance()<<"here is thread_1";
	}
	return 0;
}
DWORD WINAPI thread_2()
{
	for(int i = 0;i < 20000;i++){
		Log::getinstance()<<"here is thread_2";
	}
	return 0;
}
DWORD WINAPI thread_3()
{
	for(int i = 0;i < 20000;i++){
		Log::getinstance()<<"here is thread_3";
	}
	return 0;
}
DWORD WINAPI thread_4()
{
	for(int i = 0;i < 20000;i++){
		Log::getinstance()<<"here is thread_4";
	}
	return 0;
}
int main()
{
	HANDLE h1 = CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)&thread_1,NULL,NULL,NULL);
	HANDLE h2 = CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)&thread_2,NULL,NULL,NULL);
	HANDLE h3 = CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)&thread_3,NULL,NULL,NULL);
	HANDLE h4 = CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)&thread_4,NULL,NULL,NULL);
	WaitForSingleObject(h1,INFINITE);
	WaitForSingleObject(h2,INFINITE);
	WaitForSingleObject(h3,INFINITE);
	WaitForSingleObject(h4,INFINITE);
	cout<<Lock::c<<endl;
	cout<<Lock::d<<endl;
	return 0; 
}
#endif
