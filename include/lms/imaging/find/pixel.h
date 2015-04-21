#ifndef IMAGE_PIXEL
#define IMAGE_PIXEL

#include <stdint.h>
#include <lms/imaging/image.h>

#define DEBUG_PAINT 1

namespace lms{
namespace imaging{
namespace find{

class Pixel {

public:
    uint16_t x;
    uint16_t y;

    Pixel() : x(0), y(0) {}
    Pixel(uint16_t _x, uint16_t _y) : x(_x), y(_y) {}

    int fastGaussian();
    void GaussianSurround();
    void sobel(int* gradient, int* direction, float* real_direction);
    bool insideImage();
    bool move(int dx, int dy);
    Image* getImage();
    void setImage(Image *image);
private:
    /**
     * @brief image the image containing the pixel
     */
    Image *image;
    int sobelX();
    int sobelY();
    int calcGradientMag(int valueSobelx, int valueSobely);
    int calcGradientDir(int valueSobelx, int valueSobely, float* real_direction);
    int reflectX(int px);
    int reflectY(int py);

public:

    void paint(int color);

    #if DEBUG_PAINT

        ///all colours
        void blue() { paint(1); }
        void lightblue() { paint(10); }
        void red() { paint(2); }
        void green() { paint(3); }
        void yellow() { paint(4); }
        void purple() { paint(5); }
        void rosa() { paint(6); }


        ///only blue and purple
    //    void paint(int color);
    //    void blue() { paint(1); }
    //    void lightblue() { }
    //    void red() {  }
    //    void green() {  }
    //    void yellow() {  }
    //    void purple() { paint(5); }
    //    void rosa() {  }
    //
         ///without blue and purple
    //    void paint(int color);
    //    void blue() {  }
    //    void lightblue() { paint(10); }
    //    void red() { paint(2); }
    //    void green() { paint(3); }
    //    void yellow() { paint(4); }
    //    void purple() {  }
    //    void rosa() { paint(6); }


        void color_verified() { green (); }
        void color_searched() { lightblue(); }

        void color_selected_local_maximum() { blue(); }
        void color_searched_local_maximum() { red(); }

    #endif // DEBUG_PRINT

    #if !DEBUG_PAINT

        void blue() {}
        void lightblue() {}
        void red() {}
        void green() {}
        void yellow() {}
        void purple() {}
        void rosa() {}

        void color_verified() {}
        void color_searched() {}

        void color_selected_local_maximum() {}
        void color_searched_local_maximum() {}

    #endif // DEBUG_PRINT

    void paint_cross();



};

} //namepsace find
} //namespace imaging
} //namespace lms
#endif // IMAGE_PIXEL
