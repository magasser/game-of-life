#include <stdlib.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <cmocka.h>

#include "../src/game/game.h"

typedef struct {
    game_t* game;
} ts_t;

static int before_all(void** state) {
    ts_t* ts = malloc(sizeof(ts_t));

    *state = ts;
    return 0;
}

static int before(void** state) {
    ts_t* ts = *state;

    size_t height = 3, width = 3;
    uint8_t* cells = calloc(height * width, sizeof(uint8_t));
    
    ts->game = malloc(sizeof(game_t));

    ts->game->cells = cells;
    ts->game->height = height;
    ts->game->width = width;

    return 0;
}

/******************** Core Tests ********************/

static void test_game_create(void** state) {
    /* Arrange */
    ts_t* ts = *state;
    game_t* game = ts->game;

    size_t height = 3, width = 3;

    /* Act */
    game_t* result = game_create(game->cells, height, width);

    /* Assert */
    assert_int_equal(result->height, height);
    assert_int_equal(result->width, width);

    for (uint64_t i = 0; i < width * height; ++i) {
        assert_true(result->cells[i] == game->cells[i]);
    }

    free_game(result);
}

static void test_is_alive(void** state) {
    /* Arrange */
    ts_t* ts = *state;
    game_t* game = ts->game;

    game->cells[0] = ALIVE;

    cell_t alive_cell = { .x = 0, .y = 0 };
    cell_t dead_cell = { .x = 0, .y = 1 };

    /* Act */
    uint8_t result_alive = is_alive(game, alive_cell);
    uint8_t result_dead = is_alive(game, dead_cell);

    /* Assert */
    assert_true(result_alive == ALIVE);
    assert_true(result_dead == DEAD);
}

/******************** End Core Tests ********************/

/******************** Generation Tests ********************/

static void test_alive_cell_with_no_neighbours_dies(void** state) {
    /* Arrange */
    size_t height = 1, width = 1;

    uint8_t* cells = calloc(1, sizeof(uint8_t));

    cells[0] = ALIVE;

    game_t* one_cell_game = game_create(cells, height, width);

    /* Act */
    next_gen(one_cell_game);

    /* Assert */
    assert_true(one_cell_game->cells[0] == DEAD);
    
    free_game(one_cell_game);
}

static void test_alive_cell_with_two_alive_neighbours_survives(void** state) {
    /* Arrange */
    ts_t* ts = *state;
    game_t* game = ts->game;
    
    game->cells[0] = ALIVE;
    game->cells[3] = ALIVE;

    game->cells[4] = ALIVE;

    /* Act */
    next_gen(game);

    /* Assert */
    assert_true(game->cells[4] == ALIVE);
}

static void test_alive_cell_with_three_alive_neighbours_survives(void** state) {
    /* Arrange */
    ts_t* ts = *state;
    game_t* game = ts->game;

    game->cells[0] = ALIVE;
    game->cells[3] = ALIVE;
    game->cells[5] = ALIVE;

    game->cells[4] = ALIVE;

    /* Act */
    next_gen(game);

    /* Assert */
    assert_true(game->cells[4] == ALIVE);
}

static void test_alive_cell_with_more_than_three_alive_neighbours_dies(void** state) {
    /* Arrange */
    ts_t* ts = *state;
    game_t* game = ts->game;

    game->cells[0] = ALIVE;
    game->cells[3] = ALIVE;
    game->cells[5] = ALIVE;
    game->cells[7] = ALIVE;

    game->cells[4] = ALIVE;

    /* Act */
    next_gen(game);

    /* Assert */
    assert_true(game->cells[4] == DEAD);
}

static void test_alive_cell_with_less_than_two_alive_neighbours_dies(void** state) {
    /* Arrange */
    ts_t* ts = *state;
    game_t* game = ts->game;

    game->cells[0] = ALIVE;

    game->cells[4] = ALIVE;

    /* Act */
    next_gen(game);

    /* Assert */
    assert_true(game->cells[4] == DEAD);
}

static void test_dead_cell_with_three_alive_neighbours_becomes_alive(void** state) {
    /* Arrange */
    ts_t* ts = *state;
    game_t* game = ts->game;

    game->cells[0] = ALIVE;
    game->cells[3] = ALIVE;
    game->cells[5] = ALIVE;

    game->cells[4] = DEAD;

    /* Act */
    next_gen(game);

    /* Assert */
    assert_true(game->cells[4] == ALIVE);
}

/******************** End Generation Tests ********************/

static int after(void** state) {
    ts_t* ts = *state;

    free(ts->game->cells);
    free(ts->game);

    return 0;
}

static int after_all(void** state) {
    ts_t* ts = *state;

    free(ts);

    return 0;
}

const struct CMUnitTest core_tests[] = {
    cmocka_unit_test_setup_teardown(test_game_create, before, after),
    cmocka_unit_test_setup_teardown(test_is_alive, before, after),
};

const struct CMUnitTest generation_tests[] = {
    cmocka_unit_test_setup_teardown(test_alive_cell_with_no_neighbours_dies, before, after),
    cmocka_unit_test_setup_teardown(test_alive_cell_with_two_alive_neighbours_survives, before, after),
    cmocka_unit_test_setup_teardown(test_alive_cell_with_three_alive_neighbours_survives, before, after),
    cmocka_unit_test_setup_teardown(test_alive_cell_with_more_than_three_alive_neighbours_dies, before, after),
    cmocka_unit_test_setup_teardown(test_alive_cell_with_less_than_two_alive_neighbours_dies, before, after),
    cmocka_unit_test_setup_teardown(test_dead_cell_with_three_alive_neighbours_becomes_alive, before, after),
};

int main(void) {

    uint64_t nr_failed_tests = 0;

    nr_failed_tests += cmocka_run_group_tests_name("Core", core_tests, before_all, after_all);
    nr_failed_tests += cmocka_run_group_tests_name("Generation", generation_tests, before_all, after_all);

    return nr_failed_tests;
}
