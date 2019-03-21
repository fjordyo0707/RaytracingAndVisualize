#include "Ray.h"
#include <math.h>

using std::cout;
using std::endl;
using std::vector;
using namespace cv;

vector<Sphere> Ray::sphereBuffer;
vector<Triangle> Ray::triangleBuffer;
cv::Point3f Ray::eye;
cv::Point3f Ray::light;
float myDot(Vec3f, Vec3f);

Ray::Ray(vector<Sphere>& sphereB, vector<Triangle>& triangleB, Point3f eye_, Point3f light_){
    Ray::sphereBuffer = sphereB;
    Ray::triangleBuffer = triangleB;
    Ray::eye = eye_;
    Ray::light = light_;
}
Ray::Ray(Point3f rayOrigin_, Point3f rayVector_, bool isHit_, bool init_, bool isShadow_){
    rayOrigin = rayOrigin_;
    rayVector = rayVector_;
    isHit = isHit_;
    myHitPoint.init = init_;
    isShadow = isShadow_;
}

Mat Ray::phongShading(){
    Mat RGBVector = Mat::zeros(1,3, CV_8UC1);
    myHitPoint.normalVector = Mat(3,1,CV_32F);
    for(int k = 0; k<sphereBuffer.size(); k++){
        float a = pow(rayVector.x,2) + pow(rayVector.y,2) + pow(rayVector.z,2);
        float b = 2*(rayVector.x*(rayOrigin.x - sphereBuffer[k].core.x) + rayVector.y*(rayOrigin.y - sphereBuffer[k].core.y) + 
            rayVector.z*(rayOrigin.z - sphereBuffer[k].core.z));
        float c = pow((rayOrigin.x - sphereBuffer[k].core.x), 2) + pow((rayOrigin.y - sphereBuffer[k].core.y),2) + 
        pow((rayOrigin.z - sphereBuffer[k].core.z),2) - pow(sphereBuffer[k].radius,2);
        float theta = pow(b,2)-4*a*c;
        float t1 = (-b+sqrt(theta))/(2*a);
        float t2 = (-b-sqrt(theta))/(2*a);

        if( theta >= 0 &&( t1 >0.001 || t2 >0.001)){
            isHit = true;
            if( t1<=t2 ){
                myHitPoint.tScale = t1;
            } else{
                myHitPoint.tScale = t2;
            }

            if( myHitPoint.init == false){
                myHitPoint.tScaleMin = myHitPoint.tScale;
                myHitPoint.init = true;             
                myHitPoint.thePoint.x = rayOrigin.x+ myHitPoint.tScaleMin*rayVector.x;
                myHitPoint.thePoint.y = rayOrigin.y+ myHitPoint.tScaleMin*rayVector.y;
                myHitPoint.thePoint.z = rayOrigin.z+ myHitPoint.tScaleMin*rayVector.z;
                myHitPoint.normalVector.at<float>(0,0) = myHitPoint.thePoint.x - sphereBuffer[k].core.x;
                myHitPoint.normalVector.at<float>(1,0) = myHitPoint.thePoint.y - sphereBuffer[k].core.y;
                myHitPoint.normalVector.at<float>(2,0) = myHitPoint.thePoint.z - sphereBuffer[k].core.z;
                myHitPoint.myMaterial = sphereBuffer[k].myMaterial;
            } else if(myHitPoint.tScale<myHitPoint.tScaleMin){
                myHitPoint.tScaleMin = myHitPoint.tScale;
                myHitPoint.thePoint.x = rayOrigin.x+ myHitPoint.tScaleMin*rayVector.x;
                myHitPoint.thePoint.y = rayOrigin.y+ myHitPoint.tScaleMin*rayVector.y;
                myHitPoint.thePoint.z = rayOrigin.z+ myHitPoint.tScaleMin*rayVector.z;
                myHitPoint.normalVector.at<float>(0,0) = myHitPoint.thePoint.x - sphereBuffer[k].core.x;
                myHitPoint.normalVector.at<float>(1,0) = myHitPoint.thePoint.y - sphereBuffer[k].core.y;
                myHitPoint.normalVector.at<float>(2,0) = myHitPoint.thePoint.z - sphereBuffer[k].core.z;
                myHitPoint.myMaterial = sphereBuffer[k].myMaterial;
            }
        }
    }

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
            float t = ans.at<float>(2,0);
            if((ans.at<float>(0,0)>=0) && (ans.at<float>(1,0)>=0) && (ans.at<float>(0,0)+ans.at<float>(1,0)<=1) && t>0.001){
                isHit = true;
                myHitPoint.tScale = t;
                if( myHitPoint.init == false){
                    myHitPoint.tScaleMin = myHitPoint.tScale;
                    myHitPoint.init = true;
                    myHitPoint.thePoint.x = rayOrigin.x+ myHitPoint.tScaleMin*rayVector.x;
                    myHitPoint.thePoint.y = rayOrigin.y+ myHitPoint.tScaleMin*rayVector.y;
                    myHitPoint.thePoint.z = rayOrigin.z+ myHitPoint.tScaleMin*rayVector.z;
                    Mat v1(3,1,CV_32F);
                    Mat v2(3,1,CV_32F);
                    v1.at<float>(0,0) = m.at<float>(0,0);
                    v1.at<float>(1,0) = m.at<float>(1,0);
                    v1.at<float>(2,0) = m.at<float>(2,0);
                    v2.at<float>(0,0) = m.at<float>(0,1);
                    v2.at<float>(1,0) = m.at<float>(1,1);
                    v2.at<float>(2,0) = m.at<float>(2,1);                   
                    myHitPoint.normalVector = v1.cross(v2);
                    myHitPoint.myMaterial = triangleBuffer[k].myMaterial;
                } else if(myHitPoint.tScale<myHitPoint.tScaleMin){
                    myHitPoint.tScaleMin = myHitPoint.tScale;
                    Mat v1(3,1,CV_32F);
                    Mat v2(3,1,CV_32F);
                    myHitPoint.thePoint.x = rayOrigin.x+ myHitPoint.tScaleMin*rayVector.x;
                    myHitPoint.thePoint.y = rayOrigin.y+ myHitPoint.tScaleMin*rayVector.y;
                    myHitPoint.thePoint.z = rayOrigin.z+ myHitPoint.tScaleMin*rayVector.z;
                    v1.at<float>(0,0) = m.at<float>(0,0);
                    v1.at<float>(1,0) = m.at<float>(1,0);
                    v1.at<float>(2,0) = m.at<float>(2,0);
                    v2.at<float>(0,0) = m.at<float>(0,1);
                    v2.at<float>(1,0) = m.at<float>(1,1);
                    v2.at<float>(2,0) = m.at<float>(2,1);
                    myHitPoint.normalVector = v1.cross(v2);
                    myHitPoint.myMaterial = triangleBuffer[k].myMaterial;
                }
            }
        }
    }
    if(isHit==true){
        Vec3f lightVector(Ray::light.x - myHitPoint.thePoint.x, Ray::light.y - myHitPoint.thePoint.y,
                    Ray::light.z - myHitPoint.thePoint.z);
        Vec3f Ia(myHitPoint.myMaterial.r, myHitPoint.myMaterial.g, myHitPoint.myMaterial.b);
        Vec3f eyeVector(Ray::eye.x - myHitPoint.thePoint.x, Ray::eye.y - myHitPoint.thePoint.y, Ray::eye.z - myHitPoint.thePoint.z);
        Vec3f Id, H, Is;
        Vec3f thisRayRGB;
        Vec3f vecNormal((float*)myHitPoint.normalVector.data);
        checkShadow();   
        float absNdotL, absNdotH;
        normalize(eyeVector,eyeVector);
        normalize(lightVector,lightVector);
        normalize(vecNormal,vecNormal); 
        if(isShadow==true){
            Id = 0;
            Is = 0;
        } else{
            absNdotL = abs(myDot(vecNormal, lightVector));
            Id = absNdotL*Ia;
            H = lightVector+eyeVector;
            absNdotH = abs(myDot(vecNormal, H));
            Is = pow(absNdotH,myHitPoint.myMaterial.exp)*Ia;
        }

        Vec3f rayVectorforRec(rayVector.x, rayVector.y, rayVector.z);
        normalize(rayVectorforRec, rayVectorforRec); 
        float rayProjectToNormalScale = myDot(vecNormal, rayVectorforRec);
        Vec3f rayProjectToNormal = -1*(rayProjectToNormalScale*vecNormal);
        Vec3f reflectEyeVector = -1*rayVectorforRec+ 2*(rayProjectToNormal+rayVectorforRec);
        Point3f tempRay(reflectEyeVector[0], reflectEyeVector[1], reflectEyeVector[2]);
        Ray recursiveRay(myHitPoint.thePoint, tempRay, false, false, false);
        Mat recursiveRGBMat =  recursiveRay.phongShading();

        Vec3f recursiveRGB(recursiveRGBMat.at<uchar>(0,0)/255.0, recursiveRGBMat.at<uchar>(0,1)/255.0, recursiveRGBMat.at<uchar>(0,2)/255.0);
        thisRayRGB = (1- myHitPoint.myMaterial.reflect)*(myHitPoint.myMaterial.ka*Ia + myHitPoint.myMaterial.kd*Id + myHitPoint.myMaterial.ks*Is)
                        + myHitPoint.myMaterial.reflect*recursiveRGB;
        if(thisRayRGB[0]>1.0){
            thisRayRGB[0] = 1.0f;
        }
        if(thisRayRGB[1]>1.0){
            thisRayRGB[1] = 1.0f;
        }
        if(thisRayRGB[2]>1.0){
            thisRayRGB[2]=1.0f;
        }
        RGBVector.at<uchar>(0,0) = int(thisRayRGB[0]*255);
        RGBVector.at<uchar>(0,1) = int(thisRayRGB[1]*255);
        RGBVector.at<uchar>(0,2) = int(thisRayRGB[2]*255);
    }
    return RGBVector;
}

