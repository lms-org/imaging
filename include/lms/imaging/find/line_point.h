#ifndef IMAGE_LINE_POINT
#define IMAGE_LINE_POINT

#include "lms/imaging/find/edge_point.h"
#include "lms/imaging/find/pixel.h"

namespace lms{
namespace imaging{
namespace find{


class LinePoint {

public:
    EdgePoint low_high, high_low;
    bool find(Pixel &startPoint, int searchLength, float searchAngle,int minWidth,int maxWidth, int sobelThreshold,Image &gaussBuffer);

};

} //namepsace find
} //namespace imaging
} //namespace lms
#endif // IMAGE_LINE_POINT
