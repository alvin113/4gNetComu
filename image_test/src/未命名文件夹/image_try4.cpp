#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include "geometry_msgs/Twist.h"


static const std::string OPENCV_WINDOW = "Image window";
int i=0;
char image_name[13];

int uavinfo1=0;
int uavinfo2=0;
int uavinfo3=0;
int uavinfo4=0;
void imageproce(cv::Mat* img)
{

}
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
    //image_sub_ = it_.subscribe("/image_raw", 1, &ImageConverter::imageCb, this);
    image_sub_ = it_.subscribe("/image_converter/output_video4", 1, &ImageConverter::imageCb, this);
    //image_pub_ = it_.advertise("/image_converter/output_video1", 1);

    cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }
  void tryCb()
  {
  }
  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
   // std::cout<<"11"<<std::endl;
    cv_bridge::CvImagePtr cv_ptr;
    cv::Mat eigImage;
    cv::Mat tempImage;
    cv::Mat prev_image;
    cv::Mat orig_image;
    cv::Mat mask;
    std::vector<cv::Point2f> corners;
    //cv::OutputArray corners;
    //cv::Point2f corners[500];
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
     orig_image=cv_ptr->image.clone(); //原始图像
     eigImage = prev_image.clone();    //黑白图像  
     tempImage = prev_image.clone();
     mask.size()=prev_image.size();
    //Goodfeatures to track GoodFeaturesToTrack(const CvArr* image,CvArr* eigImage, CvArr* tempImage,CvPoint2D32f* corners,int*     cornerCount,double qualityLevel, double minDistance, const CvArr* mask=NULL,int blockSize=3,int useHarris=0,double k=0.04 ); 
     //cv::goodFeaturesToTrack(prev_image,corners,500,0.01,12,mask,3,1,0.04);
       
    // Draw an example circle on the video stream
    //if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
     // cv::circle(eigImage, cv::Point(50, 50), 10, CV_RGB(255,0,0));
    //for(int i=0; i< corners.size();i++)
     //cv::circle(cv_ptr->image, cv::Point(corners[i].x, corners[i].y), 10, CV_RGB(255,0,0));

    // Update GUI Window
    //cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::imshow(OPENCV_WINDOW, orig_image);
    cv::waitKey(3);
    
   //write image to file
    sprintf(image_name, "%s%d%s", "4号机共享图像信息", ++i, ".jpg");//保存的图片名
    // printf("image_name\n");
    cv::imwrite(image_name,orig_image);
    //cv::imshow("保存的图像", orig_image);
    //Output modified video stream
    //image_pub_.publish(cv_ptr->toImageMsg());
  }
};

void KeyboardCallback(const geometry_msgs::Twist & msg)
{/*
    if(msg.linear.x==2) 
    {
     uavinfo1=1;
     ROS_INFO("uavinfo1= %d",uavinfo1);
    //sleep(1);
    }
    if(msg.linear.x==(-2))   //目标出现在2号机视野内
     {
     uavinfo2=1;
     ROS_INFO("uavinfo2= %d",uavinfo2); 
     //sleep(1);
     }
    if(msg.angular.z==2)   //目标出现在3号机视野内
     {
     uavinfo3=1;
     ROS_INFO("uavinfo3= %d",uavinfo3);
      //sleep(1);
     }
    if(msg.angular.z==(-2))  //目标出现在4号机视野内
     {
     uavinfo4=1;
     ROS_INFO("uavinfo4= %d",uavinfo4);
     //sleep(1);
    
     }

*/
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_try4");
  ros::NodeHandle n;
  ImageConverter ic;
  
  ros::Subscriber sub = n.subscribe("/turtle1/cmd_vel", 1, KeyboardCallback);
  ROS_INFO_STREAM("hello  ros!");
  
  ros::spin();
  return 0;
}
