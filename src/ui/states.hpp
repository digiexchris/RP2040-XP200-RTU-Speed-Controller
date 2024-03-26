#include "fsm/basestates.hpp"

class Init : public State {
    public:
        virtual bool OnEnter()
        {
            myMachine->DisableDrive();
            //make sure SON is OFF!!
            //read speed from eeprom and direction L/R switch state
            //restore vfd speed and direction
            myMachine->SetSpeedDirection(loadedSpeed, loadedDirection)
            //Transition to Ready
        }
        virtual bool IsValidActionForState(Action anAction)
        {
            return false; //we don't accept any actions until after we've restored the saved state
        }


}

class Ready : public State {
    public:
        virtual bool IsValidActionForState(Action anAction)
        {
            return myValidActions.contains(anAction);
        }
    private:
        const Action myValidActions = Action::SetSpeedDirection | Action::Start | Action::Stop | Action::EStop

}

class Menu : public State {
    public:
        virtual bool IsValidActionForState(Action anAction)
        {
            return myValidActions.contains(anAction);
        }
    private:
        const Action myValidActions = Action::SetJogSpeed | Action::ExitMenu | Action::EStop

}

class EStop : public State {
    public:
        virtual bool IsValidActionForState(Action anAction)
        {
            eturn myValidActions.contains(anAction);
        }
        virtual bool OnEnter() override
        {
            myMachine->EStop();
        }
    private:
        const Action myValidActions = Action::ACLR;

}