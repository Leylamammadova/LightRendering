
uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToProjection;


uniform vec3 light_pos_;
uniform vec3 view_pos_;

// attributes from vertex buffer
attribute vec3 pos;
attribute vec3 normal;

// outputs
varying vec3 rel_normal_;
varying vec3 vert_to_light_;
varying vec3 vert_to_camera_;
varying vec3 world_pos;


void make_lighting() {
  vec4 world_pos = modelToWorld * vec4(pos, 1.0);
  rel_normal_ = (modelToWorld * vec4(normal,0.0)).xyz;
  vert_to_light_ = light_pos_ - world_pos.xyz;

  vert_to_camera_ = view_pos_ - world_pos.xyz;
}

void main() {
  make_lighting();

  gl_Position = (cameraToProjection * worldToCamera * modelToWorld) * vec4(pos,1.0);
}

