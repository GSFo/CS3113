#include "Entity.h"
#define PI 3.14159265
//gameplay constants
float GRAVITY = -5;
float FRICTION = -1.5;
int sign(float a) {
	if (a > 0) return 1;
	if (a < 0) return -1;
	return 0;
}



WeakenBuff::WeakenBuff(Unit* buffTaker):Buff(0, "weaken"),buffTaker(buffTaker) {

}

void WeakenBuff::gain(float time) {
	buffTaker->status->buffed(.5, .1,.8);
	Buff::gain(time);
}

void WeakenBuff::update(float time) {
	if (timer > 0) {
		timer -= time;
		if (timer <= 0) {
			buffTaker->status->buffed(2, 10, 1.25);
			timer = 0;
		}
	}
}

class Player;
void Unit::moveLeft(float time) { if (isAlive()) { v->accel(-status->getSpeed() * time, 0, 0); size.x = 1; } }
void Unit::moveRight(float time) { if (isAlive()) v->accel(status->getSpeed() * time, 0, 0); size.x = -1; }
void Unit::moveUp(float time) { if (isAlive()) v->accel(0, status->getSpeed() * time, 0); }
void Unit::moveDown(float time) { if (isAlive()) v->accel(0, -status->getSpeed() * time, 0); }
	
void Unit::accelR(float r) {
	v->accelR(r); 
};

void Unit::applyFriction(float time) {
	float theta = atan2(v->getV().x, v->getV().y);
	if (pow(getV().x, 2) + pow(getV().y, 2) > pow(FRICTION * time, 2)) {
		accel(sin(theta) * FRICTION * time, cos(theta) * FRICTION * time, 0);
	}
	else {
		accel(-getV().x, -getV().y, 0);
	}
}

float Unit::getDistance(Unit* other) const {
	return std::sqrt(std::pow(location->getX() - other->location->getX(), 2) + std::pow(location->getY() - other->location->getY(), 2));
}

UnitType Unit::getType() const { return type; };
const glm::vec3& Unit::getV() const { return v->getV(); }
bool Unit::isAlive() const { return !dead; }
bool Unit::landed() const { return collidedBottom; }
const Status* Unit::getStatus() const { return status; };


bool Unit::onBoarder(Map* map) {
	if (!boarderFlagX) {
		if (location->getX() > map->getWidth() - abs(size.x) || location->getX() < 0) {
			bounceX();
			//__states.__GAMEOVER__();
			boarderFlagX = true;
		}
	}
	else {
		if (!(location->getX() > map->getWidth() - abs(size.x) || location->getX() < 0)) {
			boarderFlagX = false;
		}
	}

	if (!boarderFlagY) {
		if (location->getY() > 1 - abs(size.y) / 2 || location->getY() < -11- abs(size.y)/2) {
			
			bounceY();
			boarderFlagY = true;
		}
	}
	else {
		if (!(location->getY() > map->getHeight() - abs(size.y) / 2 || location->getY() < abs(size.y) / 2 - map->getHeight())) {
			boarderFlagY = false;
		}
	}
	return boarderFlagX || boarderFlagY;
}


Player::Player(float speed, float power, float health, float x, float y, float r) :Unit(speed,power,health, x, y, r), powerShield(10, 3, "power shield") ,pistol(1),weaken(this){
		Unit::type = UnitType::Player; 
		failureChunk = Mix_LoadWAV("music/Dog Barking-SoundBible.com-1918920417.wav");
		size = glm::vec3(-1, 1, 1);
};

void Player::update(float time, Player* player, Map* map) {
	//no gravity for this game; camera is upside down
	//accel(0, GRAVITY * time, 0);
	//player is always on land
	//if (landed()) {
	float d = sqrt(pow(getV().x, 2)+ pow(getV().y, 2));
	float xp = getV().x/d;
	float yp = getV().y/d;
	applyFriction(time);
	//}

	move(time);
	weaken.update(time);
	pistol.update(time);
	powerShield.update(time);
	if (powerShield.inEffect()) {
		if (!boosted) { boosted = true;}
	}
	else {
		if (boosted) { boosted = false; }
	}
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;
	checkCollisionsX(map);
	checkCollisionsY(map);
};
void Player::activateSkill() {
	powerShield.activate();
	weaken.update(100);
}

void Player::shoot(std::vector<Projectile*>& projLst) {
	Projectile* bullet;
	if (pistol.activate() != -1) {
		if (size.x < 0) {
			bullet = new Projectile(5, status->getPower(), 1, 3, location->getX() + 1, location->getY() + .2, 0, bulletTexture, ProjType::Friendly);
			bullet->moveRight(1);
		}
		else {
			bullet = new Projectile(5, status->getPower(), 1, 3, location->getX() - 1, location->getY() + .2, 0, bulletTexture, ProjType::Friendly);
			bullet->moveLeft(1);
		}
		bullet->size = glm::vec3(.7, .5, .5);
		projLst.push_back(bullet);
	}
}

