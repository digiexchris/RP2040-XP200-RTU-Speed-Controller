#pragma once

#include <inttypes.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/zbus/zbus.h>

#include "buttonbit.hpp"

extern struct zbus_channel BUTTONS;

class Button
{
	friend class Buttons;

public:
	Button(const struct gpio_dt_spec aButton, ButtonBit aBit);

private:
	const struct gpio_dt_spec myGPIO;
	const ButtonBit myBit;
	struct gpio_callback myButtonCallback;
	static void ISR(const struct device *port,
					struct gpio_callback *cb,
					gpio_port_pins_t pins)
	{
		Button *button = CONTAINER_OF(cb, Button, myButtonCallback);

		button->Publish();
	}

	bool IsSet() const
	{
		return gpio_pin_get_dt(&myGPIO);
	}

	void Publish()
	{
		bool state = gpio_pin_get_dt(&myGPIO);
		ButtonStateMsg msg;
		msg.state = myBit;
		msg.isSet = state;
		zbus_chan_pub(&BUTTONS, &msg, K_NO_WAIT);
	}
};
