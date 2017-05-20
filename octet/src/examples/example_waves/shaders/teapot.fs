uniform vec3 object_colour_;
uniform vec3 light_colour_;

varying vec3 rel_normal_;
varying vec3 vert_to_light_;
varying vec3 world_pos;

void main()
{
	float brightness = max(dot(normalize(rel_normal_), normalize(vert_to_light_)), 0.1);
	vec3 diffuse = brightness * light_colour_;



    gl_FragColor = vec4(diffuse, 1.0) * vec4(object_colour_,1.0);
}