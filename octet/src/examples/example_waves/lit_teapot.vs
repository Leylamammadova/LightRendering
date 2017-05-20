uniform mat4 modelToProjection;
//uniform mat4 modelToCamera;

// attributes from vertex buffer
attribute vec3 pos;
attribute vec3 normal;


//attribute vec2 uv;
//attribute vec4 color;

// outputs
varying vec3 normal_;


//varying vec2 uv_;
//varying vec4 color_;
//varying vec3 model_pos_;
//varying vec3 camera_pos_;



void main() {
  gl_Position = modelToProjection * vec4(pos,1.0);
  normal_ = normal;
  
  //vec3 tnormal = (modelToCamera * vec4(normal, 0.0)).xyz;
  //vec3 tpos = (modelToCamera * pos).xyz;
  //normal_ = tnormal;
  //uv_ = uv;
  //color_ = vec4(1.0, 0.0, 0.0, 1.0);
  //camera_pos_ = tpos;
  //model_pos_ = pos.xyz;
  
    vec3 lightDir;
	vec4 diffuse, ambient, specular, globalAmbient;
	float NdotL;

	normal = normalize(gl_NormalMatrix * gl_Normal);
	lightDir = normalize(vec3(gl_LightSource[0].position));
	NdotL = max(dot(normal, lightDir), 0.0);
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	specular = gl_FrontMaterial.specular * gl_LightSource[0].specular;
	globalAmbient = gl_LightModel.ambient * gl_FrontMaterial.ambient;
	gl_FrontColor =  NdotL * diffuse + globalAmbient + ambient+specular;
	gl_Position = ftransform();

}

