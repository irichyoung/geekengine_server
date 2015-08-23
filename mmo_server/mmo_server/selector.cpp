#include"selector.h"
using namespace server;
bool selector::start()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	listen_sock = socket(AF_INET, SOCK_STREAM, IPPORT_FTP);

	struct sockaddr_in server, client;
	int sockaddr_len = sizeof(client);
	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(6666);

	if (bind(listen_sock, (sockaddr*)&server, sizeof(server)) == -1){
		cout << "·þÎñÆ÷°ó¶¨Ê§°Ü" << endl;
		return false;
	}

	if (listen(fdserver, 4) == -1){
		cout << "·þÎñÆ÷¼àÌýÊ§°Ü" << endl;
		return false;
	}
}