#ifndef TECHBRICK_PNEUMATICS_CPP_
#define TECHBRICK_PNEUMATICS_CPP_

#include "WPILib.h"

class Pneumatic {
	Solenoid InSole;
	Solenoid OutSole;
public:
	Pneumatic (UINT32 InSoleChannel, UINT32 OutSoleChannel):
		InSole (InSoleChannel),
		OutSole (OutSoleChannel)
	{    }

	void Set (bool state);	// true is out, false is in
	bool Get ();			// the same
};

void Pneumatic::Set (bool state)
{
	if (InSole.Get())
	{
		InSole.Set (!state);
		OutSole.Set (state);
	}
	else
	{
		OutSole.Set (state);
		InSole.Set (!state);
	}
}

bool Pneumatic::Get ()
{
	return OutSole.Get();
}

#endif