#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/TwistStamped.h"////
#include "std_msgs/Float64.h"////
#include <sstream>
//#include "geometry_msgs/TwistStamped.h"////
//#include  "std_msgs/Header.h"   ////
//#include  "geometry_msgs/Twist.h"   ////
//#include  "std_msgs/Header.h"   ////


#include <sstream>

bool talk_con=true;
int main(int argc, char **argv)
{
  
 
  ros::init(argc, argv, "talker");

   ros::NodeHandle n;
 
  
  //mavros/setpoint_velocity/cmd_vel


  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
  //ros::Publisher throttle_pub = nh2.advertise<std_msgs::Float64>("/mavros/setpoint_attitude/att_throttle", 1000);////

  ros::Rate loop_rate(10);

  int count = 1;
  while (ros::ok())
  {
  
    std_msgs::String msg;
    //geometry_msgs::TwistStamped msg01;

    std::stringstream ss;
    ss << "go to Waypoint ";
    msg.data = ss.str();
   if(talk_con==true)
   {
    ROS_INFO("%s", msg.data.c_str());
    //std_msgs::Float64   msgthro


    chatter_pub.publish(msg);
    //throttle_pub.publish(msgthro);
    ++count;
   // talk_con=false;
   }
    ros::spinOnce();

    loop_rate.sleep();
   
    
  }


  return 0;
}
