#include "WPILib.h"
#include "PinDefinitions.h"
#include "Pneumatic.cpp"
#include "Potentiometer.cpp"

float fabs (float a)
{
	return (a<0 ? -a : a);	
}

class Shooter {
	Relay winch;
	Encoder winchDigEncoder;

	Pneumatic motorLockPiston;	// Engages motor
	Pneumatic ratchetPiston;	// Engages ratchet

	Talon arm;
	DigitalInput armBackLimitSwitch;
	DigitalInput armFrontLimitSwitch;
	Potentiometer armPot;

	Task primeTask;

	void primeTaskFunc();

public:
	Shooter () :
		winch (SHOOTER_WINCH_SPIKE),
		winchDigEncoder (SHOOTER_WINCHDIGITALENCA, SHOOTER_WINCHDIGITALENCB),
		motorLockPiston (SHOOTER_WINCHMOTORSOLIN, SHOOTER_WINCHMOTORSOLOUT),
		ratchetPiston (SHOOTER_WINCHRATCHSOLIN, SHOOTER_WINCHRATCHSOLOUT),
		arm (SHOOTER_ARMTALON),
		armBackLimitSwitch (SHOOTER_ARMLIMITSWITCHBACK),
		armFrontLimitSwitch (SHOOTER_ARMLIMITSWITCHFRONT),
		armPot (SHOOTER_ARMPOT),
		primeTask ("Prime", (FUNCPTR) Robot::primeTaskFunc)
	{
		winchDigEncoder.Start();
		winchDigEncoder.Reset();
	}

	void prime()
	{
		primeTask.Start();
	}

	bool cancel()
	{
		primeTask.Stop();
	}

	void fire()
	{
		if (primeTask.GetID())
			SmartDashboard::PutString("Errors", "Shooter::fire(): Shooter::prime() still running");
		else
		{
			//motorLockPiston=in, ratchetPiston=out, winch=off
			//state: ready
			if (!motorLockPiston.Get() && ratchetPiston.Get() && !winch.Get())
			{
				ratchetPiston.Set(false);	// fire.
				Wait(0.1);
				winchDigEncoder.Reset();
			}
			else
			{
				//other
				///state: probably very broken.
				SmartDashboard::PutString("Errors", "Shooter::fire(): Unexpected state, potential danger!");
			}
		}
	}

	float getAngle()
	{
		return armPot.Get()*160.0 + 20.0;
	}

	float setAngle(float angle)
	{
		while ( fabs((armPot.Get()*160.0 + 20.0) - angle) > 0.5)
		{    }
	}

	void calibrate()
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

	float getWinchDistance()
	{
		return winchDigEncoder.Get(); //TODO: convert this to actual distance!
	}
};



void primeTaskFunc()
{
	//pn1=in, pn2=in, winch=off
	//state: fired
	if (!motorLockPiston.Get() && !ratchetPiston.Get() && !winch.Get())
	{
		ratchetPiston.Set(true);	// This order is important
		motorLockPiston.Set(true);


		winch.Set(Relay.kOn);
		winchDigEncoder.Reset();

		//while the winch has spun less than 10 revs. This is temporary.
		//TODO: replace this with something useful.
		while ( (winchDigEncoder.Get()/255.0)*360.0) < 360.0*10.0 )
		{   }

		winch.Set(Relay.kOff);
		motorLockPiston.Set(false);
	}
	else
	{
		//motorLockPiston=in, ratchetPiston=out, winch=off
		//state: ready
		if (!motorLockPiston.Get() && ratchetPiston.Get() && !winch.Get())
		{
			//Do nothing.
		}
		else
		{
			//other
			//state: either winching, transitioning, or broken!
			SmartDashboard::PutString("Errors","Shooter::prime(): Shooter in unexpected state");
		}
	}
}
