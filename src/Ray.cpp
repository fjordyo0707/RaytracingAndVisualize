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

vector<float> Ray::hitSphereOrTriangle(){
    vector<float> RGBVector(3, 0.0f);
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
        cout<<"Hit Point is "<<myHitPoint.thePoint<<endl;
        cout<<"NormalVector"<<myHitPoint.normalVector<<endl;
        cout<<"RGB is :"<<myHitPoint.myMaterial.r<<" "<<myHitPoint.myMaterial.g<<" "<<myHitPoint.myMaterial.b<<endl;
        cout<<"Eye place is : "<<Ray::eye<<endl;
        cout<<"Light place is : "<<Ray::light<<endl;
        Vec3f lightVector(myHitPoint.thePoint.x - Ray::light.x, myHitPoint.thePoint.y - Ray::light.y,
                    myHitPoint.thePoint.z - Ray::light.z);
        Vec3f Ia(myHitPoint.myMaterial.r, myHitPoint.myMaterial.g, myHitPoint.myMaterial.b);
        Vec3f NdotL;//= myHitPoint.normalVector.mul(lightVector);
        multiply(myHitPoint.normalVector, lightVector, NdotL);
        float absNdotL = NdotL[0]+NdotL[1]+NdotL[2];
        cout<<"lightVector : "<<lightVector<<endl;
        cout<<"absNdotL : "<<absNdotL<<endl<<endl;;
    }
    return RGBVector;
}















