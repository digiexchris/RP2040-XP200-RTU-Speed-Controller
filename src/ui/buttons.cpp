#include "buttons.hpp"

Buttons *Buttons::myInstance = nullptr;

LOG_MODULE_REGISTER(Buttons, LOG_LEVEL_INF);

void Buttons::Receive(void *sub)
{
	const struct zbus_channel *chan;

	ButtonStateMsg acc;

	const struct zbus_observer *subscriber = static_cast<const struct zbus_observer *>(sub);

	while (!zbus_sub_wait(subscriber, &chan, K_FOREVER))
	{
		if (&BUTTONS != chan)
		{
			LOG_ERR("Wrong channel %p!", chan);

			continue;
		}
		zbus_chan_read(chan, &acc, K_MSEC(250));

		LOG_INF("Received button state: %d %d", static_cast<int>(acc.state), acc.isSet);

		Buttons::GetInstance()->OnMessage(&acc);
	}
}

void Buttons::OnMessage(ButtonStateMsg *msg)
{
	if (msg->isSet)
	{
		myState |= static_cast<int8_t>(msg->state);
	}
	else
	{
		myState &= ~static_cast<int8_t>(msg->state);
	}

	// decide what to do, and emit more messages to channels
	// otherwise this is the source of truth for the state of all of the buttons
}

void Buttons::SyncState()
{
	// for safety, set e-stop first since we're going to send a bunch of button messages

	ButtonStateMsg eStopMsg;
	eStopMsg.state = ButtonBit::EStop;
	eStopMsg.isSet = true;
	zbus_chan_pub(&BUTTONS, &eStopMsg, K_NO_WAIT);

	for (uint8_t i = 1; i <= numberOfButtons; i++)
	{
		if (i == static_cast<uint8_t>(ButtonBit::EStop))
			continue; // we'll do this at the end to ensure we're safe to move

		ButtonBit bit = static_cast<ButtonBit>(i);
		myButtons[bit]->Publish();
	}

	// check and send the actual state of the estop switch
	myButtons[ButtonBit::EStop]->Publish();
}

ZBUS_SUBSCRIBER_DEFINE(BUTTON_STATE_SUB, 4);

K_THREAD_DEFINE(ButtonSubscriberThread, CONFIG_MAIN_STACK_SIZE, Buttons::Receive, &BUTTON_STATE_SUB, NULL,
				NULL, 2, 0, 0);