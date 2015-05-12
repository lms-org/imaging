#include "lms/imaging/graphics.h"
#include "lms/imaging/image_factory.h"

namespace lms {
namespace imaging {

ARGBColor::ARGBColor(std::uint8_t alpha, std::uint8_t red, std::uint8_t green,
                     std::uint8_t blue) :
    value(alpha << 24 | red << 16 | green << 8  | blue) {}
ARGBColor::ARGBColor(std::uint8_t red, std::uint8_t green, std::uint8_t blue) :
    value(0xff << 24 | red << 16 | green << 8  | blue) {}
ARGBColor::ARGBColor(std::uint32_t value) : value(value) {}
ARGBColor::ARGBColor() : value(0x00000000) {}

/**
 * Helper function that is used to draw horizontal and vertical
 * lines.
 */
template<int dx, int dy>
inline void drawDeltaLine(Graphics *g, int x, int y, int length) {
    while(length -- > 0) {
        g->drawPixel(x, y);
        x += dx;
        y += dy;
    }
}

void Graphics::drawHorizontalLine(int x, int y, int length) {
    if(length >= 0) {
        drawDeltaLine<1, 0>(this, x, y, length);
    } else {
        // support negative lengths as well
        drawDeltaLine<-1, 0>(this, x, y, -length);
    }
}

void Graphics::drawVerticalLine(int x, int y, int length) {
    if(length >= 0) {
        drawDeltaLine<0, 1>(this, x, y, length);
    } else {
        // support negative lengths as well
        drawDeltaLine<0, -1>(this, x, y, -length);
    }
}

void Graphics::drawLine(int x1, int y1, int x2, int y2) {
    bresenhamLine(x1, y1, x2, y2, [this](int x, int y) {
        drawPixel(x, y);
        return true;
    });
}

void Graphics::drawRect(int x, int y, int width, int height) {
    drawHorizontalLine(x, y, width);
    drawHorizontalLine(x, y + height, width);
    drawVerticalLine(x, y, height);
    drawVerticalLine(x + width, y, height);
}

void Graphics::drawPolyline(int *x, int *y, int n) {
    for(int i = 0; i < n - 1; i++) {
        drawLine(x[i], y[i], x[i+1], y[i+1]);
    }
}

void Graphics::drawCross(int x,int y){
    drawPixel(x-1,y);
    drawPixel(x,y);
    drawPixel(x+1,y);
    drawPixel(x,y-1);
    drawPixel(x,y+1);
}

BGRAImageGraphics::BGRAImageGraphics(Image &image) : image(image) {}

void BGRAImageGraphics::setColor(ARGBColor color) {
    this->color = color;
}

void BGRAImageGraphics::drawPixel(int x, int y) {
    if(x >= 0 && y >= 0 && x < image.width() && y < image.height()) {
        std::uint32_t *data = reinterpret_cast<std::uint32_t*>(image.data());
        *(data + x + y * image.width()) = color.value;
    }
}

void BGRAImageGraphics::fillWhite() {
    image.fill(255);
}

void BGRAImageGraphics::fillTransparent() {
    image.fill(0);
}

}  // namespace imaging
}  // namespace lms
