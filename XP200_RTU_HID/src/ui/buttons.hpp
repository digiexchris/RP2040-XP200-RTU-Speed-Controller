#pragma once

#include <cstdint>
#include <unordered_map>
#include <zephyr/logging/log.h>

#include "button.hpp"
#include "buttonbit.hpp"

using ButtonList = std::unordered_map<ButtonBit, class Button *>;

extern struct zbus_channel BUTTONS;

class Buttons
{
public:
	Buttons() : myState(0x00)
	{
		myInstance = this;

		Button *encSwitch = new Button(GPIO_DT_SPEC_GET_OR(EncSwitch, gpios, {0}), ButtonBit::EncSwitch);
		myButtons[ButtonBit::EncSwitch] = encSwitch;

		Button *runF = new Button(GPIO_DT_SPEC_GET_OR(RunF, gpios, {0}), ButtonBit::RunF);
		myButtons[ButtonBit::RunF] = runF;

		Button *runR = new Button(GPIO_DT_SPEC_GET_OR(RunR, gpios, {0}), ButtonBit::RunR);
		myButtons[ButtonBit::RunR] = runR;

		Button *aclr = new Button(GPIO_DT_SPEC_GET_OR(ACLR, gpios, {0}), ButtonBit::ACLR);
		myButtons[ButtonBit::ACLR] = aclr;

		Button *jogF = new Button(GPIO_DT_SPEC_GET_OR(JogF, gpios, {0}), ButtonBit::JogF);
		myButtons[ButtonBit::JogF] = jogF;

		Button *jogR = new Button(GPIO_DT_SPEC_GET_OR(JogR, gpios, {0}), ButtonBit::JogR);
		myButtons[ButtonBit::JogR] = jogR;

		Button *eStop = new Button(GPIO_DT_SPEC_GET_OR(EStop, gpios, {0}), ButtonBit::EStop);
		myButtons[ButtonBit::EStop] = eStop;

		SyncState();
	}

	static void Receive(void *sub);

	static Buttons *GetInstance()
	{
		if (myInstance == nullptr)
		{
			myInstance = new Buttons();
		}
		return myInstance;
	}

private:
	void OnMessage(ButtonStateMsg *msg);

	void SyncState();

	int8_t myState;
	static Buttons *myInstance;

	ButtonList myButtons;

	constexpr static const uint8_t numberOfButtons = static_cast<uint8_t>(ButtonBit::NUM_BUTTONS) - 1;
};
