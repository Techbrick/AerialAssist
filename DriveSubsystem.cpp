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
	DriveSubsystem (UINT32 frontLeft, UINT32 rearLeft, UINT32 frontRight,
		UINT32 rearRight) : RobotDrive(frontLeft, rearLeft, frontRight, rearRight)
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
