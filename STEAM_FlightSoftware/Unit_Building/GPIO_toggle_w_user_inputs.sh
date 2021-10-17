#!/bin/bash                                                 
                                 
sudo gpiodetect

echo "Enter gpiochip number: "
read GPIOnum
GPIO = "gpiochip" + $GPIOnum  
echo "Enter first gpio line number you wish to toggle: "
read line1
echo "Enter second gpio line number you wish to toggle: "                                                                    
read line2                                                         
                                                            
echo "Current state of line1 and line2 gpios: "
gpioget GPIO line1 line2
echo "Setting both LEDs to low: "
gpioset GPIO line1=0 line2=0
echo "Starting toggle routine..."
                             
while true; do               
                             
        echo "line1 high" 
        gpioset GPIO line1=1
        gpioget GPIO line1
        sleep 1              
                             
        echo "line2 high"  
        gpioset GPIO line2=1
        gpioget GPIO line2
        sleep 1              
                             
        echo "line1 low"  
        gpioset GPIO line1=0
        gpioget GPIO line1
                             
        echo "line2 low"   
        gpioset GPIO line2=0
        gpioget GPIO line2
        sleep 1              
                             
        echo "line2 high"  
        gpioset GPIO line2=1
        gpioget GPIO line2
        sleep 1       

        echo "line1 high" 
        gpioset GPIO line1=1
        gpioget GPIO line1
        sleep 1              
                             
        echo "line1 low"  
        gpioset GPIO line1=0
        gpioget GPIO line1
                             
        echo "line2 low"   
        gpioset GPIO line2=0
        gpioget GPIO line2
        sleep 1              
                             
done

