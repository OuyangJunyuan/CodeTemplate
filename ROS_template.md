

# 一些头文件

```cpp
#include <sstream>  //stringstream
#include <unistd.h> //有sleep usleep
#include <stdint> //uint16 uint32 uint64...
#include <chrono> //chrono::steady_clock::time_point  t1=chrono::steady_clock::now(); chrono::duration<double> = chrono::duration_cast<chrono::duration<double>> (t2-t1);
#include <boost/format.hpp> //格式化字符串: boost::format fmt(./%s/%s%d.%s); string fs=(fmt%"color"%"image_"%i%"png").str();//fs="./color/image_i.png"


#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"

#include <opencv2/opencv.hpp> //OpenCV库
#include <ceres/ceres.h> //ceres优化库
```



# ROS

### CMakeList.txt

```cmake
############################################################################################
##                                          常用宏
##                                      ${PROJECT_NAME}
##										${PROJECT_SOURCE_DIR} 本文件地址
##										${PROJECT_BINARY_DIR})  编译地址
##											输出信息
##										message(STATUS "xxx ${xxx}")
############################################################################################
cmake_minimum_required(VERSION 3.0.2)
##需和xml文件中的name一致
project(mypack) 
#set( CMAKE_BUILD_TYPE "Release" )  #release模式
#set( CMAKE_CXX_FLAGS "-std=c++11 -O3" ) #c++11   o3优化

############################################################################################
##                                       ros packages
############################################################################################
find_package(catkin REQUIRED COMPONENTS
        roscpp
        rospy
        std_msgs
        )
        
##添加 add_message_files	和 generate_messages 于下方

#一般可以不管他，只是为使其他依赖于本包的程序通过find_package就能获取本包内的所有依赖而不用重新添加(只能添加ROS包)。
catkin_package(
        #  INCLUDE_DIRS include
        #  LIBRARIES mypackage
        #  CATKIN_DEPENDS roscpp rospy std_msgs
        #  DEPENDS system_lib
)
##开始指定路径,禁止在之后寻找ROS的库
include_directories(
        # include
        ${catkin_INCLUDE_DIRS}
)
############################################################################################
##                                      non-ros packages:具体路径宏可以查询<库名Config.cmake>查看定义
## <name>_FOUND
## <name>_INCLUDE_DIRS  /  <name>_DIRS
## <name>_LIBRARIES  /  <name>_LIBS  /  <name>_LIBRARY
############################################################################################
##                          当前目录下放的头文件
#####################################################
#添加头文件目录
#include_directories(''./include'')  #相对于CMakeLists.txt
#添加库文件目录
#link_directories(" ./lib")  #相对于执行CMake的目录或路径一般是在
#添加所有源文件名到SRC_DIRS中
#aux_source_directory(./src  SRC_DIRS)
#set(EXECUTABLE_OUTPUT_PATH ./bin)
#set(LIBRARY_OUTPUT_PATH ./lib)




##                          Eigen
#find_package(Eigen3  REQUIRED)
#include_directories(${Eigen3_INCLUDE_DIRS})
#list(APPEND ALL_LIBS ${Eigen3_LIBRARIES})

##                          Boost
#find_package(Boost REQUIRED COMPONENTS
# system  # format 
#)
#include_directories(${Boost_INCLUDE_DIRS})
#list(APPEND ALL_LIBS ${Boost_LIBRARIES})

##                          OpenCV
#find_package(OpenCV  REQUIRED)
#include_directories(${OpenCV_INCLUDE_DIRS})
#list(APPEND ALL_LIBS ${OpenCV_LIBRARIES})

##                          Sophus
#find_package(Sophus REQUIRED) 
#include_directories(${Sophus_INCLUDE_DIRS}) #link  ${Sophus_LIBRARIES}
#list(APPEND ALL_LIBS ${Sophus_LIBRARIES})

##							Ceres
find_package( Ceres REQUIRED )
include_directories( ${CERES_INCLUDE_DIRS} ) #link  ${CERES_LIBRARIES} 
#list(APPEND ALL_LIBS ${CERES_LIBRARIES})

##                         G2O
#find_package(g2o REQUIRED)
#include_directories(${G2O_INCLUDE_DIRS}) 
#list(APPEND ALL_LIBS ${G2O_LIBRARIES})

##                         glog
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/cmake)
#find_package(Glog REQUIRED)
#include_directories(${GLOG_INCLUDE_DIRS}) #link   ${GLOG_LIBRARIES}
#list(APPEND ALL_LIBS ${GLOG_LIBRARIES})

##                         gtest
#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS}) #link  ${GTEST_BOTH_LIBRARIES}
#list(APPEND ALL_LIBS ${GTEST_LIBRARIES})

##                         gflags
#find_package(GFlags REQUIRED)
#include_directories(${GFLAGS_INCLUDE_DIRS}) #link  ${GFLAGS_LIBRARIES}
#list(APPEND ALL_LIBS ${GFLAGS_LIBRARIES})

############################################################################################
##                                       build
############################################################################################
##                     generate libs like .s .so
#####################################################
#MESSAGE(WARNING xxxx)会让字体变红色方便观看。
#MESAAGE(ERROR xxx) 会终止CMAKE
MESSAGE(STATUS "This is BINARY dir :${${PROJECT_NAME}_BINARY_DIR}")
MESSAGE(STATUS "This is SOURCE dir :${${PROJECT_NAME}_SOURCE_DIR}")
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/mypackage.cpp
# )
##库内有用到自定义消息类型时启用,否则无法生成消息头文件
#add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

#####################################################
##                          node
#####################################################
add_executable(${PROJECT_NAME}_node src/node.cpp)
##可执行文件内有用到自定义消息类型时启用,否则无法生成消息头文件
#add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME}_node
        ${catkin_LIBRARIES}
        )

#####################################################
##                         non-node
#####################################################
#add_executable(${PROJECT_NAME}_node src/node.cpp)
#target_link_libraries(${PROJECT_NAME}_node
#        ${catkin_LIBRARIES}
#        )

```

