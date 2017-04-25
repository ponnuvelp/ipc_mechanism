#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
/* Include Files */
#include <stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
/* Definitions */
#define CLIENT_USAGE "cse473-p1 <input> <output>"
#define TRUE 1
#define MTYPE 1
#define MEND 2
#define MSG_SIZE 30

struct msgbuf_t {
	long    type;                       /* message type */
	char    text[MSG_SIZE];             /* some text */
}svar;

#define MSGBUF_SIZE (sizeof(struct msgbuf_t) - sizeof(long))

int create(char *,int);
int receive(char *,int);
/* 
   send contents of a file to another process via IPC 
   or should it be multiple passes...
 */


/**********************************************************************

Function    : main
Description : this is the main function for project #1 pipe IPC
Inputs      : argc - number of command line parameters
argv - the text of the arguments
Outputs     : 0 if successful, -1 if failure

 ***********************************************************************/

/* Functions */
int main( int argc, char **argv ) 
{
	int pid;

	int err;
	int new=0;

	/* Check for arguments */
	if ( argc < 3 ) 
	{
		/* Complain, explain, and exit */
		fprintf( stderr, "missing or bad command line arguments\n" );
		fprintf( stderr, CLIENT_USAGE );
		exit( -1 );
	}
	new=msgget(1,IPC_CREAT|0644);
	if(new<0){
	perror("msggget");
	return 0;
	}


	/* create receiver process */
	if (( pid = fork() != 0 )) {
	/* in parent */
	create(argv[1],new);

	}
	else {
	/* in child */
	receive(argv[2],new);

	}

	exit( 0 );
}
int  create(char *name,int new)
{
	char s[30];
	int fd=0;
	int err=0;
	fd= open(name,O_RDONLY);
	if(fd<0){
	perror("open in creat");
	return 0;}
	read(fd,s,sizeof(s));
	svar.type=1;
	strcpy(svar.text,s);
	err=msgsnd(new,&svar,strlen(svar.text),0);
	if(err<0){
	perror("msgsnd");	
	return 0;
	}
	
}

int  receive(char *name,int new)
{
	char s[30];	
	int fd=0;
	int err=0;
	fd=open(name,O_WRONLY|O_CREAT|0644);
	if(fd<0){
	perror("open_inreceive");
	return 0;}

	err=msgrcv(new,&svar,sizeof(svar.text),1,0);
	if(err<0){
	perror("msgrcv");	
	return 0;}
	
	err=write(fd,svar.text,strlen(svar.text)+1);
	if(err<0){
	perror("write");
	return 0;}

	if(err==0)
	{
	printf("writting is completed\n");
	exit(0);}

}
