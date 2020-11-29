#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <card.h>

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
