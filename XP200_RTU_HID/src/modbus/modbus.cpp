#include "modbus.hpp"
#include "modbus_internal.h"
#include "zephyr/kernel.h"
#include "zephyr/modbus/modbus.h"

LOG_MODULE_REGISTER(modbus_app, LOG_LEVEL_DBG);

#define MODBUS_NODE DT_CHOSEN(zephyr_modbus_serial)
#define MODBUS_UART_NODE DT_PARENT(MODBUS_NODE)
#define MODBUS_UART_SPEED DT_PROP(MODBUS_UART_NODE, current_speed)

// K_THREAD_STACK_DEFINE(modbusConnectPollingLoopStack, 4096);

XP200RTU::XP200RTU(const char *anIfaceName, const uint32_t aUartSpeed, const uint32_t anRxTimeout, const uint8_t aUnitId) : myIfaceName(anIfaceName), myUnitId(aUnitId)

{
	myClientParam.rx_timeout = anRxTimeout;
	myClientParam.serial.baud = aUartSpeed;

	myClientIface = modbus_iface_get_by_name(myIfaceName.c_str());

	__ASSERT(myClientIface >= 0, "Invalid Modbus interface");

	myConnectPollingThreadStack = k_thread_stack_alloc(2048, 0); // Use 0 instead of K_USER when not using userspace
	__ASSERT(myConnectPollingThreadStack != nullptr, "Failed to allocate thread stack");

	myConnectPollingThread = new k_thread();
	__ASSERT(myConnectPollingThread != nullptr, "Failed to allocate thread");

	k_thread_create(myConnectPollingThread, myConnectPollingThreadStack,
					2048,
					ConnectLoop,
					this, NULL, NULL,
					5, K_ESSENTIAL, K_NO_WAIT);
}

void XP200RTU::ConnectLoop(void *p1, void *, void *)
{

	XP200RTU *instance = static_cast<XP200RTU *>(p1);

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

		k_sleep(Z_TIMEOUT_MS(2500));
	}
}

bool XP200RTU::IsDriveConnected()
{
	return myIsConnected;
}

bool XP200RTU::RequestDriveDiagnostics()
{
	uint16_t result;
	int err = modbus_request_diagnostic(myClientIface, myUnitId, 0x08, 0x1234, &result);

	if (err != 0)
	{
		return false;
	}

	if (result != 0x1234)
	{
		return false;
	}

	return true;
}

// note: negative is reverse, positive is forwards, 0 is stop
bool XP200RTU::SetSpeed(int16_t aSpeed)
{

	if (!WriteParam(WriteableParams::SetRPM, aSpeed))
	{
		return false;
	}

	return true;
}

Packet XP200RTU::ReadStatus(uint16_t address)
{
	uint16_t regs[1] = {0};

	uint16_t result = modbus_read_input_regs(myClientIface, myUnitId, address, regs, 1);
	if (result != 0)
	{
		LOG_ERR("ReadInputRegisters failed with %d", result);
		return Packet(FunctionCode::ReadInputRegisters, address, ERROR_VALUE);
	}

	return Packet(FunctionCode::ReadInputRegisters, address, regs[0]);
}

bool XP200RTU::WriteParam(uint16_t address, uint16_t value)
{
	uint16_t result = modbus_write_holding_reg(myClientIface, myUnitId, address, value);
	if (result != 0)
	{
		LOG_ERR("WriteSingleRegister failed with %d", result);
		return false;
	}

	return true;
};

Packet XP200RTU::ReadParam(uint16_t address)
{
	uint16_t regs[1] = {0};

	int result = modbus_read_holding_regs(myClientIface, myUnitId, address, regs, 1);
	if (result != 0)
	{
		LOG_ERR("ReadHoldingRegisters failed with %d", result);
		return Packet{FunctionCode::ReadHoldingRegisters, address, ERROR_VALUE};
	}

	return Packet{FunctionCode::ReadInputRegisters, address, regs[0]};
}

int XP200RTU::MbcSendCmd(modbus_context *ctx, const uint8_t unit_id,
						 uint8_t fc, uint8_t *data)
{
	int err;

	ctx->tx_adu.unit_id = unit_id;
	ctx->tx_adu.fc = fc;
	ctx->tx_adu.data[0] = *data;
	ctx->tx_adu.length = 1;

	err = modbus_tx_wait_rx_adu(ctx);
	if (err != 0)
	{
		return err;
	}

	err = mbc_validate_response_fc(ctx, unit_id, fc);
	if (err < 0)
	{
		LOG_ERR("Failed to validate unit ID or function code");
		return err;
	}
	else if (err > 0)
	{
		LOG_INF("Modbus FC %u, error code %u", fc, err);
		return err;
	}

	err = mbc_validate_rd_response(ctx, unit_id, fc, data);

	return err;
}

bool XP200RTU::SendCustomFunctionByte(uint8_t functionCode, uint8_t *data)
{
	struct modbus_context *ctx = modbus_get_context(myClientIface);
	int err;

	if (ctx == NULL)
	{
		return -ENODEV;
	}

	k_mutex_lock(&ctx->iface_lock, K_FOREVER);

	err = MbcSendCmd(ctx, myUnitId, functionCode, data);
	k_mutex_unlock(&ctx->iface_lock);
	if (err == FunctionCode::EnableDisableDriveError || err != 0)
	{
		return false;
	}

	return true;
}

bool XP200RTU::EnableDrive()
{
	uint8_t data = FunctionCode::EnableDrive;
	return SendCustomFunctionByte(FunctionCode::EnableDisableDrive, &data);
}

bool XP200RTU::DisableDrive()
{
	uint8_t data = FunctionCode::DisableDrive;
	return SendCustomFunctionByte(FunctionCode::EnableDisableDrive, &data);
}