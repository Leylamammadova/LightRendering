
uniform mat4 modelToProjection;

// attributes from vertex buffer
attribute vec3 pos;
attribute vec3 normal;

// outputs
varying vec3 normal_;

void main() {
  gl_Position = modelToProjection * vec4(pos,1.0);
  normal_ = normal;
}
