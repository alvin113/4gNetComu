#include <ros/ros.h>
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

static const std::string OPENCV_WINDOW = "Image window";
bool pub_con=true;
cv_bridge::CvImagePtr cv_ptr;
int  CameraID = 0;
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
      break;
    }

        default:
    {

    }
  }
}


/////////////////////usb_cam0///////////////////////////
class ImageConverter00
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  
public:
  ImageConverter00()       
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/usb_cam0/image_raw", 1, &ImageConverter00::imageCb, this);
    image_pub_ = it_.advertise("/usb_cam/image_raw", 1);///usb_cam/image_raw

   // cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter00()
  {
    //cv::destroyWindow("Image window_usb_cam0");
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
   
    cv_bridge::CvImagePtr cv_ptr00;
    cv::Mat eigImage;
    cv::Mat tempImage;
    cv::Mat prev_image;
    cv::Mat mask;
    std::vector<cv::Point2f> corners;
    
    try
    {
      cv_ptr00 = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;   
    }
     cv::cvtColor(cv_ptr00->image,prev_image,CV_BGR2GRAY);
     eigImage = prev_image.clone(); 
     tempImage = prev_image.clone();
     mask.size()=prev_image.size();
          
     // Output modified video stream  
     
    if(pub_con==true)
     
    {
    //cv_ptr = cv_ptr00;
    //std::cout<<"00"<<std::endl;
    //cv::imshow("Image window_usb_cam0", cv_ptr00->image);
    //cv::waitKey(3);
    image_pub_.publish(cv_ptr00->toImageMsg());
        //pub_con=false;
      
    }
  } 

};
/////////////////////usb_cam0///////////////////////////

/////////////////////usb_cam1///////////////////////////
class ImageConverter01
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  
public:
  ImageConverter01()       
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/usb_cam1/image_raw", 1, &ImageConverter01::imageCb, this);
    image_pub_ = it_.advertise("/usb_cam/image_raw", 1);///usb_cam/image_raw

   // cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter01()
  {
   // cv::destroyWindow("Image window_usb_cam1");
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    //std::cout<<"01"<<std::endl;
    cv_bridge::CvImagePtr cv_ptr00;
    cv::Mat eigImage;
    cv::Mat tempImage;
    cv::Mat prev_image;
    cv::Mat mask;
    std::vector<cv::Point2f> corners;
    
    try
    {
      cv_ptr00 = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;   
    }
     cv::cvtColor(cv_ptr00->image,prev_image,CV_BGR2GRAY);
     eigImage = prev_image.clone(); 
     tempImage = prev_image.clone();
     mask.size()=prev_image.size();
    

       
     // Output modified video stream  
     
      if(pub_con==false)
      {
      //cv_ptr = cv_ptr00;
      //std::cout<<"01"<<std::endl;
      //cv::imshow("Image window_usb_cam1", cv_ptr00->image);
      //cv::waitKey(3);
      image_pub_.publish(cv_ptr00->toImageMsg());
       //pub_con=false;
      
      }
  }

};
/////////////////////usb_cam1///////////////////////////

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ros::NodeHandle nh;
  ros::Subscriber commandSubscriber = nh.subscribe("/keyboard/keydown", 1, sendCommand);
/*
  ros::NodeHandle nodeHandle;////
  image_transport::ImageTransport im;
  //image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

  image_pub_ = im.advertise("/usb_cam/image_raw", 1);///usb_cam/image_raw
*/
  ImageConverter00 ic0;
  ImageConverter01 ic1;
 // ImageConverter ic;
   
/*
ros::Rate loop_rate(20.0);
  while (ros::ok())
 {

   image_pub_.publish(cv_ptr->toImageMsg());

    ros::spinOnce();
    loop_rate.sleep();
 }
*/
 ros::spin();
  return 0;
}







