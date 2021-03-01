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
SDL_Window* __displayWindow;
bool gameIsRunning = true;
ShaderProgram __program;
glm::mat4 __viewMatrix, __modelMatrix, __projectionMatrix;


float __boardSizeX=10;
float __boardSizeY=8;
const Uint8* __keys = SDL_GetKeyboardState(NULL);

float __deltaTime;
float __ticks;
float __lastTicks;

class Unit;

class GameStates {
public:
	Unit* player;
	Unit* playerB;//players are not in a list, since each player would require one control method
	std::vector<Unit*> NPCList;//maybe the size of this should be set before the game in the future
	std::queue<GLuint> deadNPCQueue;//maybe some NPC will die in the future; keep this queue to get the first available slot
	void __GAMEOVER__() {
		MessageBox(0, "******GAME OVER******", "!!!!!!!", MB_OK);
		gameIsRunning = false;
		SDL_Quit();
	};
	void spawnNPC(GLuint num);
	void moveNPC();
	void renderNPC();
};

GameStates __states;

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
	this->location = glm::vec3(x,y,0);
	this->r = r;
};
float Location::getX() const { return this->location.x; };
float Location::getY() const { return this->location.y; };
float Location::getR() const { return this->r; };

void Location::move(glm::vec3 v) {
	location += __deltaTime * v;
}

void Location::spin(const float r) {
	this->r += __deltaTime*r;
}


class Status {
private:
	float speed;
public:
	//Status() {};
	Status(float speed) {
		this->speed = speed;
	};
	float getSpeed() const{
		return speed;
	}
};

class Velocity {
private:
	glm::vec3 v=glm::vec3(0, 0, 0.0f);
	float vr=0;
public:
	Velocity() {};
	void accel(float x, float y, float z);
	void accelR(float r) { vr += r; };
	glm::vec3 getV() const { return v; };
	float getVr() const{ return vr; }
};


void Velocity::accel(float x, float y, float z) {
	v +=  glm::vec3(x, y, z);

}

class Unit {
private:
	Status* status;
	Location* location;
	GLuint textureID;
	Velocity* v;
	bool boarderFlagX=false;
	bool boarderFlagY=false;
	bool collisionFlag = false;
	

public:
	glm::vec3 size;//probably gonna make a class for this later
	//Unit() {};
	Unit(float speed, float x, float y, float r);
	void loadTexture(const char*);
	void moveLeft() { v->accel(-status->getSpeed() * __deltaTime, 0, 0); }
	void moveRight() { v->accel(status->getSpeed() * __deltaTime, 0, 0); }
	void moveUp() { v->accel(0,status->getSpeed() * __deltaTime, 0); }
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
			if (location->getX() > __boardSizeX-abs(size.x)/2 || location->getX() < abs(size.x)/2-__boardSizeX) {
				bounceX();
				__states.__GAMEOVER__();
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

};

void GameStates::spawnNPC(GLuint num){
	for (GLuint  i = 0; i < num; ++i) {
		Unit* newNPC = new Unit(4, -4,2*i, 0);//need some other way to get the spawn location
		newNPC->loadTexture("images/Haunter.png");//need NPC image list in future 
		newNPC->accel(i, pow(-1,i), 0);//speed
		NPCList.push_back(newNPC);
	}
}

void GameStates::moveNPC(){
	for (Unit* NPC : NPCList) {
		NPC->move();
		NPC->processCollision(player);
		NPC->processCollision(playerB);
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
}

void GameStates::renderNPC() {
	for (Unit* NPC : NPCList) {
		NPC->render();
	}
}

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
	__modelMatrix = glm::mat4(1.0f);
	__modelMatrix = glm::translate(__modelMatrix, location->getLocation());
	__modelMatrix = glm::rotate(__modelMatrix, glm::radians(location->getR()), glm::vec3(0.0f, 0.0f, 1));
	__modelMatrix = glm::scale(__modelMatrix, size);
	__program.SetModelMatrix(__modelMatrix);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Unit::loadTexture(const char* filePath) {
	int w, h, n;
	unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
	
	if (image == NULL) {
		std::cout << "Unable to load image. Make sure the path is correct\n";
		assert(false);
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(image);
	std::stringstream os;
	os << textureID;
	std::string intString = os.str();
	MessageBox(0, (LPCTSTR)intString.c_str(), "", MB_OK);
}



void Initialize() {
	SDL_Init(SDL_INIT_VIDEO);
	__displayWindow = SDL_CreateWindow("PROFESSOR CHASED BY HAUNTER!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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

	//transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//intialize units and set attributes
	__states.player = new Unit(4, -9.5,0 , 0);
	__states.player->loadTexture("images/ctg.png");
	__states.player->size = glm::vec3(1, 5, 1);
	__states.playerB = new Unit(4, 9.5, 0, 0);
	__states.playerB->loadTexture("images/ctg.png");
	__states.playerB->size = glm::vec3(1, -5, 1);
	__states.spawnNPC(3);
	__lastTicks = (float)SDL_GetTicks() / 1000.0f;
}

void ProcessInput() {
	//get delta time
	__ticks = (float)SDL_GetTicks() / 1000.0f;
	__deltaTime = __ticks - __lastTicks;
	__lastTicks = __ticks;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type){
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


	if (__keys[SDL_SCANCODE_W]) {
		__states.player->moveUp();
	}
	
	if (__keys[SDL_SCANCODE_S]) {
		__states.player->moveDown();
	}
	/* disabling player horizontal move
	if (keys[SDL_SCANCODE_A]) {
		__states.player->moveLeft();
	}

	if (keys[SDL_SCANCODE_D]) {
		__states.player->moveRight();
	}
	*/

	if (__keys[SDL_SCANCODE_UP]) {
		__states.playerB->moveUp();
	}

	if (__keys[SDL_SCANCODE_DOWN]) {
		__states.playerB->moveDown();
	}

}
void Update() {
	__states.player->move();
	__states.playerB->move();
	__states.moveNPC();
	if (__states.player->onBoarder()) {
		//__states.__GAMEOVER__();
	}
	//boardercheck
	__states.playerB->onBoarder();
	
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	float vertices[] = { -.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
	glVertexAttribPointer(__program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(__program.positionAttribute);
	glVertexAttribPointer(__program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(__program.texCoordAttribute);
	//draw items

	//MessageBox(0, "Prepared to render", "", MB_OK);

	__states.player->render();
	__states.playerB->render();
	__states.renderNPC();
	
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
		Update();
		Render();

	}

	Shutdown();
	return 0;
}