bool Player::isBoosted() const {
	return boosted;
}

void Block::update(float time, Player* player, Map* map) { if (dead) return; processCollision(player); };


Magniton::Magniton(float x, float y, float r) :Block(BlockType::Bouncy, x, y, r), flee(5.0) {};

void Magniton::update(float time, Player* player, Map* map) {
	if (getDistance(player) < 2 && player->isBoosted()) {
		if (flee.activate() != -1) {
			accel(player->getV().x * .3, player->getV().y * .3, 0);
		}
	}

	if (onBoarder(map)) { getHit(); }
	Unit::move(time);
	Block::update(time, player, map);
}
Gengar::Gengar(float x, float y, float z) :Block(BlockType::Deadly, x, y, z), shadowSneak(10.0, 4.0, "shadow sneak"), shadowBall(3),dash(10),weaken(10){ Unit::type = UnitType::Gengar; };

void Gengar::render(ShaderProgram& __program, glm::mat4& __viewMatrix, glm::mat4& __modelMatrix1, glm::mat4& __projectionMatrix1,GameStatus& __states) const {
	if (!shadowSneak.inEffect()) {
		Unit::render(__program, __viewMatrix, __modelMatrix1, __projectionMatrix1,__states);
	}
	else {
		Utility::DrawTextGL(&__program, __states.textureIDs["font1"], "Shadow Sneak", .6f, -.3f, glm::vec3(location->getX(), location->getY(), 0));
	}
	Utility::DrawTextGL(&__program, __states.textureIDs["font1"], "HP: " + std::to_string((int)status->getHealth()), .6f, -.3f, glm::vec3(location->getX(), location->getY() - .5, 0));

	glVertexAttribPointer(__program.positionAttribute, 2, GL_FLOAT, false, 0, __states.unitVertices);
	glEnableVertexAttribArray(__program.positionAttribute);
	glVertexAttribPointer(__program.texCoordAttribute, 2, GL_FLOAT, false, 0, __states.unitTexCoords);
	glEnableVertexAttribArray(__program.texCoordAttribute);
}

void Gengar::setBossptr(Gengar*& bossptr) {
	bossptrptr = &bossptr;
}

void Gengar::update(float time, Player* player, Map* map) {
	if (dead) {
		return;
	}

	shadowBall.update(time);
	shadowSneak.update(time);
	dash.update(time);
	weaken.update(time);
	applyFriction(time);
	move(time);
	if (getDistance(player) < 7) {
		if (player->isBoosted()) {
			shadowSneak.activate();
		}
	}
	processCollision(player);

	onBoarder(map);
}

