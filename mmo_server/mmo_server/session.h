/*By irichyoung */
/*One thread,one session.A session contains some talk.Every talk contains some socket and a WSAEVENT*/
#ifndef MMO_SERVER_SESSION
#define MMO_SERVER_SESSION
#include<WinSock2.h>
#include<iostream>
#include<Windows.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
#define PER_SESSION_SOCK  1024
#define PER_SESSION_TALK    16
#define PER_TALK_SOCK       64
namespace server
{
	class talk
	{
	private:
		int           _load;
		SOCKET    _sock[64];
	public:
		int getload(){
			return _load;
		}
		talk() :_load(0){};
		friend class session;
		friend DWORD WINAPI ioloop(LPVOID);
		friend bool cleanup(int talk, int sock, session*);
	};
	class session
	{
	private:
		int			_talk_num;
		int                         _load;
		talk      _talk[PER_SESSION_TALK];
		WSAEVENT _event[PER_SESSION_TALK];
		HANDLE                    _thread;
	public:
		session() :_load(0), _talk_num(0)
		{
			_thread = CreateThread(NULL, NULL, ioloop, (LPVOID)this, NULL, NULL);
		};
		~session(){
			CloseHandle(_thread);
		}
		bool addsocket(SOCKET s);
		bool cleanup(int, int);
		int gettalknum(){ return _talk_num; }
		int getload(){ return _load; }
		friend DWORD WINAPI ioloop(LPVOID);
	};
}
#endif