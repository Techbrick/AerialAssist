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
	DriveSubsystem ( )
		: RobotDrive (DRIVE_FRNTLEFT, DRIVE_REARLEFT, DRIVE_FRNTRIGHT, DRIVE_REARRIGHT),
		LeftSuper (DRIVE_LEFT_SUPERSHFTR_PNE),
		RightSuper (DRIVE_RIGHT_SUPERSHFTR_PNE)
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
