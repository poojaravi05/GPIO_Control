#!/bin/bash



# defining the period & time between evets in usec
cycle_duration=20000;
tim=500000;



# reading values from user
read -p "Enter the PWM :" pwm
read -p "Enter the RED LED  connected to : " r
./selection.sh $r
RED_LED=$?
./mux.sh $RED_LED
read -p "Enter the GREEN LED  connected to : " g
./selection.sh $g
GREEN_LED=$?
./mux.sh $GREEN_LED
read -p "Enter the BLUE LED  connected to : " b
./selection.sh $b
BLUE_LED=$?
./mux.sh $BLUE_LED

#checking if its a valid input
c=$?
if [ "$pwm" -gt "100" ] || [ "$pwm" -lt "0" ]; then
  echo invalid pwm entered
  exit
fi
if [ "$r" -gt "13" ] || [ "$r" -lt "0" ] ; then
  echo invalid red pin entered
  exit
fi

if [ "$g" -gt "13" ] || [ "$g" -lt "0" ] ; then
  echo invalid green pin entered
  exit
fi

if [ "$b" -gt "13" ] || [ "$b" -lt "0" ] ; then
  echo invalid green pin entered
  exit
fi


t=$(( tim / cycle_duration))
i=$(( pwm * cycle_duration /100000))
j=$(( (100-pwm) * cycle_duration /100000))

#echo "sequence time in usecond $t"
echo "on time =$i ms"
echo "off time =$j ms"


globe=1

while [ $globe ];do
  echo -n 0 > /sys/class/gpio/gpio$GREEN_LED/value
  echo -n 0 > /sys/class/gpio/gpio$BLUE_LED/value
  for as in `seq 1 $t` ; do
    echo -n 1 > /sys/class/gpio/gpio$RED_LED/value
    # on time delay
    for as in `seq 1 $i` ; do
  i=1;  i=1;  i=1;
        done
    echo -n 0 > /sys/class/gpio/gpio$RED_LED/value
    # off time delay
    for as in `seq 1 $j` ; do
  i=1;  i=1;  i=1;
          done
  done
echo -n 0 > /sys/class/gpio/gpio$RED_LED/value
echo -n 0 > /sys/class/gpio/gpio$BLUE_LED/value
  for as in `seq 1 $t` ; do
    echo -n 1 > /sys/class/gpio/gpio$GREEN_LED/value
    for as in `seq 1 $i` ; do
  i=1;  i=1;  i=1;
done
    echo -n 0 > /sys/class/gpio/gpio$GREEN_LED/value
    for as in `seq 1 $j` ; do
  i=1;  i=1;  i=1;
done
  done
  echo -n 0 > /sys/class/gpio/gpio$RED_LED/value
  echo -n 0 > /sys/class/gpio/gpio$GREEN_LED/value
  for as in `seq 1 $t` ; do
  echo -n 1 > /sys/class/gpio/gpio$BLUE_LED/value
    for as in `seq 1 $i` ; do
  i=1;  i=1;  i=1;
done
    echo -n 0 > /sys/class/gpio/gpio$BLUE_LED/value
    for as in `seq 1 $j` ; do
  i=1;  i=1;  i=1;
done
  done
  echo -n 0 > /sys/class/gpio/gpio$BLUE_LED/value
    for as in `seq 1 $t` ; do
      echo -n 1 > /sys/class/gpio/gpio$RED_LED/value
      echo -n 1 > /sys/class/gpio/gpio$GREEN_LED/value
      for as in `seq 1 $i` ; do
  i=1;  i=1;  i=1;
done
      echo -n 0 > /sys/class/gpio/gpio$RED_LED/value
      echo -n 0 > /sys/class/gpio/gpio$GREEN_LED/value
      for as in `seq 1 $j` ; do
  i=1;  i=1;  i=1;
done
    done
    echo -n 0 > /sys/class/gpio/gpio$RED_LED/value
  for as in `seq 1 $t` ; do
    echo -n 1 > /sys/class/gpio/gpio$GREEN_LED/value
    echo -n 1 > /sys/class/gpio/gpio$BLUE_LED/value
    for as in `seq 1 $i` ; do
  i=1;  i=1;  i=1;
done
    echo -n 0 > /sys/class/gpio/gpio$GREEN_LED/value
    echo -n 0 > /sys/class/gpio/gpio$BLUE_LED/value
    for as in `seq 1 $j` ; do
  i=1;  i=1;  i=1;
done
  done
  echo -n 0 > /sys/class/gpio/gpio$GREEN_LED/value
  for as in `seq 1 $t` ; do
    echo -n 1 > /sys/class/gpio/gpio$RED_LED/value
    echo -n 1 > /sys/class/gpio/gpio$BLUE_LED/value
    for as in `seq 1 $i` ; do
  i=1;  i=1;  i=1;
done
    echo -n 0 > /sys/class/gpio/gpio$RED_LED/value
    echo -n 0 > /sys/class/gpio/gpio$BLUE_LED/value
    for as in `seq 1 $j` ; do
  i=1;  i=1;  i=1;
done
  done

    for as in `seq 1 $t` ; do
      echo -n 1 > /sys/class/gpio/gpio$RED_LED/value
      echo -n 1 > /sys/class/gpio/gpio$GREEN_LED/value
      echo -n 1 > /sys/class/gpio/gpio$BLUE_LED/value
      for as in `seq 1 $i` ; do
  i=1;  i=1;  i=1;
done
      echo -n 0 > /sys/class/gpio/gpio$RED_LED/value
      echo -n 0 > /sys/class/gpio/gpio$GREEN_LED/value
      echo -n 0 > /sys/class/gpio/gpio$BLUE_LED/value
      for as in `seq 1 $j` ; do
  i=1;  i=1;  i=1;
done
    done
  sleep 2
done

# unexport the pins


echo -n $RED_LED > /sys/class/gpio/unexport
echo -n $GREEN_LED > /sys/class/gpio/unexport
echo -n $BLUE_LED > /sys/class/gpio/unexport

exit
