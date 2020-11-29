#include <ncurses.h>
#include <unistd.h>
#define true 1
#define false 0

//potential todo: have this dynamically change based on terminal size?
#define CARD_WIDTH 5
#define CARD_HEIGHT 5
#define CARD_SPACING 1

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

int main() {
	int c, counter = 0;
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

	init_card(&card1, 2, 3, 'K', true);
	init_card(&card2, 6, 3, 'L', true);
	init_card(&card3, 9, 7, 'V', true);
	init_card(&card4, 10, 11, 'E', false);
	while (true) {
		c = getch();
		mvprintw(1, 0, "%d %d %d", KEY_MOUSE, counter++, OK);
		if (c == KEY_MOUSE && getmouse(&event) == OK) {
			//Mouse clicked somewhere
			mvprintw(2, 1, "Mouse clicked at %d %d", event.x+1, event.y+1);
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

