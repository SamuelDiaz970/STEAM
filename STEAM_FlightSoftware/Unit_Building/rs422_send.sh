#!/bin/bash

# configure RS-422 port
stty -F /dev/ttyPS1 115200

# output hello world message to RS-422 port
#while :
#do
	echo "Hello World!\n" > /dev/ttyPS1

#	sleep 1
#done
