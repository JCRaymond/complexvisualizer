
#include <iostream>

#include <citerpainter.hpp>
#include <blender.hpp>
#include <iterlimiter.hpp>

#include <image.hpp>
#include <pixel.hpp>

struct mandelbrot {
   size_t iterlim;
   real rad;

   mandelbrot (size_t iterlim, real rad): iterlim(iterlim), rad(rad) {}

   inline size_t iterate(comp c) {
      auto z = c;
      size_t i = 0;
      while (i < iterlim) {
         z = z*z + c;
         if (std::norm(z) >= rad)
            break;
         i++;
      }
      return i;
   }
};

struct julia {
   comp c;
   size_t iterlim;
   real rad;

   julia (comp c, size_t iterlim, real rad): c(c), iterlim(iterlim), rad(rad) {}

   inline size_t iterate(comp z) {
      size_t i = 0;
      while (i < iterlim) {
         z = z*z + c;
         if (std::norm(z) >= rad)
            break;
         i++;
      }
      return i;
   }
};


int main() {
   const size_t im_x = 1920; //3840;
   const size_t im_y = 1080; //2160;
   const size_t K = 4;
   const size_t iterlim = 200000;
   const real rad = 2;

   mandelbrot m(iterlim, rad);
   //julia j({.01548,.86415}, iterlim, rad);

   /*
   std::vector<im::pixel> palette = {
      {0xFC, 0xF4, 0x34},
      {0xFF, 0xFF, 0xFF},
      {0x9C, 0x59, 0xD1},
      {0x2C, 0x2C, 0x2C}
   };
  
   size_t offset = 9800;
   blender<
      0,
      blend_to<1, 150, 0, 1>,
      blend_to<2, 150, -1, 1>,
      blend_to<3, 400, 0, 0>,
      blend_to<0, 10000, 0, 0>
   > b(palette, offset);
   */

   std::vector<im::pixel> palette = {
      {255, 0, 0},
      {255, 255, 0},
      {0, 255, 0},
      {0, 255, 255},
      {0, 0, 255},
      {255, 0, 255}
   };

   static const size_t rate = 30; 
   blender<
      0,
      blend_to<1, rate>,
      blend_to<2, rate>,
      blend_to<3, rate>,
      blend_to<4, rate>,
      blend_to<5, rate>,
      blend_to<0, rate>
   > b(palette);

   iterlimiter il(iterlim, b);

   static const real xmin = -0.777120713150274923773;
   static const real xmax = -0.777120273471042550002;
   static const real ymin = 0.126857151509958518545;
   static const real ymax = 0.126857366062765260933;
   static const real height = (ymax - ymin)/2;
   static const comp center = {(xmin + xmax)/2, (ymin + ymax)/2};
   //static const real height = 1.1;
   //static const comp center = {0,0};

   static real p_height = 2 * (height / im_y);
   static comp dx = {p_height, 0};
   static comp dy = {0, -p_height};
   static comp tl = center - (((real)im_x)/2) * dx - (((real)im_y) / 2) * dy;
   resolution<K>::citer_painter cip(m, il, tl, dx, dy);

   im::image<im_x, im_y> im;
   im.paint_frame(cip);
   im.write("mandel_blend.png");
}


