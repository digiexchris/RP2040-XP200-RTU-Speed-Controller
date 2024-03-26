#pragma once;

enum class Action : int8_t // actions that the UI can accept and process
{
	EncSwitch = 0x01,
	SetSpeedDirection = 0x02,
	RunF = 0x03,
	RunR = 0x04,
	Stop = 0x05,
	Enable = 0x06,
	Disable = 0x07,
	ACLR = 0x08, // alarm clear
	JogF = 0x09,
	JogR = 0x10,
	EStop = 0x11
};


// Enable bitmask operations for Stuff
inline Action operator|(Action a, Action b) {
    return static_cast<Action>(static_cast<int8_t>(a) | static_cast<int8_t>(b));
}

inline Action operator&(Action a, Action b) {
    return static_cast<Action>(static_cast<int8_t>(a) & static_cast<int8_t>(b));
}

// Check if `all` contains `check`
bool contains(Action all, Action check) {
    return (all & check) == check;
}