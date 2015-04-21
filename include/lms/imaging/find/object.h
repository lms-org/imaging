#ifndef IMAGE_OBJECT
#define IMAGE_OBJECT

#include "stdint.h"

#include <image/image_pixel.h>
#include <data/hints.h>
#include <data/image_data.h>

class ImageObject{

public:
    virtual bool find(Pixel px, uint16_t length, int16_t phi) = 0;
    virtual bool verify() = 0;
    virtual int serialize (Data::ImageDataSpace::Image *imageStruct) = 0;
    virtual void init() = 0;
    virtual void deinit() = 0;
    virtual ~ImageObject() {}

    const Data::HintDataSpace::Hint* get_myHint() const { return myHint; }
    void set_myHint(const Data::HintDataSpace::Hint *hint) { myHint = hint; }

    virtual bool verifyable() { return true; }

private:
    const Data::HintDataSpace::Hint *myHint;



};

#endif // IMAGE_OBJECT
