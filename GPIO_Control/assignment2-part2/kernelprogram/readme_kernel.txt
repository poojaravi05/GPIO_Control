# KERNEL -space application for RGB LED display

==================================================================
Getting Started
==================================================================

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

==================================================================
 Running the tests
==================================================================
(Please set the mouse event in RGBled.c . Currently its set event3)

1) "make" -- The program will begin preparing the executable files which is cross compiled.

2) kmodule.ko & sample are the module & executable file that needs to be transferred to galelio board.

3) Setting up Galileo Board 
Connect the TTL & Ethernet cable to Galileo Board & host system & set up static ip address 
open terminal & execute : sudo screen /dev/ttyUSB0 115200
Double click enter & you can get the console of Galileo Board.
Set up static ip address using :
ifconfig enp0s20f6192.168.1.5 netmask 255.255.0.0 up
Ensure that host Ethernet connection is on same netmask & you will be able execute:  ping 192.168.1.5 with successful transmission of bytes to the board

4) Now that you have setup Connection execute on a new terminal:
	"scp kmodule.ko & sample root@192.168.1.5:/home"

5)In console window navigate to home Directory & Execute :

	insmod kmodule.ko  -- this command will mount the module in kernel sapce
	./sample 50 2 3 4 (50-pwm(duty) ,2,3,4 arduino sheild pins the led is connected to) . These are the arguments to sample progam

This implies 50% duty cycle & 2,3,4 are the arduino sheild connectors that various LED Pins are connected to i.e(IO0 , IO1 , IO2)

Note : Range of Duty Cycle : 0-100
	  Range of IO pin numbers :0-13 (Ensure that you enter the same Pin numbers as input argumets that you have connected the digital Connectors to)

6) Click on mouse would terminate the program

7) "make clean" on host window  which would remove all the executable files created


==================================================================
Annexure
==================================================================

 
a) gedit kmodule.c -- instruction to access the file contents of module program

b) gedit sample.c -- instruction to access the file contents of user space program

==================================================================
Authors
==================================================================


NITHEESH MUTHURAJ 
ASU ID:1213383988
nitheesh@asu.edu
 
REVANTH RAJSHEKAR 
ASU ID:1213380816
rmrajash@asu.edu

