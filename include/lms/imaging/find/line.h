#ifndef IMAGE_LINE
#define IMAGE_LINE

#include <deque>
#include <iterator>

#include "image/image_object.h"
#include "image/image_line_point.h"


class Line : public ImageObject {
public:
    bool find(Pixel px, uint16_t length, int16_t phi);
    bool verify();
    int serialize (Data::ImageDataSpace::Image *imageStruct);
    void init() {};
    void deinit(){};
    int getPointSize() { return points.size(); }
    int getMinPointSize() {return minPoints;}
    Line (){};

protected:
    std::deque<LinePoint> points;
    static const size_t minPoints = 2;

    /// PS2
    static const int extendSearchLength = 10;
    // Suchlänge wenn Linie gefunden ist
    static const int initialSearchLength = 30;
    // Suchlänge zum verifizieren eines Punktes
    static const int verifySearchLength = 10;
    // angestrebter Abstand zum nächsten Linepoint
    static const int desiredDistance = 15;
    // Abstand in x- und y-Richtung zum nächsten gesuchten Pixel
    static const int stepsizeX = 15;
    static const int stepsizeY = 15;

//    /// U-Eye
//    static const int extendSearchLength = 20;
//    // Suchlänge wenn Linie gefunden ist
//    static const int initialSearchLength = 50;
//    // Suchlänge zum verifizieren eines Punktes
//    static const int verifySearchLength = 40;
//    // angestrebter Abstand zum nächsten Linepoint
//    static const int desiredDistance = 40;
//    // Abstand in x- und y-Richtung zum nächsten gesuchten Pixel
//    static const int stepsizeX = 20;
//    static const int stepsizeY = 20;


    int limitAngle(int dir);
    bool initialSearch(Pixel px, uint16_t length, int16_t phi);
    int extend(LinePoint &lp, int stepsizeX, int stepsizeY, bool sign, bool changingSearchDir, std::deque<LinePoint>::iterator insertAt, int counter = 100, std::deque<LinePoint>::iterator* outIt = NULL, LinePoint* startPoint = NULL);
    void extendVerifiedLine(int stepsizeX, int stepsizeY);
    void verifyLastPoints();
    void insertbetween(int maxDist, int stepsizeX, int stepsizeY);
    int calcDistance(LinePoint &first, LinePoint &next);
    bool checkAngle(LinePoint &start, LinePoint &next);
};

#endif // IMAGE_LINE
