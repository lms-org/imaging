#ifndef LMS_IMAGING_TRANSFORM_IMAGE_H
#define LMS_IMAGING_TRANSFORM_IMAGE_H

#include <lms/imaging/image.h>
#include <functional>
#include <lms/math/vertex.h>
namespace lms{
namespace imaging{

struct Transformation{
    virtual void t(const int &x,const int &y,float &rX,float &rY) const = 0;

    virtual void ti(int &x,int &y,const float &rX,const float &rY) const = 0;

    void vt(const lms::math::vertex2i &ip, lms::math::vertex2f &p) const{
        t(ip.x,ip.y,p.x,p.y);
    }
    void vti(lms::math::vertex2i &ip,const lms::math::vertex2f &p) const{
        ti(ip.x,ip.y,p.x,p.y);
    }

};

struct Distortion: public Transformation{
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
    virtual void t(const int x,const int y,float &rX,float &rY) const{
        //TODO
    }

    virtual void ti(int &x,int &y,const float &rX,const float &rY) const{
        //TODO
    }
};

struct Homography: public Transformation{
    /**
     * @brief cam2world transformation from pxl to meter
     */
    std::vector<float> cam2world;
    /**
     * @brief world2cam transformation from real world coordinates to pxl
     */
    std::vector<float> world2cam;
    /**
     * @brief ti transformation from meter to pxl
     * @param x
     * @param y
     * @param rX
     * @param rY
     */
    void ti(int &x,int &y,const float &rX,const float &rY) const override{
        float a = rX * world2cam[0] + rY * world2cam[1] + world2cam[2];
        float b = rX * world2cam[3] + rY * world2cam[4] + world2cam[5];
        float c = rX * world2cam[6] + rY * world2cam[7] + world2cam[8];
        x = a / c;
        y = b / c;
    }
    /**
     * @brief t transformation from pxl to real world
     * @param x
     * @param y
     * @param rX
     * @param rY
     */
    void t(const int &x,const int &y,float &rX,float &rY) const override{
        float a = x * cam2world[0] + y * cam2world[1] + cam2world[2];
        float b = x * cam2world[3] + y * cam2world[4] + cam2world[5];
        float c = x * cam2world[6] + y * cam2world[7] + cam2world[8];
        rX = a / c;
        rY = b / c;
    }
};

}//imaging
}//lms
#endif //LMS_IMAGING_TRANSFORM_IMAGE_H
