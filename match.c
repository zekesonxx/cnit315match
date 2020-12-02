#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <card.h>
#define true 1
#define false 0

int play_game(int numcards) {
    int c, i, clicks = 0, clicked = -1, remaining=numcards/2;
	MEVENT event;
	CARD* cards;
    cards = init_cards(numcards);
    
    // The game
	while (remaining != 0) {
        clear();
        mvprintw(0, 0, "Matching Game!");
        mvprintw(1, 0, "Remaining matches: %d", remaining);
        mvprintw(2, 0, "Clicks taken: %d", clicks);
        draw_cards(cards, numcards);
		refresh();
        c = getch();
		if (c == KEY_MOUSE && getmouse(&event) == OK) {
			//Mouse clicked somewhere
            i = handle_potential_collision(cards, numcards, event.x, event.y);
            if (i != -1) {
                clicks++;
                //Card was clicked
                if (clicked == -1) {
                    //This is the first card that was clicked, make note of it
                    clicked = i;
                } else if (clicked != i) { //Only compare if they didn't click the same card again
                    //Let them see what they clicked
                    draw_cards(cards, numcards);
                    refresh();
                    sleep(1);
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
			}
		}
	}
	
	// Post game
	
    
}

int main() {

	initscr();
	clear();
	//start_color();
	keypad(stdscr, TRUE);
	cbreak(); //Disable line buffering
	noecho();

	printw("Card matching!");
	refresh();
	
	mousemask(ALL_MOUSE_EVENTS, NULL);
    
    play_game(6);

	endwin();
	return 0;
}

