#ifndef FRAGMENT_SHADER_H
#define FRAGMENT_SHADER_H

const char* fragment_shader_source = R"(
#version 330 core

in vec3 color;

out vec4 FragColor;

void main() {
	FragColor = vec4(color, 1.0);
}
)";


#endif //FRAGMENT_SHADER_H
