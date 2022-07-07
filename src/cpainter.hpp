#ifndef CPAINTER_HPP
#define CPAINTER_HPP

#include <types.hpp>
#include <pixel.hpp>

template <size_t K, typename c_colorizer>
struct c_painter {
   c_colorizer cc;
   comp tl;
   comp dx;
   comp dy;
   comp ddx;
   comp ddy;

   c_painter (c_colorizer cc, comp tl, comp dx, comp dy) : cc(cc), tl(tl), dx(dx), dy(dy) {
      ddx = dx/((real)K);
      ddy = dy/((real)K);
   }

   inline im::pixel paint(size_t x, size_t y) {
      auto base = tl + ((real)x)*dx + ((real)y)*dy + ddx/((real)2) + ddy/((real)2);
      size_t r = 0;
      size_t g = 0;
      size_t b = 0;

      for (size_t i=0; i<K; i++) {
         auto curr = base + ddx*((real)i);
         auto scolor = cc.get_color(curr);
         r += scolor.r;
         g += scolor.g;
         b += scolor.b;
         for (size_t j=1; j<K; j++) {
            curr += ddy;
            scolor = cc.get_color(curr);
            r += scolor.r;
            g += scolor.g;
            b += scolor.b;
         }
      }

      return {r/(K*K), g/(K*K), b/(K*K)};
   }
};

#endif
