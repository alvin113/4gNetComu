#include <ros/ros.h>
#include <data4G/PlatformState.h>
#include <std_msgs/Header.h>
#include <data4G/Command.h>
#include <keyboard/Key.h>

#include "net4all.h"
#include <ctime>
#include <time.h>
#include <stdio.h>

#include   <stdlib.h>
#include   <sys/time.h>
//Global Data

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
    //enlarge the size of data to be sent,  testing latency   50K
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
}h_data;


//for data seperate
int DataCountTime=0;

//TCP Socket
Socket_TCP_CS* g_Socket=NULL;

//数据更新标志
bool g_HasNewData=false;

/********zk********/

void *thread_socket_recv(void * param);
pthread_t pthread_recv = -1;
bool recv_run = false;

void *thread_socket_send(void * param);
pthread_t pthread_send = -1;
bool send_run = false;

ros::NodeHandle* nh; 
/********zk end********/

//Get system time 
long getCurrentTime()    
{    
   struct timeval tv;    
   gettimeofday(&tv,NULL);    
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;    
} 

//MSG:PlatformState Subscribe Callback Function
void PlatformStateCallback(const data4G::PlatformState::ConstPtr& state)  
{
	//DEBUG
	ROS_INFO("ID:%d L:%f B:%f Z:%f x:%f y:%f z:%f w:%f", state->ID,state->Longitude,state->Latitude,state->Altitude,state->x,state->y,state->z,state->w);  
	
	//Copy Data From Global Data
	g_Data.ID=state->ID;
	g_Data.TimeStamp_us=state->TimeStamp_us;
	g_Data.TimeStamp_s=state->TimeStamp_s;
	g_Data.Longitude=state->Longitude;
	g_Data.Latitude=state->Latitude;
	g_Data.Altitude=state->Altitude;
	g_Data.x=state->x;
	g_Data.y=state->y;
	g_Data.z=state->z;
	g_Data.w=state->w;

	g_HasNewData=true;
}

int main ( int argc , char ** argv ) 
{
	char ServerIP[16]="103.44.145.247";
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
	}*/
	
	if(argc > 1) {ServerPort = atoi(argv[1]);}                             //zk

	//TCP Initialization
	printf("Connecting to server at %s:%d...\n",ServerIP,ServerPort); 
	g_Socket=new Socket_TCP_CS(ServerIP,ServerPort);
	bool connected=false;
	while (!connected)
	{
		connected=g_Socket->connect_to_server();
		usleep(1000000);
	}
        printf("Connected to server!\n"); 
	
	//ROS Initialization
	ros::init ( argc , argv , "Client2" ) ;

/*****zk***********/
	nh = new ros::NodeHandle ;
          recv_run = true;
	  send_run = true;
	  int ret = pthread_create(&pthread_recv, NULL, thread_socket_recv, NULL);
	  pthread_create(&pthread_send, NULL, thread_socket_send, NULL);
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
/******zk end*****/

}

void *thread_socket_send(void *param)
{
	//Subscribe MSG:PlatformState
	ros::Subscriber PlatformState_suber=nh->subscribe<data4G::PlatformState>("PlatformState",1, &PlatformStateCallback);
 	printf("Waiting for PlatformState from ROS...\n"); 
	ros::Rate loop_rate(10);
	
	//open file with time-name
	char filename[80];
 	char str[80];
 	time_t t = std::time(0);////
 	struct tm * now = std::localtime(&t);////
 	sprintf(filename,"%d%s%d%s%d%s%d%s%d%s%d%s",now->tm_year+1900,"_",now->tm_mon+1,"_",now->tm_mday,"_",now->tm_hour,"_",now->tm_min,"_",now->tm_sec,".txt");
 	FILE* fp2=NULL;
 	fp2 = fopen(filename,"w"); 
    //for (int i=0;i<5;i++)
     // fprintf (fp2,"%d%s",i,"\n");////
    //printf ("这5个数已成功保存到filename.txt文件中\n");	
    //fclose(fp2);

    DataCountTime=int(getCurrentTime()/1000);  //second of system

       /* for(int i=0;i<12800;i++)
       {
           g_Data.latencyTest[i]=i;
       }*/
	while (send_run)  
  	{   
  		//send current time
  	    struct timeval tv22;
		gettimeofday(&tv22, NULL);
        g_Data.TimeStamp_us=tv22.tv_usec;
	    g_Data.TimeStamp_s=tv22.tv_sec;
        
		//若数据更新，则发送


		if (g_HasNewData)
		{
			g_HasNewData=false;

			//Sending Data
			int size=g_Socket->send_data((char*)&g_Data,sizeof(DATA));  

			//if Connection Lost...
			/*if (size<=0)
			{
			   printf("send Connection lost ! Reconneting...\n"); 
			   
			   //Reconnecting
			   bool connected=false;
			   while (!connected && send_run)
			   {
				connected=g_Socket->connect_to_server();
				usleep(1000000);
			   }
			   printf("Reconnet successed!\n"); 
			}*/
          
			
			//zkzk
			
			ros::Time current_time;
			current_time = ros::Time::now();
			//std_msgs::Header header;
			//header.stamp = current_time;  
			//char RosTime[10];
			//printf(RosTime,"%d%d",current_time.sec,int(current_time.nsec/1000));
            ////system_time, ROS_time, TimeStamp_us,
            time_t t2 = std::time(0);////
 	        struct tm * now2 = std::localtime(&t2);////
            now2 = std::localtime(&t2);
            
           /* if((int(getCurrentTime()/1000)-DataCountTime)>=30)
            {   
            	
            	DataCountTime=int(getCurrentTime()/1000);
            	
                fclose(fp2);
                //filename=[];
                sprintf(filename,"%d%s%d%s%d%s%d%s%d%s%d%s",now2->tm_year+1900,"_",now2->tm_mon+1,"_",now2->tm_mday,"_",now2->tm_hour,"_",now2->tm_min,"_",now2->tm_sec,".txt");
 	            fp2=NULL;

 	            fp2 = fopen(filename,"w"); 
 	            while(fp2==NULL)
                {
                 	fp2 = fopen(filename,"w");
                }

            	fprintf(fp2,"%ld\t%d%03d\t%d%03d\t%d\t%f\t%f\t%f\t%f\t%f\t%f\n",getCurrentTime(),current_time.sec,int(current_time.nsec/1000000),g_Data.TimeStamp_s,int(g_Data.TimeStamp_us/1000),g_Data.ID,g_Data.Latitude,g_Data.Longitude,g_Data.Altitude,g_Data.Heading,g_Data.Pitching,g_Data.Rolling);
                
                ROS_INFO("system time now is %d",now2->tm_sec);

            }
*/
          

			//
			//else
			{
			
		    fprintf(fp2,"%ld\t%d%03d\t%d%03d\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",getCurrentTime(),current_time.sec,int(current_time.nsec/1000000),g_Data.TimeStamp_s,int(g_Data.TimeStamp_us/1000),g_Data.ID,g_Data.Latitude,g_Data.Longitude,g_Data.Altitude,g_Data.x,g_Data.y,g_Data.z,g_Data.w);
			}
			//fprintf(fp2,"%s\t%d\t%d\t%f\t%f\t%f\t%f\t%f\t%f\n",'RosTime,g_Data.TimeStamp_s,g_Data.TimeStamp_us,g_Data.ID,g_Data.Latitude,g_Data.Longitude,g_Data.Altitude,g_Data.Heading,g_Data.Pitching,g_Data.Rolling);
			//zkzk
			
		


		}

		ros::spinOnce();  
		loop_rate.sleep();  
	}
fclose(fp2);

}


