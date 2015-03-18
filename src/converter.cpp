#include "lms/imaging/converter.h"

namespace lms {
namespace imaging {

bool convert(const Image &input, Image &output, Format targetFormat) {
    if(input.format() == Format::YUYV && targetFormat == Format::GREY) {
        convertYUYVtoGREY(input, output);
        return true;
    }
    if(input.format() == Format::YUYV && targetFormat == Format::BGRA) {
        convertYUYVtoBGRA(input, output);
        return true;
    }
    if(input.format() == Format::GREY && targetFormat == Format::BGRA) {
        convertGREYtoBGRA(input, output);
        return true;
    }

    return false;
}

void convertYUYVtoGREY(const Image &input, Image &output) {
    output.resize(input.width(), input.height(), Format::GREY);

    // Only for sizes dividable by 4
    size_t i = output.size() / 4;
    const std::uint8_t* src = input.data();
    std::uint8_t* dst = output.data();
    while( i-- ) {
        *dst++ = *src; src += 2;
        *dst++ = *src; src += 2;
        *dst++ = *src; src += 2;
        *dst++ = *src; src += 2;
    }

    // do the rest
    i = output.size() % 4;
    while( i-- ) {
        *dst++ = *src; src += 2;
    }
}

void yuv2rgb(int y, int u, int v, std::uint8_t *r, std::uint8_t *g, std::uint8_t *b) {
    int r1, g1, b1;
    int c = y - 16, d = u - 128, e = v - 128;

    r1 = (298 * c           + 409 * e + 128) >> 8;
    g1 = (298 * c - 100 * d - 208 * e + 128) >> 8;
    b1 = (298 * c + 516 * d           + 128) >> 8;

    if(r1 > 255) r1 = 255;
    if(g1 > 255) g1 = 255;
    if(b1 > 255) b1 = 255;
    if(r1 < 0) r1 = 0;
    if(g1 < 0) g1 = 0;
    if(b1 < 0) b1 = 0;

    *r = r1;
    *g = g1;
    *b = b1;
}

void convertYUYVtoBGRA(const Image &input, Image &output) {
    output.resize(input.width(), input.height(), Format::BGRA);

    size_t i = input.size() / 4;
    const std::uint8_t *src = input.data();
    std::uint8_t *dst = output.data();

    int y, u, y2, v;

    while(i--) {
        y = *src++;
        u = *src++;
        y2 = *src++;
        v = *src++;

        //               R        G        B
        yuv2rgb(y, u, v, dst + 2, dst + 1, dst);
        dst[3] = 255;  // Alpha

        //                R        G        B
        yuv2rgb(y2, u, v, dst + 6, dst + 5, dst + 4);
        dst[7] = 255;  // Alpha

        dst += 8;  // Two pixels with each 4 bytes
    }
}

void convertGREYtoBGRA(const Image &input, Image &output) {
    output.resize(input.width(), input.height(), Format::BGRA);

    size_t i = input.size();
    const std::uint8_t *src = input.data();
    std::uint8_t *dst = output.data();

    while(i--) {
        *dst++ = *src;  // B
        *dst++ = *src;  // G
        *dst++ = *src;  // R
        *dst++ = 255;   // A

        src++;
    }
}

}  // namespace imaging
}  // namespace lms
