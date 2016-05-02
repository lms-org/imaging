set ( SOURCES
    "src/image.cpp"
    "src/static_image.cpp"
    "src/format.cpp"
    "src/converter.cpp"
    "src/pnm.cpp"
    "src/image_factory.cpp"
    "src/graphics.cpp"
)

set (HEADERS
    "include/lms/imaging/image.h"
    "include/lms/imaging/static_image.h"
    "include/lms/imaging/format.h"
    "include/lms/imaging/converter.h"
    "include/lms/imaging/pnm.h"
    "include/lms/imaging/image_factory.h"
    "include/lms/imaging/graphics.h"
    "include/lms/imaging/draw_debug.h"
)

include_directories("include")
add_library (imaging SHARED ${SOURCES} ${HEADERS})
target_link_libraries (imaging PRIVATE lmscore)


if(USE_OPENCV)
    find_package( OpenCV REQUIRED )
    include_directories(${OpenCV_INCLUDE_DIRS} )
    target_link_libraries(imaging PRIVATE ${OpenCV_LIBS})
    message(STATUS "Enabling OpenCV extensions in imaging lib")
endif()
