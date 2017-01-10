#include "ros/ros.h"
#include "tf/transform_broadcaster.h"
#include "data4G/PlatformState.h"

#define P_zk 206264.806247096355
#define PI 3.141592653589793

void GaosZ_fun(double B,double L, double *x ,double *y);  //高斯转换

int main(int argc ,char** argv)
{
    ros::Time current_time;
    ros::init(argc,argv,"tf_pub");
    ros::NodeHandle nh;
    tf::TransformBroadcaster br;
    ros::Subscriber pos_pub = nh.subscribe<data4G::PlatformState>()
    ros::Rate rate(10);
    while(ros::ok())
    {
      
    }
}

void GaosZ_fun(double B,double L, double *x ,double *y)
{
 double l,N;
 double a0,a4,a6,a3,a5,cB2;
 //double t,V,c,n2;

B=B*3600/P_zk;
L=L*3600/P_zk;
 //e2=0.006738525414683;
 //c=6399698.901782711;


 l=L-111*3600/P_zk;
 //t=tan(B);
 //n2=e2*cos(B)*cos(B);
 //V=sqrt(1+n2);
 cB2=pow(cos(B),2);
 N=6399698.902-(21562.267-(108.973-0.612*cB2)*cB2)*cB2;   //  N=c/V;
 a0=32140.404-(135.3302-(0.7092-0.004*cB2)*cB2)*cB2;
 a4=(0.25+0.00252*cB2)*cB2-0.04166;
 a6=(0.166*cB2-0.084)*cB2;
 a3=(0.3333333+0.001123*cB2)*cB2-0.1666667;
 a5=0.0083-(0.1667-(0.1968+0.0040*cB2)*cB2)*cB2;


 *x=6367558.4969*B-(a0-(0.5+(a4+a6*l*l)*l*l)*l*l*N)*sin(B)*cos(B);
 *y=(1+(a3+a5*l*l)*l*l)*l*N*cos(B);

// printf("x=%lf\ny=%lf\n",*x,*y);
}
