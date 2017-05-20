////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//

#include "obj_file_io.h"

// Gameobjects
#include "entity.h"
#include "plane_mesh.h"

namespace octet {
  /// Scene containing a box with octet.
  class example_waves : public app {
  private:
    mat4t cameraToWorld;
    std::vector<entity> gameObjects;

  public:
    /// this is called when we construct the class before everything is initialised.
    example_waves(int argc, char **argv) : app(argc, argv) {
    }

    void KeyboardInput() {
      if (is_key_down(key::key_esc)) {
        exit(1);
      }

      float increment = 1;
      
      // Translate camera - X-axis
      if (is_key_down('A')) { cameraToWorld.translate(-increment, 0, 0); }
      if (is_key_down('D')) { cameraToWorld.translate(increment, 0, 0); }

      // Translate camera - Y-axis
      if (is_key_down('Q')) { cameraToWorld.translate(0, increment, 0); }
      if (is_key_down('E')) { cameraToWorld.translate(0, -increment, 0); }

      // Translate camera - Z-axis
      if (is_key_down('W')) { cameraToWorld.translate(0, 0, -increment); }
      if (is_key_down('S')) { cameraToWorld.translate(0, 0, increment); }
      
      // Rotate camera - X-axis
      if (is_key_down('Z')) { cameraToWorld.rotateX(increment); }
      if (is_key_down('X')) { cameraToWorld.rotateX(-increment); }

      // Rotate camera - Y axis
      if (is_key_down('C')) { cameraToWorld.rotateY(increment); }
      if (is_key_down('V')) { cameraToWorld.rotateY(-increment); }

      // Rotate camera - Y axis
      if (is_key_down('R')) { cameraToWorld.rotateZ(increment); }
      if (is_key_down('F')) { cameraToWorld.rotateZ(-increment); }

      // Set wireframe on or off
      static bool wireframe = false;
      if (is_key_going_down(' ')) {
        wireframe = !wireframe;
        if (wireframe) {
          glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
          glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
      }
    }

    void PrintUI() {
      printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
      printf("Use WSAD to move the camera\n");
      printf("Press Q to raise the camera\n");
      printf("Press E to lower the camera\n");
      printf("Press Z to rotate the camera up\n");
      printf("Press X to rotate the camera down\n");
      printf("Press C to rotate the camera left\n");
      printf("Press V to rotate the camera right\n");
      printf("Press the space bar to toggle wireframe.");
      printf("\n_____________________\n");
      //printf("Width: %d\nDepth: %d\nWaveLength: %f\nAmplitude: %f\n", waves.GetMeshWidth(), waves.GetMeshDepth(), waves.GetWaveLength(), waves.GetAmplitude());
    }

    /// this is called once OpenGL is initialized
    void app_init() {
      cameraToWorld.loadIdentity();
      cameraToWorld.translate(0, 0, 20);

      ////opengl light
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
      glEnable(GL_NORMALIZE); 

      GLfloat AmbientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
      GLfloat DiffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
      GLfloat SpecularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
      glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
      glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);

      //light position
      GLfloat LightPosition[] = { 20.5f, 20.5f, 0.0f, 1.0f };
      glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
  
      // teapot
      obj_file_io file_reader;
      std::string vertShader = file_reader.load_file("shaders/teapot.vs").c_str();
      std::string fragShader = file_reader.load_file("shaders/teapot.fs").c_str();
      
      obj_file_io::opengl_data teapot_data = file_reader.parse_file_data(file_reader.load_mesh_file("wt_teapot.obj"));

      entity teapot;
      teapot.init(0, 0, 0, vertShader, fragShader, teapot_data.vertex_object, teapot_data.indices);
      teapot.scale(10, 10, 10);

      gameObjects.push_back(teapot);

      // Cube
      vertShader = file_reader.load_file("shaders/teapot.vs").c_str();
      fragShader = file_reader.load_file("shaders/teapot.fs").c_str();

      obj_file_io::opengl_data cube_data = file_reader.parse_file_data(file_reader.load_mesh_file("cube.obj"));

      entity cube;
      cube.init(5, 5, 5, vertShader, fragShader, cube_data.vertex_object, cube_data.indices);
      cube.scale(10, 10, 10);

      gameObjects.push_back(cube);

      // plane
      plane_mesh plane_data;
      entity plane;
      plane.init(0, 0, 0, plane_data.get_vertex_shader(), plane_data.get_fragment_shader(), plane_data.get_vertices(), plane_data.get_indices());
      plane.scale(10, 10, 10);

      gameObjects.push_back(plane);
   
      //PrintUI();
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {

      KeyboardInput();
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      glViewport(x, y, w, h);

      glEnable(GL_DEPTH_TEST);
      glClearColor(0.3f, 0.67f, 0.28f, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // render all gameObjects
      for (entity object : gameObjects) {
        object.render(cameraToWorld);
      }

    }
  };
}
