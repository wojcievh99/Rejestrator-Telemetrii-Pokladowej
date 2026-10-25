#pragma once

# include < cstdint >

// cyclic bufer 

class Buffer {

	uint32_t cyclicBuffer_[16] = { 0 };

	/*
		0-2		state
		3		parachute
		4		baro_error
		5-7		reserved (always 0)
		8-23	altitude (16 bits)
		24-31	checksum
	*/

	void makeSpaceInBuffer();

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

	void recordState(uint8_t state, bool parachute,
		bool baro_error, uint16_t altitude);

};

