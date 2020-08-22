# OpenCV template	

## CMakeList

```cmake
############################################################################################
##                                          			常用宏
##                                    			  PROJECT_NAME
############################################################################################
cmake_minimum_required(VERSION 3.0.2)
##需和xml文件中的name一致
project(mypack)
# add_compile_options(-std=c++11)

############################################################################################
##                                  			    packages
## <name>_FOUND
## <name>_INCLUDE_DIRS
## <name>_LIBRARIES
############################################################################################
##                          当前目录下放的头文件
#####################################################
#添加头文件目录 
#include_directories(''./include'')
#添加库文件目录 
#link_directories(" ./lib")
#添加所有源文件名到SRC_DIRS中
#aux_source_directory(. SRC_DIRS)
#EXECUTABLE_OUTPUT_PATH
#LIBRARY_OUTPUT_PATH


##                          Eigen
#include_directories("/usr/include/eigen3")

##                          EBoost
#find_package(Boost REQUIRED COMPONENTS
# system
#)

##                          OpenCV
#find_package(OpenCV  REQUIRED)
#include_directories(${OpenCV_INCLUDE_DIRS})


############################################################################################
##                                       build
############################################################################################
##                     generate libs like .s .so
#####################################################
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/mypackage.cpp
# )
##库内有用到自定义消息类型时启用,否则无法生成消息头文件
#add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

#####################################################
##                         exe
#####################################################
#add_executable(${PROJECT_NAME}_node src/node.cpp)
#target_link_libraries(${PROJECT_NAME}_node
#        ${catkin_LIBRARIES}
#        )

```



## Core-Code

```cpp
#include "opencv2/opencv.hpp"
using namespace cv;
int main(int argc, char **argv)
{
    VideoCapture capture;
    capture.open(0);
    Mat frame;
    while(capture.isOpened())
    {
        capture.read(frame);
        imshow("origin",frame);
        if(waitKey(1)=='q')
            break;
    }
    return 0;
}
```

