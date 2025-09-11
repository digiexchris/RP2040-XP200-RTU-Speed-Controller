#include "machine.hpp"

Machine *Machine::myInstance = nullptr;

Machine::Machine()
{
    myHIDState = new HIDState();
    myDriveState = new DriveState();
}