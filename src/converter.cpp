#include "lms/imaging/converter.h"
#include <cstring>

namespace lms {
namespace imaging {

bool convert(const Image &input, Image &output, Format targetFormat) {
    output.resize(input.width(), input.height(), targetFormat);

    return convertRaw(input.format(), input.data(), input.size(),
               targetFormat, output.data());
}

bool convertRaw(Format srcFormat, const std::uint8_t *src, int srcSize,
                Format dstFormat, std::uint8_t *dst) {

    if(srcFormat == dstFormat) { // if source format == destination format
        // then just copy the data
        std::copy_n(src, srcSize, dst);
        return true;
    }
    if(srcFormat == Format::YUYV && dstFormat == Format::GREY) {
        convertYUYVtoGREY(src, srcSize, dst);
        return true;
    }
    if(srcFormat == Format::YUYV && dstFormat == Format::BGRA) {
        convertYUYVtoBGRA(src, srcSize, dst);
        return true;
    }
    if(srcFormat == Format::GREY && dstFormat == Format::BGRA) {
        convertGREYtoBGRA(src, srcSize, dst);
        return true;
    }
    if(srcFormat == Format::YUYV && dstFormat == Format::RGB) {
        convertYUYVtoRGB(src, srcSize, dst);
        return true;
    }
    if(srcFormat == Format::GREY && dstFormat == Format::RGB) {
        convertGREYtoRGB(src, srcSize, dst);
        return true;
    }
    if(srcFormat == Format::RGB && dstFormat == Format::BGRA) {
        convertRGBtoBGRA(src, srcSize, dst);
        return true;
    }
    return false;
}

void convertYUYVtoGREY(const std::uint8_t *src, int srcSize, std::uint8_t *dst) {
    // Only for sizes dividable by 8
    int i = srcSize / 8;
    while(i--) {
        *dst++ = *src; src += 2;
        *dst++ = *src; src += 2;
        *dst++ = *src; src += 2;
        *dst++ = *src; src += 2;
    }

    // do the rest
    i = srcSize % 8;
    while(i--) {
        *dst++ = *src; src += 2;
    }
}

/*
 * Helper function for YUYV -> BGRA conversion.
 *
 * Found on:
 * https://social.msdn.microsoft.com/Forums/windowsdesktop/en-US/1071301e-74a2-4de4-be72-81c34604cde9/program-to-translate-yuyv-to-rgbrgb?forum=windowsdirectshowdevelopment
 *
 * The alternatives use slow floating point operations:
 * - http://pastebin.com/mDcwqJV3
 * - http://stackoverflow.com/questions/9098881/convert-from-yuv-to-rgb-in-c-android-ndk
 */
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

void convertYUYVtoBGRA(const std::uint8_t *src, int srcSize, std::uint8_t *dst) {
    int i = srcSize / 4;
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

void convertGREYtoBGRA(const std::uint8_t *src, int srcSize, std::uint8_t *dst) {
    size_t i = srcSize;
    std::uint8_t srcVal;

    while(i--) {
        srcVal = *src++;
        *dst++ = srcVal;  // B
        *dst++ = srcVal;  // G
        *dst++ = srcVal;  // R
        *dst++ = 255;   // A
    }
}

void convertYUYVtoRGB(const std::uint8_t *src, int srcSize, std::uint8_t *dst) {
    int i = srcSize / 4;
    int y, u, y2, v;

    while(i--) {
        y = *src++;
        u = *src++;
        y2 = *src++;
        v = *src++;

        //               R    G        B
        yuv2rgb(y, u, v, dst, dst + 1, dst + 2);

        //                R        G        B
        yuv2rgb(y2, u, v, dst + 3, dst + 4, dst + 5);

        dst += 6;  // Two pixels with each 3 bytes
    }
}

void convertGREYtoRGB(const std::uint8_t *src, int srcSize, std::uint8_t *dst) {
    size_t i = srcSize;
    std::uint8_t srcVal;

    while(i--) {
        srcVal = *src++;
        *dst++ = srcVal;  // R
        *dst++ = srcVal;  // G
        *dst++ = srcVal;  // B
    }
}

void convertRGBtoBGRA(const std::uint8_t *src, int srcSize, std::uint8_t *dst) {
    size_t i = srcSize / 3;

    while(i--) {
        *dst++ = src[2];  // B
        *dst++ = src[1];  // G
        *dst++ = src[0];  // R
        *dst++ = 255;     // A
        src += 3;
    }
}

void scaleDown(const Image &input, Image &output, int factor) {
    if (factor < 1) {
        return;
    } else if (factor == 1) {
        output = input;
    }
    int bpp = bytesPerPixel(input.format());
    int new_width = input.width() / factor;
    int new_height = input.height() / factor;

    // resize or (if necessary) create new Image
    output.resize(new_width, new_height, input.format());

    const std::uint8_t *src = input.data();
    std::uint8_t *dst = output.data();

    int offset_dst = 0;
    int offset_src_row = 0, offset_src;
    // skip every factor-th pixel and every factor-th row
    for (int row=output.height(); row>0; row--) {
        offset_src = offset_src_row;
        for (int pixel=output.width(); pixel>0; pixel--) {
            // copy one pixel
            for (int byte=bpp; byte>0; byte--) {
                dst[offset_dst++] = src[offset_src++];
            }
            // skip (factor-1) pixels
            offset_src += bpp*(factor-1);
        }
        // skip factor rows
        offset_src_row += factor*bpp*input.width();
    }
}

void scaleUp(const Image &input, Image &output, int factor) {
    if (factor < 1) {
        return;
    } else if (factor == 1) {
        output = input;
    }
    int bpp = bytesPerPixel(input.format());
    int new_width = input.width() * factor;
    int new_height = input.height() * factor;

    // resize or (if necessary) create new Image
    output.resize(new_width, new_height, input.format());

    const std::uint8_t *src = input.data();
    std::uint8_t *dst = output.data();

    int offset_dst = 0;
    // copy every pixel and every row factor times
    int offset_src_row = 0, offset_src_pixel, offset_src_byte;
    for (int row=input.height(); row>0; row--) {
        offset_src_pixel = offset_src_row;

        // copy every pixel factor times in first row
        for(int pixel=input.width(); pixel>0; pixel--) {
            for (int pixel=factor; pixel>0; pixel--) {
                offset_src_byte = offset_src_pixel;
                for (int byte=bpp; byte>0; byte--) {
                    dst[offset_dst++] = src[offset_src_byte++];
                }
            }
            offset_src_pixel += bpp;
        }

        // replicate last row (factor-1) times
        for (int copyrow=factor-1; copyrow>0; copyrow--) {
            std::memcpy(dst+offset_dst, src+offset_src_row, new_width*bpp);
            offset_dst += new_width*bpp;
        }
        offset_src_row += input.width()*bpp;
    }
}

}  // namespace imaging
}  // namespace lms