void Gengar::processCollision(Player* other) {
	if (!shadowSneak.inEffect()) {
		if (!collisionFlag) {
			if (checkCollisionBox(other)) {

				if (other->isBoosted()) {
					getHit();
					return;
				}
				other->getHit(1);
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

}

void Gengar::useShadowBall(std::vector<Projectile*>& projLst, float  x, float y) {
	//launch a shadowball to (x,y)
	if (shadowBall.activate() != -1) {
		Projectile* bullet = new Projectile(2, 1, 2, 18, location->getX(), location->getY(), 0, shadowBallTexture,ProjType::Hostile);
		float theta = atan2(x - location->getX(), y - location->getY());
		bullet->accel(sin(theta)*bullet->getStatus()->getSpeed(),cos(theta) * bullet->getStatus()->getSpeed(), 0);
		//bullet->moveLeft(1);
		projLst.push_back(bullet);
	}
}

void Gengar::useDash(float x, float y) {
	if (dash.activate() != -1) {
		float theta = atan2(x - location->getX(), y - location->getY());
		accel(sin(theta)*getStatus()->getSpeed(), cos(theta)*getStatus()->getSpeed(), 0);
	}
}

void Gengar::activateWeaken(Player* target) {
	if (weaken.activate() != -1) {
		target->weaken.gain(4);
	}
}

void Gengar::getHit() {
	Unit::getHit();
}



Haunter::Haunter(float speed, float x, float y, float r) :Unit(speed, x, y, r) { Unit::type = UnitType::Haunter; };
	

void Haunter::update(float time, Player* player, Map* map) {
	if (dead) return;
	move(time);
	processCollision(player);
	onBoarder(map);
}





Unit::Unit(float speed, float x, float y, float r) {
	status = new Status(speed);
	location = new Location(x, y, r);
	v = new Velocity();
	textureID = -1;
	size = glm::vec3(1, 1, 1);
	type = UnitType::NA;
};

Unit::Unit(float speed, float power,float health, float x, float y, float r) {
	status = new Status(speed,power,health);
	location = new Location(x, y, r);
	v = new Velocity();
	textureID = -1;
	size = glm::vec3(1, 1, 1);
	type = UnitType::NA;
};

void Unit::update(float time, std::vector<Unit*> targets, Map* map) {

}

void Unit::update(float time, Player* player, Map* map) {}

void Unit::accel(float x, float y, float z) {
	v->accel(x, y, z);
}

void Unit::move(float time) {
	location->move(v->getV(), time);
	location->spin(v->getVr(), time);
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
			other->getHit(status->getPower());
			getHit(other->status->getPower());
			//bounceX();
			collisionFlag = true;
		}
	}
	else {
		if (!checkCollisionBox(other)) {
			collisionFlag = false;
		}
	}
}

void Unit::processCollision(std::vector<Unit*> other) {
	for (Unit* item : other) {
		processCollision(item);
	}
}

void Unit::bounceX() {
	size.x *= -1;
	v->accel(-2 * v->getV().x, 0, 0);
}

void Unit::bounceY() {
	v->accel(0, -2 * v->getV().y, 0);
}

void Unit::setMatrix(float* toSetV, float* toSetT, float* modelV, float* modelT) const {
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

void Unit::getHit() { dead = true; }
void Unit::getHit(float dmg) {
	if (status->loseLife(dmg)) {
		Unit::getHit();
	}
}
void Player::getHit(float dmg){
	if (!powerShield.inEffect()) {
		Mix_PlayChannel(-1, failureChunk, 0);
		Unit::getHit(dmg);
	}
}

void Player::render(ShaderProgram& __program, glm::mat4& __viewMatrix, glm::mat4& __modelMatrix1, glm::mat4& __projectionMatrix1, GameStatus& __states) const{
	Unit::render(__program, __viewMatrix, __modelMatrix1, __projectionMatrix1, __states);
	Utility::DrawTextGL(&__program, __states.textureIDs["font1"], "HP: "+std::to_string((int)status->getHealth()), .6f, -.3f, glm::vec3(location->getX()-.5, location->getY()-1, 0));
	if (weaken.inEffect()) {
		Utility::DrawTextGL(&__program, __states.textureIDs["font1"], "Weaken", .6f, -.3f, glm::vec3(location->getX()-.5, location->getY() +1, 0));

	}
	if (powerShield.inEffect()) {
		Utility::DrawTextGL(&__program, __states.textureIDs["font1"], "Shield Up", .6f, -.3f, glm::vec3(location->getX() - .5, location->getY() + 1, 0));

	}
	glVertexAttribPointer(__program.positionAttribute, 2, GL_FLOAT, false, 0, __states.unitVertices);
	glEnableVertexAttribArray(__program.positionAttribute);
	glVertexAttribPointer(__program.texCoordAttribute, 2, GL_FLOAT, false, 0, __states.unitTexCoords);
	glEnableVertexAttribArray(__program.texCoordAttribute);

}

void Unit::land() {
	v->accel(0, -v->getV().y, 0);
	onland = true;
}

void Unit::jump() {
	if (collidedBottom) {
		v->accel(0, -1 * GRAVITY, 0);
	}
}

void Unit::render(ShaderProgram& __program2, glm::mat4& __viewMatrix, glm::mat4& __modelMatrix1, glm::mat4& __projectionMatrix1,GameStatus &__states) const {
	/*
	std::stringstream os;
	os << "TID: "<<textureID<<"\nx: "<< std::fixed<<std::setprecision(2)<<location->getX()<<"\ny: "<<location->getY();
	std::string intString = os.str();
	MessageBox(0, (LPCTSTR)intString.c_str(), "", MB_OK);
	*/
	if (dead) {
		return;
	}
	if (textureID == -1) {
		assert(false);
	}
	this->setMatrix(__states.unitVertices, __states.unitTexCoords, __states.modelUnitVertices, __states.modelTexCoords);
	__program2.SetProjectionMatrix(__projectionMatrix1);
	__program2.SetViewMatrix(__viewMatrix);
	__modelMatrix1 = glm::mat4(1.0f);
	__modelMatrix1 = glm::translate(__modelMatrix1, location->getLocation());
	__modelMatrix1 = glm::rotate(__modelMatrix1, glm::radians(location->getR()), glm::vec3(0.0f, 0.0f, 1));
	//__modelMatrix = glm::scale(__modelMatrix, size);
	__program2.SetModelMatrix(__modelMatrix1);
	
	if (repeat) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	}
	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}



void Unit::setTexture(const char* name, GameStatus& __states, bool repeat) {
	this->textureID = __states.textureIDs[name];
	this->repeat = repeat;
	/*
	std::stringstream os;
	os << textureID;
	std::string intString = os.str();
	MessageBox(0, (LPCTSTR)intString.c_str(), "", MB_OK);
	*/
}



Block::Block(BlockType type, float x, float y, float r) :Unit(3,5,50, x, y, r) {
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
		}
		return;

	case BlockType::Platform:
		if (checkCollisionBox(other)) {
			//__states.__GAMEWIN__();
			other->land();
		}
		return;
	}
}

