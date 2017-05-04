#pragma once

namespace octet {
  class water_surface {
    struct my_vertex {
      vec3p pos;
      uint32_t color;
    };

    // Mesh variables
    int sizeX, sizeZ;
    ref<mesh> water_mesh;
    std::vector<vec3> water_plane;
    std::vector<uint32_t> indices;

    // Wave variables
    float height;
    float pi = 3.141592653f;
    float wavelength, amplitude, speed, frequency, phase;

    // this function converts three floats into a RGBA 8 bit color
    static uint32_t make_color(float r, float g, float b) {
      return 0xff000000 + ((int)(r*255.0f) << 0) + ((int)(g*255.0f) << 8) + ((int)(b*255.0f) << 16);
    }

  public:
    water_surface() {
      sizeX = 0;
      sizeZ = 0;
      height = 0.0f;
      wavelength = 0.0f;
      amplitude = 0.0f;
      speed = 0.0f;
      frequency = 0.0f;
      phase = 0.0f;
    }

    // Getters
    ref<mesh> Get_Mesh() { return water_mesh; }
    int GetMeshWidth() { return sizeX; }
    int GetMeshDepth() { return sizeZ; }
    float GetWaveLength() { return wavelength; }
    float GetAmplitude() { return amplitude; }

    // Setters
    void AdjustWaveLength(float num) { wavelength += num; }
    void AdjustAmplitude(float num) { amplitude += num; }

    void init(int xSize = 100, int zSize = 100) {
      water_mesh = new mesh();
      sizeX = xSize;
      sizeZ = zSize;

      // preparing the buffers for OpenGL
      water_plane.resize(sizeX*sizeZ);
      indices.resize((sizeX - 1) * (sizeZ - 1) * 6);

      // allocate vertices and indices into OpenGL buffers
      water_mesh->allocate(sizeof(my_vertex) * water_plane.size(), sizeof(uint32_t) * indices.size());
      water_mesh->set_params(sizeof(my_vertex), indices.size(), water_plane.size(), GL_TRIANGLES, GL_UNSIGNED_INT);

      // describe the structure of my_vertex to OpenGL
      water_mesh->add_attribute(attribute_pos, 3, GL_FLOAT, 0);
      water_mesh->add_attribute(attribute_color, 4, GL_UNSIGNED_BYTE, 12, GL_TRUE);

      int i = 0;
      for (int z = 0, t = 0; z < sizeZ - 1; ++z) {
        for (int x = 0; x < sizeX - 1; ++x) {
          indices[i] = t;
          indices[i + 1] = t + sizeX;
          indices[i + 2] = t + sizeX + 1;

          indices[i + 3] = t;
          indices[i + 4] = t + sizeX + 1;
          indices[i + 5] = t + 1;
          i += 6;
          t++;
        }
        t++;
      }
      gl_resource::wolock il(water_mesh->get_indices());
      uint32_t *indx = il.u32();
      memcpy(indx, indices.data(), sizeof(uint32_t)*indices.size());

      // Default values for the wave
      wavelength = 16;
      amplitude = 1.0f;
      speed = 1.0f;
      frequency = (2 * pi) / wavelength;
      phase = speed * frequency;
    }

    void AnimateWaves(float time) {
      frequency = (2 * pi) / wavelength;
      phase = speed * frequency;

      // these write-only locks give access to the vertices and indices.
      // they will be released at the next } (the end of the scope)
      gl_resource::wolock vl(water_mesh->get_vertices());
      my_vertex *vtx = (my_vertex *)vl.u8();

      for (int i = 0; i < water_plane.size(); ++i) {
        float r = 0.0f, g = 1.0f * i / water_plane.size(), b = 1.0f;

        // Sine Wave formula
        height = amplitude * sinf(2 * pi * frequency * time + phase);
        time += 0.1f;

        water_plane[i] = vec3p((i % sizeX), height, (i / sizeX));

        vtx->pos = water_plane[i];
        vtx->color = make_color(r, g, b);
        vtx++;
      }

    }
  };
}