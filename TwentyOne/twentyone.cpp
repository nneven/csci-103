/*******************************************************************************
* CS 103 Twenty-One (Blackjack) PA
* Name: Nicolas Neven
* USC email: nneven@usc.edu
* Comments (you want us to know):
*
*
******************************************************************************/

// Add other #includes if you need
#include <iostream>
#include <cstdlib>

using namespace std;

/* Prototypes */
void shuffle(int cards[]);
void printCard(int id);
int cardValue(int id);
void printHand(int hand[], int numCards);
int getBestScore(int hand[], int numCards);

const int NUM_CARDS = 52;

/**
* Global arrays to be used as look-up tables, if desired.
* It is up to you if and how you want to use these
*/
const char suit[4] = {'H','S','D','C'};
const char* type[13] = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
const int value[13] = {2,3,4,5,6,7,8,9,10,10,10,10,11};

/**
* Should permute the deck of cards, effectively shuffling it.
* You must use the Fisher-Yates / Durstenfeld shuffle algorithm
* described in the assignment writeup.
*/
void shuffle(int cards[])
{
	/******** You complete ****************/
	for (int i = 0; i < NUM_CARDS; i++) {
		cards[i] = i;
	}
	for (int i = NUM_CARDS - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		int temp = cards[i];
		cards[i] = cards[j];
		cards[j] = temp;
	}
}

/**
* Prints the card in a "pretty" format:   "type-suit"
* Examples:  K-C  (King of clubs), 2-H (2 of hearts)
* Valid Types are: 2,3,4,5,6,7,8,9,10,J,Q,K,A
* Valid Suits are: H, D, C, S
*/
void printCard(int id)
{
	/******** You complete ****************/
	cout << " " << type[id % 13]  << "-" << suit[id / 13];
}

/**
* Returns the numeric value of the card.
* Should return 11 for an ACE and can then
* be adjusted externally based on the sum of the score.
*/
int cardValue(int id)
{
	/******** You complete ****************/
	return value[id % 13];	
}

/**
* Should print out each card in the hand separated by a space and
* then print a newline at the end.
* Should use printCard() to print out each card.
*/
void printHand(int hand[], int numCards)
{
	/******** You complete ****************/
	for (int i = 0; i < numCards; i++) {
		printCard(hand[i]);
	}
	cout << endl;
}

/**
* Should return the total score of the provided hand.
* ACES should be treated as 11s to make the highest possible hand
* and only being reduced to 1s as needed to avoid busting.
*/
int getBestScore(int hand[], int numCards)
{
	/******** You complete ****************/
	int total = 0;
	int aces = 0;
	for (int i = 0; i < numCards; i++) {
		if (cardValue(hand[i]) == 11) {
			aces++;
		}
		total += cardValue(hand[i]);
	}
	while (total > 21 && aces > 0) {
		total -= 10;
		aces--;
	}
	return total;
}

/**
* Main program logic for the game of 21
*/
int main(int argc, char* argv[])
{
	//---------------------------------------
	// Do not change this code -- Begin
	//---------------------------------------
	if (argc < 2) {
		cout << "Error - Please provide the seed value." << endl;
		return 1;
	}
	int seed = atoi(argv[1]);
	srand(seed);
	
	int cards[52];
	int dhand[9];
	int phand[9];
	//---------------------------------------
	// Do not change this code -- End
	//---------------------------------------
	
	/******** You complete ****************/
	char play = 'y';
	while (play == 'y') {
        shuffle(cards);
		phand[0] = cards[0];
		phand[1] = cards[2];
		dhand[0] = cards[1];
		dhand[1] = cards[3];
		cout << "Dealer: ?";
        printCard(dhand[1]);
        cout << endl << "Player:";
        printHand(phand, 2);
        int pScore = getBestScore(phand, 2);
        int dScore = getBestScore(dhand, 2);
        int hit = 0;
        char move = 'h';
        while (move == 'h') {
            if (pScore != 21) {
                cout << "Type 'h' to hit and 's' to stay:" << endl;
        	    cin >> move;
            }
        	if (move == 'h' && pScore != 21) {
        		hit++;
        		phand[1 + hit] = cards[3 + hit];
        		cout << "Player:";
        		printHand(phand, 2 + hit);
        		pScore = getBestScore(phand, 2 + hit);
        		if (pScore > 21) {
        			cout << "Player busts" << endl << "Lose " << pScore
                    << " " << dScore << endl << endl;
        			move = 's';
        		}
        	} else {
                move = 's';
        		int dCards = 2;
        		while (dScore < 17) {
        			hit++;
        			dhand[dCards] = cards[3 + hit];
        			dCards++;
        			dScore = getBestScore(dhand, dCards);
     		    }
                cout << "Dealer:";
                printHand(dhand, dCards);
                if (dScore == pScore) {
        			cout << "Tie";
       		    } else if (dScore < 21 && dScore > pScore) {
        	    	cout << "Lose";
        		} else {
                    if (dScore > 21) {
                        cout << "Dealer busts" << endl;
                    }
        			cout << "Win";
        		}
        		cout << " " << pScore << " " << dScore << endl << endl;
        	}
        }

        cout << "Play again? [y/n]" << endl;
        cin >> play;
	}
	return 0;
}