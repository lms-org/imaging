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
    bool found= false;
    //end-points for the bresenham-function
    int xMax = startPoint.x+searchLength*cos(searchAngle);
    int yMax = startPoint.y+searchLength*sin(searchAngle);
    std::cout<<"###################################"<<std::endl;
    bresenhamLine(startPoint.x,startPoint.y,xMax,yMax,[this,&found,&startPoint,&gaussBuffer,&searchtype,&threshold DRAWDEBUG_CAPTURE](int _x, int _y){
        //draw debug point
        DRAWPOINT(_x,_y,0,0,255);
        //gauss surrounding
        /*
         * TODO that could be optimized as the next pixel will be inside the gaussed rectangle!
         * That's why we don't need to gauss a rectangle. Gaussing 3 to 5 pixel instead of 9 would ne enough
         */
        op::gaussBox(*startPoint.getImage(),gaussBuffer,_x-1,_y-1,_x+1,_y+1);
        //sobel pxl
        m_sobelX = op::sobelX(_x,_y,gaussBuffer);
        m_sobelY = op::sobelY(_x,_y,gaussBuffer);
        //std::cout <<"x: " << _x <<" sobel: "<< (int)searchtype<< " "  << m_sobelX << " " << m_sobelY <<std::endl;
        //check if gradient of sobel is big enough
        if(pow(m_sobelX,2)+pow(m_sobelY,2) > pow(threshold,2)){
            //found an edge
            //set the type
            std::cout <<"sobel: "<< (int)searchtype<< " "  << m_sobelX << " " << m_sobelY <<" norm: "<<m_sobelX/8 <<std::endl;

            setType();
            if(type() == searchtype){
                //found an edge you were looking for :)
                //calculate the angle
                m_sobelAngle = atan(m_sobelY/m_sobelX);
                this->x = _x;
                this->y = _y;
                found = true;
                //stop the bresenham
                return false;
            }
        }
        //continue searching points!
        return true;
    });
    return found;
    //return false;
}

//setzt high-low oder low-high kante
EdgePoint::EdgeType EdgePoint::setType() {

    //Funktioniert nicht mit rauschen...
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
