uniform vec3 object_colour_;
uniform vec3 light_colour_;

uniform float shineDamper;
uniform float reflectivity;
uniform samplerCube cube_texture;

varying vec3 rel_normal_;
varying vec3 vert_to_light_;
varying vec3 vert_to_camera_;
varying vec3 world_pos;
varying vec3 reflectedVector;
varying vec3 refractedVector;

void main()
{
	float brightness = max(dot(normalize(rel_normal_), normalize(vert_to_light_)), 0.1);
	vec3 diffuse = brightness * light_colour_;

	// Specular
	vec3 norm_vert_to_camera = normalize(vert_to_camera_);
	vec3 light_to_vert = -vert_to_light_;
	vec3 reflected_light = reflect(light_to_vert, normalize(rel_normal_));

	float specular = max( dot( reflected_light, norm_vert_to_camera ), 0.0);
	float damping = pow(specular, shineDamper);
	vec3 specular_output = damping * reflectivity * light_colour_;
    

	 vec3 result = (diffuse + specular_output) * object_colour_;

   vec4 color1 = textureCube(cube_texture,reflectedVector);
   vec4 color2 = textureCube(cube_texture,refractedVector);
   vec4 color = color1; //mix(color1,color2, 0.5);
    gl_FragColor = color;
}