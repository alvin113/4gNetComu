#include <ros/ros.h>
#include <data4G/PlatformState.h>
#include <data4G/Command.h>
#include <pthread.h>
#include <string.h>
#include "net4all.h"

//无人机端接收数据的测试程序
//尝试打开两个线程分别进行收发

//TCP Socket
Socket_TCP_CS* g_socket=NULL;

void *thread_socket_recv(void * socket);
pthread_t pthread_recv = -1;
bool recv_run = false;

void *thread_socket_send(void * arg);
pthread_t pthread_send = -1;
bool send_run = false;

data4G::PlatformState g_Data;
//数据更新标志
bool g_HasNewData=false;

//传递给线程的形参
struct ARG
{
  class Socket_TCP_CS* socket;
  ros::NodeHandle* nh; 
} arg;

//MSG:PlatformState Subscribe Callback Function
void PlatformStateCallback(const data4G::PlatformState::ConstPtr& state)  
{
	//DEBUG
    ROS_INFO("ID:%d L:%f B:%f Z:%f H:%f P:%f R:%f", state->ID,state->Longitude,state->Latitude,state->Altitude,state->Heading,state->Pitching,state->Rolling);
	
	//Copy Data From Global Data
    memcpy(&g_Data,&(state->ID),sizeof(g_Data));
	g_HasNewData=true;
}

int main( int argc , char ** argv)
{
  char ServerIP[16]="127.0.0.1";
  unsigned short ServerPort=14782; 
  //Get INIFIle path
  char IniFileName[255];
  readlink("/proc/self/exe",IniFileName,255);
  int len=strlen(IniFileName);
  int index=0;
  int count=0;
  for(;(index<len)&&(count<3);index++)
  {
    if (IniFileName[index]=='/') count++;
  }
  sprintf(IniFileName+index,"client.ini");
  
  //Initialization File
  FILE* fp = fopen(IniFileName,"r");
  if (fp)
  {	
    char TempString[255];
    fgets(TempString,255,fp);
    fgets(TempString,255,fp);
    if (strlen(TempString)>0)
      TempString[strlen(TempString)-1]=0;
    if ((TempString[0]>='0')&&(TempString[0]<='9'))
      sprintf(ServerIP,"%s",TempString);	
    fgets(TempString,255,fp);
    fgets(TempString,255,fp);
    ServerPort=atoi(TempString);
    if ((ServerPort<0)||(ServerPort>65535))
      ServerPort=14782;
    fclose(fp);
  }
  
  //TCP Initialization
  printf("Connecting to server at %s:%d...\n",ServerIP,ServerPort); 
  g_socket=new Socket_TCP_CS(ServerIP,ServerPort);
  bool connected=false;
  while (!connected)
  {
    connected=g_socket->connect_to_server();
    usleep(1000000);
  }
  printf("Connected to server!\n"); 
  
  //ROS Initialization
  ros::init ( argc , argv , "CommandClient" ) ;
  ros::NodeHandle nh ;
  //Subscribe MSG:PlatformState
  ros::Subscriber PlatformState_suber=nh.subscribe<data4G::PlatformState>("PlatformState",10, &PlatformStateCallback);
  printf("Waiting for PlatformState from ROS...\n"); 

  ros::Rate loop_rate(10);  
  
  recv_run = true;
  send_run = true;
  arg.socket = g_socket;
  arg.nh = &nh;
  int ret = pthread_create(&pthread_recv, NULL, thread_socket_recv, (void*)g_socket);
  pthread_create(&pthread_send, NULL, thread_socket_send, (void *)&arg);
  if (ret<0){
    printf("Fail to creat pthread");
    return -2;
  }
  while(ros::ok())
  {

  }
  recv_run = false;
  send_run = false;
  ret = pthread_join(pthread_recv, NULL);
  if(ret == 0) printf("Recv pthread has over!\n");
  ret = pthread_join(pthread_send, NULL);
  if(ret == 0) printf("Send pthread has over!\n");
}

void *thread_socket_recv(void *socket)
{
  ros::Rate loop_rate(10);
  class Socket_TCP_CS* g_socket = (class Socket_TCP_CS*)socket;
  while(recv_run)
  {
    data4G::Command command;
    int size = g_socket->recv_data((char*)(&command),sizeof(command));
    //if connection Lost...
    if (size<=0)
    {
      printf("Connection lost ! Reconneting...\n"); 
   
      //Reconnecting
      bool connected=false;
      while (!connected)
      {
        connected=g_socket->connect_to_server();
        usleep(1000000);
      }
      printf("Reconnet successed!\n"); 
    } 
    if (size!=sizeof(command)) continue;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int DeltaTime = (tv.tv_sec-command.TimeStamp_s)*1000000+tv.tv_usec-command.TimeStamp_us;
    ROS_INFO("ID:%d TimeDelay:%d x:%f y:%f z:%f\n",command.ID,DeltaTime,command.x,command.y,command.z);
    loop_rate.sleep();
  }
  //printf("Recv pthread has over!\n");
}

void *thread_socket_send(void *arg)
{
  ros::Rate loop_rate(10);
  
  class Socket_TCP_CS* g_socket = ((struct ARG*)(arg))->socket;
  ros::NodeHandle* nh = ((struct ARG*)(arg))->nh;

  while(send_run)
  {
    //若数据更新，则发送
    if (g_HasNewData)
    {
      g_HasNewData=false;
      //Sending Data
      int size=g_socket->send_data((char*)&g_Data,sizeof(g_Data));  
      
      //if Connection Lost...
      if (size<=0)
      {
        printf("Connection lost ! Reconneting...\n"); 
   
        //Reconnecting
        bool connected=false;
        while (!connected)
        {
          connected=g_socket->connect_to_server();
	  usleep(1000000);
        }
        printf("Reconnet successed!\n"); 
      }  	
    }
    
    ros::spinOnce();
    loop_rate.sleep();
  }
  //printf("Send pthread has over!\n");
}
