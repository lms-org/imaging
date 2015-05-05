set ( SOURCES 
	"src/image.cpp"
	"src/static_image.cpp"
	"src/format.cpp"
	"src/converter.cpp"
	"src/pnm.cpp"
	"src/image_factory.cpp"
    "src/graphics.cpp"
##########find
	"src/find/edge_point.cpp"
	"src/find/line.cpp"
        "src/find/line_point.cpp"
	"src/find/pixel.cpp"

)

set (HEADERS 
	"include/lms/imaging/image.h"
	"include/lms/imaging/static_image.h"
	"include/lms/imaging/format.h"
	"include/lms/imaging/converter.h"
	"include/lms/imaging/pnm.h"
	"include/lms/imaging/image_factory.h"
    "include/lms/imaging/graphics.h"
##########find
	"include/lms/imaging/find/edge_point.h"
	"include/lms/imaging/find/line.h"
	"include/lms/imaging/find/line_point.h"
	"include/lms/imaging/find/object.h"
	"include/lms/imaging/find/pixel.h"
        "include/lms/imaging/draw_debug.h"
)

include_directories("include")

add_library (imaging SHARED ${SOURCES} ${HEADERS})
target_link_libraries (imaging PRIVATE lms)
