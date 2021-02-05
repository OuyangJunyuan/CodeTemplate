###########################################################################################
######################################### NODE ############################################
###########################################################################################
                                 #### usual macro ####
##  ${PROJECT_NAME}         :project name
##	${PROJECT_SOURCE_DIR}   :this CmakeLists dir
##	${PROJECT_BINARY_DIR})  :build dir —— where you type command "cmake" in Terminal
##################################################################
                         #### send msg in Terminal when cmake ####
##	message(STATUS "xxx ${xxx}")
#   MESSAGE(WARNING xxxx)           words turn red
#   MESAAGE(ERROR xxx)              assert cmake

###########################################################################################
################################  CMake Project Setting ###################################
###########################################################################################
cmake_minimum_required(VERSION 3.0.2)
project(oslam)
	#set project name to the files that content this CMakeLists.txt
	#get_filename_component(PROJECT ${CMAKE_SOURCE_DIR} NAME)
set( CMAKE_BUILD_TYPE "release" )  #release debug 模式
#set( CMAKE_CXX_FLAGS "-std=c++11 -O3" ) #c++11   o3优化


############################################################################################
###################################### Files struction  ####################################
############################################################################################
## add src file ##
aux_source_directory(${PROJECT_SOURCE_DIR}/src  SRC_DIRS)
## add head dir ##
include_directories(${PROJECT_SOURCE_DIR}/include)#relative to this CMakeLists.txt
## add lib dir ##
link_directories(${PROJECT_SOURCE_DIR}/lib)#relative to this CMakeLists.txt
## add module dir ##
list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/module )
## set dir to gen exe ##
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
	#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/controllers/mywebotsdemo_node)
	#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)   
	#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../bin) 
## set dir to gen lib ##
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

############################################################################################
################################### Packages/dependencies  #################################
############################################################################################
                                    #### some tips ####
## if didn't find packages,you can try to ctl+f to search "<PACKNAME>Config.Cmake" in "/usr"
###################################################################
                                    #### some macro #### 
## <name>_FOUND                    
## <name>_LIBRARY_DIRS                 for include_directories()
## <name>_INCLUDE_DIRS(DIR)            for link_directories()
## <name>_LIBRARIES(LIBS/LIBRARY)      for target_link_libraries()
###################################################################
                                  ####  dependencies  ####

##                          ROS
##  用来寻找ROS 的组件,加在components
##  后面可以把找到的包对应的环境变量合并
##  到catkin的环境变量中。
find_package(catkin REQUIRED COMPONENTS
        roscpp
        rospy
        std_msgs
        geometry_msgs
        nav_msgs
        sensor_msgs
        #message_generation
        )
include_directories( ${catkin_INCLUDE_DIRS})
catkin_package(
        #  INCLUDE_DIRS include
        #  LIBRARIES mypack
        #  CATKIN_DEPENDS
        # roscpp rospy std_msgs #message_runtime
        #catkin_project的依赖
        #  DEPENDS system_lib   #非catkin project的依赖
)
list(APPEND ALL_LIBS ${catkin_LIBRARIES})

##                          gazebo
#find_package(gazebo REQUIRED)
#include_directories(${GAZEBO_INCLUDE_DIRS})
#list(APPEND ALL_LIBS ${GAZEBO_LIBRARY_DIRS})
#list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

##                          Eigen
#find_package(Eigen3  REQUIRED)
#include_directories(${EIGEN3_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIRS}/Eigen)


##                          Boost
#find_package(Boost REQUIRED COMPONENTS
# system  # format
#)
#include_directories(${Boost_INCLUDE_DIRS})
#list(APPEND ALL_LIBS ${Boost_LIBRARIES})

#                          OpenCV
#find_package(OpenCV  REQUIRED)
#include_directories(${OpenCV_INCLUDE_DIRS})
#list(APPEND ALL_LIBS ${OpenCV_LIBRARIES})

##                          Sophus
#find_package(Sophus REQUIRED )
#include_directories( ${Sophus_INCLUDE_DIRS} )#link  ${Sophus_LIBRARIES}
#list(APPEND ALL_LIBS ${Sophus_LIBRARIES})
#MESSAGE(STATUS "Sophus inc dir : ${Sophus_INCLUDE_DIR}")

##                          Ceres
#find_package( Ceres REQUIRED )
#include_directories( ${CERES_INCLUDE_DIRS} ) #link  ${CERES_LIBRARIES}
#list(APPEND ALL_LIBS ${CERES_LIBRARIES})

##                          G2O
##需要添加:module/FindGlog.cmake
#find_package(G2O REQUIRED)
#include_directories(${G2O_INCLUDE_DIR})
#list(APPEND ALL_LIBS g2o_core g2o_stuff g2o_types_sba g2o_csparse_extension)
##                          CSparse
#需要添加:module/FindCSparse.cmake
#find_package( CSparse REQUIRED)
#include_directories(${CSPARSE_INCLUDE_DIR})
#list(APPEND ALL_LIBS ${CSPARSE_LIBRARY})

##                          pcl
# 多版本PCL 需指定路径防止冲突，这里使用本地安装的pcl-1.8, pcl-1.11装入系统
# set(PCL_DIR "/home/ou/software/library-3-part/pcl-pcl-1.8.1/lib_install/share/pcl-1.8")
#find_package(PCL 1.8 REQUIRED)
#include_directories(${PCL_INCLUDE_DIRS})
#link_directories(${PCL_LIBRARY_DIRS})
#add_definitions(${PCL_DEFINITIONS})
#list(APPEND ALL_LIBS ${PCL_LIBRARIES})

##                          glog
##需要添加:module/Findglog.cmake
#find_package(Glog REQUIRED)
#include_directories(${GLOG_INCLUDE_DIRS}) #link   ${GLOG_LIBRARIES}
#list(APPEND ALL_LIBS ${GLOG_LIBRARIES})

##                          gtest
#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS}) #link  ${GTEST_BOTH_LIBRARIES}
#list(APPEND ALL_LIBS ${GTEST_LIBRARIES})

##                          gflags
#find_package(GFlags REQUIRED)
#include_directories(${GFLAGS_INCLUDE_DIRS}) #link  ${GFLAGS_LIBRARIES}
#list(APPEND ALL_LIBS ${GFLAGS_LIBRARIES})

############################################################################################
###################################### Generate lab/exe  ###################################
############################################################################################
                     #### make sure all kinds of package's dir is right ####
MESSAGE(STATUS "BUILD  dir :${${PROJECT_NAME}_BINARY_DIR}")
MESSAGE(STATUS "SOURCE dir :${${PROJECT_NAME}_SOURCE_DIR}")
###################################################################
                                #### to generate lib  ####
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/mypackage.cpp
# )
###################################################################
                                #### to generate exe  ####
#add_executable(${PROJECT_NAME}_exe ${SRC_DIRS} ./src/main.cpp)
#target_link_libraries(${PROJECT_NAME}_exe
#        ${ALL_LIBS}
#        )

#to define where to output exe files when you do not want to using CMAKE_RUNTIME_OUTPUT_DIRECTORY to set all target output-files
#set_target_properties(
#        ${PROJECT_NAME}_exe
#        PROPERTIES
#        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
#)

## In add_subdirectory() command:
## first  argument:relative ${PROJECT_NAME}_SOURCE_DIR
## second argument:relative ${PROJECT_NAME}_BINARY_DIR
## defaultly use second argument means to use ${PROJECT_NAME}_S/B_DIR
add_subdirectory(./src )
