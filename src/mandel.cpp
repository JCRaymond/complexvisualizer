
#include <iostream>
#include <iomanip>

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
   const size_t im_x = 3840;
   const size_t im_y = 2160;
   const size_t K = 8;
   size_t iterlim = 10000;
   real rad = 2;

   mandelbrot m(iterlim, rad);
   //julia j({.01548,.86415}, iterlim, rad);

   
   std::vector<im::pixel> palette = {
      {0xFC, 0xF4, 0x34},
      {0xFF, 0xFF, 0xFF},
      {0x9C, 0x59, 0xD1},
      {0x2C, 0x2C, 0x2C}
   };

   blender<
      0,
      blend_to<0, 3, 0, 1>,
      blend_to<1, 8, 0, 0>,
      blend_to<1, 3, 0, 1>,
      blend_to<2, 8, -1, 1>,
      blend_to<2, 3, 0, 1>,
      blend_to<3, 9, 0, 0>,
      blend_to<3, 3, 0, 1>,
      blend_to<0, 9, 0, 0>
   > b(palette, 12);

/*  
   size_t offset = 9800;
   blender<
      0,
      blend_to<1, 150, 0, 1>,
      blend_to<2, 150, -1, 1>,
      blend_to<3, 400, 0, 0>,
      blend_to<0, 10000, 0, 0>
   > b(palette, offset);
*/   
/*
   std::vector<im::pixel> palette = {
      {255, 0, 0},
      {255, 255, 0},
      {0, 255, 0},
      {0, 255, 255},
      {0, 0, 255},
      {255, 0, 255}
   };

   static const size_t rate = 10; 
   blender<
      0,
      blend_to<1, rate>,
      blend_to<2, rate>,
      blend_to<3, rate>,
      blend_to<4, rate>,
      blend_to<5, rate>,
      blend_to<0, rate>
   > b(palette);
*/
   iterlimiter il(iterlim, b);

   /*
   static const real xmin = -0.777120713150274923773;
   static const real xmax = -0.777120273471042550002;
   static const real ymin = 0.126857151509958518545;
   static const real ymax = 0.126857366062765260933;
   static const real height = (ymax - ymin)/2;
   static const comp center = {(xmin + xmax)/2, (ymin + ymax)/2};
   */
   real height = 1e-9;
   comp center = {-0.749569999999999958539831368398, 0.0343750989999999992385859570732};

   real p_height = 2 * (height / im_y);
   comp dx = {p_height, 0};
   comp dy = {0, -p_height};
   comp tl = center - (((real)im_x)/2) * dx - (((real)im_y) / 2) * dy;
   resolution<K>::citer_painter cip(m, il, tl, dx, dy);

   im::image<im_x, im_y> im;
   im.paint_frame(cip);
   im.write("new_mandel.png");

   while (false) {
      char c;
      std::cin >> c;
      switch (c) {
         case 'h':
            center -= dx*((real)im_x)/((real)4);
            break;
         case 'c':
            center -= dy*((real)im_y)/((real)4);
            break;
         case 'n':
            center += dx*((real)im_x)/((real)4);
            break;
         case 't':
            center += dy*((real)im_y)/((real)4);
            break;
         case 'u':
            height /= 2;
            p_height = 2 * (height / im_y);
            dx = {p_height, 0};
            dy = {0, -p_height};
            break;
         case 'e':
            height *= 2;
            p_height = 2 * (height / im_y);
            dx = {p_height, 0};
            dy = {0, -p_height};
            break;
         case 'p':
            iterlim += 100;
            m.iterlim = iterlim;
            il.iterlim = iterlim;
            break;
         case '.':
            iterlim -= 100;
            m.iterlim = iterlim;
            il.iterlim = iterlim;
            break;
         case 'a':
            std::cout << "===" << std::endl;
            std::cout << "{" << std::setprecision(30) << center.real() << ", " << std::setprecision(30) << center.imag() << "}" << std::endl;
            std::cout << std::setprecision(30) << height << std::endl;
            std::cout << "===" << std::endl;
            break;
      }

      tl = center - (((real)im_x)/2) * dx - (((real)im_y) / 2) * dy;
      cip = resolution<K>::citer_painter(m, il, tl, dx, dy);

      im.paint_frame(cip);
      im.write("mandel_search.png");
   }
}


