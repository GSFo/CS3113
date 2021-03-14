#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif


#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "windows.h"
#include <iomanip>
#include <vector>
#include<queue>

#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#include<hash_map>
#include<string>

SDL_Window* __displayWindow;
bool gameIsRunning = true;
ShaderProgram __program;
glm::mat4 __viewMatrix, __modelMatrix, __projectionMatrix;


float __boardSizeX = 10;
float __boardSizeY = 8;
const Uint8* __keys = SDL_GetKeyboardState(NULL);

float __deltaTime;
float __ticks;
float __lastTicks;

class Unit;
class Block;

enum class GameState { GAME, WIN, LOSE };

class GameStatus {
public:
	Unit* player;
	float* modelUnitVertices;
	float* unitVertices;
	float* unitTexCoords;
	float* modelTexCoords;
	GameState state;
	void DrawTextGL(ShaderProgram* program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position);

	std::hash_map<std::string, GLuint> textureIDs;
	void loadTexture(std::string& name, const char* filename);
	float gravity;
	//p2 code. Unit* playerB;//players are not in a list, since each player would require one control method
	std::vector<Unit*> NPCList;//maybe the size of this should be set before the game in the future
	std::vector<Block*> blockList;
	std::queue<GLuint> deadNPCQueue;//maybe some NPC will die in the future; keep this queue to get the first available slot
	void __GAMEOVER__() {
		state = GameState::LOSE;
		//gameIsRunning = false;
		//SDL_Quit();
	};

	void __GAMEWIN__() {
		state = GameState::WIN;
	};
	void spawnNPC(GLuint num);
	void updateNPC();
	void renderNPC();
	void buildPlatform();
	void buildBlocks();
};

void GameStatus::loadTexture(std::string& name, const char* filePath){
	int w, h, n;
	unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

	if (image == NULL) {
		std::cout << "Unable to load image. Make sure the path is correct\n";
		assert(false);
	}
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	stbi_image_free(image);
	textureIDs[name] = textureID;
}


GameStatus __states;

class Location {
private:
	glm::vec3 location;
	float r;
public:
	Location(float x, float y, float r);
	float getR() const;
	void spin(const float r);
	float getX() const;
	float getY() const;
	glm::vec3 getLocation() const {
		return location;
	}
	void move(glm::vec3);
};

Location::Location(float x, float y, float r) {
	this->location = glm::vec3(x, y, 0);
	this->r = r;
};
float Location::getX() const { return this->location.x; };
float Location::getY() const { return this->location.y; };
float Location::getR() const { return this->r; };

void Location::move(glm::vec3 v) {
	location += __deltaTime * v;
}

void Location::spin(const float r) {
	this->r += __deltaTime * r;
}


class Status {
private:
	float speed;
public:
	//Status() {};
	Status(float speed) {
		this->speed = speed;
	};
	float getSpeed() const {
		return speed;
	}
};

class Velocity {
private:
	glm::vec3 v = glm::vec3(0, 0, 0.0f);
	float vr = 0;
public:
	Velocity() {};
	void accel(float x, float y, float z);
	void accelR(float r) { vr += r; };
	glm::vec3 getV() const { return v; };
	float getVr() const { return vr; }
};


void Velocity::accel(float x, float y, float z) {
	v += glm::vec3(x, y, z);
}

class Unit {
private:
	Status* status;
	Location* location;
	GLuint textureID;
	Velocity* v;
	bool repeat=false;
	bool boarderFlagX = false;
	bool boarderFlagY = false;

protected:
	bool collisionFlag = false;


public:
	glm::vec3 size;//probably gonna make a class for this later
	//Unit() {};
	Unit(float speed, float x, float y, float r);
	void setTexture(const char*,bool repeat=false);
	void moveLeft() { v->accel(-status->getSpeed() * __deltaTime, 0, 0); }
	void moveRight() { v->accel(status->getSpeed() * __deltaTime, 0, 0); }
	void moveUp() { v->accel(0, status->getSpeed() * __deltaTime, 0); }
	void moveDown() { v->accel(0, -status->getSpeed() * __deltaTime, 0); }
	void accel(float x, float y, float z);
	void accelR(float r) {
		v->accelR(r); if (v->getVr() > 80) { __states.__GAMEOVER__(); }
	};
	void bounceX();
	void bounceY();
	void move();//move with velocity
	void render() const;
	bool checkCollisionBox(Unit* other);
	void processCollision(Unit* other);
	float getDistance(Unit* other) const {
		return std::sqrt(std::pow(location->getX() - other->location->getX(), 2) + std::pow(location->getY() - other->location->getY(), 2));
	}

