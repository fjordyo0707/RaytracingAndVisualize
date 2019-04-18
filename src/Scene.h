#include "Ray.h"

struct SceneParameter{
    double angle;
    cv::Point3f eye, light;
    cv::Mat viewDir;
    cv::Mat viewUp;
    cv::Point3f outputImagePoint1, outputImagePoint2;
    cv::Point resolution;
    Sphere bulb; 
};

class Scene{
public:
    Scene(SceneParameter&, std::vector<Sphere>&, std::vector<Triangle>&);
    void startRay( void );
    void setImage( void );
    void printInfo( void );
    cv::Mat getHitImage();
    cv::Mat getColorImage();
private:
    cv::Mat image;
    cv::Mat colorImage;
    SceneParameter sceneInfo;
protected:
    std::vector<Sphere> sphereBuffer;
    std::vector<Triangle> triangleBuffer;
};
