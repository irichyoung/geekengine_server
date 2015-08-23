/*By multiapple:2015.8.17*/
#include "buffer.h"
using namespace server;

bool buffer::append(void *data,size_t sizeInBytes)
{
	size_t start = producer_index;
	buff.resize(start + sizeInBytes);
	if (memcpy(&buff[start], data, sizeInBytes) != NULL){
		producer_index += sizeInBytes;
		return true;
	}
	else return false;
}

buffer& buffer::operator<< (UINT32 data)
{
	UINT32 toWrite = htonl(data);
	append(&toWrite, sizeof(data));
	return *this;
}

buffer& buffer::operator<< (UINT16 data)
{
	UINT16 toWrite = htons(data);
	append(&toWrite, sizeof(data));
	return *this;
}

buffer& buffer::operator<< (const char* data)
{
	size_t length = std::strlen(data);
	buff.resize(producer_index + length + sizeof(UINT32));
	append(&length, sizeof(UINT32));
	append((void*)data, length);
	return *this;
}

buffer& buffer::operator>> (UINT16 &data)
{
	UINT16 toRead = ntohs(data);
	data = *reinterpret_cast<UINT16 *>(&buff[consumer_index]);
	consumer_index += sizeof(data);
	if (consumer_index == producer_index){
		consumer_index = 0;
		producer_index = 0;
	}
	return *this;
}

buffer& buffer::operator>> (UINT32 &data)
{
	UINT16 toRead = ntohs(data);
	data = *reinterpret_cast<UINT32 *>(&buff[consumer_index]);
	consumer_index += sizeof(data);
	if (consumer_index == producer_index){
		consumer_index = 0;
		producer_index = 0;
	}
	return *this;
}

buffer& buffer::operator>> (char *data)
{
	UINT32 sizeInBytes;
	*this >> sizeInBytes;
	memcpy(data, &buff[consumer_index], sizeInBytes);
	consumer_index += sizeInBytes;
	data[sizeInBytes] = '\0';
	if (consumer_index == producer_index){
		consumer_index = 0;
		producer_index = 0;
	}
	return *this;
}

bool buffer::decode()
{

}