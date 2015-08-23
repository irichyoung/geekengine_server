#include<WinSock2.h>
#include<Windows.h>
#include<iostream>
#include<map>
#include"entity.h"
#include"buffer.h"

using namespace std;

#define address "127.0.0.1"
#define port  6666
#define MAX_CLIENT 30

#pragma comment(lib,"ws2_32.lib")

SOCKET connect_socket[MAX_CLIENT];
fd_set write_fd, read_fd;
map<SOCKET, server::buffer> connector;
int main()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	SOCKET fdserver = socket(AF_INET, SOCK_STREAM, IPPORT_FTP);
	SOCKET accept_socket;

	struct sockaddr_in server,client;
	int sockaddr_len = sizeof(client);
	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = inet_addr(address);
	server.sin_port = htons(port);

	if (bind(fdserver, (sockaddr*)&server, sizeof(server)) == -1){
		cout << "服务器绑定失败" << endl;
	}

	if (listen(fdserver, 4) == -1){
		cout << "服务器监听失败" << endl;
	}

	HANDLE handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)read_message_loop, NULL, 0, 0);
	if (handle == NULL){
		cout << "创建读处理线程时失败" << endl;
	}

	while (TRUE){
		static int count = 0;
		if (accept_socket = accept(fdserver, (sockaddr*)&client, &sockaddr_len) != NULL){
			cout << inet_ntoa(client.sin_addr) << " " << ntohs(client.sin_port) << " 接入系统" << endl;
			int n = ioctlsocket(accept_socket, FIONBIO, (u_long *)1);
			if (n != 0){
				cout << "设置非阻塞socket时失败" << endl;
			}
			connect_socket[count] = accept_socket;
			connector[accept_socket] = server::buffer();
			count++;
		}
	}
}

DWORD WINAPI read_message_loop()
{
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 100;
	for (int i = 0; i < MAX_CLIENT; i++){
		FD_ZERO(&read_fd);
		FD_SET(connect_socket[i], &read_fd);
	}
	select(0, &read_fd, NULL, NULL, &timeout);
	for (int i = 0; i < MAX_CLIENT; i++){
		if (FD_ISSET(connect_socket[i], &read_fd)){
			char temp[256];
			int n = recv(connect_socket[i], (char*)&temp, sizeof(temp), NULL);
			connector[connect_socket[i]] << temp;
			connector[connect_socket[i]]
		}
	}
}