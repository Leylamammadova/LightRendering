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
    
    void init(int size_x = 10, int size_y = 10, 
      const char* vert_shader_loc = "shaders/plane.vs", 
      const char* frag_shader_loc = "shaders/plane.fs") {

      obj_file_io file_reader;
      vertex_shader = file_reader.load_file(vert_shader_loc).c_str();
      fragment_shader = file_reader.load_file(frag_shader_loc).c_str();

      // Calculating vertices and indices from:
      // http://stackoverflow.com/questions/10114577/a-method-for-indexing-triangles-from-a-loaded-heightmap
      int plane_width = size_x;
      int plane_height = size_y;
      vertices.resize(plane_width * plane_height * 3);
      for (int z = 0; z < plane_height; ++z) {
        for (int x = 0; x < plane_width; ++x) {
          // Vertex Positions
          vertices.push_back(x);
          vertices.push_back(0.0f);
          vertices.push_back(z);

          // Vertex Normals
          vertices.push_back(0.0f);
          vertices.push_back(1.0f);
          vertices.push_back(0.0f);
        }
      }

      int grid_width = plane_width - 1;
      int grid_height = plane_height - 1;

      indices.resize(grid_width * grid_height * 6);
      int i = 0;
      for (int z = 0; z < grid_height; ++z) {
        for (int x = 0; x < grid_width; ++x) {
          int start = z * plane_width + x;
          indices[i] = start;
          indices[i + 1] = start + 1;
          indices[i + 2] = start + plane_width;

          indices[i + 3] = start + 1;
          indices[i + 4] = start + 1 + plane_width;
          indices[i + 5] = start + plane_width;
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

