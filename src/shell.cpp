/*
 * Copyright (c) 2015 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>

static int GetParamsInit(void)
{
	printk("Get and print all params from the XP200 device.\n");
	if (!CONFIG_SHELL_CMD_ROOT[0])
	{
		shell_set_root_cmd("params");
	}
	return 0;
}

static int GetCoilsInit(void)
{
	printk("Get and print all coils from the XP200 device.\n");
	if (!CONFIG_SHELL_CMD_ROOT[0])
	{
		shell_set_root_cmd("coils");
	}
	return 0;
}

static int GetAllCoilsCmd(const struct shell *sh, size_t argc, char **argv)
{
	shell_set_root_cmd("coils");
	shell_print(sh, "\n");
	return 0;
}

static int GetAllParamsCmd(const struct shell *sh, size_t argc, char **argv)
{
	shell_set_root_cmd("params");
	shell_print(sh, "\n");
	return 0;
}

SHELL_CMD_REGISTER(params, NULL, "Get all params.", GetAllParamsCmd);
SHELL_CMD_REGISTER(coils, NULL, "Get all coils.", GetAllCoilsCmd);

SYS_INIT(GetParamsInit, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
SYS_INIT(GetCoilsInit, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);