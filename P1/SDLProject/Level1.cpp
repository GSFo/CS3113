#include "Level1.h"


#pragma once

#define LEVEL1_WIDTH 56
#define LEVEL1_HEIGHT 12
unsigned int level1_data[] =
{
1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,



0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0,


0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,


0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,


0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,

0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0,


0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0,


0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0,


0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0,

1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
};





void Level1::Initialize(size_t lifeCount) {
	__entities.switchScene = false;
	__displayWindow = SDL_GL_GetCurrentWindow();

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);

	__program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
	__viewMatrix = glm::mat4(1.0f);
	__modelMatrix1 = glm::mat4(1.0f);
	__projectionMatrix1 = glm::ortho(-8.0f, 8.0f, -11.0f, 0.0f, -1.0f, 1.0f);
	__program.SetProjectionMatrix(__projectionMatrix1);
	__program.SetViewMatrix(__viewMatrix);
	__program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);

	glUseProgram(__program.programID);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);


	//load textures
	std::string a("Shadowball");
	__states.loadTexture(a, "images/Shadowball.png");
	a = "Haunter";
	__states.loadTexture(a, "images/Haunter.png");
	a = "ctg";
	__states.loadTexture(a, "images/Squirtle.jfif");
	a = "Waterball";
	__states.loadTexture(a, "images/Waterball.png");
	a = "Magnimon";
	__states.loadTexture(a, "images/Magnimon.png");
	a = "font1";
	__states.loadTexture(a, "fonts/font1.png");
	a = "Gengar";
	__states.loadTexture(a, "images/Gengar.png");


	//transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//intialize units and set attributes
	//player
	__entities.player = new Player(2,2,15, 1, -5, 0);
	__entities.FriendLst.push_back(__entities.player);
	__entities.player->setTexture("ctg",__states);
	__entities.player->bulletTexture = Utility::loadTexture("images/Waterball.png");
	
	//NPC
	spawnNPC(6);
	bossPtr=spawnBoss();
	bossPtr->shadowBallTexture = Utility::loadTexture("images/Shadowball.png");
	bossPtr->setBossptr(bossPtr);
	//Blocks
	//__entities.buildPlatform();
	//__entities.buildNPC2();
	__states.unitVertices = new float[12]{ -.5, -.5, .5, -.5, .5, .5, -.5, -.5, .5, .5, -.5, 0.5 };
	__states.modelUnitVertices = new float[12]{ -.5, -.5, .5, -.5, .5, .5, -.5, -.5, .5, .5, -.5, 0.5 };
	__states.unitTexCoords = new float[12]{ 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
	__states.modelTexCoords = new float[12]{ 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
	__states.state = GameStatus::GameState::GAME;
	GLuint mapTextureID = Utility::loadTexture("images/tileset.png");
	__entities.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
}

void Level1::Update(float deltaTime) {
	//check if win
	if (__states.state == GameStatus::GameState::GAME) {
		if (!__entities.player->isAlive()) {
			__entities.player->accel(-__entities.player->getV().x, 0, 0);
			__states.__GAMEOVER__();
			return;
		}
		bool winFlag = true;
		for (Unit* unit : __entities.NPCList) {
			if (unit->isAlive()) {
				winFlag = false;
			}
		}
		if (winFlag) {
			__states.__GAMEWIN__();
		}


		__entities.player->update(deltaTime, __entities.player, __entities.map);

		//__states.playerB->move();
		updateNPC(deltaTime);
		if (bossPtr != nullptr) {
			bossPtr->useShadowBall(__entities.projectileList, __entities.player->location->getX(), __entities.player->location->getY());
			bossPtr->useDash(__entities.player->location->getX(), __entities.player->location->getY());
			if (!__entities.player->isBoosted()) {
				bossPtr->activateWeaken(__entities.player);
			}
		}

		
		if (__entities.player->onBoarder(__entities.map)) {
			//__states.__GAMEOVER__();
		}
		//boardercheck
		//__states.playerB->onBoarder();
		
		if (bossPtr != nullptr && abs(bossPtr->location->getX() - __entities.player->location->getX()) < 6) {
			lockView = true;
		}
		if (bossPtr == nullptr) {
			lockView = false;
		}

		if (!lockView) {
			__viewMatrix = glm::mat4(1.0f);
				if (__entities.player->location->getLocation().x > 7) {
					__viewMatrix = glm::translate(__viewMatrix, glm::vec3(-__entities.player->location->getLocation().x, 0, 0));
				}
				else {
					__viewMatrix = glm::translate(__viewMatrix, glm::vec3(-7, 0, 0));
				}
		}
	}
}
void Level1::Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	__entities.map->Render(&__program);
	glVertexAttribPointer(__program.positionAttribute, 2, GL_FLOAT, false, 0, __states.unitVertices);
	glEnableVertexAttribArray(__program.positionAttribute);
	glVertexAttribPointer(__program.texCoordAttribute, 2, GL_FLOAT, false, 0, __states.unitTexCoords);
	glEnableVertexAttribArray(__program.texCoordAttribute);
	//draw items

	//MessageBox(0, "Prepared to render", "", MB_OK);
	


	__entities.player->render(__program, __viewMatrix, __modelMatrix1, __projectionMatrix1,__states);
	//__states.playerB->render();
	renderNPC();
	if (__states.state == GameStatus::GameState::WIN) {
		Utility::DrawTextGL(&__program, __states.textureIDs["font1"], "MISSION ACCOMPLISHED", 1.0f, -.5f, glm::vec3(-4, 2, 0) + glm::vec3(__entities.player->location->getX(), -5, 0));
	}
	if (__states.state == GameStatus::GameState::LOSE) {
		Utility::DrawTextGL(&__program, __states.textureIDs["font1"], "MISSION FAILED", 1.0f, -.5f, glm::vec3(-4, 2, 0) + glm::vec3(__entities.player->location->getX(), -5, 0));
	}
	//if (__states.state == GameStatus::GameState::GAME) {
		//Utility::DrawTextGL(&__program, __states.textureIDs["font1"], "TIME TO FIGHT BACK! Life: "+std::to_string(__entities.player->lifeCount), .6f, -.3f, glm::vec3(-4, 1, 0) + glm::vec3(__entities.player->location->getX(), 0, 0));
		//Utility::DrawTextGL(&__program, __states.textureIDs["font1"], "Press Space To Jump, Press E to activate POWER MODE to destroy the ghosts!", .2f, -.1f, glm::vec3(-4, 2, 0) + glm::vec3(__entities.player->location->getX(), 0, 0));

	//}

	glDisableVertexAttribArray(__program.positionAttribute);
	glDisableVertexAttribArray(__program.texCoordAttribute);

	SDL_GL_SwapWindow(__displayWindow);
}



