#include "lms/imaging/converter.h"

namespace lms {
namespace imaging {

bool convert(const Image &input, Image &output, Format targetFormat) {
    if(input.format() == Format::YUYV && targetFormat == Format::GREY) {
        convertYUYVtoGREY(input, output);
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

}  // namespace imaging
}  // namespace lms
