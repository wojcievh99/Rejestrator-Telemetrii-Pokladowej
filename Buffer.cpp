#include "Buffer.h"
#include <iostream>

Buffer& Buffer::instance() {
	static Buffer i;
	return i;
}

void Buffer::makeSpaceInBuffer() {
	cyclicBuffer_[15] = 0;

	// moving
	for (int i = 15; i >= 1; i--) {

		cyclicBuffer_[i] = cyclicBuffer_[i - 1];
		cyclicBuffer_[i - 1] = 0;

	}
	// thanks to this cyclicBuffer[0] is empty
}

void Buffer::setState(uint32_t& dataFrame, uint8_t state) {

	// clean state(for clarity) (3 bits) and set new one
	dataFrame = dataFrame & 0xFFFFFFF8;
	dataFrame = dataFrame | (state & 0x7);
	
}

void Buffer::setParachute(uint32_t& dataFrame, bool parachute) {

	dataFrame = dataFrame & ~(1 << 3); 

	if (parachute)
		dataFrame = dataFrame | (1 << 3);

}

void Buffer::setBaroError(uint32_t& dataFrame, bool baro_error) {

	dataFrame = dataFrame & ~(1 << 4); 
	
	if (baro_error)
		dataFrame = dataFrame | (1 << 4);

}

void Buffer::setAltitude(uint32_t& dataFrame, uint16_t altitude) {

	// clean 8-23
	dataFrame = dataFrame & ~(0xFFFF << 8);

	uint8_t state = dataFrame & (0b111); // first 3 bits
	if (state == 0 or state == 4)
		return;
	// set to min

	bool baro_error = (dataFrame & (1 << 4)) >> 4; // 4th bit
	if (baro_error == true)
		altitude = 0xFFFF;
	// set to max

	dataFrame = dataFrame | (altitude << 8);

}

void Buffer::checkSum(uint32_t& dataFrame) {

	// compute
	uint8_t checksum =
		(dataFrame & 0xFF) ^
		((dataFrame & (0xFF << 8)) >> 8) ^
		((dataFrame & (0xFF << 16)) >> 16);

	// save
	dataFrame = dataFrame & ~(0xFF << 24);
	dataFrame = dataFrame | (checksum << 24);

}

bool Buffer::validateChecksum(uint32_t dataFrame) {

	uint8_t correct_checksum =
		(dataFrame & 0xFF) ^
		((dataFrame & (0xFF << 8)) >> 8) ^
		((dataFrame & (0xFF << 16)) >> 16);

	uint8_t alleged_checksum = dataFrame >> 24;

	return (correct_checksum == alleged_checksum);

}


void Buffer::recordDataFrame(uint8_t state, bool parachute,
	bool baro_error, uint16_t altitude) 
{
	uint32_t df = 0;

	setState(df, state);
	setParachute(df, parachute);
	setBaroError(df, baro_error);
	setAltitude(df, altitude);
	checkSum(df);

	makeSpaceInBuffer();
	cyclicBuffer_[0] = df;

}

uint32_t Buffer::getLastDataFrame(uint8_t& state, bool& parachute,
	bool& baro_error, uint16_t& altitude, uint8_t& checksum) 
{
	uint32_t df = cyclicBuffer_[0];

	// state:
	state = (df & 0b111);

	// parachute:
	parachute = ((df & (1 << 3)) >> 3);

	// baro_error:
	baro_error = ((df & (1 << 4)) >> 4);

	// altitude:
	altitude = ((df & (0xFFFF << 8)) >> 8);

	// checksum:
	checksum = df >> 24;

	return df;
}
