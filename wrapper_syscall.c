#include "common.h"
#include "wrapper_syscall.h"
int open_shm(key_t key,size_t size)
{
   return(shmget(key,size,IPC_CREAT|0666));
}
char *attch_shm(int shm_id)
{

char *p= (char *)shmat(shm_id,NULL,0);
 return p;
}

