# ACG assignment1

Student ID: B04901190 Name: 范晟祐

### Dependencies

##### OpenCV (version >3.0)

##### Cmake (version >2.8)

### Install Denpendencies

##### MacOS

```bash
$ brew install cmake
$ brew install opencv
$ brew install pkg-config
```

##### Linux (Ubuntu)

```bash
$ sudo apt-get update
$ sudo apt-get install cmake
$ sudo apt-get install libopencv-dev python-opencv
```

##### Windows

Go here [Cmake](https://cmake.org/download/) to install.

Install OpenCV depend on your IDE or compiler.


### Run 

##### Unix (recommend)

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./bin/output <path/to/input.txt> <path/to/output.png> 
# For example : $ ./bin/output ../test/mytest.txt ./output.png
```
##### Win (not recommend)

1. Open your IDE
2. Set OpenCV to your path environment
3. Link by yourself
4. (Optional) Download the prebuild OpenCV and use Visual Studio 