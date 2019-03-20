#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <string>

struct Material{
    double r, g, b, ka, kd, ks, exp, reflect;
};

struct Sphere {
    cv::Point3f core;
    float radius;
    Material myMaterial;
};

struct Triangle {
    cv::Point3f vertex1, vertex2, vertex3;
    Material myMaterial;
};

struct HitPoint{
    bool init;
    float tScale, tScaleMin;
    cv::Point3f thePoint;
    cv::Mat normalVector;
    Material myMaterial;
};


class Ray{
private:
    cv::Point3f rayOrigin;
    cv::Point3f rayVector;
    HitPoint myHitPoint;
    static cv::Point3f eye;
    static cv::Point3f light;
    static std::vector<Sphere> sphereBuffer;
    static std::vector<Triangle> triangleBuffer;
public:
    Ray(std::vector<Sphere>&, std::vector<Triangle>&, cv::Point3f, cv::Point3f);
    Ray(cv::Point3f, cv::Point3f, bool, bool);
    cv::Mat hitSphereOrTriangle();
    bool isHit;
};