#include <ros/ros.h>
#include <data4G/PlatformState.h>

#include "net4all.h"

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
    int latencyTest[128];
} g_Data;
//TCPSocket
Socket_TCP_CS* g_socket=NULL;

long getCurrentTime()    
{    
   struct timeval tv;    
   gettimeofday(&tv,NULL);    
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;    
} 

int main ( int argc , char ** argv ) 
{
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
	}
	
	//TCP initialization
	g_socket=new Socket_TCP_CS(ServerPort);
	printf("Server start listening at port : %d...\n",ServerPort); 
	g_socket->accept_connection();
	printf("Client connected!\n"); 
	
	//ROS initialization
	ros::init ( argc , argv , "PlatformServerRemote" ) ;
	ros::NodeHandle nh ;
	ros::Publisher PlatformState_puber  =nh.advertise<data4G::PlatformState>("PlatformStateRemote",1000);

	ros::Rate loop_rate(20); 



	char filename[80];
 	char str[80];
 	time_t t = std::time(0);////
 	struct tm * now = std::localtime(&t);////
 	sprintf(filename,"%d%s%d%s%d%s%d%s%d%s%s",now->tm_year+1900,"-",now->tm_mon+1,"-",now->tm_mday,"-",now->tm_hour,"-",now->tm_min,"remote",".txt");
 	FILE* fp2=NULL;
 	fp2 = fopen(filename,"w");  
	while(ros::ok())  
	{     
		int size=g_socket->recv_data((char*)(&g_Data),sizeof(DATA));
		//if connection Lost...
		if(size<=0)  
		{  
			printf("Client connection lost , waiting for reconnect...\n");  
			usleep(100000);
			int ret=g_socket->accept_connection();
			printf("ret=%d\n",ret);
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


		ros::spinOnce();  
		loop_rate.sleep();  
    	}  
    	fclose(fp2);
	g_socket->close_connection();
}
