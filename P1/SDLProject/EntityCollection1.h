#include "Entity.h"
#include "Map.h"
class EntityCollection {
public:
	Player* player;
	std::vector<Unit*> NPCList;//maybe the size of this should be set before the game in the future
	std::vector<Block*> blockList;
	Map* map;
	bool switchScene;
	//void buildPlatform();
	//void buildNPC2();
};