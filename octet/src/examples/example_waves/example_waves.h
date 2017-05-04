////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//

#include "water_surface.h"

namespace octet {
  /// Scene containing a box with octet.
  class example_waves : public app {
    // scene for drawing box
    ref<visual_scene> app_scene;
    water_surface waves;
    float time;
  public:
    /// this is called when we construct the class before everything is initialised.
    example_waves(int argc, char **argv) : app(argc, argv) {
    }

    void KeyboardInput() {
      if (is_key_down(key::key_esc)) {
        exit(1);
      }

      float increment = 1;
      mat4t &camera = app_scene->get_camera_instance(0)->get_node()->access_nodeToParent();
      
      // Translate camera - X-axis
      if (is_key_down('A')) { camera.translate(-increment, 0, 0); }
      if (is_key_down('D')) { camera.translate(increment, 0, 0); }

      // Translate camera - Y-axis
      if (is_key_down('Q')) { camera.translate(0, increment, 0); }
      if (is_key_down('E')) { camera.translate(0, -increment, 0); }

      // Translate camera - Z-axis
      if (is_key_down('W')) { camera.translate(0, 0, -increment); }
      if (is_key_down('S')) { camera.translate(0, 0, increment); }
      
      // Rotate camera - X-axis
      if (is_key_down('Z')) { camera.rotateX(increment); }
      if (is_key_down('X')) { camera.rotateX(-increment); }

      // Rotate camera - Y axis
      if (is_key_down('C')) { camera.rotateY(increment); }
      if (is_key_down('V')) { camera.rotateY(-increment); }

      // Rotate camera - Y axis
      if (is_key_down('R')) { camera.rotateZ(increment); }
      if (is_key_down('F')) { camera.rotateZ(-increment); }

      // Adjust wave parameters
      // Wave length
      if (is_key_going_down('1')) { 
        waves.AdjustWaveLength(-increment);
        PrintUI();
      }

      if (is_key_going_down('2')) {
        waves.AdjustWaveLength(increment);
        PrintUI();
      }

      // Amplitude
      if (is_key_going_down('3')) {
        waves.AdjustAmplitude(-0.25f);
        PrintUI();
      }

      if (is_key_going_down('4')) {
        waves.AdjustAmplitude(0.25f);
        PrintUI();
      }

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
      printf("Width: %d\nDepth: %d\nWaveLength: %f\nAmplitude: %f\n", waves.GetMeshWidth(), waves.GetMeshDepth(), waves.GetWaveLength(), waves.GetAmplitude());
    }

    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene = new visual_scene();
      app_scene->create_default_camera_and_lights();
      app_scene->get_camera_instance(0)->set_far_plane(1000000.0f);
      
      // init camera position
      mat4t &camera = app_scene->get_camera_instance(0)->get_node()->access_nodeToParent();
      camera.translate(50, 80, 170);
      camera.rotateX(-20);

      // init shaders
      param_shader *shader = new param_shader("shaders/default.vs", "shaders/simple_color.fs");
      material *colour = new material(vec4(0, 0, 1, 1), shader);
      scene_node *node = new scene_node();
      app_scene->add_child(node);

      time = 0.0f;

      // Asks user to specify the size of the mesh
      int Width, Depth;
      printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
      printf("Please set the size of the mesh.\nWidth: ");
      std::cin >> Width;
      printf("Depth: ");
      std::cin >> Depth;
      waves.init(Width, Depth);

      mesh *water = waves.Get_Mesh();
      app_scene->add_mesh_instance(new mesh_instance(node, water, colour));

      PrintUI();
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      KeyboardInput();
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

      // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);

      waves.AnimateWaves(time += 1.0f/30);

      // draw the scene
      app_scene->render((float)vx / vy);
    }
  };
}
