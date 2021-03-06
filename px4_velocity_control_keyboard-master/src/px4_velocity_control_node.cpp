#include <ros/ros.h>

#include <keyboard/Key.h>

#include <geometry_msgs/Point.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TwistStamped.h>
#include <geometry_msgs/PoseArray.h>

#include <tf/tf.h>
#include <tf/transform_datatypes.h>

#include <stdio.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>

#include <mavros_msgs/Waypoint.h>
#include <mavros_msgs/WaypointPush.h>
#include <mavros_msgs/CommandCode.h>
#include <mavros_msgs/WaypointClear.h>

bool waypoints_push_clear=false;
geometry_msgs::TwistStamped vs;
geometry_msgs::TwistStamped vs_body_axis;
geometry_msgs::PoseStamped ps;
geometry_msgs::PoseStamped uavCurrentLocalPose;
geometry_msgs::PoseStamped uavLockPose;
geometry_msgs::PoseStamped uavNextPose;

double uavRollENU, uavPitchENU, uavYawENU;
bool isSendNextPose;
bool isSendNextVelocity;
bool isSendNextPoseInitial;
bool isPositionReached;

int rosInfoTimeDealy;
int K=1;

mavros_msgs::SetMode offb_set_mode;
mavros_msgs::State current_state;
mavros_msgs::CommandBool arm_cmd;

ros::ServiceClient arming_client;
ros::ServiceClient set_mode_client;

////czj
ros::ServiceClient wp_push_client;
ros::ServiceClient wp_clear_client;

/*
std::vector<mavros_msgs::Waypoint> waypoints_vec;
mavros_msgs::Waypoint waypoint_msg;
mavros_msgs::WaypointPush::Request req_wp;
mavros_msgs::WaypointPush::Response resp_wp;
*/

mavros_msgs::WaypointClear waypoint_clear;


int wpCount = 0;  

void state_cb(const mavros_msgs::State::ConstPtr& msg){
    current_state = *msg;
}

void updateControlCommandStats()
{
  std::string controlCommandStatus = "";
  if (isSendNextVelocity)
    controlCommandStatus  += "[isSendNextVelocity = true]  ";
  else
    controlCommandStatus  += "[isSendNextVelocity = false]  ";

  if (isSendNextPose)
    controlCommandStatus  += "[isSendNextPose = true]  ";
  else
    controlCommandStatus  += "[isSendNextPose = false]  ";

  if (isSendNextPoseInitial)
    controlCommandStatus  += "[isSendNextPoseInitial = true]  ";
  else
    controlCommandStatus  += "[isSendNextPoseInitial = false]  ";
  ROS_INFO_STREAM(controlCommandStatus);

  std::string connectionStatus = "";
  if (current_state.connected)
    connectionStatus += "[Connected] ";
  else
    connectionStatus += "[Disconnected] ";
  
  if (current_state.armed)
    connectionStatus += "[Armed] ";
  else
    connectionStatus += "[Disarmed] ";

  if (current_state.mode == "OFFBOARD")
    connectionStatus += "[OFFBOARD] ";
  else
    connectionStatus += "[NOT OFFBOARD] ";
  ROS_INFO_STREAM(connectionStatus);

  ROS_INFO("current yaw = %0.3f\n", uavYawENU*180/3.1415926);
  ROS_INFO("[vbx, vby, vbaz] = [%0.5f, %0.5f, %0.5f]\n", vs.twist.linear.x, vs.twist.linear.y, vs.twist.linear.z);
      //printf("[vroll, vpitch, vyaw = [%0.5f, %0.5f, %0.5f]\n", vs.twist.angular.x, vs.twist.angular.y, vs.twist.angular.z);
      
  ROS_INFO("[vx, vy, vz] = [%0.5f, %0.5f, %0.5f]\n", vs_body_axis.twist.linear.x, vs_body_axis.twist.linear.y, vs_body_axis.twist.linear.z);
      //printf("[vroll, vpitch, vyaw = [%0.5f, %0.5f, %0.5f]\n", vs_body_axis.twist.angular.x, vs_body_axis.twist.angular.y, vs_body_axis.twist.angular.z);
}


