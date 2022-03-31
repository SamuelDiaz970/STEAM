#include <stdio.h>
#include <unistd.h>

#include <gpiod.h> /* needs LIBGPIOD */


#define SXR_GPIOCHIP_ID     ((const char *)"gpiochip12")
#define SXR_GPIOLINE_ID     (0U)

#define HXR_GPIOCHIP_ID     ((const char *)"gpiochip11")
#define HXR_GPIOLINE_ID     (0U)

/* global handles */
static struct gpiod_chip *chip_hxr = NULL;
static struct gpiod_chip *chip_sxr = NULL;
static struct gpiod_line *hxr_gpio = NULL;
static struct gpiod_line *sxr_gpio = NULL;

void gpio_init(void)
{
	chip_sxr = gpiod_chip_open_by_name(SXR_GPIOCHIP_ID);
    chip_hxr = gpiod_chip_open_by_name(HXR_GPIOCHIP_ID);

    sxr_gpio = gpiod_chip_get_line(chip_sxr, 0);
	hxr_gpio = gpiod_chip_get_line(chip_hxr, 0);

    /* set it to output */
	gpiod_line_request_output(sxr_gpio, "sxr gpio", 0);
    gpiod_line_request_output(hxr_gpio, "hxr gpio", 0);
}

int gpio_config_sxr(bool on)
{

    if(sxr_gpio == NULL)
    {
        return -1; //error with driver 
    }

    if(on == true)
    {
        gpiod_line_set_value(sxr_gpio, 1);
    }
    else
    {
        gpiod_line_set_value(sxr_gpio, 0);
    }

    return 0;
}

int gpio_config_hxr(bool on)
{

    if(hxr_gpio == NULL)
    {
        return -1; //error with driver 
    }

    if(on == true)
    {
        gpiod_line_set_value(hxr_gpio, 1);
    }
    else
    {
        gpiod_line_set_value(hxr_gpio, 0);
    }

    return 0;
}

void gpio_deinit(void)
{
    if(hxr_gpio != NULL)
    {
        gpiod_line_release(hxr_gpio);
    }

    if(sxr_gpio != NULL)
    {
        gpiod_line_release(sxr_gpio);
    }

    if(chip_sxr != NULL)
    {
        gpiod_chip_close(chip_sxr);
    }

    if(chip_hxr != NULL)
    {
        gpiod_chip_close(chip_hxr);
    }
}
