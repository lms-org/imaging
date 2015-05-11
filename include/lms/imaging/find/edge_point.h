#ifndef IMAGE_EDGE_POINT

#define IMAGE_EDGE_POINT

#include "lms/imaging/find/pixel.h"
#include <math.h>
#include <lms/imaging/draw_debug.h>
#include "lms/deprecated.h"

namespace lms{
namespace imaging{
namespace find{
class EdgePoint: public Pixel{

public:
    enum class EdgeType {LOW_HIGH, HIGH_LOW, PLANE};

    struct EdgePointParam{
        EdgePointParam():x(0),y(0),target(nullptr),searchLength(0),searchAngle(0),searchType(EdgeType::PLANE),sobelThreshold(0),gaussBuffer(nullptr){
        }

        int x;
        int y;
        const Image *target;
        float searchLength;
        float searchAngle;
        EdgeType searchType;
        int sobelThreshold;
        Image *gaussBuffer;
    };

private:
    EdgePointParam m_searchParam;
    int m_sobelX;
    int m_sobelY;
    float m_sobelNormal;
    float m_sobelTangent;
    EdgeType m_type;

    public:
        void setSearchParam(const EdgePointParam &searchParam);
        bool find(DRAWDEBUG_PARAM_N);
        bool find(const EdgePointParam &searchParam DRAWDEBUG_PARAM);
        int sobelX();
        int sobelY();
        /**
         * @brief sobelAngle
         * @return the angle from -PI to PI
         */
        float sobelTangent();
        float sobelNormal();
        EdgeType type();


protected:
        bool confirmEdgePoint(int threshold);
        void nonMaximumSupression(int rowShift, int colShift);
        /**
         * @brief setType "calculates" the type
         * @return the found type
         */
        EdgePoint::EdgeType setType();

};

} //namepsace find
} //namespace imaging
} //namespace lms

#endif // IMAGE_EDGE_POINT
