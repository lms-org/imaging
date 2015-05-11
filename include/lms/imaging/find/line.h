#ifndef IMAGE_LINE
#define IMAGE_LINE

#include <lms/imaging/find/object.h>
#include <lms/imaging/find/line_point.h>
#include <deque>
#include <lms/imaging/draw_debug.h>
namespace lms{
namespace imaging{
namespace find{

class Line{
public:
    struct LineParam:public LinePoint::LinePointParam{
        LineParam():stepLengthMin(0),stepLengthMax(0),maxLength(INFINITY),approxEdge(false){
        }
        float stepLengthMin;
        float stepLengthMax;
        float maxLength;
        bool approxEdge;
    };
    bool find(LineParam lineParam DRAWDEBUG_PARAM);
    bool find(DRAWDEBUG_PARAM_N);
    bool findPoint(LinePoint &pointToFind,LinePoint::LinePointParam linePointParam DRAWDEBUG_PARAM);

    void setParam(const LineParam &lineParam);
    bool find(Pixel &startPoint, int searchLength, float searchAngle,int minWidth,int maxWidth, int sobelThreshold,Image &gaussBuffer DRAWDEBUG);
    //bool findPoint(LinePoint &pointToFind,Pixel &startPoint, int searchLength, float searchAngle, int minWidth,int maxWidth, int sobelThreshold,Image &gaussBuffer DRAWDEBUG);

protected:
    LineParam m_LineParam;
    std::deque<LinePoint> points;

    bool verify();


    int limitAngle(int dir);
    bool initialSearch(Pixel px, uint16_t length, int16_t phi);
    int extend(LinePoint &start,bool direction DRAWDEBUG);

    int calcDistance(LinePoint &first, LinePoint &next);
    bool checkAngle(LinePoint &start, LinePoint &next);
};

} //namepsace find
} //namespace imaging
} //namespace lms
#endif // IMAGE_LINE
