#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/time.h>
#include<math.h>
#include<semaphore.h>
#include<sched.h>
#include<inttypes.h>
#include<x86intrin.h>
#include<fcntl.h>
#include<linux/input.h>


#define MOUSEFILE "/dev/input/event6"


static int globe=1;

struct input_event ie;

void *mouse_check(void* arg)
{
	int fdm;
	struct input_event ie;
	printf("thread\n");
	if((fdm=open(MOUSEFILE,O_RDONLY))==-1)
	{
		perror("opening device");
		exit(EXIT_FAILURE);
	}
	printf("thread1\n");

	while(read(fdm,&ie,sizeof(struct input_event)))
	{
		printf("time %ld.%06ld\ttype %d\tcode %d\tvalue %d\n",
		    ie.time.tv_sec, ie.time.tv_usec, ie.type, ie.code, ie.value);
  		if(ie.code==272 || ie.code==273)
			return 0;;
	}
	return 0;
}

int main()
{
	void *thread_result;

	pthread_t thread_id;
	printf("main\n");

	pthread_create( &thread_id, NULL, mouse_check , NULL );
	pthread_join( thread_id,  &thread_result );
	pthread_exit(NULL);

	return 0;
}
