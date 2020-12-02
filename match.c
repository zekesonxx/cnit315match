#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <card.h>
#define true 1
#define false 0

int play_game(int numcards, int unfun_mode) {
	int c, i, clicked = -1;
	// how many times the player has clicked on a card
	int clicks = 0;
	// how many matches are still in play
	int remaining=numcards/2;
	// game timer, in tenths of a second
	int timer=0;
	MEVENT event;
	CARD* cards;
	cards = init_cards(numcards);
	halfdelay(1);
	// The game
	while (remaining != 0) {
		clear();
		mvprintw(0, 0, "Matching Game!");
		mvprintw(1, 0, "Remaining matches: %d", remaining);
		mvprintw(2, 0, "Clicks taken: %d", clicks);
		mvprintw(3, 0, "Timer: %02d:%02d", (timer/10)/60, (timer/10)%60);
		draw_cards(cards, numcards);
		refresh();
		
		c = getch();
		if (c == ERR) {
			//This is a timer call from halfdelay
			timer += 1;
		} else if (c == KEY_MOUSE && getmouse(&event) == OK) {
			//Mouse clicked somewhere
			i = handle_potential_collision(cards, numcards, event.x, event.y);
			if (i != -1) {
				//Card was clicked
				clicks++;
				if (clicked == -1) {
					//This is the first card that was clicked, make note of it
					clicked = i;
				} else if (clicked != i) { //Only compare if they didn't click the same card again
					//Let them see what they clicked
					draw_cards(cards, numcards);
					refresh();
					napms(500);
					timer += 5;
					//This is the second card that was clicked, see if we matched
					if (cards[clicked].value == cards[i].value) {
						// They matched!
						cards[clicked].hidden = true;
						cards[i].hidden = true;
						remaining--;
						if (unfun_mode) {
							//shuffle the deck every match
							draw_cards(cards, numcards);
							refresh();
							napms(500);
							timer += 5;
							shuffle_cards(cards, numcards);
						}
					} else {
						// No match, hide and move on
						cards[clicked].visible = false;
						cards[i].visible = false;
					}
					// Reset the clicked value
					clicked = -1;
				}
			}
		}
	}
	
	free(cards);
	
	nocbreak(); //disable halfdelay
	keypad(stdscr, TRUE);
	cbreak();
	
	// Post game
	clear();
	
	mvprintw(1, 1, "You won!");
	if (timer > 600) {
		//more than a minute
		mvprintw(2, 1, "%d clicks in %d minutes %d seconds", (timer/10)/60, (timer/10)%60);
	} else {
		//less than a minute
		mvprintw(2, 1, "%d clicks in %d minutes %d seconds", (timer/10)%60);
	}
	mvprintw(5, 1, "Play again?");
	attron(A_REVERSE);
	mvprintw(6, 1, " YES ");
	mvprintw(6, 7, " NO ");
	attroff(A_REVERSE);
	refresh();
	do {
		c = getch();
		if (c == KEY_MOUSE && getmouse(&event) == OK && event.y == 6) {
			if (event.x >= 1 && event.x <= 6) {
				//YES
				return 1;
			} else if (event.x >= 7 && event.x <= 11) {
				//NO
				return 0;
			}
		}
		refresh();
	} while (1);
	
}

int main() {
	int c, again = 1, mode = 0;
	MEVENT event;

	initscr();
	clear();
	//start_color();
	keypad(stdscr, TRUE);
	cbreak(); //Disable line buffering
	noecho();
	curs_set(0);
	
	mousemask(ALL_MOUSE_EVENTS, NULL);
	
	while (again == 1) {
		clear();
		mvprintw(1, 1, "A card matching game.");
		mvprintw(2, 1, "Choose a difficulty: ");
		attron(A_REVERSE);
		mvprintw(6,  1, "  TOO EASY ");
		mvprintw(8,  1, "    EASY   ");
		mvprintw(10, 1, "   MEDIUM  ");
		mvprintw(12, 1, "    HARD   ");
		mvprintw(14, 1, "  NOT FUN  ");
		attroff(A_REVERSE);
		
		do {
			c = getch();
			// See if the user clicked one of the buttons, or it's some other event
			if (c == KEY_MOUSE && getmouse(&event) == OK && event.x >= 1 && event.x <= 11) {
				switch (event.y) {
					case 6:
					case 8:
					case 10:
					case 12:
					case 14:
						// Set the mode, which will break us out of the loop
						mode = event.y;
						break;
				}
			}
			refresh();
		} while (mode == 0);
		switch (mode) {
			case 6:
				again = play_game(6, 0);
				break;
			case 8:
				again = play_game(16, 0);
				break;
			case 10:
				again = play_game(24, 0);
				break;
			case 12:
				again = play_game(40, 0);
				break;
			case 14:
				again = play_game(40, 1);
				break;
		}
	}

	endwin();
	return 0;
}

