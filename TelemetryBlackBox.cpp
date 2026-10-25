#include "TelemetryBlackBox.h"
#include "Buffer.h"

#include <iostream>
#include <bitset>

void TelemetryBlackBox::onRocketStateChange(RocketState state) {

	state_ = state;

	if (!baro_error_ and (state_ == 0 or state_ == 4))
		altitude_ = 0;

}

void TelemetryBlackBox::onParachuteDeployed() {
	
	parachute_ = true;

}

void TelemetryBlackBox::onBaroReading(bool error, uint16_t value) {
	
	if (!error) {

		baro_error_ = false;

		if (state_ != 0 and state_ != 4)
			altitude_ = value;
		else
			altitude_ = 0;

	}
	else {

		baro_error_ = true;
		altitude_ = 0xFFFF;

	}
	
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

	uint32_t df; 
	
	if (!Buffer::instance().getLastDataFrame(
		state, parachute, baro_error, altitude, checksum, df)
		)
	{
		
		std::cout << "\n-===== No DataFrame Records =====-\n";
		return 0;

	}

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

	std::cout << "CHECKSUM:\t\t" << std::bitset<8>(checksum) << "\n\n";

	if (Buffer::instance().validateChecksum(df))
		return df;
	else
		return 0xFFFFFFFF;

}
