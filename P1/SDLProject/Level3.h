
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include "Scene.h"
#include "Entity.h"
class Level3 : public Scene {

public:
	void Initialize(size_t lifeCount) override;
	void Update(float deltaTime) override;
	void Render() override;
	void spawnNPC(GLuint num) override;
	void updateNPC(float time) override;
	void renderNPC() override;
	SDL_Window* __displayWindow;
	glm::mat4 __viewMatrix, __modelMatrix1, __projectionMatrix1;
	ShaderProgram __program;
	GameStatus __states;

};


