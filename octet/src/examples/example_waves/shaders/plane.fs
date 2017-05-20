varying vec3 normal_;
varying vec3 frag_pos_;

uniform vec3 object_colour_;
uniform vec3 light_colour_;
uniform vec3 light_pos_;
uniform vec3 view_pos_;

void main() {
    // Ambient
    float ambient_intensity = 0.1;
    vec3 ambient = ambient_intensity * light_colour_;
    
    // Diffuse
    vec3 norm = normalize(normal_);
    vec3 light_dir = normalize(light_pos_ - frag_pos_);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light_dir;

    // Specular
    float specular_intensity = 0.5;
    vec3 view_dir = normalize(view_pos_ - frag_pos_);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular = specular_intensity * spec * light_colour_;

    vec3 result = (ambient + diffuse + specular) * object_colour_;
    gl_FragColor = vec4(result, 1.0);
}