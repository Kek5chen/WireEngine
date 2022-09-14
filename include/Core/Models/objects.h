#ifndef OBJECTS_H
#define OBJECTS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct vector3
{
	float x;
	float y;
	float z;
};

struct vector4
{
	float x;
	float y;
	float z;
	float w;
};

struct matrix4
{
	float m[4][4];
};

struct vertex
{
	vector3 position;
	vector3 color;
};

class wire_model_base
{
public:
	GLuint shader_program = 0;
	GLuint vertex_buffer = 0;

	wire_model_base(GLuint vertex_buffer, GLuint shader_program);
	virtual void draw() = 0;
	virtual ~wire_model_base();
};

class wire_triangle : public wire_model_base
{
private:
	vertex v[3];
public:
	wire_triangle(vertex v1, vertex v2, vertex v3);
	void draw();
};

#endif //OBJECTS_H
