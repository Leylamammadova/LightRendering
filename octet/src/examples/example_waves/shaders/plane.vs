uniform mat4 modelToProjection;

// attributes from vertex buffer
attribute vec3 pos;
attribute vec3 normal;

// outputs
varying vec3 normal_;
varying vec3 frag_pos_;

void main() {
  gl_Position = modelToProjection * vec4(pos, 1.0);
  frag_pos_ = vec3(modelToProjection * vec4(pos, 1.0));
  normal_ = normal;
}