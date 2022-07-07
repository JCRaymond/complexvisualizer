#ifndef BLENDER_HPP
#define BLENDER_HPP

#include <vector>
#include <cmath>

#include <pixel.hpp>

template <int N, size_t D>
inline real blend(real x) {
   if (x == 0.0)
      return 0;
   if (D == 0)
      return x;
   if (N == 0)
      return 0.0;
   if (N == 1)
      return x - x * std::log(x);
   if (N == -1)
      return x + (1-x) * std::log(1-x);
   if (N > 0)
      return (N*x - D*std::pow(x, ((real)N)/D))/(N-D);
   if (N < 0)
      return (-D - N*x - D*std::pow(1-x, -((real)N)/D))/(-N-D);
   return -1; // Should never happen!
}

inline png_byte blend(png_byte c1, png_byte c2, real p) {
   real q = 1 - p;
   return (png_byte)(((real)c1)*q + ((real)c2)*p);
}

inline im::pixel blend(im::pixel c1, im::pixel c2, real p) {
   return {
      blend(c1.r, c2.r, p),
      blend(c1.g, c2.g, p),
      blend(c1.b, c2.b, p)
   };
}

template <size_t to_idx, size_t iters, int N=0, size_t D=0>
struct blend_to {
   static constexpr int N_ = N;
   static constexpr size_t D_ = D;
   static constexpr size_t to_idx_ = to_idx;
   static constexpr size_t iters_ = iters;
};


struct blend_info {
   size_t f_idx;
   size_t t_idx;
   real p;
};

template <size_t s_idx, typename n_blend, typename... blends>
struct blender_info {
   static constexpr size_t cycle_len = n_blend::iters_;

   static inline blend_info get_blend_info(size_t i) {
      auto p = blend<n_blend::N_, n_blend::D_>(((real)i)/n_blend::iters_);
      return {s_idx, n_blend::to_idx_, p};
   }

};

template <size_t s_idx, typename n_blend, typename nn_blend, typename... blends>
struct blender_info<s_idx, n_blend, nn_blend, blends...> {
   static constexpr size_t cycle_len = n_blend::iters_ + blender_info<s_idx, nn_blend, blends...>::cycle_len;

   static inline blend_info get_blend_info(size_t i) {
      if (i >= n_blend::iters_) 
         return blender_info<n_blend::to_idx_, nn_blend, blends...>::get_blend_info(i - n_blend::iters_);
      auto p = blend<n_blend::N_, n_blend::D_>(((real)i)/n_blend::iters_);
      return {s_idx, n_blend::to_idx_, p};
   }

};

template <size_t s_idx, typename... blends>
struct blender {
   using info = blender_info<s_idx, blends...>;

   std::vector<im::pixel> palette;
   size_t offset;

   blender (std::vector<im::pixel> palette, size_t offset): palette(palette), offset(offset) {}
   blender (std::vector<im::pixel> palette): blender(palette, 0) {}

   inline im::pixel get_color(size_t i) {
      auto b_info = info::get_blend_info((i+offset) % info::cycle_len);
      return blend(palette[b_info.f_idx], palette[b_info.t_idx], b_info.p);
   }
};


#endif
