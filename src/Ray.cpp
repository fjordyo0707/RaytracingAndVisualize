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

Ray::Ray(vector<Sphere>& sphereB, vector<Triangle>& triangleB, Point3f eye_, Point3f light_){
    Ray::sphereBuffer = sphereB;
    Ray::triangleBuffer = triangleB;
    Ray::eye = eye_;
    Ray::light = light_;
}
Ray::Ray(Point3f rayOrigin_, Point3f rayVector_, bool isHit_, bool init_){
    rayOrigin = rayOrigin_;
    rayVector = rayVector_;
    isHit = isHit_;
    myHitPoint.init = init_;
}

Mat Ray::hitSphereOrTriangle(){
    Mat RGBVector = Mat::zeros(1,3, CV_8UC1);
    myHitPoint.normalVector = Mat(3,1,CV_32F);
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
                normalize(myHitPoint.normalVector, myHitPoint.normalVector);
                myHitPoint.myMaterial = sphereBuffer[k].myMaterial;
            } else if(myHitPoint.tScale<myHitPoint.tScaleMin){
                myHitPoint.tScaleMin = myHitPoint.tScale;
                myHitPoint.thePoint.x = rayOrigin.x+ myHitPoint.tScaleMin*rayVector.x;
                myHitPoint.thePoint.y = rayOrigin.y+ myHitPoint.tScaleMin*rayVector.y;
                myHitPoint.thePoint.z = rayOrigin.z+ myHitPoint.tScaleMin*rayVector.z;
                myHitPoint.normalVector.at<float>(0,0) = myHitPoint.thePoint.x - sphereBuffer[k].core.x;
                myHitPoint.normalVector.at<float>(1,0) = myHitPoint.thePoint.y - sphereBuffer[k].core.y;
                myHitPoint.normalVector.at<float>(2,0) = myHitPoint.thePoint.z - sphereBuffer[k].core.z;
                normalize(myHitPoint.normalVector, myHitPoint.normalVector);
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
            if((ans.at<float>(0,0)>=0) && (ans.at<float>(1,0)>=0) && (ans.at<float>(0,0)+ans.at<float>(1,0)<=1) && t>0){
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
        cout<<myHitPoint.myMaterial.r<<" "<<myHitPoint.myMaterial.g<<" "<<myHitPoint.myMaterial.b<<endl;
        Vec3f lightVector(Ray::light.x - myHitPoint.thePoint.x, Ray::light.y - myHitPoint.thePoint.y,
                    Ray::light.z - myHitPoint.thePoint.z);
        Vec3f Ia(myHitPoint.myMaterial.r, myHitPoint.myMaterial.g, myHitPoint.myMaterial.b);
        Vec3f NdotL, LdotV;//= myHitPoint.normalVector.mul(lightVector);
        normalize(lightVector,lightVector);
        normalize(myHitPoint.normalVector,myHitPoint.normalVector);
        multiply(-1*myHitPoint.normalVector, lightVector, NdotL);
        float absNdotL = abs(NdotL[0]+NdotL[1]+NdotL[2]);
        /*
        float absNdotL = 0;
        if((NdotL[0]+NdotL[1]+NdotL[2])>0){
            absNdotL = NdotL[0]+NdotL[1]+NdotL[2];
        }*/
        Vec3f Id = absNdotL*Ia;

        Vec3f eyeVector(Ray::eye.x - myHitPoint.thePoint.x, Ray::eye.y - myHitPoint.thePoint.y, Ray::eye.z - myHitPoint.thePoint.z);
        normalize(eyeVector,eyeVector);
        multiply(eyeVector, lightVector, LdotV);
        float  proScale = abs(LdotV[0]+LdotV[1]+LdotV[2]);
        Vec3f normalhalfV = 0.5*(lightVector - proScale*eyeVector);
        Vec3f H = proScale*eyeVector + normalhalfV;
        Vec3f NdotH;
        multiply(myHitPoint.normalVector, H, NdotH);
        float absNdotH = abs(NdotH[0]+NdotH[1]+NdotH[2]);
        Vec3f Is = pow(absNdotH,myHitPoint.myMaterial.exp)*Ia;//pow(absNdotH,myHitPoint.myMaterial.exp)*Ia;absNdotH*myHitPoint.myMaterial.exp

        Vec3f thisRayRGB = myHitPoint.myMaterial.ka*Ia + myHitPoint.myMaterial.kd*Id + myHitPoint.myMaterial.ks*Is;
        cout<<"Ia "<<Ia<<endl;
        cout<<"Id "<<Id<<endl;
        cout<<"Is "<<Is<<endl;
        cout<<"NdotH "<<absNdotH<<endl;
        cout<<"exp "<<myHitPoint.myMaterial.exp<<endl;
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
        //cout<<"RGBVector"<<RGBVector<<endl<<endl;

    }
    return RGBVector;
}















