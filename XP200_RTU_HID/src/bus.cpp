#include "bus.hpp"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util_macro.h>
#include <zephyr/zbus/zbus.h>

#include "machine/machine.hpp"
ZBUS_CHAN_DEFINE_WITH_ID(WRITE,                                 /* Name */
                         0x12345678,                            /* Unique channel identifier */
                         WriteMsg *,                            /* Message type */
                         NULL,                                  /* Validator */
                         Machine::GetInstance()->GetHIDState(), /* User data */
                         ZBUS_OBSERVERS_EMPTY,                  /* observers */
                         new WriteMsg());

Bus::Bus()
{
    /* Initial value */
    //);
}