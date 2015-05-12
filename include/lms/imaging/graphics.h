#ifndef LMS_IMAGING_GRAPHICS_H
#define LMS_IMAGING_GRAPHICS_H

#include <vector>
#include <cstdint>

#include "lms/imaging/image.h"

namespace lms {
namespace imaging {

struct ARGBColor {
    ARGBColor(std::uint8_t alpha, std::uint8_t red, std::uint8_t green, std::uint8_t blue);
    ARGBColor(std::uint8_t red, std::uint8_t green, std::uint8_t blue);
    ARGBColor(std::uint32_t value);
    ARGBColor();

    std::uint32_t value;
};

const ARGBColor black(0, 0, 0);
const ARGBColor white(255, 255, 255);
const ARGBColor red(255, 0, 0);
const ARGBColor green(0, 255, 0);
const ARGBColor blue(0, 0, 255);
const ARGBColor yellow(255, 255, 0);

class Graphics {
public:
    /**
     * @brief Set the color that is used for subsequent calls to draw* methods.
     * @param color RGB color to use (red, green, blue)
     */
    virtual void setColor(ARGBColor color) =0;

    /**
     * @brief Fill the whole image with opaque white.
     */
    virtual void fillWhite() =0;

    /**
     * @brief Fill the whole image with a transparent color.
     */
    virtual void fillTransparent() =0;

    /**
     * @brief Draw a single pixel at (x, y).
     * @param x coordinate x
     * @param y coordinate y
     */
    virtual void drawPixel(int x, int y) =0;

    /**
     * @brief Draw a horizontal line from (x, y) to (x+length, y).
     *
     * This function is similar to drawLine() but is usually considered to
     * be faster.
     *
     * Negative length values work as expected. A length of zero results in a
     * single drawPixel(x, y) call.
     *
     * @param x coordinate x
     * @param y coordinate y
     * @param length length of the line
     */
    virtual void drawHorizontalLine(int x, int y, int length);

    /**
     * @brief Draw a vertical line from (x, y) to (x, y+length).
     *
     * This function is similar to drawLine() but is usually considered to
     * be faster.
     *
     * Negative length values work as expected. A length of zero results in a
     * single drawPixel(x, y) call.
     *
     * @param x coordinate x
     * @param y coordinate y
     * @param length length of the line
     */
    virtual void drawVerticalLine(int x, int y, int length);

    /**
     * @brief Draw a line from (x1, y1) to (x2, y2).
     * @param x1 coordinate x of point 1
     * @param y1 coordinate y of point 1
     * @param x2 coordinate x of point 2
     * @param y2 coordinate y of point 2
     */
    virtual void drawLine(int x1, int y1, int x2, int y2);

    virtual void drawRect(int x, int y, int width, int height);

    virtual void drawPolyline(int *x, int *y, int n);

    virtual void drawCross(int x, int y);
    virtual void drawCross(int x, int y, int length);
};

class BGRAImageGraphics : public Graphics {
public:
    /**
     * @brief Create a graphics object that is drawing into an image with BGRA
     * image format.
     *
     * @param image image to draw in
     */
    BGRAImageGraphics(Image &image);

    void setColor(ARGBColor color) override;
    void drawPixel(int x, int y) override;

    virtual void fillWhite() override;
    virtual void fillTransparent() override;
private:
    ARGBColor color;
    Image &image;
};

}  // namespace imaging
}  // namespace lms

#endif /* LMS_IMAGING_GRAPHICS_H */
