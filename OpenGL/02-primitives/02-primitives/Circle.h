#ifndef _CIRCLE_INCLUDE
#define _CIRCLE_INCLUDE

#include <GL/glew.h>
#include <GL/gl.h>
#include <cmath>
#include "../ShaderProgram.h"

#pragma once

class Circle
{
private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation;
	int vertexCount;
public:
	Circle(float x, float y, float radii, int N, ShaderProgram &program);
	static Circle *createCircle(float x, float y, float radii, int N, ShaderProgram &program);
	void render() const;
	void free();
};

#endif // _CIRCLE_INCLUDE