void Level1::spawnNPC(size_t num) {
	for (int i = 0; i < num; ++i) {
		Unit* newNPC = new Haunter(4, 3, -6+i, 0);//need some other way to get the spawn location
		newNPC->setTexture("Haunter",__states);//need NPC image list in future 
		newNPC->accel(i%3+1, pow(-1, i), 0);//speed
		__entities.NPCList.push_back(newNPC);
	}

	for (int i = 0; i < num; ++i) {
		Unit* newNPC = new Haunter(20, 3, -6 + i, 0);//need some other way to get the spawn location
		newNPC->setTexture("Haunter", __states);//need NPC image list in future 
		newNPC->accel(i%3 + 1, pow(-1, i), 0);//speed
		__entities.NPCList.push_back(newNPC);
	}

	for (int i = 0; i < num; ++i) {
		Unit* newNPC = new Haunter(30, 3, -6 + i, 0);//need some other way to get the spawn location
		newNPC->setTexture("Haunter", __states);//need NPC image list in future 
		newNPC->accel(i%3 + 1, pow(-1, i), 0);//speed
		__entities.NPCList.push_back(newNPC);
	}
}

Gengar* Level1::spawnBoss() {
	Gengar* boss = new Gengar(50,-5,0);
	boss->setTexture("Gengar", __states);
	boss->size = glm::vec3(3,3,0);
	__entities.NPCList.push_back(boss);
	return boss;
}

void Level1::updateNPC(float time) {
	for (size_t i = 0; i < __entities.NPCList.size();++i) {
		Unit* NPC = __entities.NPCList[i];
		NPC->update(time, __entities.player, __entities.map);
		if (!NPC->isAlive()) {
			if (NPC->getType() == UnitType::Gengar) {
				bossPtr = nullptr;
			}
			delete NPC;
			__entities.NPCList.erase(__entities.NPCList.begin() + i);
		}
	}
	for (Block* block : __entities.blockList) {
		block->update(time, __entities.player, __entities.map);
	}
	for (size_t i = 0; i < __entities.projectileList.size();++i) {
		Projectile* proj = __entities.projectileList[i];
		if (proj->getType() == ProjType::Friendly) {
			proj->update(time, __entities.NPCList, __entities.map);
		}
		else {
			proj->update(time, __entities.FriendLst, __entities.map);
		}
		if (!proj->isAlive()) {
			delete proj;
			__entities.projectileList.erase(__entities.projectileList.begin()+i);
		}
	}
}

void Level1::renderNPC() {//and blocks
	for (Unit* NPC : __entities.NPCList) {
		NPC->render(__program, __viewMatrix, __modelMatrix1, __projectionMatrix1,__states);
	}

	for (Block* block : __entities.blockList) {
		block->render(__program, __viewMatrix, __modelMatrix1, __projectionMatrix1,__states);
	}

	for (Projectile* proj : __entities.projectileList) {
		proj->render(__program, __viewMatrix, __modelMatrix1, __projectionMatrix1, __states);

	}
}
/*
void EntityCollection::buildPlatform() {
	Block* newBlock = new Block(BlockType::Platform, 0, -7, 0);
	newBlock->setTexture("Squirtle", __states,true);//need NPC image list in future
	newBlock->size.x = 20;
	blockList.push_back(newBlock);
}
*/

/*
void EntityCollection::buildNPC2() {
	bool isDeadly = false;
	for (float x = -8; x < 10; x += 7) {
		if (isDeadly) {
			Block* newBlock = new Gasty(x, 0, 0);
			newBlock->setTexture("Gasty",__states);
			NPCList.push_back(newBlock);
		}

		else {
			Block* newBlock = new Magniton(x, 0, 0);
			newBlock->setTexture("Magnimon",__states);
			NPCList.push_back(newBlock);
		}

		isDeadly = !isDeadly;
	}
}
*/

void Level1::playerShoot() {
	__entities.player->shoot(__entities.projectileList);
}