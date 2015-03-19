#include <cstdio>
#include <string>

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

}  // namespace imaging
}  // namespace lms
