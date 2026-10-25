
#include <iostream>
#include <bitset>

#include "TelemetryBlackBox.h"

// Wojciech Niedbała, rekrutacja do Simba Software

/*
    Testy:

    1. getLastDataframe() bez żadnego zapisu
    2. baro_error = true, zapis i getLastDataframe()
    3. state = LANDED, zapis i getLastDataframe()
    4. baro_error = true i state = DISARM, odczyt wysokości
    5. 17 zapisów i odczyt całego buffora kołowego

*/

void test1() {

    TelemetryBlackBox tbb;

    uint32_t response = tbb.getLastDataframe();
    
    std::cout << "\nResponse: " << std::bitset<32>(response) << "\n\n";
}

void test2() {

    TelemetryBlackBox tbb;

    tbb.onBaroReading(true, 98);
    tbb.recordCurrentState();

    uint32_t response = tbb.getLastDataframe();
    
    std::cout << "\nResponse: " << std::bitset<32>(response) << "\n\n";

}

void test3() {

    TelemetryBlackBox tbb;

    tbb.onBaroReading(false, 32);
    tbb.onRocketStateChange(LANDED);

    tbb.recordCurrentState();

    uint32_t response = tbb.getLastDataframe();

    std::cout << "\nResponse: " << std::bitset<32>(response) << "\n\n";

}

void test4() {

    TelemetryBlackBox tbb;

    tbb.onBaroReading(true, 32);
    tbb.onRocketStateChange(DISARM);

    tbb.recordCurrentState();

    uint32_t response = tbb.getLastDataframe();

    std::cout << "\nResponse: " << std::bitset<32>(response) << "\n\n";

}

void test5() {

    TelemetryBlackBox tbb;

    // 16 takich samych
    for (int i = 0; i < 16; i++) {

        tbb.onBaroReading(false, 132);
        tbb.onRocketStateChange(FLIGHT);

        tbb.recordCurrentState();

    }

    // 17sty inny
    tbb.onBaroReading(true, 32);

    tbb.recordCurrentState();

    uint32_t response = tbb.getLastDataframe();

    tbb.getAllRecords();

    std::cout << "\nResponse: " << std::bitset<32>(response) << "\n\n";

}


int main()
{
    bool testing_active = true;

    while (testing_active) {
        std::cout << 
            "Testing module. Select number from <1; 5> or 0 to exit.\n";

        unsigned int i;
        std::cin >> i;

        std::cout << "\n";

        switch (i)
        {
        case 0:
            testing_active = false;
            break;
        case 1:
            test1();
            break;
        case 2:
            test2();
            break;
        case 3:
            test3();
            break;
        case 4:
            test4();
            break;
        case 5:
            test5();
            break;
        default:
            std::cout << "Unknown test.";
        }

    }

    return 0;
}
