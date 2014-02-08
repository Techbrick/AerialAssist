#include "WPILib.h"

#include "PinDefinitions.h"
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

public:
	RobotDemo(void):
		myRobot(FRONTLEFT_JAG, BACKLEFT_JAG, FRONTRIGHT_JAG, BACKRIGHT_JAG),
                leftStick(1),
                rightStick(2)
	{
		myRobot.SetExpiration(0.1);
	}


	///////////////////////////////////////////////////////////////
	void Autonomous(void)
	{
		myRobot.SetSafetyEnabled(false);
	}

	
	///////////////////////////////////////////////////////////////
	void OperatorControl(void)
	{
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
                        myRobot.TankDrive(leftStick, rightStick);
			Wait(0.005);								// wait for a motor update time
		}
	}
	
	///////////////////////////////////////////////////////////////
	void Test() {
		
	}
};

START_ROBOT_CLASS(RobotDemo);

