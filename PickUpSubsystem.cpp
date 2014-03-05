#include "WPILib.h"			// the library
#include "PinDefinitions.h" // holds all the pin numbers
#include "Pneumatic.cpp"
// actual path of the library: wpilib/WPILib.h
// press ctrl+shift+o when the cursor is on ^ to view the file

class PickUpSubsystem {
	Relay rollerSpike;
	Pneumatic pickupArm;
	
public:
	PickUpSubsystem (void) :
		rollerSpike(PICKUP_ROLLERSPIKE),
		pickupArm(PICKUP_PICKUPARMSOLIN, PICKUPARMSOLOUT)		
	{	}
	
	void SetArm(bool setting)
	{
		pickupArm.Set(setting);
	}
	
	void SetRoller(int direction)
	{
		if (direction == -1)
		{
			rollerSpike.Set(Relay::kReverse);
		}
		
		if (direction == 0)
		{
			rollerSpike.Set(Relay::kOff);
		}
		
		if (direction == 1)
		{
			rollerSpike.Set(Relay::kForward);
		}
	}
};
