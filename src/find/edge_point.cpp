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
    bresenhamLine(startPoint.x,startPoint.y,xMax,yMax,[this,&found,&searchAngle,&startPoint,&gaussBuffer,&searchtype,&threshold DRAWDEBUG_CAPTURE](int _x, int _y){
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
         //check if gradient of sobel is big enough
        if(pow(m_sobelX,2)+pow(m_sobelY,2) > pow(threshold,2)){
            //found an edge
            //set the type
            setType();
            if(type() == searchtype){
                //found an edge you were looking for :)
                //calculate the angle

                m_sobelTangent = atan2(m_sobelY,m_sobelX);
                /*
                if(m_sobelX < 0 && m_sobelY < 0){
                    m_sobelTangent -= M_PI_2l;
                }else if(m_sobelX < 0 &&m_sobelY > 0){
                    m_sobelTangent -= M_PI_2l;
                }
                */
                m_sobelNormal = m_sobelTangent;
                //TODO doesnt care about rotation-count of the searchAngle % PI
                if(-M_PI_2l <searchAngle < M_PI_2l){
                    m_sobelNormal+=M_PI_2l;
                }else{
                    m_sobelNormal-=M_PI_2l;
                }
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


float EdgePoint::sobelTangent(){
    return m_sobelTangent;
}

float EdgePoint::sobelNormal(){
    return m_sobelNormal;
}

EdgePoint::EdgeType EdgePoint::type(){
    return m_type;
}
} //namepsace find
} //namespace imaging
} //namespace lms
