
namespace octet {
  class obj_file_io {
  public: 

    obj_file_io(){}
    ~obj_file_io(){}

    struct obj_data {
      std::vector< unsigned int > vertexIndices;
      std::vector< unsigned int > uvIndices;
      std::vector< unsigned int > normalIndices;
      std::vector< float > vertices;
      std::vector< float > uvs;
      std::vector< float > normals;
    };

    struct opengl_data {
      std::vector<float> vertex_object;
      std::vector<unsigned int> indices;
      int * atributeSizes; // Size of the atribs in bytes in order. So vtx, normal etc == 3 * sizeof(float), 3 * sizeof(float) etc..
    };

    std::string load_file(const char* file_path) {
      std::ifstream file(file_path);
      if (file.bad() || !file.is_open()) { return nullptr; }
      char buffer[2048];
      std::string out;
      while (!file.eof()) {
        file.getline(buffer, sizeof(buffer));
        out += buffer;
        out += "\n";
      }
      return out;
    }

    obj_data load_mesh_file(char* file_name) {
      obj_data data;

      FILE * file = fopen(file_name, "r");
      if (file == NULL) {
        printf("Impossible to open the file !\n");
        return data;
      }
      while (1) {

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
          break; // EOF = End Of File. Quit the loop.

        if (strcmp(lineHeader, "v") == 0) {
          float x, y, z;
          fscanf(file, "%f %f %f\n", &x, &y, &z);
          data.vertices.push_back(x);
          data.vertices.push_back(y);
          data.vertices.push_back(z);
        }
        else if (strcmp(lineHeader, "vt") == 0) {
          float tx, ty;
          fscanf(file, "%f %f\n", &tx, &ty);
          data.uvs.push_back(tx);
          data.uvs.push_back(ty);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
          float nx, ny, nz;
          fscanf(file, "%f %f %f\n", &nx, &ny, &nz);
          data.normals.push_back(nx);
          data.normals.push_back(ny);
          data.normals.push_back(nz);
        }
        else if (strcmp(lineHeader, "f") == 0) {
          std::string vertex1, vertex2, vertex3;
          unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
          int matches = fscanf(file, "%u/%u/%u %u/%u/%u %u/%u/%u\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
          if (matches != 9) {
            matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
            if (matches != 6) {
              matches = fscanf(file, "%d/%d/ %d/%d/ %d/%d/\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
              if (matches != 6) {
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return data;
              }
            }
            
          }
          data.vertexIndices.push_back(vertexIndex[0]);
          data.vertexIndices.push_back(vertexIndex[1]);
          data.vertexIndices.push_back(vertexIndex[2]);
          data.uvIndices.push_back(uvIndex[0]);
          data.uvIndices.push_back(uvIndex[1]);
          data.uvIndices.push_back(uvIndex[2]);
          data.normalIndices.push_back(normalIndex[0]);
          data.normalIndices.push_back(normalIndex[1]);
          data.normalIndices.push_back(normalIndex[2]);
        }

      }

      return data;
    }

    opengl_data parse_file_data(obj_data in_data) {
      opengl_data out_data;
      // Set the index from base 1 (obj format) to base 0 (opengl format)
      for (int i = 0; i < in_data.vertexIndices.size(); i++) {
        in_data.vertexIndices[i] -= 1;
        in_data.uvIndices[i] -= 1;
        in_data.normalIndices[i] -= 1;
      }

      for (int i = 0; i < in_data.vertexIndices.size(); i ++) {
        // Construct element
        std::vector<float> element;
        element.push_back(in_data.vertices[ 3 * in_data.vertexIndices[i]     ]); // x
        element.push_back(in_data.vertices[ 3 * in_data.vertexIndices[i] + 1 ]); // y
        element.push_back(in_data.vertices[ 3 * in_data.vertexIndices[i] + 2 ]); // x
        element.push_back(in_data.normals[  3 * in_data.normalIndices[i]     ]); // Norm vec for the above vertex
        element.push_back(in_data.normals[  3 * in_data.normalIndices[i] + 1 ]);
        element.push_back(in_data.normals[  3 * in_data.normalIndices[i] + 2 ]);
        if (in_data.uvs.size() != 0) {
          element.push_back(in_data.uvs[2 * in_data.uvIndices[i]]); // u
          element.push_back(in_data.uvs[2 * in_data.uvIndices[i] + 1]); // v
        }
        // Check if element already exsist in ouput,
        unsigned int index;
        if (is_element_in_object(element, out_data.vertex_object, index)) {
          //  if true then look up the index of that element and add it to the new indices
          out_data.indices.push_back(index);
        }
        else {
          //  if false then add the new element to the output and look up it's location to update the indices
          out_data.vertex_object.insert(out_data.vertex_object.end(), element.begin(), element.end());
          out_data.indices.push_back(index);
        }
      }
      return out_data;
    }


  private:
    bool is_element_in_object(std::vector<float> element, std::vector<float> object, unsigned int &idx) {
      unsigned int elementIdx = 0;
      for (int i = 0; i < object.size(); i = i + element.size()) {
        bool match = true;
        for (int j = 0; j < element.size(); j++) {
          if (element[j] != object[i + j]) {
            match = false;
            break; // Element in obj not the same as check element so break to skip to next element in object.
          }

        }

        if (match) {
          idx = elementIdx;
          return true;
        }

        elementIdx++;
      }
      idx = elementIdx;
      return false;
    }

    //std::vector<float> load_file_vertdata(char* file_name) {
    //  std::vector<float> verticies;
    //  std::vector<float> normals;
    //  std::vector<uint32_t> face_vert;
    //  std::vector<uint32_t> face_norm;

    //  std::ifstream is(file_name);
    //  //if (is.bad() || !is.is_open()) return NULL;
    //  // store the line here
    //  char buffer[2048];
    //  // loop over lines
    //  while (!is.eof()) {
    //    is.getline(buffer, sizeof(buffer));
    //    // loop over columns
    //    char *b = buffer;
    //    //printf("%c\n", *b);
    //    if (*b == '#') continue; // Ignore lines starting with #

    //                             //std::string sett = b;
    //                             //settings.push_back(sett);

    //    char *line = strtok(b, " ");
    //    while (line != NULL) {
    //      char* type = line;
    //      line = strtok(NULL, " ");
    //      while (line != NULL) {
    //        if (!std::strcmp(type, "v")) {
    //          verticies.push_back(std::stof(line));
    //          printf("vertex: %s\n", line);
    //        }
    //        else if (!std::strcmp(type, "vn")) {
    //          normals.push_back(std::stof(line));
    //        }
    //        else if (!std::strcmp(type, "f")) {
    //          char *face = strtok(line, "//");
    //          face_vert.push_back(std::stoi(face));
    //          face = strtok(NULL, "//");
    //          face_norm.push_back(std::stoi(face));
    //        }
    //        line = strtok(NULL, " ");
    //      }
    //    }
    //  }
    //  printf("Num of verts: %d\n", verticies.size() / 3);
    //  printf("Num of norms: %d\n", normals.size() / 3);
    //  printf("Num of faces: %d\n", face_vert.size() / 3);

    //  ref<mesh> load_mesh = new mesh();

    //  // allocate vertices and indices into OpenGL buffers
    //  load_mesh->allocate(sizeof(float) * verticies.size(), sizeof(uint32_t) * face_vert.size());
    //  load_mesh->set_params(sizeof(uint32_t), face_vert.size(), verticies.size(), GL_TRIANGLES, GL_UNSIGNED_INT);

    //  // describe the structure of my_vertex to OpenGL
    //  load_mesh->add_attribute(attribute_pos, 3, GL_FLOAT, 0);
    //  //load_mesh->add_attribute(attribute_normal, 3, GL_FLOAT, 12);

    //  gl_resource::wolock il(load_mesh->get_indices());
    //  uint32_t *indx = il.u32();
    //  memcpy(indx, face_vert.data(), sizeof(uint32_t)*face_vert.size());

    //  gl_resource::wolock vl(load_mesh->get_vertices());
    //  float *vtx = (float *)vl.u8();
    //  memcpy(vtx, verticies.data(), sizeof(float)*verticies.size());

    //  return verticies;
    //}
    //static std::vector<int> load_file_facedata(char* file_name) {
    //  std::vector<float> verticies;
    //  std::vector<float> normals;
    //  std::vector<int> face_vert;
    //  std::vector<int> face_norm;

    //  std::ifstream is(file_name);
    //  //if (is.bad() || !is.is_open()) return NULL;
    //  // store the line here
    //  char buffer[2048];
    //  // loop over lines
    //  while (!is.eof()) {
    //    is.getline(buffer, sizeof(buffer));
    //    // loop over columns
    //    char *b = buffer;
    //    //printf("%c\n", *b);
    //    if (*b == '#') continue; // Ignore lines starting with #

    //                             //std::string sett = b;
    //                             //settings.push_back(sett);

    //    char *line = strtok(b, " ");
    //    while (line != NULL) {
    //      char* type = line;
    //      line = strtok(NULL, " ");
    //      while (line != NULL) {
    //        if (!std::strcmp(type, "v")) {
    //          verticies.push_back(std::stof(line));
    //          printf("vertex: %s\n", line);
    //        }
    //        else if (!std::strcmp(type, "vn")) {
    //          normals.push_back(std::stof(line));
    //        }
    //        else if (!std::strcmp(type, "f")) {
    //          char *face = strtok(line, "//");
    //          face_vert.push_back(std::stoi(face));
    //          face = strtok(NULL, "//");
    //          face_norm.push_back(std::stoi(face));
    //        }
    //        line = strtok(NULL, " ");
    //      }
    //    }
    //  }
    //  printf("Num of verts: %d\n", verticies.size() / 3);
    //  printf("Num of norms: %d\n", normals.size() / 3);
    //  printf("Num of faces: %d\n", face_vert.size() / 3);

    //  ref<mesh> load_mesh = new mesh();

    //  // allocate vertices and indices into OpenGL buffers
    //  load_mesh->allocate(sizeof(float) * verticies.size(), sizeof(uint32_t) * face_vert.size());
    //  load_mesh->set_params(sizeof(uint32_t), face_vert.size(), verticies.size(), GL_TRIANGLES, GL_UNSIGNED_INT);

    //  // describe the structure of my_vertex to OpenGL
    //  load_mesh->add_attribute(attribute_pos, 3, GL_FLOAT, 0);
    //  //load_mesh->add_attribute(attribute_normal, 3, GL_FLOAT, 12);

    //  gl_resource::wolock il(load_mesh->get_indices());
    //  uint32_t *indx = il.u32();
    //  memcpy(indx, face_vert.data(), sizeof(uint32_t)*face_vert.size());

    //  gl_resource::wolock vl(load_mesh->get_vertices());
    //  float *vtx = (float *)vl.u8();
    //  memcpy(vtx, verticies.data(), sizeof(float)*verticies.size());

    //  return face_vert;
    //}




    //ref<mesh> load_mesh_fileold(char* file_name) {
    //  std::vector<float> verticies;
    //  std::vector<float> normals;
    //  std::vector<uint32_t> face_vert;
    //  std::vector<uint32_t> face_norm;

    //  std::ifstream is(file_name);
    //  if (is.bad() || !is.is_open()) return NULL;
    //  // store the line here
    //  char buffer[2048];
    //  // loop over lines
    //  while (!is.eof()) {
    //    is.getline(buffer, sizeof(buffer));
    //    // loop over columns
    //    char *b = buffer;
    //    //printf("%c\n", *b);
    //    if (*b == '#' || *b == '\0') continue; // Ignore lines starting with #

    //    char *line = strtok(b, " ");
    //    while (line != NULL) {
    //      char* type = line;
    //      line = strtok(NULL, " ");
    //      while (line != NULL) {
    //        if (!std::strcmp(type, "v")) {
    //          verticies.push_back(std::stof(line));
    //        }
    //        else if (!std::strcmp(type, "vn")) {
    //          normals.push_back(std::stof(line));
    //        }
    //        else if (!std::strcmp(type, "f")) {
    //          char tokenBuff[128];
    //          strcpy(tokenBuff, line);
    //          char *faceToken = strtok(tokenBuff, "//");
    //          face_vert.push_back(std::stoi(faceToken));
    //          faceToken = strtok(NULL, "//");
    //          face_norm.push_back(std::stoi(faceToken));
    //          faceToken = strtok(NULL, "//");
    //        }

    //        line = strtok(NULL, " ");
    //      }
    //    }
    //  }
    //  printf("Num of verts: %d\n", verticies.size() / 3);
    //  printf("Num of norms: %d\n", normals.size() / 3);
    //  printf("Num of faces: %d\n", face_vert.size() / 3);

    //  ref<mesh> load_mesh = new mesh(verticies.size(), face_vert.size());

    //  // allocate vertices and indices into OpenGL buffers
    //  load_mesh->allocate(sizeof(float) * verticies.size(), sizeof(uint32_t) * face_vert.size());
    //  load_mesh->set_params(sizeof(uint32_t), face_vert.size(), verticies.size(), GL_TRIANGLES, GL_UNSIGNED_INT);

    //  load_mesh->add_attribute(attribute_pos, 3, GL_FLOAT, 0);

    //  load_mesh->get_indices()->assign(&face_vert[0], 0, sizeof(unsigned int)*face_vert.size());
    //  load_mesh->get_vertices()->assign(&verticies[0], 0, sizeof(float)*verticies.size());

    //  return load_mesh;
    //}

  };
}