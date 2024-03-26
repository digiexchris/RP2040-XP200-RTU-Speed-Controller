#pragma once

#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/modbus/modbus.h>

#include <zephyr/logging/log.h>

// enum class FunctionCode : uint8_t //implemented in Zephyr's modbus lib, listed here so that the unsupported ones on the XP200 are known
// {
// 	ReadCoils = 0x01,		   // N/A on this unit
// 	ReadDiscreteInputs = 0x02,	   // N/A on this unit
// 	ReadHoldingRegisters = 0x03,	   // ie. parameters or options currently set, such as Speed Mode
// 	ReadInputRegisters = 0x04,	   // ie. status of drive and motor such as Current RPM
// 	WriteSingleCoil = 0x05,		   // N/A on this unit
// 	WriteSingleRegister = 0x06,	   // ie. write single parameter
// 	DiagnosticFunction = 0x08,	   // ie. echos back whatever is sent
// 	WriteMultipleCoils = 0x0F,	   // N/A on this unit
// 	WriteMultipleRegisters = 0x10,	   // ie. write multiple parameters
// 	ReadWriteMultipleRegisters = 0x17, // N/A on this unit
// 	MaskWriteRegister = 0x16,	   // N/A on this unit
// 	ReadFIFOQueue = 0x18,		   // N/A on this unit
// 	ReadDeviceIdentification = 0x2B,   // N/A on this unit
// 	WriteParametersToEeprom = 0x41	   // N/A on this unit
// };
// }

enum class StatusAddress : uint16_t // 0x1000 - 0x1027 via 0x04H
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

enum class ParamAddress : uint16_t // 0x0000 - 0x0027 via 0x03H
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

enum class WriteableParams //via 06h
{
	SetSpeedCommandSource = 0x0019,
	Run = 0x0062,
	SetRPM = 0x0089

}

struct Packet
{
	uint8_t functionCode;
	uint16_t address;
	uint16_t value;
};

// note this drive requires at least 3.5 characters of wait time after the end of the frame

class Modbus
{
public:
	/* first param is passed like DEVICE_DT_NAME(MODBUS_NODE) */
	Modbus(const char *anIfaceName, const uint8_t aUnitId = 0x01);
	bool IsDriveConnected();
	Packet ReadStatus(uint16_t address);		     // AKA Read Input Registers (FC04)
	Packet WriteParam(uint16_t address, uint16_t value); // AKA Write single holding register (FC06)
	Packet ReadParam(uint16_t address);		     // AKA Read holding registers (FC03)
	bool EnableDrive();
	bool DisableDrive();
	bool SetSpeedDirection(int16_t aSpeed, bool aDirection = true)
	
private:
	struct k_timer myFrameEndTimer;
	void UnlockEndOfFrame(struct k_timer *timer_id) {
		if(endOfFrame)
		{
			unlock(endOfFrame);
		}
	};


	const float FrameEndWait = 0.6*3.5;
	std::mutex endOfFrame;
	int myClientIface;
	const char *myIfaceName;
	const uint8_t myUnitId;
	const static struct modbus_iface_param myClientParam =
	    {
		.mode = MODBUS_MODE_RTU,
		.rx_timeout = 50000,
		.serial = {
		    .baud = 19200,
		    .parity = UART_CFG_PARITY_NONE,
		    .stop_bits_client = UART_CFG_STOP_BITS_2,
		},
	};
};
