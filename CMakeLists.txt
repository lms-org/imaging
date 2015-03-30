set ( SOURCES 
	"src/image.cpp"
	"src/static_image.cpp"
	"src/format.cpp"
	"src/converter.cpp"
	"src/pnm.cpp"
	"src/image_factory.cpp"
)

set (HEADERS 
	"include/lms/imaging/image.h"
	"include/lms/imaging/static_image.h"
	"include/lms/imaging/format.h"
	"include/lms/imaging/converter.h"
	"include/lms/imaging/pnm.h"
	"include/lms/imaging/image_factory.h"
)

include_directories("include")

add_library (imaging SHARED ${SOURCES} ${HEADERS})
target_link_libraries (imaging PRIVATE lms)
