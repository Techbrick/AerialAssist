#include "WPILib.h"
#include "Pneumatic.cpp"

/* This is a simple wrapper for RobotDrive.
 * Includes TankDrive w/ Super Shifters
 */

//TODO: Possibly a Talon current check to prevent burnouts.

class DriveSubsystem : public RobotDrive {

	Pneumatic LeftSuper;	//Left Super Shifter
	Pneumatic RightSuper;	//Right - -

public:
	DriveSubsystem ( ) :
		RobotDrive (DRIVE_FRONTLEFT, DRIVE_REARLEFT, DRIVE_FRONTRIGHT, DRIVE_REARRIGHT),
		LeftSuper (DRIVE_LEFT_SHFT_SOL1, DRIVE_LEFT_SHFT_SOL2),
		RightSuper (DRIVE_RIGHT_SHFT_SOL1, DRIVE_RIGHT_SHFT_SOL2)
	{    }

	void TankDrive(GenericHID &leftStick, GenericHID &rightStick)
	{
		TankDrive(leftStick.GetY(), rightStick.GetY());

		if (leftStick.GetButton(1))
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