### XML

```xml
<?xml version="1.0"?>
<package format="2">
  <!-- info -->
  <name>mypack</name>
  <version>0.0.0</version>
  <description>The mypack package</description>
  <maintainer email="ou@todo.todo">ou</maintainer>
  <license>TODO</license>

  <!-- fatal depend declare -->
  <buildtool_depend>catkin</buildtool_depend>
  <build_depend>roscpp</build_depend>
  <build_depend>rospy</build_depend>
  <build_depend>std_msgs</build_depend>
  <build_export_depend>roscpp</build_export_depend>
  <build_export_depend>rospy</build_export_depend>
  <build_export_depend>std_msgs</build_export_depend>
  <exec_depend>roscpp</exec_depend>
  <exec_depend>rospy</exec_depend>
  <exec_depend>std_msgs</exec_depend>

  <!-- optional depend declare -->
    
  
  <!-- The export tag contains other, unspecified, tags -->
  <export>
    <!-- Other tools can request additional information be placed here -->

  </export>
</package>
```



## 启动文件

详见：[launch/xml](http://wiki.ros.org/roslaunch/XML)

```xml
<launch>
    <!--	<include file="xxx" ns="" />	-->
    <arg name="arg1" value="$(dirname)" />
    <node pkg="mypack" name="node" type="mypack_node" >
  	    <param  name="launchpath"	 value="$(arg arg1)" />
	    <rosparam file="$(dirname)/../config/config.yaml" command="load" />
    </node>
</launch>

```



## 参数服务器的参数文件yaml

* 拓展:[ros参数](https://www.jianshu.com/p/6c1ec58b3f24)

  ros参数服务器保持参数直到master被关闭，并且在各个ros程序间共用。

  ``````launch
  启动文件
  <param name="ns/para" value="10.0" />
    <rosparam>
           a: 1
           b: 2
    </rosparam>
    
  <rosparam file="$(dirname)/../config/config.yaml" command="load" /> 
  ``````

  yaml:

  ```yaml
  #注意: 和数字之间有一个  
  #支持向量格式
  
  footprint: 		[[-0.325, -0.325], [-0.325, 0.325], [0.325, 0.325], [0.46, 0.0], [0.325, -0.325]]
  floatvalue:		1.00
  boolvalue:		true
  #嵌套命名空间
  base_scan_marking:		 {sensor_frame: base_laser, topic: /base_scan_marking, data_type}
  ```

  cpp:

  ```c++
  node.getParamNames(names)//names is a vetor
  node.getParam(name);
  node.setParam(name, value)；
  node.hashParam(name)；//判断参数是否存在
  node.param<变量类型>("参数名",变量,"参数不存在时默认值");
  ```

  

* 程序入口参数

  1. 启动文件launch启动时指定参数

     ```launch中:	<arg name="my_args" default="arg1 arg2"/>```  default可被重新指定值覆盖。

     ```terminal:	roslaunch xxxpack xxx.launch  my_args:=newvalue```

     ```srccode-cpp:     std::cout<<"argv["<<i<<"] "<<argv[i]<<std::endl;```

  2. node 传参数

     ```launch中可以有: <node pkg="pack" type="node1" name="node1" args="$(arg my_args)"/>```

     ```或者启动节点时候:rosrun xxxpack xxxexe arg1 arg2 或```

     



## 自定义消息类型

```
Header header
int32[] data
geometry_msgs/Twist
```

1. package.xml 需要添加消息生成器的依赖，其中depend可以避免写3个类型的depend。

   ```xml
   <build_depend>message_generation</build_depend>
   <exec_depend>message_runtime</exec_depend>
   <depend> geometry_msgs </depend>
   ```

2. CMakeList.txt  

   * 在catkin寻找依赖中增加message_generation（在catkin_package前）

     ```cmake
     find_package(catkin REQUIRED COMPONENTS
     		...		
             message_generation##增加
     )
     ```

    * 增加消息文件 使消息文件包含于编译环境中（在catkin_package前）

      ```cmake
        add_message_files(
            FILES
            test_msg.msg##增加
        )
      ```

   * 增加消息生成指令及配置依赖（在catkin_package前）

     ```cmake
     generate_messages(
     	DEPENDENCIES
     	...
     	#如果依赖于其他非原生类型则需要添加依赖如
     	geometry_msgs  ##增加 ——不需要geometry/Twist 这样，因为没有这种.cmakeconfig文件，会找不到。
     )
     ```

   * 在catkin包声明时候增加CATKIN_DEPENDS中对message_runtime依赖

     ```cmake
     catkin_package(
      	 ...
       	CATKIN_DEPENDS ... message_runtime ##增加
     	 ...
     )
     ```


   * 在直接或间接包含这个消息头文件的可执行文件或库的add_exe/add_lib后跟上。否则可能得不到**<u>头文件</u>**。

     ```cmake
     add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS}) ##增加
     ```

     

3. 然后

   ```catkin_make```

   就会生成一个```mypack/test_msg```类型的消息，注意前面有一个mypack表示是这个项目名或说包下的消息类型

### 发送节点

区分节点名，可执行文件名。rosrun只是指定可执行文件名，为何用rosrun只是因为可以自动找到ws下的对应可执行文件位置而不用手动输入文件路径

```c++
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"
#include <sstream>

int count;
double Kp,Ki,Kd;
int main(int argc, char **argv)
{
    //第三个参数(基本名字即不能带'/')为节点名，除非在launch中的<node ... name="xxx"/>重新指定
    //rosrun pack <exename> 由CMakeList指定
    //rosnode list 中出现的是 <nodename> 由第三个参数
    ros::init(argc, argv, "talker");
    //作为和ROS通信的主要途径：在第一个Handle被完全初始化本节点；在最后销毁一个的Handle来完全关闭节点
    ros::NodeHandle n;
    //发布到的话题名，在本节点名称空间下/nodename/topicname，第二个参数为消息队列大小，还可以指定话题被接受者连接与断开连接回调函数。
    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
    ros::Rate loop_rate(10);

    std_msgs::String msg;
    while (ros::ok())
    {
        n.param<double>("/node/Kp",Kp,1.0);
        n.param<double>("/node/Ki",Ki,1.0);
        n.param<double>("/node/Kd",Kd,1.0);
        std::stringstream ss;
        ss << Kp <<" "<<Ki<<" "<< Kd<<" "<< count;
        msg.data = ss.str();
        //ROS_INFO可以在被launch时输出到终端。
        ROS_INFO("%s", msg.data.c_str());
        chatter_pub.publish(msg);
        count++;
        //注意spinOnce()区别:spinOnce只读取当前消息队列第一个元素调用callback后返回;spin直接堵塞，一有消息进队就调用回调，否则原地死循环。
        ros::spinOnce();
        //根据ros::Rate loop_rate(10)指定的频率，自动记录上次sleep到本次sleep之间的时间，然后休眠直到超过对应频率的延时。        
        loop_rate.sleep();
    }
    return 0;
}
```



### 接受节点

```cpp
#include "ros/ros.h"
#include "std_msgs/String.h"

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}
int main(int argc, char **argv)
{

  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
  //注意spinOnce()区别:spinOnce只读取当前消息队列第一个元素调用callback后返回;spin直接堵塞，一有消息进队就调用回调，否则原地死循环直到ctl+c
  ros::spin();
  return 0;
}
```



## 服务

1. 创建一个文件 xxx.srv

   上方定义的是请求的消息类型

   下方定义的是响应的消息类型

   ```
   xxx.srv：
   
   int64 a
   int64 b
   ---
   int64 sum
   ```

   

2. xml和消息一样添加，添加message_generate和message_runtime 和嵌套消息类型依赖。

3. cmakelist和消息一样，如果已经有用到消息了，则不需要在增加指令，只需要增加参数即可

   在已经添加消息的基础上(已经find_package，generate_messages, carkin_package依赖了)

   只需要添加

   ```
   add_service_files(
   	FILES
   	xxx.srv
   )
   ```


   * 在直接或间接包含这个服务头文件的可执行文件或库的add_exe/add_lib后跟上。否则可能得不到**<u>头文件</u>**。

     ```cmake
     add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS}) ##增加
     ```

     

### 服务器

假设添加了服务类型,如何生成服务类型看上面

```
add_service_files(
  FILES
  srvtype.srv
)
```

则可以编写服务器节点为

```c++
#include "ros/ros.h"
//自动根据srv文件创建的头文件
#include "packname/srvtype.h"

bool request_function(packname::srrvtype::Request  &req,
         packname::srrvtype::Response &res)
{
  res.sum = req.a + req.b;
  //ROS_INFO("request: x=%ld, y=%ld", (long int)req.a, (long int)req.b);
  //ROS_INFO("sending back response: [%ld]", (long int)res.sum);
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "server_node_name");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("srv_name", request_function);
  //ROS_INFO("Ready to add two ints.");
  ros::spin();

  return 0;
}
```



### 客户端

```c++
#include "ros/ros.h"
#include "packname/srvtype.h"
#include <cstdlib>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "client_node_name");
  //传参2个值
  if (argc != 3)
  {
    ROS_INFO("usage: add_two_ints_client X Y");
    return 1;
  }

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<packname::srvtype>("srv_name");
  packname::srvtype srv;
  //srv.request.a = atoll(argv[1]);
  //srv.request.b = atoll(argv[2]);
  if (client.call(srv))
  {
    ROS_INFO("Sum: %ld", (long int)srv.response.sum);
  }
  else
  {
    ROS_ERROR("Failed to call service add_two_ints");
    return 1;
  }

  return 0;
}
```



CMakeList

```cmake
add_executable(server_exe_name src/server_src.cpp)
target_link_libraries(server_exe_name ${catkin_LIBRARIES})
add_dependencies(server_exe_name beginner_tutorials_gencpp)
#client 同上
```

可执行文件存放在

```~/ros_ws/devel/lib/share/。```

运行起来和节点一样，先开启服务器server

```rosrun packname server_exe_name```

再启动服务器，具体使用可以运行程序传参进行请求得到回应然后结束，也可以是在程序内进行不断请求和回应轮寻。





