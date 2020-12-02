#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <card.h>
#define true 1
#define false 0

#define NUM_VALUES 10
const char VALUES[NUM_VALUES]={'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};

void init_card(CARD *card, char v, int vis, int hidden) {
	card->value = v;
	card->visible = vis;
    card->hidden = hidden;
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


void draw_cards(CARD *cards, int numcards) {
    int gridheight = (int) floor(sqrt((double) numcards));
    int gridwidth = numcards/gridheight;
    int card = 0;
    for (int gridx = 0; gridx<gridwidth; gridx++) {
        for (int gridy = 0; gridy<gridheight; gridy++) {
            draw_card(&cards[card], gridx, gridy);
            card++;
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
    int gridheight = (int) floor(sqrt((double) numcards));
    int gridwidth = numcards/gridheight;
    int card = 0;
    for (int gridx = 0; gridx<gridwidth; gridx++) {
        for (int gridy = 0; gridy<gridheight; gridy++) {
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
