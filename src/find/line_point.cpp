#include "image/image_line_point.h"
#include "image/image_edge_point.h"
#include "image/image_tools.h"
#include <math.h>

bool LinePoint::find(Pixel start, int length, int16_t dir) {
    //printf("pixel: %i %i \n", start.x, start.y);
    int newDir = 0;
    bool foundFirst = up.find(start, length, dir, EdgePoint::low_high, linepointThreshold);
    bool foundSecond = false;
    //printf("Search Linepoint \n");
    //start.green();
    //Standardmäßig verified auf false
    verified = false;

    if(foundFirst) {
        up.non_disc_direction = limitAngle(up.non_disc_direction);
        newDir = secondSearchDir(up.non_disc_direction);
        Pixel px = up.pixel;
        px.move(
                (int)(2.0*cos(newDir * M_PI/180.) + 0.5),
                (int)(2.0*sin(newDir * M_PI/180.) + 0.5)
        );

        foundSecond = down.find(px, downSearchLength, newDir, EdgePoint::high_low, linepointThreshold);
        //printf("pixel found\n");
        //px.rosa();

        // Wenn der down EdgePoint mit der berechneten Richtung newDir nicht gefunden wird, dann
        // wird der Punkt nochmal gesucht in Richtung des Suchstreifens
        if(!foundSecond) {

            Pixel px = up.pixel;
            px.move(
                (int)(1.0*cos(dir* M_PI/180.) + 0.5),
                (int)(1.0*sin(dir* M_PI/180.) + 0.5)
            );
            foundSecond = down.find(px, downSearchLength, dir, EdgePoint::high_low, linepointThreshold);
            //px.rosa();
        }

    }

    if(foundFirst && foundSecond) {
/*
        if(!checkAngle() || !checkLength()) {
            //up.pixel.yellow();
            Pixel px = up.pixel;
            px.move(
                (int)(2.0*cos(dir* M_PI/180.) + 0.5),
                (int)(2.0*sin(dir* M_PI/180.) + 0.5)
            );

            foundSecond = down.find(&px, downSearchLength, dir, EdgePoint::high_low, 254);

        }
*/
        if(foundSecond && checkAngle() && checkLength()) {
            verified = true;
            up.pixel.green();
            down.pixel.green();

        }
    }
    if(verified){
       return true;
    }else{
        return false;
    }


}


bool LinePoint::checkAngle() {


    if (fabs(180.0 - fabs(up.non_disc_direction - down.non_disc_direction)) < maxDiffAngle)
        return true;

    //printf("180 - abs(%f - %f )= %f\n", up.non_disc_direction, down.non_disc_direction, 180.0 - abs(up.non_disc_direction - down.non_disc_direction));

    return false;
}

bool LinePoint::checkLength() {

    int dx = up.pixel.x - down.pixel.x;
    int dy = up.pixel.y - down.pixel.y;

    int length = dx*dx + dy*dy;

    if(minSquareLength < length) {
        return true;
    }

    else return false;
}

int LinePoint::secondSearchDir(int gradientDir) {

    int dir = gradientDir-90;

    return limitAngle(dir);
}

