set ( SOURCES 
	"src/image.cpp"
	"src/static_image.cpp"
	"src/format.cpp"
)

set (HEADERS 
	"include/lms/imaging/image.h"
	"include/lms/imaging/static_image.h"
	"include/lms/imaging/format.h"
	${HEADERS_SHARED}
)

include_directories("include")

add_library (imaging SHARED ${SOURCES} ${HEADERS})
target_link_libraries (imaging PRIVATE lms)
