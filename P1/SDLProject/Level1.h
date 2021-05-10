
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include "Scene.h"
#include "Entity.h"
#include <SDL_mixer.h>

class Level1 : public Scene {
	bool lockView = false;
public:
	void Initialize(size_t lifeCount) override;
	void Update(float deltaTime) override;
	void Render() override;
	void spawnNPC(size_t num) override;
	Gengar* spawnBoss() override;
	void updateNPC(float time) override;
	void renderNPC() override;
	void playerShoot() override;
	Mix_Chunk* failureChunk;
	SDL_Window* __displayWindow;
	glm::mat4 __viewMatrix, __modelMatrix1, __projectionMatrix1;
	ShaderProgram __program;
	GameStatus __states;
	Gengar* bossPtr;

};


