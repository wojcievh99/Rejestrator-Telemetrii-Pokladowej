#include "DataFrame.h"
#include <iostream>

DataFrame& DataFrame::instance() {
	static DataFrame i;
	return i;
}

void DataFrame::setState(uint8_t state) {
	// clean current state (3 bits) and set new one
	dataFrame_ = dataFrame_ & 0xFFFFFFF8;
	dataFrame_ = dataFrame_ | (state & 0x7);
}

uint8_t DataFrame::getCurrentState() {
	return dataFrame_ & 0x7;
}

void DataFrame::onParachuteDeploy() {
	dataFrame_ = dataFrame_ & ~(1 << 3); // for clarity
	dataFrame_ = dataFrame_ | (1 << 3);
}

bool DataFrame::getParechuteState() {
	return (dataFrame_ & 0x8) >> 3;
}

void DataFrame::onBaroError() {
	dataFrame_ = dataFrame_ & ~(1 << 4); 
	dataFrame_ = dataFrame_ | (1 << 4);

	// altitude to be done

}

bool DataFrame::getBaroState() {
	return (dataFrame_ & (1 << 4)) >> 4;
}

void DataFrame::setAltitude(uint16_t altitude) {
	// check baro error to be done

	dataFrame_ = dataFrame_ & ~(0xFFFF << 8);
	dataFrame_ = dataFrame_ | (altitude << 8);
}

uint16_t DataFrame::getAltitude() {
	return (dataFrame_ & (0xFFFF << 8)) >> 8;
}

