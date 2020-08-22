

# 一些头文件

```cpp
#include <sstream>  //stringstream
#include <unistd.h> //有sleep usleep
#include <stdint> //uint16 uint32 uint64...



#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
```



# ROS

### CMakeList.txt

```cmake
############################################################################################
##                                          常用宏
##                                      PROJECT_NAME
############################################################################################
cmake_minimum_required(VERSION 3.0.2)
##需和xml文件中的name一致
project(mypack)
# add_compile_options(-std=c++11)

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
##                                      non-ros packages
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







## 参数服务器





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





