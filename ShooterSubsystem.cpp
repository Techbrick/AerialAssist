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

inline void primeTaskFunc(UINT32 motorLockPistonPtr, UINT32 ratchetPistonPtr, UINT32 winchPtr, UINT32 winchDigEncoderPtr, UINT32 winchLimitSwitch ...)
{
	va_list arguments;
	va_start(arguments, winchDigEncoderPtr);

	Pneumatic *motorLockPiston = (Pneumatic *) va_arg(arguments, UINT32);
	Pneumatic *ratchetPiston = (Pneumatic *) va_arg(arguments, UINT32);
	Relay *winch = (Relay *) va_arg(arguments, UINT32);
	Encoder *winchDigEncoder = (Encoder *) va_arg(arguments, UINT32);
	DigitalInput *winchLimitSwitch = (DigitalInput *) va_arg(arguments, UINT32);

	//pn1=in, pn2=in, winch=off
	//state: fired
	if (!motorLockPiston->Get() && !ratchetPiston->Get() && !winch->Get())
	{
		ratchetPiston->Set(true);	// This order is important
		motorLockPiston->Set(true);

		winch->Set(Relay::kOn);
		winchDigEncoder->Reset();

		//while the winch has spun less than 10 revs. This is temporary.
		//TODO: replace this with something useful.
		while ( (winchDigEncoder->Get()) < 360.0*10.0 && winchLimitSwitch->Get() == false )
		{   }

		winch->Set(Relay::kOff);
		motorLockPiston->Set(false);
	}
	else
	{
		//motorLockPiston=in, ratchetPiston=out, winch=off
		//state: ready
		if (!motorLockPiston->Get() && ratchetPiston->Get() && !winch->Get())
		{
			//Do nothing.
		}
		else
		{
			//other
			//state: either winching, transitioning, or broken!
			SmartDashboard::PutString("Errors","Shooter::Prime(): Shooter in unexpected state");
		}
	}

	va_end(arguments);
}


class Shooter {
	Relay winch;
	Encoder winchDigEncoder;
	DigitalInput winchLimitSwitch;

	bool winchPrimed;

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
		winchDigEncoder (SHOOTER_WINCHDIGITALENCA, SHOOTER_WINCHDIGITALENCB),
		winchLimitSwitch (SHOOTER_WINCHLIMITSWITCH),
		motorLockPiston (SHOOTER_WINCHMOTORSOLIN, SHOOTER_WINCHMOTORSOLOUT),
		ratchetPiston (SHOOTER_WINCHRATCHSOLIN, SHOOTER_WINCHRATCHSOLOUT),
		arm (SHOOTER_ARMTALON),
		armBackLimitSwitch (SHOOTER_ARMLIMITSWITCHBACK),
		armFrontLimitSwitch (SHOOTER_ARMLIMITSWITCHFRONT),
		armPot (SHOOTER_ARMPOT),
		primeTask ("Prime", (FUNCPTR) primeTaskFunc)
	{
		winchDigEncoder.Start();
		winchDigEncoder.Reset();
	}

	void Prime()
	{
		primeTask.Start( (UINT32) &motorLockPiston, (UINT32) &ratchetPiston, (UINT32) &winch, (UINT32) &winchDigEncoder, (UINT32) &winchLimitSwitch);
	}

	bool Cancel()
	{
		return primeTask.Stop();
	}

	bool IsPrimed()
	{
		return winchPrimed;
	}

	void Relax()
	{
		if (IsPrimed())
		{
			motorLockPiston.Set(true);
			Wait(0.5);
			ratchetPiston.Set(false);
			Wait(1);
			ratchetPiston.Set(true);
			Wait(0.3);
			motorLockPiston.Set(false);
		}
	}

	void Fire()
	{
		while (primeTask.GetID() + 1)
		{
			;
		}

		//motorLockPiston=in, ratchetPiston=out, winch=off
		//state: ready
		if (!motorLockPiston.Get() && ratchetPiston.Get() && !winch.Get())
		{
			ratchetPiston.Set(false);	// fire.
			Wait(0.1);
			winchDigEncoder.Reset();
			winchPrimed = false;
		}
		else
		{
			//other
			///state: probably very broken.
			SmartDashboard::PutString("Errors", "Shooter::Fire(): Unexpected state, potential danger!");
		}
	}

	float GetAngle()
	{
		return armPot.Get()*160.0 + 20.0;
	}

	void SetAngle(float targetAngle)
	{
		float currentAngle = armPot.Get()*160.0 + 20.0;
		float rate = 0.8;
		float initialDistance = abs(angle - target);

		arm.Set(rate);

		while ( fabs(currentAngle - targetAngle) > 0.5)
		{
			currentAngle = armPot.Get()*160.0 + 20.0;
			rate = (float)0.5*(currentAngle - targetAngle)/initialDistance +
				((float)0.3*sign((float)0.5*(angle-target)/initialDistance));
			arm.Set(rate);
		}

		arm.Set(0);
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

	float GetWinchDistance()
	{
		return (winchDigEncoder.Get()/360.0) * 3.14; //TODO: convert this to actual distance!
	}
};
