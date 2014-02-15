#include "WPILib.h"

#include "PinDefinitions.h"
#include "Pneumatic.cpp"
#include "DriveSubsystem.cpp"
#include "NetworkListener.cpp"
#include "ShooterSubsystem.cpp"

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
        Shooter shooter;
        Joystick leftStick;
        Joystick rightStick;
        Joystick operatorStick;
        Compressor compressor;
        NetworkListener l;

public:
	RobotDemo(void):
		myRobot(),
                leftStick(2),
                rightStick(1),
                operatorStick(3),
                compressor(PRESSURESWITCH_PIN, COMPRELAY_PIN),
                l()
	{
		myRobot.SetExpiration(0.1);

                myRobot.SetInvertedMotor(RobotDrive::kFrontLeftMotor, true);
                myRobot.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
                myRobot.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
                myRobot.SetInvertedMotor(RobotDrive::kRearRightMotor, true);
	}


	///////////////////////////////////////////////////////////////
	void Autonomous(void)
	{
                compressor.Start();
		myRobot.SetSafetyEnabled(false);

                int resp = l.findtargets();
                if (resp == 0) {
                        SmartDashboard::PutString("Data Recvd", "GOAL IS HOT");
                }

                compressor.Stop();
	}

	
	///////////////////////////////////////////////////////////////
	void OperatorControl(void)
	{
                compressor.Start();
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
                        myRobot.TankDrive(leftStick, rightStick);

                        shooter.MoveArm(operatorStick.GetY());
                        
                        if (operatorStick.GetRawButton(6)) {
                                //adsf
                        } else if (operatorStick.GetRawButton(7)) {
                                //asdf
                        }

			Wait(0.005);								// wait for a motor update time
		}
                compressor.Stop();
	}
	
	///////////////////////////////////////////////////////////////
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

