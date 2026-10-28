#pragma once

namespace test {

    // 1. getLastDataframe() bez żadnego zapisu
    void test1();

    // 2. baro_error = true, zapis i getLastDataframe()
    void test2();

    // 3. state = LANDED, zapis i getLastDataframe()
    void test3();

    // 4. baro_error = true i state = DISARM, odczyt wysokości
    void test4();

    // 5. 17 zapisów i odczyt całego buffora kołowego
    void test5();

}
