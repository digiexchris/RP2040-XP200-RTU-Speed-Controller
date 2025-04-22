#include "modbus.hpp"

Modbus::Modbus(const char *anIfaceName, const uint8_t aUnitId) : myIfaceName(anIfaceName), myUnitId(aUnitId)
{
    myClientIface = modbus_iface_get_by_name(myIfaceName);
    if (modbus_init_client(myClientIface, myClientParam))
    {
        LOG_ERR("Modbus RTU client initialization failed");
        // todo fire off an event to show it on the LCD if there is one. At least blink an LED
        return;
    }

    k_timer_init(&myFrameEndTimer, UnlockEndOfFrame, NULL);
}

bool Modbus::IsDriveConnected()
{
    uint16_t result = modbus_request_diagnostic(myClientIface, myUnitId, 0x08, 0x1234);

    if (result != 0x1234)
    {
        // TODO handle error
        return false;
    }

    return true;
}


bool Modbus::Start()
{
	WriteParam(WriteParams::Run, 1);
}

bool Modbus::Stop()
{
	WriteParam(WriteParams::Run, 0);
}

bool Modbus::SetSpeedDirection(int16_t aSpeed, bool aDirection = true)
{
	if(aSpeed > 0 && !aDirection)
	{
		aSpeed = 0 - aSpeed;
	}

	WriteParam(WriteParams::SetSpeed, aSpeed)
	{

	}
}

Packet Modbus::ReadStatus(uint16_t address)
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

Packet Modbus::WriteParam(uint16_t address, uint16_t value)
{
    uint16_t result = modbus_write_holding_reg(myClientIface, myUnitId, address, value);
    if (result != 0)
    {
        LOG_ERR("FC06 failed with %d", err);
        return result;
    }

    return result;
};

Packet Modbus::ReadParam(uint16_t address)
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

Packet Modbus::EnableDrive()
{
    return modbus_raw_submit_rx(FunctionCode::WriteSingleCoil, 0x55);
}

Packet Modbus::DisableDrive()
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