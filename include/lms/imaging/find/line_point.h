#ifndef IMAGE_LINE_POINT
#define IMAGE_LINE_POINT

#include "lms/imaging/find/edge_point.h"
#include "lms/imaging/find/pixel.h"
#include <lms/imaging/draw_debug.h>

namespace lms{
namespace imaging{
namespace find{


class LinePoint {

public:

    struct LinePointParam:public EdgePoint::EdgePointParam{
    LinePointParam():lineWidthMin(0),lineWidthMax(0){
    }
        float lineWidthMin;
        float lineWidthMax;
    };

    void setParam(const LinePointParam &param);
    bool find(const LinePointParam &param DRAWDEBUG_PARAM);
    bool find(DRAWDEBUG_PARAM_N);

    EdgePoint low_high, high_low;
    float getAngle();
    float getSlope();
    float distance();

private:
    LinePointParam m_LinePointParam;
};

} //namepsace find
} //namespace imaging
} //namespace lms
#endif // IMAGE_LINE_POINT
