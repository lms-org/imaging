set ( SOURCES 
	"src/dynamic_image.cpp"
	"src/static_image.cpp"
)

set (HEADERS 
	"include/lms/imaging/dynamic_image.h"
	"include/lms/imaging/static_image.h"
	${HEADERS_SHARED}
)

include_directories("include")

add_library (imaging MODULE ${SOURCES} ${HEADERS})
target_link_libraries (imaging PRIVATE lms)
