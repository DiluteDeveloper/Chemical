#include "terrain_state.hpp"
#include "marching_cubes.hpp"
#include "maths/perlin_noise.hpp"

#include <glad/glad.h>

namespace Chemical {

  namespace Util {

    int i = 1;
    void TerrainState::Generate() {
      IsoGrid grid;
      grid.reserve(size.x);

      Maths::SetPerlinNoiseSeed(i);
      i++;

      for (size_t x = 0; x < size.x; x++) {
        grid.emplace_back();
        grid[x].reserve(size.y);

        for (size_t y = 0; y < size.y; y++) {
          grid[x].emplace_back();
          grid[x][y].reserve(size.z);

          for (size_t z = 0; z < size.z; z++) {
            double noise_val = Maths::PerlinNoise3D(glm::vec3(
                                   x * noise_frequency, y * noise_frequency,
                                   z * noise_frequency)) *
                               noise_amplitude;
            grid[x][y].emplace_back(noise_val);
          }
        }
      }

      mesh = GenerateMarchingCubes(grid, iso_value);
      vao = mesh.AsVAO();
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
