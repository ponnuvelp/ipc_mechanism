#include "main_shared.h"

key_t key=1234;
int shm_id;

int main(int argc,char **argv)
{
	
  char *shm_addr=NULL,*temp=0;
  int i;

  //to creat shared memory
  shm_id=shm_creat(key,SHM_SIZE);
  

  printf("shmid=%d\n",shm_id);
  
  if(shm_id==-1)
    perror("shmget");
#if 0
else

    	DEBUG_PRINTF("SHM CREATYED SUCCES\n");
#endif
  if(fork()==0)
  {
   sleep(1);
   // to attach shared memory segmet to process address space
   shm_addr=(char *)attch_shm(shm_id);
/*#if 0
   DEBUG_PRINTF("SHM attached SUCCES\n");
#endif*/
   temp=shm_addr;

    int cnt=0;
    
    int fd;
    fd=creat(argv[2],0666);
    perror("creat");
   
    write(fd,temp,SHM_SIZE); 
    cnt=cnt+1;
  
}
 else
 {
   shm_addr=(char*)attch_shm(shm_id);

    int fd;
    char str;
    int pid=0;
    fd=open(argv[1],O_RDONLY);	
    perror("open");
    temp=shm_addr;
    
    while(read(fd,&str,1)!=0)
    {
        *temp++=str;
        putchar(str);   
        
        
    } 
   
   pid=wait(&i);
}
/*#if 0
          DEBUG_PRINTF("END\n");
#endif*/
          return 0;
}

int shm_creat(key_t key,size_t size)
{


      int shm_id=open_shm(key,size);
 

      return shm_id;
}
