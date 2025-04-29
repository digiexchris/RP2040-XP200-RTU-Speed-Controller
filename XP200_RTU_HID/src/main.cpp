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
#include <zephyr/device.h>
#include <zephyr/modbus/modbus.h>

#include "ui/buttons.hpp"
#include "modbus/CommandQueue.hpp"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

#define SLEEP_TIME_MS 1

/*
 * Get button configuration from the devicetree sw0 alias. This is mandatory.
 */
// #define EncSwitch_NODE DT_ALIAS(EncSwitch)
// #if !DT_NODE_HAS_STATUS(EncSwitch_NODE, okay)
// #error "Unsupported board: sw0 devicetree alias is not defined"
// #endif

// static const struct Button buttons[] = {
//     {GPIO_DT_SPEC_GET_OR(EncSwitch, gpios, {0}), Action::EncSwitch},
//     {GPIO_DT_SPEC_GET_OR(RunF, gpios, {0}), Action::RunF},
//     {GPIO_DT_SPEC_GET_OR(RunR, gpios, {0}), Action::RunR},
//     {GPIO_DT_SPEC_GET_OR(Stop, gpios, {0}), Action::Stop},
//     {GPIO_DT_SPEC_GET_OR(Enable, gpios, {0}), Action::Enable},
//     {GPIO_DT_SPEC_GET_OR(JogF, gpios, {0}), Action::JogF},
//     {GPIO_DT_SPEC_GET_OR(JogR, gpios, {0}), Action::JogR},
//     {GPIO_DT_SPEC_GET_OR(EStop, gpios, {0}), Action::EStop},
// };

// const struct device *uart0 = DEVICE_DT_GET(DT_NODELABEL(uart0));
static int client_iface;

#define MODBUS_NODE DT_CHOSEN(zephyr_modbus_serial)
#define MODBUS_UART_NODE DT_PARENT(MODBUS_NODE)
#define MODBUS_UART_SPEED DT_PROP(MODBUS_UART_NODE, current_speed)

const static struct modbus_iface_param client_param = {
    .mode = MODBUS_MODE_RTU,
    .rx_timeout = 50000,
    .serial = {
        .baud = MODBUS_UART_SPEED,
        .parity = UART_CFG_PARITY_NONE,
        .stop_bits_client = UART_CFG_STOP_BITS_2,
    },
};

static int init_modbus_client(void)
{
    const char *iface_name = DEVICE_DT_NAME(MODBUS_NODE);

    client_iface = modbus_iface_get_by_name(iface_name);
    return modbus_init_client(client_iface, client_param);
}

int main(void)
{
    uint16_t holding_reg[8] = {'H', 'e', 'l', 'l', 'o'};
    const uint8_t coil_qty = 3;
    uint8_t coil[1] = {0};
    const int32_t sleep = 250;
    static uint8_t node = 1;
    int err;

    if (init_modbus_client())
    {
        LOG_ERR("Modbus RTU client initialization failed");
        return 0;
    }

    err = modbus_write_holding_regs(client_iface, node, 0, holding_reg,
                                    ARRAY_SIZE(holding_reg));
    if (err != 0)
    {
        LOG_ERR("FC16 failed with %d", err);
        return 0;
    }

    err = modbus_read_holding_regs(client_iface, node, 0, holding_reg,
                                   ARRAY_SIZE(holding_reg));
    if (err != 0)
    {
        LOG_ERR("FC03 failed with %d", err);
        return 0;
    }

    LOG_HEXDUMP_INF(holding_reg, sizeof(holding_reg),
                    "WR|RD holding register:");

    while (true)
    {
        uint16_t addr = 0;

        err = modbus_read_coils(client_iface, node, 0, coil, coil_qty);
        if (err != 0)
        {
            LOG_ERR("FC01 failed with %d", err);
            return 0;
        }

        LOG_INF("Coils state 0x%02x", coil[0]);

        err = modbus_write_coil(client_iface, node, addr++, true);
        if (err != 0)
        {
            LOG_ERR("FC05 failed with %d", err);
            return 0;
        }

        k_msleep(sleep);
        err = modbus_write_coil(client_iface, node, addr++, true);
        if (err != 0)
        {
            LOG_ERR("FC05 failed with %d", err);
            return 0;
        }

        k_msleep(sleep);
        err = modbus_write_coil(client_iface, node, addr++, true);
        if (err != 0)
        {
            LOG_ERR("FC05 failed with %d", err);
            return 0;
        }

        k_msleep(sleep);
        err = modbus_read_coils(client_iface, node, 0, coil, coil_qty);
        if (err != 0)
        {
            LOG_ERR("FC01 failed with %d", err);
            return 0;
        }

        LOG_INF("Coils state 0x%02x", coil[0]);

        coil[0] = 0;
        err = modbus_write_coils(client_iface, node, 0, coil, coil_qty);
        if (err != 0)
        {
            LOG_ERR("FC15 failed with %d", err);
            return 0;
        }

        k_msleep(sleep);
    }
}