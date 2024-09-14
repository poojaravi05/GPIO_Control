#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/jiffies.h>
#include <linux/pci.h>
#include <linux/param.h>
#include <linux/list.h>
#include <linux/semaphore.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/gpio.h>
#include <linux/timer.h>

#define cycle_duration 20    //Set the cycle duration to 20 milli seconds
#define tim 0.5

#define PRIN_MAGIC 'q'
#define SET_PWM _IOW('q',1,int)
#define SET_IOPIN1 _IOW('q', 2 , int)
#define SET_IOPIN2 _IOW('q', 3 , int)
#define SET_IOPIN3 _IOW('q', 4 , int)
#define DEVICE_NAME "RGBLed"
#define DEVICE_CLASS "virtual_char"
#define NUM_DEVICE 1

struct timer_list my_timer;
struct class *my_class;
int addflag=0;
unsigned long gpio[4];
unsigned int duty_cycle;
unsigned int RED_LED ;
unsigned int GREEN_LED ;
unsigned int BLUE_LED ;
unsigned int on, off, t; // used for handling duty cycle & Pwm .they get initialized after kmodule receives pwm from user space
int flag_tt =0;
int red_flag=0, green_flag=0, blue_flag=0;
unsigned int MAJOR_NUMBER;
unsigned int MINOR_NUMBER;
int flag=0;
int ptr=0;
int a=0, b=0, c=0;
int val=0;
int bal=0;
unsigned long args[4], tmp[12];

static dev_t dev; /* Allotted device number */

spinlock_t my_lock ; /* the lock for the counter */

/*My device structure*/
struct my_dev
{
	struct cdev cdev;  /* The cdev structure */
  	char name[20];
}*my_devp;

//Function declarations
unsigned long* gpio_pins(unsigned long io);

/*
 * TIMER FUNCTION
 * */
static void timer_function(unsigned long data)
{
	//ON PERIOD TIME
    if(flag_tt==0)
    {
		//Protect the critical section
		spin_lock (&my_lock);

		//RED LED ON/OFF
        if(a==1)
        {
			red_flag=1;
			gpio_set_value_cansleep(RED_LED,1);
		}
        else
		{
			red_flag=0;
			gpio_set_value_cansleep(RED_LED,0);
		}

		//GREEN LED ON/OFF
		if(b==1)
		{
			green_flag=1;
			gpio_set_value_cansleep(GREEN_LED,1);
		}
		else
		{
			green_flag=0;
			gpio_set_value_cansleep(GREEN_LED,0);
		}

		//BLUE LED ON/OFF
	    if(c==1)
        {
			blue_flag=1;
			gpio_set_value_cansleep(BLUE_LED,1);
		}
        else
		{
			blue_flag=0;
			gpio_set_value_cansleep(BLUE_LED,0);
		}

		spin_unlock (&my_lock);
		
		mod_timer(&my_timer, msecs_to_jiffies(on));

		flag_tt=1;
	}
	//OFF PERIOD TIME
	else
	{
		spin_lock (&my_lock);
		gpio_set_value_cansleep(RED_LED,0);
		gpio_set_value_cansleep(GREEN_LED,0);
    	gpio_set_value_cansleep(BLUE_LED,0);
		spin_unlock (&my_lock);

		mod_timer(&my_timer, msecs_to_jiffies(off));

		flag_tt=0;
	}
}


/* ************ Open RGBLed driver**************** */
int RGBLed_open(struct inode *inode, struct file *file)
{
  	struct my_dev *ptr1;
  	printk("ENTERED THE OPEN FUNCTION....!!!\n");
  	printk(KERN_INFO "Driver: open()\n");

	/* Get the per-device structure that contains this cdev */
 	 ptr1 = container_of(inode->i_cdev, struct my_dev, cdev);

  	/* Easy access to cmos_devp from rest of the entry points */
  	file->private_data = ptr1;
  	printk("%s: Device opening\n", ptr1->name);
  	return 0;
}

/*****************Release RGBLed driver******************/
int RGBLed_release(struct inode *inode, struct file *file)
{
	int i;
 	struct my_dev *ptr2;
   	printk("RELEASING THE  DRIVER....!!!\n");
	ptr2 = file->private_data;
  	val=0;
	bal=0;
        gpio_set_value_cansleep(RED_LED,0);
	gpio_set_value_cansleep(GREEN_LED,0);
      	gpio_set_value_cansleep(BLUE_LED,0);
  	del_timer(&my_timer); // delete the timer
	for(i=0;i<12;i++)
		{	if(tmp[i]<100)
			{
				gpio_set_value_cansleep(tmp[i],0);
				gpio_free(tmp[i]);

			}
		}

	red_flag=0;
	addflag=0;
	blue_flag=0;
	flag_tt=0;
	green_flag=0;
	printk(KERN_INFO "Driver: close()\n");
  	printk("%s: Device closing\n", ptr2->name);
    	return 0;
}


