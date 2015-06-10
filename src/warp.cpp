#include <math.h>

#include "lms/imaging/warp.h"
#include "lms/math/vertex.h"
#include "lms/type/module_config.h"
#include "lms/framework.h"

namespace lms {
namespace imaging {

//TODO Warum kann man die beiden nicht in die Header verschieben?
WarpContent defaultContent;
bool defaultSet = false;

void setDefaultWarpContent(){
    defaultSet = true;
    defaultContent.fromConfigDirectory();
}

bool C2V(const lms::math::vertex2i* lp, lms::math::vertex2f* rp, float *angle_out) {
    if(!defaultSet){
        setDefaultWarpContent();
    }
    float xtemp, ytemp, dxdx, dydy, dxdy, dydx;

    int x = lp->x;
    int y = lp->y;

    //Quentin LUT: Umrechnung in ein unverzerrtes Zentralprojektionsbild.
    ///TODO: Image Size
    if (x >= 0 && y >= 0 && x < defaultContent.CALI_WIDTH - 1 && y < defaultContent.CALI_HEIGHT - 1) {
        xtemp = defaultContent.d2nX[x*defaultContent.CALI_WIDTH+y];
        ytemp = defaultContent.d2nY[x*defaultContent.CALI_WIDTH+y];

        dxdx = defaultContent.d2nX[(x + 1)*defaultContent.CALI_WIDTH+y] - xtemp;
        dxdy = defaultContent.d2nX[x*defaultContent.CALI_WIDTH+y + 1] - xtemp;
        dydx = defaultContent.d2nY[(x + 1)*defaultContent.CALI_WIDTH+y] - ytemp;
        dydy = defaultContent.d2nY[x*defaultContent.CALI_WIDTH+ y + 1] - ytemp;

    } else {
        return false;
    }

    //Felix: Umrechnung des unverzerrten bilds in Auto/Straßenkoordinaten.
    float a = xtemp * defaultContent.tb2a[0] + ytemp * defaultContent.tb2a[1] + defaultContent.tb2a[2];
    float b = xtemp * defaultContent.tb2a[3] + ytemp * defaultContent.tb2a[4] + defaultContent.tb2a[5];
    float c = xtemp * defaultContent.tb2a[6] + ytemp * defaultContent.tb2a[7] + defaultContent.tb2a[8];
    rp->x = (a / c);  ///To M
    rp->y = (b / c);  ///To M

    if (angle_out != nullptr) {
        float phi = LP_Angle_to_rad(*angle_out);
        float cphi = cos(phi);
        float sphi = sin(phi);
        float dxtemp = dxdx * cphi + dxdy*sphi;
        float dytemp = dydx * cphi + dydy*sphi;
        float dx = ((defaultContent.tb2a[0] * c - a * defaultContent.tb2a[6]) * dxtemp + (defaultContent.tb2a[1] * c - a * defaultContent.tb2a[7]) * dytemp) / (c * c);
        float dy = ((defaultContent.tb2a[3] * c - b * defaultContent.tb2a[6]) * dxtemp + (defaultContent.tb2a[4] * c - b * defaultContent.tb2a[7]) * dytemp) / (c * c);
        *angle_out = atan2(dy, dx);
    }
    return true;
}

bool V2C(const lms::math::vertex2f* rp, lms::math::vertex2i* px, float *direction) {
    if(!defaultSet){
        setDefaultWarpContent();
    }
    //Felix: Umrechnung der Autokoordinaten ins unverzerrte Bild
    float x = (float)rp->x;
    float y = (float)rp->y;
    float a = x * defaultContent.ta2b[0] + y * defaultContent.ta2b[1] + defaultContent.ta2b[2];
    float b = x * defaultContent.ta2b[3] + y * defaultContent.ta2b[4] + defaultContent.ta2b[5];
    float c = x * defaultContent.ta2b[6] + y * defaultContent.ta2b[7] + defaultContent.ta2b[8];
    x = a / c;
    y = b / c;
    const float xNorm = x, yNorm = y;
    n2d(xNorm, yNorm, x, y);
    px->x = (int16_t)x;
    px->y = (int16_t)y;

    if(direction != nullptr) {
        float phi = *direction*(M_PI/(180.));
        float dx = ((defaultContent.ta2b[0] * c - a * defaultContent.ta2b[6]) * cos(phi)+(defaultContent.ta2b[1] * c - a * defaultContent.ta2b[7]) * sin(phi)) / (c * c);
        float dy = ((defaultContent.ta2b[3] * c - b * defaultContent.ta2b[6]) * cos(phi)+(defaultContent.ta2b[4] * c - b * defaultContent.ta2b[7]) * sin(phi)) / (c * c);
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
    if(!defaultSet){
        setDefaultWarpContent();
    }
    const float xnorm =(float)(xn-defaultContent.Cx)/defaultContent.Fx, ynorm = (float)(yn-defaultContent.Cy)/defaultContent.Fy;
    float r2 = xnorm*xnorm + ynorm*ynorm;
    float dist = 1 + defaultContent.K1*r2 + defaultContent.K2*r2*r2;
    float dx = 2*defaultContent.K3*xnorm*ynorm + defaultContent.K4*(r2+xnorm*xnorm);
    float dy = defaultContent.K3*(r2+ynorm*ynorm) + 2*defaultContent.K4*xnorm*ynorm;

    // Distortion
    xdist = dist*xnorm + dx;
    ydist = dist*ynorm + dy;

    // Camera position
    xdist = defaultContent.Fx*xdist + defaultContent.Cx;
    ydist = defaultContent.Fy*ydist + defaultContent.Cy;

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

    lms::math::vertex2f in;
    lms::math::vertex2i out;
    bool success;
    std::uint8_t color;

    for(int y = 0; y < textureSize; y++) {
        for(int x = 0; x < textureSize; x++) {
            in.x = planeSize / (float)textureSize * (float) x;
            in.y = -planeSize / (float)textureSize
                    * (float)(y - (float)textureSize / 2.);

            success = V2C(&in, &out);

            //std::cout << x << " " << y << " -> " << out[0] << " " << out[1] << std::endl;

            if(success && out.x >= 0 && out.x < input.width()
                    && out.y >= 0 && out.y < input.height()) {
                color = *(input.data() + out.y * input.width() + out.x);
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
