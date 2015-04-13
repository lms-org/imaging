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

namespace op{
//##################################################
//################FILTER
//##################################################
const double KERNEL_GAUSS_5[5][5] = {{1,4,6,4,1},{4,16,24,16,4},{6,24,36,24,6},{4,16,24,16,4},{1,4,6,4,1}};
const int GAUSS_5_DIV = 256;
const double KERNEL_SOBEL_3_Y[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
const double KERNEL_SOBEL_3_X_TODO[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
//##################################################
//################GAUS
//##################################################
//default methods

int validateX(int x,int width,bool reflect);
int validateY(int x,int width,bool reflect);
void imageOperator(const Image &input, Image &output,const double *mat,int matRows, int matCols,bool reflectBorders,int devisor=1);
double imageOperator(const Image &image,int x, int y,const double *mat,int matRows, int matCols,bool reflectBorders,int devisor = 1);
/**
 * TODO remove that function, use validateX
 */
int reflectX(int width, int px);
/**
 * TODO remove that function, use validateY
 */
int reflectY(int height, int py);
//not sure if we need getPixel and setPixel
void getPixel(Pixel &pixel,Image &image);
void setPixel(Pixel &pixel,Image &image);
//gaus
void gaussPxl(const Image &input,Image *output, Pixel *pixel,int x, int y);
void gaussPxl(const Image &input,Image *output, Pixel &pixel);
int gaussPxlGrey(const Image &input,int x, int y);
/**
 * TODO set the size of the output image if it's not set yet and the type etc...
 * @brief gauss
 * @param input
 * @param output
 */
void gauss(const Image &input,Image &output);
//##################################################
//################GAUS END
//##################################################


//##################################################
//################SOBEL
//##################################################
//sobel
void sobelPxl(const Image &input,Image *output, Pixel *pixel,int x, int y);
void sobel(const Image &input,Image &output);

void sobelX(const Image &input, Image &output);
void sobelY(const Image &input, Image &output);
int sobelX(int x, int y,const Image &image);
int sobelY(int x, int y,const Image &image);
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
//##################################################
//################SOBEL END
//##################################################

//##################################################
//################STANDARD
//##################################################

void subtract(const Image &input1, const Image &input2, Image &output, int minVal=0, int maxVal=255);
}  //namepace op
}  // namespace imaging
}  // namespace lms

#endif /* LMS_IMAGING_IMAGE_FACTORY_H */