void sendCommand(const keyboard::Key &key)
{
  switch (key.code)
  {
    case 'q':
    {
      ROS_WARN_STREAM("up");
     // vs.twist.linear.z += 0.1;
      //isSendNextPose = false;      
      //isSendNextVelocity = true; 
      ////pose
      uavNextPose.pose.position.x = uavCurrentLocalPose.pose.position.x;// 
      uavNextPose.pose.position.y = uavCurrentLocalPose.pose.position.y;// 
      uavNextPose.pose.position.z = uavCurrentLocalPose.pose.position.z+0.2*K;//
      isSendNextPose = true;
      isSendNextVelocity = false; 
      updateControlCommandStats();
      break;
    }
    case 'a':
    {
      /*
      ROS_WARN_STREAM("z is zero");
      vs.twist.linear.z = 0.0;
      isSendNextPose = false;      
      isSendNextVelocity = true; 


      updateControlCommandStats();
      */
      break;
    }
    case 'z':
    {
      ROS_WARN_STREAM("down");
      //vs.twist.linear.z -= 0.1;
      //isSendNextPose = false;      
      //isSendNextVelocity = true; 
      ////pose
      uavNextPose.pose.position.x = uavCurrentLocalPose.pose.position.x;// 
      uavNextPose.pose.position.y = uavCurrentLocalPose.pose.position.y;// 
      uavNextPose.pose.position.z = uavCurrentLocalPose.pose.position.z - 0.2*K;//
      isSendNextPose = true;
      isSendNextVelocity = false; 
      updateControlCommandStats();
      break;
    }
    case 'j':
    {
      ROS_WARN_STREAM("left");
      // vs.twist.linear.y -= 0.1; 
      //isSendNextPose = false;      
      //isSendNextVelocity = true; 
      ////pose
      uavNextPose.pose.position.x = uavCurrentLocalPose.pose.position.x;// 
      uavNextPose.pose.position.y = uavCurrentLocalPose.pose.position.y-0.2*K;// 
      uavNextPose.pose.position.z = uavCurrentLocalPose.pose.position.z;//
      isSendNextPose = true;
      isSendNextVelocity = false; 
      updateControlCommandStats();
      break;
    }
    case 'l':
    {
      ROS_WARN_STREAM("right");
      //vs.twist.linear.y += 0.1;
      //isSendNextPose = false;      
      //isSendNextVelocity = true; 
      ////pose
      uavNextPose.pose.position.x = uavCurrentLocalPose.pose.position.x;// 
      uavNextPose.pose.position.y = uavCurrentLocalPose.pose.position.y + 0.2*K;// 
      uavNextPose.pose.position.z = uavCurrentLocalPose.pose.position.z;//
      isSendNextPose = true;
      isSendNextVelocity = false; 
      updateControlCommandStats();
      break;
    }
    case 'i':
    {
      ROS_WARN_STREAM("forward");
      //vs.twist.linear.x += 0.1;
      //isSendNextPose = false;      
      //isSendNextVelocity = true; 
     ////pose
      uavNextPose.pose.position.x = uavCurrentLocalPose.pose.position.x + 0.2*K;// 
      uavNextPose.pose.position.y = uavCurrentLocalPose.pose.position.y;// 
      uavNextPose.pose.position.z = uavCurrentLocalPose.pose.position.z;//
      isSendNextPose = true;
      isSendNextVelocity = false; 
      updateControlCommandStats();
      break;
    }
    case 'k':
    {
      ROS_WARN_STREAM("set XY to zero");
      //vs.twist.linear.x = 0.0;
      //vs.twist.linear.y = 0.0;
      //vs.twist.angular.x = 0.0;
      //vs.twist.angular.y = 0.0;
      //vs.twist.angular.z = 0.0;
      //isSendNextPose = false;      
      //isSendNextVelocity = true; 
      //updateControlCommandStats();
      break;
    }
    case ',':
    {
      ROS_WARN_STREAM("backward");
      //vs.twist.linear.x -= 0.1;
      //isSendNextPose = false;      
      //isSendNextVelocity = true; 
      ////pose
      uavNextPose.pose.position.x = uavCurrentLocalPose.pose.position.x-0.2*K;// 
      uavNextPose.pose.position.y = uavCurrentLocalPose.pose.position.y;// 
      uavNextPose.pose.position.z = uavCurrentLocalPose.pose.position.z;//
      isSendNextPose = true;
      isSendNextVelocity = false; 
      updateControlCommandStats();
      break;
    }
    case 'u':
    {
      
      ROS_WARN_STREAM("turn clockwise");
      vs.twist.angular.z += 0.1;
      isSendNextPose = false;      
      isSendNextVelocity = true; 
      updateControlCommandStats();
      
      break;
    }
    case 'o':
    {
      
      ROS_WARN_STREAM("turn anti-clockwise");
      vs.twist.angular.z -= 0.1;
      isSendNextPose = false;      
      isSendNextVelocity = true; 
      updateControlCommandStats();
      
      break;
    }
    case 'h':
    {
      
      ROS_WARN_STREAM("set all to zero");
      vs.twist.linear.x = 0.0;
      vs.twist.linear.y = 0.0;
      vs.twist.linear.z = 0.0; 
      vs.twist.angular.x = 0.0;
      vs.twist.angular.y = 0.0;
      vs.twist.angular.z = 0.0;
      isSendNextPose = false;      
      isSendNextVelocity = true; 
      updateControlCommandStats();
      
      break;
    }
    case 'n':
    {
      ROS_INFO_STREAM("get current lock position");
      uavLockPose.pose.position.x = uavCurrentLocalPose.pose.position.x;
      uavLockPose.pose.position.y = uavCurrentLocalPose.pose.position.y;
      uavLockPose.pose.position.z = uavCurrentLocalPose.pose.position.z;

      ROS_INFO("current lock position: Pos:[%0.3f, %0.3f, %0.3f]", uavLockPose.pose.position.x, uavLockPose.pose.position.y, uavLockPose.pose.position.z);
      
      /*
      float body_next_pos_x = 0;
      //float body_next_pos_y = -3.0;
      float body_next_pos_y = 0;
      float body_next_pos_z = 3.0;
      float body_vector_length = sqrt((body_next_pos_x)*(body_next_pos_x)+(body_next_pos_y)*(body_next_pos_y)+(body_next_pos_z)*(body_next_pos_z));
      float body_xy_length = sqrt((body_next_pos_x)*(body_next_pos_x) + (body_next_pos_y)*(body_next_pos_y));
      


      float delta_x = body_next_pos_x * cos(uavYawENU) - body_next_pos_y * sin(uavYawENU);
      float delta_y = body_next_pos_x * sin(uavYawENU) + body_next_pos_y * cos(uavYawENU);



      uavNextPose.pose.position.x = uavLockPose.pose.position.x + delta_x;// + 3.0;
      uavNextPose.pose.position.y = uavLockPose.pose.position.y + delta_y;// - 3.0;
      uavNextPose.pose.position.z = uavLockPose.pose.position.z + body_next_pos_z;//+ 3.0;
      */
      uavNextPose.pose.position.x    = uavCurrentLocalPose.pose.position.x;// 
      uavNextPose.pose.position.y    = uavCurrentLocalPose.pose.position.y;// 
      uavNextPose.pose.position.z    = uavCurrentLocalPose.pose.position.z;//

      uavNextPose.pose.orientation.x = uavCurrentLocalPose.pose.orientation.x;
      uavNextPose.pose.orientation.y = uavCurrentLocalPose.pose.orientation.y;
      uavNextPose.pose.orientation.z = uavCurrentLocalPose.pose.orientation.z;
      uavNextPose.pose.orientation.w = uavCurrentLocalPose.pose.orientation.w;

      ROS_INFO("next position: Pos:[%0.3f, %0.3f, %0.3f]", uavNextPose.pose.position.x, uavNextPose.pose.position.y, uavNextPose.pose.position.z);

      isSendNextPoseInitial = true;
      isSendNextPose = true;
      updateControlCommandStats();
      break;
    }
    case 'y':
    {
      isSendNextPose = true;      
      isSendNextVelocity = false;   
      break;
    }
    case 't':
    {
      offb_set_mode.request.custom_mode = "OFFBOARD";
      set_mode_client.call(offb_set_mode);////

      if (offb_set_mode.response.success)
        ROS_WARN_STREAM("Offboard enabled");
      break;
    }
    case 'g':
    { 
      arm_cmd.request.value = true;
      arming_client.call(arm_cmd);
      if (arm_cmd.response.success)
        ROS_WARN_STREAM("Vehicle armed");
      break;
    }
    case 'b':
    {
      arm_cmd.request.value = false;
      arming_client.call(arm_cmd);
      if (arm_cmd.response.success)
        ROS_WARN_STREAM("Vehicle disarmed");
      break;
    }

     case 'w': ////set to mission mode and sent waypoints to px4
    {
      offb_set_mode.request.custom_mode = "AUTO.MISSION";////WAYPOINT
    
      set_mode_client.call(offb_set_mode);////

     if (offb_set_mode.response.success)
        ROS_WARN_STREAM("AUTO.MISSION enabled");

      break;
    }

        case '=':
    {
      
       
       if(K<10)
       {
        K++;

       }
       ROS_INFO("Magnification factor is %d",K); 
      
       break;
    }
       case '-':
    {
      
       
       if(K>2)
       {
        K--;

       }
       ROS_INFO("Magnification factor is %d",K); 
       break;
     
    }
       case 's': ////set to mission mode and sent waypoints to px4
    {
      /*
      offb_set_mode.request.custom_mode = "AUTO.MISSION";////WAYPOINT
    
      set_mode_client.call(offb_set_mode);////

      if (offb_set_mode.response.success)
        ROS_WARN_STREAM("AUTO.MISSION enabled");

      */

      std::vector<mavros_msgs::Waypoint> waypoints_vec;
      mavros_msgs::Waypoint waypoint_msg;
      mavros_msgs::WaypointPush::Request req_wp;
      mavros_msgs::WaypointPush::Response resp_wp;


      if(waypoints_push_clear==true)////all waypoints cleared, send new waypoints
      {      
   //point 1
      waypoint_msg.frame = mavros_msgs::Waypoint::FRAME_GLOBAL_REL_ALT;
      waypoint_msg.command = mavros_msgs::CommandCode::NAV_WAYPOINT;////????
      waypoint_msg.is_current = true;////there is only one waypoint set to true at the same time
      waypoint_msg.autocontinue = true;
      waypoint_msg.param1 = 30.0;
      waypoint_msg.param2 = 0.0;
      waypoint_msg.param3 = 0.0;
      waypoint_msg.param4 = 0.0;
      waypoint_msg.x_lat =  0.5*(28.2212047577+28.2212696075);
      waypoint_msg.y_long = 0.5*(112.991485596+112.991317749);
      waypoint_msg.z_alt = 20.0;
      waypoints_vec.push_back(waypoint_msg);

   

     ////point 2
      waypoint_msg.frame = mavros_msgs::Waypoint::FRAME_GLOBAL_REL_ALT;
      waypoint_msg.command = mavros_msgs::CommandCode::NAV_WAYPOINT;////????
      waypoint_msg.is_current = false;////there is only one waypoint set to true at the same time
      waypoint_msg.autocontinue = true;
      waypoint_msg.param1 = 15.0;
      waypoint_msg.param2 = 0.0;
      waypoint_msg.param3 = 0.0;
      waypoint_msg.param4 = 0.0;
      waypoint_msg.x_lat =  0.5*(28.2209358215+28.2209892273);
      waypoint_msg.y_long = 0.5*(112.991340637+112.991195679);
      waypoint_msg.z_alt = 20.0;
      waypoints_vec.push_back(waypoint_msg);
      
/*     ////point 1
    
      waypoint_msg.frame = mavros_msgs::Waypoint::FRAME_GLOBAL_REL_ALT;
      waypoint_msg.command = mavros_msgs::CommandCode::NAV_WAYPOINT;////????
      waypoint_msg.is_current = true;////there is only one waypoint set to true at the same time
      waypoint_msg.autocontinue = true;
      waypoint_msg.param1 = 30.0;
      waypoint_msg.param2 = 0.0;
      waypoint_msg.param3 = 0.0;
      waypoint_msg.param4 = 0.0;
      waypoint_msg.x_lat = 28.2209358215;
      waypoint_msg.y_long = 112.991340637;
      waypoint_msg.z_alt = 15.0;
      waypoints_vec.push_back(waypoint_msg);
      

     ////point 2
      waypoint_msg.frame = mavros_msgs::Waypoint::FRAME_GLOBAL_REL_ALT;
      waypoint_msg.command = mavros_msgs::CommandCode::NAV_WAYPOINT;////????
      waypoint_msg.is_current = false;////there is only one waypoint set to true at the same time
      waypoint_msg.autocontinue = true;
      waypoint_msg.param1 = 15.0;
      waypoint_msg.param2 = 0.0;
      waypoint_msg.param3 = 0.0;
      waypoint_msg.param4 = 0.0;
      waypoint_msg.x_lat = 28.2212047577;
      waypoint_msg.y_long = 112.991485596;
      waypoint_msg.z_alt = 15.0;
      waypoints_vec.push_back(waypoint_msg);
     

      ////point 3
      waypoint_msg.frame = mavros_msgs::Waypoint::FRAME_GLOBAL_REL_ALT;
      waypoint_msg.command = mavros_msgs::CommandCode::NAV_WAYPOINT;////????
      waypoint_msg.is_current = false;////there is only one waypoint set to true at the same time
      waypoint_msg.autocontinue = true;
      waypoint_msg.param1 = 10.0;
      waypoint_msg.param2 = 0.0;
      waypoint_msg.param3 = 0.0;
      waypoint_msg.param4 = 0.0;
      waypoint_msg.x_lat = 28.2212696075;
      waypoint_msg.y_long = 112.991317749;
      waypoint_msg.z_alt = 15.0;
      waypoints_vec.push_back(waypoint_msg);   



      ////point 4
      waypoint_msg.frame = mavros_msgs::Waypoint::FRAME_GLOBAL_REL_ALT;
      waypoint_msg.command = mavros_msgs::CommandCode::NAV_WAYPOINT;////????
      waypoint_msg.is_current = false;////there is only one waypoint set to true at the same time
      waypoint_msg.autocontinue = true;
      waypoint_msg.param1 = 15.0;
      waypoint_msg.param2 = 0.0;
      waypoint_msg.param3 = 0.0;
      waypoint_msg.param4 = 0.0;
      waypoint_msg.x_lat = 28.2209892273;
      waypoint_msg.y_long = 112.991195679;
      waypoint_msg.z_alt = 15.0;
      waypoints_vec.push_back(waypoint_msg);
*/




      req_wp.waypoints = waypoints_vec;
  
      wp_push_client.call(req_wp,resp_wp);
        
      if (resp_wp.success)
      ROS_WARN_STREAM("WyPoints Transferreded ");
      waypoints_push_clear=false;
      }
      else
      ROS_WARN_STREAM("Previous WyPoints are not cleared");
      
      
       
      break;
    }
        case 'r': ////set to mission mode and sent waypoints to px4(Same as QGC)
    {


      std::vector<mavros_msgs::Waypoint> waypoints_vec;
      mavros_msgs::Waypoint waypoint_msg;
      mavros_msgs::WaypointPush::Request req_wp;
      mavros_msgs::WaypointPush::Response resp_wp;


      if(waypoints_push_clear==true)////all waypoints cleared, send new waypoints
      {
           ////point 2
      waypoint_msg.frame = mavros_msgs::Waypoint::FRAME_GLOBAL_REL_ALT;
      waypoint_msg.command = mavros_msgs::CommandCode::NAV_WAYPOINT;////????
      waypoint_msg.is_current = true;////there is only one waypoint set to true at the same time
      waypoint_msg.autocontinue = true;
      waypoint_msg.param1 = 15.0;
      waypoint_msg.param2 = 0.0;
      waypoint_msg.param3 = 0.0;
      waypoint_msg.param4 = 0.0;
      waypoint_msg.x_lat =  0.5*(28.2209358215+28.2209892273);
      waypoint_msg.y_long = 0.5*(112.991340637+112.991195679);
      waypoint_msg.z_alt = 20.0;
      waypoints_vec.push_back(waypoint_msg);

           //point 1
      waypoint_msg.frame = mavros_msgs::Waypoint::FRAME_GLOBAL_REL_ALT;
      waypoint_msg.command = mavros_msgs::CommandCode::NAV_WAYPOINT;////????
      waypoint_msg.is_current = false;////there is only one waypoint set to true at the same time
      waypoint_msg.autocontinue = true;
      waypoint_msg.param1 = 30.0;
      waypoint_msg.param2 = 0.0;
      waypoint_msg.param3 = 0.0;
      waypoint_msg.param4 = 0.0;
      waypoint_msg.x_lat =  0.5*(28.2212047577+28.2212696075);
      waypoint_msg.y_long = 0.5*(112.991485596+112.991317749);
      waypoint_msg.z_alt = 20.0;
      waypoints_vec.push_back(waypoint_msg);
      
     






      req_wp.waypoints = waypoints_vec;
  
      wp_push_client.call(req_wp,resp_wp);
        
      if (resp_wp.success)
      ROS_WARN_STREAM("WyPoints Transferreded as QGC ");
      waypoints_push_clear=false;
      }
      else
      ROS_WARN_STREAM("Previous WyPoints are not cleared");
      
      
       
      break;
    }
     case 'c':
    {
      
       
       wp_clear_client.call(waypoint_clear);
       if(waypoint_clear.response.success)
       ROS_WARN_STREAM("all waypoints deleted"); 
       waypoints_push_clear=true;
     
    }
  

    default:
    {

    }
  }
}




