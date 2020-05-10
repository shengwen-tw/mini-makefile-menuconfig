#include <string.h>
#include <curses.h>
#include <stdlib.h>
#include <locale.h>

#define BACKGROUND_PAIR 2
#define CONTEXT_PAIR 3
#define SHADE_PAIR 4

WINDOW *worksp;

void draw_single(int x, int y, char *c, int pair)
{
	wattron(worksp, COLOR_PAIR(pair));
	mvwaddstr(worksp, x, y, c);
	wattroff(worksp, COLOR_PAIR(pair));
}

void draw_column(int x, int y, char *c, int pair, int len)
{
	wattron(worksp, COLOR_PAIR(pair));
	for(int i = 0; i < len; i++) {
		mvwaddstr(worksp, x + i, y, c);
	}
	wattroff(worksp, COLOR_PAIR(pair));
}

void draw_line(int x, int y, char *c, int pair, int len)
{
	wattron(worksp, COLOR_PAIR(pair));
	for(int i = 0; i < len; i++) {
		mvwaddstr(worksp, x, y + i, c);
	}
	wattroff(worksp, COLOR_PAIR(pair));
}

void draw_center_string(int x, char *s, int pair)
{
	int len = strlen(s);
	int y = (COLS - len) / 2;

	wattron(worksp, COLOR_PAIR(pair));
	for(int i = 0; i < len; i++) {
		mvwaddch(worksp, x, y + i, s[i]);
	}
	wattroff(worksp, COLOR_PAIR(pair));
}

void draw_string_block(int x, int y, char *s, int max_str_len, int pair)
{
	int len = strlen(s);
	int total_lines = len / max_str_len + 1;

	wattron(worksp, COLOR_PAIR(pair));
	for(int i = 0; i < total_lines; i++) {
		if(i > 2) {
			return;
		}

		for(int j = 0; j < max_str_len; j++) {
			if(s[i * max_str_len + j] == '\0') {
				return;
			}

			mvwaddch(worksp, x + i, y + j, s[i * max_str_len + j]);
		}
	}
	wattroff(worksp, COLOR_PAIR(pair));
}

void draw_selection_item(int index, char *s, int pair)
{
	int start_y = COLS / 5;

	int len = strlen(s);

	wattron(worksp, COLOR_PAIR(pair));
	for(int i = 0; i < len; i++) {
		mvwaddch(worksp, index + 7, start_y + i, s[i]);
	}
	wattroff(worksp, COLOR_PAIR(pair));
}

void draw_buttons(char *button_names[], int button_cnt, int pair)
{
	int total_s_len = 0;
	for(int i = 0; i < button_cnt; i++) {
		total_s_len += strlen(button_names[i]);
	}
	total_s_len += (button_cnt - 1) * 3; //spaces

	char curr_cols = (COLS - total_s_len) / 2;
	wattron(worksp, COLOR_PAIR(pair));
	for(int i = 0; i < button_cnt; i++) {
		int j = 0;
		for(j = 0; j < strlen(button_names[i]); j++) {
			mvwaddch(worksp, LINES-4, curr_cols+j, button_names[i][j]);
		}
		mvwaddch(worksp, LINES-4, curr_cols+j+1, ' ');
		mvwaddch(worksp, LINES-4, curr_cols+j+2, ' ');
		mvwaddch(worksp, LINES-4, curr_cols+j+3, ' ');
		curr_cols += (strlen(button_names[i]) + 3);
	}
	wattroff(worksp, COLOR_PAIR(pair));
}

