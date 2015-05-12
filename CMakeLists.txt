set ( SOURCES 
    "src/image.cpp"
    "src/static_image.cpp"
    "src/format.cpp"
    "src/converter.cpp"
    "src/pnm.cpp"
    "src/image_factory.cpp"
    "src/graphics.cpp"
    "src/warp.cpp"
##########find
    "src/find/edge_point.cpp"
    "src/find/line.cpp"
    "src/find/line_point.cpp"

)

set (HEADERS 
    "include/lms/imaging/image.h"
    "include/lms/imaging/static_image.h"
    "include/lms/imaging/format.h"
    "include/lms/imaging/converter.h"
    "include/lms/imaging/pnm.h"
    "include/lms/imaging/image_factory.h"
    "include/lms/imaging/graphics.h"
    "include/lms/imaging/warp.h"
##########magic
    "include/lms/imaging/magic/n2d.h"
    "include/lms/imaging/magic/lutx.h"
    "include/lms/imaging/magic/luty.h"
    "include/lms/imaging/magic/cali_ab.h"
##########find
    "include/lms/imaging/find/edge_point.h"
    "include/lms/imaging/find/line.h"
    "include/lms/imaging/find/line_point.h"
    "include/lms/imaging/find/image_hint.h"
    "include/lms/imaging/draw_debug.h"
)

include_directories("include")

add_library (imaging SHARED ${SOURCES} ${HEADERS})
target_link_libraries (imaging PRIVATE lms math_lib)
