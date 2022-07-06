#include <image.hpp>
#include <pixel.hpp>
#include <timer.hpp>
#include <types.hpp>

#include <iostream>
#include <cmath>

// 1k
//static const unsigned im_x = 1920;
//static const unsigned im_y = 1080;

// 2k
//static const unsigned im_x = 2560;
//static const unsigned im_y = 1440;

// 4k
//static const unsigned im_x = 3810;
//static const unsigned im_y = 2160;

// 8k
//static const unsigned im_x = 7620;
//static const unsigned im_y = 4320;

// 16k
const unsigned im_x = 15240;
const unsigned im_y = 8640;
   

static const real xmin = -0.777120713150274923773;
static const real xmax = -0.777120273471042550002;
static const real ymin = 0.126857151509958518545;
static const real ymax = 0.126857366062765260933;
static const real height = (ymax - ymin)/2;
static const comp center = {(xmin + xmax)/2, (ymin + ymax)/2};
static const int iterlim = 200000;

//static const real height = 0.001;
//static const comp center = {-0.81055,0.2015};
//comp center = {-0.761575970961605,-0.084759600310245}; // Spiral!
//comp center = {-0.7746806106269039,-0.1374168856037867};
//complex center = {-0.685, 0.3};

static const size_t step[4] = {150, 150, 400, 10000};
static constexpr size_t N[4] = {0,1,1000,1000};
static constexpr size_t D[4] = {1,1,1,1};
static constexpr bool inv[4] = {false, true, false, false};
static const size_t offset = 9800;

static const im::pixel Y = {0xFC, 0xF4, 0x34}; 
static const im::pixel W = {0xFF, 0xFF, 0xFF}; 
static const im::pixel P = {0x9C, 0x59, 0xD1}; 
static const im::pixel B = {0x2C, 0x2C, 0x2C};
static const im::pixel colors[4] = {Y, W, P, B};
static const size_t step_sum[5] = {
   0, 
   step[0], 
   step[0] + step[1], 
   step[0] + step[1] + step[2], 
   step[0] + step[1] + step[2] + step[3]
};

template<int im_x, int im_y>
class mandelbrot {
   real height;
   comp center;

   comp pixel_tl;
   comp pixel_dx;
   comp pixel_dy;

   unsigned iterlim;
   unsigned cycle;

public:

   mandelbrot(real _height, comp _center, unsigned _iterlim) {
      height = _height;
      center = _center;
      iterlim = _iterlim;

      auto pixel_height = 2 * (height / im_y);

      pixel_dx = {pixel_height, 0};
      pixel_dy = {0,-pixel_height};

      auto frame_tl = center - (((real)im_x) / 2) * pixel_dx - (((real)im_y) / 2) * pixel_dy;
      pixel_tl = frame_tl + (0.5*pixel_dx) + (0.5*pixel_dy);
   }
   
   template <int N, size_t D>
   inline real blend(real x) {
      if (x == 0.0)
         return 0.0;
      if (N == 0)
         return x - x * std::log(x);
      if (D == 0)
         return x;
      return ((N + D) * x - D*std::pow(x, ((real)(D+N))/D))/N;
   }

   template <int N, size_t D>
   inline real blend_inv(real x) {
      return std::pow(x, ((double)N)/D);
   }


   template <size_t N, size_t D>
   inline png_byte blend(png_byte c1, png_byte c2, real p) {
      real bp = blend<N, D>(p);
      real bq = 1 - bp;
      return (png_byte)(((real)c1) * bq + ((real)c2) * bp);
   }

   template <size_t N, size_t D>
   inline png_byte blend_inv(png_byte c1, png_byte c2, real p) {
      real bp = blend_inv<N, D>(p);
      real bq = 1 - bp;
      return (png_byte)(((real)c1) * bq + ((real)c2) * bp);
   }

   template <size_t N, size_t D = 1>
   inline im::pixel blend(im::pixel p1, im::pixel p2, int n, int d) {
      real p = ((real)n)/d;
      return {
         blend<N, D>(p1.r, p2.r, p),
         blend<N, D>(p1.g, p2.g, p),
         blend<N, D>(p1.b, p2.b, p)
      };
   }

   template <size_t N, size_t D = 1>
   inline im::pixel blend_inv(im::pixel p1, im::pixel p2, int n, int d) {
      real p = ((real)n)/d;
      return {
         blend_inv<N, D>(p1.r, p2.r, p),
         blend_inv<N, D>(p1.g, p2.g, p),
         blend_inv<N, D>(p1.b, p2.b, p)
      };
   }

   inline im::pixel paint(unsigned x, unsigned y) {

      auto c = pixel_tl + ((real)x) * pixel_dx + ((real)y) * pixel_dy;
      
      auto z = c;
      int i = 0;
      while (i < iterlim) {
         z = z*z + c; 
         if (std::norm(z) >= 4)
            break;
         i++;
      }

      if (i == iterlim)
         return {0,0,0};

      size_t cycle = (i + offset) % step_sum[4];
      if (cycle < step_sum[1]) {
         constexpr int i = 0;
         if (inv[i])
            return blend_inv<N[i], D[i]>(colors[i], colors[(i+1)%4], cycle - step_sum[i], step[i]);
         return blend<N[i], D[i]>(colors[i], colors[(i+1)%4], cycle - step_sum[i], step[i]);
      }
      if (cycle < step_sum[2]) {
         constexpr int i = 1;
         if (inv[i])
            return blend_inv<N[i], D[i]>(colors[i], colors[(i+1)%4], cycle - step_sum[i], step[i]);
         return blend<N[i], D[i]>(colors[i], colors[(i+1)%4], cycle - step_sum[i], step[i]);
      }
      if (cycle < step_sum[3]) {
         constexpr int i = 2;
         if (inv[i])
            return blend_inv<N[i], D[i]>(colors[i], colors[(i+1)%4], cycle - step_sum[i], step[i]);
         return blend<N[i], D[i]>(colors[i], colors[(i+1)%4], cycle - step_sum[i], step[i]);
      }
      if (cycle < step_sum[4]) {
         constexpr int i = 3;
         if (inv[i])
            return blend_inv<N[i], D[i]>(colors[i], colors[(i+1)%4], cycle - step_sum[i], step[i]);
         return blend<N[i], D[i]>(colors[i], colors[(i+1)%4], cycle - step_sum[i], step[i]);
      }
      return {0,0,0};
   }
};


int main() {
   auto t = timer();

   auto m = mandelbrot<im_x, im_y>(height, center, iterlim);

   im::image<im_x, im_y> im;

   im.paint_frame(m);

   im.write("nbmandel.png"); 
   std::cout << t.get_time() << std::endl;
}
