#ifndef IMAGE_EDGE_POINT

#define IMAGE_EDGE_POINT

#include "lms/imaging/find/pixel.h"
#include <math.h>

namespace lms{
namespace imaging{
namespace find{
class EdgePoint{

    public:

        enum edgetype {low_high, high_low, plane};

        Pixel pixel;
        int gradient = NAN;
        int direction = NAN;
        float non_disc_direction = NAN; //in Bogenmaß
        bool verified;
        edgetype type;

        bool find(Pixel pixel, int length, int16_t dir, edgetype searchtype, int threshold);
    protected:
        Pixel lastPoints[2];
        bool confirmEdgePoint(int threshold);
        void nonMaximumSupression(int rowShift, int colShift);
        void checkType();

};

} //namepsace find
} //namespace imaging
} //namespace lms

#endif // IMAGE_EDGE_POINT
