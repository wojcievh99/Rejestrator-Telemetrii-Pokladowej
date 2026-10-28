# pragma once
# include < cstdint >

enum RocketState : uint8_t {
	DISARM = 0,
	ARM = 1,
	FLIGHT = 2,
	PARACHUTE = 3,
	LANDED = 4
};

class TelemetryBlackBox {

	RocketState state_{DISARM};

	bool parachute_ = false; 
	bool baro_error_ = false;

	uint16_t altitude_ = 0;

public:
	void onRocketStateChange(RocketState state);
	void onParachuteDeployed();
	/**
	* @param error - Informuje czy wystąpił błąd barometru
	* @param value - Odczyt wysoko ści w stopach
	*/
	void onBaroReading(bool error, uint16_t value);
	/**
	* @brief Pobiera bieżący stan , składa go w 32 - 
	bitow ą ramk ę z sumą kontroln ą
	i zapisuje do bufora .
	*/
	void recordCurrentState();
	/**
	* @brief Funkcja pobiera ostatni ą ramk ę z bufora kołowego , 
	dekoduje ją oraz
	loguje zdekodowane warto ści w czytelny spos ób na konsol ę.
	* @Return Ostatnia ramka zapisana do bufora , 
	jeśli xor się zgadza .
	0 xFFFFFFFF jeśli suma kontrolna jest niepoprawna .
	0 jeśli nie ma żadnych zapisów w buforze
	*/
	uint32_t getLastDataframe();

	// @brief loguje na konsolę wszystkie zapisy z buffora
	// bez dekodowania
	void getAllRecords();
	
};
