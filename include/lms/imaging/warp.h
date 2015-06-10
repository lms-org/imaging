#ifndef LMS_IMAGING_WARP_H
#define LMS_IMAGING_WARP_H

#include <cstdint>
#include <array>

#include "lms/imaging/image.h"
#include "lms/math/vertex.h"
#include "lms/type/module_config.h"
#include "lms/framework.h"

// DIRTY CODE -> DO NOT READ

namespace lms {
namespace imaging {


struct WarpContent{
    int CALI_WIDTH;
    int CALI_HEIGHT;
    std::vector<float> d2nX;
    std::vector<float> d2nY;
    std::vector<float> ta2b;
    std::vector<float> tb2a;

    float Fx;
    float Fy;
    float Cx;
    float Cy;
    float K1;
    float K2;
    float K3;
    float K4;
    float K5;

    void fromConfigDirectory(){
        fromConfigFile(lms::Framework::configsDirectory+"/camera/cali.lconf");
    }

    void fromConfigFile(std::string pathToConfig){
        lms::type::ModuleConfig mc;
        mc.loadFromFile(pathToConfig);
        fromConfig(&mc);
    }

    void fromConfig(const lms::type::ModuleConfig *cali){
        CALI_WIDTH = cali->get<int>("col");
        CALI_HEIGHT = cali->get<int>("row");
        ta2b = cali->getArray<float>("world2cam");
        tb2a = cali->getArray<float>("cam2world");
        d2nX = cali->getArray<float>("d2nX");
        d2nY = cali->getArray<float>("d2nY");
        Fx = cali->get<float>("Fx");
        Fy = cali->get<float>("Fy");
        Cx = cali->get<float>("Cx");
        Cy = cali->get<float>("Cy");
        K1 = cali->get<float>("K1");
        K2 = cali->get<float>("K2");
        K3 = cali->get<float>("K3");
        K4 = cali->get<float>("K4");
        K5 = cali->get<float>("K5");

        //debug

        std::cout << "row: " <<CALI_WIDTH << " " << " col: " << CALI_HEIGHT << std::endl;
        std::cout << "t2: " <<ta2b.size() << " " << tb2a.size() << std::endl;
        std::cout << "d2nX: " <<d2nX.size() << " " << " d2nY: " << d2nY.size() << std::endl;
    }

};

// Camera to Vehicle
bool C2V(const lms::math::vertex2i*, lms::math::vertex2f*, float* angle_out = nullptr);

//Vehicle to Camera
bool V2C(const lms::math::vertex2f *, lms::math::vertex2i *, float* angle_out = nullptr);

float LP_Angle_to_rad(int8_t);
int8_t rad_to_angle_lp(float);

bool n2d(const float & xn, const float & yn, float & xdist, float & ydist);

void imageV2C(const Image &input, Image &output);

}  // namespace imaging
}  // namespace lms

#endif /* LMS_IMAGING_WARP_H */
