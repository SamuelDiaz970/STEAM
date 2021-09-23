#include<gpiod.h>
#include<stdio.h>
#include<unistd.h>
#include<time.h>

//also check out ~/Desktop/Q7_BASE/misc/u-boot/include

int main(int argc, char *argv[]){
	const char *chipname = "gpiochip4";
	struct gpiod_chip *chip;
	struct gpiod_line *pwm_gpio;
	chip = gpiod_chip_open_by_name(chipname);
	pwm_gpio = gpiod_chip_get_line(chip,1);
	gpiod_line_request_output(pwm_gpio, "pwm gpio", 0);

	//argv[1] is the duty cycle percentage
	//argv[2] is the period in seconds
	float duty_cycle = argv[1]/100;
	float period = argv[2];
	float freq = 1/period;
	while(!feof(stdin)){ //ctrl d will stop the loop
		gpiod_line_set_value(pwm_gpio, 1);
		usleep(duty_cycle * period * 1000000); //multiply by 1000000 to get seconds (have to use usleep instead of sleep because sleep only takes in ints for seconds of sleep)
		gpiod_line_set_value(pwm_gpio, 0);
		usleep((1-duty_cycle) * period * 1000000);
	}
	gpiod_line_set_value(pwm_gpio, 0); //double check the gpio is set to off
	gpiod_line_release(pwm_gpio);
	gpiod_chip_close(chip);
	return 0;
}
