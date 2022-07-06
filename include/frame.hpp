
#ifndef FRAME_HPP
#define FRAME_HPP

#include <pixel.hpp>

#include <functional>
#include <stdlib.h>
#include <png.h>

namespace im {

   template<unsigned width, unsigned height>
   struct frame {

      pixel* _pixels;
      pixel** _pixel_rows;

      frame() {
         _pixels = new pixel[width * height];
         _pixel_rows = new pixel*[height];
         for (int j = 0; j < height; j++)
            _pixel_rows[j] = &_pixels[j*width];
      }

      template<typename painter>
      inline void paint(painter &p) {
         #pragma omp parallel for schedule(guided)
         for (int j = 0; j < height; j++) {
            int idx = j*width;
            for (int i = 0; i < width; i++) {
               _pixels[idx++] = p.paint(i,j);
            }
         }
      }

      inline pixel paint(unsigned x, unsigned y) {
         return _pixels[y*width + x];
      }

      ~frame() {
         delete[] _pixels;
         delete[] _pixel_rows;
      }
   };
}

#endif

