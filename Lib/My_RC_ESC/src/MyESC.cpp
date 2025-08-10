#include "MyESC.h"

MyESC::MyESC(byte ESC_pin, int outputMin, int outputMax, int armVal)
{
	oPin = ESC_pin;
	oMin = outputMin;
	oMax = outputMax;
	oArm = armVal;
}

// < Destructor >
MyESC::~MyESC()
{
	// Nothing to destruct
}

/*
 * Calibrate the maximum and minimum PWM signal the ESC is expecting
 * depends on the outputMin, outputMax values from the constructor
 */
void MyESC::calib(void)
{
	myESC.attach(oPin);  			// attaches the ESC on pin oPin to the ESC object
	myESC.writeMicroseconds(oMax);
		delay(calibrationDelay);
	myESC.writeMicroseconds(oMin);
		delay(calibrationDelay);
	arm();
}

/*
 * Sent a signal to Arm the ESC
 * depends on the Arming value from the constructor
 */
void MyESC::arm(void)
{
	myESC.attach(oPin);  			// attaches the ESC on pin oPin to the ESC object
	myESC.writeMicroseconds (oArm);
}

/*
 * Sent a signal to stop the ESC
 * depends on the ESC stop pulse value
 */
void MyESC::stop(void)
{
	myESC.writeMicroseconds (stopPulse);
}

/*
 * Sent a signal to set the ESC speed
 * depends on the calibration minimum and maximum values
 */
void MyESC::speed(int outputESC)
{
	oESC = constrain(outputESC, oMin, oMax);
	myESC.writeMicroseconds(oESC);
}

/*
 * Set the current calibration delay in miliseconds
 *
 */
void MyESC::setCalibrationDelay(uint32_t calibration_delay)
{
	calibrationDelay = calibration_delay;
}

/*
 * Set the current Stop pulse in microseconds
 *
 */
void MyESC::setStopPulse(uint32_t stop_pulse)
{
	stopPulse = stop_pulse;
}
