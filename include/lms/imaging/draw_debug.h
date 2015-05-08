//Just for testing
#define IMAGING_DRAW_DEBUG 1

#if IMAGING_DRAW_DEBUG == 1

#include <lms/imaging/graphics.h>
	#ifndef DRAWDEBUG
        #define DRAWDEBUG ,lms::imaging::Graphics &debug
        #define DRAWDEBUG_ARG ,debug
        #define DRAWDEBUG_CAPTURE ,&debug
	#endif

	#ifndef DRAWCROSS
        #define DRAWCROSS(x,y,r,g,b)\
        debug.setColor(lms::imaging::ARGBColor(r,g,b));\
        debug.drawCross(x,y)
    #endif

    #ifndef DRAWLINE
        #define DRAWLINE(x1,y1,x2,y2,r,g,b)\
        debug.setColor(lms::imaging::ARGBColor(r,g,b));\
        debug.drawLine(x1,y1,x2,y2)
    #endif

	#ifndef DRAWPOINT
        #define DRAWPOINT(x,y,r,g,b)\
        debug.setColor(lms::imaging::ARGBColor(r,g,b));\
        debug.drawPixel(x,y)
	#endif
#else
    #define DRAWDEBUG
    #define DRAWDEBUG_ARG
    #define DRAWDEBUG_CAPTURE
    #define DRAWCROSS(x,y,r,g,b)
    #define DRAWPOINT(x,y,r,g,b)
    #define DRAWLINE(x1,y1,x2,y2)
#endif