void uavLocalPoseReceived(const geometry_msgs::PoseStampedConstPtr& msg)
{
  
  //uavPose = *msg; 
  uavCurrentLocalPose.pose.position.x = msg->pose.position.x;
  uavCurrentLocalPose.pose.position.y = msg->pose.position.y;
  uavCurrentLocalPose.pose.position.z = msg->pose.position.z;
  uavCurrentLocalPose.pose.orientation.x = msg->pose.orientation.x;
  uavCurrentLocalPose.pose.orientation.y = msg->pose.orientation.y;
  uavCurrentLocalPose.pose.orientation.z = msg->pose.orientation.z;
  uavCurrentLocalPose.pose.orientation.w = msg->pose.orientation.w;


  ////////////////pose//////////
  //isSendNextPose = true;      
  //isSendNextVelocity = false;   
  //////////////////////////////  
  // Using ROS tf to get RPY angle from Quaternion
  tf::Quaternion quat;
  tf::quaternionMsgToTF(uavCurrentLocalPose.pose.orientation, quat);  
  tf::Matrix3x3(quat).getRPY(uavRollENU, uavPitchENU, uavYawENU);

  //ROS_INFO("local_position/local: Pos:[%0.3f, %0.3f, %0.3f], RPY:[%0.3f, %0.3f, %0.3f]", uavCurrentLocalPose.pose.position.x, uavCurrentLocalPose.pose.position.y, uavCurrentLocalPose.pose.position.z,uavRollENU*180/3.1415926, uavPitchENU*180/3.1415926, uavYawENU*180/3.1415926);
  //ROS_INFO("Current UAV angles: roll=%0.3f, pitch=%0.3f, yaw=%0.3f", uavRollENU*180/3.1415926, uavPitchENU*180/3.1415926, uavYawENU*180/3.1415926); 
  rosInfoTimeDealy++;
  if (rosInfoTimeDealy >=15)
  {
    ROS_INFO("local_position/local: Pos:[%0.3f, %0.3f, %0.3f], RPY:[%0.3f, %0.3f, %0.3f]", uavCurrentLocalPose.pose.position.x, uavCurrentLocalPose.pose.position.y, uavCurrentLocalPose.pose.position.z,uavRollENU*180/3.1415926, uavPitchENU*180/3.1415926, uavYawENU*180/3.1415926);
    
    updateControlCommandStats();    
    rosInfoTimeDealy = 0;
  }
/*
  if ( isSendNextPose )
  {
    double distance = sqrt((uavCurrentLocalPose.pose.position.x - uavNextPose.pose.position.x) * (uavCurrentLocalPose.pose.position.x - uavNextPose.pose.position.x)  +
    (uavCurrentLocalPose.pose.position.y - uavNextPose.pose.position.y) * (uavCurrentLocalPose.pose.position.y - uavNextPose.pose.position.y) + (uavCurrentLocalPose.pose.position.z - uavNextPose.pose.position.z) * (uavCurrentLocalPose.pose.position.z - uavNextPose.pose.position.z));
    double threshold = 0.2;
    if (distance < threshold)
    {
      ROS_INFO("Reached!!!");
      isSendNextPose = false;
      isSendNextVelocity = true;
      vs.twist.linear.x = 0.0;
      vs.twist.linear.y = 0.0;
      vs.twist.linear.z = 0.0;
      vs.twist.angular.x = 0.0;
      vs.twist.angular.y = 0.0;
      vs.twist.angular.z = 0.0;
    }
    else
    {
      double deltaX = uavCurrentLocalPose.pose.position.x - uavNextPose.pose.position.x;
      double deltaY = uavCurrentLocalPose.pose.position.y - uavNextPose.pose.position.y;
      double deltaZ = uavCurrentLocalPose.pose.position.y - uavNextPose.pose.position.z;
      ROS_INFO("Delta Pos:[%0.3f, %0.3f, %0.3f], Delta Distance:[%0.3f]", deltaX, deltaY, deltaZ, distance);
    }
  }
*/



  
}

