#pragma once

#include "modbus_internal.h"
#include <stdint.h>
#include <string>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/modbus/modbus.h>
#include <zephyr/sys/util.h>

extern int mbc_validate_response_fc(struct modbus_context *ctx,
									const uint8_t unit_id, uint8_t fc);
extern int mbc_validate_rd_response(struct modbus_context *ctx,
									const uint8_t unit_id, uint8_t fc,
									uint8_t *data);

/*
expected setup:
PR025 = 1 (segmented speeds) // note, this will overwrite any pre-existing speed segments
PR004 = 1 (speed mode)
PR097 = 33 (fwd/reverse inhibit disabled) (or enable one if it should only turn in one direction)
*/

struct FunctionCode // implemented in Zephyr's modbus lib, listed here so that the unsupported ones on the XP200 are known
{
	static const uint8_t ReadCoils = 0x01;					  // N/A on this unit
	static const uint8_t ReadDiscreteInputs = 0x02;			  // N/A on this unit
	static const uint8_t ReadHoldingRegisters = 0x03;		  // ie. parameters or options currently set, such as Speed Mode
	static const uint8_t ReadInputRegisters = 0x04;			  // ie. status of drive and motor such as Current RPM
	static const uint8_t WriteSingleCoil = 0x05;			  // N/A on this unit
	static const uint8_t WriteSingleRegister = 0x06;		  // ie. write single parameter
	static const uint8_t DiagnosticFunction = 0x08;			  // ie. echos back whatever is sent
	static const uint8_t WriteMultipleCoils = 0x0F;			  // N/A on this unit
	static const uint8_t WriteMultipleRegisters = 0x10;		  // ie. write multiple parameters
	static const uint8_t ReadWriteMultipleRegisters = 0x17;	  // N/A on this unit
	static const uint8_t MaskWriteRegister = 0x16;			  // N/A on this unit
	static const uint8_t ReadFIFOQueue = 0x18;				  // N/A on this unit
	static const uint8_t ReadDeviceIdentification = 0x2B;	  // N/A on this unit
	static const uint8_t WriteParametersToEeprom = 0x41;	  // custom function to save params to eeprom for the XP200
	static const uint8_t WriteParametersToEepromError = 0xc1; // custom exception to save params to eeprom for the XP200
	static const uint8_t EnableDisableDrive = 0x42;			  // custom function to enable or disable the drive
	static const uint8_t EnableDisableDriveError = 0xc2;	  // custom exception to enable or disable the drive
	static const uint8_t ResetAlarm = 0x43;					  // custom function to reset alarms
	static const uint8_t ResetAlarmError = 0xc3;			  // custom exception to reset alarms

	static const uint8_t EnableDrive = 0x55;
	static const uint8_t DisableDrive = 0xAA;
};

enum class ExceptionCode : uint8_t
{
	IllegalFunction = 0x01,
	IllegalDataAddress = 0x02,
	IllegalDataValue = 0x03,
	SlaveDeviceFailure = 0x04,
	Acknowledge = 0x05,
	Unknown = 0xff,
};

