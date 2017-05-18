#pragma once

namespace octet {
  /// Scene containing a box with octet.

  class entity {
  private:
    mat4t modelToWorld;
    GLuint modelToProjectionIndex_;

    float rot_angle;

    bool enabled;

    GLuint vertex_buffer;
    shader road_shader;

  public:
    std::vector<float> vertBuff;
    std::vector<int> faceBuff;

    entity() {}

    void init(float x, float y, float z, float sx = 1.0f, float sy = 1.0f, float sz = 1.0f) {
      modelToWorld.loadIdentity();
      modelToWorld.translate(x, y, z);
      modelToWorld.scale(sx, sy, sz);

      vertBuff = ObjReader::load_file_vertdata("wt_teapot.obj");
      faceBuff = ObjReader::load_file_facedata("wt_teapot.obj");

      rot_angle = 0;
      enabled = true;

      glGenBuffers(1, &vertex_buffer); // Sets up our vertex array buffer for rendering
      road_shader.init(ObjReader::load_file("road.vert").c_str(), ObjReader::load_file("road.frag").c_str()); // loads, compiles and links our shader programs
      modelToProjectionIndex_ = glGetUniformLocation(road_shader.get_program(), "modelToProjection");
    }


    void refresh_track(std::vector<float> newVertBuff, std::vector<int> newFaceBuff) {
      vertBuff = newVertBuff;
      faceBuff = newFaceBuff;
    }


    void render(mat4t &cameraToWorld) {

      if (!is_enabled()) return;


      //mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);
      //----------------------
      mat4t worldToCamera;
      cameraToWorld.invertQuick(worldToCamera);

      // build a projection matrix to add perspective
      mat4t cameraToProjection;
      cameraToProjection.loadIdentity();
      cameraToProjection.frustum(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 1000.0f);

      mat4t modelToProjection = modelToWorld * worldToCamera * cameraToProjection;

      glUseProgram(road_shader.get_program());
      glUniformMatrix4fv(modelToProjectionIndex_, 1, GL_FALSE, modelToProjection.get());

      // Grass green colour
      // openGL takes in an array of floats. Every 3 floats represents one vertex. 
      // Bellow is code telling opengl what float vertex data to use.
      // openGL reads the raw bytes in memory, so we need to tell it how many bytes per value (in this case float 4 bytes) 
      // and we also need to tell it how many values per vertex (in this case 3 for x, y and z)
      // We then tell openGL what shader program to use to render the mesh 
      // and we specify the render mode, here, GL_TRIANGLE_STRIP tells opengl to make the vertex data connect up into a mesh like this:
      //  The numbers represent the vertices, each vertex is three floats wide (z,y,z)
      //
      //   0-----2-----4
      //   |    /|    /|
      //   |   / |   / |
      //   |  /  |  /  |
      //   | /   | /   |
      //   |/    |/    |
      //   1-----3-----5

      //std::vector<float> leftTrackVertBuff = vertBuff;
      //std::vector<float> rightTrackVertBuff = vertBuff;
      //for (int i = 0; i < leftTrackVertBuff.size(); i += 3) {
      //  leftTrackVertBuff[i] = 0.5f * leftTrackVertBuff[i] - 0.5f;
      //  leftTrackVertBuff[i + 1] = 0.66666f * leftTrackVertBuff[i + 1] - 0.33333f;
      //}


      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
      glBufferData(GL_ARRAY_BUFFER, vertBuff.size() * sizeof(GLfloat), &vertBuff[0], GL_DYNAMIC_DRAW);


      glEnableVertexAttribArray(attribute_pos);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
      //glUseProgram(road_shader.get_program());
      glDrawArrays(GL_TRIANGLE_STRIP, 0, vertBuff.size() / 3);
      glDisableVertexAttribArray(attribute_pos);

    }

    bool &is_enabled() {
      return enabled;
    }

    // move the object
    void translate(float x, float y) {
      modelToWorld.translate(x, y, 0);
    }

    void rotate(float angle) {
      rot_angle += angle;
      modelToWorld.rotateZ(angle);
    }

    void rotate(float angle, float X, float Y, float Z) {
      if (Z != 0) rot_angle += angle;
      modelToWorld.rotate(angle, X, Y, Z);
    }

    void scale(float scaleX, float scaleY, float scaleZ) {
      modelToWorld.scale(scaleX, scaleY, scaleZ);
    }

  };
}