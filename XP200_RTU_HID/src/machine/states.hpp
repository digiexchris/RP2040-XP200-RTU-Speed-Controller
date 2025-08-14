#pragma once

#include "basestates.hpp"

class Init : public State
{
public:
	virtual bool OnEnter();

	virtual bool IsValidActionForState(Action anAction) override;
};

class Running : public State
{
	virtual bool OnEnter() override;

private:
	const Action myValidActions = Action::SetSpeed | Action::RunF | Action::RunR | Action::Stop | Action::EStop;
};

class Stopping : public State
{
public:
	virtual bool OnEnter() override;

	virtual bool OnUpdate() override;

private:
	const Action myValidActions = Action::SetSpeed | Action::RunF | Action::RunR | Action::Stop | Action::EStop;
};

class Stopped : public State
{
public:
	virtual bool OnEnter() override;

private:
	const Action myValidActions = Action::SetSpeed | Action::RunF | Action::RunR | Action::Stop | Action::EStop;
};

class EStop : public State
{
public:
	virtual bool OnEnter() override;

private:
	const Action myValidActions = Action::ACLR;
};