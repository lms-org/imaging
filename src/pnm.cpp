#include <cstdio>
#include <ios>
#include <fstream>
#include <string>
#include <algorithm>

#include "lms/imaging/pnm.h"
#include "lms/imaging/image.h"

namespace lms {
namespace imaging {

bool savePGM(const Image &image, const std::string &path) {
    if(image.format() != lms::imaging::Format::GREY) {
        return false;
    }

    std::ofstream of(path, std::ios::binary);
    of << "P5\n" << image.width() << " " << image.height() << " " << 255 << "\n";

    std::copy(
            reinterpret_cast<const char*>(image.data()),
            reinterpret_cast<const char*>(image.data() + image.size()),
            std::ostreambuf_iterator<char>(of));

    of.close();

    return true;
}

bool readPGM(Image &image, const std::string &path) {
    std::ifstream is(path, std::ios::binary);

    std::string magicLine;
    if(!std::getline(is, magicLine) || magicLine != "P5") {
        return false;
    }

    int width, height, maxVal;
    is >> width >> height >> maxVal;

    if(maxVal != 255) {
        // we are stupid here, because we rely on the behavior of savePGM
        return false;
    }

    image.resize(width, height, lms::imaging::Format::GREY);

    // skip one byte (is will be a whitespace character)
    is.ignore(1);

    // read data from ifstream into buffer
    std::copy(
            std::istreambuf_iterator<char>(is),
            std::istreambuf_iterator<char>( ),
            reinterpret_cast<char*>(image.data()));
    is.close();

    return true;
}

}  // namespace imaging
}  // namespace lms
