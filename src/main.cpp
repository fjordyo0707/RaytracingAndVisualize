#include <iostream>
#include <fstream>
#include "Raytracing.h"

using namespace std;
using namespace cv;

void readFile(string, Scene&);

int main(int argc, char** argv )
{
    Scene myScene;
    readFile(argv[1], myScene);
    myScene.startRay();
    cout<<"Save Image at "<<argv[2]<<endl;
    Mat savedImage = myScene.getImage();
    imwrite(argv[2], savedImage);
    
    return 0;
}

void readFile(string filename, Scene& myScene){
    ifstream file;
    file.open(filename,ios::in);
    
    if(!file.is_open()){
        cerr<<"File could not be opened"<<endl;
        exit(1);
    }
    else{
        string fileStr;
        SceneParameter sceneInfoTemp;
        vector<Sphere> sphereBufferTemp;
        vector<Triangle> triangleBufferTemp;
        while ( file>>fileStr ){
            if(fileStr=="E"){
                file>>sceneInfoTemp.eye.x;
                file>>sceneInfoTemp.eye.y;
                file>>sceneInfoTemp.eye.z;
            }
            else if(fileStr=="O"){
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
                triangleBufferTemp.push_back(Tri_temp);
            }
        }
        myScene.setSceneInfo( sceneInfoTemp );
        myScene.setSphereBuffer( sphereBufferTemp );
        myScene.setTriangleBuffer( triangleBufferTemp );
        myScene.setImage();
    }
    file.close();
}
