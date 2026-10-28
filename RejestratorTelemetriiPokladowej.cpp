
#include <iostream>
#include <bitset>

#include "TelemetryBlackBox.h"
#include "tests.h"

// Wojciech Niedbała, rekrutacja do Simba Software

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
            test::test1();
            break;
        case 2:
            test::test2();
            break;
        case 3:
            test::test3();
            break;
        case 4:
            test::test4();
            break;
        case 5:
            test::test5();
            break;
        default:
            std::cout << "Unknown test.";
        }

    }

    return 0;
}
