#include <ros/ros.h>
#include <data4G/PlatformState.h>

//平台状态信息回调
void PlatformStateCallback(const data4G::PlatformState::ConstPtr& state)  
{ 
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int DeltaTime=(tv.tv_sec-state->TimeStamp_s)*1000000+tv.tv_usec-state->TimeStamp_us;

	ROS_INFO("ID:%d TimeDelay:%d L:%f B:%f", state->ID,DeltaTime,state->Longitude,state->Latitude);  
}

int main ( int argc , char ** argv ) 
{
	ros::init ( argc , argv , "PlatformClientRemote" ) ;

	//ROS
	
	ros::NodeHandle nh ;
	ros::Subscriber PlatformState_suber=nh.subscribe<data4G::PlatformState>("PlatformStateRemote",10, &PlatformStateCallback);
	
 	ros::Rate loop_rate(100);  
	while (ros::ok())  
  	{ 
		ros::spinOnce();  
		loop_rate.sleep();  
	}
}
