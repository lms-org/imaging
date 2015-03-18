#include <lms/imaging/dynamic_image.h>

namespace lms {
namespace imaging {

constexpr DynamicImage::DynamicImage() : m_width(0), m_height(0), m_fmt(Format::UNKNOWN),
    m_size(0), m_capacity(0) {
}

DynamicImage::DynamicImage(int width, int height, Format fmt) : m_width(width), m_height(height),
    m_fmt(fmt), m_size(imageBufferSize(width, height, fmt)), m_capacity(m_size),
    m_data(new std::uint8_t[m_size]) {
}

DynamicImage::DynamicImage(const DynamicImage &obj) : m_width(obj.m_width), m_height(obj.m_height),
    m_fmt(obj.m_fmt), m_size(obj.m_size), m_capacity(obj.m_size),
    m_data(new std::uint8_t[m_size]) {

    std::copy(obj.m_data.get(), obj.m_data.get() + m_size, m_data.get());
}

DynamicImage& DynamicImage::operator=(const DynamicImage &rhs) {
    this->m_width = rhs.m_width;
    this->m_height = rhs.m_height;
    this->m_fmt = rhs.m_fmt;
    this->m_size = rhs.m_size;

    // check if capacity is not sufficient
    if(this->capacity() < rhs.size()) {
        // allocate new heap memory and discard the old memory
        m_data.reset(new std::uint8_t[m_size]);

        // trim the image to the needed size
        m_capacity = m_size;  // capacity is always the size of the allocated memory block
    }
    std::copy(rhs.m_data.get(), rhs.m_data.get() + m_size, m_data.get());
    return *this;
}

void DynamicImage::resize(int width, int height, Format fmt) {
    // using move assignment operator here
    *this = DynamicImage(width, height, fmt);
}

void DynamicImage::fill(std::uint8_t value) {
    std::fill_n(m_data.get(), m_size, value);
}

void DynamicImage::trim() {
    if(m_size != m_capacity) {
        std::uint8_t *newMem = new std::uint8_t[m_size];
        std::copy(m_data.get(), m_data.get() + m_size, newMem);

        m_data.reset(newMem);
        m_capacity = m_size;
    }
}

int DynamicImage::width() const {
    return m_width;
}

int DynamicImage::height() const {
    return m_height;
}

Format DynamicImage::format() const {
    return m_fmt;
}

int DynamicImage::size() const {
    return m_size;
}

int DynamicImage::capacity() const {
    return m_capacity;
}

std::uint8_t* DynamicImage::data() {
    return m_data.get();
}

const std::uint8_t* DynamicImage::data() const {
    return m_data.get();
}

} // namespace imaging
} // namespace lms
