#ifndef LMS_IMAGING_CONVERTER_H
#define LMS_IMAGING_CONVERTER_H

#include "lms/imaging/image.h"
#include "lms/imaging/format.h"

namespace lms {
namespace imaging {

bool convert(const Image &input, Image &output, Format targetFormat);

void convertYUYVtoGREY(const Image &input, Image &output);

void convertYUYVtoBGRA(const Image &input, Image &output);

void convertGREYtoBGRA(const Image &input, Image &output);

}  // namespace imaging
}  // namespace lms

#endif /* LMS_IMAGING_CONVERTER_H */