void *thread_socket_recv(void *param)
{
  ros::Rate loop_rate(10);

//open file with time-name
  char filename[80];
  char str[80];
  time_t t = std::time(0);////
  struct tm * now = std::localtime(&t);////
  sprintf(filename,"%d%s%d%s%d%s%d%s%d%s%d%s%s",now->tm_year+1900,"_",now->tm_mon+1,"_",now->tm_mday,"_",now->tm_hour,"_",now->tm_min,"_",now->tm_sec,"_recv",".txt"); 
  FILE *fp_recv = NULL;
  fp_recv = fopen(filename,"w");
  ros::Publisher key_pub = nh->advertise<keyboard::Key>("keyboard/keydown",100);
  
  keyboard::Key key;
  while(recv_run)
  {
    /*data4G::Command command;
    int size = g_Socket->recv_data((char*)(&command),sizeof(command));
    ros::Time current_time;
    current_time = ros::Time::now();
    //if connection Lost...
    if (size<=0)
    {
      printf("Connection lost ! Reconneting...\n"); 
   
      //Reconnecting
      bool connected=false;
      while (!connected && recv_run)
      {
        connected=g_Socket->connect_to_server();
        usleep(1000000);
      }
      printf("Reconnet successed!\n"); 
    } 
    if (size!=sizeof(command)) continue;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int DeltaTime = (tv.tv_sec-command.TimeStamp_s)*1000000+tv.tv_usec-command.TimeStamp_us;
    ROS_INFO("ID:%d TimeDelay:%d x:%f y:%f z:%f\n",command.ID,DeltaTime,command.x,command.y,command.z);
    fprintf(fp_recv,"%ld\t%d%03d\t%d%03d\t%d\t%f\t%f\t%f\n",getCurrentTime(),current_time.sec,int(current_time.nsec/1000000),g_Data.TimeStamp_s,int(g_Data.TimeStamp_us/1000),command.ID,command.x,command.y,command.z);*/
    
    int size = g_Socket->recv_data((char*)(&h_data),sizeof(h_data));
//    ROS_INFO("sizeof(key)=%d",sizeof(h_data));
    ROS_INFO("size  =%d",size);

    //if connection Lost...
    if (size<=0)
    {
      printf("recv Connection lost ! Reconneting...\n"); 
   
      //Reconnecting
      bool connected=false;
      while (!connected && recv_run)
      {
        connected=g_Socket->connect_to_server();
        usleep(1000000);
      }
      printf("Reconnet successed!\n"); 
    } 
    if (size!=sizeof(h_data)) continue;
    ros::Time current_time;
    current_time = ros::Time::now();
    key.header.stamp = current_time;
    key.code = h_data.code;
    key_pub.publish(key);
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int DeltaTime = (tv.tv_sec-h_data.TimeStamp_s)*1000000+tv.tv_usec-h_data.TimeStamp_us;
    ROS_INFO("ID:%d TimeDelay:%d code:%d",h_data.ID,DeltaTime,h_data.code);
    fprintf(fp_recv,"%d\t%d\t%d\n",h_data.ID,DeltaTime,h_data.code);
    loop_rate.sleep();
  }
  fclose(fp_recv);
  //printf("Recv pthread has over!\n");
}

