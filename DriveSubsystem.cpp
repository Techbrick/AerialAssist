#include "WPILib.h"
// Specific path for Numeri: ..\..\Techbrick\MainWorkingCode\WPILib\WPILib\WPILib.h

/* This is a simple wrapper for RobotDrive.
 * If needed, we could add more functions to it.
 * (Possibly a Talon current threshold function to prevent
 * burning them out)
 */

class DriveSubsystem : public RobotDrive {

public:
	DriveSubsystem (UINT32 frontLeftMotorChannel, UINT32 rearLeftMotorChannel, UINT32 frontRightMotorChannel, UINT32 rearRightMotorChannel, float sensitivity = 0.5) : RobotDrive(UINT32 frontLeftMotorChannel, UINT32 rearLeftMotorChannel, UINT32 frontRightMotorChannel, UINT32 rearRightMotorChannel, float sensitivity = 0.5)
	{

	}
};
