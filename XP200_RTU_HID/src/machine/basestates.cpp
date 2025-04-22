#include "basestates.hpp"
#include "machine.hpp"

bool State::Execute(Action anAction)
{
    Machine *myMachine = Machine::GetInstance();
    switch (anAction)
    {
    case Action::SetSpeed:
        myMachine->SetSpeed(myMachine->GetHIDState()->speed);
        break;
    case Action::RunF:
        myMachine->SetDirection(true);
        myMachine->TransitionTo(new Running());
        break;
    case Action::RunR:
        myMachine->SetDirection(false);
        myMachine->TransitionTo(new Running());
        break;
    case Action::Stop:
        myMachine->TransitionTo(new Stopping());
        break;
    case Action::EStop:
        myMachine->TransitionTo(new EStop());
        break;
    case Action::ACLR:
        myMachine->ResetError();
        myMachine->TransitionTo(new Stopped());
        break;
    default:
        return false;
    }
    return true;
}