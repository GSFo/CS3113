#define GL_SILENCE_DEPRECATION



#define GL_GLEXT_PROTOTYPES 1
#include "Entity.h"
#include "Map.h"
#include <iomanip>
#include <vector>
#include<queue>
#include "Level1.h"
#include "Level0.h"
#include <SDL_Mixer.h>
float __deltaTime;
float __ticks;
float __lastTicks;

bool gameIsRunning = true;


const Uint8* __keys = SDL_GetKeyboardState(NULL);



enum class GameState { GAME, WIN, LOSE };



Scene* currentLevel;
int levelN = -1;
Mix_Music *music;
void Initialize() {
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	__lastTicks = (float)SDL_GetTicks() / 1000.0f;
	music = Mix_LoadMUS("music/BGM.mid");
	Mix_PlayMusic(music, -1);
}
bool begin = false;

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

	///* disabling player horizontal move
	if (begin) {
		if (__keys[SDL_SCANCODE_A]) {
			currentLevel->__entities.player->moveLeft(__deltaTime);
		}
		if (__keys[SDL_SCANCODE_D]) {
			currentLevel->__entities.player->moveRight(__deltaTime);
		}
		if (__keys[SDL_SCANCODE_W]) {
			currentLevel->__entities.player->moveUp(__deltaTime);
		}

		if (__keys[SDL_SCANCODE_S]) {
			currentLevel->__entities.player->moveDown(__deltaTime);
		}
		
		/*
		if (__keys[SDL_SCANCODE_SPACE]) {
			currentLevel->__entities.player->jump();
		}
		*/
		if (__keys[SDL_SCANCODE_K]) {
			currentLevel->__entities.player->activateSkill();
		}

		//shoot
		if (__keys[SDL_SCANCODE_J]) {
			currentLevel->playerShoot();
		}
	}
	else {
		if (__keys[SDL_SCANCODE_RETURN]) {
				begin = true;
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

bool Update() {
	currentLevel->Update(__deltaTime);
	if (currentLevel->__entities.switchScene) {
		levelN += 1;
		return true;
	}
	return false;
}

void Render() {
	currentLevel->Render();
}


void Shutdown() {
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	
	Initialize();
	currentLevel = new Level0;
	currentLevel->Initialize(3);
	

	Scene* levelLst[3];
	levelLst[0] = new Level1;
	levelLst[1] = new Level1;
	levelLst[2]=  new Level1;
	size_t life = 3;
	while (gameIsRunning) {
		ProcessInput();
		if (begin) {
			if (levelN == -1) {
				levelN += 1;
				currentLevel = levelLst[levelN];
				currentLevel->Initialize(life);
			}
			if (Update()) {
				currentLevel = levelLst[levelN];
				currentLevel->Initialize(life);
			};
		}
		Render();
	}
	Shutdown();
	return 0;
}