#pragma once

# include < cstdint >

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

	void onBaroError();
	// true = error
	bool getBaroState(); 

	void setAltitude(uint16_t altitude);
	uint16_t getAltitude();

};

