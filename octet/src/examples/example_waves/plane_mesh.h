#pragma once

namespace octet {
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

  class plane_mesh {
    GLuint VAO;
    std::vector<GLuint> indices;
    shader plane_shader;

  public:
    plane_mesh() {}
    
    void Init(int size_x = 100, int size_y = 100) {

      plane_shader.init(load_file("plane_vertex_shader.vs").c_str(),
        load_file("plane_fragment_shader.fs").c_str());

      std::vector<GLfloat> plane;
      for (int i = 0; i < 3 * (size_x * size_y); i++) {
        plane.push_back(GLfloat(i % size_x));
        plane.push_back(0.0f); 
        plane.push_back(GLfloat(i / size_x));
      }

      int i = 0;
      for (int z = 0, t = 0; z < size_y - 1; ++z) {
        for (int x = 0; x < size_x - 1; ++x) {
          indices[i] = t;
          indices[i + 1] = t + x;
          indices[i + 2] = t + x + 1;

          indices[i + 3] = t;
          indices[i + 4] = t + x + 1;
          indices[i + 5] = t + 1;
          i += 6;
          t++;
        }
        t++;
      }

      GLuint VBO, EBO;
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);

      glBindVertexArray(VAO);

      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, plane.size() * sizeof(GLfloat), &plane.front(), GL_DYNAMIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_DYNAMIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (GLvoid*)0);

      glEnableVertexAttribArray(0);
    }
    
    void Draw() {
      plane_shader.render();

      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
    }
  };
}

