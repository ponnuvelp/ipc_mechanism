
/**********************************************************************

   File          : cse473-p1-mypipe.c

   Description   : This is the library file for Linux Shared Memory IPC
                   (see .h for applications)
                   See http://www.cs.cf.ac.uk/Dave/C/node27.html for info

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
#include <sys/shm.h>
#include <sched.h>

    /* 
       send contents of a file to another process via IPC 
       or should it be multiple passes...
    */

#include "cse473-p1-mypipe.h"

#define SHM_KEY 1236
#define SHM_SZ  32

struct mypipe {
  int shmid;
  int read;
  int write;
  int size;
  char shm[SHM_SZ];
};

struct mypipe *only_pipe = NULL;

/**********************************************************************

    Function    : mypipe_init
    Description : create a new shared memory pipe -- return an index for communication
    Inputs      : none
    Outputs     : >0 if successful, -1 if failure

***********************************************************************/

/* Functions */
int mypipe_init( void )
{
  int shmid;

  /* YOUR CODE HERE */

  return shmid;
}


/**********************************************************************

    Function    : mypipe_attach
    Description : create a new shared memory region and setup
    Inputs      : none
    Outputs     : >0 if successful, -1 if failure

***********************************************************************/


int mypipe_attach( int index, int *new )
{

  /* YOUR CODE HERE */

 return 0;
}



/**********************************************************************

    Function    : mypipe_read
    Description : read message from pipe into buffer of max bytes
    Inputs      : index to mypipe
                  buf to write characters from pipe
                  max is number of bytes maximum
    Outputs     : >0 if successful (number of bytes read), <0 otherwise

***********************************************************************/

int mypipe_read( int index, char *buf, int max )
{
  int read, write;
  int bytes;

  /* need to store vars -- in case of concurrent update */
  read = only_pipe->read;
  write = only_pipe->write; 

  /* if write == read, give the writer a chance */
  while ( write == read ) { 
    sched_yield(), read = only_pipe->read, write = only_pipe->write; 
  }

  /* YOUR CODE HERE */

  return bytes;
}


/**********************************************************************

    Function    : mypipe_write
    Description : write message from buffer into shm of len bytes
    Inputs      : index to mypipe
                  buf to read characters for pipe
                  len is number of bytes to read
    Outputs     : >0 if successful (number of bytes read), <0 otherwise

***********************************************************************/

int mypipe_write( int index, char *buf, int len ) 
{
  int read, write;

#if DEBUG
  printf("only_pipe (write): %d; %d; %d; %d; %s\n", 
	only_pipe->shmid,
	only_pipe->read,
	only_pipe->write,
	only_pipe->size,
	only_pipe->shm);
#endif

 /* need to store vars -- in case of concurrent update */
  read = only_pipe->read;
  write = only_pipe->write; 

  /* cannot let the writes overlap the reads */
  while ((( write >= read ) && (( write + len ) >= ( only_pipe->size + read ))) ||
	 (( write < read ) && (( write + len ) >= read ))) { 
    sched_yield(), read = only_pipe->read, write = only_pipe->write; 
  }

  /* YOUR CODE HERE */

  return len;
}

