////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//

#include "water_surface.h"

#include "entity.h"

namespace octet {
  /// Scene containing a box with octet.
  class example_waves : public app {
    // scene for drawing box
    //water_surface waves;
    //float time;
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

      // Adjust wave parameters
      // Wave length
      //if (is_key_going_down('1')) { 
      //  waves.AdjustWaveLength(-increment);
      //  PrintUI();
      //}

      //if (is_key_going_down('2')) {
      //  waves.AdjustWaveLength(increment);
      //  PrintUI();
      //}

      //// Amplitude
      //if (is_key_going_down('3')) {
      //  waves.AdjustAmplitude(-0.25f);
      //  PrintUI();
      //}

      //if (is_key_going_down('4')) {
      //  waves.AdjustAmplitude(0.25f);
      //  PrintUI();
      //}

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

      entity teapot;
      teapot.init(0, 0, 0, "cube.obj");
      teapot.scale(10, 10, 10);
      gameObjects.push_back(teapot);

      PrintUI();
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      KeyboardInput();
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);


      glViewport(x, y, w, h);

      glClearColor(0.3f, 0.67f, 0.28f, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // render all gameObjects
      for (entity object : gameObjects) {
        object.render(cameraToWorld);
      }

    }
  };
}
