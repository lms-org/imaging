#ifndef LMS_IMAGING_IMAGE_FACTORY_H
#define LMS_IMAGING_IMAGE_FACTORY_H

#include <lms/imaging/image.h>
namespace lms {
namespace imaging {
struct Pixel{
    //position
    int x;
    int y;
    //color
    int r;
    int g;
    int b;
    int a;
};
//default methods
/**
 * don't like it
 */
int reflectX(int width, int px);
/**
 * don't like it
 */
int reflectY(int height, int py);
void getPixel(Pixel &pixel,Image &image);
void setPixel(Pixel &pixel,Image &image);
//gaus
void gaussPxl(const Image &input,Image *output, Pixel *pixel,int x, int y);
void gaussPxl(const Image &input,Image *output, Pixel &pixel);
int gaussPxlGrey(const Image &input,int x, int y);
void gauss(const Image &input,Image &output);
//sobel
void sobelPxl(const Image &input,Image *output, Pixel *pixel,int x, int y);
void sobel(const Image &input,Image &output);
/**
 * @brief cannyPxl
 * @param input
 * @param gaussbuffer
 * @param output
 * @param x
 * @param y
 * @return pixel color at the given position, maybe wrong here as we would like to say canny in x and y direction to calculate the angle
 */
//pixel cannyPxl(Image input, Image gaussbuffer, Image output, int x, int y);


}  // namespace imaging
}  // namespace lms

#endif /* LMS_IMAGING_IMAGE_FACTORY_H */
