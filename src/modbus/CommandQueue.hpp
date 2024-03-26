#pragma once

#include <zephyr/zephyr.h>
#include <zephyr/sys/printk.h>
#include <unordered_map>

class Command
{

};

class Start : public Command {};

class Stop : public Command {};

class SetSpeedDirection : public Command 
{
    public:

        SetSpeed(int16_t aSpeed, bool aDirection = true) : speed(aSpeed), direction(aDirection) {};
        int16_t speed = 0;
        bool direction = true;
};

enum class CommandQueueResult
{
    Ack,
    Full
}

/* Specify the maximum size of the queue */
#define COMMAND_QUEUE_SIZE 10

/* Create a message queue for RTU Modbus commands */
K_MSGQ_DEFINE(CommandQueue, sizeof(std::pair<Action, *Command> *), COMMAND_QUEUE_SIZE, 4);

/* Example usage of the CommandQueue */
CommandQueueResult enqueueCommand(std::pair<Action, *Command> *cmd)
{
    if (cmd->first == Action::EStop)
    {
        printk("ESTOP!!! Purging queue and stopping.\n");
        k_msgq_purge(&CommandQueue);
        k_msgq_put(&CommandQueue, cmd, K_NO_WAIT);
        return CommandQueueResult::Ack;
    }
    if (k_msgq_put(&CommandQueue, cmd, K_NO_WAIT) == 0)
    {
        return CommandQueueResult::Ack;
    }
    else
    {
        printk("CommandQueue full. Setting a drive alert.\n");
        // todo set drive alert and stop
        return return CommandQueueResult::Full;
    }   
}

void processQueue()
{
    std::pair<Action, *Command> *cmd;

    while (true)
    {
        k_msgq_get(&CommandQueue, cmd, K_FOREVER);

        switch(cmd->first)
        {
            case Action::SetSpeedDirection:
                SetSpeedDirection* payload = static_cast<SetSpeedDirection*>(cmd->second);
                myModbus.SetSpeedDirection(payload.speed, payload.direction);
                break;
            case Action::Start:
                myModbus.Start();
                break;
            case Action::Stop:
                myModbus.Stop();
                break;
            case Action::EStop:
                myDigitalOut.EStop();
                myModbus.Stop();
                myModbus.SetSpeedDirection(0, false);
                break;
            case Action::ACLR:
                myDigitalOut.ACLR();
                break;
            default:
                break;
        }
    }
}