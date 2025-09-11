// #pragma once

// #include <cstdint>

// #include "states.hpp"

// struct HIDState
// {
// 	bool estopActive = true;
// 	uint32_t speed = 0;
// 	bool direction = true; // true = forward, false = reverse
// 	bool enabled = false;
// };

// struct DriveState
// {
// 	uint32_t speed = 0;
// 	bool direction = true; // true = forward, false = reverse
// 	bool enabled = false;
// 	bool estopActive = false;
// };

// class Machine
// {
// public:
// 	Machine();

// 	static Machine *GetInstance()
// 	{
// 		if (myInstance == nullptr)
// 		{
// 			myInstance = new Machine();
// 		}
// 		return myInstance;
// 	}

// 	HIDState *GetHIDState()
// 	{
// 		return myHIDState;
// 	}

// 	void SetSpeed(uint32_t aSpeed)
// 	{
// 		myHIDState->speed = aSpeed;
// 	}

// 	void SetDirection(bool aDirection)
// 	{
// 		myHIDState->direction = aDirection;
// 	}

// 	// start the drive (enable the drive, and set the speed to the last set speed)
// 	void Start()
// 	{
// 		// set the speed

// 		// enable the drive
// 		EnableDrive();

// 		TransitionTo(new Running());
// 	}

// 	// stop the drive (set state to stopping, set speed to 0, wait for stop, then disable)
// 	void Stop()
// 	{
// 		// set the speed to 0
// 		myHIDState->speed = 0;
// 	}

// 	// Enable the drive to run at the last set speed
// 	void EnableDrive()
// 	{
// 		myHIDState->enabled = true;
// 	}

// 	// stop the drive
// 	void DisableDrive()
// 	{
// 		myHIDState->enabled = false;
// 	}

// 	void EStop()
// 	{
// 		myHIDState->estopActive = true;
// 		DisableDrive();
// 	}

// 	void ResetDrive()
// 	{
// 		myHIDState->estopActive = false;
// 	}

// 	void TransitionTo(State *aState)
// 	{
// 		if (myState != nullptr)
// 		{
// 			myState->OnExit();
// 		}
// 		myState = aState;
// 		myState->OnEnter();
// 	}

// 	void Update()
// 	{
// 		if (myState != nullptr)
// 		{
// 			myState->OnUpdate();
// 		}
// 	}

// 	uint32_t GetCurrentSpeed()
// 	{
// 		return myDriveState->speed;
// 	}

// private:
// 	static Machine *myInstance;
// 	HIDState *myHIDState;
// 	DriveState *myDriveState;
// 	State *myState = nullptr;
// };