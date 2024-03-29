/** @file  smphr.c 
 *  @brief implementation definition 
 *  @author Umar Ba  , LoopFocus jUmarB@protonmail.com  
 */

#include <semaphore.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <errno.h>
#include <err.h>
#include <assert.h> 
#include <unistd.h>
#include <pthread.h>

#ifdef NAMEDSMPR
#include <fcntl.h> 
#include <sys/stat.h> 
#endif  

#include "smphr.h"  
 

static sem_t  sem_tmp  ; 

struct sem_agent_t  * configure  ( struct  sem_agent_t * agent , uint * count_limit )  
{ 
  errno = 0 ;
  agent->index =0  ; 
  uint lcount_until =  LCOUNT_UNTIL ; 
  if ( count_limit != _nullable ) 
  {
    lcount_until = *count_limit ; 
  }
  assert (lcount_until > 0  && "should be greater than zero  !") ; 
  agent->lm_counter = lcount_until ; 

#ifndef  NAMEDSMPR 
  agent->smphr_init =  sem_init  ;
  agent->smphr_init(&sem_tmp ,  0 ,  0 /** hardcoded  */) ; 
  agent->smphr_h =  &sem_tmp ; 
  if (errno !=0) errx(errno ,  "cannot configure agent") ;
  
  agent->smphr_terminate = sem_destroy; 
#else  
  agent->smphr_init = sem_open ; 

  agent->smphr_terminate  = sem_close ;
#endif
  
  return agent  ; 
}


struct __semphr_t *configure_sa (  struct __semphr_t  * restrict agent ,  uint * count_limit) 
{
  agent = malloc (sizeof(struct  sem_agent_t )) ;  
 
  smphr_t  * b_agent = (smphr_t *)agent;   //((smphr_t*)agent)->index  = 3 ; 

  configure ( (struct sem_agent_t *)b_agent , count_limit ) ; 

  return agent ;  

}

void * count ( void * args  )  
{
  smphr_t * count_h =  (struct sem_agent_t *)  args ;  
   
  while ( 1 ) 
  {
     count_h->index++ ;  
     printf("%i\n" , count_h->index) ; 
     if  (count_h->index ==  count_h->lm_counter)  
     {
       sem_post(count_h->smphr_h) ; 
     } 
     sleep(1) ; 
  }

  pthread_exit (_nullable) ; 
}

void *reset ( void * args  ) 
{
  smphr_t  * reset_h = (struct  sem_agent_t *) args ; 

  while ( 1 ) 
  {
     sem_wait(reset_h->smphr_h);  
     reset_h->index = 0 ;  
  }

  pthread_exit(_nullable);
}
