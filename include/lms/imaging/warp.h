#ifndef LMS_IMAGING_WARP_H
#define LMS_IMAGING_WARP_H

#include <cstdint>
#include <array>

#include "lms/imaging/image.h"
#include "lms/math/vertex.h"

// DIRTY CODE -> DO NOT READ

namespace lms {
namespace imaging {

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
