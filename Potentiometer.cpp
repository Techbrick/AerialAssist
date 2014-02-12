#include "WPILib.h"

class Potentiometer {

public:
	AnalogChannel potent;
	float start, end;	//TODO: check if start and end sound like starting
						// and ending a process. (Esp. in function names.)

	Potentiometer (UINT32 channel) : potent (channel), start (0), end (5)
	{    }

	Potentiometer (UINT32 channel, float startV, float endV) :
		potent (channel), start (startV), end (endV)
	{    }

	float Get();
	void CalibrateStart();
	void CalibrateEnd();
};

float Potentiometer::Get()
{
	return (potent.GetAverageVoltage() - start)/(end - start);
}

void Potentiometer::CalibrateStart()
{
	start = potent.GetAverageVoltage();
}

void Potentiometer::CalibrateEnd()
{
	end = potent.GetAverageVoltage();
}