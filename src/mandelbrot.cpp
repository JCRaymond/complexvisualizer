#include <types.hpp>

class mandelbrot {
   const size_t im_x;
   const size_t im_y;
   const real height;
   const comp center;
   const mandel_colorizer mc;
   const size_t iterlim;
   const size_t samples;

   const comp pixel_tl;
   const comp pixel_dx;
   const comp pixel_dy;
   const comp sample_dx;
   const comp sample_dy;

   inline size_t iterate(comp c) {
      auto z = c;
      size_t i = 0;
      while (i < iterlim) {
         z = z*z + c;
         if (std::norm(z) >= 4)
            break;
         i++;
      }

      return i;
   }

public:

   mandelbrot(size_t im_x, size_t im_y, real height, comp center, mandel_colorizer mc, size_t iterlim = 1000, size_t samples = 1) : im_x(im_x), im_y(im_y), height(height), center(center), iterlim(iterlim), samples(samples) {
      auto pixel_height = 2 * (height / im_y);
      pixel_dx = {pixel_height, 0};
      pixel_dy = {0, -pixel_height};
      sample_dx = pixel_dx / samples;
      sample_dy = pixel_dy / samples;

      auto frame_tl = center - (((real)im_x) / 2) * pixel_dx - (((real)im_y) / 2) * pixel_dy;
      pixel_tl = frame_tl + ((1.0/(samples+1))*pixel_dx) + ((1.0/(samples+1))*pixel_dy);
   }

   inline im::pixel paint(size_t x, size_t y) {
      auto c_base = pixel_tl + ((real)x) * pixel_dx + ((real)y) * pixel_dy;

      size_t r = 0, g = 0, b = 0;

      for (int i=0; i<samples; i++) {
         auto c_col_base = c_base + ((real)i)*sample_dx;
         for (int j=0; j<samples; i++) {
            auto c = c_col_base + ((real)j)*sample_dy;
            auto i = iterate(c);
            auto color = mc.get_color(i);
            r += color.r;
            g += color.g;
            b += color.b;
         }
      }
      r /= samples*samples;
      g /= samples*samples;
      b /= samples*samples;
      return {r, g, b};
   }

}

