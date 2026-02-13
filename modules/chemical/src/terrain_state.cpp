#include "terrain_state.hpp"
#include "marching_cubes.hpp"
#include "maths/perlin_noise.hpp"
#include "spdlog/spdlog.h"

#include <ctime>
#include <glad/glad.h>

namespace Chemical {

  namespace Util {

    TerrainState::TerrainState() {
      AddNoiseOctave(std::make_pair(0.1f, 1.0f));
    }
    int noise_seed = 1;
    void TerrainState::Generate() {

      glDeleteVertexArrays(1, &vao);

      clock_t time_before = clock();
      IsoGrid grid;
      grid.reserve(size.x);

      for (size_t x = 0; x < size.x; x++) {
        grid.emplace_back();
        grid[x].reserve(size.y);

        for (size_t y = 0; y < size.y; y++) {
          grid[x].emplace_back();
          grid[x][y].reserve(size.z);

          for (size_t z = 0; z < size.z; z++) {
            double noise_val = 0;
            int i = 0;
            for (const NoiseOctave &octave : noise_octaves) {

              // Maths::SetPerlinNoiseSeed(noise_seed + i); commented out to
              // debug performance leak in Marching Cubes
              noise_val +=
                  Maths::PerlinNoise3D(glm::vec3(
                      x * octave.first, y * octave.first, z * octave.first)) *
                  octave.second;
              i++;
            }
            grid[x][y].emplace_back(noise_val);
          }
        }
      }
      noise_seed += GetNoiseOctaveCount();

      clock_t time_after = clock();
      SPDLOG_INFO("grid and perlin noise time: {}",
                  float(time_after - time_before) / CLOCKS_PER_SEC);
      mesh = GenerateMarchingCubes(grid, iso_value);
      vao = mesh.AsVAO();
    }
    unsigned int TerrainState::GetNoiseOctaveCount() {
      return noise_octaves.size();
    }
    TerrainState::NoiseOctave &TerrainState::GetNoiseOctave(unsigned int idx) {
      return noise_octaves.at(idx);
    }
    void TerrainState::AddNoiseOctave(const NoiseOctave &octave) {
      noise_octaves.emplace_back(octave);
    }

    void TerrainState::RemoveLastNoiseOctave() {
      noise_octaves.pop_back();
    }
    unsigned int TerrainState::GetVAO() {
      return vao;
    }
    unsigned int TerrainState::GetIndiceCount() {
      return mesh.indices.size();
    }
    TerrainState::~TerrainState() {
      glDeleteVertexArrays(1, &vao);
    }
  } // namespace Util

} // namespace Chemical
