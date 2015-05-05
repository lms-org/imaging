#ifndef IMAGE_EDGE_POINT

#define IMAGE_EDGE_POINT

#include "lms/imaging/find/pixel.h"
#include <math.h>
#include <lms/imaging/draw_debug.h>

namespace lms{
namespace imaging{
namespace find{
class EdgePoint: public Pixel{

public:
    enum class EdgeType {LOW_HIGH, HIGH_LOW, PLANE};

private:
    int m_sobelX;
    int m_sobelY;
    float m_sobelAngle;
    EdgeType m_type;

    public:
        int sobelX();
        int sobelY();
        /**
         * @brief sobelAngle
         * @return the angle from -PI to PI
         */
        float sobelAngle();
        EdgeType type();
        /**
         * @brief find
         * @param startPoint starting point
         * @param searchLength length of the line used to find an edge
         * @param searchAngle angle in radians
         * @param searchtype
         * @param threshold
         * @return true if the edgePoint was found, if not the contained values like sobelX/Y etc. are just random!
         */
        bool find(Pixel &startPoint, int searchLength, float searchAngle, EdgeType searchtype, int threshold,Image &gaussBuffer DRAWDEBUG);

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
