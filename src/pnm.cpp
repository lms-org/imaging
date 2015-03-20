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

    FILE* file = fopen(path.c_str(), "w");
    fprintf(file, "P5\n%i %i %i\n", image.width(), image.height(), 255);
    fwrite(image.data(), image.size(), 1, file);
    fclose(file);

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
