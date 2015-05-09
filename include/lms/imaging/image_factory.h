#ifndef LMS_IMAGING_IMAGE_FACTORY_H
#define LMS_IMAGING_IMAGE_FACTORY_H

#include <lms/imaging/image.h>
#include <functional>
#include <vector>

namespace lms {
namespace imaging {

namespace op{
//##################################################
//################FILTER
//##################################################
/*
 * How to use it:
 * op::imageOperator(imageTarget,x,y,&lms::imaging::op::KERNEL_SOBEL_5_X[0][0],5,5)
 */
const double KERNEL_GAUSS_5[5][5] = {{1,4,6,4,1},{4,16,24,16,4},{6,24,36,24,6},{4,16,24,16,4},{1,4,6,4,1}};
const int GAUSS_5_DIV = 256;
const double KERNEL_SOBEL_3_Y[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
const double KERNEL_SOBEL_3_X[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
const double KERNEL_SOBEL_5_X[5][5] = {{1,  2, 0,  -2, -1},
                                    {4 , 8 ,0 , -8 ,-4},
                                    {6 ,12, 0 ,-12, -6},
                                    {4 , 8 ,0  ,-8 ,-4},
                                    {1 , 2 ,0  ,-2 ,-1}};
const double KERNEL_SOBEL_5_Y[5][5] = {{1,4,6,4,1},
                                       {2,8,12,8,2},
                                       {0,0,0,0,0},
                                       {-2,-8,-12,-8,-2},
                                       {-1,-4,-6,-4,-1}};
//##################################################
//################GAUS
//##################################################
//default methods

int validateX(int x,int width,bool reflect);
int validateY(int x,int width,bool reflect);
void imageOperator(const Image &input, Image &output,const double *mat,int matRows, int matCols,bool reflectBorders=true,int devisor=1);
double imageOperator(const Image &image,int x, int y,const double *mat,int matRows, int matCols,bool reflectBorders=true,int devisor = 1);


//##################################################
//################GAUS
//##################################################

/**
 * TODO set the size of the output image if it's not set yet and the type etc...
 * @brief gauss
 * @param input
 * @param output
 */
void gauss(const Image &input,Image &output);
void gaussPxl(const Image &input,Image &output,int x, int y);
int gaussGrey(const Image &input,int x, int y);

/**
 * @brief gaussBox
 * @param input
 * @param output
 * @param xMin
 * @param yMin
 * @param xMax
 * @param yMax
 */
void gaussBox(const Image &input,Image &output,int xMin, int yMin,int xMax, int yMax);

//##################################################
//################GAUS END
//##################################################


//##################################################
//################SOBEL
//##################################################
//sobel
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

void bresenhamLine(int x0, int y0, int x1, int y1,std::function<bool(int,int)> foundPixel);
void bresenhamLine(int x0, int y0, int x1, int y1, std::vector<int> &vX, std::vector<int> &vY);

}  // namespace imaging
}  // namespace lms

#endif /* LMS_IMAGING_IMAGE_FACTORY_H */
