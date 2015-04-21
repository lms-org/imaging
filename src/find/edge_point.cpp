#include <lms/imaging/find/edge_point.h>
//#include "image/image_factory.h"
#include <math.h>
#include <stdio.h>
#include <algorithm>

namespace lms{
namespace imaging{
namespace find{
bool EdgePoint::find(Pixel px, int length, int16_t dir, EdgePoint::edgetype searchtype, int threshold) {
    int dx;
    int dy;
    double cosDir = 0;
    double sinDir = 0;
    verified = false;

    cosDir = cos(dir * M_PI/180.);
    sinDir = sin(dir * M_PI/180.) ;

    this->pixel = px;

    if (cosDir == NAN || sinDir == NAN) {
        printf("Error: angle NAN!");
    }

    // Kleiner Hack am Rande: Folgender Fehler ist passiert: Wenn der Anfang der Suchlinie
    // mit einer Kante zusammenfällt war ohne die Initialisierung (nächsten 3 Zeilen) immer
    // eine low_high Kante entdeckt. Lösung: Initialisieren des lastPoints[1] mit
    // pstart - 1*Suchrichtung.
    //pixel.GaussianSurround();
    lastPoints[0] = pixel;
    lastPoints[1] = pixel;
    lastPoints[1].move(- (int)(cosDir+0.5), - (int)(sinDir+0.5));

    for(int i = 0; i < length; i++) {

        if(pixel.insideImage()) {
            pixel.GaussianSurround();
            pixel.sobel(&gradient, &direction, &non_disc_direction);
            //printf("Non_disc_direction: %f \n", non_disc_direction);
            //Confirm EdgePoint kann den Pixel verschieben, in richtung des Lokalen Maximums.
            //Daher wird die Position, im falle einer nicht erfolgreichen suche zurückgesetzt.
		//Braucht man vll nicht mehr
            Pixel tmp = pixel;
		//punkt ist maximum, also sobel gradient ist groß
            verified = confirmEdgePoint(threshold);

            if(i!=0)
                pixel.lightblue();
//            if(i==0 || i==1)
//                pixel.paint_cross();
		//set high-low oder low high kante
            checkType();

	
            if( (type == searchtype) && verified) {
                //printf("verified");
                pixel.yellow();
                return true;
            } else pixel = tmp;
//
//            else if((type == any) && point.verified) {
//                *edgePoint = point;
//                return;
//            }
        }

        dx = (int) ((i + 1) * cosDir + 0.5) - (int) (i * cosDir + 0.5);
        dy = (int) ((i + 1) * sinDir + 0.5) - (int) (i * sinDir + 0.5);

        pixel.move(dx, dy);
    }
    return false;

}

bool EdgePoint::confirmEdgePoint( int threshold) {



    if (gradient > threshold) {

//        switch (direction) {
//            case 0:
//            default:
//                nonMaximumSupression(0, 1);
//                break;
//            case 45:
//                nonMaximumSupression(1, 1);
//                break;
//            case 90:
//                nonMaximumSupression(1, 0);
//                break;
//            case 135:
//                nonMaximumSupression(1, -1);
//                break;
//        }
        return true;
    }

    else {
        //verified = false;
        return false;
    }

}

void EdgePoint::nonMaximumSupression(int rowShift, int colShift) {

    EdgePoint edgePoints[2] = {*this, *this};

    edgePoints[0].pixel.move(colShift, rowShift);

    if(!edgePoints[0].pixel.insideImage()) {

        edgePoints[0].pixel.x = 0;
        edgePoints[0].pixel.y = 0;
        edgePoints[0].gradient = 0;
        edgePoints[0].direction = -1;
    }
    else {
        ///TODO Herrgott du solltest mal kommentieren
        edgePoints[0].pixel.fastGaussian();
        edgePoints[0].pixel.sobel(&edgePoints[0].gradient, &edgePoints[0].direction, &edgePoints[0].non_disc_direction);
    }

    edgePoints[1].pixel.move(-colShift, -rowShift);

    if(!edgePoints[1].pixel.insideImage()) {
        edgePoints[1].pixel.x = 0;
        edgePoints[1].pixel.y = 0;
        edgePoints[1].gradient = 0;
        edgePoints[1].direction = -1;
    }
    else {

        edgePoints[1].pixel.fastGaussian();
        edgePoints[1].pixel.sobel(&edgePoints[1].gradient, &edgePoints[1].direction, &edgePoints[1].non_disc_direction);
    }

    //edgePoints[0].pixel.color_searched_local_maximum();
    //edgePoints[1].pixel.color_searched_local_maximum();

    int maxGradient = std::max(std::max(edgePoints[0].gradient, edgePoints[1].gradient), this->gradient);

    if(this->gradient == maxGradient) {

        //verified = true;
        //pixel.color_selected_local_maximum();

    }
    else {
    // Check ob Nachbarpixel in Richtung des Gradienten des aktuellen Pixels einen niedrigeren Grauwert haben
        for(int i = 0; i<2; i++) {

            //TODO: Johannes teste das da unten!
            if(edgePoints[i].gradient == maxGradient) {
                //edgePoints[i].verified = true;
                *this = edgePoints[i];
                //pixel.color_selected_local_maximum();
                break;

            }

        }
    }

}

//setzt high-low oder low-high kante
void EdgePoint::checkType() {

    int grayscale[3] = {};

    grayscale[0] = *(GAUSS + (IMAGE_WIDTH*lastPoints[0].y) + lastPoints[0].x);
    grayscale[1] = *(GAUSS + (IMAGE_WIDTH*lastPoints[1].y) + lastPoints[1].x);
    grayscale[2] = *(GAUSS + (IMAGE_WIDTH*pixel.y) + pixel.x);


    lastPoints[0] = lastPoints[1];
    lastPoints[1] = pixel;

    if (!verified) return;

    if(grayscale[2] == grayscale[1]) {

        if(grayscale[2] == grayscale[0]) {
            //printf("Johannes hat nicht recht: es gibt Planes!");
		//falls type == plane -> punkt wird verworfen (später ignoriert)
            type = plane;
        }
        else if(grayscale[2] > grayscale[0])
            type = low_high;
        else type = high_low;

    }

    else if(grayscale[2] > grayscale[1])
        type = low_high;
    else type = high_low;

}




} //namepsace find
} //namespace imaging
} //namespace lms