	bool onBoarder() {
		if (!boarderFlagX) {
			if (location->getX() > __boardSizeX - abs(size.x) / 2 || location->getX() < abs(size.x) / 2 - __boardSizeX) {
				bounceX();
				//__states.__GAMEOVER__();
				boarderFlagX = true;
			}
		}
		else {
			if (!(location->getX() > __boardSizeX - abs(size.x) / 2 || location->getX() < abs(size.x) / 2 - __boardSizeX)) {
				boarderFlagX = false;
			}
		}

		if (!boarderFlagY) {
			if (location->getY() > __boardSizeY - abs(size.y) / 2 || location->getY() < abs(size.y) / 2 - __boardSizeY) {
				bounceY();
				boarderFlagY = true;
			}
		}
		else {
			if (!(location->getY() > __boardSizeY - abs(size.y) / 2 || location->getY() < abs(size.y) / 2 - __boardSizeY)) {
				boarderFlagY = false;
			}
		}
		return boarderFlagX || boarderFlagY;
	}
	void setMatrix(float* m, float*,float*, float* model) const;
};

enum class BlockType { Deadly, Bouncy, Platform };

class Block :public Unit {
private:
	BlockType type;
public:
	Block(BlockType type, float x, float y, float r);
	const BlockType getType();
	void processCollision(Unit* other);

};



void GameStatus::spawnNPC(GLuint num) {
	for (GLuint i = 0; i < num; ++i) {
		Unit* newNPC = new Unit(4, -4, 2 * i, 0);//need some other way to get the spawn location
		newNPC->setTexture("Haunter");//need NPC image list in future 
		newNPC->accel(i, pow(-1, i), 0);//speed
		NPCList.push_back(newNPC);
	}
}

void GameStatus::updateNPC() {
	for (Unit* NPC : NPCList) {
		NPC->move();
		NPC->processCollision(player);
		//NPC->processCollision(playerB);
		//check collision circular
		/*not for this task
		if (NPC->getDistance(player) < 1) {
			player->bounceX();
			player->bounceY();
			player->accelR(5);
			//__states.__GAMEOVER__();
		}*/
		//check boarder
		NPC->onBoarder();
	}
	for (Block* block : blockList) {
		block->processCollision(player);
	}
}

void GameStatus::renderNPC() {//and blocks
	for (Unit* NPC : NPCList) {
		NPC->render();
	}

	for (Block* block : blockList) {
		block->render();
	}
}

void GameStatus::buildPlatform() {
	Block* newBlock = new Block(BlockType::Platform, 0, -7, 0);
	newBlock->setTexture("Squirtle",true);//need NPC image list in future
	newBlock->size.x = 20;
	blockList.push_back(newBlock);
}


void GameStatus::buildBlocks() {
	bool isDeadly = false;
	for (float x = -9.5; x < 10; x += 4) {
		if (isDeadly) {
			Block* newBlock = new Block(BlockType::Deadly, x, 0, 0);
			newBlock->setTexture("Gasty");//need NPC image list in future 
			blockList.push_back(newBlock);
		}

		else {
			Block* newBlock = new Block(BlockType::Bouncy, x, 0, 0);
			newBlock->setTexture("Magnimon");//need NPC image list in future 
			blockList.push_back(newBlock);
		}

		isDeadly = !isDeadly;
	}
}

void GameStatus::DrawTextGL(ShaderProgram* program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position)
{
	float width = 1.0f / 16.0f;
	float height = 1.0f / 16.0f;

	std::vector<float> vertices;
	std::vector<float> texCoords;

	for (int i = 0; i < text.size(); i++) {

		int index = (int)text[i];
		float offset = (size + spacing) * i;
		float u = (float)(index % 16) / 16.0f;
		float v = (float)(index / 16) / 16.0f;
		vertices.insert(vertices.end(), {
		 offset + (-0.5f * size), 0.5f * size,
		 offset + (-0.5f * size), -0.5f * size,
		 offset + (0.5f * size), 0.5f * size,
		 offset + (0.5f * size), -0.5f * size,
		 offset + (0.5f * size), 0.5f * size,
		 offset + (-0.5f * size), -0.5f * size, });
		texCoords.insert(texCoords.end(), {
			u, v,
			u, v + height,
			u + width, v,
			u + width, v + height,
			u + width, v,
			u, v + height,
			});

	} // end of for loop 
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	program->SetModelMatrix(modelMatrix);

	glUseProgram(program->programID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, fontTextureID);
	glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}//copied from lecture slide

