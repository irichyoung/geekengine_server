#ifndef MMO_SERVER_ENTITY
#define MMO_SERVER_ENTITY

namespace server
{
	class entity
	{
	private:
		long int id;
		int x, y;
		int speed;
		int damage;
		int health;
	public:
		void move(int x, int y){ this->x = x; this->y = y; }
	};
}
#endif