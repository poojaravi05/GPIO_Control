#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <poll.h>
#include<stdint.h>
#include<linux/input.h>
#include <errno.h>

#define path1 "/dev/RGBLed"
#define PRIN_MAGIC 'q'
#define SET_PWM _IOW('q',1,int)
#define SET_IOPIN1 _IOW('q', 2 , int)
#define SET_IOPIN2 _IOW('q', 3 , int)
#define SET_IOPIN3 _IOW('q', 4 , int)
#define cycle_duration 20    //Set the cycle duration to 20 milli seconds
#define tim 0.5
#define MOUSEFILE "/dev/input/event3"

//Variable used to monitor if the mouse click has been identified.
//Set it to 0 upon mouse click detection.
int global = 1;

// A seperate thread to monitor the mouse click continuously.
void *mouse_check(void* arg)
{
	int fdm;
	struct input_event ie;

	if((fdm = open(MOUSEFILE,O_RDONLY)) == -1)
	{
	    perror("opening device");
	    exit(EXIT_FAILURE);
	}

	while(global)
	{
		read(fdm, &ie, sizeof(struct input_event));
		
		if((ie.code==272) || (ie.code==273))
			global=0;
	}
	return 0;
}

int main(int argc,char **argv)
{

	int fp,k,i,j,z;
	int buff;
    int pw,i1,i2,i3;
	unsigned long pwm,io1,io2,io3;
 	unsigned long *temp;
	pthread_t thread_id;
	void *thread_result;

	//Get the PWM and the I/O pin inputs from the user.
	pw=atoi(argv[1]);
	i1=atoi(argv[2]);
	i2=atoi(argv[3]);
	i3=atoi(argv[4]);

	pwm=(unsigned long)pw;
	io1=(unsigned long)i1;
	io2=(unsigned long)i2;
	io3=(unsigned long)i3;

	//Validate user input
	if(pwm > 100)
	{
		printf("INVALID duty_cycle!!\n");
		return 0;
	}

	if(io1>13||io2>13||io3>13)
	{
		printf("INVALID IO Pin!!\n");
		return 0;
	}

	i = 0;
	fp = open(path1, O_RDWR);

	k = ioctl(fp, SET_PWM ,pwm);
	printf("THE PWM TRANSMITTED IS %lu\n",pwm);

	k = ioctl(fp, SET_IOPIN1 ,io1);
	printf("THE IOPIN1 TRANSMITTED IS %lu\n",io1);

	k = ioctl(fp, SET_IOPIN2 ,io2);
	printf("THE IOPIN1 TRANSMITTED IS %lu\n",io2);

	k = ioctl(fp, SET_IOPIN3 ,io3);
	printf("THE IOPIN1 TRANSMITTED IS %lu\n",io3);

	//Create thread to detect mouse button click event.
	pthread_create(&thread_id, NULL, mouse_check , NULL);

	while(global) 
	{
		buff = 1;
		k = write(fp,&buff,sizeof(buff));
		printf("The vale of k[%d] is : %d \n",i,k);
		i++;
		usleep(500000);
		if(global!=1)
			break;

		buff=2;
		k=write(fp,&buff,sizeof(buff));
		printf("The vale of k[%d] is : %d \n",i,k);
		i++;
		usleep(500000);
		if(global!=1)
			break;
		
		buff=3;
		k=write(fp,&buff,sizeof(buff));
		printf("The vale of k[%d] is : %d \n",i,k);
		i++;
		usleep(500000);
		if(global!=1)
			break;

		buff=4;
		k=write(fp,&buff,sizeof(buff));
		printf("The vale of k[%d] is : %d \n",i,k);
		i++;
		usleep(500000);
		if(global!=1)
			break;

		buff=5;
		k=write(fp,&buff,sizeof(buff));
		printf("The vale of k[%d] is : %d \n",i,k);
		i++;
		usleep(500000);
		if(global!=1)
			break;

		buff=6;
		k=write(fp,&buff,sizeof(buff));
		printf("The vale of k[%d] is : %d \n",i,k);
		i++;
		usleep(500000);
		if(global!=1)
			break;

		buff=7;
		k=write(fp,&buff,sizeof(buff));
		printf("The vale of k[%d] is : %d \n",i,k);
		i++;
		usleep(500000);
		if(global!=1)
			break;

	}

	// Terminating the main program on the detection of mouse click 
	pthread_join( thread_id,  &thread_result );
	printf("Mouse Event has been detected & exiting the main function .. \n"); 
	close(fp);
	return 0;
}
