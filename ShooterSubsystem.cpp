#include "WPILib.h"
#include "PinDefinitions.h"
// Computer specific path for Numeri: ..\..\Techbrick\MainWorkingCode\WPILib\
// WPILib\WPILib.h


class Shooter {
	Relay winch;
	Encoder winchDigEncoder;
	Solenoid motorPn1_0;  // For engaging the motor with the winch spool.
	Solenoid motorPn1_1;  // 1_0 is false (0) when the pneumatic is out,
							// 1_1 is true (1) when the pneumatic is out.
							// In other words, the solenoid 1_x.Get() is
							// true when the state of the pneumatic is x
							// (0 = in, 1 = out).

	Solenoid RatchPn2_0;  // For engaging the ratchet.
	Solenoid RatchPn2_1;  // Similar naming scheme.
	
	Talon arm;
	DigitalInput armBackLimitSwitch;
	DigitalInput armFrontLimitSwitch;
	AnalogChannel armPotent;

	Task primeTask;

public:
	Shooter () :
		winch				(SHOOTER_WINCH_SPIKE),
		winchDigEncoder		(SHOOTER_WINCH_DE_A, SHOOTER_WINCH_DE_B),
		motorPn1_0		(SHOOTER_WINCH_MOTORPNEUM1_0),
		motorPn1_1		(SHOOTER_WINCH_MOTORPNEUM1_1),
		RatchPn2_0		(SHOOTER_WINCH_RATCHPNEUM2_0),
		RatchPn2_1		(SHOOTER_WINCH_RATCHPNEUM2_1),
		arm					(SHOOTER_ARM_TALON),
		armBackLimitSwitch	(SHOOTER_ARM_LIMSWIT_BACK),
		armFrontLimitSwitch	(SHOOTER_ARM_LIMSWIT_FRONT),
		armPotent			(SHOOTER_ARM_POT),
		primeTask			("Prime", prime);

	float getAngle();
	float getWinchDistance();
	void calibrate();

	void prime();
	void cancel();

	void fire();
};

void Shooter::prime()
{
	//pn1=in, pn2=in, winch=off
	//state: fired
	if (motorPn1_0.Get() && RatchPn2_0.Get() && !winch.Get())
	{
		RatchPn2_0.Set(false);
		RatchPn2_1.Set(true);
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
}