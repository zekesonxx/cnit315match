#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#define true 1
#define false 0

//potential todo: have this dynamically change based on terminal size?
// height and width of the cards
#define CARD_WIDTH 5
#define CARD_HEIGHT 5
// how much space between cards
#define CARD_SPACING_X 2
#define CARD_SPACING_Y 1

// the top-left position of the top-left card
// we have to leave room for UI/debugging elements
#define CARDS_START_X 1
#define CARDS_START_Y 5


typedef struct _CARD_struct {
	// The X and Y of the card, on the grid of the cards
	// This is not the raw drawing position!
	int x,y;
	char value;
	int visible;
}CARD;

void init_card(CARD *card, int x, int y, char v, int vis) {
	card->x = x;
	card->y = y;
	card->value = v;
	card->visible = vis;
}

void draw_card(CARD *card) {
	//Top-left position of the card
	int x = CARDS_START_X+((CARD_SPACING_X+CARD_WIDTH)*card->x);
	int y = CARDS_START_Y+((CARD_SPACING_Y+CARD_HEIGHT)*card->y);

	int w = CARD_WIDTH-1;
	int h = CARD_HEIGHT-1;
	// Find the center of the card, for displaying the value
	int cx = x+(CARD_WIDTH/2); 
	int cy = y+(CARD_HEIGHT/2);
	
	// Draw the box of the card
	mvaddch(y, x, '+');
	mvaddch(y, x+w, '+');
	mvaddch(y+h, x, '+');
	mvaddch(y+h, x+w, '+');
	mvhline(y, x+1, '-', w-1);
	mvhline(y+h, x+1, '-', w-1);
	mvvline(y+1, x, '|', h-1);
	mvvline(y+1, x+w, '|', h-1);

	// Draw the value of the card
	// Either the actual value, or a space to clear the previously drawn value
	if (card->visible) {
		mvaddch(cy, cx, card->value);
	} else {
		mvaddch(cy, cx, ' ');
	}

	// Refresh the screen
	//refresh();
}

void handle_potential_collision(CARD *card, int clickx, int clicky) {
	// Top left position
	int x = CARDS_START_X+((CARD_SPACING_X+CARD_WIDTH)*card->x);
	int y = CARDS_START_Y+((CARD_SPACING_Y+CARD_HEIGHT)*card->y);
	// Bottom right position
	int w = x+CARD_WIDTH-1;
	int h = y+CARD_HEIGHT-1;
	if (x <= clickx && clickx <= w &&
		y <= clicky && clicky <= h) {
		if (card->visible == true) {
			card->visible = false;
		} else {
			card->visible = true;
		}
	}
}

int main() {
	int c;
	MEVENT event;
	CARD card1;
	CARD card2;
	CARD card3;
	CARD card4;

	initscr();
	clear();
	//start_color();
	keypad(stdscr, TRUE);
	cbreak(); //Disable line buffering
	noecho();

	printw("Card matching!");
	refresh();
	
	mousemask(ALL_MOUSE_EVENTS, NULL);

	
	init_card(&card1, 0, 0, 'K', true);
	init_card(&card2, 1, 0, 'L', true);
	init_card(&card3, 0, 1, 'V', true);
	init_card(&card4, 1, 1, 'E', false);
	while (true) {
		c = getch();
		if (c == KEY_MOUSE && getmouse(&event) == OK) {
			//Mouse clicked somewhere
			mvprintw(2, 1, "Mouse clicked at %d %d", event.x+1, event.y+1);
			handle_potential_collision(&card1, event.x, event.y);
			handle_potential_collision(&card2, event.x, event.y);
			handle_potential_collision(&card3, event.x, event.y);
			handle_potential_collision(&card4, event.x, event.y);
		}
		draw_card(&card1);
		draw_card(&card2);
		draw_card(&card3);
		draw_card(&card4);
		//card2.visible = false;
		refresh();
	}
	endwin();
	return 0;
}

