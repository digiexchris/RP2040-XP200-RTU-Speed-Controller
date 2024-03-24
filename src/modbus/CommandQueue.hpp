#pragma once

#include <zephyr/zephyr.h>
#include <zephyr/sys/printk.h>
#include <unordered_map>

// note this drive requires at least 3.5 characters of wait time after the end of the frame




// std::unordered_map<Action, Command> coilCommands = {
//     //{Action::EncSwitch, {0x06, 0x0000, 0x0001}},
//     // {Action::RunF, {0x06, 0x0001, 0x0001}},
//     // {Action::RunR, {0x06, 0x0002, 0x0001}},
//     // {Action::Stop, {0x06, 0x0003, 0x0001}},
//     // {Action::JogF, {0x06, 0x0005, 0x0001}},
//     // {Action::JogR, {0x06, 0x0006, 0x0001}},
//     // {Action::EStop, {0x06, 0x0007, 0x0001}},
//     {Action::Enable, {0x01, 0x42, 0x55}},     // func 0xC2 is an error, with a value 0x01 or 02 or 03 or 04
//     {Action::Disable, {0x01, 0x42, 0xAA}},    // func 0xC2 is an error, with a value 0x01 or 02 or 03 or 04
//     {Action::ACLR, {0x01, 0x43, 0x55}},       // func 0xC3 is an error
//     {Action::SaveParams, {0x01, 0x41, 0x00}}, // func oxC1 is an error
//     //{Action::Diag, {0x01, 0x08, 0x1234}},     // (will respond with 0x1234 or whatever value sent)
//     //{Action::SetSpeed, {0x01, 0x06, 0x0001}
}
;

/* Specify the maximum size of the queue */
#define COMMAND_QUEUE_SIZE 10

/* Create a message queue for RTU Modbus commands */
K_MSGQ_DEFINE(CommandQueue, sizeof(std::pair<Action, Command> *), COMMAND_QUEUE_SIZE, 4);

/* Example usage of the CommandQueue */
void enqueueCommand(std::pair<Action, Command> *cmd)
{
    if (cmd->first == Action::EStop)
    {
        printk("ESTOP!!! Purging queue and stopping.\n");
        k_msgq_purge(&CommandQueue);
        k_msgq_put(&CommandQueue, cmd, K_NO_WAIT);
        return;
    }
    if (k_msgq_put(&CommandQueue, cmd, K_NO_WAIT) != 0)
    {
        printk("CommandQueue full. Setting a drive alert.\n");
        // todo set drive alert and stop
    }
}

void processQueue()
{
    std::pair<Action, Command> *cmd;

    while (true)
    {
        k_msgq_get(&CommandQueue, cmd, K_FOREVER);

        modbus_write_coil(client_iface, node, cmd->second.registerAddress, cmd->second.registerValue);
    }
}