#ifndef IMAGE_LINE
#define IMAGE_LINE

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

    typedef LineParam parameterType;

    bool find(LineParam lineParam DRAWDEBUG_PARAM);
    bool find(DRAWDEBUG_PARAM_N);
    bool findPoint(LinePoint &pointToFind,LinePoint::LinePointParam linePointParam DRAWDEBUG_PARAM);

    void setParam(const LineParam &lineParam);

    const std::deque<LinePoint> points() const;
protected:
    LineParam m_LineParam;
    std::deque<LinePoint> m_points;

    bool initialSearch(Pixel px, uint16_t length, int16_t phi);
    void extend(LinePoint &start,bool direction DRAWDEBUG);
};

} //namepsace find
} //namespace imaging
} //namespace lms
#endif // IMAGE_LINE
