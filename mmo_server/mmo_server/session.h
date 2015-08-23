/*By multiapple Date:2015/8/21*/
#ifndef MMO_SERVER_SESSION
#define MMO_SERVER_SESSION
#include<WinSock2.h>
#include<Windows.h>
#include"buffer.h"
#include"selector.h"
namespace server
{
	class session
	{
		friend class selector;
	private:
		SOCKET sockets[64];
		buffer buff[64];
		UINT32 session_id[64];
		UINT8 load;
		bool clean[64];
		bool used[64];
	public:
		session() :load(0)
		{
			memset(used, 0, sizeof(clean));
			for (int i = 0; i < 64; i++)clean[i] = 1;
		}
		void read_message_loop()
		{
			int i = 0;
			while (int n = load){
				if (used[i] == TRUE){

				}
			}
		}

	};
}
#endif