#include "TelemetryBlackBox.h"
#include "Buffer.h"

#include <iostream>

void TelemetryBlackBox::onRocketStateChange(RocketState state) {

	state_ = state;

}

void TelemetryBlackBox::onParachuteDeployed() {
	
	parachute_ = true;

}

void TelemetryBlackBox::onBaroReading(bool error, uint16_t value) {
	
	if (!error)
		altitude_ = value;
	else
		baro_error_ = true;
	
}

void TelemetryBlackBox::recordCurrentState() {
	
	Buffer::instance().recordDataFrame(
		state_, parachute_, baro_error_, altitude_
	);

}

uint32_t TelemetryBlackBox::getLastDataframe() {

	uint8_t state, checksum; 
	bool parachute, baro_error; 
	uint16_t altitude; 

	uint32_t df = Buffer::instance().getLastDataFrame(
		state, parachute, baro_error, altitude, checksum
	);

	std::cout << "\n-===== Last DataFrame Recorded =====-\n\n";

	std::cout << "STATE:\t\t";
	switch (state)
	{
	case 0:
		std::cout << "DISARM";
		break;
	case 1:
		std::cout << "ARM";
		break;
	case 2:
		std::cout << "FLIGHT";
		break;
	case 3:
		std::cout << "PARACHUTE";
		break;
	case 4:
		std::cout << "LANDED";
		break;
	default:
		std::cout << "unknown";
		break;
	}
	std::cout << "\n";

	std::cout << "PARACHUTE:\t\t" << parachute << "\n";

	std::cout << "BARO_ERROR:\t\t" << baro_error << "\n";

	std::cout << "ALTITUDE:\t\t" << altitude << "\n";

	std::cout << "CHECKSUM:\t\t" << checksum << "\n";

	if (Buffer::instance().validateChecksum(df))
		return df;
	else
		return 0xFFFFFFFF;

}
