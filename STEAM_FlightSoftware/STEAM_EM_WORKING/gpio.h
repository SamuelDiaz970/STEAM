#ifndef _GPIO_H_
#define _GPIO_H_
#include <stdbool.h>
/* global prototypes */
void gpio_init(void);
int gpio_config_sxr(bool on);
int gpio_config_hxr(bool on);
void gpio_deinit(void);

#endif /* _GPIO_H_ */