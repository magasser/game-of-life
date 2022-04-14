#include <stdlib.h>
#include <check.h>

#include "../src/game/game.h"

game_t* game;

void setup(void) {
    size_t height = 10, width = 10;
    uint8_t* cells = calloc(height * width, sizeof(uint8_t));

    cells[0] = 1;

    game = malloc(sizeof(game_t));

    game->cells = cells;
    game->height = height;
    game->width = width;
}

void teardown(void) {
    free(game->cells);
    free(game);
}

/*********** Core Tests **********/
START_TEST(test_game_create) {
    /* Arrange */
    size_t height = 10, width = 10;

    /* Act */
    game_t* result = game_create(height, width);

    /* Assert */
    ck_assert_uint_eq(result->height, height);
    ck_assert_uint_eq(result->width, width);

    for (uint64_t i = 0; i < width * height; ++i) {
        ck_assert(result->cells[i] == 0);
    }

    free_game(result);
} END_TEST

START_TEST(test_is_alive) {
    /* Arrange */
    game->cells[0] = ALIVE;

    cell_t alive_cell = { .x = 0, .y = 0 };
    cell_t dead_cell = { .x = 0, .y = 1 };

    /* Act */
    uint8_t result_alive = is_alive(game, alive_cell);
    uint8_t result_dead = is_alive(game, dead_cell);

    /* Assert */
    ck_assert(result_alive == ALIVE);
    ck_assert(result_dead == DEAD);
} END_TEST
/*********** End Core Tests **********/

/*********** Generation Tests **********/
START_TEST(test_cell_with_no_neighbours_dies) {
    /* Arrange */
    size_t height = 1, width = 1;
    game_t* game = game_create(height, width);

    game->cells[0] = ALIVE;

    /* Act */
    next_generation(game);

    /* Assert */
    ck_assert(game->cells[0] == DEAD);
} END_TEST
/*********** End Generation Tests **********/

Suite* game_suite(void) {
    Suite* suite;
    TCase* tc_core;
    TCase* tc_generation;

    suite = suite_create("Game");

    tc_core = tcase_create("Core");
    tc_generation = tcase_create("Generation");

    tcase_add_checked_fixture(tc_core, setup, teardown);

    tcase_add_test(tc_core, test_game_create);
    tcase_add_test(tc_core, test_is_alive);

    tcase_add_test(tc_generation, test_cell_with_no_neighbours_dies);

    suite_add_tcase(suite, tc_core);
    suite_add_tcase(suite, tc_generation);

    return suite;
}

int main(void) {
    int number_failed;
    Suite* suite;
    SRunner* srunner;

    suite = game_suite();
    srunner = srunner_create(suite);

    srunner_run_all(srunner, CK_NORMAL);
    number_failed = srunner_ntests_failed(srunner);
    srunner_free(srunner);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
