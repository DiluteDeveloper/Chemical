#pragma once

#include "rendering/mesh.hpp"
namespace Chemical {
  namespace Util {

    using IsoGrid = std::vector<std::vector<std::vector<double>>>;

    class TerrainState {

    public:
      TerrainState() = default;
      TerrainState(const TerrainState &) = delete;
      TerrainState &operator=(const TerrainState &) = delete;

      TerrainState(TerrainState &&) = delete;
      TerrainState &operator=(TerrainState &&) = delete;

      void Generate();

      unsigned int GetVAO();
      unsigned int GetIndiceCount();

      float GetNoiseFrequency();
      void SetNoiseFrequency(float freq);

      float GetNoiseAmplitude();
      void SetNoiseAmplitude(float amp);

      glm::uvec3 GetSize();
      void SetSize(const glm::uvec3 &in_size);

      ~TerrainState();

      float noise_frequency = .1f;
      float noise_amplitude = 1.0f;
      float iso_value = 0.6f;

      glm::uvec3 size = glm::uvec3(50);

    private:
      Mesh mesh;
      unsigned int vao;
    };
  } // namespace Util

} // namespace Chemical
