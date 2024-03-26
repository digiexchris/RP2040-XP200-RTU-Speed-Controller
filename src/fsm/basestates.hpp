#pragma once

class State {

    public:
        State(Machine *machine): myMachine(machine) {};
        virtual bool OnEnter() {};
        virtual bool IsValidActionForState(Action anAction) = 0;
    protected:
        Machine* myMachine;
}