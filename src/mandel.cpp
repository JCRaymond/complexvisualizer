
#include <citerpainter.hpp>

#include <image.hpp>
#include <pixel.hpp>

struct mandelbrot {
   size_t iterlim;
   real rad;

   mandelbrot (size_t iterlim, real rad): iterlim(iterlim), rad(rad) {}

   inline size_t iterate(comp c) {
      auto z = c;
      size_t i = 0;
      while (i < mandelbrot::iterlim) {
         z = z*z + c;
         if (std::norm(z) >= mandelbrot::rad)
            break;
         i++;
      }
      return i;
   }
};

struct mandelbrot_colorizer {
   size_t iterlim;

   mandelbrot_colorizer (size_t iterlim): iterlim(iterlim) {}

   inline im::pixel get_color(size_t iters) {
      if (iters >= mandelbrot_colorizer::iterlim)
         return {0,0,0};
      switch (iters % 3) {
         case 0:
            return {255, 0, 0};
         case 1:
            return {0, 255, 0};
      }
      return {0, 0, 255};
   }
};

int main() {
   const size_t im_x = 1920;
   const size_t im_y = 1080;
   const size_t K = 4;
   const size_t iterlim = 200000;
   const real rad = 2;

   mandelbrot m(iterlim, rad);
   mandelbrot_colorizer mc(iterlim);

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
   citer_painter<K, mandelbrot_colorizer, mandelbrot> cip(m, mc, tl, dx, dy);

   im::image<im_x, im_y> im;
   im.paint_frame(cip);
   im.write("test.png");
}


