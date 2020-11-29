#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <card.h>
#define true 1
#define false 0

int main() {
	int c, i, j;
	MEVENT event;
	CARD cards[NUM_CARDS];

	initscr();
	clear();
	//start_color();
	keypad(stdscr, TRUE);
	cbreak(); //Disable line buffering
	noecho();

	printw("Card matching!");
	refresh();
	
	mousemask(ALL_MOUSE_EVENTS, NULL);

	init_card(&cards[0], 0, 0, 'A', false);
	init_card(&cards[1], 1, 0, 'B', false);
	init_card(&cards[2], 2, 0, 'C', false);
	init_card(&cards[3], 3, 0, 'D', false);
	init_card(&cards[4], 0, 1, 'E', false);
	init_card(&cards[5], 1, 1, 'F', false);
	init_card(&cards[6], 2, 1, 'G', false);
	init_card(&cards[7], 3, 1, 'H', false);
	init_card(&cards[8], 0, 2, 'I', false);
	init_card(&cards[9], 1, 2, 'J', false);
	init_card(&cards[10], 2, 2, 'K', false);
	init_card(&cards[11], 3, 2, 'L', false);
	while (true) {
		c = getch();
		if (c == KEY_MOUSE && getmouse(&event) == OK) {
			//Mouse clicked somewhere
			mvprintw(2, 0, "Mouse clicked at %d %d", event.x+1, event.y+1);
			for (i = 0; i<NUM_CARDS; i++) {
				handle_potential_collision(&cards[i], event.x, event.y);
			}
		}
		for (i = 0; i<NUM_CARDS; i++) {
			draw_card(&cards[i]);
		}
		refresh();
	}
	endwin();
	return 0;
}