enum class StatusAddress : uint16_t // 0x1000 - 0x1027 via 0x04H (aka ReadInputRegisters)
{
	MotorSpeed_r_min = 0x1000,
	OriginalPositionLow = 0x1001,
	OriginalPositionHigh = 0x1002,
	PositionInstructionLow = 0x1003,
	PositionInstructionHigh = 0x1004,
	CurrentMotorPositionLow = 0x1005,
	CurrentMotorPositionHigh = 0x1006,
	PositionDeviationLow = 0x1007,
	PositionDeviationHigh = 0x1008,
	MotorTorque_Percent = 0x1009,
	PeakTorque_Percent = 0x100A,
	InstantaneousCurrent_0_1A = 0x100B,
	PeakCurrent_0_1A = 0x100C,
	PositionInstructionPulseFrequency_0_1kHz = 0x100D,
	SpeedInstruction_r_min = 0x100E,
	TorqueInstruction_Percent = 0x100F,
	SpeedAnalogCommandVoltage_mV = 0x1010,
	TorqueAnalogCommandVoltage_mV = 0x1011,
	InputTerminalDIStatus = 0x1012,
	OutputTerminalDOStatus = 0x1013,
	MotorEncoderAbsolutePositionLow = 0x1014,
	MotorEncoderAbsolutePositionHigh = 0x1015,
	EncoderMultiTurnPosition = 0x1016,
	RegenerativeBrakingLoadRate_Percent = 0x1017,
	AverageLoadRatio_Percent = 0x1018,
	OutputVoltage_Percent = 0x1019,
	AlarmCode = 0x101A,
	MotorSpeed_0_1r_min = 0x101B,
	SecondEncoderPositionLow = 0x101C,
	SecondEncoderPositionHigh = 0x101D,
	DCBusVoltageReserved = 0x101E,
	AbsolutePositionLower = 0x101F,
	AbsolutePositionHigher = 0x1020,
	Reserved_1021 = 0x1021,
	Reserved_1022 = 0x1022,
	Reserved_1023 = 0x1023,
	Reserved_1024 = 0x1024,
	Reserved_1025 = 0x1025,
	Reserved_1026 = 0x1026,
	Reserved_1027 = 0x1027
};

enum class ParamAddress : uint16_t // 0x0000 - 0x0027 via 0x03H (aka ReadHoldingRegisters)
{
	MotorSpeed_RPM = 0x0000,
	OriginalPositionInputPulseLower = 0x0001,
	OriginalPositionInputPulseHigher = 0x0002,
	PositionInstructionPulseLower = 0x0003,
	PositionInstructionPulseHigher = 0x0004,
	CurrentMotorPositionPulseLower = 0x0005,
	CurrentMotorPositionPulseHigher = 0x0006,
	PositionDeviationPulseLower = 0x0007,

	PositionDeviationPulseHigher = 0x0008,
	MotorTorque_Percent = 0x0009,
	PeakTorque_Percent = 0x000A,
	MotorInstantaneousCurrent_0_1A = 0x000B,
	MotorPeakCurrent_0_1A = 0x000C,
	PositionInstructionPulseFrequency_0_1kHz = 0x000D,
	SpeedInstruction_RPM = 0x000E,
	TorqueInstruction_Percent = 0x000F,
	SpeedAnalogCommandVoltage_mV = 0x0010,
	TorqueAnalogCommandVoltage_mV = 0x0011,
	InputTerminalDIStatus = 0x0012,
	OutputTerminalDOStatus = 0x0013,
	MotorEncoderAbsolutePositionPulseLower = 0x0014,
	MotorEncoderAbsolutePositionPulseHigher = 0x0015,
	MotorEncoderMultiTurnPosition = 0x0016,
	RegenerativeBrakingLoadRate_Percent = 0x0017,
	AverageLoadRatio_Percent = 0x0018,
	OutputVoltage_Percent = 0x0019,
	AlarmCode = 0x001A,
	MotorSpeed_0_1RPM = 0x001B,
	SecondEncoderPositionPulseLower = 0x001C,
	SecondEncoderPositionPulseHigher = 0x001D,
	DCBusVoltageReserved = 0x001E,
	AbsolutePositionOfTheMotorLower = 0x001F,
	AbsolutePositionOfTheMotorHigher = 0x0020,
	Reserved_1 = 0x0021,
	Reserved_2 = 0x0022,
	Reserved_3 = 0x0023,
	Reserved_4 = 0x0024,
	Reserved_5 = 0x0025,
	Reserved_6 = 0x0026,
	Reserved_7 = 0x0027
};

struct WriteableParams // via 06h
{
	uint16_t address;

