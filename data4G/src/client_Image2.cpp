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

int CameraID=3; //2 for send image 
bool pub_con=false;
bool sendOver=true;
cv::Mat eigImage;
cv::Mat M(240,320,CV_8UC(1));

unsigned char a;
struct DATA
{
	int16_t ID;
	unsigned char  blank[320]; //  640/2=320
} g_Data;

//for data seperate
int DataCountTime=0;

//TCP Socket
Socket_TCP_CS* g_Socket=NULL;

//数据更新标志
bool g_HasNewData=false;

//Get system time 
long getCurrentTime()    
{    
   struct timeval tv;    
   gettimeofday(&tv,NULL);    
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;    
} 

void sendCommand(const keyboard::Key &key)
{
  switch (key.code)
  {
    case '0':
    {
      CameraID = 0;
      pub_con=true;
      ROS_INFO("0 PRESSS DOWN");
      break;
    }
    case '1':
    {
      CameraID = 1;
      
      ROS_INFO("1 PRESSS DOWN");
      pub_con=false;
      break;
    }
    case '2':
    {
      CameraID = 2;
      pub_con=true;
      ROS_INFO("2 PRESSS DOWN");
      break;
    }

      default:
    {

    }
  }
}

/////////////
class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  
public:
  ImageConverter()       
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/usb_cam0/image_raw", 1, &ImageConverter::imageCb, this);
    //image_pub_ = it_.advertise("/usb_cam/image_raw", 1);///usb_cam/image_raw

   // cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter()
  {
    cv::destroyWindow("Image window_usb_cam0");
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
   
    cv_bridge::CvImagePtr cv_ptr;
    
    cv::Mat tempImage;
    cv::Mat prev_image;
    cv::Mat mask;
    std::vector<cv::Point2f> corners;
    
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;   
    }
     cv::cvtColor(cv_ptr->image,prev_image,CV_BGR2GRAY);
     //eigImage = prev_image.clone(); 
     tempImage = prev_image.clone();
      mask.size()=prev_image.size();
          
  
    //M=prev_image.clone(); 
    //cv::imshow("MAT", M);
    //cv::waitKey(3);
      //if(sendOver==false)
      	//ROS_INFO("sendOver = false");
   if((pub_con==true)&&(sendOver==true))
     
    {
    //cv_ptr = cv_ptr00;
    //std::cout<<"00"<<std::endl;
    eigImage = prev_image.clone(); 
    g_HasNewData = true;

   /*
    int rowNumber = eigImage.rows;
    int colNumber = eigImage.cols;
    
    
   for(int i=0;i<rowNumber;i++)
   {
   	uchar* data = eigImage.ptr<uchar>(i);
   	uchar* dataM = M.ptr<uchar>(i);
    for(int j=0;j<colNumber;j++)
    {   
    	g_Data.blank[j]=data[j];
    	dataM[j]=g_Data.blank[j];
    	
    }

   }
   
*/
    cv::imshow("Image window_usb_cam0", eigImage);
    // cv::imshow("M ", M);
    cv::waitKey(10);
    //image_pub_.publish(cv_ptr00->toImageMsg());
    pub_con=false;
    sendOver = false;
    

      
    }


  } 

};
//MSG:PlatformState Subscribe Callback Function





int main ( int argc , char ** argv ) 
{  
    ros::init ( argc , argv , "client_Image2") ;
	ros::NodeHandle nh ;

	//char ServerIP[16]="103.44.145.247";
	//unsigned short ServerPort=11858; ////11858

	char ServerIP[16]="127.0.0.1";
	unsigned short ServerPort=14560; ////11858

    g_Data.ID=-1;
    if(argc > 1) {ServerPort = atoi(argv[1]);}                           
	printf("Connecting to server at %s:%d...\n",ServerIP,ServerPort);  //TCP Initialization
	g_Socket=new Socket_TCP_CS(ServerIP,ServerPort);
	bool connected=false;
	while (!connected)
	{
		connected=g_Socket->connect_to_server();
		usleep(1000000);
	}
    printf("Connected to server!\n"); 
	


	ros::Subscriber commandSubscriber = nh.subscribe("/keyboard/keydown", 1, sendCommand);
    ImageConverter ic;


	ros::Rate loop_rate(100);
	
	////



	while (ros::ok())  
  	{   
  
       int rowNumber = eigImage.rows;
       int colNumber = eigImage.cols;
		
	    if(g_Data.ID==-1)
	   {
		   g_HasNewData=false;  ////all Mat data send
		   //g_Data.ID=0;
		   
          if(sendOver==false)
          {
          	g_HasNewData=true;
          }

          
	   }
			
        //若数据更新，则发送
		if (g_HasNewData)
		{   
			g_Data.ID = g_Data.ID+1;
            sendOver==false;
            uchar* data = eigImage.ptr<uchar>(g_Data.ID); 
                        
            for (int i=0;i<colNumber;i++)
            {
             g_Data.blank[i]=data[i];
             
            }
             uchar* dataM = M.ptr<uchar>(g_Data.ID);
            for (int j=0;j<colNumber;j++)
            {
                dataM[j]=g_Data.blank[j];
             
            }

            
			//Sending Data
			int size=g_Socket->send_data((char*)&g_Data,sizeof(DATA));  
			//if Connection Lost...
			if(size<=0)
			{
			   printf("Connection lost ! Reconneting...\n");   
			   //Reconnecting
			   bool connected=false;
			   while (!connected)
			   {
				connected=g_Socket->connect_to_server();
				usleep(1000000);
			   }
			   printf("Reconnet successed!\n"); 
			}

            ROS_INFO("g_Data.ID= %d",g_Data.ID);
			////


            if(g_Data.ID==(rowNumber-1))
            {
               g_Data.ID=-1;
               sendOver=true;
               cv::imshow("image M", M);
               cv::waitKey(3);
            }
               
		

		}

		ros::spinOnce();  
		loop_rate.sleep();  
	}

}
