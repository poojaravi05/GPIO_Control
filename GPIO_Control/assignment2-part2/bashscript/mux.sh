#!/bin/bash

echo -n $1 > /sys/class/gpio/export
echo -n out > /sys/class/gpio/gpio$1/direction
echo -n 0 > /sys/class/gpio/gpio$1/value
