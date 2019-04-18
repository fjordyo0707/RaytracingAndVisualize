#include "Ray.h"
#include "utils.h"
#include <math.h>
#include <random>

using std::cout;
using std::endl;
using std::vector;
using namespace cv;

vector<Sphere> Ray::sphereBuffer;
vector<Triangle> Ray::triangleBuffer;
cv::Point3f Ray::eye;
cv::Point3f Ray::light;
float Ray::lightRadius;

Ray::Ray(vector<Sphere>& sphereB, vector<Triangle>& triangleB, Point3f eye_, Point3f light_, float lightR){
    Ray::sphereBuffer = sphereB;
    Ray::triangleBuffer = triangleB;
    Ray::eye = eye_;
    Ray::light = light_;
    Ray::lightRadius = lightR;
}
Ray::Ray(Point3f rayOrigin_, Point3f rayVector_, bool isHit_, bool init_, bool isShadow_, bool isRecursiveRay_){
    rayOrigin = rayOrigin_;
    rayVector = rayVector_;
    isHit = isHit_;
    myHitPoint.init = init_;
    isShadow = isShadow_;
    isRecursiveRay = isRecursiveRay_;
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
        float absNdotL, absNdotH;
        normalize(eyeVector,eyeVector);
        normalize(lightVector,lightVector);
        normalize(vecNormal,vecNormal); 
        

        Vec3f rayVectorforRec(rayVector.x, rayVector.y, rayVector.z);
        normalize(rayVectorforRec, rayVectorforRec); 
        float rayProjectToNormalScale = myDot(vecNormal, rayVectorforRec);
        Vec3f rayProjectToNormal = -1*(rayProjectToNormalScale*vecNormal);
        Vec3f reflectEyeVector = -1*rayVectorforRec+ 2*(rayProjectToNormal+rayVectorforRec);
        Point3f tempRay(reflectEyeVector[0], reflectEyeVector[1], reflectEyeVector[2]);
        Ray recursiveRay(myHitPoint.thePoint, tempRay, false, false, false, true);
        Mat recursiveRGBMat =  recursiveRay.phongShading();

        isShadow = checkShadow(Ray::light);
        float shadowRatio = 1;
        if(isShadow==true && isRecursiveRay==true){
            Id = 0;
            Is = 0;
        } else if( isShadow==true && isRecursiveRay==false){
        	shadowRatio = softShadow(Ray::light);
        	absNdotL = abs(myDot(vecNormal, lightVector));
        	Id = absNdotL*Ia*(1-shadowRatio);
        } else{
            absNdotL = abs(myDot(vecNormal, lightVector));
            Id = absNdotL*Ia;
            H = lightVector+eyeVector;
            absNdotH = abs(myDot(vecNormal, H));
            Is = pow(absNdotH,myHitPoint.myMaterial.exp)*Ia;
        }
        /*
        if(isRecursiveRay==false && isShadow==true){
        	shadowRatio = softShadow(Ray::light);
        	cout<<"shadow ratio: "<<shadowRatio<<endl;
        	//myHitPoint.myMaterial.ka = myHitPoint.myMaterial.ka+ (1-shadowRatio)*0.5; //0.5 is self-defined, can add or minus
        	Id = (1-shadowRatio)
        }*/



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

bool Ray::checkShadow(Point3f sampleLight){
	bool sampleShadow = false;
    Point3f pointToLight(sampleLight.x - myHitPoint.thePoint.x, sampleLight.y - myHitPoint.thePoint.y,
                    sampleLight.z - myHitPoint.thePoint.z);
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

        if( theta >= 0 &&( t1 >0.00001 || t2 >0.00001)){
            sampleShadow = true;
            break;
        }
    }

    for(int k = 0; k<triangleBuffer.size(); k++){
        if(sampleShadow==true){
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
            if((ans.at<float>(0,0)>=0) && (ans.at<float>(1,0)>=0) && (ans.at<float>(0,0)+ans.at<float>(1,0)<=1) && t>0.00001){
                sampleShadow = true;
                break;
            }
        }
    }
    return sampleShadow;
}

float Ray::softShadow(Point3f sourceLightCore){
	int numSample = 1000;
	int numShadow = 0;
	float radiusCore = Ray::lightRadius;
	float floatArrayToLightCore[3] = {sourceLightCore.x - myHitPoint.thePoint.x, sourceLightCore.y - myHitPoint.thePoint.y, 
		sourceLightCore.z - myHitPoint.thePoint.z};
	float d = -(sourceLightCore.x* floatArrayToLightCore[0]+ sourceLightCore.y*floatArrayToLightCore[1]+ sourceLightCore.z*floatArrayToLightCore[2]);
	float randomZ = -d/floatArrayToLightCore[2];
	float floatArrayRandomSampleDisp_vector[3] = {sourceLightCore.x, sourceLightCore.y, sourceLightCore.z - randomZ};
	Mat vectorToLightCore = Mat(1, 3, CV_32F, floatArrayToLightCore);
	Mat vectorToRandomSampleDisp_1 = Mat(1, 3, CV_32F, floatArrayRandomSampleDisp_vector);
	Mat vectorToRandomSampleDisp_2 = vectorToRandomSampleDisp_1.cross(vectorToLightCore);
	normalize(vectorToRandomSampleDisp_1, vectorToRandomSampleDisp_1);
	normalize(vectorToRandomSampleDisp_2, vectorToRandomSampleDisp_2);
	//cout<<"Normal Vector :"<<vectorToLightCore<<endl;
	//cout<<"First Random Vector :"<<vectorToRandomSampleDisp_1<<endl;
	//cout<<"Second Random Vector :"<<vectorToRandomSampleDisp_2<<endl;
	std::random_device rd;
	std::default_random_engine gen = std::default_random_engine(rd());
	std::uniform_real_distribution<> dis(0,radiusCore);
	for(int i=0; i<numSample; i++){
		float scale_1 = dis(gen);
		float scale_2 = dis(gen);
		Point3f sampleLight(sourceLightCore.x+scale_1*vectorToRandomSampleDisp_1.at<float>(0,0)+scale_2*vectorToRandomSampleDisp_2.at<float>(0,0),
							sourceLightCore.y+scale_1*vectorToRandomSampleDisp_1.at<float>(0,1)+scale_2*vectorToRandomSampleDisp_2.at<float>(0,1),
							sourceLightCore.z+scale_1*vectorToRandomSampleDisp_1.at<float>(0,2)+scale_2*vectorToRandomSampleDisp_2.at<float>(0,2));
		bool isSampleShadow = checkShadow(sampleLight);
		if(isSampleShadow==true){
			numShadow++;
		}
	}
	float ratio = (float)numShadow/(float)numSample;
	return ratio;
}































