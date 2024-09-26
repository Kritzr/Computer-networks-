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

