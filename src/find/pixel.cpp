#include <lms/imaging/find/pixel.h>
#include <lms/imaging/image_factory.h>
#include <math.h>

namespace lms{
namespace imaging{
namespace find{
bool Pixel::insideImage() {
    if(x >= getImage()->width())
        return false;
    if(y >= getImage()->height())
        return false;
    return true;

}

bool Pixel::move(int dx, int dy) {
    if(getImage()->inside(x + dx, y +dy)){
        x += dx;
        y += dy;
        return true;
    }
    return false;
}


Image* Pixel::getImage(){
    return image;
}

void Pixel::setImage(Image *image){
    this->image = image;
}

float Pixel::distance(Pixel &pixel){
    return sqrt(pow(x-pixel.x,2)+pow(y-pixel.y,2));
}
} //namepsace find
} //namespace imaging
} //namespace lms
