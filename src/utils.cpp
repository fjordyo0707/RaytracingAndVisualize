#include "utils.h"
using namespace cv;
float myDot(Vec3f a, Vec3f b){
    Vec3f c;
    normalize(a,a);
    normalize(b,b);
    multiply(a, b, c);
    float ouptut = c[0]+c[1]+c[2];
    return ouptut;
}