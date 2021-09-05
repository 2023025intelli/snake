#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include "snake.h"
#include <menu.h>

typedef struct {
    char *title;
    void *usr_ptr;
} s_menu_item;

void init_ncurses();

void destroy_ncurses();

void init_windows(int max_y, int max_x);

void game();

void pause_game(snake_game *game);

void restart_game(snake_game *game);

void stop_game(snake_game *game);

void show_menu(snake_game *game);

void msleep(int milliseconds);

WINDOW *sw_main;
WINDOW *sw_score;
int running = 1;
int game_over = 0;
int paused = 0;

int main(int argc, char *argv[]) {
    init_ncurses();
    int row, col;
    getmaxyx(stdscr, row, col);
    init_windows(row, col);
    game();
    destroy_ncurses();
    return 0;
}

void game() {
    snake_game *s_game = s_init_game(sw_main);
    s_set_food(sw_main, s_game);
    s_print_score(sw_score, s_game);
    s_draw_snake(sw_main, s_game);
    int c, ticks = 0, changed = 0;
    while (running) {
        switch ((c = getch())) {
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_UP:
            case KEY_DOWN:
                if (!paused && !changed) {
                    s_change_direction(s_game, c);
                    changed = 1;
                }
                break;
            case ' ':
                paused = !paused;
                pause_game(s_game);
                break;
            case 'q':
                return;
            default:
                break;
        }
        if (game_over) {
            show_menu(s_game);
        }
        if (paused) {
            continue;
        }
        if (ticks >= s_game->tick_period / SLEEP_DURATION) {
            changed = 0;
            ticks = 0;
            s_move_snake(sw_main, s_game);
            s_draw_snake(sw_main, s_game);
            if (s_check_game_over(s_game)) {
                game_over = 1;
                continue;
            }
            if (s_check_food(s_game)) {
                s_set_food(sw_main, s_game);
                s_check_level(s_game);
            };
        }
        ticks++;
        s_print_score(sw_score, s_game);
        doupdate();
        msleep(SLEEP_DURATION);
    }
    s_destroy_game(s_game);
}

void init_windows(int max_y, int max_x) {
    wclear(stdscr);
    WINDOW *sw_score_border = newwin(3, max_x, 0, 0);
    WINDOW *sw_main_border = newwin(max_y - 3, max_x, 3, 0);
    sw_score = newwin(1, max_x - 2, 1, 1);
    sw_main = newwin(max_y - 5, max_x - 2, 4, 1);
    refresh();
    box(sw_main_border, 0, 0);
    box(sw_score_border, 0, 0);
    wrefresh(sw_main_border);
    wrefresh(sw_score_border);
    wrefresh(sw_main);
    wrefresh(sw_score);
}

void init_ncurses() {
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    start_color();
}

void destroy_ncurses() {
    nocbreak();
    wclear(stdscr);
    endwin();
}

void show_menu(snake_game *game) {
    int active = 1;
    nodelay(stdscr, FALSE);
    s_menu_item m_items[] = {
        {" play again ", restart_game},
        {"    exit    ", stop_game}
    };
    int n_menu_items = sizeof(m_items) / sizeof(m_items[0]);
    WINDOW *sw_menu = newwin(n_menu_items + 2, 14, game->max_y / 2 + 1, game->max_x / 2 - 6);
    ITEM **items = (ITEM **) calloc(n_menu_items + 1, sizeof(ITEM *));
    for (int i = 0; i < n_menu_items; ++i) {
        items[i] = new_item(m_items[i].title, NULL);
        set_item_userptr(items[i], m_items[i].usr_ptr);
    }
    items[n_menu_items] = (ITEM *) NULL;
    MENU *menu = new_menu(items);
    keypad(sw_menu, TRUE);
    set_menu_win(menu, sw_menu);
    set_menu_sub(menu, derwin(sw_menu, n_menu_items, 12, 1, 1));
    set_menu_mark(menu, "");
    box(sw_menu, 0, 0);
    refresh();
    post_menu(menu);
    wrefresh(sw_menu);
    int c;
    while (active && (c = getch())) {
        switch (c) {
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
            case 10: { // Enter key pressed
                ITEM *cur = current_item(menu);
                ((void (*)(snake_game *)) item_userptr(cur))(game);
                pos_menu_cursor(menu);
                active = 0;
                break;
            }
            default:
                break;
        }
        wrefresh(sw_menu);
    }
    unpost_menu(menu);
    free_item(items[0]);
    free_item(items[1]);
    free_menu(menu);
    delwin(sw_menu);
    nodelay(stdscr, TRUE);
}

void restart_game(snake_game *game) {
    wclear(sw_main);
    wclear(sw_score);
    s_reset_game(game);
    s_set_food(sw_main, game);
    game_over = 0;
    running = 1;
}

void pause_game(snake_game *game) {
    if (paused) {
        mvwprintw(sw_main, game->max_y / 2 - 1, game->max_x / 2 - 2, "PAUSE");
        wrefresh(sw_main);
    } else {
        mvwprintw(sw_main, game->max_y / 2 - 1, game->max_x / 2 - 2, "     ");
        wrefresh(sw_main);
    }
}

void stop_game(snake_game *game) {
    running = 0;
}

void msleep(int milliseconds) {
    usleep(milliseconds * 1000);
}