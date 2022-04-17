#include "../tests.h"

#include "../src/game/game.h"

static int setup(void** state) {
    size_t height = 3, width = 3;
    uint8_t* cells = calloc(height * width, sizeof(uint8_t));
    
    game_t* game = malloc(sizeof(game_t));

    game->cells = cells;
    game->height = height;
    game->width = width;

    *state = game;

    return 0;
}

/******************** Core Tests ********************/

static void test_game_create(void** state) {
    /* Arrange */

    size_t height = 3, width = 3;
    uint8_t* cells = calloc(height * width, sizeof(uint8_t));

    /* Act */
    game_t* result = game_create(cells, height, width);

    /* Assert */
    assert_int_equal(result->height, height);
    assert_int_equal(result->width, width);

    for (uint64_t i = 0; i < width * height; ++i) {
        assert_true(result->cells[i] == cells[i]);
    }

    free_game(result);
}

static void test_is_alive(void** state) {
    /* Arrange */
    game_t* game = *state;

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
    game_t* game = *state;
    
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
    game_t* game = *state;

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
    game_t* game = *state;

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
    game_t* game = *state;

    game->cells[0] = ALIVE;

    game->cells[4] = ALIVE;

    /* Act */
    next_gen(game);

    /* Assert */
    assert_true(game->cells[4] == DEAD);
}

static void test_dead_cell_with_three_alive_neighbours_becomes_alive(void** state) {
    /* Arrange */
    game_t* game = *state;

    game->cells[0] = ALIVE;
    game->cells[3] = ALIVE;
    game->cells[5] = ALIVE;

    game->cells[4] = DEAD;

    /* Act */
    next_gen(game);

    /* Assert */
    assert_true(game->cells[4] == ALIVE);
}

static void test_next_gen_updates_correct_gen_info(void** state) {
    /* Arrange */
    game_t* game = *state;

    game->cells[1] = ALIVE;
    game->cells[3] = ALIVE;
    game->cells[4] = ALIVE;
    game->cells[5] = ALIVE;
    game->cells[7] = ALIVE;

    gen_stat_t before = {
        .alive = 5,
        .gen = 1,
        .died = 0,
        .revived = 0,
    };

    game->gen = before;

    /* Act */
    next_gen(game);

    gen_stat_t result = game->gen;

    /* Assert */
    assert_int_equal(result.alive, 8);
    assert_int_equal(result.gen, 2);
    assert_int_equal(result.died, 1);
    assert_int_equal(result.revived, 4);
}

/******************** End Generation Tests ********************/

static int teardown(void** state) {
    game_t* game = *state;

    free(game->cells);
    free(game);

    return 0;
}

uint32_t run_game_tests(void) {

    const struct CMUnitTest game_tests[] = {
        cmocka_unit_test_setup_teardown(test_game_create, setup, teardown),
        cmocka_unit_test_setup_teardown(test_is_alive, setup, teardown),
        cmocka_unit_test_setup_teardown(test_alive_cell_with_no_neighbours_dies, setup, teardown),
        cmocka_unit_test_setup_teardown(test_alive_cell_with_two_alive_neighbours_survives, setup, teardown),
        cmocka_unit_test_setup_teardown(test_alive_cell_with_three_alive_neighbours_survives, setup, teardown),
        cmocka_unit_test_setup_teardown(test_alive_cell_with_more_than_three_alive_neighbours_dies, setup, teardown),
        cmocka_unit_test_setup_teardown(test_alive_cell_with_less_than_two_alive_neighbours_dies, setup, teardown),
        cmocka_unit_test_setup_teardown(test_dead_cell_with_three_alive_neighbours_becomes_alive, setup, teardown),
        cmocka_unit_test_setup_teardown(test_next_gen_updates_correct_gen_info, setup, teardown),
    };

    return cmocka_run_group_tests_name("Game Tests", game_tests, NULL, NULL);
}
