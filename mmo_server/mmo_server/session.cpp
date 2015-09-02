#include"session.h"
using namespace server;
bool session::addsocket(SOCKET s)
{
	if (INVALID_SOCKET == s)return false;
	if (this->_load == PER_SESSION_SOCK){
		char buff[20] = "server is overload";
		send(s, buff, sizeof(buff), NULL);
		return false;
	}
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
bool session::cleanup(int talk, int sock)
{
	closesocket(_talk[talk]._sock[sock]);
	cout << "out" << endl;
	int load = _talk[talk].getload();
	_talk[talk]._sock[sock] = _talk[talk]._sock[load];
	_talk[talk]._load--;
	_load--;
	return true;
}
namespace server
{
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