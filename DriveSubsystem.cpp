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

		if (leftStick.GetRawButton(1))
		{
			LeftSuper.Set(true);	//Push out both shifters 
			RightSuper.Set(true);
		}
		else
		{
			LeftSuper.Set(false);	//Pull in both shifters 
			RightSuper.Set(false);
		}
	}
};