/*****************IOCTL RGBLed driver******************/
/******
***Function to perform the configuration of the IO pins
***and set the PWM .
**/
static long RGBLed_ioctl(struct file *f, unsigned int cmd, unsigned long argu)
{
	int i,z,x,y;
	unsigned long *temp;
	printk("ENTERED THE ioctl FUNCTION....!!!\n");
	printk("The Arg value is %lu\n",argu);
	switch (cmd)
    	{
        	case SET_PWM:
	    		printk("Entered SET_PWM\n");
            		duty_cycle=(unsigned int)argu;
	    		printk("The Duty_Cycle is %d \n",duty_cycle);
          		t=(tim)*1000/(cycle_duration);
          		x=(int) ((duty_cycle*cycle_duration)/100);
          		y=(int) (((100-duty_cycle)*cycle_duration)/100);

							on=x;
							off=y;
	    		break;

		case SET_IOPIN1:
            		printk("Entered SET_IO-PIN1\n");
			temp=gpio_pins(argu);
			for(z=0;z<4;z++)
				{	args[z]=temp[z];
					tmp[z]=temp[z];
					printk("%lu\t",args[z]);
				}
            		RED_LED=(unsigned int)args[0];
	    		printk("The RED_LED is %d \n",RED_LED);
			gpio_export(RED_LED,1);
	    		gpio_request(RED_LED,"RED_LED");
	    		gpio_direction_output(RED_LED,0);

			{for(i=1;i<4;i++)
			{	if(args[i]<100)
				{	gpio_export((unsigned int)args[i],1);
					gpio_request((unsigned int)args[i],"red_LED");
	    				gpio_direction_output((unsigned int)args[i],0);
					gpio_set_value_cansleep((unsigned int)args[i],0);
				}

				else;
			}}

            		break;

        	case SET_IOPIN2:
            		printk("Entered SET_IO-PIN2\n");
			temp=gpio_pins(argu);
			for(z=0;z<4;z++)
				{
					args[z]=temp[z];
					tmp[z+4]=temp[z];
					printk("%lu\t",args[z]);
				}
            		GREEN_LED=(unsigned int)args[0];
	    		printk("The  GREEN_LED is %d \n", GREEN_LED);
			gpio_export(GREEN_LED,1);
	    		gpio_request(GREEN_LED,"GREEN_LED");
	    		gpio_direction_output(GREEN_LED,0);
					//if(args[0]!=7 || args[0]!=8)
			{for(i=1;i<4;i++)
			{	if(args[i]<100)
				{	gpio_export((unsigned int)args[i],1);
					gpio_request((unsigned int)args[i],"red_LED");
	    				gpio_direction_output((unsigned int)args[i],0);
					gpio_set_value_cansleep((unsigned int)args[i],0);
				}

				else;
			}}
            		break;

        	case SET_IOPIN3:
            		printk("Entered SET_IO-PIN3\n");
			temp=gpio_pins(argu);
			for(z=0;z<4;z++)
				{
					args[z]=temp[z];
					tmp[z+8]=temp[z];
					printk("%lu\t",args[z]);
				}
            		BLUE_LED = (unsigned int)args[0];
	    		printk("The BLUE_LED is %d \n",BLUE_LED);
			gpio_export(BLUE_LED,1);
			gpio_request(BLUE_LED,"BLUE_LED");
	    		gpio_direction_output(BLUE_LED,0);

			{for(i=1;i<4;i++)
			{	if(args[i]<100)
				{	gpio_export((unsigned int)args[i],1);
					gpio_request((unsigned int)args[i],"red_LED");
	    				gpio_direction_output((unsigned int)args[i],0);
					gpio_set_value_cansleep((unsigned int)args[i],0);
				}

				else;
			}}

            		break;

        	default:
            		return -EINVAL;
    	}

    	return 0;
}


/**********RGBLed write function**********************/
ssize_t RGBLed_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
  	int res =0;
  	my_devp = file->private_data;

  	printk(KERN_INFO "DRIVER TO WRITE\n");
  	res = copy_from_user((void *)&val, buf, sizeof(val)); // GET A COPY FROM THE USER //
  	bal=val;

	spin_lock (&my_lock);
	/***convert the number in to corresponding binary***/
	a=val%2;
	val=(int)(val/2);

	b=val%2;
	val=(int)(val/2);
	
	if(bal>3)
	{
	   c=val%2;
	   val/=2;
	}
	else
	{
		c=0;
	}
	spin_unlock (&my_lock);

	if (addflag==0)
	{
		add_timer(&my_timer); // start the timer
		addflag=1;
 	}

	return res;
}

