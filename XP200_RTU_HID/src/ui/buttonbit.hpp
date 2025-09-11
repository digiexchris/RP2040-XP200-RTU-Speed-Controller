#pragma once
#include <cstdint>

enum class ButtonBit
{
	None = 0x00,
	EncSwitch = 0x01,
	RunF = 0x02,
	RunR = 0x03,
	ACLR = 0x04, // alarm clear
	JogF = 0x05,
	JogR = 0x06,
	EStop = 0x07,
	NUM_BUTTONS = 0x08
};

// Enable bitmask operations for Stuff
inline ButtonBit operator|(ButtonBit a, ButtonBit b)
{
	return static_cast<ButtonBit>(static_cast<int8_t>(a) | static_cast<int8_t>(b));
}

inline ButtonBit operator&(ButtonBit a, ButtonBit b)
{
	return static_cast<ButtonBit>(static_cast<int8_t>(a) & static_cast<int8_t>(b));
}

struct ButtonStateMsg
{
	ButtonBit state = ButtonBit::None;
	bool isSet = false;
};