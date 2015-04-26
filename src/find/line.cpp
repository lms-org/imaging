#include "lms/imaging/find/line.h"
#include "lms/imaging/find/line_point.h"
#include <math.h>
#include <algorithm>
namespace lms{
namespace imaging{
namespace find{
//initialsuche wenn man davor noch nichts gefunden hat
bool Line::find(Pixel &startPoint, int searchLength, float searchAngle,int minWidth,int maxWidth, int sobelThreshold,Image &gaussBuffer){
    //clear old stuff
    points.clear();
    //find first point
    LinePoint initPoint;
    if(!initPoint.find(startPoint, searchLength, searchAngle,minWidth,maxWidth, sobelThreshold,gaussBuffer))
        return false;
    int maxSteps = 100;
    float stepLength = 10;
    //Wie entscheidet man, in welche richtung man erweitern möchte?
    //----> man gibt eine suchrichtung in, in welche der nächste punkt gesucht werden soll.
    //Senkrecht zur suchrichtung verläuft dann die suchlinie für den nächsten punk
    //
    //TODO extend below
    //extend(initPoint,true,maxSteps,stepLength);
    //TODO extend above
    //extend(initPoint,false,maxSteps,stepLength);

    /*
    points.clear();
    LinePoint newpoint;
	//sucht einen linepoint
    bool found = newpoint.find(px, length, phi);

    if(found) {

        points.push_back(newpoint);
	//sucht nach unten
        int found_below = extend(newpoint, stepsizeX, stepsizeY, 0, true, points.end());
	//sucht nach oben um linie zu erweitern
        int found_above = extend(newpoint, -stepsizeX, -stepsizeY, 1, true, points.begin());

        ///Found below & Above are not used - silence the compiler
        (void)found_above;
        (void)found_below;

        std::sort(points.begin(), points.end());

        for(std::deque<LinePoint>::iterator i = points.begin(); i != points.end(); i++) {
            i->up.pixel.yellow();
        }

        return true;

    }
    */
    return false;

}

bool Line::verify(){
    /*
    // Verifizierung von jedem Linienpunkt
    //printf("points: %i\n", points.size());
    if(points.size() > 2) {

        //printf("verify \n");

        for(std::deque<LinePoint>::iterator i = points.begin(); (i != points.end()-1) && (i != points.end()-2) && (i != points.end()); i++)
        {
                //printf("\n Pointer %i | %i | %i \t", i->up.pixel.y, (i+1)->up.pixel.y, (i+2)->up.pixel.y);


                int dis1 = calcDistance(*i, *(i+1));
                int dis2 = calcDistance(*(i+1), *(i+2));

                /// Sind 3 Linienpunkte zu nah beieinander -> mittleren löschen
                if((dis1 < desiredDistance) && (dis2 < desiredDistance)) {
                    //printf("Erase: %i \n", (i+1)->up.pixel.y);
                    i = points.erase(i+1)- 2;
                    continue;
                }

                Pixel px = i->up.pixel;
                px.move(-3, 0);

                bool found = i->find(px, verifySearchLength, limitAngle(i->up.non_disc_direction - 90.0));
                if(found) {
                    //i->up.pixel.green();
                } else {
                    //printf("erase i: %i", i->up.pixel.y);
                    i = points.erase(i)-1;
                }
        }

//        for(std::deque<LinePoint>::iterator i = points.begin(); i != points.end(); i++) {
//                printf("\n After Verify: y %i \t", i->up.pixel.y);
//        }
        /// Die letzten zwei Linienpunkte müssen gesondert verifiziert werden (ohne Abstandsbedingung)
        verifyLastPoints();

//        for(std::deque<LinePoint>::iterator i = points.begin(); i != points.end(); i++) {
//                printf("\n After VerifyLast: y %i \t", i->up.pixel.y);
//        }

        /// Falls eine zu große Lücke zwischen zwei Linienpunkten ist -> auffüllen
        insertbetween(verifySearchLength, stepsizeX, stepsizeY);

//        for(std::deque<LinePoint>::iterator i = points.begin(); i != points.end(); i++) {
//                printf("\n After insertbetween: y %i \t", i->up.pixel.y);
//        }

        /// Versuche die Linie nach oben und unten zu erweitern
        extendVerifiedLine(stepsizeX, stepsizeY);


        if(points.size() > minPoints) {

            for(std::deque<LinePoint>::iterator i = points.begin(); i != points.end(); i++) {
                i->up.pixel.green();
                i->down.pixel.green();
                //printf("\n After Extend: y %i \t", i->up.pixel.y);
            }
            return true;
        }
        else{
            points.clear();
            return false;
        }
    }
    else{
        points.clear();
        return false;
    }
    */
    return false;

}

/// Falls der Abstand zw. zwei Punkten zu groß wird -> neue Suche von Linienpunkten dazwischen

void Line::insertbetween(int maxDist, int stepsizeX, int stepsizeY){

    for (auto it = points.begin(); (it != points.end() - 1) && (it != points.end()); it++) {
        int dist = calcDistance(*it, *(it+1));
        if (dist > maxDist) {
            //printf("insert between %i | %i \n", it->up.pixel.y, (it+1)->up.pixel.y);
		//anzahl wie oft gesucht werden soll (wieviele linePoints man gerne hätte (abschätzung))
            int counter = dist/stepsizeY - 1;
            if(counter > 0) {
                //printf("counter: %i @%i\n", counter, it - points.begin());
                LinePoint newLinePoint = *it;
                int cnt = 0;// extend(*(it+1), -stepsizeX, -stepsizeY, 1, false, it+1, counter, &it, &newLinePoint);
                ///We do not use the number of how many points were inserted -> silence the compiler
                (void)cnt;
            }
        }
    }

}

/// Die letzten zwei Linienpunkte müssen gesondert verifiziert werden
void Line::verifyLastPoints(){
    /*
    // Vorletzten Punkt verifizieren
    std::deque<LinePoint>::iterator last = points.end() - 2;
    last->up.pixel.yellow();
    Pixel px = last->up.pixel;
    px.move(-3, 0);
    bool foundLast = last->find(px, verifySearchLength, limitAngle(last->up.non_disc_direction - 90.0));

    if(foundLast) {
        //last->up.pixel.red();
    } else {
        last = points.erase(last);
    }
    // Letzten Punkt verifizieren
    last = points.end() - 1;
    last->up.pixel.yellow();
    px = last->up.pixel;
    px.move(-3, 0);
    foundLast = last->find(px, verifySearchLength, limitAngle(last->up.non_disc_direction - 90.0));

    if(foundLast) {
        //last->up.pixel.yellow();
    } else {
        last = points.erase(last);
    }
    */
}

/// Ab dem obersten und untersten Linienpunkt wird versucht die Linie zu erweitern
void Line::extendVerifiedLine(int stepsizeX, int stepsizeY){
    /*
    std::deque<LinePoint>::iterator i = points.end()-1;
    i->up.pixel.blue();
    int found_below = extend(*i, stepsizeX+8, stepsizeY+8, 0, false, points.end());

    i = points.begin();
    i->up.pixel.blue();
    int found_above = extend(*i, -stepsizeX+8, -stepsizeY+8, 1, false, points.begin());

    ///We do not user found_below/above, so silence the compiler
    (void) found_above;
    (void) found_below;
    */
}

int Line::extend(LinePoint &start, bool direction,int stepLengthMin, int stepLengthMax, float searchLength){
    LinePoint newpoint = start;
/*
    LinePoint newpoint = lp;
    bool found = 0;
    int sumFound = 0;
    int dx = 0;
    int dy = 0;

    do {

        /// Ausgehend vom neuen Linepoints wird der nächste gesucht
        int angle = (int)newpoint.up.non_disc_direction;
        LinePoint start = newpoint;
        Pixel px = newpoint.up.pixel;
        dx = (int)(stepsizeX*cos((newpoint.up.non_disc_direction)*M_PI/180.0) + 0.5);
        dy = (int)(stepsizeY*sin((newpoint.up.non_disc_direction)*M_PI/180.0) + 0.5);

        /// Falls nicht in die richtige Suchrichtung (oben bzw. unten) gesucht wird -> Abbruch
        if((sign != std::signbit(dy)) && (changingSearchDir == false)) {
            break;
        }

        bool moved =  px.move(dx-3, dy);
        /// Falls der Pixel nicht bewegt wurde (z.B. Bildgrenze) -> Abbruch
        if(!moved)
            break;

        /// Neuen Linepoint suchen
        found = newpoint.find(px, extendSearchLength, limitAngle(angle - 90));
        /// Falls der Linepoint nicht gefunden wird wird der Startpixel der Suchlinie höchstens
        /// 1 mal um dx, dy verschoben und erneut gesucht.
        if(!found) {
            for(int i = 0; i<1; i++) {
                bool moved = px.move(dx, dy);
                if(!moved)
                    break;
                found = newpoint.find(px, extendSearchLength, limitAngle(angle - 90));
                if(found) {
                    break;
                }
            }
        }

        /// Wenn der Linienpunkt gefunden wird  und der Winkel zwischen dem Startpunkt und den gefundenen
        /// Punkt nicht zu groß ist, gehört dieser zur Linie und wird in der deque gespeichert
        if(found) {

            /// Falls zw. 2 Punkten extend() aufgerufen wird wird hier zusätzlich gecheckt ob die Grenzen eingehalten werden
            if(startPoint != NULL) {

                if((newpoint.up.pixel.y == startPoint->up.pixel.y) || (newpoint.up.pixel.y < startPoint->up.pixel.y)) {
                    //printf("Break: %i | %i \n", newpoint.up.pixel.y, startPoint->up.pixel.y);
                    break;
                }

            }
            //printf("gradient: %f | %f \n", start.up.non_disc_direction, newpoint.up.non_disc_direction);
            /// Checken ob der Winkel zwischen den beiden letzten Punkten zu groß ist
            /// (Es könnte sich z.B. um eine Start- od. Kreuzungslinie handeln)

            if(fabs(start.up.non_disc_direction - newpoint.up.non_disc_direction) > 25.0){
                found = false;
                newpoint.up.pixel.red();
                newpoint.down.pixel.red();
                //printf("\033[031m gradient: %f | %f \n \033[0m", start.up.non_disc_direction, newpoint.up.non_disc_direction);
                break;

            }


            //printf("\n Inserting : %i|%i\n", newpoint.up.pixel.x, newpoint.up.pixel.y);
            it = points.insert(it, newpoint);
            newpoint.up.pixel.yellow();

            if (!sign && it -1 != points.end()) {
                it++;
            }

            sumFound++;
        }

    } while(counter-- > 0 && found);

    /// Falls zw. 2 Punkten extend() aufgerufen wird muss der richtige iterator zurückgegeben werden.
    if (outIt != NULL) {
        *outIt = it + sumFound;
    }

    return sumFound;
    */
    return 0;

}

int Line::calcDistance(LinePoint &first, LinePoint &next) {

    int dx = first.low_high.x - next.low_high.x;
    int dy = first.low_high.y - next.low_high.y;

    return sqrt(dx*dx + dy*dy);

}

int Line::limitAngle(int dir) {
    while (dir > 360)
        dir = dir - 360;
    while (dir < 0)
        dir = 360 + dir;

    return dir;
}

bool Line::checkAngle(LinePoint &start, LinePoint &next) {

    float angle_start = 0.0;
    float angle_next = 0.0;

    float dx_start = abs(start.low_high.x - start.high_low.x);
    float dy_start = abs(start.low_high.y - start.high_low.y);

    float dx_next = abs(next.low_high.x - next.high_low.x);
    float dy_next = abs(next.low_high.y - next.high_low.y);

    if(dx_start == 0)
        angle_start = M_PI;
    if(dx_next == 0)
        angle_next = M_PI;

    angle_start = atan2(dy_start,dx_start);
    angle_next = atan2(dy_next, dx_next);

    float dangle = fabs(angle_next - angle_start);
    float dgradient = 0;//fabs(start.up.non_disc_direction - next.up.non_disc_direction);

    if(dangle > (70.0/180.0)*M_PI || dgradient > 60) {
        //printf("%f | %f \n", (dangle/M_PI)*180.0, dgradient);
        return false;
    }
    else
        return true;

}

} //namepsace find
} //namespace imaging
} //namespace lms
