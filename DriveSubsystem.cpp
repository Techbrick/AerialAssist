#include "WPILib.h"
#include "Pneumatic.cpp"
#include "PinDefinitions.h"

/* This is a simple wrapper for RobotDrive.
 * Includes TankDrive w/ Super Shifters
 */

//TODO: Possibly a Talon current check to prevent burnouts.


class DriveSubsystem : public RobotDrive {
	Pneumatic SuperShifter;	//Super Shifter Pneumatic
	
public:
	DriveSubsystem ( ) :
		RobotDrive (DRIVE_FRONTLEFT, DRIVE_REARLEFT, DRIVE_FRONTRIGHT, DRIVE_REARRIGHT),
		SuperShifter (DRIVE_SUPERSHIFTSOLIN, DRIVE_SUPERSHIFTSOLOUT)
	{    }

	void TankDrive(Joystick &leftStick, Joystick &rightStick)
	{
		RobotDrive::TankDrive(leftStick, rightStick);

		if (leftStick.GetRawButton(1) && SuperShifter.Get() == false)
		{
			SuperShifter.Set(true);	//Push pistons out
		}
		else if (leftStick.GetRawButton(1) == false && SuperShifter.Get())
		{
			SuperShifter.Set(false);	//Pull pistons in
		}
	}

	void TankDrive(float leftValue, float rightValue)
	{
		RobotDrive::TankDrive(leftValue, rightValue);
	}
};

