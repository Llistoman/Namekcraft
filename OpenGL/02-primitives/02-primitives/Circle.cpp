#include "Circle.h"
#include <cmath>

Circle *Circle::createCircle(float x, float y, float radii, int N, ShaderProgram &program) {
	Circle *circle = new Circle(x, y, radii, N, program);
	return circle;
}

Circle::Circle(float x, float y, float radii, int N, ShaderProgram &program)
{
	//int vertexCount = 30;
	//float radius = 1.0f;
	//float center_x = 0.0f;
	//float center_y = 0.0f;

	vertexCount = N;

	// Create a buffer for vertex data
	float* buffer = new float[N * 2]; // (x,y) for each vertex
	int idx = 0;

	// Center vertex for triangle fan
	buffer[idx++] = x;
	buffer[idx++] = y;

	// Outer vertices of the circle
	int outerVertexCount = N - 1;

	for (int i = 0; i < outerVertexCount; ++i){
		float percent = (i / (float)(outerVertexCount - 1));
		float rad = percent * 2 * 3.1415f;

		//Vertex position
		float outer_x = x + radii * cos(rad);
		float outer_y = y + radii * sin(rad);

		buffer[idx++] = outer_x;
		buffer[idx++] = outer_y;
	}
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, N * 2 * sizeof(float), buffer, GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2);
}


void Circle::render() const {
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
}

void Circle::free() {
	glDeleteBuffers(1, &vbo);
}

