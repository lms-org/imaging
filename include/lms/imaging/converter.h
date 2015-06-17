#ifndef LMS_IMAGING_CONVERTER_H
#define LMS_IMAGING_CONVERTER_H

#include "lms/imaging/image.h"
#include "lms/imaging/format.h"

namespace lms {
namespace imaging {

bool convert(const Image &input, Image &output, Format targetFormat);

bool convertRaw(Format srcFormat, const std::uint8_t *src, int srcSize,
                Format dstFormat, std::uint8_t *dst);

void convertYUYVtoGREY(const std::uint8_t *src, int srcSize, std::uint8_t *dst);

void convertYUYVtoBGRA(const std::uint8_t *src, int srcSize, std::uint8_t *dst);

void convertGREYtoBGRA(const std::uint8_t *src, int srcSize, std::uint8_t *dst);

void convertYUYVtoRGB(const std::uint8_t *src, int srcSize, std::uint8_t *dst);

void convertGREYtoRGB(const std::uint8_t *src, int srcSize, std::uint8_t *dst);

void convertRGBtoBGRA(const std::uint8_t *src, int srcSize, std::uint8_t *dst);


/**
 * @brief Scales the image up (increases the size).
 * The resulting image will have dimensions of
 * old_width*factor and old_height*factor, respectively.
 * @return The resized image
 *
 * @param input input image
 * @param output resized output image
 * @param factor upscaling factor (>1)
 */
void scaleUp(const Image &input, Image &output, int factor);

/**
 * @brief Scales the image down (decreases the size).
 * The resulting image will have dimensions of
 * old_width/factor and old_height/factor, respectively.
 *
 * @param input input image
 * @param output resized output image
 * @param factor downscaling factor (>1)
 */
void scaleDown(const Image &input, Image &output, int factor);

}  // namespace imaging
}  // namespace lms

#endif /* LMS_IMAGING_CONVERTER_H */
