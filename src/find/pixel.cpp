#include <lms/imaging/find/pixel.h>
#include <lms/imaging/image_factory.h>
#include <math.h>

namespace lms{
namespace imaging{
namespace find{

bool Pixel::move(int dx, int dy, const lms::imaging::Image &image) {
    if(image.inside(x + dx, y +dy)){
        x += dx;
        y += dy;
        return true;
    }
    return false;
}

float Pixel::distance(Pixel &pixel){
    return sqrt(pow(x-pixel.x,2)+pow(y-pixel.y,2));
}
} //namepsace find
} //namespace imaging
} //namespace lms
