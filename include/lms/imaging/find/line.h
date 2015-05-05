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
    bool find(Pixel &startPoint, int searchLength, float searchAngle,int minWidth,int maxWidth, int sobelThreshold,Image &gaussBuffer DRAWDEBUG);

protected:
    //TODO use a doubly linked list!
    std::vector<LinePoint> points;
    //std::deque<LinePoint> points;

    //Das kommt alles weh
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

    bool verify();


    int limitAngle(int dir);
    bool initialSearch(Pixel px, uint16_t length, int16_t phi);
    int extend(LinePoint &start, bool direction,int stepLengthMin, int stepLengthMax,float lineWidth, float lineLength,Image &gaussBuffer DRAWDEBUG);
    void extendVerifiedLine(int stepsizeX, int stepsizeY);
    void verifyLastPoints();
    void insertbetween(int maxDist, int stepsizeX, int stepsizeY);
    int calcDistance(LinePoint &first, LinePoint &next);
    bool checkAngle(LinePoint &start, LinePoint &next);
};

} //namepsace find
} //namespace imaging
} //namespace lms
#endif // IMAGE_LINE
