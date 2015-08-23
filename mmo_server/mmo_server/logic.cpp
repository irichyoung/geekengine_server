#include"logic.h"
using namespace server;
bool logic::message_handle(std::string message, Message_Type M_Type){
	switch (M_Type)
	{
	case LOGIN:
	{
	}
	case MOVE:
	{
		Move move;
		move.ParseFromString(message);
		entity_m[move.id].x = move.x;
		entity_m[move.id].y = move.y;
	}
	case ATTACK:
	{

	}
	case BUY:
	{

	}
	default:
		break;
	}
}