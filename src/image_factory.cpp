#include <lms/imaging/image_factory.h>
#include <lms/imaging/image.h>
#include <cmath>
namespace lms {
namespace imaging {
void getPixel(Pixel &pixel,Image &image){
    //TODO
    switch (image.format()) {
    case Format::GREY:
        //TODO
        //*(image.data()+input.width()*pixel.y + pixel.x );
        break;
    default:
        break;
    }
}

void setPixel(Pixel &pixel,Image &image){
    //TODO
    switch (image.format()) {
    case Format::GREY:
        //TODO
        *(image.data()+image.width()*pixel.y + pixel.x ) = pixel.r;
        break;
    default:
        break;
    }
}

namespace op{

void gaussPxl(const Image &input,Image *output, Pixel *pixel,int x, int y){
    //TODO
    if(input.format() == Format::GREY){
        int gauss = gaussPxlGrey(input,x,y);
        if(output != nullptr){
            if(output->format() != input.format()){
                //TODO throw error
                return;
            }
            // set pixel
            *(output->data()+input.width()*y + x ) = gauss;

        }
        if(pixel != nullptr){
            pixel->x = x;
            pixel->y = y;
            pixel->a = 1;
            pixel->r=pixel->g=pixel->b=gauss;
        }
    }else{
        //ERROR not implemented yetgaussPxlGrey
        return;
    }
}

int gaussPxlGrey(const Image &input, int x, int y){
    //TODO nur vorrübergehend
    const std::uint8_t *IMAGE = input.data();
    int IMAGE_WIDTH = input.width();
    int IMAGE_HEIGHT = input.height();

    int valueGauss = 0;
    int x_index = 0;
    int y_index = 0;

    y_index = IMAGE_WIDTH * (reflectY(IMAGE_HEIGHT,y + (0-1)));

    x_index = reflectX(IMAGE_WIDTH,x + (0-1));
    valueGauss += *(IMAGE + (y_index + (x_index))) >> 4;
    x_index = reflectX(IMAGE_WIDTH,x + (1-1));
    valueGauss += *(IMAGE + (y_index + (x_index))) >> 3;
    x_index = reflectX(IMAGE_WIDTH,x + (2-1));
    valueGauss += *(IMAGE + (y_index + (x_index))) >> 4;

    y_index = IMAGE_WIDTH * (reflectY(IMAGE_HEIGHT,y + (1-1)));
    x_index = reflectX(IMAGE_WIDTH,x + (0-1));
    valueGauss += *(IMAGE + (y_index + (x_index))) >> 3;
    x_index = reflectX(IMAGE_WIDTH,x + (1-1));
    valueGauss += *(IMAGE + (y_index + (x_index))) >> 2;
    x_index = reflectX(IMAGE_WIDTH,x + (2-1));
    valueGauss += *(IMAGE + (y_index + (x_index))) >> 3;

    y_index = IMAGE_WIDTH * (reflectY(IMAGE_HEIGHT,y + (2-1)));
    x_index = reflectX(IMAGE_WIDTH,x + (0-1));
    valueGauss += *(IMAGE + (y_index + (x_index))) >> 4;
    x_index = reflectX(IMAGE_WIDTH,x + (1-1));
    valueGauss += *(IMAGE + (y_index + (x_index))) >> 3;
    x_index = reflectX(IMAGE_WIDTH,x + (2-1));
    valueGauss += *(IMAGE + (y_index + (x_index))) >> 4;

   return valueGauss;
}

void gaussPxl(const Image &input,Image *output, Pixel &pixel){
    gaussPxl(input,output,&pixel,pixel.x,pixel.y);
}

void gauss(const Image &input,Image &output){
    for(int x = 0; x < input.width(); x++){
        for(int y = 0; y < input.height(); y++){
            gaussPxl(input,&output,nullptr,x,y);
        }
    }
}

void sobelPxl(const Image &input,Image *output, Pixel pixel,int x, int y){
    //TODO
}

void sobel(const Image &input,Image &output){
    for(int x = 0; x < input.width(); x++){
        for(int y = 0; y < input.height(); x++){
            sobelPxl(input,&output,nullptr,x,y);
        }
    }
}

void sobelX(const Image &input, Image &output) {
    //TODO error checking
    for(int x = 0; x < input.width();x++){
        for(int y = 0; y< input.height(); y++){
            *(output.data()+y*input.width()+x)=sobelX(x,y,input);
        }
    }
}
void sobelY(const Image &input, Image &output) {
    //TODO error checking
    for(int x = 0; x < input.width();x++){
        for(int y = 0; y< input.height(); y++){
            *(output.data()+y*input.width()+x)=sobelY(x,y,input);
        }
    }
}
void imageOperator(const Image &input,Image &output,const double *mat,int matRows, int matCols,bool reflectBorders,int devisor) {
    //TODO error checking
    for(int x = 0; x < input.width();x++){
        for(int y = 0; y< input.height(); y++){
            *(output.data()+y*input.width()+x)=(int)imageOperator(input,x,y,mat,matRows,matCols,reflectBorders,devisor);
        }
    }
}

double imageOperator(const Image &image,int x, int y,const double *mat,int matRows, int matCols,bool reflectBorders,int devisor){
    //TODO error checking
    int x_index = 0;
    int y_index = 0;

    double result= 0;
    int x_start = x-matCols/2;
    int y_start = y-matRows/2;
    //y-axis
    for(int k = 0; k < matRows; k++) {
        y_index = image.width()*(validateY(y_start+k,image.height(),reflectBorders));
        if(y_index < 0){
            continue;
        }
        for(int j = 0; j < matCols; j++) {
            x_index = validateX(x_start + j,image.width(),reflectBorders);
            if(x_index < 0){
                continue;
            }
            result += *(image.data() + y_index + x_index)* *(mat+k*matCols+j);
        }
    }
    return result/devisor;

}




int validateX(int x,int width,bool reflect){
    if(x < 0){
        if(reflect){
            return - x - 1;
        }else
            return 0;
    }
    if(x >= width){
        if(reflect){
            return 2*width-x-1;
        }else
            return width -1;
    }
    return x;
}
int validateY(int y,int height,bool reflect){
    if(y < 0){
        if(reflect){
            return - y - 1;
        }else
            return -1;
    }
    if(y >= height){
        if(reflect){
            return 2*height-y-1;
        }else
            return -1;
    }
    return y;
}


int sobelX(int x, int y, const Image &image) {
    //TODO error checking
    const std::uint8_t* data = image.data();
    int height = image.height();
    int width = image.width();

    int valueSobelX = 0;
    int y_index = width * (reflectY(height,y + (0 - 1)));

    valueSobelX -= *(data + y_index + reflectX(width,x + (0-1)));
    valueSobelX += *(data + y_index + reflectX(width,x + (2-1)));

    y_index = width * (reflectY(height,y + (1 - 1)));

    valueSobelX -= *(data + y_index + reflectX(width,x + (0-1))) << 1;
    valueSobelX += *(data + y_index + reflectX(width,x + (2-1))) << 1;

    y_index = width * (reflectY(height,y + (2 - 1)));

    valueSobelX -= *(data + y_index + reflectX(width,x + (0-1)));
    valueSobelX += *(data + y_index + reflectX(width,x + (2-1)));

    return valueSobelX;
}

int sobelY(int x, int y,const Image &image) {
    //TODO error checking
    const std::uint8_t* data = image.data();
    int height = image.height();
    int width = image.width();

    int valueSobelY = 0;
    int y_index = width * (reflectY(height,y + (0 - 1)));

    valueSobelY += *(data + y_index + reflectX(width,x + (0-1)));
    valueSobelY += *(data + y_index + reflectX(width,x + (1-1))) << 1;
    valueSobelY += *(data + y_index + reflectX(width,x + (2-1)));

    y_index = width * (reflectY(height,y + (2 - 1)));

    valueSobelY -= *(data + y_index + reflectX(width,x + (0-1)));
    valueSobelY -= *(data + y_index + reflectX(width,x + (1-1))) << 1;
    valueSobelY -= *(data + y_index + reflectX(width,x + (2-1)));

    return valueSobelY;

}

int reflectX(int width, int px) {

    if(px < 0)
    {
        //printf("KLeiner Null");
        return - px - 1;
    }

    else if(px >= width)
    {
        //printf("Größer size");
        return 2*width - px - 1;

    }

   return px;

}

int reflectY(int height, int py) {

    if(py < 0){
        return - py - 1;
    }

    else if(py >= height){
        return 2*height - py - 1;

    }

   return py;

}


void subtract(const Image &input1, const Image &input2, Image &output, int minVal, int maxVal){
    //TODO errorchecking
    int value = 0;
    if(input1.format() == Format::GREY){
        for(int x = 0; x < input1.width(); x++){
            for(int y = 0; y < input1.height(); y++){
                int v1 = *(input1.data()+y*input1.width()+x);
                int v2 = *(input2.data()+y*input1.width()+x);
                value = abs(v1-v2);
                if(value <= minVal)
                    value = 0;
                if(value >= maxVal){
                    value = 255;
                }
                *(output.data()+y*input1.width()+x) = value;
            }
        }
    }else{
        //not implemented yet
    }
}//namespace op
}//namespace imaging
}//namespace lms
}
