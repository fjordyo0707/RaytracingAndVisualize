#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <vector>

struct SceneParameter{
    double angle;
    cv::Point3f eye, light;
    cv::Mat viewDir;//viewDir = cv::Mat(3,1,CV_32F);
    cv::Mat viewUp;//viewUp = cv::Mat(3,1,CV_32F);
    cv::Point3f outputImagePoint1, outputImagePoint2;
    cv::Point resolution;
};

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


class Scene{
public:
    Scene(SceneParameter&, std::vector<Sphere>&, std::vector<Triangle>&);
    void startRay( void );
    void setImage( void );
    cv::Mat getImage();
private:
    cv::Mat image;
    SceneParameter sceneInfo;
protected:
    std::vector<Sphere> sphereBuffer;
    std::vector<Triangle> triangleBuffer;
};

class Ray{
private:
    cv::Point3f rayOrigin;
    cv::Point3f rayVector;
    std::vector<Sphere> sphereBuffer;
    std::vector<Triangle> triangleBuffer;
public:
    bool isHit;
    Ray(cv::Point3f, cv::Point3f, bool, std::vector<Sphere>&, std::vector<Triangle>&);
    void hitSphere();
    void hitTriangle();
};