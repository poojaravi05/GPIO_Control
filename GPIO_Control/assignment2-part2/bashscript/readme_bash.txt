# User-space application for RGB LED display in bash script

==================================================================
Getting Started
==================================================================

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

==================================================================
 Running the tests
==================================================================
(Please set the mouse event in mouse.c . Currently its set event3)

1) test.sh file is the main file which needs to be executed on galelio board.

2) Setting up Galileo Board 
Connect the TTL & Ethernet cable to Galileo Board & host system & set up static ip address 
open terminal & execute : sudo screen /dev/ttyUSB0 115200
Double click enter & you can get the console of Galileo Board.
Set up static ip address using :
ifconfig enp0s20f6192.168.1.5 netmask 255.255.0.0 up
Ensure that host Ethernet connection is on same netmask & you will be able execute:  ping 192.168.1.5 with successful transmission of bytes to the board

3) Now that you have setup Connection execute on a new terminal:
	"scp *.sh *.c main root@192.168.1.5:/home"  -- this would transfer all bash script files for board

5)In console window navigate to home Directory & Execute :
	
./test.sh -- this is the main script file which will initiate other script files & c programs

the user will be prompted to enter PWM , RED GREEN & BLUE arduino shield pins

you should now be able to see the R,G,BRG,GB,RB,RGB sequence

Note : Range of Duty Cycle : 0-100
	  Range of IO pin numbers :0-13 (Ensure that you enter the same Pin numbers as input argumets that you have connected the digital Connectors to)

6) Click on mouse would terminate the program

