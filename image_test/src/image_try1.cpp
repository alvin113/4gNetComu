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
#include <sstream>
#include "std_msgs/String.h"
static const std::string OPENCV_WINDOW = "Image window";
char image_name[13];
int  uavinfo1=0;
int  uavinfo2=0;
int uavinfo3=0;
int uavinfo4=0;
int uavinfo5=0;
int uavinfo6=0;

void imageproce(cv::Mat* img)
{

}

/************** ImageConverter****************/
class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  int i=0;
     
public:
  ImageConverter()       
    : it_(nh_)
  {

    image_sub_ = it_.subscribe("/image_raw1", 1, &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("/image_converter/output_video1", 1);

    cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }
 
  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    cv::Mat eigImage;
    cv::Mat tempImage;
    cv::Mat prev_image;
    cv::Mat orig_image;
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
     orig_image=cv_ptr->image.clone(); //原始图像
     eigImage = prev_image.clone();    //黑白图像  
     tempImage = prev_image.clone();
     mask.size()=prev_image.size(); 
     if(uavinfo1==1)
     {
      sprintf(image_name, "%s%d%s", "1号机共享图像信息", ++i, ".jpg");//保存的图片名
      cv::imwrite(image_name,orig_image);
      image_pub_.publish(cv_ptr->toImageMsg());
      uavinfo1=0;
     }
  }
};
/************** ImageConverter****************/

/************** ImageConverter2****************/
class ImageConverter2
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  int i=0;

public:
  ImageConverter2()
    : it_(nh_)
  {

    image_sub_ = it_.subscribe("/image_converter/output_video2", 1, &ImageConverter2::imageCb, this);
   // image_pub_ = it_.advertise("/image_converter/output_video1", 1);

    cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter2()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }
  void tryCb()
  {
  }
  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    cv::Mat eigImage;
    cv::Mat tempImage;
    cv::Mat prev_image;
    cv::Mat orig_image;
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
     orig_image=cv_ptr->image.clone(); //原始图像
     eigImage = prev_image.clone();    //黑白图像
     tempImage = prev_image.clone();
     mask.size()=prev_image.size();
     sprintf(image_name, "%s%d%s", "2号机共享图像信息", ++i, ".jpg");//保存的图片名
      cv::imwrite(image_name,orig_image);

       }
};
/************** ImageConverter2****************/

/************** ImageConverter3****************/
class ImageConverter3
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  int i=0;

public:
  ImageConverter3()
    : it_(nh_)
  {

    image_sub_ = it_.subscribe("/image_converter/output_video3", 1, &ImageConverter3::imageCb, this);
   // image_pub_ = it_.advertise("/image_converter/output_video1", 1);

    cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter3()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }
  void tryCb()
  {
  }
  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    cv::Mat eigImage;
    cv::Mat tempImage;
    cv::Mat prev_image;
    cv::Mat orig_image;
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
     orig_image=cv_ptr->image.clone(); //原始图像
     eigImage = prev_image.clone();    //黑白图像
     tempImage = prev_image.clone();
     mask.size()=prev_image.size();
     sprintf(image_name, "%s%d%s", "3号机共享图像信息", ++i, ".jpg");//保存的图片名
      cv::imwrite(image_name,orig_image);

       }
};
/************** ImageConverter3****************/
/************** ImageConverter4****************/
class ImageConverter4
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  int i=0;
 public:
  ImageConverter4()
    : it_(nh_)
  {

    image_sub_ = it_.subscribe("/image_converter/output_video4", 1, &ImageConverter4::imageCb, this);
   // image_pub_ = it_.advertise("/image_converter/output_video1", 1);

    cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter4()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }
  void tryCb()
  {
  }
  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    cv::Mat eigImage;
    cv::Mat tempImage;
    cv::Mat prev_image;
    cv::Mat orig_image;
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
     orig_image=cv_ptr->image.clone(); //原始图像
     eigImage = prev_image.clone();    //黑白图像
     tempImage = prev_image.clone();
     mask.size()=prev_image.size();
     sprintf(image_name, "%s%d%s", "4号机共享图像信息", ++i, ".jpg");//保存的图片名
      cv::imwrite(image_name,orig_image);

       }
};
/************** ImageConverter4****************/


void KeyboardCallback(const geometry_msgs::Twist & msg)
{  
   if(msg.linear.y==0.1)   //目标出现在1号机视野内
    {
     uavinfo1=1;
     ROS_INFO("uavinfo1= %d",uavinfo1);
    }
    if(msg.linear.y==0.2)   //目标出现在2号机视野内
     {
     uavinfo2=1;
     ROS_INFO("uavinfo2= %d",uavinfo2); 
     }
    if(msg.linear.y==0.3)   //目标出现在3号机视野内
     {
     uavinfo3=1;
     ROS_INFO("uavinfo3= %d",uavinfo3);
     }
    if(msg.linear.y==0.4 ) //目标出现在4号机视野内
     {
     uavinfo4=1;
     ROS_INFO("uavinfo4= %d",uavinfo4);
      }
       if(msg.linear.y==0.5)   //waypoint model forward (counter clockwise)
     {
     uavinfo5=1;
     ROS_INFO("uavinfo5= %d",uavinfo5);
      //sleep(1);
     }
    if(msg.linear.y==0.6 ) //
     {
     uavinfo6=1;
     ROS_INFO("uavinfo6= %d",uavinfo6);
      }

}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_try1");
  ros::NodeHandle n;
  ImageConverter ic;
  ImageConverter2 ic2;
  ImageConverter3 ic3;
  ImageConverter4  ic4;
  int pos=1;

  ros::Subscriber sub = n.subscribe("key_command", 1, KeyboardCallback);
  ROS_INFO_STREAM("hello  ros!");
  while(ros::ok())
  {
      if(uavinfo5==1)//--
      {
          if(pos==1)pos=4;
          else pos--;
          ROS_INFO("Please go to WayPoint  %d",pos);
          uavinfo5=0;
        }
      if(uavinfo6==1)//++
      {
          if(pos==4)pos=1;
          else pos++;
          ROS_INFO("Please go to WayPoint  %d",pos);
          uavinfo6=0;
        }

  ros::spinOnce();
  }
  return 0;
}
