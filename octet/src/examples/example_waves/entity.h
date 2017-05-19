#pragma once
#include "obj_file_io.h"
namespace octet {
  /// Scene containing a box with octet.

  class entity {
    struct Vertex {
      vec3 pos;
      vec2 uvs;
      vec3 normals;
    };


  private:
    mat4t modelToWorld;
    GLuint modelToProjectionIndex_;

    bool enabled;

    GLuint VAO, VBO, EBO;

    std::vector<GLuint> indices;

    shader default_shader;

  public:
    entity() {}

    void init(float x, float y, float z, char* mesh_file = NULL) {
      modelToWorld.loadIdentity();
      modelToWorld.translate(x, y, z);

      enabled = true;

      obj_file_io file_reader;
      std::string vertShader = file_reader.load_file("teapot.vs").c_str();
      std::string fragShader = file_reader.load_file("teapot.fs").c_str();
      default_shader.init(const_cast<char*>(vertShader.c_str()), const_cast<char*>(fragShader.c_str())); // loads, compiles and links our shader programs
      modelToProjectionIndex_ = glGetUniformLocation(default_shader.get_program(), "modelToProjection");

      if (mesh_file != NULL) {
        obj_file_io::opengl_data teapot_data = file_reader.parse_file_data(file_reader.load_mesh_file(mesh_file));
        set_mesh_data(teapot_data.vertex_object, teapot_data.indices);
      }
    }

    void set_mesh_data(std::vector<float> &vertBuff, std::vector<unsigned int> &indiceseBuff) {
      indices = indiceseBuff;
      std::vector<GLuint>::iterator it;
   

      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);

      glBindVertexArray(VAO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, vertBuff.size() * sizeof(GLfloat), &vertBuff[0], GL_STATIC_DRAW);

      glVertexAttribPointer(attribute_pos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
      glEnableVertexAttribArray(attribute_pos);
      glVertexAttribPointer(attribute_normal, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
      glEnableVertexAttribArray(attribute_normal);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
      glBindVertexArray(0);

    }

    void render(mat4t &cameraToWorld) {
      if (!is_enabled()) return;
      mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);
      //----------------------
      //mat4t worldToCamera;
      //cameraToWorld.invertQuick(worldToCamera);

      //// build a projection matrix to add perspective
      //mat4t cameraToProjection;
      //cameraToProjection.loadIdentity();
      //cameraToProjection.frustum(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 10000.0f);
      //mat4t modelToProjection = modelToWorld * worldToCamera * cameraToProjection;

      // Load object transform data to the shader
      glUseProgram(default_shader.get_program());
      glUniformMatrix4fv(modelToProjectionIndex_, 1, GL_FALSE, modelToProjection.get());

      // Draw the polygon
      glBindVertexArray(VAO);
      glEnableVertexAttribArray(attribute_pos); // replace with enable attribs function in future when more attribs are needed
      glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);

    }

    bool &is_enabled() {
      return enabled;
    }

    // move the object
    void translate(float x, float y) {
      modelToWorld.translate(x, y, 0);
    }

    void rotate(float angle) {
      modelToWorld.rotateZ(angle);
    }

    void rotate(float angle, float X, float Y, float Z) {
      modelToWorld.rotate(angle, X, Y, Z);
    }

    void scale(float scaleX, float scaleY, float scaleZ) {
      modelToWorld.scale(scaleX, scaleY, scaleZ);
    }

  };
}