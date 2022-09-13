#ifndef VERTEX_SHADER_H
#define VERTEX_SHADER_H

const char* vertex_shader_source = R"(
#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;

out vec3 color;

void main() {

	// Output position of the vertex, in clip space : MVP * position
	gl_Position = vec4(pos, 1.0);

	// Output color of the vertex, in clip space : MVP * position
	color = col;

}
)";


#endif //VERTEX_SHADER_H
