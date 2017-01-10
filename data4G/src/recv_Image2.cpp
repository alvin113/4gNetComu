#include <ros/ros.h>
#include <data4G/PlatformState.h>
#include <std_msgs/Header.h>
#include <data4G/Command.h>
#include <keyboard/Key.h>

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <vector>

#include "std_msgs/String.h"
#include <sstream>
#include <keyboard/Key.h>

#include "net4all.h"
#include <ctime>
#include <time.h>
#include <stdio.h>

#include   <stdlib.h>
#include   <sys/time.h>

struct DATA
{
	int16_t ID;
	unsigned char  blank[320]; //  640/2=320
} g_Data;

cv::Mat M(240,320,CV_8UC(1));

Socket_TCP_CS* g_socket=NULL;//TCPSocket

int main ( int argc , char ** argv ) 
{
	ros::init ( argc , argv , "recv_Image2" ) ;
    ros::NodeHandle nh;

    int rowNumber = 240;
    int colNumber = 320;
	//unsigned short ServerPort=11858; 
	unsigned short ServerPort=14560; 
	if(argc >1)  ServerPort = atoi(argv[1]);
	

	g_socket=new Socket_TCP_CS(ServerPort);
	printf("Server start listening at port : %d...\n",ServerPort); 
	g_socket->accept_connection();
	printf("Client connected!\n"); 


	ros::Rate loop_rate(100); 

	while(ros::ok())
    {


    int size=g_socket->recv_data((char*)(&g_Data),sizeof(DATA));
    //if connection Lost...
	if(size<=0)  
	  {  
	  printf("Client connection lost , waiting for reconnect...\n");  
	  usleep(100000);
	  int ret=g_socket->accept_connection();
	  printf("ret=%d\n",ret);
	  } 
	if (size!=sizeof(DATA)) continue;
    
  
     uchar* dataM = M.ptr<uchar>(g_Data.ID);
     for (int j=0;j<colNumber;j++)
     {
       dataM[j]=g_Data.blank[j];    
     }
     ROS_INFO("g_Data.ID = %d",g_Data.ID);
     if(g_Data.ID ==(rowNumber-1)) 
    {
      cv::imshow("image M", M);
      cv::waitKey(3);
    }

    ros::spinOnce();  
	loop_rate.sleep(); 

    }	

    g_socket->close_connection();////!!!!
}