void draw_background(void)
{
	/* title in the background */
	char *title = " config.mk - NCRL Flight Controller configuration";
	wattron(worksp, COLOR_PAIR(BACKGROUND_PAIR));
	for(int j = 0; j < COLS; j++) {
		if(j < strlen(title)) {
			mvwaddch(worksp, 0, j, title[j]);
		} else {
			mvwaddch(worksp, 0, j, ' ');
		}
	}
	wattroff(worksp, COLOR_PAIR(BACKGROUND_PAIR));

	draw_line(1, 1, "─", BACKGROUND_PAIR, COLS - 2);
	draw_single(1, 0, " ", BACKGROUND_PAIR);
	draw_single(1, COLS-1, " ", BACKGROUND_PAIR);

	/* left/right background */
	draw_column(2, 0, " ", BACKGROUND_PAIR, COLS - 2);
	draw_column(2, 1, " ", BACKGROUND_PAIR, COLS - 2);
	draw_column(2, COLS-1, " ", BACKGROUND_PAIR, COLS-2);
	draw_column(2, COLS-2, " ", BACKGROUND_PAIR, 1);
	draw_column(2, COLS-3, " ", BACKGROUND_PAIR, 1);

	/* lower background */
	draw_line(LINES-2, 2, " ", BACKGROUND_PAIR, 2);
	draw_line(LINES-1, 2, " ", BACKGROUND_PAIR, COLS - 3);

	/* dialog lower buttons */
	draw_line(LINES-4, 3, " ", CONTEXT_PAIR, COLS - 7);

	/* dialog left border */
	draw_column(3, 2, "│", CONTEXT_PAIR, LINES - 6);
	draw_single(2, 2, "┌", CONTEXT_PAIR);
	draw_single(LINES - 3, 2, "└", CONTEXT_PAIR);

	/* dialog right border */
	draw_column(3, COLS-4, "│", CONTEXT_PAIR, LINES - 6);
	draw_single(2, COLS-4, "┐", CONTEXT_PAIR);
	draw_single(LINES - 3, COLS-4, "┘", CONTEXT_PAIR);

	/* dialog lower border */
	draw_line(LINES-3, 3, "─", CONTEXT_PAIR, COLS - 7);

	/* dialog botton shadow */
	draw_line(LINES-2, 4, " ", SHADE_PAIR, COLS - 6);

	/* dialog inner left border */
	draw_column(6, 3, " ", CONTEXT_PAIR, LINES - 10);
	draw_column(7, 4, "│", CONTEXT_PAIR, LINES - 12);
	draw_single(6, 4, "┌", CONTEXT_PAIR);
	draw_single(LINES-5, 4, "└", CONTEXT_PAIR);

	/* dialog inner right border */
	draw_column(6, COLS-5, " ", CONTEXT_PAIR, LINES - 10);
	draw_column(7, COLS-6, "│", CONTEXT_PAIR, LINES - 12);
	draw_single(6, COLS-6, "┐", CONTEXT_PAIR);
	draw_single(LINES-5, COLS-6, "┘", CONTEXT_PAIR);

	/* dialog inner top border */
	draw_line(6, 5, "─", CONTEXT_PAIR, COLS - 11);

	/* dialog inner lower border */
	draw_line(LINES-5, 5, "─", CONTEXT_PAIR, COLS - 11);

	/* dialog title area */
	draw_line(2, 3, "─", CONTEXT_PAIR, COLS - 7);

	/* description area */
	draw_line(3, 3, " ", CONTEXT_PAIR, COLS - 7);
	draw_line(4, 3, " ", CONTEXT_PAIR, COLS - 7);
	draw_line(5, 3, " ", CONTEXT_PAIR, COLS - 7);

	/* options area */
	for(int i = 0; i < LINES - 12; i ++) {
		draw_line(7 + i, 5, " ", CONTEXT_PAIR, COLS - 11);
	}

	/* draw prompt strings */
	char *dialog_title = "NCRL Flight Controller configuration";
	draw_center_string(2, dialog_title, CONTEXT_PAIR);

	char *dialog_prompt =
		"Arrow keys navigate the menu. <Enter> select submenus --->. "
		"Pressing <Y> enables, <N> disables. Press <Q> to exit. Legends: [*] enabled"
                " [] disabled";
	draw_string_block(3, 5, dialog_prompt, COLS-11, CONTEXT_PAIR);

	draw_selection_item(0, "    UAV ID --->", CONTEXT_PAIR);
	draw_selection_item(1, "    Localization --->", CONTEXT_PAIR);
	draw_selection_item(2, "    Telemetary --->", CONTEXT_PAIR);
	draw_selection_item(3, "    Attitude and Heading Reference System --->", CONTEXT_PAIR);
	draw_selection_item(4, "    Controller --->", CONTEXT_PAIR);
	draw_selection_item(5, "[*] Debug shell --->", CONTEXT_PAIR);

	char *buttons[3] = {"<Save>", "<Default>", "<Exit>"};
	draw_buttons(buttons, 3, CONTEXT_PAIR);

}

int main(void)
{
	setlocale(LC_ALL, "en_US.UTF-8");

	int ch;

	initscr();
	keypad(stdscr, TRUE);
	cbreak();
	noecho();

	if (has_colors() == FALSE) {
		endwin();
		printf("your terminal does not support color\n");
		exit(1);
	}

	start_color();
	init_pair(BACKGROUND_PAIR, COLOR_WHITE, COLOR_BLUE);
	init_pair(CONTEXT_PAIR, COLOR_BLACK, COLOR_WHITE);
	init_pair(SHADE_PAIR, COLOR_BLACK, COLOR_BLACK);

	worksp = subwin(stdscr, LINES, COLS, 0, 0);

	clear();
	draw_background();

	do {
		ch = getch();
	} while ((ch != 'q') && (ch != 'Q'));

	endwin();

	exit(0);
}
