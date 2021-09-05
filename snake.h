#ifndef SNAKE_SNAKE_H
#define SNAKE_SNAKE_H

#define SLEEP_DURATION 10
#define TICK_PERIOD 120
#define MAX_LEVEL 10
#define LEVEL_MODIFIER 10
#define LEVEL_PERIOD 10
#define SCORE_PER_FOOD 1
#define INITIAL_SNAKE_LENGTH 4
#define NUM_OF_COLORS 7

typedef struct {
    int y;
    int x;
} coordinate;

typedef struct {
    coordinate *snake;
    coordinate food;
    int tick_period;
    int length;
    int level;
    int max_length;
    int direction;
    int score;
    int max_y;
    int max_x;
} snake_game;

typedef enum {
    SC_NONE = 0,
    SC_RED = 1,
    SC_WHITE = 2,
    SC_BLUE = 3,
    SC_GREEN = 4,
    SC_YELLOW = 5,
    SC_MAGENTA = 6,
    SC_CYAN = 7
} snake_color;

void s_init_colors();

snake_game *s_init_game(WINDOW *win);

void s_destroy_game(snake_game *game);

void s_reset_game(snake_game *game);

void s_draw_snake(WINDOW *win, snake_game *game);

void s_change_direction(snake_game *game, int direction);

void s_move_snake(WINDOW *win, snake_game *game);

void s_set_food(WINDOW *win, snake_game *game);

int s_check_food(snake_game *game);

void s_print_score(WINDOW *win, snake_game *game);

void s_check_level(snake_game *game);

int s_check_game_over(snake_game *game);

void t_update_speed(snake_game *game);

int t_random_color();

#endif //SNAKE_SNAKE_H
