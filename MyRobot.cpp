#include "WPILib.h"

#include "PinDefinitions.h"
#include "Pneumatic.cpp"
#include "DriveSubsystem.cpp"
#include "NetworkListener.cpp"
#include "ShooterSubsystem.cpp"
#include "PickUpSubsystem.cpp"

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
        PickUpSubsystem grabber;
        Joystick leftStick;
        Joystick rightStick;
        Joystick operatorStick;
        Compressor compressor;
        DigitalInput lpswitch;
        DigitalInput rpswitch;
        NetworkListener l;

public:
	RobotDemo(void):
		myRobot(),
                shooter(),
                grabber(),
                leftStick(2),
                rightStick(1),
                operatorStick(3),
                compressor(PRESSURESWITCH_PIN, COMPRELAY_PIN),
                lpswitch(DRIVE_LEFTPSWITCH),
                rpswitch(DRIVE_RIGHTPSWITCH),
                l()
	{
		myRobot.SetExpiration(0.1);

                myRobot.SetInvertedMotor(RobotDrive::kFrontLeftMotor, true);
                myRobot.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
                myRobot.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
                myRobot.SetInvertedMotor(RobotDrive::kRearRightMotor, true);
	}

        void LockToLine ()
        {
            // make sure value is correct

            while (lpswitch.Get() == 0 || rpswitch.Get() == 0) { // drive until switch is triggered
                myRobot.TankDrive(.3, .3);
            }

            if (lpswitch.Get() != 0) { // if it hit the left switch first
                while (rpswitch.Get() == 0) {
                    myRobot.TankDrive(0, .3);   
                }
            } else if (rpswitch.Get() != 0) { // if it hit the right switch first
                while (lpswitch.Get() == 0) {
                    myRobot.TankDrive(.3, 0);
                }
            }

            // we are now on the line
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
                        shooter.MoveArm(-operatorStick.GetY());
                        shooter.ShowPotentiometer();

                        // testing controls for the ratchet
                        if (operatorStick.GetRawButton(11) && operatorStick.GetRawButton(10)) {
                            shooter.SetRatchet(true);
                        } else if (operatorStick.GetRawButton(11)) {
                            shooter.SetRatchet(false);
                        }

                        // shooting controls
                        if (operatorStick.GetRawButton(8)) {
                            shooter.Prime(&operatorStick, SHOOTER_CANCELBUTTON);
                        } else if (operatorStick.GetRawButton(1)) {
                            shooter.Fire();
                        } else if (operatorStick.GetRawButton(11)) {
                            shooter.Cancel(); // doesn't actually work for some reason
                        }

                        // grabber arm controls
                        if (operatorStick.GetRawButton(4)) {
                            grabber.SetArm(true);
                        } else if (operatorStick.GetRawButton(5)) {
                            grabber.SetArm(false);
                        }

                        // grabber roller controls
                        if (operatorStick.GetRawButton(2)) {
                            grabber.SetRoller(-1);
                        } else if (operatorStick.GetRawButton(3)) {
                            grabber.SetRoller(1);
                        } else {
                            grabber.SetRoller(0);
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

