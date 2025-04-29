#include "modbus.hpp"
#include "zephyr/kernel.h"

LOG_MODULE_REGISTER(modbus, LOG_LEVEL_DBG);

#define MODBUS_NODE DT_CHOSEN(zephyr_modbus_serial)
#define MODBUS_UART_NODE DT_PARENT(MODBUS_NODE)
#define MODBUS_UART_SPEED DT_PROP(MODBUS_UART_NODE, current_speed)

Modbus::Modbus(const char *anIfaceName, const uint32_t aUartSpeed, const uint32_t anRxTimeout, const uint8_t aUnitId) : myIfaceName(anIfaceName), myUnitId(aUnitId)

{
    myClientParam.rx_timeout = anRxTimeout;
    myClientParam.serial.baud = aUartSpeed;

    myClientIface = modbus_iface_get_by_name(myIfaceName.c_str());

    assert(myClientIface >= 0);

    myConnectPollingThreadStack = k_thread_stack_alloc(4096);

    k_thread_create(NULL, myConnectPollingThreadStack,
                    4096,
                    ConnectLoop,
                    this, NULL, NULL,
                    5, K_ESSENTIAL, K_NO_WAIT);
}

void Modbus::ConnectLoop(void *p1, void *, void *)
{

    Modbus *instance = static_cast<Modbus *>(p1);

    while (42)
    {
        if (!instance->myIsInitialized)
        {
            if (modbus_init_client(instance->myClientIface, instance->myClientParam))
            {
                LOG_ERR("Modbus RTU client initialization failed");
                k_sleep(Z_TIMEOUT_MS(100));
                continue;
            }
            instance->myIsInitialized = true;
        }

        if (instance->RequestDriveDiagnostics())
        {
            if (!instance->myIsConnected)

            {
                instance->myIsConnected = true;
                LOG_INF("Drive reconnected");
            }
        }
        else
        {
            LOG_ERR("Drive not connected");
            instance->myIsConnected = false;
            k_sleep(Z_TIMEOUT_MS(100));
            continue;
        }

        k_sleep(Z_TIMEOUT_MS(500));
    }
}

bool Modbus::IsDriveConnected()
{
    return myIsConnected;
}

bool Modbus::RequestDriveDiagnostics()
{
    uint16_t result;
    int err = modbus_request_diagnostic(myClientIface, myUnitId, 0x08, 0x1234, &result);

    if (result != 0x1234)
    {
        return false;
    }

    return true;
}

// bool Modbus::Start()
// {
//     if (!WriteParam(WriteableParams::Run, 1))
//     {
//         return false;
//     }
// }

// bool Modbus::Stop()
// {
//     if (!WriteParam(WriteableParams::Run, 0))
//     {
//         return false;
//     }
// }

// note: negative is reverse, positive is forwards, 0 is stop
bool Modbus::SetSpeed(int16_t aSpeed)
{

    if (!WriteParam(WriteableParams::SetRPM, aSpeed))
    {
        return false;
    }
}

Packet Modbus::ReadStatus(uint16_t address)
{
    uint16_t regs[1] = {0};

    uint16_t result = modbus_read_input_regs(myClientIface, myUnitId, address, regs, 1);
    if (result != 0)
    {
        LOG_ERR("ReadInputRegisters failed with %d", err);
        return Packet{FunctionCode::ReadInputRegisters, address, ERROR_VALUE};
    }

    return Packet{FunctionCode::ReadInputRegisters, address, regs[0]};
}

bool Modbus::WriteParam(uint16_t address, uint16_t value)
{
    uint16_t result = modbus_write_holding_reg(myClientIface, myUnitId, address, value);
    if (result != 0)
    {
        LOG_ERR("WriteSingleRegister failed with %d", err);
        return false;
    }

    return true;
};

Packet Modbus::ReadParam(uint16_t address)
{
    uint16_t regs[1] = {0};

    int result = modbus_read_holding_regs(myClientIface, myUnitId, address, regs, 1);
    if (result != 0)
    {
        LOG_ERR("ReadHoldingRegisters failed with %d", result);
        return Packet{FunctionCode::ReadHoldingRegisters, address, ERROR_VALUE};
    }

    return return Packet{FunctionCode::ReadInputRegisters, address, regs[0]};
}

bool Modbus::EnableDrive()
{
    // need tp find a way to send a function that's not in zephyr's function list. modbus_raw_something probably with a custom context.
    return false;
    // return modbus_raw_submit_rx(FunctionCode::WriteSingleCoil, 0x55);
}

bool Modbus::DisableDrive()
{
    return false;
    // return WriteParam(FunctionCode::WriteSingleCoil, 0xAA);
}