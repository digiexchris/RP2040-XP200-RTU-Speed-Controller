/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>
#include "buttons.hpp"
#include "modbus/CommandQueue.hpp"

#define SLEEP_TIME_MS 1

/*
 * Get button configuration from the devicetree sw0 alias. This is mandatory.
 */
#define EncSwitch_NODE DT_ALIAS(EncSwitch)
#if !DT_NODE_HAS_STATUS(EncSwitch_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif

static const struct Button buttons[] = {
    {GPIO_DT_SPEC_GET_OR(EncSwitch, gpios, {0}), Action::EncSwitch},
    {GPIO_DT_SPEC_GET_OR(RunF, gpios, {0}), Action::RunF},
    {GPIO_DT_SPEC_GET_OR(RunR, gpios, {0}), Action::RunR},
    {GPIO_DT_SPEC_GET_OR(Stop, gpios, {0}), Action::Stop},
    {GPIO_DT_SPEC_GET_OR(Enable, gpios, {0}), Action::Enable},
    {GPIO_DT_SPEC_GET_OR(JogF, gpios, {0}), Action::JogF},
    {GPIO_DT_SPEC_GET_OR(JogR, gpios, {0}), Action::JogR},
    {GPIO_DT_SPEC_GET_OR(EStop, gpios, {0}), Action::EStop},
};

void button_pressed(const struct device *dev, struct gpio_callback *cb,
                    uint32_t pins)
{
    printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

int main(void)
{
    int ret;

        return 0;
}