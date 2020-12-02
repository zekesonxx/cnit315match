#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <card.h>
#define true 1
#define false 0

#define NUM_VALUES 25
const char VALUES[NUM_VALUES]="ABCDEFGHIJKLMNOP123456789";

void init_card(CARD *card, char value) {
	card->value = value;
	card->visible = false;
    card->hidden = false;
}

CARD* init_cards(int numcards) {
    if (numcards%2 != 0) {
        printf("Programmer error: numcards isn't even");
        exit(2);
    } else if (numcards/2 > NUM_VALUES) {
        printf("Programmer error: not enough values for the cards");
        exit(2);
    }
	CARD* cards = malloc(numcards*sizeof(CARD));
    if (cards == NULL) {
        printf("Failed to allocate memory.\n");
        exit(2);
    }
    // Put in two matching cards at a time
    // Doesn't matter that they're next to each other since they get shuffled after
    for (int i = 0; i < numcards; i+=2) {
        init_card(&cards[i], VALUES[i/2]);
        init_card(&cards[i+1], VALUES[i/2]);
    }
    shuffle_cards(cards, numcards);
    return cards;
}

// Full credit:
// https://benpfaff.org/writings/clc/shuffle.html
void shuffle_cards(CARD *cards, int numcards) {
    if (numcards > 1) {
        int i;
        for (i = 0; i < numcards - 1; i++) {
            int j = i + rand() / (RAND_MAX / (numcards - i) + 1);
            CARD t = cards[j];
            cards[j] = cards[i];
            cards[i] = t;
        }
    }
}

void calculate_grid(int numcards, int* gridwidth, int* gridheight) {
    int max_grid_cols = (COLS-CARDS_START_X)/(CARD_SPACING_X+CARD_WIDTH);
    int max_grid_rows = (LINES-CARDS_START_Y)/(CARD_SPACING_Y+CARD_HEIGHT);
    int square = (int) ceil(sqrt((double) numcards));
    mvprintw(1, 0, "max_grid_cols=%d, max_grid_rows=%d, square=%d", max_grid_cols, max_grid_rows, square);
    if (max_grid_rows < square) {
        mvhline(4, 0, '#', COLS);
        *gridwidth = max_grid_rows;
        *gridheight = (int) ceil((double) numcards/(double) max_grid_rows);;
    } else if (max_grid_cols < square) {
        mvvline(0, 4, '#', LINES);
        *gridheight = max_grid_cols;
        *gridwidth = (int) ceil((double) numcards/(double) max_grid_cols);
    } else {
        *gridheight = square;
        *gridwidth = square;
    }
}

void draw_cards(CARD *cards, int numcards) {
    int gridwidth, gridheight;
    calculate_grid(numcards, &gridwidth, &gridheight);
    mvprintw(2, 0, "%dx%d=%d%c", gridwidth, gridheight, gridwidth*gridheight, gridwidth*gridheight<numcards?'!':' ');
    int card = 0;
    for (int gridy = 0; gridy<gridwidth; gridy++) {
        for (int gridx = 0; gridx<gridheight; gridx++) {
            draw_card(&cards[card], gridx, gridy);
            card++;
            if (card > numcards) return;
        }
    }
}

void draw_card(CARD *card, int gridx, int gridy) {
    //Top-left position of the card
    int x = CARDS_START_X+((CARD_SPACING_X+CARD_WIDTH)*gridx);
    int y = CARDS_START_Y+((CARD_SPACING_Y+CARD_HEIGHT)*gridy);

    int w = CARD_WIDTH-1;
    int h = CARD_HEIGHT-1;
    // Find the center of the card, for displaying the value
    int cx = x+(CARD_WIDTH/2); 
    int cy = y+(CARD_HEIGHT/2);
    if (card->hidden) {
        //Card is hidden, draw over the old card with spaces
        mvaddch(y, x, ' ');
        mvaddch(y, x+w, ' ');
        mvaddch(y+h, x, ' ');
        mvaddch(y+h, x+w, ' ');
        mvhline(y, x+1, ' ', w-1);
        mvhline(y+h, x+1, ' ', w-1);
        mvvline(y+1, x, ' ', h-1);
        mvvline(y+1, x+w, ' ', h-1);
        mvaddch(cy, cx, ' ');
    } else {
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
    }
}

int handle_potential_collision(CARD *cards, int numcards, int clickx, int clicky) {
    int gridwidth, gridheight;
    calculate_grid(numcards, &gridwidth, &gridheight);
    int card = 0;
    for (int gridy = 0; gridy<gridwidth; gridy++) {
        for (int gridx = 0; gridx<gridheight; gridx++) {
                if (!cards[card].hidden) {
                    // Top left position
                    int x = CARDS_START_X+((CARD_SPACING_X+CARD_WIDTH)*gridx);
                    int y = CARDS_START_Y+((CARD_SPACING_Y+CARD_HEIGHT)*gridy);
                    // Bottom right position
                    int w = x+CARD_WIDTH-1;
                    int h = y+CARD_HEIGHT-1;
                    if (x <= clickx && clickx <= w &&
                        y <= clicky && clicky <= h) {
                        if (cards[card].visible == true) {
                            //card->visible = false;
                        } else {
                            cards[card].visible = true;
                        }
                        return card;
                    }
                }
                card++;
        }
    }
    return -1;
}
