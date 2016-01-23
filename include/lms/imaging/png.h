#ifndef LMS_IMAGING_PNG_H
#define LMS_IMAGING_PNG_H

#include "lms/imaging/image.h"

namespace lms {
    namespace imaging {

/**
 * @brief Save a image as Portable Network Graphics.
 *
 * @param image the image to save
 * @param path file path
 */
        bool savePNG(const Image &image, const std::string &path);

/**
 * @brief Read a file as Portable Network Graphics.
 *
 * @param image image data to put
 * @param path file to load
 * @return true if successful, otherwise false
 */
        bool readPNG(Image &image, const std::string &path);

    }  // namespace imaging
}  // namespace lms

#endif /* LMS_IMAGING_PNG_H */
