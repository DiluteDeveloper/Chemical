#pragma once

#include "chemical/util/image.h"

namespace Chemical {

  namespace Graphics {

    // Represents an OpenGL texture object and its functionality
    // Deleted copy ctor & operator, custom move ctor and operator
    class Texture {
    public:
      enum class ScalingFilter { NEAREST = 0x2600, LINEAR = 0x2601 };

      // Generates the OpenGL texture object
      Texture(const Util::Image &image, ScalingFilter scaling_filter = ScalingFilter::NEAREST);

      // Deletes the OpenGL texture object
      ~Texture();

      Texture &operator=(Texture &&other) {
        gl_id = other.gl_id;
        other.gl_id = 0;
        return *this;
      }
      Texture(Texture &&other) : gl_id(other.gl_id) {
        other.gl_id = 0;
      }

    protected:
      // All functionality that only the renderer/s should have access to
      friend class Renderer;

      Texture(const Texture &other) = delete;
      Texture &operator=(const Texture &other) = delete;

      void Bind() const;

      unsigned int gl_id = 0;
    };
  } // namespace Graphics
} // namespace Chemical
