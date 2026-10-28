
#include <iostream>
#include <bitset>
#include "TelemetryBlackBox.h"

namespace test {

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

}

