
/**********************************************************************

File          : cse473-p1.c

Description   : This is the main file for pipe-based IPC
(see .h for applications)

Last Modified : Jan 1 09:54:33 EST 2009
By            : Trent Jaeger

 ***********************************************************************/
/**********************************************************************
  Copyright (c) 2008 The Pennsylvania State University
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of The Pennsylvania State University nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***********************************************************************/

/* Include Files */
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

/* Definitions */
#define CLIENT_USAGE "cse473-p1 <input> <output>\n"
#define TRUE 1
#define STDIN 0
#define STDOUT 1

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
	int err=0;
	struct stat v;

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
	if (( pid = fork() != 0 )) {
		/* child */
			sleep(1);
		/* setup the IPC channel */
		setup_ipc_child( read_index, write_index,  &new_index );
		if ( err ) {
			fprintf( stderr, "child: error in ipc setup\n" );
			exit(-1);
		}
		/* dump the file contents */
		rcv_file( argv[2], new_index );
		fprintf( stderr, "child: finished\n" );




	}
	else {
		/* parent */

		/* setup the IPC channel */
		err = setup_ipc_parent( read_index, write_index, &new_index );
		if ( err ) {
			printf("%d\n",err);
			fprintf( stderr, "parent: error in ipc setup\n" );
			exit(-1);
		}
		/* send the file contents */
		send_file( argv[1], new_index );
		fprintf( stderr, "parent: finished\n" );


		exit( 0 );
	}

}
/**********************************************************************

Function    : create_ipc
Description : obtain the descriptor to the new IPC channel 
Inputs      : read and write index pointers (ignore the value)
Outputs     : >0 if successful, <0 otherwise

 ***********************************************************************/

int create_ipc( int *read, int *write )
{
	int result;
	int fd[2];

	result=pipe(fd);
	if(result<0){
		fprintf(stderr,"pipe\n");
		return result;
	}


	*read=fd[0];
	*write=fd[1];
	return 0;
}


/**********************************************************************

Function    : setup_ipc_child
Description : Using the IPC descriptor 'index', create a 'new' descriptor for reading data
Inputs      : read -- read descriptor for pipe
write -- write descriptor for pipe 
new -- pointer to holder of new descriptor on output
Outputs     : 0 if successful, <0 otherwise

 ***********************************************************************/

int setup_ipc_child( int read, int write, int *new ) 
{
	int err = 0; 
	close(0);
	close(write);
	*new=dup(read);
	if(new<0){
		fprintf(stderr,"setup_ipc_child_dup\n");
		exit(-1);}

	/* YOUR CODE HERE */

	return err;
}


/**********************************************************************

Function    : setup_ipc_parent
Description : Using the IPC descriptor 'index', create a 'new' descriptor for writing data
Inputs      : read -- read descriptor for pipe
write -- write descriptor for pipe 
new -- pointer to holder of new descriptor on output
Outputs     : 0 if successful, <0 otherwise

 ***********************************************************************/

int setup_ipc_parent( int read, int write, int *new ) 
{
	int err = 0;
	close(1);
	close(read);
	*new=dup(write);
	if(0>new){
		fprintf(stderr,"setup_ipc_parent_dup\n");
		exit(-1);
	}


	return err;
}


/**********************************************************************

Function    : send_file
Description : Send file 'name' data over IPC channel 'index'
Inputs      : name -- file path
index -- IPC descriptor
Outputs     : 0 if successful, -1 otherwise

 ***********************************************************************/

int send_file( char *name, int index )
{

	struct stat v;
	int fd=0;
	int err=0;
#if 0
	if(write(index,name,strlen(name)+1)<0){
		fprintf(stderr,"send_file_write\n");
		exit(-1);}
#endif
	if(stat(name,&v)<0){
		perror("stat:");
		exit(-1);
	}
	fd=open(name,O_RDONLY);
	if(fd<0){
		fprintf(stderr,"open:err in toopen the the file\n");
	}
	int vv=v.st_size,i;
	char s[vv+1];
	err=read(fd,s,sizeof(s));	
	if(err<0){
		fprintf(stderr,"read:err in the to read the file\n");
		exit(-1);
	}
	for(i=0;i<vv;i++)
		err=write(index,&s[i],1);
	if(err<0){
		fprintf(stderr,"write:err in to write in to the pipe\n");
		exit(-1);}
	return 0;
}


/**********************************************************************

Function    : rcv_file
Description : Receive file 'name' data over IPC channel 'index'
Inputs      : name -- file path
index -- IPC descriptor
Outputs     : 0 if successful, -1 otherwise

 ***********************************************************************/


int rcv_file( char *name, int index )
{
	char ch;
	int fd=0,err=0;
	fd=open(name,O_WRONLY);
	if(fd<0){
		fprintf(stderr,"rec:open failed");
		return 0;}
	while(1){
		err=read(index,&ch,1);
		if(err==0){
			fprintf(stderr,"receiving:finised\n");
			return 0;}
	
		if((err=write(fd,&ch,1))==0)
		fprintf(stderr,"writting is completed\n");
		if(err<0){
			fprintf(stderr,"write failed\n");
			return 0;}
	}
	return 0;
}

