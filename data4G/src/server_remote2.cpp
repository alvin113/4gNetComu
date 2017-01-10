#include <ros/ros.h>
#include <data4G/PlatformState.h>
#include <data4G/Command.h>
#include <keyboard/Key.h>
#include "data4G/PlatformState.h"
#include "tf/transform_broadcaster.h"

#include "net4all.h"

#define P_zk 206264.806247096355
#define PI 3.141592653589793
void GaosZ_fun(double B,double L, double *x ,double *y);  //高斯转换

//Global data
struct DATA
{
	int16_t ID;
	int16_t blank[3];
	int32_t TimeStamp_us;
	int32_t TimeStamp_s;
	double Longitude;
	double Latitude;
	double Altitude;
	double x;
	double y;
	double z;
	double w;
    //int latencyTest[12799];
} g_Data;

//Global Data
struct h_DATA
{
	int16_t ID;
	int16_t blank[3];
	int32_t TimeStamp_us;
	int32_t TimeStamp_s;
	uint16_t code;
} h_data;

//TCPSocket
Socket_TCP_CS* g_socket=NULL;

/******zk******/
void *thread_socket_send(void *param);
pthread_t pthread_send = -1;
bool send_run = false;

void *thread_socket_recv(void *param);
pthread_t pthread_recv = -1;
bool recv_run = false;

ros::NodeHandle *nh ;
/*****zk end *****/

keyboard::Key key;
bool key_new = false;
void KeyCallback(const keyboard::Key::ConstPtr &key_recv)
{
  key.code = key_recv->code ;
  key_new = true;
  ROS_INFO("new key!!");
}

long getCurrentTime()    
{    
   struct timeval tv;    
   gettimeofday(&tv,NULL);    
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;    
} 

int main ( int argc , char ** argv ) 
{
	unsigned short ServerPort=14782; 

	/*//Get INIFIle path
	char IniFileName[255];
	readlink("/proc/self/exe",IniFileName,255);
	int len=strlen(IniFileName);
	int index=0;
	int count=0;
	for(;(index<len)&&(count<3);index++)
	{
		if (IniFileName[index]=='/') count++;
	}
	sprintf(IniFileName+index,"server.ini");
	
	//Initialization from INIFile
	FILE* fp = fopen(IniFileName,"r");
	if (fp)
	{	
		char TempString[255];
		fgets(TempString,255,fp);
		fgets(TempString,255,fp);
		ServerPort=atoi(TempString);
		
		if ((ServerPort<0)||(ServerPort>65535))
			ServerPort=14782;
		fclose(fp);
	}*/
	/*printf("argc = %d\n",argc);
	for(int k=0;k<argc;k++)
	  printf("%s\t",argv[k]);
	printf("\n");*/
	if(argc >1)  ServerPort = atoi(argv[1]);
	
	//TCP initialization
	g_socket=new Socket_TCP_CS(ServerPort);
	printf("Server start listening at port : %d...\n",ServerPort); 
	g_socket->accept_connection();
	printf("Client connected!\n"); 
	
	//ROS initialization
	ros::init ( argc , argv , "ServerRemote2" ) ;

/*******zk*******/
        nh = new ros::NodeHandle ;
	  send_run = true;
	  recv_run = true;
	  int ret = pthread_create(&pthread_send, NULL, thread_socket_send, NULL);
	  pthread_create(&pthread_recv, NULL, thread_socket_recv, NULL);
	  if (ret < 0) {
	    printf("Fail to creat pthread");
	    return -2;
	  }
	  while(ros::ok())
	  {

	  }
	  send_run = false;
	  recv_run = false;
	  ret = pthread_join(pthread_send, NULL);
	  if(ret == 0) printf("Send pthread has over!\n");
	  ret = pthread_join(pthread_recv, NULL);
	  if(ret == 0) printf("Recv pthread has over!\n");
/*******zk end*******/
	
	g_socket->close_connection();
}

