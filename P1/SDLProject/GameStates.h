#pragma once
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#include <hash_map>
#include <queue>

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "Utility.h"


class GameStatus {
public:
	static enum class GameState { GAME, WIN, LOSE };
	float* modelUnitVertices;
	float* unitVertices;
	float* unitTexCoords;
	float* modelTexCoords;
	GameState state;
	std::hash_map<std::string, GLuint> textureIDs;
	void loadTexture(std::string& name, const char* filename);
	//p2 code. Unit* playerB;//players are not in a list, since each player would require one control method
	void __GAMEOVER__() {
		state = GameState::LOSE;
		//gameIsRunning = false;
		//SDL_Quit();
	};

	void __GAMEWIN__() {
		state = GameState::WIN;
	};
};

