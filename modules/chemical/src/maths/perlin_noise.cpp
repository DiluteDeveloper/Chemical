#include "perlin_noise.hpp"

#include <array>
#include <glm/glm.hpp>
#include <unordered_map>

#include <spdlog/spdlog.h>

namespace Chemical {

  namespace Maths {

    // Hash lookup table as defined by Ken Perlin.  This is a randomly
    // arranged array of all numbers from 0-255 inclusive - doubled.
    static constexpr int static_perm[] = {
        151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233,
        7,   225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,
        23,  190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252,
        219, 203, 117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,
        174, 20,  125, 136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,
        27,  166, 77,  146, 158, 231, 83,  111, 229, 122, 60,  211, 133, 230,
        220, 105, 92,  41,  55,  46,  245, 40,  244, 102, 143, 54,  65,  25,
        63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169,
        200, 196, 135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186,
        3,   64,  52,  217, 226, 250, 124, 123, 5,   202, 38,  147, 118, 126,
        255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,  182, 189,
        28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,  154, 163, 70,
        221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253, 19,  98,
        108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228,
        251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,
        145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157,
        184, 84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236,
        205, 93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,
        215, 61,  156, 180, 151, 160, 137, 91,  90,  15,  131, 13,  201, 95,
        96,  53,  194, 233, 7,   225, 140, 36,  103, 30,  69,  142, 8,   99,
        37,  240, 21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,
        197, 62,  94,  252, 219, 203, 117, 35,  11,  32,  57,  177, 33,  88,
        237, 149, 56,  87,  174, 20,  125, 136, 171, 168, 68,  175, 74,  165,
        71,  134, 139, 48,  27,  166, 77,  146, 158, 231, 83,  111, 229, 122,
        60,  211, 133, 230, 220, 105, 92,  41,  55,  46,  245, 40,  244, 102,
        143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187,
        208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,  164, 100,
        109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,   202,
        38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,
        58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,
        44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,
        39,  253, 19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104,
        218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179,
        162, 241, 81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,
        181, 199, 106, 157, 184, 84,  204, 176, 115, 121, 50,  45,  127, 4,
        150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243, 141,
        128, 195, 78,  66,  215, 61,  156, 180};

    unsigned int cur_seed = 0;
    std::unordered_map<unsigned int, std::array<int, 255>>
        seed_permutation_cache;
    void SetPerlinNoiseSeed(unsigned int seed) {

      cur_seed = seed;
      if (!seed_permutation_cache.contains(seed)) {
        std::array<int, 255> &perm = seed_permutation_cache[seed];
        srand(seed);
        for (int i = 0; i < 255; i++) {
          perm[i] = static_perm[rand() % 256];
        }
      }
    }
    const std::array<int, 255> &GetPermutationTable() {

      if (!seed_permutation_cache.contains(cur_seed))
        SetPerlinNoiseSeed(cur_seed);

      return seed_permutation_cache[cur_seed];
    }

    double Fade(double t) {
      return t * t * t * (t * (t * 6 - 15) + 10);
    }
    double Lerp(double a, double b, double x) {
      return a + x * (b - a);
    }

    double Gradient(int hash, double x, double y, double z) {

      switch (hash & 0xF) {
      case 0x0:
        return x + y;
      case 0x1:
        return -x + y;
      case 0x2:
        return x - y;
      case 0x3:
        return -x - y;
      case 0x4:
        return x + z;
      case 0x5:
        return -x + z;
      case 0x6:
        return x - z;
      case 0x7:
        return -x - z;
      case 0x8:
        return y + z;
      case 0x9:
        return -y + z;
      case 0xA:
        return y - z;
      case 0xB:
        return -y - z;
      case 0xC:
        return y + x;
      case 0xD:
        return -y + z;
      case 0xE:
        return y - x;
      case 0xF:
        return -y - z;
      default:
        return 0; // never happens
      }
    }

    double PerlinNoise3D(const glm::vec3 &val) {

      double xf = std::fmod(val.x, 1);
      double yf = std::fmod(val.y, 1);
      double zf = std::fmod(val.z, 1);

      int xi = (int)floor(val.x) % 255;
      int yi = (int)floor(val.y) % 255;
      int zi = (int)floor(val.z) % 255;

      double u = Fade(xf);
      double v = Fade(yf);
      double w = Fade(zf);

      auto inc = [](int v) {
        v++;
        v %= 255;
        return v;
      };

      // Random values between 0 and 255 for each vertex on the square

      const auto &p = GetPermutationTable();

      int aaa, aba, aab, abb, baa, bba, bab, bbb;
      aaa = p[p[p[xi] + yi] + zi];
      aba = p[p[p[xi] + inc(yi)] + zi];
      aab = p[p[p[xi] + yi] + inc(zi)];
      abb = p[p[p[xi] + inc(yi)] + inc(zi)];
      baa = p[p[p[inc(xi)] + yi] + zi];
      bba = p[p[p[inc(xi)] + inc(yi)] + zi];
      bab = p[p[p[inc(xi)] + yi] + inc(zi)];
      bbb = p[p[p[inc(xi)] + inc(yi)] + inc(zi)];
      // Random values between 0 and 255 for each vertex on the square

      double x1, x2, y1, y2;
      x1 = Lerp(Gradient(aaa, xf, yf, zf), Gradient(baa, xf - 1, yf, zf), u);
      x2 = Lerp(Gradient(aba, xf, yf - 1, zf),
                Gradient(bba, xf - 1, yf - 1, zf), u);
      y1 = Lerp(x1, x2, v);

      x1 = Lerp(Gradient(aab, xf, yf, zf - 1),
                Gradient(bab, xf - 1, yf, zf - 1), u);
      x2 = Lerp(Gradient(abb, xf, yf - 1, zf - 1),
                Gradient(bbb, xf - 1, yf - 1, zf - 1), u);
      y2 = Lerp(x1, x2, v);

      return (Lerp(y1, y2, w) + 1) / 2;
    }

  } // namespace Maths
} // namespace Chemical
