#include "Ray.h"

struct SceneParameter{
    double angle;
    cv::Point3f eye, light;
    cv::Mat viewDir;
    cv::Mat viewUp;
    cv::Point3f outputImagePoint1, outputImagePoint2;
    cv::Point resolution;
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
