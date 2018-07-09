#ifndef DECK_H
#define DECK_H

struct Card
{
    char suit;     // S,C,D,H
    int value;     // 1-13
    bool inDeck;
    bool visible;
    char name;
    int Count1;
    int Count2;
};

class DECK
{
    private:
        Card *deck;
        Card *copy1;
        int numInDeck;
        int runningCountHiLo;
        int runningCountOmega;
        int numberOfDecks;
        int numberOfCards;
        bool display;
        int shoe;
    public:
        DECK(int);
        Card draw();
        Card drawHole();
        void shuffle(int);
        void print();
        double decks();
        int trueHiLo();
        int trueOmega();
        int runCount();
        int aceAdjusted();
        bool returnDisplay();
        void setDisplay(bool);
        void reset();
        bool offerInsurance;
        void shoeSize();
        int extraAces();
};

#endif // DECK_H

