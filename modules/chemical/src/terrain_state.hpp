#pragma once

#include "rendering/mesh.hpp"
namespace Chemical {
  namespace Util {

    using IsoGrid = std::vector<std::vector<std::vector<double>>>;

    class TerrainState {

    public:
      TerrainState();
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

      float iso_value = 0.6f;

      glm::uvec3 size = glm::uvec3(50);

      using NoiseFrequency = float;
      using NoiseAmplitude = float;
      using NoiseOctave = std::pair<NoiseFrequency, NoiseAmplitude>;

      unsigned int GetNoiseOctaveCount();
      NoiseOctave &GetNoiseOctave(unsigned int idx);
      void AddNoiseOctave(const NoiseOctave &octave);
      void RemoveLastNoiseOctave();

    private:
      Mesh mesh;
      unsigned int vao;

      std::vector<NoiseOctave> noise_octaves;
    };
  } // namespace Util

} // namespace Chemical
