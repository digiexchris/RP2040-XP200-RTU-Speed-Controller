#include "buttonbit.hpp"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>

ZBUS_CHAN_DEFINE(BUTTONS,						   /* Name */
				 ButtonStateMsg *,				   /* Message type */
				 NULL,							   /* Validator */
				 NULL,							   /* User data */
				 ZBUS_OBSERVERS(BUTTON_STATE_SUB), /* observers */
				 ZBUS_MSG_INIT(new ButtonStateMsg()));
