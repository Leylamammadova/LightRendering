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
#include "Skybox.h"

namespace octet {
  /// Scene containing a box with octet.
  class example_waves : public app {
    sky_box sb;
    GLuint frame_texture;
    GLuint depth_render_buffer;
    GLuint frame_buffer;
    GLuint texture;
  private:
    mat4t cameraToWorld;
    std::vector<entity*> gameObjects;

    entity* teapot;
    entity* teapot2;
    entity* teapot3;
    entity* cube;

  public:

    /// this is called when we construct the class before everything is initialised.
    example_waves(int argc, char **argv) : app(argc, argv), sb(sky_box::get_sky_box()) {
    }

    ~example_waves() {
      for (entity* obj : gameObjects) {
        delete obj;
      }
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
      cameraToWorld.translate(10, 5, 40);

      sb.init();
      generate_frame_buffer();


      ////opengl light
      //glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
      //glEnable(GL_LIGHTING);
      //glEnable(GL_LIGHT0);
      //glEnable(GL_NORMALIZE); 

      //GLfloat AmbientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
      //GLfloat DiffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
      //GLfloat SpecularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
      //glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);
      //glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
      //glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);

      ////light position
      //GLfloat LightPosition[] = { 0.5f, 0.5f, 0.0f, 1.0f };
      //glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
  
      //-----------------------------------------------------------------------
      // ..:: GAME OBJECTS ::..
      //-----------------------------------------------------------------------
      
      obj_file_io file_reader;
      std::string vertShader;
      std::string fragShader;

      // Teapot
      vertShader = file_reader.load_file("shaders/transparent.vs");
      fragShader = file_reader.load_file("shaders/transparent.fs");
      
      obj_file_io::opengl_data teapot_data = file_reader.parse_file_data(file_reader.load_mesh_file("wt_teapot.obj"));

      teapot = new entity();
      teapot->init(0, 0, 0, vertShader, fragShader, teapot_data.vertex_object, teapot_data.indices);
      teapot->scale(10, 10, 10);
      gameObjects.push_back(teapot);

      // Second Teapot
      teapot2 = new entity();
      vertShader = file_reader.load_file("shaders/transparent.vs");
      fragShader = file_reader.load_file("shaders/mirror.fs");

      teapot2->init(20, 0, 0, vertShader, fragShader, teapot_data.vertex_object, teapot_data.indices);
      teapot2->scale(10, 10, 10);
      gameObjects.push_back(teapot2);

      //third teapot
      teapot3 = new entity();
      vertShader = file_reader.load_file("shaders/transparent.vs");
      fragShader = file_reader.load_file("shaders/mirrorwithlight.fs");

      teapot3->init(40, 0, 0, vertShader, fragShader, teapot_data.vertex_object, teapot_data.indices);
      teapot3->scale(10, 10, 10);
      gameObjects.push_back(teapot3);

      // Cube
      //vertShader = file_reader.load_file("shaders/transparent.vs").c_str();
      //fragShader = file_reader.load_file("shaders/transparent.fs").c_str();

      //obj_file_io::opengl_data cube_data = file_reader.parse_file_data(file_reader.load_mesh_file("cube.obj"));

      //cube = new entity();
      //cube->init(-5, 0, 20, vertShader, fragShader, cube_data.vertex_object, cube_data.indices);
      //cube->scale(10, 10, 10);
      //cube->rotate(45, 0, 1, 0);
      //gameObjects.push_back(cube);

      // Plane
      //plane_mesh plane_data;
      //entity plane;
      //plane.init(-20, -10, -10, plane_data.get_vertex_shader(), plane_data.get_fragment_shader(), plane_data.get_vertices(), plane_data.get_indices());
      //plane.scale(5, 5, 5);
      //gameObjects.push_back(plane);
   
      //PrintUI();
     
    }
    void generate_frame_buffer() {
      int w, h;
      get_viewport_size(w, h);
      glGenFramebuffers(1, &frame_buffer);
      glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

      glGenTextures(1, &frame_texture);
      glBindTexture(GL_TEXTURE_2D, frame_texture);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_BYTE, 0);

      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame_texture, 0);

      glGenRenderbuffers(1, &depth_render_buffer);
      glBindRenderbuffer(GL_RENDERBUFFER, depth_render_buffer);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
      glBindRenderbuffer(GL_RENDERBUFFER, 0);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_render_buffer);
      GLenum res = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      assert(res == GL_FRAMEBUFFER_COMPLETE);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {

      KeyboardInput();
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      glViewport(x, y, w, h);


      //teapot->rotate(2, 0, 1, 0);
      //cube->rotate(2,0,1,0)
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);

      glEnable(GL_DEPTH_TEST);
      glClearColor(0, 0, 0, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      //enable alfa channel blend
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      
      mat4t modelToWorld;
      modelToWorld.loadIdentity();
      mat4t modelToProjection = mat4t::build_projection_matrix(modelToWorld, cameraToWorld);
      sb.render(modelToProjection,0);

      // render all gameObjects
      for (entity* object : gameObjects) {
        object->render(cameraToWorld);
      }

    }
  };
}
