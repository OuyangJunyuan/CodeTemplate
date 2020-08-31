#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <mypack/test_msg.h>
int count;
double Kp,Ki,Kd;
int main(int argc, char **argv)
{
    //第三个参数(基本名字即不能带'/')为节点名，除非在launch中的<node ... name="xxx"/>重新指定
    //rosrun pack <exename> 由CMakeList指定
    //rosnode list 中出现的是 <nodename> 由第三个参数
    ros::init(argc, argv, "talker");
    //作为和ROS通信的主要途径,可以有多个Handle：在第一个Handle被完全初始化本节点；在最后销毁一个的Handle来完全关闭节点
    ros::NodeHandle n;
    //发布到的话题名，在本节点名称空间下/nodename/topicname，第二个参数为消息队列大小，还可以指定话题被接受者连接与断开连接回调函数。
    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
    ros::Publisher chatter_pub1 = n.advertise<std_msgs::String>("chatter1", 1000);
    ros::Rate loop_rate(10);

    std_msgs::String msg;
    mypack::test_msg testmsg;

    while (ros::ok())
    {
        n.param<double>("/node/Kp",Kp,1.0);
        n.param<double>("/node/Ki",Ki,1.0);
        n.param<double>("/node/Kd",Kd,1.0);
        std::stringstream ss;
        ss << Kp <<" "<<Ki<<" "<< Kd<<" "<< count;
        msg.data = ss.str();
        ROS_INFO("%s", msg.data.c_str());
        chatter_pub.publish(msg);
        chatter_pub1.publish(msg);
        count++;
        //注意spinOnce()区别:spinOnce只读取当前消息队列第一个元素调用callback后返回;spin直接堵塞，一有消息进队就调用回调，否则原地死循环。
        ros::spinOnce();
        //根据ros::Rate loop_rate(10)指定的频率，自动记录上次sleep到本次sleep之间的时间，然后休眠直到超过对应频率的延时。
        loop_rate.sleep();
    }
    return 0;
}