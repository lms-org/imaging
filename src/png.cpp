#include <cstdio>
#include <png.h>

#include "lms/imaging/png.h"
#include "lms/imaging/image.h"

namespace lms {
namespace imaging {

bool readPNG(Image &image, const std::string &path) {
    FILE *fp = fopen(path.c_str(), "rb");
    if(!fp) {
        return false;
    }

    // Read first 8 bytes to check header
    png_byte header[8];
    size_t numHeaderBytes = 8;
    if(fread(header, 1, numHeaderBytes, fp) != numHeaderBytes)
    {
        return false;
    }

    if(png_sig_cmp(header, 0, numHeaderBytes)) {
        // Header byte mismatch
        // -> not a valid png file
        return false;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if(!png_ptr) {
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return false;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, numHeaderBytes);

    png_read_info(png_ptr, info_ptr);

    auto bitDepth = png_get_bit_depth(png_ptr, info_ptr);
    if(bitDepth != 8) {
        // Currently only 8bit is supported
        return false;
    }

    auto colorType = png_get_color_type(png_ptr, info_ptr);
    Format format;
    if(PNG_COLOR_TYPE_GRAY == colorType) {
        format = Format::GREY;
    } else if(PNG_COLOR_TYPE_RGB == colorType) {
        format = Format::RGB;
    } else if(PNG_COLOR_TYPE_RGB_ALPHA == colorType) {
        format = Format::BGRA;
    } else {
        return false;
    }

    auto width = png_get_image_width(png_ptr, info_ptr);
    auto height = png_get_image_height(png_ptr, info_ptr);
    image.resize(width, height, format);

    // Fill image data
    png_bytep* rowPointers = new png_bytep[height];
    int bytesPerRow = image.width() * bytesPerPixel(image.format());
    for(int i = 0; i < image.height(); i++) {
        rowPointers[i] = image.data() + i * bytesPerRow;
    }

    png_read_image(png_ptr, rowPointers);
    delete[] rowPointers;

    fclose(fp);

    return true;
}

bool savePNG(const Image& image, const std::string& path)
{
    FILE *fp = fopen(path.c_str(), "wb");
    if(!fp) {
        return false;
    }

    auto png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr) {
        return false;
    }

    auto info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr) {
        return false;
    }

    png_init_io(png_ptr, fp);

    // Write header
    int colorType;
    if(image.format() == Format::GREY) {
        colorType = PNG_COLOR_TYPE_GRAY;
    } else if(image.format() == Format::RGB) {
        colorType = PNG_COLOR_TYPE_RGB;
    } else if(image.format() == Format::BGRA) {
        colorType = PNG_COLOR_TYPE_RGB_ALPHA;
    } else {
        return false;
    }
    png_set_IHDR(png_ptr, info_ptr, image.width(), image.height(),
                 8, colorType, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    // Fill image data
    png_bytep* rowPointers = new png_bytep[image.height()];
    int bytesPerRow = image.width() * bytesPerPixel(image.format());
    for(int i = 0; i < image.height(); i++) {
        rowPointers[i] = const_cast<png_bytep>(image.data() + i * bytesPerRow);
    }

    png_write_image(png_ptr, rowPointers);
    png_write_end(png_ptr, NULL);

    delete[] rowPointers;

    fclose(fp);

    return true;
}
}  // namespace imaging
}  // namespace lms
