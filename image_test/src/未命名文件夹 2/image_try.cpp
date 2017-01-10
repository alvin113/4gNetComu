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
int i=0;
char image_name[13];
int uavinfo1=0;
int uavinfo2=0;
int uavinfo3=0;
int uavinfo4=0;
void imageproce(cv::Mat* img)
{

}

/*************ImageConverter*******************/
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

    image_sub_= it_.subscribe("/image_raw1", 1, &ImageConverter::imageCb, this);

    cv::namedWindow("Image window 1");
  }

  ~ImageConverter()
  {
    cv::destroyWindow("Image window 1");
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
    cv::imshow("Image window 1", orig_image);
    cv::waitKey(3);

  }
};
/*************ImageConverter*******************/

/*************ImageConverter2*******************/
class ImageConverter2
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

  public:
  ImageConverter2()
    : it_(nh_)
  {

    image_sub_= it_.subscribe("/image_raw2", 1, &ImageConverter2::imageCb, this);

    cv::namedWindow("Image window 2");
  }

  ~ImageConverter2()
  {
    cv::destroyWindow("Image window 2");
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
    cv::imshow("Image window 2", orig_image);
    cv::waitKey(3);

  }
};
/*************ImageConverter2*******************/

/*************ImageConverter3*******************/
class ImageConverter3
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

  public:
  ImageConverter3()
    : it_(nh_)
  {

    image_sub_= it_.subscribe("/image_raw3", 1, &ImageConverter3::imageCb, this);

    cv::namedWindow("Image window 3");
  }

  ~ImageConverter3()
  {
    cv::destroyWindow("Image window 3");
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
    cv::imshow("Image window 3", orig_image);
    cv::waitKey(3);

  }
};
/*************ImageConverter3*******************/

/*************ImageConverter4*******************/
class ImageConverter4
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

  public:
  ImageConverter4()
    : it_(nh_)
  {

    image_sub_= it_.subscribe("/image_raw4", 1, &ImageConverter4::imageCb, this);

    cv::namedWindow("Image window 4");
  }

  ~ImageConverter4()
  {
    cv::destroyWindow("Image window 4");
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
    cv::imshow("Image window 4", orig_image);
    cv::waitKey(3);

  }
};
/*************ImageConverter4*******************/
int main(int argc, char** argv)
{
  int key=-1;
  ros::init(argc, argv, "image_try");
  ros::NodeHandle n;
  ImageConverter ic;
  ImageConverter2 ic2;
  ImageConverter3 ic3;
  ImageConverter4 ic4;


  ros::Publisher comd_pub = n.advertise<geometry_msgs::Twist>("key_command", 1000);
  ROS_INFO_STREAM("hello  ros!");
  while(ros::ok())
  {
         geometry_msgs::Twist key_msg;
      std::stringstream ss;
      key=cv::waitKey(300);
       if(key>0)
      {
      printf("输入字符为%c\n",key);
            switch(key)
      {
      case '1':key_msg.linear.y = 0.1;  printf("发布的数字为 %f\n", key_msg.linear.y);break;   //uav No.1 take a picture
      case '2':key_msg.linear.y = 0.2;break;   //uav No.2 take a picture
      case '3':key_msg.linear.y=0.3; printf("发布的数字为 %f\n", key_msg.linear.y);break;  //uav No.3 take a picture
      case '4':key_msg.linear.y=0.4; break;  //uav No.4 take a picture
      case 'w': key_msg.linear.y=0.5; break;  //waypoint model forward (counter clockwise)
      case 's':key_msg.linear.y=0.6; break;   //waypoint model backward  (clockwise)
      default:  break;
       }
      comd_pub .publish(key_msg);
      key=-1;
      }
     ros::spinOnce();

}
 return 0;
}
