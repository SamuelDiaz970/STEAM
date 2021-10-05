#!/bin/bash                                                 
                                 
GPIO ="gpiochip9"            
ORANGE ="1"                                                           
GREEN ="2"                                                            
                                                            
echo "Current state of orange and green LEDs: 'gpioget gpiochip9 1 2'"
echo "Setting both LEDs to low: 'gpioset gpiochip9 1=0 2=0'"
echo "Starting toggle routine..."
                             
while true; do               
                             
        ##echo "Orange high" 
        gpioset gpiochip9 1=1
        sleep 1              
                             
        ##echo "Green high"  
        gpioset gpiochip9 2=1
        sleep 1              
                             
        ##echo "Orange low"  
        gpioset gpiochip9 1=0
                             
        ##echo "Green low"   
        gpioset gpiochip9 2=0
        sleep 1              
                             
        ##echo "Green high"  
        gpioset gpiochip9 2=1
        sleep 1       

        ##echo "Orange high" 
        gpioset gpiochip9 1=1
        sleep 1              
                             
        ##echo "Orange low"  
        gpioset gpiochip9 1=0
                             
        ##echo "Green low"   
        gpioset gpiochip9 2=0
        sleep 1              
                             
done

