#include "WPILib.h"

#include "PinDefinitions.h"
#include "Pneumatic.cpp"
#include "DriveSubsystem.cpp"

/**
 * MyRobot.cpp
 * The main class of FRC Team 3941's Aerial Assist robot.
 * Subsystems are included by .cpp file because otherwise not all files are
 * sent to the robot.
 */ 
class RobotDemo : public SimpleRobot
{
	// CANNOT INITIALIZE THE SAME JAGUAR TWICE!!!!
        DriveSubsystem myRobot;
        Joystick leftStick;
        Joystick rightStick;
        Compressor comp;

public:
	RobotDemo(void):
		myRobot(),
                leftStick(1),
                rightStick(2),
                compressor(PRESSURESWITCH_PIN, COMPRELAY_PIN)
	{
		myRobot.SetExpiration(0.1);
	}


	///////////////////////////////////////////////////////////////
	void Autonomous(void)
	{
                compressor.Start()
		myRobot.SetSafetyEnabled(false);
                compressor.Stop()
	}

	
	///////////////////////////////////////////////////////////////
	void OperatorControl(void)
	{
                compressor.Start()
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
                        myRobot.TankDrive(leftStick, rightStick);
			Wait(0.005);								// wait for a motor update time
		}
                compressor.Stop()
	}
	
	///////////////////////////////////////////////////////////////
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

