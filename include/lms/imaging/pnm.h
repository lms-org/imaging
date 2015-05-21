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

/**
 * @brief Save a Portable Pixmap. This works only for images
 * in the format Format::RGB.
 *
 * @param image image to svae
 * @param path file path
 * @return
 */
bool savePPM(const Image &image, const std::string &path);

/**
 * @brief Read a file as Portable Graymap or Portable Pixmap.
 *
 * PGM files will result in GREY images.
 * PPM files will result in RGB images.
 *
 * @param image image data to put
 * @param path file to load
 * @return true if successful, otherwise false
 */
bool readPNM(Image &image, const std::string &path);

}  // namespace imaging
}  // namespace lms

#endif /* LMS_IMAGING_PNM_H */
