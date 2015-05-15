#include "lms/imaging/find/line.h"
#include "lms/imaging/find/line_point.h"
#include <lms/imaging/graphics.h>
#include <cmath>
#include <lms/imaging/draw_debug.h>
#include <algorithm>
#include "lms/math/vertex.h"
namespace lms{
namespace imaging{
namespace find{

bool Line::find(LineParam lineParam DRAWDEBUG_PARAM){
    setParam(lineParam);
    return find(DRAWDEBUG_ARG_N);
}

void Line::setParam(const LineParam &lineParam){
    m_LineParam = lineParam;
}
bool Line::find(DRAWDEBUG_PARAM_N){
    LinePoint lp;
    LinePoint::LinePointParam lParam = m_LineParam;
    bool found = false;
    if(m_LineParam.preferVerify){
        if(m_LineParam.verify){
            found = verifyPoint(lp,lParam DRAWDEBUG_ARG);
        }
        std::cout << "preferVerify: found "<<found << std::endl;
        if(!found){
            found = findPoint(lp,lParam DRAWDEBUG_ARG);
        }
    }else{
        found = findPoint(lp,lParam DRAWDEBUG_ARG);
        if(!found && m_LineParam.verify){
            found = verifyPoint(lp,lParam DRAWDEBUG_ARG);
        }
    }
    m_points.clear();

    if(!found){
        return false;
    }
    m_points.push_front(lp);
    //found receptor point -> try to extend the line
    extend(true DRAWDEBUG_ARG);
    extend(false DRAWDEBUG_ARG);
    return true;
}

bool Line::verifyPoint(LinePoint &lp, LinePoint::LinePointParam lParam DRAWDEBUG_PARAM){
    bool found = false;
    if(m_LineParam.verify && m_points.size() > 0){
        //get the ending that is close to the search-point
        int start = m_points.size()/3;
        if(pow(m_points[0].param().x-m_LineParam.x,2) +pow(m_points[0].param().y-m_LineParam.y,2)<pow(m_points[m_points.size()-1].param().x-m_LineParam.x,2) +pow(m_points[m_points.size()-1].param().y-m_LineParam.y,2)){
            for(uint i = start; i < m_points.size(); i++){
                LinePoint &old = m_points[i];
                lParam.x = old.param().x;
                lParam.y = old.param().y;
                lParam.searchAngle = old.getAngle();
                //std::cout << "try to find: " << lParam.x << " "<<lParam.y << " "<< lParam.searchAngle<<std::endl;
                if(findPoint(lp,lParam DRAWDEBUG_ARG)){
                    found = true;
                    break;
                }
            }
        }else{
            for(int i = m_points.size()-1-start; i >= 0; i--){
                LinePoint &old = m_points[i];
                lParam.x = old.param().x;
                lParam.y = old.param().y;
                lParam.searchAngle = old.getAngle();
                //std::cout << "try to find: " << lParam.x << " "<<lParam.y << " "<< lParam.searchAngle<<std::endl;

                if(findPoint(lp,lParam DRAWDEBUG_ARG)){
                    found = true;
                    break;
                }
            }
        }
    }
    return found;
}


bool Line::findPoint(LinePoint &pointToFind,LinePoint::LinePointParam linePointParam DRAWDEBUG_PARAM){
    //find first point
    //Draw red cross
    DRAWCROSS(linePointParam.x,linePointParam.y,255,0,0);
    bool found =  pointToFind.find(linePointParam DRAWDEBUG_ARG);
    if(found && m_LineParam.validPoint){
        found = m_LineParam.validPoint(pointToFind);
        //TODO
        //float diffAngle = m_LineParam.searchAngle-;
    }
    return found;
}
/**
  * TODO es kommt auf die suchrichtung an, was direction==true oder false tut, ab dem winkel > 90 dreht sich die suchrichtung im Bild um. Veranschaulicht wird das durch einen Kreis wobei man in den beiden oberen Quadranten sucht.
  */
void Line::extend(bool direction DRAWDEBUG){
    lms::math::vertex2i pixel;

    float searchStepX;
    float searchStepY;
    float searchTangentAngle;
    float currentLength = 0;
    float currentStepLength = m_LineParam.stepLengthMax;

    bool found = false;
    //search as long as the searchLength isn't reached
    while(currentLength < m_LineParam.maxLength){
        found = false;
        //Create new point using the last data
        LinePoint searchPoint;
        if(direction){
            searchPoint = m_points[m_points.size()-1];
        }else{
            searchPoint = m_points[0];
        }

        //get needed stuff
        //float lineWidth = searchPoint.distance();

        pixel.x() = searchPoint.low_high.x();
        pixel.y() = searchPoint.low_high.y();
        float searchNormalAngle = m_LineParam.searchAngle;
        if(m_points.size() > 1){
            //get angle between last two points
            EdgePoint *top;
            EdgePoint *bot;
            if(direction){
                top = &m_points[m_points.size()-1].low_high;
                bot = &m_points[m_points.size()-2].low_high;
            }else{
                top = &m_points[1].low_high;
                bot = &m_points[0].low_high;
            }
            searchNormalAngle = atan2(top->y() - bot->y(),top->x()-bot->x());
            //TODO "-" only works because the low_high edge is on the left!
            searchNormalAngle -= M_PI_2l;
        }

        if(direction){
            searchTangentAngle = searchNormalAngle+M_PI_2l;
        }else{
            searchTangentAngle = searchNormalAngle-M_PI_2l;
        }
        //move the point along the tangent of the line and afterwards move it from the line so the point isn't already on the line
        searchStepX = cos(searchTangentAngle)*currentStepLength-m_LineParam.lineWidthTransMultiplier*m_LineParam.lineWidthMax*cos(searchNormalAngle);
        searchStepY = sin(searchTangentAngle)*currentStepLength-m_LineParam.lineWidthTransMultiplier*m_LineParam.lineWidthMax*sin(searchNormalAngle);

        //try to find a new point
        //calculate new searchPoint
        if(m_LineParam.target->inside(pixel.x()+searchStepX,pixel.y()+searchStepY)){
            //move pixel
            pixel += lms::math::vertex2i(searchStepX,searchStepY);
            //TODO that could be made more efficient
            LinePoint::LinePointParam param = m_LineParam;
            param.x = pixel.x();
            param.y = pixel.y();
            param.searchLength = m_LineParam.lineWidthMax*3;
            param.searchAngle = searchNormalAngle;


            if(findPoint(searchPoint,param DRAWDEBUG_ARG)){
                found = true;
                if(direction){
                    currentLength += searchPoint.low_high.distance(m_points[m_points.size()-1].low_high);
                    m_points.push_back(searchPoint);
                }else{
                    currentLength += searchPoint.low_high.distance(m_points[0].low_high);
                    m_points.push_front(searchPoint);
                }

            }
        }
        if(!found){
            //found no point, decrease length
            //TODO add some better algo.
            currentStepLength *= 0.5;

            if(currentStepLength < m_LineParam.stepLengthMin){
                //stop searching, no more points can be found on this line
                //TODO return
                break;
            }
        }
    }
}

const std::deque<LinePoint>& Line::points() const {
    return m_points;
}

} //namepsace find
} //namespace imaging
} //namespace lms