void Ray::checkShadow(){

    Point3f pointToLight(Ray::light.x - myHitPoint.thePoint.x, Ray::light.y - myHitPoint.thePoint.y,
                    Ray::light.z - myHitPoint.thePoint.z);
    Point3f ckPoint =  myHitPoint.thePoint;

    for(int k = 0; k<sphereBuffer.size(); k++){
        float a = pow(pointToLight.x,2) + pow(pointToLight.y,2) + pow(pointToLight.z,2);
        float b = 2*(pointToLight.x*(ckPoint.x - sphereBuffer[k].core.x) + pointToLight.y*(ckPoint.y - sphereBuffer[k].core.y) + 
            pointToLight.z*(ckPoint.z - sphereBuffer[k].core.z));
        float c = pow((ckPoint.x - sphereBuffer[k].core.x), 2) + pow((ckPoint.y - sphereBuffer[k].core.y),2) + 
        pow((ckPoint.z - sphereBuffer[k].core.z),2) - pow(sphereBuffer[k].radius,2);
        float theta = pow(b,2)-4*a*c;
        float t1 = (-b+sqrt(theta))/(2*a);
        float t2 = (-b-sqrt(theta))/(2*a);

        if( theta >= 0 &&( t1 >0.001 || t2 >0.001)){
            isShadow = true;
            break;
        }
    }

    for(int k = 0; k<triangleBuffer.size(); k++){
        if(isShadow==true){
            break;
        }
        Mat m(3,3,CV_32F);
        m.at<float>(0,0) = triangleBuffer[k].vertex2.x - triangleBuffer[k].vertex1.x;
        m.at<float>(0,1) = triangleBuffer[k].vertex3.x - triangleBuffer[k].vertex1.x;
        m.at<float>(0,2) = -pointToLight.x;
        m.at<float>(1,0) = triangleBuffer[k].vertex2.y - triangleBuffer[k].vertex1.y;
        m.at<float>(1,1) = triangleBuffer[k].vertex3.y - triangleBuffer[k].vertex1.y;
        m.at<float>(1,2) = -pointToLight.y;
        m.at<float>(2,0) = triangleBuffer[k].vertex2.z - triangleBuffer[k].vertex1.z;
        m.at<float>(2,1) = triangleBuffer[k].vertex3.z - triangleBuffer[k].vertex1.z;
        m.at<float>(2,2) = -pointToLight.z;

        Mat left(3,1,CV_32F);
        left.at<float>(0,0) = ckPoint.x - triangleBuffer[k].vertex1.x;
        left.at<float>(1,0) = ckPoint.y - triangleBuffer[k].vertex1.y;
        left.at<float>(2,0) = ckPoint.z - triangleBuffer[k].vertex1.z;

        if (determinant(m) != 0){
            Mat ans = m.inv()*left;
            float t = ans.at<float>(2,0);
            if((ans.at<float>(0,0)>=0) && (ans.at<float>(1,0)>=0) && (ans.at<float>(0,0)+ans.at<float>(1,0)<=1) && t>0.001){
                isShadow = true;
                break;
            }
        }
    }

}

float myDot(Vec3f a, Vec3f b){
    Vec3f c;
    normalize(a,a);
    normalize(b,b);
    multiply(a, b, c);
    float ouptut = c[0]+c[1]+c[2];
    return ouptut;
}