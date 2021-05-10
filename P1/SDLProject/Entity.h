
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
#include <math.h>


class Projectile;
class Player;
enum class UnitType { Gengar, Haunter, Player, Projectile, NA };
class WeakenBuff;
class Unit {

	friend WeakenBuff;
private:
	bool repeat;
	bool boarderFlagX;
	bool boarderFlagY;
	bool onland;

	Velocity* v;




protected:
	void applyFriction(float time);
	Status* status;
	GLuint textureID;
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
	Unit(float speed, float power, float health, float x, float y, float z);
	virtual void update(float time, Player* player, Map* map);
	virtual void update(float time, std::vector<Unit*> targets, Map* map);

	virtual void render(ShaderProgram& __program2, glm::mat4& __viewMatrix, glm::mat4& __modelMatrix1, glm::mat4& __projectionMatrix1, GameStatus& __states) const;
	void setTexture(const char*, GameStatus& __states, bool repeat = false);
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
	bool checkCollisionBox(Unit* other);
	void checkCollisionsX(Map* map);
	void checkCollisionsY(Map* map);
	virtual void processCollision(Unit* other);
	virtual void processCollision(std::vector<Unit*> targets);
	float getDistance(Unit* other) const;
	bool onBoarder(Map* map);
	void setMatrix(float* m, float*, float*, float* model) const;
	virtual void getHit();
	virtual void getHit(float damage);
	UnitType getType() const;
	const glm::vec3& getV() const;
	bool isAlive() const;
	bool landed() const;
	const Status* getStatus() const;
};

enum class BlockType { Deadly, Bouncy, Platform };


class WeakenBuff :public Buff {
private:
	Unit* buffTaker;
public:
	WeakenBuff(Unit* buffTaker);
	void gain(float duration) override;
	void update(float time) override;
};


class Player :public Unit {
private:
	BuffSkill powerShield;
	Skill pistol;
	//ShootSkillP gunshot;
	bool boosted = false;
	
public:
	WeakenBuff weaken;

	GLuint bulletTexture;
	Player(float speed, float health, float power, float x, float y, float r);
	void update(float time, Player* player, Map* map);
	void shoot(std::vector<Projectile*>& projLst);
	void activateSkill();
	bool isBoosted() const;
	void getHit(float dmg) override;
	Mix_Chunk* failureChunk;
	virtual void render(ShaderProgram& __program2, glm::mat4& __viewMatrix, glm::mat4& __modelMatrix1, glm::mat4& __projectionMatrix1, GameStatus& __states) const;
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

class Gengar :public Block {
private:
	BuffSkill shadowSneak;
	Skill shadowBall;
	Skill dash;
	Gengar** bossptrptr;
	Skill weaken;
public:
	void setBossptr(Gengar*& bossptr);
	GLuint shadowBallTexture;
	void update(float time,Player* player,Map* map);
	Gengar(float x, float y, float z);
	void processCollision(Player* other);
	void render(ShaderProgram& __program2, glm::mat4& __viewMatrix, glm::mat4& __modelMatrix1, glm::mat4& __projectionMatrix1,GameStatus&) const override;
	void useShadowBall(std::vector<Projectile*>& projLst, float x, float y);
	void useDash(float x, float y);
	void activateWeaken(Player* player);
	void getHit() override;
	
};


class Haunter :public Unit {
public:
	Haunter(float speed, float x, float y, float r);
	void update(float time,Player* player, Map* map);
};



enum class ProjType { Friendly, Hostile };

class Projectile : public Unit {
private:
	Buff duration;
	std::vector<Unit*>* target;
	ProjType type;
public:
	Projectile(float speed, float power, float health, float duration, float x, float y, float r, GLuint texture,ProjType type);//speed: movement speed, power: hit power, health: num of penetration
	void update(float time, std::vector<Unit*> targets, Map* map);
	void setAttribute(float speed, float power, float health, float duration, float x, float y, float r, GLuint texture);
	ProjType getType() const;
};
