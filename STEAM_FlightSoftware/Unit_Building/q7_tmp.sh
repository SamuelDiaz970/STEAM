#!/bin/bash

sensors_file='sensors.txt'

# Get temp values every 3 sec, print to file (infinite loop)
while :
do
	# write output of sensors command to sensors.txt
	sensors > $sensors_file

	# read line for Zynq die temp (line 18), append to zynq_tmp.txt
	n=1

	filename='sensors.txt'

	while read line
	do
		if [$n -eq 18];
		then
			echo $line >> zynq_tmp.txt
		fi
		n=$((n+1))
	done < $sensors_file

	sleep 3
done
