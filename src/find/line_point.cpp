#include "lms/imaging/find/line_point.h"
#include "lms/imaging/find/edge_point.h"
//#include "image/image_tools.h"
#include <lms/imaging/draw_debug.h>
#include <cmath>

namespace lms{
namespace imaging{
namespace find{

bool LinePoint::find(Pixel &startPoint, int searchLength, float searchAngle,int minWidth,int maxWidth, int sobelThreshold,Image &gaussBuffer DRAWDEBUG) {
    //try to find first point, if it fails return as no LinePoint can be found
    if(!low_high.find(startPoint, searchLength, searchAngle,EdgePoint::EdgeType::LOW_HIGH, sobelThreshold,gaussBuffer DRAWDEBUG_ARG)){
        return false;
    }
    DRAWCROSS(low_high.x,low_high.y,0,255,0);
    //calculate search-values for low_high edge
    //angle of the sobel
    //TODO: Maybe do some error checking on the sobelAngle?
    //TODO we should reduce the search-length
    if(!high_low.find(low_high, searchLength, low_high.sobelAngle(),EdgePoint::EdgeType::LOW_HIGH, sobelThreshold,gaussBuffer DRAWDEBUG_ARG)){
        //high_low edge wasn't found, try to find it in the old searchAngle
        if(!high_low.find(low_high, searchLength, searchAngle,EdgePoint::EdgeType::LOW_HIGH, sobelThreshold,gaussBuffer DRAWDEBUG_ARG)){
            return false;
        }
    }
    //found both :)
    //check the length (add min/max Length)
    float distance = high_low.distance(low_high);
    if(distance < minWidth || distance > maxWidth)
        return false;

}

float LinePoint::getAngle(){
    return atan(getSlope());
}

float LinePoint::getSlope(){
    float dx = low_high.x-high_low.x;
    float dy = low_high.y-high_low.y;
    return dy/dx;
}

} //namepsace find
} //namespace imaging
} //namespace lms
