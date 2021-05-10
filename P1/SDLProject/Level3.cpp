#include "Level3.h"


#pragma once

#define LEVEL3_WIDTH 14
#define LEVEL3_HEIGHT 5
unsigned int level3_data[] =
{
0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 2, 2,
2, 2, 0, 0, 0, 0, 2, 2, 0, 0, 1, 2, 2, 2
};





void Level3::Initialize(size_t lifeCount) {
	__entities.switchScene = false;
	__displayWindow = SDL_GL_GetCurrentWindow();
#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);

	__program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
	__viewMatrix = glm::mat4(1.0f);
	__modelMatrix1 = glm::mat4(1.0f);
	__projectionMatrix1 = glm::ortho(-5.0f, 5.0f, -4.0f, 4.0f, -1.0f, 1.0f);
	__program.SetProjectionMatrix(__projectionMatrix1);
	__program.SetViewMatrix(__viewMatrix);
	__program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);

	glUseProgram(__program.programID);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);


	//load textures
	std::string a("Gasty");
	__states.loadTexture(a, "images/Gasty.png");
	a = "Haunter";
	__states.loadTexture(a, "images/Haunter.png");
	a = "ctg";
	__states.loadTexture(a, "images/ctg.png");
	a = "Squirtle";
	__states.loadTexture(a, "images/Squirtle.jfif");
	a = "Magnimon";
	__states.loadTexture(a, "images/Magnimon.png");
	a = "font1";
	__states.loadTexture(a, "fonts/font1.png");


	//transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//set gravity
	//intialize units and set attributes
	//player
	//__entities.player = new Player(2, 1, 7.0, 0,lifeCount);
	__entities.player->setTexture("ctg", __states);
	__entities.player->size = glm::vec3(1, 2, 1);
	/*__states.playerB = new Unit(4, 9.5, 0, 0);
	__states.playerB->loadTexture("images/ctg.png");
	__states.playerB->size = glm::vec3(1, -5, 1);*/

	//NPC
	spawnNPC(3);

	//Blocks
	//__entities.buildPlatform();
	//__entities.buildNPC2();
	__states.unitVertices = new float[12]{ -.5, -.5, .5, -.5, .5, .5, -.5, -.5, .5, .5, -.5, 0.5 };
	__states.modelUnitVertices = new float[12]{ -.5, -.5, .5, -.5, .5, .5, -.5, -.5, .5, .5, -.5, 0.5 };
	__states.unitTexCoords = new float[12]{ 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
	__states.modelTexCoords = new float[12]{ 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
	__states.state = GameStatus::GameState::GAME;
	GLuint mapTextureID = Utility::loadTexture("images/tileset.png");
	__entities.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
}

void Level3::Update(float deltaTime) {
	//check if win
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
		//__entities.switchScene = true;
	}


	__entities.player->update(deltaTime, __entities.player, __entities.map);

	//__states.playerB->move();
	updateNPC(deltaTime);
	if (__entities.player->onBoarder(__entities.map)) {
		//__states.__GAMEOVER__();
	}
	//boardercheck
	//__states.playerB->onBoarder();
	__viewMatrix = glm::mat4(1.0f);
	if (__entities.player->location->getLocation().x > 5) {
		__viewMatrix = glm::translate(__viewMatrix, glm::vec3(-__entities.player->location->getLocation().x, 0, 0));
	}
	else {
		__viewMatrix = glm::translate(__viewMatrix, glm::vec3(-5, 0, 0));
	}
}
void Level3::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glVertexAttribPointer(__program.positionAttribute, 2, GL_FLOAT, false, 0, __states.unitVertices);
	glEnableVertexAttribArray(__program.positionAttribute);
	glVertexAttribPointer(__program.texCoordAttribute, 2, GL_FLOAT, false, 0, __states.unitTexCoords);
	glEnableVertexAttribArray(__program.texCoordAttribute);
	//draw items

	//MessageBox(0, "Prepared to render", "", MB_OK);

	__entities.player->render(__program, __viewMatrix, __modelMatrix1, __projectionMatrix1, __states);
	//__states.playerB->render();
	renderNPC();
	if (__states.state == GameStatus::GameState::WIN) {
		Utility::DrawTextGL(&__program, __states.textureIDs["font1"], "MISSION ACCOMPLISHED", .6f, -.3f, glm::vec3(-4, 2, 0) + glm::vec3(__entities.player->location->getX(), 0, 0));
	}
	if (__states.state == GameStatus::GameState::LOSE) {
		Utility::DrawTextGL(&__program, __states.textureIDs["font1"], "MISSION FAILED", .6f, -.3f, glm::vec3(-4, 2, 0) + glm::vec3(__entities.player->location->getX(), 0, 0));
	}
	if (__states.state == GameStatus::GameState::GAME) {
		//Utility::DrawTextGL(&__program, __states.textureIDs["font1"], "TIME TO FIGHT BACK!Life: " + std::to_string(__entities.player->), .6f, -.3f, glm::vec3(-4, 1, 0) + glm::vec3(__entities.player->location->getX(), 0, 0));
		Utility::DrawTextGL(&__program, __states.textureIDs["font1"], "Press Space To Jump, Press E to activate POWER MODE to destroy the ghosts!", .2f, -.1f, glm::vec3(-4, 2, 0) + glm::vec3(__entities.player->location->getX(), 0, 0));

	}

	glDisableVertexAttribArray(__program.positionAttribute);
	glDisableVertexAttribArray(__program.texCoordAttribute);

	__entities.map->Render(&__program);
	SDL_GL_SwapWindow(__displayWindow);
}



void Level3::spawnNPC(GLuint num) {
	for (GLuint i = 0; i < num; ++i) {
		Unit* newNPC = new Haunter(4, 2, 2 * i, 0);//need some other way to get the spawn location
		newNPC->setTexture("Haunter", __states);//need NPC image list in future 
		newNPC->accel(i, pow(-1, i), 0);//speed
		__entities.NPCList.push_back(newNPC);
	}
}

void Level3::updateNPC(float time) {
	for (Unit* NPC : __entities.NPCList) {
		NPC->update(time, __entities.player, __entities.map);
	}
	for (Block* block : __entities.blockList) {
		block->update(time, __entities.player, __entities.map);
	}
}

void Level3::renderNPC() {//and blocks
	for (Unit* NPC : __entities.NPCList) {
		NPC->render(__program, __viewMatrix, __modelMatrix1, __projectionMatrix1, __states);
	}

	for (Block* block : __entities.blockList) {
		block->render(__program, __viewMatrix, __modelMatrix1, __projectionMatrix1, __states);
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