Unit::Unit(float speed, float x, float y, float r) {
	status = new Status(speed);
	location = new Location(x, y, r);
	v = new Velocity();
	textureID = -1;
	size = glm::vec3(1, 1, 1);
};

void Unit::accel(float x, float y, float z) {
	v->accel(x, y, z);
}

void Unit::move() {
	location->move(v->getV());
	location->spin(v->getVr());
}

bool Unit::checkCollisionBox(Unit* other) {
	if (abs(location->getX() - other->location->getX()) < (abs(size.x) + abs(other->size.x)) / 2) {
		if (abs(location->getY() - other->location->getY()) < (abs(size.y) + abs(other->size.y)) / 2) {
			/*
			std::stringstream os;
			os << "collision detected";
			std::string intString = os.str();
			MessageBox(0, (LPCTSTR)intString.c_str(), "", MB_OK);
			*/
			return true;
		}
	}
	return false;
}

void Unit::processCollision(Unit* other) {

	if (!collisionFlag) {
		if (checkCollisionBox(other)) {
			__states.__GAMEOVER__();
			bounceX();
			collisionFlag = true;
		}
	}
	else {
		if (!checkCollisionBox(other)) {
			collisionFlag = false;
		}
	}
}

void Unit::bounceX() {
	size.x *= -1;
	v->accel(-2 * v->getV().x, 0, 0);
}

void Unit::bounceY() {
	v->accel(0, -2 * v->getV().y, 0);
}

void Unit::setMatrix (float* toSetV, float* toSetT, float* modelV, float*modelT) const{
	bool isX = true;
	for (int i = 0; i < 12; ++i) {
		if (isX) {
			toSetV[i] = size.x * modelV[i];
			if (repeat) {
				toSetT[i] = modelT[i] * size.x;
			}
			else {
				toSetT[i] = modelT[i];
			}
		}
		else {
			toSetV[i] = size.y * modelV[i];
			if (repeat) {
				toSetT[i] = modelT[i] * size.y;
			}
			else {
				toSetT[i] = modelT[i];
			}
		}
		isX = !isX;
	}

}
void Unit::render() const {
	/*
	std::stringstream os;
	os << "TID: "<<textureID<<"\nx: "<< std::fixed<<std::setprecision(2)<<location->getX()<<"\ny: "<<location->getY();
	std::string intString = os.str();
	MessageBox(0, (LPCTSTR)intString.c_str(), "", MB_OK);
	*/
	if (textureID == -1) {
		assert(false);
	}
	this->setMatrix(__states.unitVertices,__states.unitTexCoords, __states.modelUnitVertices, __states.modelTexCoords);
	__program.SetProjectionMatrix(__projectionMatrix);
	__program.SetViewMatrix(__viewMatrix);
	__modelMatrix = glm::mat4(1.0f);
	__modelMatrix = glm::translate(__modelMatrix, location->getLocation());
	__modelMatrix = glm::rotate(__modelMatrix, glm::radians(location->getR()), glm::vec3(0.0f, 0.0f, 1));
	//__modelMatrix = glm::scale(__modelMatrix, size);
	__program.SetModelMatrix(__modelMatrix);

	if (repeat) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	}
	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}



void Unit::setTexture(const char* name, bool repeat) {
	this->textureID = __states.textureIDs[name];
	this->repeat = repeat;
	/*
	std::stringstream os;
	os << textureID;
	std::string intString = os.str();
	MessageBox(0, (LPCTSTR)intString.c_str(), "", MB_OK);
	*/
}



Block::Block(BlockType type, float x, float y, float r) :Unit(0.0, x, y, r) {
	this->type = type;
	
}

const BlockType Block::getType() {
	return type;
}


void Block::processCollision(Unit* other) {
	switch (type) {
	case BlockType::Bouncy:
		if (!collisionFlag) {
			if (checkCollisionBox(other)) {
				other->bounceX();
				other->bounceY();
				collisionFlag = true;
			}
		}
		else {
			if (!checkCollisionBox(other)) {
				collisionFlag = false;
			}
		}
		return;
	case BlockType::Deadly:
		if (checkCollisionBox(other)) {
			__states.__GAMEOVER__();
		}
		return;

	case BlockType::Platform:
		if (checkCollisionBox(other)) {
			__states.__GAMEWIN__();
		}
		return;
	}
}



