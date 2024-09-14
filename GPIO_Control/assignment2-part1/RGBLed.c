#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <poll.h>
#include<stdint.h>
#include<linux/input.h>

#define cycle_duration 20    //Set the cycle duration to 20 milli seconds

#define tim 0.5

int global=1;

#define MOUSEFILE "/dev/input/event3"    // Define the path for the mouse file event

#include "gpio.h"    //Include the new built library

/*
 * On board LED BLINK example
 * C program to explain how to blink LEDS
 *  By writing values to the digital pins
 */

int duty_cycle;
char buf[MAX_BUF];

int RED_LED ;
int GREEN_LED ;
int BLUE_LED ;

// Function to check the mouse click //
void *mouse_check(void* arg)
{
	int fdm;
	struct input_event ie;
	if((fdm=open(MOUSEFILE,O_RDONLY))==-1)
	{
	    perror("opening device");
	    exit(EXIT_FAILURE);
	}

	while(global)
	{
		read(fdm,&ie,sizeof(struct input_event));
		if(ie.code==272)
			global=0;
	}
	return 0;
}

// Function to setup the IO pins //
int IOSetup(int i1,int i2,int i3)
{ 
	//get the corresponding gpio pins for the given IO pins //
	RED_LED=gpio_pins(i1);
	GREEN_LED=gpio_pins(i2);
	BLUE_LED=gpio_pins(i3);

	//Export and set the direction of all the GPIO pins obtaied //
	gpio_export(GREEN_LED);
	gpio_set_dir(GREEN_LED, 1);

	gpio_export(BLUE_LED);
	gpio_set_dir(BLUE_LED, 1);

	gpio_export(RED_LED);
	gpio_set_dir(RED_LED, 1);

}

int main(int argc,char **argv)    // receive the inputs at the terminal //
{
	int Red,Green,Blue;

	// convert the inputs received into integers //
	duty_cycle=atoi(argv[1]);
	int io1=atoi(argv[2]);
	int io2=atoi(argv[3]);
	int io3=atoi(argv[4]);
	float i,j;
	int k,t;

	// Create a thread to detect the mouse click //
	pthread_t    thread_id;
	void   *thread_result;

	//Manipulate the duty cycle according to the given input //
	t=(tim)*1000/(cycle_duration);
	i=duty_cycle*cycle_duration/100;
	j=(100-duty_cycle)*cycle_duration/100;

	// Check for the validity of the Entered inputs //
	if (duty_cycle>100 || duty_cycle< 0) {printf("Invalid Duty cycle Entered !! \n");return 0;}
	if (io1 >13 || io1< 0 ||io2 >13 || io2< 0  || io3>13 || io3< 0 ) {printf("Invalid IO PIN Entered !! \n");return 0;}

	IOSetup(io1,io2,io3);    // Call the IOSetup Function to initialize the IO pins //

	int len;

	// Opening the value files of the respective GPIO pin files //
	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", RED_LED);
	Red = open(buf, O_RDONLY | O_WRONLY| O_NONBLOCK);

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value",GREEN_LED);
	Green = open(buf, O_RDONLY | O_WRONLY| O_NONBLOCK);

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value",BLUE_LED);
	Blue = open(buf, O_RDONLY | O_WRONLY| O_NONBLOCK);

	usleep(i*1000);

	pthread_create( &thread_id, NULL, mouse_check , NULL );    // Create the thread //

	// Infinite loop to blink the LED with required PWM and at their respective IO pins //

	while(global)
	{
		for(k=0;k<t;k++){
			write(Red,"1",1);
			usleep(i*1000);
			write(Red,"0",1);
			usleep(j*1000);
		}
		//usleep(tim*1000000);
		if(global!=1)
			break;

		for(k=0;k<t;k++) {
			write(Green,"1",1);
			usleep(i*1000);
			write(Green,"0",1);
			usleep(j*1000);
		}
		//usleep(tim*1000000);
		if(global!=1)
			break;

		for(k=0;k<t;k++) {
			write(Blue,"1",1);
			usleep(i*1000);
			write(Blue,"0",1);
			usleep(j*1000);
		}
		//usleep(tim*1000000);
		if(global!=1)
			break;

		for(k=0;k<t;k++) {
			write(Red,"1",1);
			write(Green,"1",1);
			usleep(i*1000);
			write(Red,"0",1);
			write(Green,"0",1);
			usleep(j*1000);
		}
		//usleep(tim*1000000);
		if(global!=1)
			break;

		for(k=0;k<t;k++) {
			write(Red,"1",1);
			write(Blue,"1",1);
			usleep(i*1000);
			write(Red,"0",1);
			write(Blue,"0",1);
			usleep(j*1000);
		}
		//usleep(tim*1000000);
		if(global!=1)
			break;

		for(k=0;k<t;k++) {
			write(Green,"1",1);
			write(Blue,"1",1);
			usleep(i*1000);
			write(Green,"0",1);
			write(Blue,"0",1);
			usleep(j*1000);
		}
		//usleep(tim*1000000);
		if(global!=1)
			break;

		for(k=0;k<t;k++) {
			write(Red,"1",1);
			write(Green,"1",1);
			write(Blue,"1",1);
			usleep(i*1000);
			write(Red,"0",1);
			write(Green,"0",1);
			write(Blue,"0",1);
			usleep(j*1000);
		}
		//usleep(tim*1000000);
		if(global!=1)
			break;
	}
	pthread_join( thread_id,  &thread_result );
	printf("Mouse Event has been detected & exiting the main function .. \n"); // Terminating the main program on the detection of mouse click //
	return 0;
}
