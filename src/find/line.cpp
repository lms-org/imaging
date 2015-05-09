#include "lms/imaging/find/line.h"
#include "lms/imaging/find/line_point.h"
#include <lms/imaging/graphics.h>
#include <cmath>
#include <lms/imaging/draw_debug.h>
#include <algorithm>
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
    points.clear();
    LinePoint &lp = points[points.size()];
    if(findPoint(lp,m_LineParam DRAWDEBUG_ARG)){
        //didn't found the receptor point!
        return false;
    }

    //found receptor point -> try to extend the line
    extend(lp,true DRAWDEBUG_ARG);
    extend(lp,false DRAWDEBUG_ARG);
    return true;
}


bool Line::findPoint(LinePoint &pointToFind,LinePoint::LinePointParam linePointParam DRAWDEBUG_PARAM){
    //find first point
    //Draw red cross
    DRAWCROSS(linePointParam.x,linePointParam.y,255,0,0);
    if(!pointToFind.find(linePointParam DRAWDEBUG_ARG)){
        //wasn't able to find a start-point :(
        return false;
    }
    return true;
}

int Line::extend(LinePoint &start,bool direction DRAWDEBUG){
    Pixel pixel;
    pixel.setImage(start.low_high.getImage());

    std::vector<LinePoint> points;
    points.push_back(start);

    float searchStepX;
    float searchStepY;
    float searchTangentAngle;
    float currentLength = 0;
    float stepLengthMax=10;
    float stepLengthMin = 5;
    float currentStepLength = 0;

    //search as long as the searchLength isn't reached
    while(currentLength < m_LineParam.maxLength){
        //Create new point using the last data
        LinePoint searchPoint = points[points.size()-1];
        //get needed stuff
        float lineWidth = searchPoint.distance();

        pixel.x = searchPoint.low_high.x;
        pixel.y = searchPoint.low_high.y;
        float searchNormalAngle = searchPoint.getAngle();

        if(direction){
            searchTangentAngle = searchNormalAngle-M_PI_2l;
        }else{
            searchTangentAngle = searchNormalAngle+M_PI_2l;
        }
        //move the point along the tangent of the line and afterwards move it from the line so the point isn't already on the line
        searchStepX = cos(searchTangentAngle)*stepLengthMax-3*lineWidth*cos(searchNormalAngle);
        searchStepY = sin(searchTangentAngle)*stepLengthMax-3*lineWidth*sin(searchNormalAngle);
        /*
        std::cout<< "posX: " << searchPoint.low_high.x << " " << searchPoint.high_low.x << std::endl;
        std::cout<< "posY: " << searchPoint.low_high.y << " " << searchPoint.high_low.y << std::endl;
        std::cout <<"normal : " << searchNormalAngle <<std::endl;
        std::cout << "deltaX " << cos(searchTangentAngle)*stepLengthMax << " " <<lineWidth*cos(searchNormalAngle) << std::endl;
        std::cout << "deltaY " << sin(searchTangentAngle)*stepLengthMax << " " <<lineWidth*sin(searchNormalAngle) << std::endl;
        */

        //try to find a new point
        //calculate new searchPoint
        if(!pixel.move(searchStepX,searchStepY)){
            //out of the image!
            return 0;
        }
        //TODO that could be made more efficient
        LinePoint::LinePointParam param = m_LineParam;
        param.x = pixel.x;
        param.y = pixel.y;
        param.searchLength = m_LineParam.lineWidthMax*5;
        param.searchAngle = searchNormalAngle;

        if(findPoint(searchPoint,param DRAWDEBUG_ARG)){
            //TODO
            currentLength += searchPoint.low_high.distance(points[points.size()-1].low_high);
            points.push_back(searchPoint);

        }else{
            //found no point, decrease length
            //TODO add some better algo.
            currentStepLength *= 0.5;

            if(currentStepLength < stepLengthMin){
                //stop searching, no more points can be found on this line
                //TODO return
                return 0;
            }
        }
    }
return 0;
}


//TODO remove
int Line::calcDistance(LinePoint &first, LinePoint &next) {

    int dx = first.low_high.x - next.low_high.x;
    int dy = first.low_high.y - next.low_high.y;

    return sqrt(dx*dx + dy*dy);

}
//TODO change to radians form PI to -PI
int Line::limitAngle(int dir) {
    while (dir > 360)
        dir = dir - 360;
    while (dir < 0)
        dir = 360 + dir;

    return dir;
}

bool Line::checkAngle(LinePoint &start, LinePoint &next) {

    float angle_start = 0.0;
    float angle_next = 0.0;

    float dx_start = abs(start.low_high.x - start.high_low.x);
    float dy_start = abs(start.low_high.y - start.high_low.y);

    float dx_next = abs(next.low_high.x - next.high_low.x);
    float dy_next = abs(next.low_high.y - next.high_low.y);

    if(dx_start == 0)
        angle_start = M_PI;
    if(dx_next == 0)
        angle_next = M_PI;

    angle_start = atan2(dy_start,dx_start);
    angle_next = atan2(dy_next, dx_next);

    float dangle = fabs(angle_next - angle_start);
    float dgradient = 0;//fabs(start.up.non_disc_direction - next.up.non_disc_direction);

    if(dangle > (70.0/180.0)*M_PI || dgradient > 60) {
        //printf("%f | %f \n", (dangle/M_PI)*180.0, dgradient);
        return false;
    }
    else
        return true;

}

} //namepsace find
} //namespace imaging
} //namespace lms
