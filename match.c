#include <ncurses.h>
#include <unistd.h>
#define true 1
#define false 0

#define CARD_WIDTH 5
#define CARD_HEIGHT 5

typedef struct _CARD_struct {
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
	int x = card->x;
	int y = card->y;
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

	if (card->visible) {
		mvaddch(cy, cx, card->value);
	} else {
		mvaddch(cy, cx, ' ');
	}

	// Refresh the screen
	refresh();
}

int main() {

	initscr();
	start_color();
	cbreak();

	noecho();

	printw("Card matching!");
	refresh();

	CARD card1;
	CARD card2;
	CARD card3;
	CARD card4;
	init_card(&card1, 2, 3, 'K', true);
	init_card(&card2, 6, 3, 'L', true);
	init_card(&card3, 9, 7, 'V', true);
	init_card(&card4, 10, 11, 'E', false);
	while (true) {
		draw_card(&card1);
		draw_card(&card2);
		draw_card(&card3);
		draw_card(&card4);
		sleep(2);
		card2.visible = false;
	}
	endwin();
	return 0;
}

