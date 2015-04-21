#ifndef IMAGE_LINE_POINT
#define IMAGE_LINE_POINT

#include "image/image_edge_point.h"
#include "image/image_pixel.h"
#include "image/image_factory.h"
#include "image/image_edge_point.h"
#include "image/image_tools.h"

class LinePoint {

public:
    EdgePoint up, down;
    bool verified;

    bool find(Pixel px, int length, int16_t dir);

    bool operator <(const LinePoint &rhs) const {
        return up.pixel.y < rhs.up.pixel.y || (up.pixel.y == rhs.up.pixel.y && up.pixel.x < rhs.up.pixel.x);
    }
protected:

    /// PS 2
    static const int downSearchLength = 20;
    static const int maxDiffAngle = 20;
    static const int minSquareLength = 6;
    static const int linepointThreshold = 230;

//    /// U-Eye
//    static const int downSearchLength = 20;
//    static const int maxDiffAngle = 20;
//    static const int minSquareLength = 4;
//    static const int linepointThreshold = 254;

    // Berechnung der Suchrichtung der low_high EdgePoint Suche
    // Ziel: Unabhängigkeit von der Suchrichtung (links->rechts; rechts->links)
    int secondSearchDir(int gradientDir);
    bool checkAngle();
    bool checkLength();

};

#endif // IMAGE_LINE_POINT
