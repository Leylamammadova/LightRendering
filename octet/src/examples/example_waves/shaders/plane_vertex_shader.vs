#version 330 core

layout (location = 0) in vec3 pos;

void main() {
  gl_Position = (pos, 1.0f);
}