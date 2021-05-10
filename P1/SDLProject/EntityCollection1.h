#include "Entity.h"
#include "Map.h"
class EntityCollection {
public:
	Player* player;
	std::vector<Unit*> FriendLst;
	std::vector<Unit*> NPCList;//maybe the size of this should be set before the game in the future
	std::vector<Block*> blockList;
	std::vector<Projectile*> projectileList;
	Map* map;
	bool switchScene;
	//void buildPlatform();
	
};