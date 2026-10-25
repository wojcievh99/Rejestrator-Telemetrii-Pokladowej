#pragma once

# include < cstdint >

// cyclic bufer here also

class DataFrame {
	uint32_t dataFrame_ = 0;

	/*
		0-2		state
		3		parachute
		4		baro_error
		5-7		reserved (always 0)
		8-23	altitude (16 bits)
		24-31	checksum
	*/

	uint32_t cyclicBuffer_[16] = { 0 };

	void moveDatainBuffer();
	// has to be called BEFORE any function makes a change
	void onDataFrameChange(uint32_t dataFrameState);

	// has to be called AFTER any change
	void checkSum();

	DataFrame() = default;                  
	~DataFrame() = default;
public:
	static DataFrame& instance();

	DataFrame(const DataFrame&) = delete;
	DataFrame& operator=(const DataFrame&) = delete;


	void setState(uint8_t state);
	uint8_t getCurrentState();

	void onParachuteDeploy();
	bool getParechuteState();

	// set error
	void onBaroError();
	// true = error
	bool getBaroState(); 

	void setAltitude(uint16_t altitude);
	uint16_t getAltitude();

	uint16_t getCurrentDataFrame();
	uint16_t getLastDataFrame();

};

