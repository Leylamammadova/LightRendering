varying vec3 normal_;

uniform vec3 object_colour_;
uniform vec3 light_colour_;

void main() {
    gl_FragColor = vec4(light_colour_ * object_colour_, 1.0);
}