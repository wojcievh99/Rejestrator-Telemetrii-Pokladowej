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


	// @param dataFrame - zapisuje się w niej wartość 
	//	określona przez drugi argument
	// @param state, parachute etc. - wartość do zapisu
	// @note funkcję czyszczą podaną w referencji ramkę
	//	(każda na swoich miejscach) dla pewności i przejrzystości
	// @warning niektóre dane (altitude, checksum) sprawdzają 
	//	też dane je poprzedzające, aby mieć pewność,
	//	że ramka jest poprawnie zakodowana,
	//	więc należy używać w tej kolejności:
	void setState(uint32_t& dataFrame, uint8_t state);
	void setParachute(uint32_t& dataFrame, bool parachute);
	void setBaroError(uint32_t& dataFrame, bool baro_error);
	void setAltitude(uint32_t& dataFrame, uint16_t altitude);
	void checkSum(uint32_t& dataFrame);

	// @brief wyłączam konstruktor, destruktor (wzorzec singleton)
	Buffer() = default;
	~Buffer() = default;

public:
	// @brief dostęp do klasy przez instancję static (wzorzec singleton)
	static Buffer& instance();

	// @brief zakaz kopiowania (wzorzec singleton)
	Buffer(const Buffer&) = delete;
	Buffer& operator=(const Buffer&) = delete;

	// @brief sprawdza czy w podanej ramce
	//	checksum (ostatnie 8 bitów) jest poprawnie zakodowane
	// @return flase, jeśli checksum jest niepoprawne 
	//	lub true jeśli jest poprawne
	bool validateChecksum(uint32_t dataFrame);

	// @brief tworzy ramkę z podanych argumentów 
	//	(wg. schematu na górze klasy) i zapisuje do
	//	bufora kołowego
	void recordDataFrame(uint8_t state, bool parachute,
		bool baro_error, uint16_t altitude);

	// @brief zapisuje w podane referencje odpowiadające im dane 
	//	z ostatniej zapisanej w buforze ramki
	// @param df - cała ramka jako 32 bitowa liczba
	// @return false jeśli nic jeszcze nie zapisano lub
	//	true jeśli jest coś zapisane
	bool getLastDataFrame(
		uint8_t& state, bool& parachute, bool& baro_error, 
		uint16_t& altitude, uint8_t& checksum, uint32_t& df
	);

	// @brief zwraca w referencji kopię aktualnego bufora kołowego (bez dekodowania)
	// @return liczbę elementów w buforze
	unsigned int getBuffer(uint32_t (&buffer)[16]);

};

