#ifndef IMAGE_PIXEL
#define IMAGE_PIXEL

#include <stdint.h>
#include <lms/imaging/image.h>
namespace lms{
namespace imaging{
namespace find{

class Pixel {

public:
    uint16_t x;
    uint16_t y;

    Pixel() : x(0), y(0) {}
    Pixel(uint16_t _x, uint16_t _y) : x(_x), y(_y) {}
    bool insideImage();
    bool move(int dx, int dy);

    Image* getImage();
    void setImage(Image *image);
    /**
     * @brief distance
     * @param pixel
     * @return the distance between two pixels
     */
    float distance(Pixel &pixel);
private:
    /**
     * @brief image the image containing the pixel
     */
    Image *image;
};

} //namepsace find
} //namespace imaging
} //namespace lms
#endif // IMAGE_PIXEL