int main(int argc, char **argv)
{
  int i=0;
  ros::init(argc, argv, "px4_velocity_control_node"); 
  ros::NodeHandle nodeHandle;

  ros::Publisher vel_sp_pub = nodeHandle.advertise<geometry_msgs::TwistStamped>("/mavros/setpoint_velocity/cmd_vel", 10);
  ros::Publisher pos_sp_pub = nodeHandle.advertise<geometry_msgs::PoseStamped>("/mavros/setpoint_position/local",10);
  ros::Subscriber state_sub = nodeHandle.subscribe<mavros_msgs::State>("mavros/state", 10, state_cb);


  isSendNextPose = false;
  isSendNextPoseInitial = false;
  isSendNextVelocity = false;

  vs.twist.linear.x = 0.0;
  vs.twist.linear.y = 0.0;
  vs.twist.linear.z = 0.0; 
  vs.twist.angular.x = 0.0;
  vs.twist.angular.y = 0.0;
  vs.twist.angular.z = 0.0;
  ////what is the difference between two group of velocity
  vs_body_axis.twist.linear.x = 0.0;
  vs_body_axis.twist.linear.y = 0.0;
  vs_body_axis.twist.linear.z = 0.0; 
  vs_body_axis.twist.angular.x = 0.0;
  vs_body_axis.twist.angular.y = 0.0;
  vs_body_axis.twist.angular.z = 0.0;


  rosInfoTimeDealy = 0;

  ros::Subscriber commandSubscriber = nodeHandle.subscribe("/keyboard/keydown", 1, sendCommand);
  ros::Subscriber uavLocalPoseSubscriber = nodeHandle.subscribe("/mavros/local_position/pose", 1000, uavLocalPoseReceived); 

  arming_client = nodeHandle.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");
  set_mode_client = nodeHandle.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");

  ////
   wp_push_client = nodeHandle.serviceClient<mavros_msgs::WaypointPush>("mavros/mission/push");
   wp_clear_client= nodeHandle.serviceClient<mavros_msgs::WaypointClear>("mavros/mission/clear");

  ////



  ros::Rate loop_rate(20.0);

  while (ros::ok())
  {

    /*if ( current_state.mode != "OFFBOARD" && (ros::Time::now() - last_request > ros::Duration(5.0))) 
    {
      if ( set_mode_client.call(offb_set_mode) && offb_set_mode.response.success) 
      {
        ROS_INFO("Offboard enabled");
      }
      last_request = ros::Time::now();
    } 
    else 
    {
      if ( !current_state.armed && (ros::Time::now() - last_request > ros::Duration(5.0))) 
      {
        if ( arming_client.call(arm_cmd) && arm_cmd.response.success) 
        {
          ROS_INFO("Vehicle armed");
        }
        last_request = ros::Time::now();
      }
    }
*/


 
   
    if (isSendNextVelocity == true)
    {
      //vs_body_axis.twist.linear.x = vs.twist.linear.x * cos(uavYawENU) + vs.twist.linear.y * sin(uavYawENU);
     // vs_body_axis.twist.linear.y = vs.twist.linear.x * sin(uavYawENU) - vs.twist.linear.y * cos(uavYawENU);
     // vs_body_axis.twist.linear.z = vs.twist.linear.z;
      vs_body_axis.twist.angular.x = vs.twist.angular.x;
      vs_body_axis.twist.angular.y = vs.twist.angular.y;
      vs_body_axis.twist.angular.z = vs.twist.angular.z;
      vs_body_axis.header.stamp = ros::Time::now();
      

      vel_sp_pub.publish(vs_body_axis);
    }
   
    if ((isSendNextPose == true) && (isSendNextPoseInitial == true))
    {
      ps.pose = uavNextPose.pose;
      ps.header.stamp = ros::Time::now();
      pos_sp_pub.publish(ps);
    }
   
    ros::spinOnce();
    loop_rate.sleep();
  }


}
