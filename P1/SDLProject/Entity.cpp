#include "Entity.h"
//gameplay constants
float GRAVITY = -5;
float FRICTION = -1.5;
int sign(float a) {
	if (a > 0) return 1;
	if (a < 0) return -1;
	return 0;
}
enum class UnitType{Gasty, Haunter, Player, NA};

class Player;
void Unit::moveLeft(float time) { if (isAlive()) { v->accel(-status->getSpeed() * time, 0, 0); } }
void Unit::moveRight(float time) { if (isAlive()) v->accel(status->getSpeed() * time, 0, 0); }
		void Unit::moveUp(float time) { if (isAlive()) v->accel(0, status->getSpeed() * time, 0); }
		void Unit::moveDown(float time) { if (isAlive()) v->accel(0, -status->getSpeed() * time, 0); }
	
		void Unit::accelR(float r) {
			v->accelR(r); 
		};
		float Unit::getDistance(Unit* other) const {
			return std::sqrt(std::pow(location->getX() - other->location->getX(), 2) + std::pow(location->getY() - other->location->getY(), 2));
		}

		UnitType Unit::getType() const { return type; };
		const glm::vec3& Unit::getV() const { return v->getV(); }
		bool Unit::isAlive() const { return !dead; }
		bool Unit::landed() const { return collidedBottom; }


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
		if (location->getY() > map->getHeight() - abs(size.y) / 2 || location->getY() < abs(size.y) / 2 - map->getHeight()) {
			if (Unit::type == UnitType::Player) {
				if (location->getY() < abs(size.y) / 2 - map->getHeight()) {
					die();
					bounceY();
					boarderFlagY = true;
				}
			}
			else {
				bounceY();
				boarderFlagY = true;
			}
		}
	}
	else {
		if (!(location->getY() > map->getHeight() - abs(size.y) / 2 || location->getY() < abs(size.y) / 2 - map->getHeight())) {
			boarderFlagY = false;
		}
	}
	return boarderFlagX || boarderFlagY;
}


	Player::Player(float speed, float x, float y, float r,size_t lifeCount) :Unit(speed, x, y, r), powerShield(15, 10, "power shield") ,lifeCount(lifeCount){ 
		Unit::type = UnitType::Player; 
		failureChunk = Mix_LoadWAV("music/Assets_Audio_failure.wav");
	};
	void Player::update(float time, Player* player, Map* map) {
		accel(0, GRAVITY * time, 0);
		if (landed()) {
			if (abs(getV().x) > abs(FRICTION * time)) {
				accel(sign(getV().x) * FRICTION * time, 0, 0);
			}
			else {
				accel(-getV().x, 0, 0);
			}
		}

		move(time);

		powerShield.update(time);
		if (powerShield.inEffect()) {
			if (!boosted) { boosted = true; size.x *= 1.6; }
		}
		else {
			if (boosted) { boosted = false; size.x /= 1.6; }
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

	if (onBoarder(map)) { die(); }
	Unit::move(time);
	Block::update(time, player, map);
}
Gasty::Gasty(float x, float y, float z) :Block(BlockType::Deadly, x, y, z), voidForm(10.0, 3.0, "void form") { Unit::type = UnitType::Gasty; };

void Gasty::render(ShaderProgram __program2, glm::mat4 __viewMatrix, glm::mat4 __modelMatrix1, glm::mat4 __projectionMatrix1,GameStatus __states) const {
	if (!voidForm.inEffect()) {
		Unit::render(__program2, __viewMatrix, __modelMatrix1, __projectionMatrix1,__states);
	}
}

void Gasty::update(float time, Player* player, Map* map) {
	if (dead) {
		return;
	}
	voidForm.update(time);
	if (getDistance(player) < 2) {
		if (player->isBoosted()) {
			voidForm.activate();
		}
	}
	processCollision(player);

}

void Gasty::processCollision(Player* other) {
	if (!voidForm.inEffect()) {
		if (!collisionFlag) {
			if (checkCollisionBox(other)) {

				if (other->isBoosted()) {
					die();
					return;
				}
				other->die();
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





	Haunter::Haunter(float speed, float x, float y, float r) :Unit(speed, x, y, r) { Unit::type = UnitType::Haunter; };
	

void Haunter::processCollision(Player* other) {
	if (!collisionFlag) {
		if (checkCollisionBox(other)) {
			die();
			if (!other->isBoosted()) {
				other->die();

				return;
			}
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

void Haunter::update(float time, Player* player, Map* map) {
	if (dead) return;
	move(time);
	processCollision(player);
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





Unit::Unit(float speed, float x, float y, float r) {
	status = new Status(speed);
	location = new Location(x, y, r);
	v = new Velocity();
	textureID = -1;
	size = glm::vec3(1, 1, 1);
	type = UnitType::NA;
};

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
			other->die();
			die();
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

void Unit::die() { dead = true; }
void Player::die(){
	Mix_PlayChannel(-1, failureChunk, 0);
	lifeCount -= 1;
	if (lifeCount == 0) {
		dead = true;
	}
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

void Unit::render(ShaderProgram __program2, glm::mat4 __viewMatrix, glm::mat4 __modelMatrix1, glm::mat4 __projectionMatrix1,GameStatus __states) const {
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



void Unit::setTexture(const char* name, GameStatus __states, bool repeat) {
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
