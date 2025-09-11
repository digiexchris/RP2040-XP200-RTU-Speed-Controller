#pragma once

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util_macro.h>
#include <zephyr/zbus/zbus.h>

#include "actions.hpp"

struct WriteMsg
{
    Action action = Action::None;
};

class Bus
{
public:
    Bus();

private:
};