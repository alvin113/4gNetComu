#include "ros/ros.h"
#include "std_msgs/String.h"

int count = 1;
bool con=true;

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
 // ROS_INFO("[%s]", msg->data.c_str());
  if(con==true)
{
  ROS_INFO("go to waypoint %d",count);
  con=false;
 } 
}

int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "listener");

 
  ros::NodeHandle n;


  ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);


  ros::spin();

  return 0;
}
