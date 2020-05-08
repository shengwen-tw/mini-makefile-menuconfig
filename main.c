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

void draw_background(void)
{
	/* title in the background */
	char *title = " proj_config.mk - NCRL Flight Controller configuration";
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
	draw_line(7, 5, " ", CONTEXT_PAIR, COLS - 11);
	draw_line(8, 5, " ", CONTEXT_PAIR, COLS - 11);
	draw_line(9, 5, " ", CONTEXT_PAIR, COLS - 11);
	draw_line(10, 5, " ", CONTEXT_PAIR, COLS - 11);
	draw_line(11, 5, " ", CONTEXT_PAIR, COLS - 11);
	draw_line(12, 5, " ", CONTEXT_PAIR, COLS - 11);
	draw_line(13, 5, " ", CONTEXT_PAIR, COLS - 11);
	draw_line(14, 5, " ", CONTEXT_PAIR, COLS - 11);
	draw_line(15, 5, " ", CONTEXT_PAIR, COLS - 11);
	draw_line(16, 5, " ", CONTEXT_PAIR, COLS - 11);
	draw_line(17, 5, " ", CONTEXT_PAIR, COLS - 11);
	draw_line(18, 5, " ", CONTEXT_PAIR, COLS - 11);
	draw_line(19, 5, " ", CONTEXT_PAIR, COLS - 11);
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
