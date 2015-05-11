#include "lms/imaging/find/line_point.h"
#include "lms/imaging/find/edge_point.h"
//#include "image/image_tools.h"
#include <lms/imaging/draw_debug.h>
#include <cmath>

namespace lms{
namespace imaging{
namespace find{



void LinePoint::setParam(const LinePointParam &param){
    m_LinePointParam = param;
}

bool LinePoint::find(const LinePointParam &param DRAWDEBUG_PARAM){
    setParam(param);
    return find(DRAWDEBUG_ARG_N);
}

bool LinePoint::find(DRAWDEBUG_PARAM_N){
    //try to find first point, if it fails return as no LinePoint can be found
    EdgePoint::EdgePointParam param = m_LinePointParam;
    //TODO those two methods are quite bad
    low_high.setImage(const_cast<lms::imaging::Image*>(param.target));
    high_low.setImage(const_cast<lms::imaging::Image*>(param.target));
    //the first searchPoint is already set in the params, just need to set the EdgeType
    m_LinePointParam.searchType = EdgePoint::EdgeType::LOW_HIGH;
    //try to find the first edge
    if(!low_high.find(m_LinePointParam DRAWDEBUG_ARG)){
        //no edge found :(
        return false;
    }

    //draw the found point
    DRAWCROSS(low_high.x,low_high.y,0,255,0);
    //check if the user is only looking for one edge
    if(m_LinePointParam.edge){
        //the low_high edge was found :)
        return true;
    }

    //set new values for second edge
    param.searchType = EdgePoint::EdgeType::HIGH_LOW;
    param.x = low_high.x;
    param.y = low_high.y;
    //TODO: Maybe do some error checking on the sobelAngle?
    param.searchAngle = low_high.sobelNormal();
    //TODO we could reduce the search-length
    //param.searchLength = ?

    //draw the tangent/normal of the sobel
    //DRAWLINE(low_high.x,low_high.y,low_high.x*10*cos(low_high.sobelNormal()),low_high.y*10*sin(low_high.sobelNormal()),255,255,0);
    //DRAWLINE(low_high.x,low_high.y,low_high.x*10*cos(low_high.sobelTangent()),low_high.y*10*sin(low_high.sobelTangent()),0,255,255);

    //TODO: Don't know why that doesn't work well! Sobel values are quite bad!
    bool found  = false;
    if(m_LinePointParam.useSobel){
        if(!high_low.find(param DRAWDEBUG_ARG)){
            found = true;
        }
    }
    if(!found){
        //high_low edge wasn't found, try to find it in the old searchAngle
        param.searchAngle = m_LinePointParam.searchAngle;
        if(!high_low.find(param DRAWDEBUG_ARG)){
            return false;
        }
    }
    //found both low->high and high->low edge!
    DRAWCROSS(high_low.x,high_low.y,255,255,0);

    //check the width of the linePoint is valid
    float _distance = distance();
    if(_distance < m_LinePointParam.lineWidthMin || _distance > m_LinePointParam.lineWidthMax){
        //width is not valid :(
        return false;
    }
    return true;
}

float LinePoint::distance(){
    return low_high.distance(high_low);
}

float LinePoint::getAngle(){
    return atan2(high_low.y-low_high.y,high_low.x-low_high.x);
}

float LinePoint::getSlope(){
    float dx = low_high.x-high_low.x;
    float dy = low_high.y-high_low.y;
    return dy/dx;
}

} //namepsace find
} //namespace imaging
} //namespace lms
