#ifndef LMS_IMAGING_WARP_H
#define LMS_IMAGING_WARP_H

#include <cstdint>
#include <array>

#include "lms/imaging/image.h"

// DIRTY CODE -> DO NOT READ

namespace lms {
namespace imaging {

/**
 * Simple vertex implementation.
 */
template <int _DIM, typename _T>
class vertex : public std::array<_T, _DIM>  {
};

typedef vertex<2, int>    vertex2i;
typedef vertex<3, int>    vertex3i;
typedef vertex<2, float>  vertex2f;
typedef vertex<3, double> vertex3d;
typedef vertex<3, float>  vertex3f;

// Camera to Vehicle
bool C2V(const vertex2i*, vertex2f*, float* angle_out = nullptr);

//Vehicle to Camera
bool V2C(const vertex2f *, vertex2i *, float* angle_out = nullptr);

float LP_Angle_to_rad(int8_t);
int8_t rad_to_angle_lp(float);

bool n2d(const float & xn, const float & yn, float & xdist, float & ydist);

void imageV2C(const Image &input, Image &output);

}  // namespace imaging
}  // namespace lms

#endif /* LMS_IMAGING_WARP_H */
