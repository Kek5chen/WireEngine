#ifndef FRAGMENT_SHADER_H
#define FRAGMENT_SHADER_H

const char* fragment_shader_source = R"(
#version 330 core

out vec3 color;

void main(){
  color = vec3(1,0,0);
}
)";


#endif //FRAGMENT_SHADER_H
