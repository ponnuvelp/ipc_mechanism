#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>

/* Project Include Files */
#include "cse473-p1.h"
#include "cse473-p1-mypipe.h"

/* Definitions */
#define CLIENT_USAGE "cse473-p1 <input> <output>"
#define TRUE 1
#define MSG_SIZE 30
#define SEND 1

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
    int read_index = 0, write_index = 0, new_index = 0;
    int err;

    /* Check for arguments */
    if ( argc < 3 ) 
    {
        /* Complain, explain, and exit */
        fprintf( stderr, "missing or bad command line arguments\n" );
        fprintf( stderr, CLIENT_USAGE );
        exit( -1 );
    }

    /* get the index for the IPC object */
    err = create_ipc( &read_index, &write_index );
    if ( err ) {
      fprintf( stderr, "create ipc error\n" );
      exit(-1);
    }

    /* create receiver process */
#if SEND
    if (( pid = fork() != 0 )) {
      /* child */
#if 0
      /* setup the IPC channel */
      setup_ipc_child( read_index, write_index,  &new_index );
      if ( err ) {
        fprintf( stderr, "child: error in ipc setup\n" );
	exit(-1);
      }
#endif
	new_index=read_index;
      /* dump the file contents */
      rcv_file( argv[2], new_index );
      fprintf( stderr, "child: finished\n" );
    }
    else {
#endif
      /* parent */
#if 0
      /* setup the IPC channel */
      err = setup_ipc_parent( read_index, write_index, &new_index );
      if ( err ) {
        fprintf( stderr, "parent: error in ipc setup\n" );
	exit(-1);
      }
#endif
	new_index=write_index; 	
      /* send the file contents */
      send_file( argv[1], new_index );
      fprintf( stderr, "parent: finished\n" );
#if SEND
    }
#endif

    exit( 0 );
}
int create_ipc(int *read,int *write)
{
	int shmid=0;
	shmid=shmget(5432,MSG_SIZE, IPC_CREAT|0644);
	perror("shmget");
	*read=shmid;
	*write=shmid;
	return 0;
}
int send_file(char *name,int new)
{
	int err=0;
	char s[MSG_SIZE];
	char *local_ptr;
	int fd =open(name,O_RDONLY|0644);
	local_ptr=shmat(new,NULL,0);
	
	if (fd<0){
	perror("open in send file");
	return 0;}
	while(1){
	err=read(fd,s,MSG_SIZE);
	if(err<0){
	perror("read in send file");
	return 0;
	}
	if(err==0)
	exit(0);
	local_ptr=s;
	}
}
int rcv_file(char *name,int new)
{

	int err=0;
	char s[MSG_SIZE];
	char *local_ptr;
	local_ptr=shmat(new,NULL,0);
	int fd =open(name,O_WRONLY|0644);
	
	if (fd<0){
	perror("open in send file");
	return 0;}
	//p=shmat(new,NULL,0);
	while(1){
	strcpy(s,local_ptr);

	err=write(fd,s,MSG_SIZE);
	if(err<0){
	perror("read in send file");
	return 0;}
	if(err==0){
	exit(0);}
	}
}
