#include "WPILib.h"
#include "PinDefinitions.h"
#include "Pneumatic.cpp"
#include <tgmath.h>

class Shooter {
	Relay winch;
	Encoder winchDigEncoder;

	Pneumatic motorPn;
	Pneumatic ratchPn;

	Talon arm;
	DigitalInput armBackLimitSwitch;
	DigitalInput armFrontLimitSwitch;
	Potentiometer armPotent;

	Task primeTask;

	void primeTaskFunc();

public:
	Shooter () :
		winch (SHOOTER_WINCH_SPIKE),
		winchDigEncoder (SHOOTER_WINCHDIGITALENCA, SHOOTER_WINCHDIGITALENCB),
		motorPn (SHOOTER_WINCHMOTORSOLIN, SHOOTER_WINCHMOTORSOLOUT),
		ratchPn (SHOOTER_WINCHRATCHSOLIN, SHOOTER_WINCHRATCHSOLOUT),
		arm (SHOOTER_ARMTALON),
		armBackLimitSwitch (SHOOTER_ARMLIMITSWITCHBACK),
		armFrontLimitSwitch (SHOOTER_ARMLIMITSWITCHFRONT),
		armPotent (SHOOTER_ARMPOT),
		primeTask ("Prime", (FUNCPTR) Robot::primeTaskFunc)
	{
		winchDigEncoder.Start();
		winchDigEncoder.Reset();
	}

	float getAngle();
	float setAngle(float);
	float getWinchDistance();
	void calibrate();

	void prime();

	bool cancel();

	void fire();
};

void Shooter::prime()
{
	primeTask.Start();
}

void Shooter::primeTaskFunc()
{
	//pn1=in, pn2=in, winch=off
	//state: fired
	if (!motorPn.Get() && !ratchPn.Get() && !winch.Get())
	{
		ratchPn.Set(true);	// This order is important
		motorPn.Set(true);


		winch.Set(Relay.kOn);
		winchDigEncoder.Reset();

		//while the winch has spun less than 10 revs. This is temporary.
		//TODO: replace this with something useful.
		while ( (winchDigEncoder.Get()/255.0)*360.0) < 360.0*10.0 )
		{   }

		winch.Set(Relay.kOff);
		motorPn1_0.Set(true);
		motorPn1_1.Set(false);
	}
	else
	{
		//motorPn=in, ratchPn=out, winch=off
		//state: ready
		if (!motorPn.Get() && ratchPn.Get() && !winch.Get())
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

bool Shooter::cancel()
{
	primeTask.Stop();
}

void Shooter::fire()
{
	if (primeTask.GetID())
		SmartDashboard::PutString("Errors", "Shooter::fire(): Shooter::prime() still running");
	else
	{
		//motorPn=in, ratchPn=out, winch=off
		//state: ready
		if (!motorPn.Get() && ratchPn.Get() && !winch.Get())
		{
			ratchPn.Set(false);	// fire.
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

float Shooter::getAngle()
{
	return armPotent.Get()*160.0 + 20.0;
}

float Shooter::setAngle(float angle)
{
	while ( fabs((armPotent.Get()*160.0 + 20.0) - angle) > 0.5)
	{    }
}

void Shooter::calibrate()
{
	arm.Set(0.8);	//TODO: make sure this is the right direction!

	while (!armFrontLimitSwitch.Get())
	{    }

	arm.Set(0.0);
	armPotent.CalibrateStart();

	arm.Set(-0.8);	//TODO: make sure this is the right direction!

	while (!armBackLimitSwitch.Get())
	{    }

	arm.Set(0.0);
	armPotent.CalibrateEnd();
}

float getWinchDistance()
{
	return winchDigEncoder.Get(); //TODO: convert this to actual distance!
}