#include <lms/imaging/find/pixel.h>
#include <lms/imaging/image_factory.h>
#include <math.h>

namespace lms{
namespace imaging{
namespace find{
void Pixel::paint(int color) {
#ifdef ENABLE_PAINT

    if( x > IMAGE_WIDTH-1 || y > IMAGE_HEIGHT-1) {
        printf("Bildkoordinaten verkackt!! Niemals painten auserhalb vom bild @%i|%i (Max: %i|%i: !\n", x, y, IMAGE_WIDTH, IMAGE_HEIGHT);
        //exit(1);
        return;
    }


    DEBUG[x + y * IMAGE_WIDTH] = color;
#endif
}

int Pixel::gauss() {
    return op::gaussPxlGrey(*getImage(),x,y);

}

void Pixel::GaussianSurround() {
}

void Pixel::sobel(int* gradient, int* direction, float* real_direction) {

    int valueSobelx = 0;
    int valueSobely = 0;

    // Berechnung des Sobel-Filters in x- und y-Richtung
    valueSobelx = sobelX();
    valueSobely = sobelY();

    // Berechnung der Stärke des Grauwertgradienten
    *gradient = calcGradientMag(valueSobelx, valueSobely);
    //printf("%i\t", *gradient);
    // Berechnung der Gradientenrichtung
    *direction = (int)calcGradientDir(valueSobelx, valueSobely, real_direction);
}

bool Pixel::insideImage() {
    if(x >= IMAGE_WIDTH)
        return false;
    if(y >= IMAGE_HEIGHT || y < 40)
        return false;
    /// Auto Bounding Box
    //if(x >= 91 && x <= 208 && y >= 121 && y <= 240)
    if(y >= 126) {
        int y1 = -3*x+450;
        int y2 = 3*x - 512;
        if(y >= y1  && y >= y2)
            return false;
    }
    return true;

}


bool Pixel::move(int dx, int dy) {

    x += dx;
    y += dy;

    if(x > IMAGE_WIDTH-1 || y > IMAGE_HEIGHT-1) {
        x -= dx;
        y -= dy;

        return false;
    }

    return true;

    // Pixel ist uint16_t, also immer positiv
    //    else if(x < 0 || y < 0) {
    //        x -= dx;
    //        y -= dy;
    //    }

}

int Pixel::sobelX() {

    //    int valueSobelX = 0;
    //    int x_index = 0;
    //    int y_index = 0;
    //
    //    const int kernelSobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    //
    //    // y-Achse durchlaufen
    //    for(int k = 0; k <= 2; k++) {
    //
    //        y_index = IMAGE_WIDTH*(reflectY(y + (k-1)));
    //
    //        // x-Achse durchlaufen
    //        for(int j = 0; j <= 2; j = j+2) {
    //
    //            x_index = reflectX(x + (j-1));
    //            valueSobelX += *(GAUSS + y_index + x_index)*kernelSobelX[k][j];
    //        }
    //    }
    //
    //    return valueSobelX;

    int valueSobelX = 0;
    int y_index = IMAGE_WIDTH * (reflectY(y + (0 - 1)));

    valueSobelX -= *(GAUSS + y_index + reflectX(x + (0-1)));
    valueSobelX += *(GAUSS + y_index + reflectX(x + (2-1)));

    y_index = IMAGE_WIDTH * (reflectY(y + (1 - 1)));

    valueSobelX -= *(GAUSS + y_index + reflectX(x + (0-1))) << 1;
    valueSobelX += *(GAUSS + y_index + reflectX(x + (2-1))) << 1;

    y_index = IMAGE_WIDTH * (reflectY(y + (2 - 1)));

    valueSobelX -= *(GAUSS + y_index + reflectX(x + (0-1)));
    valueSobelX += *(GAUSS + y_index + reflectX(x + (2-1)));

    return valueSobelX;
}

int Pixel::sobelY() {

    //    int valueSobelY = 0;
    //    int x_index = 0;
    //    int y_index = 0;
    //
    //    const int kernelSobelY[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    //
    //    // y-Achse durchlaufen
    //    for(int k = 0; k <= 2; k = k+2) {
    //
    //        y_index = IMAGE_WIDTH*(reflectY(y + (k-1)));
    //
    //        // x-Achse durchlaufen
    //        for(int j = 0; j <= 2; j++) {
    //
    //            x_index = reflectX(x + (j-1));
    //            valueSobelY += *(GAUSS + y_index + x_index)*kernelSobelY[k][j];
    //        }
    //
    //    }

    int valueSobelY = 0;
    int y_index = IMAGE_WIDTH * (reflectY(y + (0 - 1)));

    valueSobelY += *(GAUSS + y_index + reflectX(x + (0-1)));
    valueSobelY += *(GAUSS + y_index + reflectX(x + (1-1))) << 1;
    valueSobelY += *(GAUSS + y_index + reflectX(x + (2-1)));

    y_index = IMAGE_WIDTH * (reflectY(y + (2 - 1)));

    valueSobelY -= *(GAUSS + y_index + reflectX(x + (0-1)));
    valueSobelY -= *(GAUSS + y_index + reflectX(x + (1-1))) << 1;
    valueSobelY -= *(GAUSS + y_index + reflectX(x + (2-1)));

    return valueSobelY;

}

int Pixel::calcGradientMag(int valueSobelx, int valueSobely) {

    // Berechnung des Betrages des Gradientens (Näherungsformel)
    int sumsobel = fabs(valueSobelx) + fabs(valueSobely);
    // Grenzwerte für Grauwert
    if(sumsobel > 255) return 255;
    if(sumsobel < 0) return 0;
    return sumsobel;

}

int Pixel::calcGradientDir(int valueSobelx, int valueSobely, float* real_direction) {

    // Berechnung der Gradientenrichtung
    float angle = (atan2(valueSobelx,valueSobely)/M_PI)*180.0;

    // Nicht diskretisierter Wert des Winkels im Wertebereich von [0°; 360°]
    if(angle < 0)
        *real_direction = 360.0 + angle;
    else
        *real_direction = angle;

    // Gradientenrichtung wird auf 0°, 45°, 90° und 135° gerundet (waagrecht, diagonal, senkrecht, diagonal)
    if ( ( (angle < 22.5) && (angle > -22.5) ) || (angle > 157.5) || (angle < -157.5) )
        return 0;
    else if ( ( (angle > 22.5) && (angle < 67.5) ) || ( (angle < -112.5) && (angle > -157.5) ) )
        return 45;
    else if ( ( (angle > 67.5) && (angle < 112.5) ) || ( (angle < -67.5) && (angle > -112.5) ) )
        return 90;
    else if ( ( (angle > 112.5) && (angle < 157.5) ) || ( (angle < -22.5) && (angle > -67.5) ) )
        return 135;
    else return 0;
}

int Pixel::reflectX(int px) {

    if(px < 0)
    {
        //printf("KLeiner Null");
        return - px - 1;
    }

    else if(px >= IMAGE_WIDTH)
    {
        //printf("Größer size");
        return 2*IMAGE_WIDTH - px - 1;

    }

    return px;

}

int Pixel::reflectY(int py) {

    if(py < 0)
    {
        //printf("KLeiner Null");
        return - py - 1;
    }

    else if(py >= IMAGE_HEIGHT)
    {
        //printf("Größer size");
        return 2*IMAGE_HEIGHT - py - 1;

    }

    return py;

}


void Pixel::paint_cross()
{
    if(DEBUG_PAINT)
    {
        move(2,2);
        if(insideImage())
            blue();
        move(-1,-1);
        if(insideImage())
            blue();
        move(-1,-1);
        if(insideImage())
            purple();
        move(-1,-1);
        if(insideImage())
            blue();
        move(-1,-1);
        if(insideImage())
            blue();
        move(0,4);
        if(insideImage())
            blue();
        move(1,-1);
        if(insideImage())
            blue();
        move(2,-2);
        if(insideImage())
            blue();
        move(1,-1);
        if(insideImage())
            blue();
        move(-1,1);
        move(-1,1);
    }

}


} //namepsace find
} //namespace imaging
} //namespace lms
