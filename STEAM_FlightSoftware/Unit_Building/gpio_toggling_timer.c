#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    const char *chipname = "gpiochip9";
    struct gpiod_chip *chip;
    struct gpiod_line *orangeLED; // Orange LED
    struct gpiod_line *greenLED;  // Green LED

    // Open GPIO chip
    chip = gpiod_chip_open_by_name(chipname);

    // Get GPIO lines
    orangeLED = gpiod_chip_get_line(chip, 1);
    greenLED = gpiod_chip_get_line(chip, 2);

    // Make the LED lines outputs
    gpiod_line_request_output(orangeLED, "orange", 0);
    gpiod_line_request_output(greenLED, "green", 0);

    // Blink LEDs using sleep
    gpiod_line_set_value(orangeLED, 1);
    sleep(1);
    gpiod_line_set_value(greenLED, 1);
    sleep(1);
    gpiod_line_set_value(orangeLED, 0);
    sleep(1);
    gpiod_line_set_value(greenLED, 0);

    // Release lines and chip
    gpiod_line_release(orangeLED);
    gpiod_line_release(greenLED);
    gpiod_chip_close(chip);
    return 0;
}