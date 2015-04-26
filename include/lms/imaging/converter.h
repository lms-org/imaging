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

}  // namespace imaging
}  // namespace lms

#endif /* LMS_IMAGING_CONVERTER_H */
