#include "WPILib.h"
// Specific path for Numeri: ..\..\Techbrick\MainWorkingCode\
// WPILib\WPILib\WPILib.h

/* This is a simple wrapper for RobotDrive.
 * If needed, we could add more functions to it.
 * (Possibly a Talon current threshold function to prevent
 * burning them out)
 */

class DriveSubsystem : public RobotDrive {

public:
	DriveSubsystem (UINT32 frontLeft, UINT32 rearLeft, UINT32 frontRight,
		UINT32 rearRight) : RobotDrive(frontLeft, rearLeft, frontRight, rearRight)
	{

	}
};
