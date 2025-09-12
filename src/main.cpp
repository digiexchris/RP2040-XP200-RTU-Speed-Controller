/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <inttypes.h>
#include <zephyr/device.h>

#include <zephyr/kernel.h>
#include <zephyr/modbus/modbus.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#include "modbus/modbus.hpp"
#include "ui/buttons.hpp"

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

#define SLEEP_TIME_MS 1

// const struct device *uart0 = DEVICE_DT_GET(DT_NODELABEL(uart0));

#define MODBUS_NODE DT_CHOSEN(zephyr_modbus_serial)
#define MODBUS_UART_NODE DT_PARENT(MODBUS_NODE)
#define MODBUS_UART_SPEED DT_PROP(MODBUS_UART_NODE, current_speed)

const XP200RTU *modbusClient = nullptr;

const Buttons *buttons = nullptr;

int main(void)
{

	const char *modbusInterfaceName = DEVICE_DT_NAME(MODBUS_NODE);
	modbusClient = new XP200RTU(modbusInterfaceName, MODBUS_UART_SPEED);
	// uint16_t holding_reg[8] = {'H', 'e', 'l', 'l', 'o'};
	// const uint8_t coil_qty = 3;
	// uint8_t coil[1] = {0};
	// const int32_t sleep = 250;
	// static uint8_t node = 1;
	// int err;

	// err = modbus_write_holding_regs(client_iface, node, 0, holding_reg,
	//                                 ARRAY_SIZE(holding_reg));
	// if (err != 0)
	// {
	//     LOG_ERR("FC16 failed with %d", err);
	//     return 0;
	// }

	// err = modbus_read_holding_regs(client_iface, node, 0, holding_reg,
	//                                ARRAY_SIZE(holding_reg));
	// if (err != 0)
	// {
	//     LOG_ERR("FC03 failed with %d", err);
	//     return 0;
	// }

	// LOG_HEXDUMP_INF(holding_reg, sizeof(holding_reg),
	//                 "WR|RD holding register:");

	buttons = Buttons::GetInstance();

	while (42)
	{
		// uint16_t addr = 0;

		// err = modbus_read_coils(client_iface, node, 0, coil, coil_qty);
		// if (err != 0)
		// {
		//     LOG_ERR("FC01 failed with %d", err);
		//     return 0;
		// }

		// LOG_INF("Coils state 0x%02x", coil[0]);

		// err = modbus_write_coil(client_iface, node, addr++, true);
		// if (err != 0)
		// {
		//     LOG_ERR("FC05 failed with %d", err);
		//     return 0;
		// }

		// k_msleep(sleep);
		// err = modbus_write_coil(client_iface, node, addr++, true);
		// if (err != 0)
		// {
		//     LOG_ERR("FC05 failed with %d", err);
		//     return 0;
		// }

		// k_msleep(sleep);
		// err = modbus_write_coil(client_iface, node, addr++, true);
		// if (err != 0)
		// {
		//     LOG_ERR("FC05 failed with %d", err);
		//     return 0;
		// }

		k_msleep(500);
		// int err = modbus_read_coils(client_iface, node, 0, coil, coil_qty);
		// if (err != 0)
		// {
		//     LOG_ERR("FC01 failed with %d", err);
		//     return 0;
		// }

		// LOG_INF("Coils state 0x%02x", coil[0]);

		// coil[0] = 0;
		// err = modbus_write_coils(client_iface, node, 0, coil, coil_qty);
		// if (err != 0)
		// {
		//     LOG_ERR("FC15 failed with %d", err);
		//     return 0;
		// }

		// k_msleep(sleep);
	}
}