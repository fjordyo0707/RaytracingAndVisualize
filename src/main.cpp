#include <iostream>
#include <fstream>
#include "Scene.h"

using namespace std;
using namespace cv;

Scene& readFile(string);

int main(int argc, char** argv )
{
    Scene myScene = readFile(argv[1]);
    myScene.startRay();
    cout<<"Save Image at "<<argv[2]<<endl;
    Mat savedImage = myScene.getImage();
    imwrite(argv[2], savedImage);
    
    return 0;
}

Scene& readFile(string filename){
    ifstream file;
    file.open(filename,ios::in);
    SceneParameter sceneInfoTemp;
    vector<Sphere> sphereBufferTemp;
    vector<Triangle> triangleBufferTemp;
    
    if(!file.is_open()){
        cerr<<"File could not be opened"<<endl;
        exit(1);
    }
    else{
        string fileStr;
        Material materialTemp = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        while ( file>>fileStr ){
            if(fileStr=="E"){
                file>>sceneInfoTemp.eye.x;
                file>>sceneInfoTemp.eye.y;
                file>>sceneInfoTemp.eye.z;
            } else if(fileStr=="V"){
                sceneInfoTemp.viewDir = Mat(3,1,CV_32F);
                sceneInfoTemp.viewUp = Mat(3,1,CV_32F);
                file>>sceneInfoTemp.viewDir.at<float>(0,0);
                file>>sceneInfoTemp.viewDir.at<float>(1,0);
                file>>sceneInfoTemp.viewDir.at<float>(2,0);
                file>>sceneInfoTemp.viewUp.at<float>(0,0);
                file>>sceneInfoTemp.viewUp.at<float>(1,0);
                file>>sceneInfoTemp.viewUp.at<float>(2,0);
            } else if(fileStr=="F"){
                file>>sceneInfoTemp.angle;
            } else if(fileStr=="M"){
                file>>materialTemp.r;
                file>>materialTemp.g;
                file>>materialTemp.b;
                file>>materialTemp.ka;
                file>>materialTemp.kd;
                file>>materialTemp.ks;
                file>>materialTemp.exp;
                file>>materialTemp.reflect;
            } else if(fileStr=="O"){
                file>>sceneInfoTemp.outputImagePoint1.x;
                file>>sceneInfoTemp.outputImagePoint1.y;
                file>>sceneInfoTemp.outputImagePoint1.z;
                file>>sceneInfoTemp.outputImagePoint2.x;
                file>>sceneInfoTemp.outputImagePoint2.y;
                file>>sceneInfoTemp.outputImagePoint2.z;
            } else if(fileStr=="R"){
                file>>sceneInfoTemp.resolution.x;
                file>>sceneInfoTemp.resolution.y;
            } else if(fileStr=="S"){
                Sphere S_temp;
                file>>S_temp.core.x;
                file>>S_temp.core.y;
                file>>S_temp.core.z;
                file>>S_temp.radius;
                S_temp.myMaterial = materialTemp;
                sphereBufferTemp.push_back(S_temp);
            } else if(fileStr=="T"){
                Triangle Tri_temp;
                file>>Tri_temp.vertex1.x;
                file>>Tri_temp.vertex1.y;
                file>>Tri_temp.vertex1.z;
                file>>Tri_temp.vertex2.x;
                file>>Tri_temp.vertex2.y;
                file>>Tri_temp.vertex2.z;
                file>>Tri_temp.vertex3.x;
                file>>Tri_temp.vertex3.y;
                file>>Tri_temp.vertex3.z;
                Tri_temp.myMaterial = materialTemp;
                triangleBufferTemp.push_back(Tri_temp);
            } else if(fileStr=="L"){
                file>>sceneInfoTemp.light.x;
                file>>sceneInfoTemp.light.y;
                file>>sceneInfoTemp.light.z;
            }
        }
    }
    file.close();

    static Scene tempScene(sceneInfoTemp, sphereBufferTemp, triangleBufferTemp);
    return tempScene;
}
