#include "Scene.h"
#include <math.h>

using std::cout;
using std::endl;
using std::vector;
using namespace cv;

#define PI 3.14159265

Scene::Scene(SceneParameter& sceneI , vector<Sphere>& sphereB, vector<Triangle>& triangleB){
    sceneInfo = sceneI;
    sphereBuffer = sphereB;
    triangleBuffer = triangleB;
    Mat img1(sceneInfo.resolution.y , sceneInfo.resolution.x, CV_8U);
    image = img1;
    normalize(sceneInfo.viewDir, sceneInfo.viewDir);
    normalize(sceneInfo.viewUp, sceneInfo.viewUp);
    Mat right = sceneInfo.viewUp.cross(sceneInfo.viewDir);
    Mat up = sceneInfo.viewDir.cross(right);
    Point3f middle(sceneInfo.eye.x+sceneInfo.viewDir.at<float>(0,0),
                sceneInfo.eye.y+sceneInfo.viewDir.at<float>(1,0),
                sceneInfo.eye.z+sceneInfo.viewDir.at<float>(2,0));
    float x_scale = norm(sceneInfo.viewDir)*tan(sceneInfo.angle*PI/180.0);
    float y_scale = norm(sceneInfo.viewDir)*tan(sceneInfo.angle*PI/180.0);

    sceneInfo.outputImagePoint1.x = middle.x + (-1)*x_scale*right.at<float>(0,0)
                                    + (1)*x_scale*up.at<float>(0,0);
    sceneInfo.outputImagePoint1.y = middle.y + (-1)*x_scale*right.at<float>(1,0)
                                    + (1)*x_scale*up.at<float>(1,0);
    sceneInfo.outputImagePoint1.z = middle.z + (-1)*x_scale*right.at<float>(2,0)
                                    + (1)*x_scale*up.at<float>(2,0);

    sceneInfo.outputImagePoint2.x = middle.x + (-1)*y_scale*up.at<float>(0,0)
                                    + (1)*y_scale*right.at<float>(0,0);
    sceneInfo.outputImagePoint2.y = middle.y + (-1)*y_scale*up.at<float>(1,0)
                                    + (1)*y_scale*right.at<float>(1,0);
    sceneInfo.outputImagePoint2.z = middle.z + (-1)*y_scale*up.at<float>(2,0)
                                    + (1)*y_scale*right.at<float>(2,0);
}

Mat Scene::getImage(){
    return image;
}

void Scene::startRay(){
    Ray setArgumentRay(sphereBuffer, triangleBuffer);
    for(int i = 0; i<sceneInfo.resolution.y; i++){
        for(int j = 0; j<sceneInfo.resolution.x; j++){
            Point3f vectorRay;
            vectorRay.x = sceneInfo.outputImagePoint1.x +
                    (sceneInfo.outputImagePoint2.x - sceneInfo.outputImagePoint1.x)*j/sceneInfo.resolution.x - sceneInfo.eye.x;
            vectorRay.y = sceneInfo.outputImagePoint1.y + 
                    (sceneInfo.outputImagePoint2.y - sceneInfo.outputImagePoint1.y)*i/sceneInfo.resolution.y - sceneInfo.eye.y;
            vectorRay.z = sceneInfo.outputImagePoint1.z - sceneInfo.eye.z;

            Ray pixelRay(sceneInfo.eye, vectorRay, false);
            pixelRay.hitSphereOrTriangle();
            if(pixelRay.isHit==true){
                image.at<uchar>(i,j) = 255;
            }
            else{
                image.at<uchar>(i,j) = 0;
            }
        }
    }
}