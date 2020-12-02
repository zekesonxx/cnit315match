#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <card.h>
#define true 1
#define false 0

int play_game(int numcards) {
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
	
	nocbreak(); //disable halfdelay
	
	// Post game
	
    
}

int main() {

	initscr();
	clear();
	//start_color();
	keypad(stdscr, TRUE);
	cbreak(); //Disable line buffering
	noecho();
	curs_set(0);
	
	mousemask(ALL_MOUSE_EVENTS, NULL);
	
	
	
    play_game(34);

	endwin();
	return 0;
}

