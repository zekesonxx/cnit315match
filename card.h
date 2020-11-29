#ifndef CARD_H
#define CARD_H
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

// Very temporary, this should be dynamic
#define NUM_CARDS 12

typedef struct _CARD_struct {
	// The X and Y of the card, on the grid of the cards
	// This is not the raw drawing position!
	int x,y;
	char value;
	int visible;
}CARD;

void init_card(CARD *card, int x, int y, char v, int vis);

void draw_card(CARD *card);

void handle_potential_collision(CARD *card, int clickx, int clicky);

#endif
