#include <math.h>

#include "lms/imaging/magic/n2d.h"
#include "lms/imaging/magic/lutx.h"
#include "lms/imaging/magic/luty.h"
#include "lms/imaging/magic/cali_ab.h"

#include "lms/imaging/warp.h"

namespace lms {
namespace imaging {

#define CALI_WIDTH 320
#define CALI_HEIGHT 240

bool C2V(const vertex2i* lp, vertex2f* rp, float *angle_out) {
    float xtemp, ytemp, dxdx, dydy, dxdy, dydx;

    int x = (*lp)[0];
    int y = (*lp)[1];

    //Quentin LUT: Umrechnung in ein unverzerrtes Zentralprojektionsbild.
    ///TODO: Image Size
    if (x >= 0 && y >= 0 && x < CALI_WIDTH - 1 && y < CALI_HEIGHT - 1) {
        xtemp = d2nX[(int) x][(int) y];
        ytemp = d2nY[(int) x][(int) y];

        dxdx = d2nX[(int) x + 1][(int) y] - xtemp;
        dxdy = d2nX[(int) x][(int) y + 1] - xtemp;
        dydx = d2nY[(int) x + 1][(int) y] - ytemp;
        dydy = d2nY[(int) x][(int) y + 1] - ytemp;

    } else {
        return false;
    }

    //Felix: Umrechnung des unverzerrten bilds in Auto/Straßenkoordinaten.
    float a = xtemp * tb2a[0] + ytemp * tb2a[1] + tb2a[2];
    float b = xtemp * tb2a[3] + ytemp * tb2a[4] + tb2a[5];
    float c = xtemp * tb2a[6] + ytemp * tb2a[7] + tb2a[8];
    (*rp)[0] = (a / c);  ///To M
    (*rp)[1] = (b / c);  ///To M

    if (angle_out != nullptr) {
        float phi = LP_Angle_to_rad(*angle_out);
        float cphi = cos(phi);
        float sphi = sin(phi);
        float dxtemp = dxdx * cphi + dxdy*sphi;
        float dytemp = dydx * cphi + dydy*sphi;
        float dx = ((tb2a[0] * c - a * tb2a[6]) * dxtemp + (tb2a[1] * c - a * tb2a[7]) * dytemp) / (c * c);
        float dy = ((tb2a[3] * c - b * tb2a[6]) * dxtemp + (tb2a[4] * c - b * tb2a[7]) * dytemp) / (c * c);
        *angle_out = atan2(dy, dx);
    }
    return true;
}

bool V2C(const vertex2f* rp, vertex2i* px, float *direction) {

    //Felix: Umrechnung der Autokoordinaten ins unverzerrte Bild
    float x = (float)(*rp)[0];
    float y = (float)(*rp)[1];
    float a = x * ta2b[0] + y * ta2b[1] + ta2b[2];
    float b = x * ta2b[3] + y * ta2b[4] + ta2b[5];
    float c = x * ta2b[6] + y * ta2b[7] + ta2b[8];
    x = a / c;
    y = b / c;
    const float xNorm = x, yNorm = y;
    n2d(xNorm, yNorm, x, y);
    (*px)[0] = (int16_t)x;
    (*px)[1] = (int16_t)y;

    if(direction != nullptr) {
        float phi = *direction*(M_PI/(180.));
        float dx = ((ta2b[0] * c - a * ta2b[6]) * cos(phi)+(ta2b[1] * c - a * ta2b[7]) * sin(phi)) / (c * c);
        float dy = ((ta2b[3] * c - b * ta2b[6]) * cos(phi)+(ta2b[4] * c - b * ta2b[7]) * sin(phi)) / (c * c);
        phi = atan2(dy, dx);

        //Quentin Funktion: Umrechnung von unverzerrten Bilkoordinaten in verzerrte Bildkoordinaten.
        const float xNormPdx = xNorm + cos(phi), yNormPdy = yNorm + sin(phi);
        n2d(xNormPdx, yNormPdy, dx, dy);
        phi = atan2(dy - (y), dx - (x));
        *direction = ((180.)/M_PI)*phi;
    }

    return true;
}

float LP_Angle_to_rad(int8_t dir) {
    return (M_PI/(180.))*dir;
}

int8_t rad_to_angle_lp(float r) {
    return ((180.)/M_PI) * r;
}

bool n2d(const float & xn, const float & yn, float & xdist, float & ydist) {
    const float xnorm =(float)(xn-Cx)/Fx, ynorm = (float)(yn-Cy)/Fy;
    float r2 = xnorm*xnorm + ynorm*ynorm;
    float dist = 1 + K1*r2 + K2*r2*r2;
    float dx = 2*K3*xnorm*ynorm + K4*(r2+xnorm*xnorm);
    float dy = K3*(r2+ynorm*ynorm) + 2*K4*xnorm*ynorm;

    // Distortion
    xdist = dist*xnorm + dx;
    ydist = dist*ynorm + dy;

    // Camera position
    xdist = Fx*xdist + Cx;
    ydist = Fy*ydist + Cy;

    return true;
}

void imageV2C(const Image &input, Image &output) {
    if(input.width() != 320 || input.height() != 240 ||
            input.format() != Format::GREY) {
        //return false;
    }

    const int textureSize = 512;
    const int planeSize = 5;
    output.resize(textureSize, textureSize, Format::GREY);

    vertex2f in;
    vertex2i out;
    bool success;
    std::uint8_t color;

    for(int y = 0; y < textureSize; y++) {
        for(int x = 0; x < textureSize; x++) {
            in[0] = planeSize / (float)textureSize * (float) x;
            in[1] = -planeSize / (float)textureSize
                    * (float)(y - (float)textureSize / 2.);

            success = V2C(&in, &out);

            //std::cout << x << " " << y << " -> " << out[0] << " " << out[1] << std::endl;

            if(success && out[0] >= 0 && out[0] < input.width()
                    && out[1] >= 0 && out[1] < input.height()) {
                color = *(input.data() + out[1] * input.width() + out[0]);
            } else {
                color = 127;
            }
            *(output.data() + y * output.width() + x) = color;
        }
    }

    //return true;
}

}  // namespace imaging
}  // namespace lms
