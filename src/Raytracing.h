#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <vector>

struct SceneParameter{
    cv::Point3f eye;
    cv::Point3f outputImagePoint1, outputImagePoint2;
    cv::Point resolution;
};

struct Sphere {
    cv::Point3f core;
    float radius;
};

struct Triangle {
    cv::Point3f vertex1, vertex2, vertex3;
};

class Scene{
public:
    void startRay( void );
    void setImage( void );
    void setSceneInfo( SceneParameter& );
    void setSphereBuffer( std::vector<Sphere>& );
    void setTriangleBuffer( std::vector<Triangle>& );
    cv::Mat getImage();
private:
    cv::Mat image;
    SceneParameter sceneInfo;
protected:
    std::vector<Sphere> sphereBuffer;
    std::vector<Triangle> triangleBuffer;
};

class Ray: public Scene{
private:
    cv::Point3f rayOrigin;
    cv::Point3f rayVector;
public:
    bool isHit;
    Ray(cv::Point3f, cv::Point3f, bool);
    void hitSphere();
    void hitTriangle();
};