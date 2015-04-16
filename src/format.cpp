#include "lms/imaging/format.h"

namespace lms {
namespace imaging {

int bytesPerPixel(Format fmt) {
    switch(fmt) {
    case Format::GREY: return 1;
    case Format::YUYV: return 2;
    case Format::BGRA: return 4;
    case Format::RGB : return 3;
    case Format::UNKNOWN:  // go to default
    default: return -1;  // Don't forget to implement missing formats
    }
}

int imageBufferSize(int width, int height, Format fmt) {
    // TODO do something special if we have strange formats like V4L2_PIX_FMT_Y10BPACK
    return width * height * bytesPerPixel(fmt);
}

std::string formatToString(Format fmt) {
    switch(fmt) {
    case Format::GREY: return "GREY";
    case Format::YUYV: return "YUYV";
    case Format::BGRA: return "BGRA";
    case Format::RGB : return "RGB";
    case Format::UNKNOWN:  // go to default
    default: return "UNKNOWN";
    }
}

Format formatFromString(const std::string &str) {
    if(str == "GREY") return Format::GREY;
    if(str == "YUYV") return Format::YUYV;
    if(str == "BGRA") return Format::BGRA;
    if(str == "RGB")  return Format::RGB;
    return Format::UNKNOWN;
}

std::ostream& operator << (std::ostream& stream, const Format &fmt) {
    return stream << formatToString(fmt);
}

std::istream& operator >> (std::istream &stream, Format &fmt) {
    std::string str;
    if(std::getline(stream, str)) {
        fmt = formatFromString(str);
    } else {
        fmt = Format::UNKNOWN;
    }
    return stream;
}

}  // namespace imaging
}  // namespace lms
