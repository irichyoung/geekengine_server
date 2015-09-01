/*By multiapple Date:2015/8/21*/
/*By irichyoung */
/*One thread,one session.A session contains some talk.Every talk contains some socket and a WSAEVENT*/
#ifndef MMO_SERVER_SESSION
#define MMO_SERVER_SESSION
#define TEST
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
		bool addsocket(SOCKET s)
		{
			if (INVALID_SOCKET == s)return false;
			for (int i = 0; i < PER_SESSION_TALK; i++){
				if (PER_TALK_SOCK == _talk[i].getload())continue;
				if (0 == _talk[i].getload()){
					_talk[i]._sock[i] = s;
					_talk[i]._load++;
					_talk_num++;
					_load++;
					_event[i] = WSACreateEvent();
					WSAEventSelect(s, _event[i], FD_READ | FD_CLOSE);
					break;
				}
				else {
					for (int j = 0; j < PER_TALK_SOCK; j++){
						if (_talk[i]._sock[j] != NULL)continue;
						_talk[i]._sock[j] = s;
						_talk[i]._load++;
						_load++;
						WSAEventSelect(s, _event[i], FD_READ | FD_CLOSE);
						break;
					}
					break;
				}
			}
			return true;
		}
		int gettalknum(){ return _talk_num; }
		int getload(){ return _load; }
		friend DWORD WINAPI ioloop(LPVOID);
		friend bool cleanup(int talk, int sock, session*);
	};

	bool cleanup(int talk, int sock, session* s)
	{
		closesocket(s->_talk[talk]._sock[sock]);
		cout << "out" << endl;
		int load = s->_talk[talk].getload();
		s->_talk[talk]._sock[sock] = s->_talk[talk]._sock[load];
		s->_talk[talk]._load--;
		s->_load--;
		return true;
	}

	DWORD WINAPI ioloop(LPVOID p)
	{
		session *s = (session*)p;
		int ret;
		while (1){
			ret = WaitForMultipleObjects(s->gettalknum(), s->_event, false, INFINITE);
			if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)continue;
			int index = ret - WSA_WAIT_EVENT_0;
			for (int i = index; i < s->gettalknum(); i++){
				ret = WaitForSingleObject(s->_event[i], 0);
				if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)continue;
				for (int j = 0; j < s->_talk[i].getload(); j++){
					WSANETWORKEVENTS networkevents;
					if (WSAEnumNetworkEvents(s->_talk[i]._sock[j], s->_event[i], &networkevents) == SOCKET_ERROR)continue;
					if (networkevents.lNetworkEvents & FD_READ){
						char buff[100];
						ZeroMemory(buff, sizeof(buff));
						ret = recv(s->_talk[i]._sock[j], buff, sizeof(buff), NULL);
						if (!buff)continue;
						if (ret == 0 || (ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)){
							cleanup(i, j, s);
							break;
						}
						cout << buff << endl;
						char hi[20] = "hi";
						char hello[20] = "hello";
						char who[40] = "I'm geekengine,your best friend";
						char guass[20] = "you guass?";
						char know[20] = "I don't know";
						if (!strcmp(buff, "hello"))send(s->_talk[i]._sock[j], hi, sizeof(hi), NULL);
						else if (!strcmp(buff, "hi"))send(s->_talk[i]._sock[j], hello, sizeof(hello), NULL);
						else if (!strcmp(buff, "who are you?"))send(s->_talk[i]._sock[j], who, sizeof(who), NULL);
						else if (!strcmp(buff, "how old are you?"))send(s->_talk[i]._sock[j], guass, sizeof(guass), NULL);
						else send(s->_talk[i]._sock[j], know, sizeof(know), NULL);
					}
					if (networkevents.lNetworkEvents & FD_CLOSE)cleanup(i, j, s);
				}
			}
		}
		return 0;
	}
#ifdef TEST
	int main()
	{
		WSADATA wsa;
		WSAStartup(MAKEWORD(2, 2), &wsa);

		SOCKET listensock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
		SOCKADDR_IN addr;
		addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(6000);
		int ret = bind(listensock, (sockaddr*)&addr, sizeof(addr));
		if (ret == SOCKET_ERROR)cout << "bind failed" << endl;
		ret = listen(listensock, 5);
		if (ret == SOCKET_ERROR)cout << "listen failed" << endl;
		session s;
		while (1){
			SOCKADDR_IN addr;
			int len = sizeof(addr);
			SOCKET datasock = accept(listensock, (sockaddr*)&addr, &len);
			if (s.addsocket(datasock))cout << "welcome " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << endl;
		}

	}
#endif
}
#endif
