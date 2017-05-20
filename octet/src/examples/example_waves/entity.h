#pragma once
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
    
    vec3 light_pos_;

  public:
    entity() : light_pos_(1.0f, 1.0f, 1.0f) {}

    void init(float x, float y, float z, std::string vertShader, std::string fragShader, std::vector<float> &vertBuff, std::vector<unsigned int> &indiceseBuff) {
      modelToWorld.loadIdentity();
      modelToWorld.translate(x, y, z);

      enabled = true;

      default_shader.init(const_cast<char*>(vertShader.c_str()), const_cast<char*>(fragShader.c_str())); // loads, compiles and links our shader programs
      modelToProjectionIndex_ = glGetUniformLocation(default_shader.get_program(), "modelToProjection");

      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);

      

      set_mesh_data(vertBuff, indiceseBuff);

    }

    void set_mesh_data(std::vector<float> &vertBuff, std::vector<unsigned int> &indiceseBuff) {
      indices = indiceseBuff;

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

      // light uniforms
      GLint object_colour_loc = glGetUniformLocation(default_shader.get_program(), "object_colour_");
      GLint light_colour_loc = glGetUniformLocation(default_shader.get_program(), "light_colour_");
      GLint light_pos_loc = glGetUniformLocation(default_shader.get_program(), "light_pos_");
      GLint view_pos_loc = glGetUniformLocation(default_shader.get_program(), "view_pos_");
      vec4 camera_pos_x(cameraToWorld.x());
      vec4 camera_pos_y(cameraToWorld.y());
      vec4 camera_pos_z(cameraToWorld.z());

      glUniform3f(object_colour_loc, 1.0f, 0.5f, 0.31f);
      glUniform3f(light_colour_loc, 1.0f, 0.5f, 1.0f);
      glUniform3f(light_pos_loc, light_pos_.x(), light_pos_.y(), light_pos_.z());
      glUniform3f(view_pos_loc, camera_pos_x[0], camera_pos_y[1], camera_pos_z[2]);

      // Draw the polygon
      glBindVertexArray(VAO);
      glEnableVertexAttribArray(attribute_pos); // replace with enable attribs function in future when more attribs are needed
      glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);

      ////render opengl light
      //GLfloat mat_amb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
      //GLfloat mat_diff[] = { 0.0f, 1.0f, 0.0f, 1.0f };
      //GLfloat mat_spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };

      //glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
      //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
      //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
      //glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);

    }

    bool &is_enabled() {
      return enabled;
    }

    // move the object
    void translate(float x, float y, float z = 0) {
      modelToWorld.translate(x, y, z);
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