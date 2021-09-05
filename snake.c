#include <stdlib.h>
#include <curses.h>
#include "snake.h"
#include <time.h>

int accumulate(int number);

void s_init_colors() {
    init_pair(SC_RED, COLOR_RED, COLOR_BLACK);
    init_pair(SC_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(SC_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(SC_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(SC_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(SC_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(SC_CYAN, COLOR_CYAN, COLOR_BLACK);
}

snake_game *s_init_game(WINDOW *win) {
    srand(time(NULL));
    s_init_colors();
    snake_game *game = malloc(sizeof(snake_game));
    game->direction = KEY_ENTER;
    game->length = INITIAL_SNAKE_LENGTH;
    game->tick_period = TICK_PERIOD;
    game->score = 0;
    game->level = 1;
    game->max_y = getmaxy(win);
    game->max_x = getmaxx(win);
    game->max_length = (game->max_y > game->max_x ? game->max_y : game->max_x);
    game->snake = malloc(sizeof(coordinate) * game->max_length);
    int center_y = game->max_y / 2;
    int center_x = game->max_x / 2;
    for (int i = 0; i < game->length; i++) {
        game->snake[i].y = center_y;
        game->snake[i].x = center_x;
    }
    return game;
}

void s_destroy_game(snake_game *game) {
    free(game->snake);
    free(game);
}

void s_reset_game(snake_game *game) {
    game->length = INITIAL_SNAKE_LENGTH;
    game->score = 0;
    game->level = 1;
    game->direction = KEY_ENTER;
    game->tick_period = TICK_PERIOD;
    int center_y = game->max_y / 2;
    int center_x = game->max_x / 2;
    for (int i = 0; i < game->length; i++) {
        game->snake[i].y = center_y;
        game->snake[i].x = center_x;
    }
}

void s_draw_snake(WINDOW *win, snake_game *game) {
    if (game->direction == KEY_DOWN || game->direction == KEY_UP) {
        mvwaddch(win, game->snake[0].y, game->snake[0].x, ACS_VLINE);
    } else if (game->direction == KEY_LEFT || game->direction == KEY_RIGHT) {
        mvwaddch(win, game->snake[0].y, game->snake[0].x, ACS_HLINE);
    }
    for (int i = 1; i < game->length - 1; i++) {
        if (game->snake[i - 1].x > game->snake[i + 1].x) {
            if (game->snake[i - 1].y > game->snake[i + 1].y) {
                if (game->snake[i].x < game->snake[i - 1].x) {
                    mvwaddch(win, game->snake[i].y, game->snake[i].x, ACS_LLCORNER);
                } else {
                    mvwaddch(win, game->snake[i].y, game->snake[i].x, ACS_URCORNER);
                }
            } else if (game->snake[i - 1].y < game->snake[i + 1].y) {
                if (game->snake[i].x < game->snake[i - 1].x) {
                    mvwaddch(win, game->snake[i].y, game->snake[i].x, ACS_ULCORNER);
                } else {
                    mvwaddch(win, game->snake[i].y, game->snake[i].x, ACS_LRCORNER);
                }
            }
        } else if (game->snake[i - 1].x < game->snake[i + 1].x) {
            if (game->snake[i - 1].y > game->snake[i + 1].y) {
                if (game->snake[i].x > game->snake[i - 1].x) {
                    mvwaddch(win, game->snake[i].y, game->snake[i].x, ACS_LRCORNER);
                } else {
                    mvwaddch(win, game->snake[i].y, game->snake[i].x, ACS_ULCORNER);
                }
            } else if (game->snake[i - 1].y < game->snake[i + 1].y) {
                if (game->snake[i].x > game->snake[i - 1].x) {
                    mvwaddch(win, game->snake[i].y, game->snake[i].x, ACS_URCORNER);
                } else {
                    mvwaddch(win, game->snake[i].y, game->snake[i].x, ACS_LLCORNER);
                }
            }
        } else if (game->snake[i - 1].x == game->snake[i + 1].x) {
            mvwaddch(win, game->snake[i].y, game->snake[i].x, ACS_VLINE);
        } else if (game->snake[i - 1].y == game->snake[i + 1].y) {
            mvwaddch(win, game->snake[i].y, game->snake[i].x, ACS_HLINE);
        }
    }
    if (game->snake[game->length - 1].x == game->snake[game->length - 2].x) {
        mvwaddch(win, game->snake[game->length - 1].y, game->snake[game->length - 1].x, ACS_VLINE);
    } else if (game->snake[game->length - 1].y == game->snake[game->length - 2].y) {
        mvwaddch(win, game->snake[game->length - 1].y, game->snake[game->length - 1].x, ACS_HLINE);
    }
    wnoutrefresh(win);
}

void s_change_direction(snake_game *game, int direction) {
    if (direction == KEY_UP && game->direction != KEY_DOWN) {
        game->direction = KEY_UP;
    } else if (direction == KEY_DOWN && game->direction != KEY_UP) {
        game->direction = KEY_DOWN;
    } else if (direction == KEY_LEFT && game->direction != KEY_RIGHT) {
        game->direction = KEY_LEFT;
    } else if (direction == KEY_RIGHT && game->direction != KEY_LEFT) {
        game->direction = KEY_RIGHT;
    }
}

void s_move_snake(WINDOW *win, snake_game *game) {
    mvwprintw(win, game->snake[game->length - 1].y, game->snake[game->length - 1].x, " ");
    wnoutrefresh(win);
    for (int i = game->length - 1; i > 0; i--) {
        game->snake[i].x = game->snake[i - 1].x;
        game->snake[i].y = game->snake[i - 1].y;
    }
    if (game->direction == KEY_DOWN) {
        game->snake[0].y += 1;
    } else if (game->direction == KEY_UP) {
        game->snake[0].y -= 1;
    } else if (game->direction == KEY_RIGHT) {
        game->snake[0].x += 1;
    } else if (game->direction == KEY_LEFT) {
        game->snake[0].x -= 1;
    }
}

void s_set_food(WINDOW *win, snake_game *game) {
    int collides, y, x;
    do {
        collides = 0;
        y = (rand() % (game->max_y));
        x = (rand() % (game->max_x));
        for (int i = 0; i < game->length; i++) {
            if (x == game->snake[i].x && y == game->snake[i].y) {
                collides = 1;
                break;
            }
        }
    } while (collides);
    game->food.y = y;
    game->food.x = x;
    int color = t_random_color();
    wattr_on(win, COLOR_PAIR(color), NULL);
    mvwaddch(win, y, x, ACS_DIAMOND);
    wattr_off(win, COLOR_PAIR(color), NULL);
    wnoutrefresh(win);
}

int s_check_food(snake_game *game) {
    if (game->snake[0].y == game->food.y && game->snake[0].x == game->food.x) {
        game->score += SCORE_PER_FOOD;
        game->length++;
        return 1;
    }
    return 0;
}

void s_print_score(WINDOW *win, snake_game *game) {
    mvwprintw(win, 0, 0, "Score: %d  Length: %d  level: %d  ", game->score, game->length, game->level);
    wnoutrefresh(win);
}

void s_check_level(snake_game *game) {
    if (game->level >= MAX_LEVEL) {
        return;
    }
    if (game->score >= accumulate(game->level) * SCORE_PER_FOOD * LEVEL_MODIFIER) {
        game->level++;
        t_update_speed(game);
    }
}

int s_check_game_over(snake_game *game) {
    if (game->direction == KEY_ENTER) { return 0; }
    if (game->snake[0].y < 0 || game->snake[0].y > game->max_y
        || game->snake[0].x < 0 || game->snake[0].x > game->max_x) {
        return 1;
    }
    for (int i = 1; i < game->length; i++) {
        if (game->snake[0].y == game->snake[i].y && game->snake[0].x == game->snake[i].x) {
            return 1;
        }
    }
    return 0;
}

void t_update_speed(snake_game *game) {
    game->tick_period = TICK_PERIOD - (game->level - 1) * LEVEL_PERIOD;
}

int t_random_color() {
    return (rand() % NUM_OF_COLORS) + 1;
}

int accumulate(int number) {
    if (number <= 0) { return 1; }
    int sum = 0;
    for (int i = 1; i <= number; i++) {
        sum += i;
    }
    return sum;
}