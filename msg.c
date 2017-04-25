#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include <sys/types.h>
       #include <sys/stat.h>
int main(int agrc,char *argv[])
{
	int err=0,fd=0;
	char s[30];
	fd=open(argv[1],O_RDWR);
	if(err<0)
	{
		perror("open:");
		return 0;
	}
	
	while(1){
	err=read(fd,s,sizeof(s));
	if(err==0)
	{
		perror("read");

		return 0;
	}
	err=write(1,s,strlen(s)+1);
	if(err<0){
		perror("write");
		return 0;
	}
	}
}
