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

/*
 * A card in the matching game
 */
typedef struct _CARD_struct {
	// The X and Y of the card, on the grid of the cards
	// This is not the raw drawing position!
	int x,y;
    // What the face value of the card is, the thing you have to match
	char value;
    // Whether the card's value is visible.
	int visible;
    // Whether the card is being rendered at all
    int hidden;
}CARD;

/*
 * Initializes a card struct with the provided values
 * Just a conveience function.
 */
void init_card(CARD *card, int x, int y, char v, int vis, int hidden);

/*
 * Shuffles an array of cards
 */
void shuffle_cards(CARD *cards, int numcards);


/*
 * Draws a card to the screen
 */
void draw_card(CARD *card, int gridx, int gridy);

/* 
 * Draw an array of cards to the screen
 */
void draw_cards(CARD *cards, int numcards);

/*
 * Called by the main game loop to determine when a card is clicked
 * If the card is clicked and it was not already made visible, it will be made visible
 * Returns 1 if the card was clicked (and it was made visible), 0 if the card wasn't clicked
 */
int handle_potential_collision(CARD *cards, int numcards, int clickx, int clicky);

#endif
