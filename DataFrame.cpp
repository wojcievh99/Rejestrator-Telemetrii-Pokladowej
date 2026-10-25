#include "DataFrame.h"
#include <iostream>

DataFrame& DataFrame::instance() {
	static DataFrame i;
	return i;
}

void DataFrame::moveDatainBuffer() {
	cyclicBuffer_[15] = 0;

	// moving
	for (int i = 15; i >= 1; i--) {

		cyclicBuffer_[i] = cyclicBuffer_[i - 1];
		cyclicBuffer_[i - 1] = 0;

	}
	// thanks to this cyclicBuffer[0] is empty
}

void DataFrame::onDataFrameChange(uint32_t dataFrameState) {
	// I have to move existing bits so that the first 16 bits are cleared 
	// for this save. Then save it there

	// 1. erase last 32 bits (delete the oldest save)
	moveDatainBuffer();
	
	// 2. save the new state
	cyclicBuffer_[0] = dataFrameState;

}

void DataFrame::checkSum() {

	// compute
	uint8_t checksum =
		(dataFrame_ & 0xFF) ^
		((dataFrame_ & (0xFF << 8)) >> 8) ^
		((dataFrame_ & (0xFF << 16)) >> 16);

	// save
	dataFrame_ = dataFrame_ & ~(0xFF << 24);
	dataFrame_ = dataFrame_ | (checksum << 24);

}

void DataFrame::setState(uint8_t state) {

	onDataFrameChange(dataFrame_);

	// clean current state (3 bits) and set new one
	dataFrame_ = dataFrame_ & 0xFFFFFFF8;
	dataFrame_ = dataFrame_ | (state & 0x7);

	if (state == 0 or state == 4) {

		// altitude change
		dataFrame_ = dataFrame_ & ~(0xFFFF << 8);
		// to the minimum possible value

	}
	
	checkSum();

}

uint8_t DataFrame::getCurrentState() {
	return dataFrame_ & 0x7;
}

void DataFrame::onParachuteDeploy() {

	onDataFrameChange(dataFrame_);

	dataFrame_ = dataFrame_ & ~(1 << 3); // for clarity
	dataFrame_ = dataFrame_ | (1 << 3);

	checkSum();

}

bool DataFrame::getParechuteState() {
	return (dataFrame_ & 0x8) >> 3;
}

void DataFrame::onBaroError() {

	// if error already 
	if (getBaroState()) return;

	onDataFrameChange(dataFrame_);

	dataFrame_ = dataFrame_ & ~(1 << 4); 
	dataFrame_ = dataFrame_ | (1 << 4);

	// altitude change
	dataFrame_ = dataFrame_ & ~(0xFFFF << 8);
	dataFrame_ = dataFrame_ | (0xFFFF << 8);
	// to the maximum possible value

	checkSum();

}

bool DataFrame::getBaroState() {
	return (dataFrame_ & (1 << 4)) >> 4;
}

void DataFrame::setAltitude(uint16_t altitude) {

	// cannot change the altitude if baro is not working
	if (getBaroState()) return;
	// cannot change is state = DIRARM or LANDED
	if (getCurrentState() == 4 or getCurrentState() == 0) return;

	onDataFrameChange(dataFrame_);

	dataFrame_ = dataFrame_ & ~(0xFFFF << 8);
	dataFrame_ = dataFrame_ | (altitude << 8);

	checkSum();
}

uint16_t DataFrame::getAltitude() {
	return (dataFrame_ & (0xFFFF << 8)) >> 8;
}

uint16_t DataFrame::getCurrentDataFrame() {
	return dataFrame_;
}
uint16_t DataFrame::getLastDataFrame() {
	return cyclicBuffer_[0];
}
