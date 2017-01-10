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
int main(int argc,char **argv)
{
 ros::init(argc,argv,"hello_ros");
 ros::NodeHandle nh;
 image_transport::ImageTransport it(nh); 
  ROS_INFO_STREAM("hello  ros!");
  //cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
  cv::Mat num=cv::imread("1.JPG");
  cv::namedWindow("logo");
  cv::imshow("logo",num);
  cv::waitKey(3);
  ros::spin();
}
