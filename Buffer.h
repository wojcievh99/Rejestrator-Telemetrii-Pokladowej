#pragma once

# include < cstdint >

// cyclic bufer 
// here are all bit operations 

class Buffer {

	uint32_t cyclicBuffer_[16] = { 0 };
	unsigned int records_ = 0;

	/*
		0-2		state
		3		parachute
		4		baro_error
		5-7		reserved (always 0)
		8-23	altitude (16 bits)
		24-31	checksum
	*/

	// it is important to use those in this order:
	void setState(uint32_t& dataFrame, uint8_t state);
	void setParachute(uint32_t& dataFrame, bool parachute);
	void setBaroError(uint32_t& dataFrame, bool baro_error);
	void setAltitude(uint32_t& dataFrame, uint16_t altitude);
	void checkSum(uint32_t& dataFrame);

	Buffer() = default;
	~Buffer() = default;
public:
	static Buffer& instance();

	Buffer(const Buffer&) = delete;
	Buffer& operator=(const Buffer&) = delete;

	bool validateChecksum(uint32_t dataFrame);

	void recordDataFrame(uint8_t state, bool parachute,
		bool baro_error, uint16_t altitude);

	// decoding also
	/*
	@Return
		false jeśli buffer jest pusty
		true jeśli jest coś co można odczytać
	*/
	bool getLastDataFrame(
		uint8_t& state, bool& parachute, bool& baro_error, 
		uint16_t& altitude, uint8_t& checksum, uint32_t& df
	);

};

