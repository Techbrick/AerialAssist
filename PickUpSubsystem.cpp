#include "WPILib.h"			// the library
#include "PinDefinitions.h" // holds all the pin numbers
// actual path of the library: wpilib/WPILib.h
// press ctrl+shift+o when the cursor is on ^ to view the file

class PickUpSubsystem {
	//here's your initialization code
	Jaguar jag1;
	
	//methods
public:
	PickUpSubsystem (void) :
		// same order as above
		jag1(EXAMPLE_PIN)
	{
		
	}
};
