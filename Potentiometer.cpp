#include "WPILib.h"

class Potentiometer {

public:
	AnalogChannel pot;
	float start, end;	//TODO: check if start and end sound like starting
	// and ending a process. (Esp. in function names.)

	Potentiometer (UINT32 channel) : pot (channel), start (0), end (5)
	{    }

	Potentiometer (UINT32 channel, float startV, float endV) :
		pot (channel), start (startV), end (endV)
	{    }

	float Get()
	{
		return (pot.GetAverageVoltage() - start)/(end - start);
	}

	void CalibrateStart()
	{
		start = pot.GetAverageVoltage();
	}

	void CalibrateEnd()
	{
		end = pot.GetAverageVoltage();
	}
};