void Initialize() {
	SDL_Init(SDL_INIT_VIDEO);
	__displayWindow = SDL_CreateWindow("PROFESSOR ESCAPE FROM THE HAUNTERS!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(__displayWindow);
	SDL_GL_MakeCurrent(__displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);

	__program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
	__viewMatrix = glm::mat4(1.0f);
	__modelMatrix = glm::mat4(1.0f);
	__projectionMatrix = glm::ortho(-10.0f, 10.0f, -7.5f, 7.5f, -1.0f, 1.0f);
	__program.SetProjectionMatrix(__projectionMatrix);
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
	__states.gravity = -.5;
	//intialize units and set attributes
	//player
	__states.player = new Unit(3, 0, 7.0, 0);
	__states.player->setTexture("ctg");
	__states.player->size = glm::vec3(1, 2, 1);
	/*__states.playerB = new Unit(4, 9.5, 0, 0);
	__states.playerB->loadTexture("images/ctg.png");
	__states.playerB->size = glm::vec3(1, -5, 1);*/
	
	//NPC
	__states.spawnNPC(3);
	__lastTicks = (float)SDL_GetTicks() / 1000.0f;

	//Blocks
	__states.buildPlatform();
	__states.buildBlocks();
	__states.unitVertices = new float[12] { -.5, -.5, .5, -.5, .5, .5, -.5, -.5, .5, .5, -.5, 0.5 };
	__states.modelUnitVertices = new float[12]{ -.5, -.5, .5, -.5, .5, .5, -.5, -.5, .5, .5, -.5, 0.5 };
	__states.unitTexCoords = new float[12] { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
	__states.modelTexCoords= new float[12]{ 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 }; 
	__states.state = GameState::GAME;
}

void ProcessInput() {
	//get delta time
	__ticks = (float)SDL_GetTicks() / 1000.0f;
	__deltaTime = __ticks - __lastTicks;
	__lastTicks = __ticks;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			gameIsRunning = false;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				gameIsRunning = false;
				break;
			}
		}
	}

	/*disabling player vertical move
	if (__keys[SDL_SCANCODE_W]) {
		__states.player->moveUp();
	}

	if (__keys[SDL_SCANCODE_S]) {
		__states.player->moveDown();
	}*/
	///* disabling player horizontal move
	if (__states.state == GameState::GAME) {
		if (__keys[SDL_SCANCODE_A]) {
			__states.player->moveLeft();
		}
		if (__keys[SDL_SCANCODE_D]) {
			__states.player->moveRight();
		}
	}
	//*/

	/*disabling playerB
	if (__keys[SDL_SCANCODE_UP]) {
		__states.playerB->moveUp();
	}

	if (__keys[SDL_SCANCODE_DOWN]) {
		__states.playerB->moveDown();
	}*/

}
void Update() {
	__states.player->move();
	__states.player->accel(0, __states.gravity*__deltaTime, 0);
	//__states.playerB->move();
	__states.updateNPC();
	if (__states.player->onBoarder()) {
		//__states.__GAMEOVER__();
	}
	//boardercheck
	//__states.playerB->onBoarder();

}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glVertexAttribPointer(__program.positionAttribute, 2, GL_FLOAT, false, 0, __states.unitVertices);
	glEnableVertexAttribArray(__program.positionAttribute);
	glVertexAttribPointer(__program.texCoordAttribute, 2, GL_FLOAT, false, 0, __states.unitTexCoords);
	glEnableVertexAttribArray(__program.texCoordAttribute);
	//draw items
	
	//MessageBox(0, "Prepared to render", "", MB_OK);
	
	__states.player->render();
	//__states.playerB->render();
	__states.renderNPC();
	if (__states.state == GameState::WIN) {
		__states.DrawTextGL(&__program, __states.textureIDs["font1"], "MISSION ACCOMPLISHED", 1.3f, -.6f, glm::vec3(-8, 4, 0));
	}
	if (__states.state == GameState::LOSE) {
		__states.DrawTextGL(&__program, __states.textureIDs["font1"], "MISSION FAILED", 1.3f, -.3f, glm::vec3(-8, 4, 0));
	}
	if (__states.state == GameState::GAME) {
		__states.DrawTextGL(&__program, __states.textureIDs["font1"], "DONT TOUCH THE GHOST TYPE POKEMON!", .8f, -.3f, glm::vec3(-8, 4, 0));
	}

	glDisableVertexAttribArray(__program.positionAttribute);
	glDisableVertexAttribArray(__program.texCoordAttribute);

	SDL_GL_SwapWindow(__displayWindow);
}


void Shutdown() {
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	Initialize();
	while (gameIsRunning) {
		ProcessInput();
		if (__states.state == GameState::GAME) {
			Update();
		}
		Render();
	}
	Shutdown();
	return 0;
}