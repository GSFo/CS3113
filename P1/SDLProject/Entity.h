#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include"Map.h"
#include "Utility.h"
#include "Buff.h"
#include "Attribute.h"
#include "ShaderProgram.h"
#include "GameStates.h"
#include <SDL_mixer.h>

class Player;
enum class UnitType;

class Unit {
private:
	Status* status;
	GLuint textureID;
	Velocity* v;
	bool repeat;
	bool boarderFlagX;
	bool boarderFlagY;
	bool onland;





protected:
	bool collisionFlag;
	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;
	UnitType type;
	bool dead;


public:
	Location* location;
	glm::vec3 size;//probably gonna make a class for this later
	//Unit() {};
	Unit(float speed, float x, float y, float r);
	virtual void update(float time, Player* player, Map* map)=0;
	void setTexture(const char*, GameStatus __states, bool repeat = false);
	void moveLeft(float time);
	void moveRight(float time);
	void moveUp(float time);
	void moveDown(float time);
	void jump();
	void land();
	void accel(float x, float y, float z);
	void accelR(float r);
	void bounceX();
	void bounceY();
	void move(float time);//move with velocity
	virtual void render(ShaderProgram __program2, glm::mat4 __viewMatrix, glm::mat4 __modelMatrix1, glm::mat4 __projectionMatrix1,GameStatus __states) const;
	bool checkCollisionBox(Unit* other);
	void checkCollisionsX(Map* map);
	void checkCollisionsY(Map* map);
	virtual void processCollision(Unit* other);
	float getDistance(Unit* other) const;
	bool onBoarder(Map* map);
	void setMatrix(float* m, float*, float*, float* model) const;
	virtual void die();
	UnitType getType() const;
	const glm::vec3& getV() const;
	bool isAlive() const;
	bool landed() const;
};

enum class BlockType { Deadly, Bouncy, Platform };


class Player :public Unit {
private:
	BuffSkill powerShield;
	bool boosted = false;
	
public:
	size_t lifeCount;
	Player(float speed, float x, float y, float r, size_t lifeCount);
	void update(float time, Player* player, Map* map);
	void activateSkill();
	bool isBoosted() const;
	void die() override;
	Mix_Chunk* failureChunk;
};

class Block :public Unit {
private:
	BlockType type;
public:
	Block(BlockType type, float x, float y, float r);
	const BlockType getType();
	void processCollision(Unit* other);
	void update(float time, Player* player, Map* map);
};

class Magniton :public Block {
private:
	Skill flee;
public:
	Magniton(float x, float y, float r);
	void update(float time,Player* player, Map* map);
};

class Gasty :public Block {
private:
	BuffSkill voidForm;
public:
	void update(float time,Player* player,Map* map);
	Gasty(float x, float y, float z);
	void processCollision(Player* other);
	void render(ShaderProgram __program2, glm::mat4 __viewMatrix, glm::mat4 __modelMatrix1, glm::mat4 __projectionMatrix1,GameStatus) const;

};

class Haunter :public Unit {
public:
	Haunter(float speed, float x, float y, float r);
	void update(float time,Player* player, Map* map);
	void processCollision(Player* other);
};



