/** @file  main.c 
 */ 


#include <stdlib.h> 
#include <stdio.h> 
#include <signal.h> 
#include <unistd.h>
#include <err.h>
#include <sys/wait.h> 
#include <sys/types.h> 
#include <string.h> 


#include "include/ipc.h" 

int
main(int _ac  , char ** _av) { 

  /** 
   */ 
  if (_ac == 1 )  
  {
    warnx("require at least  1 argument") ; 
    ipc_exit(1);
  } 
  char *mesg_echoing=_av[_ac -1] ; 
  
  ipc_t *  ipc = init(_void_0h) ; 

  if (ipc == _void_0h )  
    raise(SIGABRT) ;   

#ifndef  IPC_FIFO 
  pid_t   child_process = fork(); 
  if (child_process <= ~0)
    errx(child_process ,  "ipc local fork : failure") ; 
  
  if (child_process == IPC_CHILDP)  
  {  
    char cbuff[100] ={0} ; 
    ipc_io_operations(&((struct _prochannel_t*)ipc)->iofd, cbuff,  100  , IPC_RSTREAM);  
    printf("-> %s" , cbuff) ; 
     
  }else 
  {
    ipc_io_operations(&((struct _prochannel_t*)ipc)->iofd, mesg_echoing ,  strlen(mesg_echoing)  , IPC_WSTREAM);  
  }
  
  wait(_nullable) ;
#else  
  /** if you define IPC_FIFO*/ 
  

#endif 

  

  return EXIT_SUCCESS; 
}
