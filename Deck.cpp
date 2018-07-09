
#include "Deck.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

DECK::DECK(int numberOfDecks)
{
    char suit[52] = {'S','S','S','S','S','S','S','S','S','S','S','S','S',
                     'H','H','H','H','H','H','H','H','H','H','H','H','H',
                     'D','D','D','D','D','D','D','D','D','D','D','D','D',
                     'C','C','C','C','C','C','C','C','C','C','C','C','C'};

    char name[52] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K',
                     'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K',
                     'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K',
                     'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};

    int value[52] = {11,2,3,4,5,6,7,8,9,10,10,10,10,
                     11,2,3,4,5,6,7,8,9,10,10,10,10,
                     11,2,3,4,5,6,7,8,9,10,10,10,10,
                     11,2,3,4,5,6,7,8,9,10,10,10,10};

          int hilo[52] = {-1,1,1,1,1,1,0,0,0,-1,-1,-1,-1,
                          -1,1,1,1,1,1,0,0,0,-1,-1,-1,-1,
                          -1,1,1,1,1,1,0,0,0,-1,-1,-1,-1,
                          -1,1,1,1,1,1,0,0,0,-1,-1,-1,-1};

    int Omega[52] = {0,1,1,2,2,2,1,0,-1,-2,-2,-2,-2,
                     0,1,1,2,2,2,1,0,-1,-2,-2,-2,-2,
                     0,1,1,2,2,2,1,0,-1,-2,-2,-2,-2,
                     0,1,1,2,2,2,1,0,-1,-2,-2,-2,-2};

    //int omega[52] = {};
    int numberOfCards = (numberOfDecks * 52);
    deck = new Card[numberOfCards];

    for (int i = 0; i < numberOfCards; i++)
    {
        deck[i].inDeck = true;
        deck[i].value = value[i%52];
        deck[i].name = name[i%52];
        deck[i].suit = suit[i%52];
        deck[i].Count1 = hilo[i%52];
        deck[i].Count2 = Omega[i%52];
    }

    runningCountHiLo = 0;
    runningCountOmega = 0;
    offerInsurance = false;
}

Card DECK::draw()
{
    for (int c = 0; c < numberOfCards; c++)
    {
        if (deck[c].inDeck)
        {
            runningCountHiLo += deck[c].Count1;
            runningCountOmega += deck[c].Count2;
            deck[c].inDeck = false;
            deck[c].visible = true;
            return deck[c];
        }
    }
    return deck[51];
}

Card DECK::drawHole()
{
    /*unsigned choice = 0;
    unsigned seed = time(0);
    srand(seed);

    do
    {
        //choice = rand() % 52;
        choice = rand() % numberOfCards;
    }
    while (!(deck[choice].inDeck));

    runningCountHiLo += deck[choice].Count1;
    runningCountOmega += deck[choice].Count2;
    deck[choice].inDeck = false;
    deck[choice].visible = false;
    return deck[choice];*/
    for (int c = 0; c < numberOfCards; c++)
    {
        if (deck[c].inDeck)
        {
            runningCountHiLo += deck[c].Count1;
            runningCountOmega += deck[c].Count2;
            deck[c].inDeck = false;
            deck[c].visible = false;
            return deck[c];
        }
    }
    return deck[51];
}

void DECK::print()
{
    for (int s = 0; s < 4; s++)
    {
        for (int v = 0; v < 13; v++)
        {
            int a = v + (13*s);
            if (deck[a].inDeck)
            {
                cout << deck[a].name << deck[a].suit << "= " << deck[a].value << ", ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

double DECK::decks()
{
    numInDeck = 0;

    for (int i = 0; i < numberOfCards; i++)
    {
        if (deck[i].inDeck)
        {
            numInDeck++;
        }
    }
    return numInDeck/52.0;
}

int DECK::trueHiLo()
{
    return (runningCountHiLo / decks());
}

int DECK::trueOmega()
{
    return (runningCountOmega / decks());
}

int DECK::runCount()
{
    return runningCountOmega;
}

void DECK::shuffle(int numberOfDecks)
{
    numberOfCards = (numberOfDecks * 52);
    for (int i = 0; i < numberOfCards; i++)
    {
        deck[i].inDeck = true;
    }
    runningCountHiLo = 0;
    runningCountOmega = 0;

    copy1 = new Card[numberOfCards];

    for (int k = 0; k < numberOfCards; k++)
    {
        int *addresses = NULL;

        // count the number of cards still in the deck
        int counter = 0;
        for (int c = 0; c < numberOfCards; c++)
        {
            if (deck[c].inDeck)
                counter++;
        }
        // Create an array only big enough to hold the
        // addresses of the cards still in the deck
        int index = 0;
        addresses = new int[counter];
        for (int c = 0; c < numberOfCards; c++)
        {
            if (deck[c].inDeck)
            {
                addresses[index] = c;
                index++;
            }
        }

        // seed random number
        unsigned choice = 0;
        unsigned seed = time(0);
        srand(seed);

        // Select a random number from the address array
        choice = (rand() % (counter)); //0 -

        // Copy the card from the random address in the deck.
        copy1[k] = deck[addresses[choice]];
        deck[addresses[choice]].inDeck = false;

        // Addresses must be cleared before the next card is selected.
        addresses = NULL;
        delete [] addresses;
    }

    for (int k = 0; k < numberOfCards; k++)
    {
        deck[k] = copy1[k];
        deck[k].inDeck = true;
    }
    delete [] copy1;
}

bool DECK::returnDisplay()
{
    return display;
}

void DECK::setDisplay(bool input)
{
    display = input;
}

void DECK::reset()
{
    for (int i = 0; i < numberOfCards; i++)
    {
        deck[i].inDeck = true;
    }
    runningCountHiLo = 0;
    runningCountOmega = 0;
}

int DECK::aceAdjusted()
{
    // number of aces - number of quarter decks multiplied by 2
    // added to true count
    int aces = 0;
    for (int i = 0; i < numberOfCards; i++)
    {
        if (deck[i].name == 'A')
        {
            aces++;
        }
    }

    double quarterDecks = (decks()*4);
    return (((aces - quarterDecks)*2) + trueOmega());
}

void DECK::shoeSize()
{
    numInDeck = 0;

    for (int i = 0; i < numberOfCards; i++)
    {
        if (deck[i].inDeck)
        {
            numInDeck++;
        }
    }

    int d = numInDeck/52;
    //int full = (quarterDecks / 4);
    int partial = (numInDeck % 52)/13;

    for (int i = 0; i < d; i++)
    {
        cout << ". | | | | ";
    }
    cout << ". ";
    for (int i = 0; i < partial; i++)
    {
        cout << "| ";
    }
}

int DECK::extraAces()
{
    // number of aces - number of quarter decks multiplied by 2
    // added to true count
    int aces = 0;
    for (int i = 0; i < numberOfCards; i++)
    {
        if (deck[i].name == 'A')
        {
            aces++;
        }
    }

    double quarterDecks = (decks()*4);
    return (aces - quarterDecks);
}
