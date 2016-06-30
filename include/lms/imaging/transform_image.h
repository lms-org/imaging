#ifndef LMS_IMAGING_TRANSFORM_IMAGE_H
#define LMS_IMAGING_TRANSFORM_IMAGE_H

#include <lms/imaging/image.h>
#include <functional>
namespace lms{
namespace imaging{

struct Distortion{
    //undistort
    float Fx;
    float Fy;
    float Cx;
    float Cy;
    float K1;
    float K2;
    float K3;
    float K4;
    float K5;
    void undistort(const int x,const int y,int &rX,int $rY){
        //TODO
    }

    void distort(const int x,const int y,int &rX,int $rY){
        //TODO
    }
};

struct Homography{//TODO naming
    //Die variablen
    void project(const int x,const int y,float &rX,float $rY){
        //TODO
    }
    void unproject(const float x,const float y,int &rX,int $rY){
        //TODO
    }
};

}//imaging
}//lms
#endif //LMS_IMAGING_TRANSFORM_IMAGE_H
