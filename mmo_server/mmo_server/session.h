/*By multiapple Date:2015/8/21*/
#ifndef MMO_SERVER_SESSION
#define MMO_SERVER_SESSION
#include<WinSock2.h>
#include<Windows.h>
#include"buffer.h"
#include"selector.h"
#define PER_SESSION_SOCK 1024
namespace server
{
	class session
	{
	private:
		int _load;
		SOCKET _sock[PER_SESSION_SOCK];
		HANDLE _thread;
	public:
		session() :_load(0)
		{
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ioloop(this), NULL, NULL, NULL);
		};
		bool addsocket(SOCKET s)
		{
			if (s != NULL){
				_sock[_load] = s;
				_load++;
				return true;
			}
			return false;
		}
		int getload(){ return _load; }
		friend DWORD WINAPI ioloop();
	};

	DWORD WINAPI ioloop(session* s)
	{
		ioctlsocket
		for (int i = 0; i < s->getload(); i++){
			recv(
		}
	}

	};
}
#endif