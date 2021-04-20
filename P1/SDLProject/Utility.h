#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif


#define GL_GLEXT_PROTOTYPES 1
#include<vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
class Utility
{
public:
	static void DrawTextGL(ShaderProgram* program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position);
	static GLuint loadTexture(const char* filename);
};