	constexpr WriteableParams(uint16_t addr = 0) : address(addr) {}
	// SetControlMode = 0x004,			// (PR004) Set the control mode
	// SetSpeedCommandSource = 0x0019, // (PR025)
	// Run = 0x0062,	// (PR098) force enable/disable. if enabled with a zero speed it will maintain position.
	// SetRPM = 0x0089 // (PR137), the first RPM segment in speed mode. negative for reverse, positive for forwards, 0 for stop

	static const WriteableParams INVALID; // used to indicate an error or invalid result
	static const WriteableParams Run;	  // (PR098) force enable/disable. if enabled with a zero speed it will maintain position.
	static const WriteableParams SetRPM;  // (PR137), the first RPM segment in speed mode. negative for reverse, positive for forwards, 0 for stop

	constexpr operator uint16_t() const
	{
		return address;
	}
};

// static const WriteableParams SetControlMode = {0x0004};		 // (PR004) Set the control mode
// static const WriteableParams SetSpeedCommandSource = {0x0019}; // (PR025)
static const WriteableParams INVALID = {0xFFFF}; // used to indicate an error or invalid result
static const WriteableParams Run = {0x0062};	 // (PR098) force enable/disable. if enabled with a zero speed it will maintain position.
static const WriteableParams SetRPM = {0x0089};	 // (PR137), the first RPM segment in speed mode. negative for reverse, positive for forwards, 0 for stop

inline uint16_t operator+(WriteableParams code)
{
	return static_cast<uint16_t>(code);
}

const uint16_t ERROR_VALUE = 0xFFFF; // used to indicate an error or invalid result

struct Packet
{
	Packet(uint8_t fc, uint16_t addr, uint16_t val)
		: functionCode(fc), address(addr), value(val) {}
	uint8_t functionCode;
	uint16_t address;
	uint16_t value;
};

// note this drive requires at least 3.5 characters of wait time after the end of the frame

class XP200RTU
{
public:
	/* first param is passed like DEVICE_DT_NAME(MODBUS_NODE) */
	XP200RTU(const char *anIfaceName, const uint32_t aUartSpeed, const uint32_t anRxTimeout = 500000, const uint8_t aUnitId = 0x01);
	bool RequestDriveDiagnostics();
	Packet ReadStatus(uint16_t address);			   // AKA Read Input Registers (FC04)
	bool WriteParam(uint16_t address, uint16_t value); // AKA Write single holding register (FC06)
	Packet ReadParam(uint16_t address);				   // AKA Read holding registers (FC03)
	bool EnableDrive();
	bool DisableDrive();
	bool ResetAlarm();
	bool SetSpeed(int16_t aSpeed);
	bool WriteSettingsToEeprom();
	bool IsDriveConnected();

private:
	static void ConnectLoop(void *p1, void *, void *);

	bool SendCustomFunctionByte(uint8_t functionCode, uint8_t *data);

	static int MbcSendCmd(modbus_context *ctx, const uint8_t unit_id,
						  uint8_t fc, uint8_t *data);

	struct k_thread *myConnectPollingThread;
	k_thread_stack_t *myConnectPollingThreadStack;
	bool myIsInitialized = false;
	bool myIsConnected = false;
	const float FrameEndWait = 0.6 * 3.5;
	int myClientIface;
	std::string myIfaceName;
	uint8_t myUnitId;
	const int myRxTimeout = 500000; // 500ms
	modbus_iface_param myClientParam =
		{
			.mode = MODBUS_MODE_RTU,
			// .rx_timeout = 500000, set in constructor
			.serial = {
				// .baud = 19200, set in constructor
				.parity = UART_CFG_PARITY_NONE,
				.stop_bits = UART_CFG_STOP_BITS_2,
			},
	};

	modbus_custom_fc myEnableDisableDriveFcConfig{
		// sys_snode_t node;
		// modbus_custom_cb_t cb;
		// void *user_data;
		// uint8_t fc;
		// uint8_t excep_code;
	};
};
