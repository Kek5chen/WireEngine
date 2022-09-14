#include "objects.h"


wire_model_base::wire_model_base(GLuint vertex_buffer, GLuint shader_program)
{
	this->vertex_buffer = vertex_buffer;
	this->shader_program = shader_program;
}

wire_model_base::~wire_model_base()
{
	glDeleteBuffers(1, &vertex_buffer);
}

wire_triangle::wire_triangle(vertex v1, vertex v2, vertex v3)
	: wire_model_base(0, 0)
{
	glGenBuffers(1, &vertex_buffer);
	this->v[0] = v1;
	this->v[1] = v2;
	this->v[2] = v3;
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * 3, v, GL_STATIC_DRAW);
}

void wire_triangle::draw()
{
	glUseProgram(shader_program);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *) 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *) (sizeof(vector3)));
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
