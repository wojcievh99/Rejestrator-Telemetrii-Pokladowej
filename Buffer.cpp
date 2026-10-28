#include "Buffer.h"
#include <iostream>

Buffer& Buffer::instance() {
	static Buffer i;
	return i;
}

void Buffer::setState(uint32_t& dataFrame, uint8_t state) {

	dataFrame = dataFrame & 0xFFFFFFF8;
	dataFrame = dataFrame | (state & 0x7);
	
}

void Buffer::setParachute(uint32_t& dataFrame, bool parachute) {

	dataFrame = dataFrame & ~(1 << 3); 

	// jeśli parachute == false, to odpowiedni bit 
	// jest ustawiony już na zero 
	if (parachute)
		dataFrame = dataFrame | (1 << 3);

}

void Buffer::setBaroError(uint32_t& dataFrame, bool baro_error) {

	dataFrame = dataFrame & ~(1 << 4); 
	
	// jeśli baro_error == false, to odpowiedni bit 
	// jest ustawiony już na zero
	if (baro_error)
		dataFrame = dataFrame | (1 << 4);

}

void Buffer::setAltitude(uint32_t& dataFrame, uint16_t altitude) {

	// czyszczenie 8-23
	dataFrame = dataFrame & ~(0xFFFF << 8);

	// odczyt state na pierwszych 3 bitach
	uint8_t state = dataFrame & (0b111); 

	if (state == 0 or state == 4)
		altitude = 0;
	
	// odczyt baro_error na 4tym bicie
	bool baro_error = (dataFrame & (1 << 4)) >> 4; 

	// baro_error ma pierszeństwo względem state
	if (baro_error == true)
		altitude = 0xFFFF;
	
	// zapisz na bitach 8-23
	dataFrame = dataFrame | (altitude << 8);

}

void Buffer::checkSum(uint32_t& dataFrame) {

	// obliczenie
	uint8_t checksum =
		(dataFrame & 0xFF) ^
		((dataFrame & (0xFF << 8)) >> 8) ^
		((dataFrame & (0xFF << 16)) >> 16);

	// czyszczenie 24-31 i zapisanie
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

	// nadpisuję najstarszy zapis
	cyclicBuffer_[records_ % 16] = df;

	records_++;

}

bool Buffer::getLastDataFrame(
	uint8_t& state, bool& parachute, bool& baro_error, 
	uint16_t& altitude, uint8_t& checksum, uint32_t& df
) 
{
	// nie ma żadnych zapisów w buforze
	if (!records_)
		return false;

	// dataframe:
	df = cyclicBuffer_[(records_ - 1) % 16];

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

	return true;
}

unsigned int Buffer::getBuffer(uint32_t (&buffer)[16]) {
	
	// kopiuję do referencji
	for (int i = 0; i < 16; i++) {
		buffer[i] = cyclicBuffer_[i];
	}

	return (records_ > 16 ? 16 : records_);

}

	