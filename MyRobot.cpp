#include "WPILib.h"

#include "PinDefinitions.h"
#include "Pneumatic.cpp"
#include "DriveSubsystem.cpp"
//#include "NetworkListener.cpp"
#include "ShooterSubsystem.cpp"
#include "PickUpSubsystem.cpp"
#include "Vision.cpp"
#include "pthread.h"

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
        AnalogChannel pot;
        Encoder leftEncoder;
        Vision vision;
        //NetworkListener l;

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
                pot(7),
                leftEncoder(11, 12, true),
                vision()
                //l()
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
                leftEncoder.Start();
                leftEncoder.Reset();
		myRobot.SetSafetyEnabled(false);
		double angle = 55;
		Wait(.25);
				if((pot.GetValue()-608.5)/(-3.566666667)>=angle)
				{
					while((pot.GetValue()-608.5)/(-3.566666667)>=angle)
					{
						shooter.MoveArm(1);
					}
				}
				else
				{
					while((pot.GetValue()-608.5)/(-3.566666667)<=angle)
					{
						shooter.MoveArm(-1);
					}
				}
				
				shooter.MoveArm(0);
				
				
				Wait(.5);
				
				
				while(leftEncoder.Get()/250*3.14*6/12 < 8){
					myRobot.TankDrive(-1, -.9);
				}
				
				vision.Process();
				
				while(leftEncoder.Get()/250*3.14*6/12 < 12){
					myRobot.TankDrive(-.55, -.5);
				}
				//Wait(3.2);
				
				myRobot.TankDrive(0, 0);
				
				Wait(.5);
				
				if(!vision.GoalIsHot())
				{
					Wait(3);
				}
				
				shooter.Fire();
				/*int resp = l.findtargets();
                if (resp == 0) {
                        SmartDashboard::PutString("Data Recvd", "GOAL IS HOT");
                }
                */
                compressor.Stop();
	}

	
	///////////////////////////////////////////////////////////////
	void OperatorControl(void)
	{
        compressor.Start();
        leftEncoder.Start();
        leftEncoder.Reset();
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
						SmartDashboard::PutNumber("Potentiometer", (int) (pot.GetValue()-608.5)/(-3.566666667));
						SmartDashboard::PutNumber("Encoder", leftEncoder.Get()/250*3.14*6/12);
						//printf("loop \n");
						myRobot.TankDrive(leftStick, rightStick);
                        shooter.MoveArm(-operatorStick.GetY());
                        
                        
                        //shooter.ShowPotentiometer();

                        // testing controls for the ratchet
                        if (operatorStick.GetRawButton(11) && operatorStick.GetRawButton(10)) {
                            shooter.SetRatchet(true);
                        } else if (operatorStick.GetRawButton(11)) {
                            shooter.SetRatchet(false);
                        }

                        // shooting controls
                        //printf("shooter buttons \n");
                        if (operatorStick.GetRawButton(8)) {
                        	printf("GO! \n");
                            shooter.Prime(&operatorStick, SHOOTER_CANCELBUTTON);
                        } else if (operatorStick.GetRawButton(1)) {
                            shooter.Fire();
                        } /*else if (operatorStick.GetRawButton(11)) {
                            shooter.Cancel(); // doesn't actually work for some reason
                        }*/

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
	
		double angle = 55;
		compressor.Start();
		while(!compressor.GetPressureSwitchValue())
		{
		                        	
		}
		                        
		                compressor.Stop();
						if((pot.GetValue()-608.5)/(-3.566666667)>=angle)
						{
							while((pot.GetValue()-608.5)/(-3.566666667)>=angle)
							{
								shooter.MoveArm(1);
							}
						}
						else
						{
							while((pot.GetValue()-608.5)/(-3.566666667)<=angle)
							{
								shooter.MoveArm(-1);
							}
						}
						
						shooter.MoveArm(0);
						

                        shooter.Prime(&operatorStick, SHOOTER_CANCELBUTTON);
                        grabber.SetArm(true);                                      
	}
};

START_ROBOT_CLASS(RobotDemo);

