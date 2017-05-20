#pragma once

namespace octet {

  class plane_mesh {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::string vertex_shader;
    std::string fragment_shader;

  public:
    plane_mesh() {
      init();
    }
    
    void init(int size_x = 10, int size_y = 10) {
      obj_file_io file_reader;
      vertex_shader = file_reader.load_file("shaders/plane_vertex_shader.vs").c_str();
      fragment_shader = file_reader.load_file("shaders/plane_fragment_shader.fs").c_str();

      // Calculating vertices and indices from:
      // http://stackoverflow.com/questions/10114577/a-method-for-indexing-triangles-from-a-loaded-heightmap
      vertices.resize(size_x * size_y * 3);
      for (int z = 0; z < size_y; ++z) {
        for (int x = 0; x < size_x; ++x) {
          vertices.push_back(x);
          vertices.push_back(0.0f);
          vertices.push_back(z);
        }
      }

      int grid_width = size_x - 1;
      int grid_height = size_y - 1;

      indices.resize(grid_width * grid_height * 6);
      int i = 0;
      for (int z = 0; z < grid_height; ++z) {
        for (int x = 0; x < grid_width; ++x) {
          int start = z * size_x + x;
          indices[i] = start;
          indices[i + 1] = start + 1;
          indices[i + 2] = start + size_x;

          indices[i + 3] = start + 1;
          indices[i + 4] = start + 1 + size_x;
          indices[i + 5] = start + size_x;
          i += 6;
        } // end of for loop
      } // end of for loop
    }// end of function.

    std::vector<GLfloat> get_vertices() {
      return vertices;
    }

    std::vector<GLuint> get_indices() {
      return indices;
    }

    std::string get_vertex_shader() {
      return vertex_shader;
    }

    std::string get_fragment_shader() {
      return fragment_shader;
    }
  };
}

