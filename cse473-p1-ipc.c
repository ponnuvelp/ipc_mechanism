/**********************************************************************

File          : cse473-p1-ipc.c

Description   : This is the main file for Linux IPC
(see .h for applications)
See http://www.cs.cf.ac.uk/Dave/C/node25.html for info

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
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sched.h>

/* Project Include Files */
#include "cse473-p1.h"

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
			fprintf( stderr, "parent: error in ipc setup\n" );
			exit(-1);
		}

		/* send the file contents */
		send_file( argv[1], new_index );
		fprintf( stderr, "parent: finished\n" );
	}

	exit( 0 );
}


/**********************************************************************

Function    : create_ipc
Description : obtain the descriptor to the new IPC channel 
Inputs      : read and write index pointers (ignore the value)
Outputs     : 0 if successful, <0 otherwise

 ***********************************************************************/

int create_ipc( int *read, int *write )
{
	int r=0;

	r=msgget(1,IPC_CREAT|0644);	
	if(r<0){perror("msgget");
		return 0;}
	perror("msgget");

	*read=r;
	*write=r;

	return 0;
}


/******************************************************************

Function    : setup_ipc_child
Description : Using the IPC descriptor 'index', create a 'new' descriptor for reading data
Inputs      : read -- read id for IPC
write -- write id for IPC (they are same)
new -- pointer to holder of new descriptor on output
Outputs     : 0 if successful, <0 otherwise

 ***********************************************************************/

int setup_ipc_child( int read, int write, int *new ) 
{

	*new=read;  

	return 0;
}


/**********************************************************************

Function    : setup_ipc_parent
Description : Using the IPC descriptor 'index', create a 'new' descriptor for writing data
Inputs      : read -- read id for IPC
write -- write id for IPC (they are same)
new -- pointer to holder of new descriptor on output
Outputs     : 0 if successful, <0 otherwise

 ***********************************************************************/

int setup_ipc_parent( int read, int write, int *new ) 
{

	*new=write;

	return 0;
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
	int err=0,fd=0;
	char s[30];
	fd=open(name,O_RDONLY);
	if(fd<0){
		perror("open:sendfile");}
		perror("open:sendfile");

while(1){
	err=read(fd,s,30);

puts(s);
	if(err<0){
		perror("read");
		return 0;}
	if(err==0){
		printf("reciving is completed\n");
		exit(0);
	}
	svar.type=1;
	strcpy(svar.text,s);
	
	err=msgsnd(index,&svar,sizeof(svar.text),0);
	if(err<0){
		perror("msgsnd");
		return 0;
	}
}

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
	int fd=0;
	int err=0;
	char s[30];
	fd=open(name,O_WRONLY|0644);
	if(fd<0){
		perror("open");
		return 0;}
	while(1){
	err=msgrcv(index,&svar,sizeof(svar.text),1,IPC_NOWAIT);
	if(err<0){
		perror("msgrcv");exit(0);}
		perror("msgrcv");
	err=write(fd,svar.text,strlen(svar.text));
	if(err<0){
		perror("rcv:write");
		return 0;}
	if(err==0){
		printf("writting is completed");
		exit(0);
	}	
}
}
