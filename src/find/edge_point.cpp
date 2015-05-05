#include <lms/imaging/find/edge_point.h>
//#include "image/image_factory.h"
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <lms/imaging/image_factory.h>

#include <lms/imaging/draw_debug.h>
#include <iostream>

namespace lms{
namespace imaging{
namespace find{
bool EdgePoint::find(Pixel &startPoint, int searchLength, float searchAngle,
                     EdgeType searchtype, int threshold,Image &gaussBuffer DRAWDEBUG){
    //pointer so it can be set in the bresenhamLine-function
    //bool *found= false;
    //end-points for the bresenham-function
    int xMax = startPoint.x+searchLength*cos(searchAngle);
    int yMax = startPoint.y+searchLength*sin(searchAngle);
    bresenhamLine(startPoint.x,startPoint.y,xMax,yMax,[this,/*&found,*/&startPoint,&gaussBuffer,&searchtype,&threshold DRAWDEBUG_CAPTURE](int x, int y){

        //std::cout << "BRES: X AND Y SEARCH "  << x << " " << y <<std::endl;
        //draw debug point
        DRAWPOINT(x,y,0,0,255);
        //gauss surrounding
        /*
         * TODO that could be optimized as the next pixel will be inside the gaussed rectangle!
         * That's why we don't need to gauss a rectangle. Gaussing 3 to 5 pixel instead of 9 would ne enough
         */
        op::gaussBox(*startPoint.getImage(),gaussBuffer,x-1,y-1,x+1,y+1);
        //sobel pxl
        m_sobelX = op::sobelX(x,y,gaussBuffer);
        m_sobelY = op::sobelY(x,y,gaussBuffer);

        //check if gradient of sobel is big enough
        if(pow(m_sobelX,2)+pow(m_sobelY,2) > pow(threshold,2)){
            //found an edge
            //*found = true;
            //set the type
            setType();
            if(type() == searchtype){
                //found an edge you were looking for :)
                //calculate the angle
                m_sobelAngle = atan(m_sobelY/m_sobelX);
                //stop the bresenham
                return false;
            }
        }
        //continue searching points!
        return true;
    });
    //return *found;
    return false;
}

//setzt high-low oder low-high kante
EdgePoint::EdgeType EdgePoint::setType() {
    if(abs(sobelX()) > abs(sobelY())){
        if(sobelX() > 0){
            m_type = EdgeType::LOW_HIGH;
        }else{
            m_type = EdgeType::HIGH_LOW;
        }
    }else if(sobelX() == 0 && sobelY() == 0){
        m_type == EdgeType::PLANE;
    }else{
        if(sobelY() > 0){
            m_type = EdgeType::LOW_HIGH;
        }else{
            m_type = EdgeType::HIGH_LOW;
        }
    }
    return m_type;
}


int EdgePoint::sobelX(){
    return m_sobelX;
}

int EdgePoint::sobelY(){
    m_sobelY;
}

float EdgePoint::sobelAngle(){
    return m_sobelAngle;
}

EdgePoint::EdgeType EdgePoint::type(){
    return m_type;
}
} //namepsace find
} //namespace imaging
} //namespace lms
