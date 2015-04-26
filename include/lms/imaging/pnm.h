#ifndef LMS_IMAGING_PNM_H
#define LMS_IMAGING_PNM_H

#include "lms/imaging/image.h"

namespace lms {
namespace imaging {

/**
 * @brief Save a Portable Graymap. This works only for images that
 * are in the image format Format::GREY.
 *
 * @param image the image to save
 * @param path file path
 */
bool savePGM(const Image &image, const std::string &path);

bool readPGM(Image &image, const std::string &path);

/**
 * @brief Save a Portable Pixmap. This works only for images
 * in the format Format::RGB.
 *
 * @param image image to svae
 * @param path file path
 * @return
 */
bool savePPM(const Image &image, const std::string &path);

bool readPPM(Image &image, const std::string &path);

}  // namespace imaging
}  // namespace lms

#endif /* LMS_IMAGING_PNM_H */