/**********************File operations*******************/
static struct file_operations fops={
.owner = THIS_MODULE,
.open = RGBLed_open,
.release = RGBLed_release,
.unlocked_ioctl = RGBLed_ioctl,
.write = RGBLed_write
};

/****************Initialization of RGBLed driver*********/
int __init RGBLed_driver_init(void)
{
  	int ret;

	my_devp=kmalloc(sizeof(struct my_dev),GFP_KERNEL);

	if(!my_devp)
		printk("Bad Malloc\n");
	
  	if (alloc_chrdev_region(&dev,MINOR_NUMBER,NUM_DEVICE, DEVICE_NAME)<0)     /* Request dynamic allocation of a device major number */
  	{
    	printk(KERN_DEBUG "Can't register device\n");
		return -1;
  	}

  	my_class = class_create(THIS_MODULE,DEVICE_NAME);
  	strcpy(my_devp->name,"RGBLed");

	/* Connect the file operations with the cdev */
   	cdev_init(&my_devp->cdev, &fops);
    
	my_devp->cdev.owner = THIS_MODULE;

   	/* Connect the major/minor number to the cdev */
    ret = cdev_add(&my_devp->cdev,MKDEV(MAJOR(dev), MINOR(dev)),NUM_DEVICE);
    if (ret)
		return ret;

	/* Send uevents to udev, so it'll create /dev nodes */
   	device_create(my_class, NULL, MKDEV(MAJOR(dev), MINOR(dev)),NULL,"%s","RGBLed");printk("RGBLed CREATED SUCCESSFULLY\n");

	/*** initialize the timer ***/
	init_timer(&my_timer);
	my_timer.expires = jiffies  ;
	my_timer.function = timer_function;
	my_timer.data = 0;

  	printk("Driver initialized.\n");

  	return 0;
}


/*******************RGBLed exit function*********************/
void __exit RGBLed_driver_exit(void)
{
  	unregister_chrdev_region(dev,1);
    /* Destroy device */
    device_destroy (my_class, MKDEV(MAJOR(dev),MINOR(dev)));
   	cdev_del(&my_devp->cdev);
  	/* Destroy driver_class */
  	class_destroy(my_class);
  	printk("Driver removed.\n");
}

module_init (RGBLed_driver_init);	/* Module initialization */
module_exit (RGBLed_driver_exit);	/* Module exit */
MODULE_LICENSE("GPL v2");

/****Function to maintain the
***gpio table
***/
unsigned long* gpio_pins(unsigned long io)
{
	switch(io)
	{
		case 0:
		{
			gpio[0]=11;
			gpio[1]=32;
			gpio[2]=200;
			gpio[3]=200;
			break;
		}

		case 1:
		{
			gpio[0]=12;gpio[1]=28;gpio[2]=45;gpio[3]=200;
			break;
		}
		case 2:
		{
			gpio[0]=13;gpio[1]=77;gpio[2]=34;gpio[3]=200;
			break;
		}
		case 3:
		{
			gpio[0]=14;gpio[1]=16;gpio[2]=76;gpio[3]=64;
			break;
		}
		case 4: 
		{
			gpio[0]=6;gpio[1]=36;gpio[2]=200;gpio[3]=200;
			break;
		}
		case 5:
		{
			gpio[0]=0;gpio[1]=18;gpio[2]=66;gpio[3]=200;
			break;
		} 
		case 6: 
		{
			gpio[0]=1;gpio[1]=20;gpio[2]=68;gpio[3]=200;
			break;
		}
		case 7:
		{
			gpio[0]=38;gpio[1]=28;gpio[2]=45;gpio[3]=18;
			break;
		}
		case 8:
		{
			gpio[0]=40;gpio[1]=28;gpio[2]=45;gpio[3]=18;
			break;
		}
		case 9:
		{
			gpio[0]=4;gpio[1]=22;gpio[2]=70;gpio[3]=200;
			break;
		}
		case 10:
		{
			gpio[0]=10;gpio[1]=26;gpio[2]=74;gpio[3]=200;
			break;
		}
		case 11:
		{
			gpio[0]=5;gpio[1]=24;gpio[2]=44;gpio[3]=72;
			break;
		}
		case 12: 
		{
			gpio[0]=15;gpio[1]=42;gpio[2]=200;gpio[3]=200;
			break;
		}
		case 13:
		{
			gpio[0]=7;gpio[1]=46;gpio[2]=30;gpio[3]=200;
			break;
		}
		default:
		{
			printk("INVALID IO PIN...\n");
		}
	}

	return gpio;

}
