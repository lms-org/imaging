#ifndef LMS_IMAGING_IMAGE_H
#define LMS_IMAGING_IMAGE_H

#include <memory>
#include <algorithm>
#include <utility>
#include <cstdint>

#include "lms/imaging/format.h"

#ifdef USE_CEREAL
#include "lms/serializable.h"
#include "cereal/cerealizable.h"
#include "cereal/cereal.hpp"
#endif

namespace lms {
namespace imaging {

/**
 * @brief Image serves as a data channel type, as an replacement
 * for char* with size parameter.
 *
 * A dynamic image is a two dimensional array where the type is already
 * given during compile time. The width and height can be changed during runtime.
 *
 * The data is stored row-by-row in a simple one-dimensional array.
 * If you want to be cache-optimized you should read the array row by row.
 *
 * @author Hans Kirchner
 */
class Image
#ifdef USE_CEREAL
    : public lms::Serializable
#endif
{
public:
    /**
     * @brief Create an image of size zero.
     */
    Image();

    /**
     * @brief Create a new dynamic image with the given width and height.
     *
     * The type is always given during compile time as the type parameter.
     *
     * @param width width of the image (x-direction)
     * @param height height of the image (y-direction)
     */
    Image(int width, int height, Format fmt);

    /**
     * @brief Copy constructor
     *
     * NOTE: The capacity is not kept
     *
     * @param Image object to copy
     */
    Image(const Image &obj);

    /**
     * @brief Move constructor
     * @param object to move
     */
    Image(Image &&obj) = default;

    /**
     * @brief Copy assignment operator
     *
     * NOTE: If the capacity of this image is sufficient
     * the no new memory will be allocated.
     *
     * @param rhs right side of the assignment
     * @return this
     */
    Image& operator=(const Image &rhs);

    /**
     * @brief Move assignment operator
     * @param rhs right side of the assignment
     * @return this
     */
    Image& operator=(Image &&rhs) = default;

    /**
     * @brief Resize the image to the new width and height.
     *
     * NOTE: This will discard all data that was in the image before.
     *
     * @param width new width of the image
     * @param height new height of the image
     */
    void resize(int width, int height, Format fmt);

    /**
     * @brief Fill the dynamic image with the given value.
     * @param value
     */
    void fill(std::uint8_t value);

    /**
     * @brief Trim the image to the needed size.
     *
     * Capacity and size will be equal afterwards.
     */
    void trim();

    /**
     * @brief Return the width of the image
     * @return width
     */
    int width() const;

    /**
     * @brief Return the height of the image
     * @return height
     */
    int height() const;

    /**
     * @brief Return the image format.
     * @return image format
     */
    Format format() const;

    /**
     * @brief Return the size of the image, that is width * height * bytesPerPixel.
     * @return size of the image
     */
    int size() const;

    /**
     * @brief inside
     * @param x
     * @param y
     * @return true if the pixel is inside the image
     */
    bool inside(int x, int y) const;

    /**
     * @brief Return the capacity of the image in bytes.
     * @return
     */
    int capacity() const;

    /**
     * @brief Return a raw data pointer
     *
     * NOTE: Do NOT delete this pointer.
     * The image object will delete it automatically.
     *
     * @return data pointer
     */
    std::uint8_t* data();

    /**
     * @brief Return a const raw data pointer
     *
     * NOTE: Do NOT delete this pointer
     * The image object will delete it automatically.
     *
     * @return data pointer
     */
    const std::uint8_t* data() const;

//implement cereal
#ifdef USE_CEREAL
    //get default interface for datamanager
    CEREAL_SERIALIZATION()

    //cereal methods
    template<class Archive>
    void save(Archive & archive) const {
        archive( m_width, m_height, m_fmt, m_size,
                 cereal::binary_data(m_data.get(), m_size * sizeof(std::uint8_t)));
    }

    template<class Archive>
    void load(Archive & archive) {
        int width, height, size;
        Format fmt;

        archive(width, height, fmt, size);
        resize(width, height, fmt);
        archive(cereal::binary_data(m_data.get(), size * sizeof(std::uint8_t)));
    }
#endif

private:
    int m_width;
    int m_height;
    Format m_fmt;
    int m_size;
    int m_capacity;
    std::unique_ptr<std::uint8_t[]> m_data;
};

} // namespace imaging
} // namespace lms

#endif /* LMS_IMAGING_IMAGE_H */
