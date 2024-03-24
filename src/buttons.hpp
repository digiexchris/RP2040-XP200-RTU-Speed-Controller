#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>
#include "modbus/CommandQueue.hpp"

class Button
{
public:
    Button(const struct gpio_dt_spec aButton, Action anAction) : myButton(aButton), myAction(anAction)
    {
        if (!gpio_is_ready_dt(&myButton))
        {
            printk("Error: button device %s is not ready\n",
                   myButton.port->name);
            return;
        }

        int err = gpio_pin_configure_dt(&myButton, GPIO_INPUT);
        if (err != 0)
        {
            printk("Error %d: failed to configure %s pin %d\n",
                   err, myButton.port->name, myButton.pin);
            return;
        }

        err = gpio_pin_interrupt_configure_dt(&myButton,
                                              GPIO_INT_EDGE_TO_ACTIVE);
        if (err != 0)
        {
            printk("Error %d: failed to configure interrupt on %s pin %d\n",
                   err, myButton.port->name, myButton.pin);
            return;
        }

        gpio_init_callback(&myButtonCallback, ISR, BIT(myButton.pin));
        gpio_add_callback(myButton.port, &myButtonCallback);
        printk("Set up button at %s pin %d\n", myButton.port->name, myButton.pin);
    }

private:
    const struct gpio_dt_spec myButton;
    const Action myAction;
    struct gpio_callback myButtonCallback;
    static void ISR(const struct device *port,
                    struct gpio_callback *cb,
                    gpio_port_pins_t pins)
    {
        Button *button = CONTAINER_OF(cb, Button, myButtonCallback);
        button->QueueCommand();
    }

    void QueueCommand()
    {
        enqueueCommand(&coilCommands[myAction]);
    }
};
