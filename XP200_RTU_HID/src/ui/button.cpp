#include "button.hpp"
#include "buttonbit.hpp"

Button::Button(const struct gpio_dt_spec aButton, ButtonBit aBit) : myGPIO(aButton), myBit(aBit)
{
	if (!gpio_is_ready_dt(&myGPIO))
	{
		printk("Error: button device %s is not ready\n",
			   myGPIO.port->name);
		return;
	}

	int err = gpio_pin_configure_dt(&myGPIO, GPIO_INPUT);
	if (err != 0)
	{
		printk("Error %d: failed to configure %s pin %d\n",
			   err, myGPIO.port->name, myGPIO.pin);
		return;
	}

	err = gpio_pin_interrupt_configure_dt(&myGPIO,
										  GPIO_INT_EDGE_TO_ACTIVE);
	if (err != 0)
	{
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			   err, myGPIO.port->name, myGPIO.pin);
		return;
	}

	gpio_init_callback(&myButtonCallback, ISR, BIT(myGPIO.pin));
	gpio_add_callback(myGPIO.port, &myButtonCallback);
	printk("Set up button at %s pin %d\n", myGPIO.port->name, myGPIO.pin);
}