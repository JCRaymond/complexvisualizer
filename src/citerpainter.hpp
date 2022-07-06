#ifndef CITERPAINTER_HPP
#define CITERPAINTER_HPP

#include <cpainter.hpp>

#include <pixel.hpp>

struct i_colorizer_default {
   im::pixel get_color(size_t i) {
      return {0,0,0};
   }
};

struct c_iterator_default {
   size_t iterate(comp c) {
      return 0;
   }
};

template <typename i_colorizer, typename c_iterator>
struct c_composer {
   i_colorizer icolor;
   c_iterator citer;

   c_composer (i_colorizer icolor, c_iterator citer) : icolor(icolor), citer(citer) {}

   inline im::pixel get_color(comp c) {
      return icolor.get_color(citer.iterate(c));
   }
};

template <size_t K = 1, typename i_colorizer = i_colorizer_default, typename c_iterator = c_iterator_default>
struct citer_painter {
   c_painter<K, c_composer<i_colorizer, c_iterator>> cp;
   
   citer_painter(c_iterator citer, i_colorizer icolor, comp tl, comp dx, comp dy) : cp(c_painter<K, c_composer<i_colorizer, c_iterator>>(c_composer(icolor, citer), tl, dx, dy)) {}

   inline im::pixel paint(size_t x, size_t y) {
      return cp.paint(x,y);
   }
};

#endif
