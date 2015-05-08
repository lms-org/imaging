#include "lms/imaging/find/line_point.h"
#include "lms/imaging/find/edge_point.h"
//#include "image/image_tools.h"
#include <lms/imaging/draw_debug.h>
#include <cmath>

namespace lms{
namespace imaging{
namespace find{

float LinePoint::distance(){
    return low_high.distance(high_low);
}

bool LinePoint::find(Pixel &startPoint, int searchLength, float searchAngle,int minWidth,int maxWidth, int sobelThreshold,Image &gaussBuffer DRAWDEBUG) {
    //try to find first point, if it fails return as no LinePoint can be found
    low_high.setImage(startPoint.getImage());
    high_low.setImage(startPoint.getImage());
    if(!low_high.find(startPoint, searchLength, searchAngle,EdgePoint::EdgeType::LOW_HIGH, sobelThreshold,gaussBuffer DRAWDEBUG_ARG)){
        return false;
    }
    DRAWCROSS(low_high.x,low_high.y,0,255,0);
    //calculate search-values for low_high edge
    //angle of the sobel
    //TODO: Maybe do some error checking on the sobelAngle?
    //TODO we should reduce the search-length
    std::cout <<"sobel angle! tangent: "<<low_high.sobelTangent() << " normal: " << low_high.sobelNormal() <<std::endl;
    DRAWLINE(low_high.x,low_high.y,low_high.x*10*cos(low_high.sobelNormal()),low_high.y*10*sin(low_high.sobelNormal()),255,255,0);
    DRAWLINE(low_high.x,low_high.y,low_high.x*10*cos(low_high.sobelTangent()),low_high.y*10*sin(low_high.sobelTangent()),0,255,255);
    if(!high_low.find(low_high, searchLength, low_high.sobelNormal(),EdgePoint::EdgeType::HIGH_LOW, sobelThreshold,gaussBuffer DRAWDEBUG_ARG)){
        //high_low edge wasn't found, try to find it in the old searchAngle
        if(!high_low.find(low_high, searchLength, searchAngle,EdgePoint::EdgeType::HIGH_LOW, sobelThreshold,gaussBuffer DRAWDEBUG_ARG)){
            return false;
        }
    }

    DRAWCROSS(high_low.x,high_low.y,0,255,0);
    //found both :)
    //check the length (add min/max Length)
    float _distance = distance();
    if(_distance < minWidth || _distance > maxWidth)
        return false;
    return true;

}

float LinePoint::getAngle(){
    return atan2(low_high.x-high_low.x,low_high.y-high_low.y);
}

float LinePoint::getSlope(){
    float dx = low_high.x-high_low.x;
    float dy = low_high.y-high_low.y;
    return dy/dx;
}

} //namepsace find
} //namespace imaging
} //namespace lms
