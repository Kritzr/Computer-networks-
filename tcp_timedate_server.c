#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<time.h>
#include<string.h>

SERVER

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#define max 30
#define PORT 2100
int main()
{
int sersoc,clisoc,conn,len,wri;
char str[max];
pid_t pid;
time_t ticks;
socklen_t clilen;
struct sockaddr_in servaddr,cliaddr;
servaddr.sin_family=AF_INET;
servaddr.sin_port=htons(PORT);
servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
if((sersoc=socket(AF_INET,SOCK_STREAM,0))<0)
{
perror("Socket Error");
exit(0);
}
if(bind(sersoc,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
{
perror("Bind Error");
exit(0);
}
listen(sersoc,10);
for(;;)
{
len=sizeof(cliaddr);
conn=(accept(sersoc,(struct sockaddr *)&clisoc,&len));
if((pid=fork())==0)
{
close(sersoc);
ticks=time(NULL);
strcpy(str,ctime(&ticks));
if(wri==(write(conn,str,sizeof(str),0))<0)
{
printf("Write Error");
exit(0);
}
close(conn);
exit(0);
}
close(conn);
}
close(sersoc);
return 0;
}


CLIENT

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<time.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdlib.h>
#define CLI_PORT 2100
#define BUFF_SIZE 30
int main(int argc,char **argv)
{
int clisoc,re;
char recbuff[BUFF_SIZE];
struct sockaddr_in cliaddr;
bzero(&cliaddr,sizeof(cliaddr));
cliaddr.sin_family=AF_INET;
cliaddr.sin_port=htons(CLI_PORT);
cliaddr.sin_addr.s_addr=inet_addr(argv[1]);
if((clisoc=socket(AF_INET,SOCK_STREAM,0))<0)
{
perror("Socket Errror");
exit(0);
}
if((connect(clisoc,(struct sockaddr *)&cliaddr,sizeof(cliaddr)))<0)
{
perror("Connect Error");
exit(0);
}
if((re=(read(clisoc,recbuff,sizeof(recbuff),0)))<0)
{
printf("Read Error");
exit(0);
}
printf("The Current Date and Time : %s\n",recbuff);
close(clisoc);
return 0;
}



