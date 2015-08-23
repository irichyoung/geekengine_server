#ifndef MMO_SERVER_LOGIC
#define MMO_SERVER_LOGIC

#include<string>
#include"commands.h"
#include"entity.h"
#include<map>
using namespace std;
namespace server
{
	class logic
	{
	private:
		map<long int, server::entity>entity_m;
	public:
		enum Message_Type{
			LOGIN         = 1,
			MOVE          = 2,
			ATTACK        = 3,
			BUY           = 4
		};
		bool message_handle(std::string message, Message_Type M_Type);
	};
}
#endif