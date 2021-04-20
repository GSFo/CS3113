#include "GameStates.h"

//my loadTexture function is following a different logic
void GameStatus::loadTexture(std::string& name, const char* filePath) {

	textureIDs[name] = Utility::loadTexture(filePath);
}
