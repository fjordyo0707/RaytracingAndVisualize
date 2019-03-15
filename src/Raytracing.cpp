#include "Raytracing.h"
#include <math.h>

using std::cout;
using std::vector;
using namespace cv;

void Scene::setImage(){
    Mat img1(sceneInfo.resolution.y , sceneInfo.resolution.x, CV_8U);
    image = img1;
}

void Scene::setSceneInfo( SceneParameter& sceneI ){
   sceneInfo = sceneI;
}

void Scene::setSphereBuffer( vector<Sphere>& sphereB ){
   sphereBuffer = sphereB;
}

void Scene::setTriangleBuffer( vector<Triangle>& triangleB){
   triangleBuffer = triangleB;
}

Mat Scene::getImage(){
    return image;
}


void Scene::startRay(){
    for(int i = 0; i<sceneInfo.resolution.y; i++){
        for(int j = 0; j<sceneInfo.resolution.x; j++){
            Point3f vectorRay;
            vectorRay.x = sceneInfo.outputImagePoint1.x + (sceneInfo.outputImagePoint2.x - sceneInfo.outputImagePoint1.x)*j/sceneInfo.resolution.x - sceneInfo.eye.x;
            vectorRay.y = sceneInfo.outputImagePoint1.y + (sceneInfo.outputImagePoint2.y - sceneInfo.outputImagePoint1.y)*i/sceneInfo.resolution.y - sceneInfo.eye.y;
            vectorRay.z = sceneInfo.outputImagePoint1.z - sceneInfo.eye.z;

            Ray pixelRay(sceneInfo.eye, vectorRay, false);
            pixelRay.setSphereBuffer(sphereBuffer);
            pixelRay.setTriangleBuffer(triangleBuffer);
            pixelRay.hitSphere();
            if(pixelRay.isHit==true){
                image.at<uchar>(i,j) = 255;
                continue;
            }
            pixelRay.hitTriangle();
            if(pixelRay.isHit==true){
                image.at<uchar>(i,j) = 255;
            }
            else{
                image.at<uchar>(i,j) = 0;
            }
        }
    }
}

Ray::Ray(Point3f rayOrigin_, Point3f rayVector_, bool isHit_){
    rayOrigin = rayOrigin_;
    rayVector = rayVector_;
    isHit = isHit_;
}

void Ray::hitSphere(){
    for(int k = 0; k<sphereBuffer.size(); k++){
        float a = pow(rayVector.x,2) + pow(rayVector.y,2) + pow(rayVector.z,2);
        float b = 2*(rayVector.x*(rayOrigin.x - sphereBuffer[k].core.x) + rayVector.y*(rayOrigin.y - sphereBuffer[k].core.y) + 
            rayVector.z*(rayOrigin.z - sphereBuffer[k].core.z));
        float c = pow((rayOrigin.x - sphereBuffer[k].core.x), 2) + pow((rayOrigin.y - sphereBuffer[k].core.y),2) + 
        pow((rayOrigin.z - sphereBuffer[k].core.z),2) - pow(sphereBuffer[k].radius,2);
        float theta = pow(b,2)-4*a*c;
        float t1 = -b+sqrt(theta)/(2*a);
        float t2 = -b-sqrt(theta)/(2*a);

        if( theta >= 0 &&( t1 >=0 || t2 >=0)){
            isHit = true;
            break;
        }
    }
}

void Ray::hitTriangle(){
    for(int k = 0; k<triangleBuffer.size(); k++){
        Mat m(3,3,CV_32F);
        m.at<float>(0,0) = triangleBuffer[k].vertex2.x - triangleBuffer[k].vertex1.x;
        m.at<float>(0,1) = triangleBuffer[k].vertex3.x - triangleBuffer[k].vertex1.x;
        m.at<float>(0,2) = -rayVector.x;
        m.at<float>(1,0) = triangleBuffer[k].vertex2.y - triangleBuffer[k].vertex1.y;
        m.at<float>(1,1) = triangleBuffer[k].vertex3.y - triangleBuffer[k].vertex1.y;
        m.at<float>(1,2) = -rayVector.y;
        m.at<float>(2,0) = triangleBuffer[k].vertex2.z - triangleBuffer[k].vertex1.z;
        m.at<float>(2,1) = triangleBuffer[k].vertex3.z - triangleBuffer[k].vertex1.z;
        m.at<float>(2,2) = -rayVector.z;

        Mat left(3,1,CV_32F);
        left.at<float>(0,0) = rayOrigin.x - triangleBuffer[k].vertex1.x;
        left.at<float>(1,0) = rayOrigin.y - triangleBuffer[k].vertex1.y;
        left.at<float>(2,0) = rayOrigin.z - triangleBuffer[k].vertex1.z;

        if (determinant(m) != 0){
            Mat ans = m.inv()*left;
            if((ans.at<float>(0,0)>=0) && (ans.at<float>(1,0)>=0) && (ans.at<float>(0,0)+ans.at<float>(1,0)<=1) && ans.at<float>(2,0)>0){
                isHit = true;
                break;
            }
        }
    }
}
















