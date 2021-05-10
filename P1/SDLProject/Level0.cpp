#include "Level0.h"


#pragma once

void Level0::Initialize(size_t lifeCount) {
	__entities.switchScene = false;
	__displayWindow = SDL_CreateWindow("TIME TO FIGHT BACK!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(__displayWindow);
	SDL_GL_MakeCurrent(__displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);

	__program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
	__viewMatrix = glm::mat4(1.0f);
	__modelMatrix1 = glm::mat4(1.0f);
	__projectionMatrix1 = glm::ortho(-5.0f, 5.0f, -4.0f, 4.0f, -1.0f, 1.0f);
	__program.SetProjectionMatrix(__projectionMatrix1);
	__program.SetViewMatrix(__viewMatrix);
	__program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);

	glUseProgram(__program.programID);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);


	//load textures
	std::string a("font1");
	__states.loadTexture(a, "fonts/font1.png");


	//transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	__states.unitVertices = new float[12]{ -.5, -.5, .5, -.5, .5, .5, -.5, -.5, .5, .5, -.5, 0.5 };
	__states.modelUnitVertices = new float[12]{ -.5, -.5, .5, -.5, .5, .5, -.5, -.5, .5, .5, -.5, 0.5 };
	__states.unitTexCoords = new float[12]{ 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
	__states.modelTexCoords = new float[12]{ 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
	__states.state = GameStatus::GameState::GAME;
}

void Level0::Update(float deltaTime) {
}
void Level0::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glVertexAttribPointer(__program.positionAttribute, 2, GL_FLOAT, false, 0, __states.unitVertices);
	glEnableVertexAttribArray(__program.positionAttribute);
	glVertexAttribPointer(__program.texCoordAttribute, 2, GL_FLOAT, false, 0, __states.unitTexCoords);
	glEnableVertexAttribArray(__program.texCoordAttribute);
	//draw items

	//MessageBox(0, "Prepared to render", "", MB_OK);
	Utility::DrawTextGL(&__program, __states.textureIDs["font1"], "TIME TO FIGHT BACK!", .6f, -.3f, glm::vec3(-4, 1, 0));
	Utility::DrawTextGL(&__program, __states.textureIDs["font1"], "Press WASD To Move, J:Shoot, K:Shield Defense", .25f, -.15f, glm::vec3(-4, 2, 0));
	Utility::DrawTextGL(&__program, __states.textureIDs["font1"], "Press enter to start the game", .4f, -.25f, glm::vec3(-4, 0, 0));


	glDisableVertexAttribArray(__program.positionAttribute);
	glDisableVertexAttribArray(__program.texCoordAttribute);

	SDL_GL_SwapWindow(__displayWindow);
}



void Level0::spawnNPC(GLuint num) {};
void Level0::updateNPC(float time) {};
void Level0::renderNPC() {};

void Level0::playerShoot() {};
Gengar* Level0::spawnBoss() { return new Gengar(1, 2, 3); };//placeholder


