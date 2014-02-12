#include "WPILib.h"
#include "PinDefinitions.h"
#include <tgmath.h>
// Computer specific path for Numeri: ..\..\Techbrick\MainWorkingCode\WPILib\
// WPILib\WPILib.h


//TODO: makesure that Potentiometer.cpp class is used!!

class Shooter {
	Relay winch;
	Encoder winchDigEncoder;
	Solenoid motorPn1_0;  // For engaging the motor with the winch spool.
	Solenoid motorPn1_1;  // 1_0 is false (0) when the pneumatic is out,
							// 1_1 is true (1) when the pneumatic is out.
							// In other words, the solenoid 1_x.Get() is
							// true when the state of the pneumatic is x
							// (0 = in, 1 = out).

	Solenoid ratchPn2_0;  // For engaging the ratchet.
	Solenoid ratchPn2_1;  // Similar naming scheme.
	
	Talon arm;
	DigitalInput armBackLimitSwitch;
	DigitalInput armFrontLimitSwitch;
	Potentiometer armPotent;

	Task primeTask;

	void primeTaskFunc();

public:
	Shooter () :
		winch				(SHOOTER_WINCH_SPIKE),
		winchDigEncoder		(SHOOTER_WINCH_DE_A, SHOOTER_WINCH_DE_B),
		motorPn1_0		(SHOOTER_WINCH_MOTORPNEUM1_0),
		motorPn1_1		(SHOOTER_WINCH_MOTORPNEUM1_1),
		ratchPn2_0		(SHOOTER_WINCH_RATCHPNEUM2_0),
		ratchPn2_1		(SHOOTER_WINCH_RATCHPNEUM2_1),
		arm					(SHOOTER_ARM_TALON),
		armBackLimitSwitch	(SHOOTER_ARM_LIMSWIT_BACK),
		armFrontLimitSwitch	(SHOOTER_ARM_LIMSWIT_FRONT),
		armPotent			(SHOOTER_ARM_POT),
		primeTask			("Prime", (FUNCPTR) Robot::primeTaskFunc);

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
	if (motorPn1_0.Get() && ratchPn2_0.Get() && !winch.Get())
	{
		ratchPn2_0.Set(false);
		ratchPn2_1.Set(true);
		motorPn1_0.Set(false);
		motorPn1_1.Set(true);


		winch.Set(Relay.kOn);
		winchDigEncoder.Reset();
		winchDigEncoder.Start();

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
		//pn1=in, pn2=out, winch=off
		//state: ready
		if (motorPn1_0.Get() && ratchPn2_1.Get() && !winch.Get())
		{
			//Do NOTHING.
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
		if (motorPn1_0.Get() && ratchPN1_1.Get() && !winch.Get())
		{
			ratchPN1_1.Set(false);
			ratchPN1_0.Set(true);
		}
		else
		{
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