#ifndef ITERLIMITER_HPP
#define ITERLIMITER_HPP

#include <pixel.hpp>

template <typename i_colorizer>
struct iterlimiter {
   size_t iterlim;
   im::pixel limcolor;
   i_colorizer otherwise;

   iterlimiter (size_t iterlim, im::pixel limcolor, i_colorizer otherwise): iterlim(iterlim), limcolor(limcolor), otherwise(otherwise) {}
   iterlimiter (size_t iterlim, i_colorizer otherwise): iterlimiter(iterlim, {0,0,0}, otherwise) {}

   inline im::pixel get_color(size_t i) {
      if (i >= iterlim)
         return limcolor;
      return otherwise.get_color(i);
   }
};

#endif
