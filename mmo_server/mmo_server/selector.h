/*By multiapple Date:2015/8/22*/
#ifndef MMO_SERVER_SELECTOR
#define MMO_SERVER_SELECTOR

#include <Windows.h>
#include<WinSock2.h>
#include"session.h""
#include<vector>
namespace server
{
	class selector
	{
	private:
		SOCKET accept_sock;
		SOCKET listen_sock;
		vector<session>channel;
	public:
		bool start();


	};
}
#endif