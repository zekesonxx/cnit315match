#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <card.h>
#define true 1
#define false 0

int main() {
	int c, i, clicked = -1, remaining=NUM_CARDS/2;
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

	init_card(&cards[0], 0, 0, 'B', false, false);
	init_card(&cards[1], 1, 0, 'A', false, false);
	init_card(&cards[2], 2, 0, 'C', false, false);
	init_card(&cards[3], 3, 0, 'D', false, false);
	init_card(&cards[4], 0, 1, 'K', false, false);
	init_card(&cards[5], 1, 1, 'B', false, false);
	init_card(&cards[6], 2, 1, 'A', false, false);
	init_card(&cards[7], 3, 1, 'D', false, false);
	init_card(&cards[8], 0, 2, 'C', false, false);
	init_card(&cards[9], 1, 2, 'L', false, false);
	init_card(&cards[10], 2, 2, 'K', false, false);
	init_card(&cards[11], 3, 2, 'L', false, false);
	while (true) {
		c = getch();
		if (c == KEY_MOUSE && getmouse(&event) == OK) {
			//Mouse clicked somewhere
			mvprintw(2, 0, "Mouse clicked at %d %d, clicked=%d           ", event.x+1, event.y+1, clicked);
			for (i = 0; i<NUM_CARDS; i++) {
				if (handle_potential_collision(&cards[i], event.x, event.y)) {
                    mvprintw(1, 0, "Mouse clicked on %d", i);
                    //Card was clicked
                    if (clicked == -1) {
                        //This is the first card that was clicked, make note of it
                        clicked = i;
                    } else if (clicked != i) { //Only compare if they didn't click the same card again
                        //Let them see what they clicked
                        draw_card(&cards[i]);
                        refresh();
                        sleep(2);
                        //This is the second card that was clicked, see if we matched
                        if (cards[clicked].value == cards[i].value) {
                            // They matched!
                            cards[clicked].hidden = true;
                            cards[i].hidden = true;
                            remaining--;
                        } else {
                            // No match, hide and move on
                            cards[clicked].visible = false;
                            cards[i].visible = false;
                        }
                        // Reset the clicked value
                        clicked = -1;
                    }
                    //You can't click multiple cards at once
                    break;
                }
			}
		}
		mvprintw(0, 16, "Remaining: %d  ", remaining);
		for (i = 0; i<NUM_CARDS; i++) {
			draw_card(&cards[i]);
		}
		refresh();
        if (remaining == 0) {
            break;
        }
	}
	endwin();
	return 0;
}