void Unit::checkCollisionsY(Map* map) {
	// Probes for tiles
	glm::vec3 top = glm::vec3(location->getLocation().x, location->getLocation().y + (size.y / 2), location->getLocation().z);
	glm::vec3 top_left = glm::vec3(location->getLocation().x - (size.x / 2), location->getLocation().y + (size.y / 2), location->getLocation().z);
	glm::vec3 top_right = glm::vec3(location->getLocation().x + (size.x / 2), location->getLocation().y + (size.y / 2), location->getLocation().z);

	glm::vec3 bottom = glm::vec3(location->getLocation().x, location->getLocation().y - (size.y / 2), location->getLocation().z);
	glm::vec3 bottom_left = glm::vec3(location->getLocation().x - (size.x / 2), location->getLocation().y - (size.y / 2), location->getLocation().z);
	glm::vec3 bottom_right = glm::vec3(location->getLocation().x + (size.x / 2), location->getLocation().y - (size.y / 2), location->getLocation().z);

	float penetration_x = 0;
	float penetration_y = 0;
	if (map->IsSolid(top, &penetration_x, &penetration_y) && v->getV().y > 0) {
		location->move(glm::vec3(0, -penetration_y, 0), 1);
		v->accel(0, -v->getV().y, 0);
		collidedTop = true;
	}
	else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && v->getV().y > 0) {
		location->move(glm::vec3(0, -penetration_y, 0), 1);
		v->accel(0, -v->getV().y, 0);
		collidedTop = true;
	}

	else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && v->getV().y > 0) {
		location->move(glm::vec3(0, -penetration_y, 0), 1);
		v->accel(0, -v->getV().y, 0);
		collidedTop = true;
	}

	if (map->IsSolid(bottom, &penetration_x, &penetration_y) && v->getV().y < 0) {
		location->move(glm::vec3(0, penetration_y, 0), 1);
		v->accel(0, -v->getV().y, 0);
		collidedBottom = true;
	}
	else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && v->getV().y < 0) {
		location->move(glm::vec3(0, penetration_y, 0), 1);
		v->accel(0, -v->getV().y, 0);
		collidedBottom = true;
	}
	else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && v->getV().y < 0) {
		location->move(glm::vec3(0, penetration_y, 0), 1);
		v->accel(0, -v->getV().y, 0);
		collidedBottom = true;
	}
}

void Unit::checkCollisionsX(Map* map)
{
	// Probes for tiles
	glm::vec3 left = glm::vec3(location->getLocation().x - (size.x / 2), location->getLocation().y, location->getLocation().z);
	glm::vec3 right = glm::vec3(location->getLocation().x + (size.x / 2), location->getLocation().y, location->getLocation().z);

	float penetration_x = 0;
	float penetration_y = 0;
	if (map->IsSolid(left, &penetration_x, &penetration_y) && v->getV().x < 0) {
		location->move(glm::vec3(penetration_x, 0, 0), 1);
		v->accel(-v->getV().x, 0, 0);
		collidedLeft = true;
	}

	if (map->IsSolid(right, &penetration_x, &penetration_y) && v->getV().x > 0) {
		location->move(glm::vec3(-penetration_x, 0, 0), 1);
		v->accel(-v->getV().x, 0, 0);
		collidedRight = true;
	}
}



Projectile::Projectile(float speed, float power, float health, float duration, float x, float y, float r, GLuint texture,ProjType type)
	:Unit(speed, power, health, x, y, r), duration(duration, "d") ,type(type){
	this->textureID = texture;
}

void Projectile::update(float time, std::vector<Unit*> targets, Map* map) {
	duration.update(time);
	if (!duration.inEffect()) {
		getHit();
	}
	if (dead) return;
	move(time);
	processCollision(targets);
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
	onBoarder(map);
}


void Projectile::setAttribute(float speed, float power, float health, float duration, float x, float y, float r, GLuint texture) {
	location->setLocation(x, y, r);
	status->setStatus(speed, power, health);
	this->duration.gain(duration);
	this->textureID = texture;
}

ProjType Projectile::getType() const{
	return type;
}
