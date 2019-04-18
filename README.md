# Raytracing using OpenCV

Author: Cheng-Yu, Fan
Check Github: https://github.com/fjordyo0707/RaytracingAndVisualize

## Dependencies

#### OpenCV (version >3.0)

#### Cmake (version >2.8)

## Results
#### Phong Model
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Ambient &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Diffusion  &nbsp; &nbsp; &nbsp; &nbsp;  &nbsp;  &nbsp;  &nbsp;  &nbsp;  &nbsp;  &nbsp;  &nbsp;  &nbsp;  &nbsp;  &nbsp;  &nbsp; &nbsp;  &nbsp;  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;  Specular <br></br>
![alt text](https://raw.githubusercontent.com/fjordyo0707/RaytracingAndVisualize/master/img/ambient.png)![alt text](https://raw.githubusercontent.com/fjordyo0707/RaytracingAndVisualize/master/img/diffusion.png)![alt text](https://raw.githubusercontent.com/fjordyo0707/RaytracingAndVisualize/master/img/specular.png)<br></br>
#### Shadow
![alt text](https://raw.githubusercontent.com/fjordyo0707/RaytracingAndVisualize/master/img/phongWithShadow.png)<br></br>
#### Reflection
![alt text](https://raw.githubusercontent.com/fjordyo0707/RaytracingAndVisualize/master/img/output.png)<br></br>
#### More complicate structure
![alt text](https://raw.githubusercontent.com/fjordyo0707/RaytracingAndVisualize/master/img/some_example.png)<br></br>

## Install Denpendencies

#### MacOS

```bash
$ brew install cmake
$ brew install opencv
$ brew install pkg-config
```

#### Linux (Ubuntu)

```bash
$ sudo apt-get update
$ sudo apt-get install cmake
$ sudo apt-get install libopencv-dev python-opencv
```

#### Windows

Go here [Cmake](https://cmake.org/download/) to install cmake.

Install OpenCV depend on your IDE or compiler.


## Run 

#### Notice
1. This code does recursion until not hitting anything, so it may take some time if you have too many objects!<br></br>
2. This code isn't fully optimized, so may look a bit dirty!<br></br> 

#### Unix (recommend)

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./bin/output <path/to/input.txt> <path/to/output.png> 
# For example : $ ./bin/output ../test/mytest.txt ../img/output.png
```
#### Win (not recommend)

(Optional) Download the prebuild OpenCV and use Visual Studio

1. Open your IDE
2. Set OpenCV to your path environment
3. Link 

## Input Data Format(.txt)
#### Eye position: E x y z
#### View direction: V Dx Dy Dz Ux Uy Uz
#### Field of view: F angle
#### Resolution: R w h
#### Sphere: S Ox Oy Oz r
#### Triangle: T x1 y1 z1 x2 y2 z2 x3 y3 z3
#### Light position: L x y z
#### Material: M r g b Ka Kd Ks exp Reflect
