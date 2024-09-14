#!/bin/bash

echo -n $1 > /sys/class/gpio/export
echo -n 0 > /sys/class/gpio/gpio$1/value
