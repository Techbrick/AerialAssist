#include "WPILib.h"
#include "PinDefinitions.h"
#include "Pneumatic.cpp"
#include "Potentiometer.cpp"

inline float fabs (float a)
{
	return (a<0 ? -a : a);	
}

inline float sign (float a)
{
	return (a<0 ? -1.0 : 1.0);
}

inline void primeTaskFunc(UINT32 motorLockPistonPtr, UINT32 ratchetPistonPtr, UINT32 winchPtr, UINT32 winchLimitSwitchPtr, UINT32 joystickCancelPtr, UINT32 joystickButtonInt, ...)
{
	Pneumatic *motorLockPiston = (Pneumatic *) motorLockPistonPtr;
	Pneumatic *ratchetPiston = (Pneumatic *) ratchetPistonPtr;
	Relay *winch = (Relay *) winchPtr;
	DigitalInput *winchLimitSwitch = (DigitalInput *) winchLimitSwitchPtr;
	Joystick *joystickCancel = (Joystick *) joystickCancelPtr;


	ratchetPiston->Set(true);	// This order is important
	motorLockPiston->Set(true);

	winch->Set(Relay::kReverse);

	//while the winch hasn't hit the limit.
	while ( winchLimitSwitch->Get() == false && !joystickCancel->GetRawButton(joystickButtonInt))
	{

	}

	winch->Set(Relay::kOff);
	motorLockPiston->Set(false);
}


class Shooter {
	Relay winch;
	DigitalInput winchLimitSwitch;

	Pneumatic motorLockPiston;	// Engages motor
	Pneumatic ratchetPiston;	// Engages ratchet

	Talon arm;
	DigitalInput armBackLimitSwitch;
	DigitalInput armFrontLimitSwitch;
	Potentiometer armPot;

	Task primeTask;
	
public:
	Shooter () :
		winch (SHOOTER_WINCHSPIKE),
		winchLimitSwitch (SHOOTER_WINCHLIMITSWITCH),
		motorLockPiston (SHOOTER_WINCHMOTORSOLIN, SHOOTER_WINCHMOTORSOLOUT),
		ratchetPiston (SHOOTER_WINCHRATCHSOLIN, SHOOTER_WINCHRATCHSOLOUT),
		arm (SHOOTER_ARMTALON),
		armBackLimitSwitch (SHOOTER_ARMLIMITSWITCHBACK),
		armFrontLimitSwitch (SHOOTER_ARMLIMITSWITCHFRONT),
		armPot (SHOOTER_ARMPOT),
		primeTask ("Prime", (FUNCPTR) primeTaskFunc)
	{    }

	void Prime(Joystick *joystickCancel, UINT32 joystickCancelButton)
	{
		primeTask.Start((UINT32) &motorLockPiston, (UINT32) &ratchetPiston, (UINT32) &winch, (UINT32) &winchLimitSwitch, (UINT32) &joystickCancel, joystickCancelButton);
	}

	bool Cancel()
	{
		return primeTask.Stop();
	}

	void Fire()
	{
		while (primeTask.GetID() + 1)
		{

		}

		ratchetPiston.Set(false);	// fire.
		Wait(0.1);
	}

	float GetAngle()
	{
		return armPot.Get()*160.0 + 20.0;
	}

	void MoveArm(float power)
	{		
		if ((armBackLimitSwitch.Get() && power<0.0) || (armFrontLimitSwitch.Get() && power>0.0))
		{
			arm.Set(0);
		}
		else
		{
			arm.Set(power);
		}
	}

	void Calibrate()
	{
		arm.Set(0.8);	//TODO: make sure this is the right direction!

		while (!armFrontLimitSwitch.Get())
		{    }

		arm.Set(0.0);
		armPot.CalibrateStart();

		arm.Set(-0.8);	//TODO: make sure this is the right direction!

		while (!armBackLimitSwitch.Get())
		{    }

		arm.Set(0.0);
		armPot.CalibrateEnd();
	}
};
