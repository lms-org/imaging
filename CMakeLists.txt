if(USE_CONAN)
cmake_minimum_required(VERSION 2.8.12)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
endif(USE_CONAN)

set ( SOURCES
    "src/image.cpp"
    "src/format.cpp"
    "src/converter.cpp"
    "src/pnm.cpp"
    "src/image_factory.cpp"
    "src/graphics.cpp"
    "src/transform_image.cpp"
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
    "include/lms/imaging/transform_image.h"
)

#set compiler flags
if((${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU"))
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wreturn-type -Wpedantic ")
endif()

include_directories("include")

add_library (lms_imaging SHARED ${SOURCES} ${HEADERS})

if(USE_CONAN)
target_link_libraries(lms_imaging ${CONAN_LIBS})
else()
target_link_libraries (lms_imaging PRIVATE lmscore lms_math)
endif(USE_CONAN)

#if(USE_OPENCV)#TODO opencv flag in conan file
#    find_package( OpenCV REQUIRED )
#    include_directories(${OpenCV_INCLUDE_DIRS} )
#    target_link_libraries(lms_imaging PRIVATE ${OpenCV_LIBS})
#    message(STATUS "Enabling OpenCV extensions in imaging lib")
#endif()
