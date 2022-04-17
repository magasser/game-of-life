#include "tests.h"

uint32_t main(void) {
    uint64_t nr_failed_tests = 0;

    nr_failed_tests += run_game_tests();

    return nr_failed_tests;
}