varying vec3 normal_;

uniform vec3 object_colour_;
uniform vec3 light_colour_;

void main() {
    float ambient_intensity = 0.1f;
    vec3 ambient = ambient_intensity * light_colour_;
    vec3 result = ambient * object_colour_;
    
    gl_FragColor = vec4(result, 1.0);
}