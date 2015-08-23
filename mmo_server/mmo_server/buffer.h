#ifndef MMO_SERVER_BUFFER
#define MMO_SERVER_BUFFER

#include"commands.h"
#include<vector>
#include<WinSock2.h>
using namespace std;
namespace server
{
	class buffer{
	private:
		size_t    producer_index;
		size_t    consumer_index;
		vector<char>    buff;

	public:
		buffer() :producer_index(0),consumer_index(0){}
		bool    append(void *, size_t){};
		buffer& operator >> (char*){};
		buffer& operator >> (UINT16&){};
		buffer& operator >> (UINT32&){};
		buffer& operator << (const char*){};
		buffer& operator << (UINT16){};
		buffer& operator << (UINT32){};
		bool      decode();
	};
}
#endif