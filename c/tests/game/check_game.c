#include <check.h>

#include "../src/game/game.h"

START_TEST(test_create_game) {
    /* Arrange */
    size_t height, width = 100;

    /* Act */
    game_t* result = create_game(height, width);

    /* Assert */
    ck_assert_uint_eq(result->height, height);
    ck_assert_uint_eq(result->width, width);

    free_game(result);
} END_TEST

int main(void) {
    return 0;
}