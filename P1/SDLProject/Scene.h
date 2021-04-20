#pragma once
#define GL_SILENCE_DEPRECATION
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1 
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Utility.h"
#include "Entity.h"
#include "Map.h"

#include "EntityCollection1.h"


class Scene {
public:
	EntityCollection __entities;
	virtual void Initialize(size_t lifeCount) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void spawnNPC(GLuint num)=0;
	virtual void updateNPC(float time)=0;
	virtual void renderNPC()=0;
};