/*******zk********/
void *thread_socket_send(void *param)
{  
  ros::Subscriber Key_suber=nh->subscribe<keyboard::Key>("/keyboard/keydown",100, &KeyCallback);
//  double x=10;y=20;z=30;
  ros::Rate loop_rate(10);
  unsigned int seq = 0;
  while(send_run)
  {
    ros::spinOnce();
  /*  data4G::Command command;
    x+=0.01;y+=0.01,z+=0.01;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    command.ID = seq++;
    command.TimeStamp_us = tv.tv_usec;
    command.TimeStamp_s = tv.tv_sec;
    command.x = x ;
    command.y = y ;
    command.z = z ;
    int size = g_socket->send_data((char*)(&command),sizeof(command));
    if (size<=0)
    {
      printf("Client connection lost , waiting for reconnect...\n"); 
   
      usleep(100000);
      int ret=g_socket->accept_connection();
      printf("ret=%d\n",ret);
    }
    ROS_INFO("ID:%d x:%f y:%f z:%f\n",command.ID,command.x,command.y,command.z);*/
    if(key_new == false) continue;
    key_new = false;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    h_data.ID = seq++;
    h_data.TimeStamp_us = tv.tv_usec;
    h_data.TimeStamp_s = tv.tv_sec;
    h_data.code = key.code;
    int size = g_socket->send_data((char*)(&h_data),sizeof(h_data)); 
    /*if (size<=0)
    {
      printf("send Client connection lost , waiting for reconnect...\n"); 
   
      usleep(100000);
      int ret=g_socket->accept_connection();
      printf("ret=%d\n",ret);
    }*/
    ROS_INFO("ID:%d code:%d\n",h_data.ID,h_data.code);
    loop_rate.sleep();
  }
  //printf("Send pthread has over!\n");
}
/***********zk end******/

//将接收程序转移到了进程里
void *thread_socket_recv(void *param)
{
        double x,y;
        double Origin_x = 0, Origin_y = 0;
        double map_x,map_y;
        ros::Publisher PlatformState_puber  = nh->advertise<data4G::PlatformState>("PlatformStateRemote",1000);
        tf::TransformBroadcaster br;  //发布TF
	ros::Rate loop_rate(20); 

	char filename[80];
 	char str[80];
 	time_t t = std::time(0);////
 	struct tm * now = std::localtime(&t);////
 	sprintf(filename,"%d%s%d%s%d%s%d%s%d%s%s",now->tm_year+1900,"-",now->tm_mon+1,"-",now->tm_mday,"-",now->tm_hour,"-",now->tm_min,"remote",".txt");
 	FILE* fp2=NULL;
 	fp2 = fopen(filename,"w");  
	while(recv_run)  
	{     
		int size=g_socket->recv_data((char*)(&g_Data),sizeof(DATA));
		//if connection Lost...
		if(size<=0)  
		{  
			printf("recv Client connection lost , waiting for reconnect...\n");  
			usleep(100000);
			int ret=g_socket->accept_connection();
			printf("recv ret=%d\n",ret);
		} 
		if (size!=sizeof(DATA)) continue;
		
		//Copy data to ROS
		data4G::PlatformState state;
		state.ID=g_Data.ID;
		state.TimeStamp_us=g_Data.TimeStamp_us;
		state.TimeStamp_s=g_Data.TimeStamp_s;
		state.Longitude=g_Data.Longitude;
		state.Latitude=g_Data.Latitude;
		state.Altitude=g_Data.Altitude;
		state.x=g_Data.x;
		state.y=g_Data.y;
		state.z=g_Data.z;
		state.w=g_Data.w;
		PlatformState_puber.publish(state);  

		//DEBUG
		ROS_INFO("ID:%d L:%f B:%f Z:%f x:%f y:%f z:%f w:%f", state.ID,state.Longitude,state.Latitude,state.Altitude,state.x,state.y,state.z,state.w);  


		ros::Time current_time;
		current_time = ros::Time::now();
        fprintf(fp2,"%ld\t%d%03d\t%d%03d\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",getCurrentTime(),current_time.sec,int(current_time.nsec/1000000),g_Data.TimeStamp_s,int(g_Data.TimeStamp_us/1000),g_Data.ID,g_Data.Latitude,g_Data.Longitude,g_Data.Altitude,g_Data.x,g_Data.y,g_Data.z,g_Data.w);

                ////发布TF
                GaosZ_fun(state.Latitude,state.Longitude,&x, &y);
                if(Origin_x ==0 && Origin_y == 0)
                {
                  Origin_x = x;
                  Origin_y = y;
                }
                map_x=x-Origin_x;
                map_y=y-Origin_y;
                map_y=-map_y;
                //current_time=ros::Time::now();
                tf::Quaternion ori = tf::Quaternion(state.x,state.y,state.z,state.w);
                tf::Vector3 xy=tf::Vector3(map_x,map_y,state.Altitude);
                br.sendTransform(
                        tf::StampedTransform(
                            tf::Transform(ori,xy),
                            ros::Time::now(),"odom","base_link"));

                
		ros::spinOnce();  
		loop_rate.sleep();  
    	}  
    	fclose(fp2);
}

void GaosZ_fun(double B,double L, double *x ,double *y)
{
  double l,N;
  int L1;//中央经线
  double a0,a4,a6,a3,a5,cB2;
 //double t,V,c,n2;

  B=B*3600/P_zk;
  L=L*3600/P_zk;
  //e2=0.006738525414683;
  //c=6399698.901782711;

  L1 = 6*(L/6+1)-3;
  l=L-L1*3600/P_zk;
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
