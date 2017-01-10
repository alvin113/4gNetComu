#include <ros/ros.h>
#include <data4G/PlatformState.h>


#include "std_msgs/String.h"
#include "sensor_msgs/Imu.h"
#include "sensor_msgs/NavSatFix.h"
#include <sstream>
#include <sys/time.h>    



//Test Data
double tL=120;
double tB=25;
double tZ=999;
double tX=0;
double tY=0;
double tz=0;
double tW=1;
/*
long getCurrentTime()    
{    
   struct timeval tv;    
   gettimeofday(&tv,NULL);    
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;    
}    
*/
void imudatarawCallback(const sensor_msgs::Imu & msg)
{
	/*
    ROS_INFO_STREAM("rceivied Imu/data-raw:"
                    <<"header seg"
                    <<msg.header.seq
              	    <<"angular_velocity"
	            <<"x:" <<msg.angular_velocity.x	
                   
                    <<"y:" <<msg.angular_velocity.y
                    
                    <<"z:" <<msg.angular_velocity.z

                    );
  */
   tX=double(msg.orientation.x);
   tY=double(msg.orientation.y);
   tz=double(msg.orientation.z);
   tW=double(msg.orientation.w);
   
}

void globalPosCallback(const sensor_msgs::NavSatFix & msg2)
{/*
    ROS_INFO_STREAM("rceivied /mavros/global_position/global:"
                    <<"header seg"
                    <<msg2.header.seq
              	    <<"global_position"
	            <<"latitude:" <<msg2.latitude	
                   
                    <<"longitude:" <<msg2.longitude
                    
                    <<"altitude:" <<msg2.altitude

                    );
  */
   tB=double(msg2.latitude);
   tL=double(msg2.longitude);
   tZ=double(msg2.altitude);

}

int main ( int argc , char ** argv ) 
{
	ros::init ( argc , argv , "PlatformServer" ) ;

	ros::NodeHandle nh ;
        ros::NodeHandle n;

	ros::Publisher  PlatformState_puber  =nh.advertise<data4G::PlatformState>("PlatformState",1);
        ros::Subscriber sub = n.subscribe("mavros/imu/data", 1, imudatarawCallback);  //imudata
	ros::Subscriber pos_sub = n.subscribe("/mavros/global_position/global", 1, globalPosCallback);
 	ros::Rate loop_rate(5);
 	int seq = 0;
 	
 	//strcpy(filename,"~/");
 	/*
 	sprintf(str,"%d",now->tm_year+1900); strcpy(filename,str);
 	sprintf(str,"%d",now->tm_mon+1); strcpy(filename,str);
 	sprintf(str,"%d",now->tm_mday) ; strcpy(filename,str);
 	sprintf(str,"%d",now->tm_hour); strcpy(filename,str);
 	sprintf(str,"%d",now->tm_min); strcpy(filename,str);
 	strcpy(filename,".txt");
 	*/
 	
 	
 	//printf("%s",filename);
 	/*
 	FILE* fp=NULL;
 	fp = fopen(filename2,"r");
 	if(fp=NULL)
 	{
		printf("create txt file failed");
	}
	else
	{
 	//fclose(fp);
 	///printf("%s",filename2);
    }

 	fclose(fp);
*/
	//Test Data
	//double tL=120;
	//double tB=25;
	//double tZ=999;
	//double tH=0;
	//double tP=0;
	//double tR=0;	
   //timeval  start,stop,diff;
   //gettimeofday(&start,0);
	while (ros::ok())  
  	{ 
	 	struct timeval tv;
		gettimeofday(&tv, NULL);

		data4G::PlatformState state;
		state.ID=seq++;
		state.TimeStamp_us=tv.tv_usec;
		state.TimeStamp_s=tv.tv_sec;
		state.Longitude=tL;
		state.Latitude=tB;
		state.Altitude=tZ;
        state.x=tX;
        state.y=tY;
        state.z=tz;
        state.w=tW;
		PlatformState_puber.publish(state);  
	
		//ROS_INFO("ID:%d L:%f B:%f Z:%f H:%f P:%f R:%f", state.ID,state.Longitude,state.Latitude,state.Altitude,state.Heading,state.Pitching,state.Rolling);  

		//tL+=0.001;
		//tB+=0.001;
		//tZ+=0.001;
		//tH+=0.001;
		//tP+=0.001;
		//tR+=0.001;
       // printf("%s",filename);
		
        ROS_INFO("c/c++ program:%ld\n",(tv.tv_usec*1000+tv.tv_sec/1000));
        
        //ROS_INFO("%f\n",start.tv_usec);
    		ros::spinOnce();  
    		loop_rate.sleep();  
	}
}
