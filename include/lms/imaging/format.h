#ifndef LMS_IMAGING_FORMAT_H
#define LMS_IMAGING_FORMAT_H

#include <cstdint>
#include <iostream>

namespace lms {
namespace imaging {

enum class Format : std::uint8_t {
    UNKNOWN = 0, GREY = 1, YUYV = 2, BGRA = 3
};

/**
 * @brief Returns the number of bytes that are needed
 * to store a single pixel in the given format.
 *
 * @param fmt image format
 * @return -1 if UNKNOWN, 0 if the number of bytes is not integer, a positive value
 * in all other cases
 */
int bytesPerPixel(Format fmt);

/**
 * @brief Compute the buffer size in bytes that is needed to store
 * an image with the given parameters.
 *
 * @param width width of the image
 * @param height height of the image
 * @param fmt image format
 * @return size in bytes
 */
int imageBufferSize(int width, int height, Format fmt);

std::string formatToString(Format fmt);

Format formatFromString(const std::string &string);

std::ostream& operator << (std::ostream &stream, const Format &fmt);

std::ostream& operator >> (std::ostream &stream, Format &fmt);

}  // namespace imaging
}  // namespace lms

#endif /* LMS_IMAGING_FORMAT_H */
