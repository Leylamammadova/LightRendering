
namespace octet {
  class ObjReader {
  public: 
    ref<mesh> load_file(char* file_name) {
      std::vector<float> verticies;
      std::vector<float> normals;
      std::vector<uint32_t> face_vert;
      std::vector<uint32_t> face_norm;

      std::ifstream is(file_name);
      if (is.bad() || !is.is_open()) return NULL;
      // store the line here
      char buffer[2048];
      // loop over lines
      while (!is.eof()) {
        is.getline(buffer, sizeof(buffer));
        // loop over columns
        char *b = buffer;
        //printf("%c\n", *b);
        if (*b == '#') continue; // Ignore lines starting with #

        //std::string sett = b;
        //settings.push_back(sett);

        char *line = strtok(b, " ");
        while (line != NULL) {
          char* type = line;
          line = strtok(NULL, " ");
          while (line != NULL) {
            if (!std::strcmp(type, "v")) {
              verticies.push_back(std::stof(line));
              printf("vertex: %s\n", line);
            }
            else if (!std::strcmp(type, "vn")) {
              normals.push_back(std::stof(line));
            }
            else if (!std::strcmp(type, "f")) {
              char *face = strtok(line, "//");
              face_vert.push_back(std::stoi(face));
              face = strtok(NULL, "//");
              face_norm.push_back(std::stoi(face));
            }
            line = strtok(NULL, " ");
          }
        }
      }
      printf("Num of verts: %d\n", verticies.size() / 3);
      printf("Num of norms: %d\n", normals.size() / 3);
      printf("Num of faces: %d\n", face_vert.size() / 3);

      ref<mesh> load_mesh = new mesh();

      // allocate vertices and indices into OpenGL buffers
      load_mesh->allocate(sizeof(float) * verticies.size(), sizeof(uint32_t) * face_vert.size());
      load_mesh->set_params(sizeof(uint32_t), face_vert.size(), verticies.size(), GL_TRIANGLES, GL_UNSIGNED_INT);

      // describe the structure of my_vertex to OpenGL
      load_mesh->add_attribute(attribute_pos, 3, GL_FLOAT, 0);
      //load_mesh->add_attribute(attribute_normal, 3, GL_FLOAT, 12);

      gl_resource::wolock il(load_mesh->get_indices());
      uint32_t *indx = il.u32();
      memcpy(indx, face_vert.data(), sizeof(uint32_t)*face_vert.size());

      gl_resource::wolock vl(load_mesh->get_vertices());
      float *vtx = (float *)vl.u8();
      memcpy(vtx, verticies.data(), sizeof(float)*verticies.size());

      return load_mesh;
    }
  };
}