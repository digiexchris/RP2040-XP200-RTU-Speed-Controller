// #pragma once

// #include "machine.hpp"
// // #include "actions.hpp"

// class Machine; // forward declaration

// class State
// {

// public:
// 	State() = default;
// 	virtual bool OnEnter() { return true; };
// 	virtual bool OnExit() { return true; };
// 	virtual bool OnUpdate() { return true; };
// 	// virtual bool OnAction(Action anAction)
// 	// {
// 	// 	if (IsValidActionForState(anAction))
// 	// 	{
// 	// 		return Execute(anAction);
// 	// 	}
// 	// 	return false;
// 	// };
// 	// virtual bool IsValidActionForState(Action anAction)
// 	// {
// 	// 	return (myValidActions & anAction) == anAction;
// 	// }

// protected:
// 	// state specific handling of actions
// 	// virtual bool Execute(Action anAction);

// 	// Action myValidActions = Action::None;
// };