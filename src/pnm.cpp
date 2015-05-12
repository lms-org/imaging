#include <cstdio>
#include <ios>
#include <fstream>
#include <string>
#include <algorithm>

#include "lms/imaging/pnm.h"
#include "lms/imaging/image.h"

namespace lms {
namespace imaging {

bool pnmHelperSave(const Image &image, const std::string &path,
                   Format checkFormat, const std::string &type) {
    if(image.format() != checkFormat) {
        return false;
    }

    std::ofstream of(path, std::ios::binary);

    if(!of.is_open()) {
        return false;
    }

    of << type << "\n" << image.width() << " " << image.height()
       << " " << 255 << "\n";

    std::copy(
            reinterpret_cast<const char*>(image.data()),
            reinterpret_cast<const char*>(image.data() + image.size()),
            std::ostreambuf_iterator<char>(of));

    of.close();

    return true;
}

bool pnmHelperRead(Image &image, const std::string &path,
                   Format checkFormat, const std::string &type) {
    std::ifstream is(path, std::ios::binary);

    if(! is.is_open()) {
        return false;
    }

    std::string magicLine;
    if(!std::getline(is, magicLine) || magicLine != type) {
        return false;
    }

    int width, height, maxVal;
    is >> width >> height >> maxVal;

    if(maxVal != 255) {
        // we are stupid here, because we rely on the behavior of save
        return false;
    }

    image.resize(width, height, checkFormat);

    // skip one byte (this will be a whitespace character)
    is.ignore(1);

    // read data from ifstream into buffer
    // TODO or use is.read() ...
    std::copy_n(
            std::istreambuf_iterator<char>(is),
            width * height,
            reinterpret_cast<char*>(image.data()));
    is.close();

    return true;
}

bool savePGM(const Image &image, const std::string &path) {
    return pnmHelperSave(image, path, Format::GREY, "P5");
}

bool readPGM(Image &image, const std::string &path) {
    return pnmHelperRead(image, path, Format::GREY, "P5");
}

bool savePPM(const Image &image, const std::string &path) {
    return pnmHelperSave(image, path, Format::RGB, "P6");
}

bool readPPM(Image &image, const std::string &path) {
    return pnmHelperRead(image, path, Format::RGB, "P6");
}

}  // namespace imaging
}  // namespace lms
