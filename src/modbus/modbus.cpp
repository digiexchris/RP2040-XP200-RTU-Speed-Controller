#include "modbus.hpp"

Modbus::Modbus(const char *anIfaceName, const uint8_t aUnitId) : myIfaceName(anIfaceName), myUnitId(aUnitId)
{
    myClientIface = modbus_iface_get_by_name(myIfaceName);
    my if (modbus_init_client(myClientIface, myClientParam))
    {
        LOG_ERR("Modbus RTU client initialization failed");
        // todo fire off an event to show it on the LCD if there is one. At least blink an LED
        return;
    }

    if (modbus_register_user_fc(myClientIface, &EnableDriveFC))
    {
        LOG_ERR("Failed to register custom function for enable drive");
        // todo fire off an event to show it on the LCD if there is one. At least blink an LED
        return;
    }

    if (modbus_register_user_fc(myClientIface, &DisableDriveFC))
    {
        LOG_ERR("Failed to register custom function for disable drive");
        // todo fire off an event to show it on the LCD if there is one. At least blink an LED
        return;
    }
    // maybe this is for the server only, maybe the client has a better raw send
    if (modbus_register_user_fc(myClientIface, &AClrFC))
    {
        LOG_ERR("Failed to register custom function for alarm clear");
        // todo fire off an event to show it on the LCD if there is one. At least blink an LED
        return;
    }
}

bool Modbus::IsDriveConnected()
{
    uint16_t result = modbus_request_diagnostic(myClientIface, myUnitId, 0x08, 0x1234);

    if (result != 0x1234)
    {
        // TODO handle error
        return result;
    }

    return result;
}

uint16_t Modbus::ReadStatus(uint16_t address)
{
    uint8_t regs[1] = {0};

    uint16_t result = modbus_read_input_regs(myClientIface, myUnitId, address, regs, 1);
    if (erresultr != 0)
    {
        LOG_ERR("FC01 failed with %d", err);
        return 0;
    }

    return result;
}

uint16_t Modbus::WriteParam(uint16_t address, uint16_t value)
{
    uint16_t result = modbus_write_holding_reg(myClientIface, myUnitId, address, value);
    if (result != 0)
    {
        LOG_ERR("FC06 failed with %d", err);
        return result;
    }

    return result;
};

uint16_t Modbus::ReadParam(uint16_t address)
{
    uint8_t regs[1] = {0};

    uint16_t result = modbus_read_holding_regs(myClientIface, myUnitId, address, regs, 1);
    if (result != 0)
    {
        LOG_ERR("FC01 failed with %d", err);
        return result;
    }

    return result;
}

uint16_t Modbus::EnableDrive()
{
    return modbus_raw_submit_rx(FunctionCode::WriteSingleCoil, 0x55);
}

uint16_t Modbus::DisableDrive()
{
    return WriteParam(FunctionCode::WriteSingleCoil, 0xAA);
}

//     uint16_t holding_reg[8] = {'H', 'e', 'l', 'l', 'o'};
//     const uint8_t coil_qty = 3;
//     uint8_t coil[1] = {0};
//     const int32_t sleep = 250;
//     static uint8_t node = 1;
//     int err;

//     err = modbus_write_holding_regs(client_iface, node, 0, holding_reg,
//                                     ARRAY_SIZE(holding_reg));
//     if (err != 0)
//     {
//         LOG_ERR("FC16 failed with %d", err);
//         return 0;
//     }

//     err = modbus_read_holding_regs(client_iface, node, 0, holding_reg,
//                                    ARRAY_SIZE(holding_reg));
//     if (err != 0)
//     {
//         LOG_ERR("FC03 failed with %d", err);
//         return 0;
//     }