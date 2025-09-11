#include "states.hpp"

#include "machine.hpp"

bool Init::OnEnter()
{
    Machine *myMachine = Machine::GetInstance();
    myMachine->DisableDrive();
    // make sure SON is OFF!!
    // read speed from eeprom and direction L/R switch state
    // restore vfd speed and direction
    myMachine->SetSpeed(0.5);
    myMachine->SetDirection(true);
    myMachine->TransitionTo(new Stopped());
}

bool Init::IsValidActionForState(Action anAction)
{
    return false; // we don't accept any actions until after we've restored the saved state
}

bool Running::OnEnter()
{
    Machine *myMachine = Machine::GetInstance();
    myMachine->EnableDrive();
    myMachine->SetSpeed(myMachine->GetHIDState()->speed);
    return true;
}

bool Stopped::OnEnter()
{
    Machine::GetInstance()->DisableDrive();
    return true;
}

bool Stopping::OnEnter()
{
    Machine::GetInstance()->SetSpeed(0);
    return true;
}

bool Stopping::OnUpdate()
{
    Machine *myMachine = Machine::GetInstance();
    if (myMachine->GetCurrentSpeed() == 0)
    {
        Machine::GetInstance()->TransitionTo(new Stopped());
        return true;
    }
    return true;
}

bool EStop::OnEnter()
{
    Machine *myMachine = Machine::GetInstance();
    // disable drive first, in case this is in response to an electricution!
    myMachine->DisableDrive();
    myMachine->SetSpeed(0);
    return true;
}