#include <iostream>
#include "Deck.h"
#include <iomanip>
#include <string>
#include <ctime>
using namespace std;

struct pixel
{
    char ink;
    int num;
};

struct Strategy
{
    char moves[21][10][3][2][3];
    int index[21][10][3];
    double data[21][10][3];
    int p,d,t;
    string name;
};

struct Hand
{
    Card cards[20];
    int total;
    bool stands;
    bool surrenders;
    bool goesBust;
    bool gotBlackJack;
    bool firstTwoCards;
    bool softHand;
    bool canSplit;
    bool doubleDown;
    bool wins;
    bool push;
};

struct PLAYER
{
    Hand hands[10];
    long double bankroll;
    double betSize;
    int betIncrement;
    int betSpread;
    int activeHands;
};

class DEALER
{
    public:
        Card hand[20];
        int total;
        bool stands;
        bool goesBust;
        bool gotBlackjack;
        bool revealHoleCard;
        void starting(DECK &);
        void turn(DECK &);
        void aceChanger();
        void show();
};

void DEALER::starting(DECK &deck)
{
    stands = false;
    goesBust = false;
    gotBlackjack = false;
    revealHoleCard = false;
    total = 0;

    for (int i = 0; i < 20; i++)
    {
        hand[i].value = 0;
        hand[i].inDeck = false;
    }

    // Draw first two cards
    hand[0] = deck.draw();
    hand[0].inDeck = true;
    hand[1] = deck.drawHole();
    hand[1].inDeck = true;
    total += hand[0].value;
    total += hand[1].value;

    // Dealer blackjack
    if (total == 21)
    {
        gotBlackjack = true;
        stands = true;
        return;
    }

    // check two card stand
    if ((total > 16) && (total < 21))
    {
        stands = true;
    }

    if (hand[0].value == 11)
    {
        deck.offerInsurance = true;
    }
}

void DEALER::turn(DECK &deck)
{
    int cards = 2;

    hand[1].visible = true;

    while ((!(goesBust)) && (!(stands)))
    {
        hand[cards] = deck.draw();
        hand[cards].inDeck = true;
        total += hand[cards].value;

        aceChanger();

        if ((total >= 17) && (total <= 21))
        {
            stands = true;
        }

        cards++;
    }
}

void DEALER::aceChanger()
{
    bool hasAce = false;
    int ace;

    if (total > 21)
        {
            for (int i = 0; i < 20; i++)
            {
                if (hand[i].value == 11)
                {
                    hasAce = true;
                    ace = i;
                }
            }

            if (hasAce == true)
            {
                hand[ace].value = 1;
                total -= 10;
                hasAce = false;
            }
            else
            {
                goesBust = true;
            }
        }
}

void DEALER::show()
{
    char picture1[28] =  {' ','_','_','_','_','_',' ',
                          '|',' ','O',' ',' ',' ','|',
                          '|',' ',' ',' ',' ',' ','|',
                          '|','_','_','_','X','_','|'};

    char picture2[55] = {' ','_','_','_','_','_',' ',' ',' ',' ',' ',
                         '|',' ','X',' ',' ','_','|','_','_','_',' ',
                         '|',' ',' ',' ','|',' ','O',' ',' ',' ','|',
                         '|','_','_','_','|',' ',' ',' ',' ',' ','|',
                         ' ',' ',' ',' ','|','_','_','_','X','_','|',};

    char picture3[90] = {' ','_','_','_','_','_',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                         '|',' ','X',' ',' ','_','|','_','_','_',' ',' ',' ',' ',' ',
                         '|',' ',' ',' ','|',' ','X',' ',' ','_','|','_','_','_',' ',
                         '|','_','_','_','|',' ',' ',' ','|',' ','O',' ',' ',' ','|',
                         ' ',' ',' ',' ','|','_','_','_','|',' ',' ',' ',' ',' ','|',
                         ' ',' ',' ',' ',' ',' ',' ',' ','|','_','_','_','X','_','|'};


    pixel display[7][105];
    for (int r = 0; r < 7; r++)
    {
        for (int c = 0; c < 105; c++)
        {
            display[r][c].ink = ' ';
            display[r][c].num = 50;
        }
    }

    int cards = 0;
    for (int a = 0; a < 20; a++)
    {
        if ((hand[a].inDeck) && (hand[a].visible))
        {
            cards++;
        }
    }
    int trips = cards / 3;
    int type = cards % 3;
    int cardIndex = 0;
    for (int t = 0; t < trips; t++)
    {
        for (int i = 0; i < 90; i++)
        {
            int r = (i/15);
            int c = ((i%15) + (15*t));
            if (picture3[i] == 'X')
            {
                display[r][c].num = cardIndex;
                cardIndex++;
            }
            else if (picture3[i] == 'O')
            {
                display[r][c].num = cardIndex;
            }
            else
                display[r][c].ink = picture3[i];
        }
    }
    if (type == 2)
    {
        for (int i = 0; i < 55; i++)
        {
            int r = i / 11;
            int c = ((i%11) + (15*trips));
            if (picture2[i] == 'X')
            {
                display[r][c].num = cardIndex;
                cardIndex++;
            }
            else if (picture2[i] == 'O')
            {
                display[r][c].num = cardIndex;
            }
            else
                display[r][c].ink = picture2[i];
        }
    }
    if (type == 1)
    {
        for (int i = 0; i < 28; i++)
        {
            int r = i/7;
            int c = ((i%7) + (15*trips));
            if (picture1[i] == 'X')
            {
                display[r][c].num = cardIndex;
                cardIndex++;
            }
            else if (picture1[i] == 'O')
            {
                display[r][c].num = cardIndex;
            }
            else
                display[r][c].ink = picture1[i];
        }
    }
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    for (int r = 0; r < 7; r++)
    {
        for (int c = 0; c < 105; c++)
        {
            if (display[r][c].num < 50)
            {
                cout << hand[display[r][c].num].name;
            }
            else
                cout << display[r][c].ink;
        }
        cout << endl;
    }
    cout << "vs.\n";
  /*  cout << "Dealer\t\t";
    {
        for (int i = 0; i < 20; i++)
        {
            if ((hand[i].inDeck) && (hand[i].visible))
            {
                cout << hand[i].name << " ";
            }
            else
            {
                cout << "  ";
            }
        }
        cout << total << endl;
    }*/
}

// Prototypes
bool initialBlackJack(PLAYER, DEALER, DECK);
char strategy(Hand, DEALER, DECK, Strategy &);
void startingHand(PLAYER &, DECK &);
void moves(PLAYER &, DEALER, DECK &, Strategy &);
void show(PLAYER);
void menu(PLAYER, int);
void aceChanger(Hand &);
void splitter(PLAYER &,DECK &, int);
void insurance(PLAYER &, DEALER, DECK, int);
void betSize(PLAYER &, DECK, int);
void realBetSize(PLAYER &, DECK, int);
void HiLo(Strategy &);
void HiLoLite(Strategy &);
void Experimental(Strategy &);
void Basic(Strategy &);
void data(Strategy &, int);
void EasyPeasy(Strategy &);

int main()
{
    double finalDeckSize = 2;

    DECK gameDeck(6);
    gameDeck.setDisplay(true);

    DEALER Dealer;

    PLAYER Player;
    Player.bankroll = 500;
    Player.betSpread = 4;
    Player.betIncrement = 5;

    bool compare = false;

    PLAYER SecondPlayer;
    SecondPlayer.bankroll = 500;
    SecondPlayer.betSpread = 4;
    SecondPlayer.betIncrement = 5;
    double high2 = SecondPlayer.bankroll;
    double low2 = SecondPlayer.bankroll;

    Strategy strat1;
    EasyPeasy(strat1);
    Strategy strat2;
    EasyPeasy(strat2);

    int numberOfHands = 0;
    double high1 = Player.bankroll;
    double low1 = Player.bankroll;
    unsigned startTime;
    unsigned endTime;
    unsigned totalTime;

    cout << "Get ready\n";
    cin.get();

    startTime = time(0);
    while(numberOfHands < 10000)
    {
        gameDeck.shuffle(6);
        if (gameDeck.returnDisplay())
        {
            cout << "Shuffling the deck...\n";
            cin.get();
        }
        while (gameDeck.decks() > finalDeckSize)
        {
            betSize(Player, gameDeck, 2);
            Dealer.starting(gameDeck);
            startingHand(Player, gameDeck);
            insurance(Player, Dealer, gameDeck, 2);

            if (!(initialBlackJack(Player,Dealer, gameDeck)))
            {
                if (gameDeck.returnDisplay())
                {
                    Dealer.show();
                    show(Player);
                }
                moves(Player, Dealer, gameDeck, strat1);
                for (int a = 0; a < Player.activeHands; a++)
                {
                    if (Player.hands[a].stands)
                    {
                        Dealer.turn(gameDeck);
                        if (Dealer.stands)
                        {
                            int win = Player.hands[a].total - Dealer.total;
                            if (win > 0)
                            {
                                if (gameDeck.returnDisplay())
                                {
                                    Player.hands[a].wins = true;
                                }
                                if (Player.hands[a].gotBlackJack)
                                    Player.bankroll += (1.5 * Player.betSize);
                                else
                                    Player.bankroll += Player.betSize;
                            }
                            else if (win < 0)
                            {
                                if (gameDeck.returnDisplay())
                                {
                                    //cout << "Dealer wins ";
                                }
                                Player.bankroll -= Player.betSize;
                            }
                            else
                            {
                                if (gameDeck.returnDisplay())
                                {
                                   // cout << "Push ";
                                    Player.hands[a].push = true;
                                }
                            }
                        }
                        else if (Dealer.goesBust)
                        {
                            if (gameDeck.returnDisplay())
                            {
                                Player.hands[a].wins = true;
                            }
                            Player.bankroll += Player.betSize;
                        }
                    }
                    else if (Player.hands[a].goesBust)
                    {
                        if (gameDeck.returnDisplay())
                        {
                            Dealer.hand[1].visible = true;
                            //cout << "Dealer Wins ";
                        }
                        Player.bankroll -= Player.betSize;
                    }
                    else if (Player.hands[a].surrenders)
                    {
                        if (gameDeck.returnDisplay())
                        {
                            Dealer.hand[1].visible = true;
                            cout << "Hand: " << a << "Surrenders: " << Player.hands[a].surrenders << endl;
                            cout << "Player surrenders ";
                        }
                        Dealer.revealHoleCard = true;
                        Player.bankroll -= (.5 * Player.betSize);
                    }
                    if (gameDeck.returnDisplay())
                    {
                        //cout << "hand " << a << endl;
                    }
                }
                if (gameDeck.returnDisplay())
                {
                    Dealer.show();
                    show(Player);
                }
            }
            if (gameDeck.returnDisplay())
            {
                cout << "$" << Player.bankroll << endl;
            }
            if (Player.bankroll < low1)
            {
                low1 = Player.bankroll;
            }
            if (Player.bankroll > high1)
            {
                high1 = Player.bankroll;
            }
            if (compare == false)
            {
                numberOfHands++;
            }
            if (gameDeck.returnDisplay())
            {
                //cin.get();
            }
        }
        if (compare == true)
        {
            if (gameDeck.returnDisplay())
            {
                cout << "Reset\tReset\tReset\n";
                cin.get();
            }
            gameDeck.reset();
            while (gameDeck.decks() > finalDeckSize)
            {
                realBetSize(SecondPlayer, gameDeck, 3);
                Dealer.starting(gameDeck);
                startingHand(SecondPlayer, gameDeck);
                insurance(SecondPlayer, Dealer, gameDeck, 2);

                if (!(initialBlackJack(SecondPlayer,Dealer, gameDeck)))
                {
                    if (gameDeck.returnDisplay())
                    {
                        Dealer.show();
                        show(SecondPlayer);
                    }
                    moves(SecondPlayer, Dealer, gameDeck, strat2);
                    for (int a = 0; a < SecondPlayer.activeHands; a++)
                    {
                        if (SecondPlayer.hands[a].stands)
                        {
                            Dealer.turn(gameDeck);
                            if (Dealer.stands)
                            {
                                int win = SecondPlayer.hands[a].total - Dealer.total;
                                if (win > 0)
                                {
                                    if (gameDeck.returnDisplay())
                                    {
                                        cout << "SecondPlayer wins ";
                                    }
                                    if (SecondPlayer.hands[a].gotBlackJack)
                                        SecondPlayer.bankroll += (1.5 * SecondPlayer.betSize);
                                    else
                                        SecondPlayer.bankroll += SecondPlayer.betSize;
                                }
                                else if (win < 0)
                                {
                                    if (gameDeck.returnDisplay())
                                    {
                                        cout << "Dealer wins ";
                                    }
                                    SecondPlayer.bankroll -= SecondPlayer.betSize;
                                }
                                else
                                {
                                    if (gameDeck.returnDisplay())
                                    {
                                        cout << "Push ";
                                        SecondPlayer.hands[a].push = true;
                                    }
                                }
                            }
                            else if (Dealer.goesBust)
                            {
                                if (gameDeck.returnDisplay())
                                {
                                    cout << "SecondPlayer wins ";
                                }
                                SecondPlayer.bankroll += SecondPlayer.betSize;
                            }
                        }
                        else if (SecondPlayer.hands[a].goesBust)
                        {
                            if (gameDeck.returnDisplay())
                            {
                                Dealer.hand[1].visible = true;
                                cout << "Dealer Wins ";
                            }
                            SecondPlayer.bankroll -= SecondPlayer.betSize;
                        }
                        else if (SecondPlayer.hands[a].surrenders)
                        {
                            if (gameDeck.returnDisplay())
                            {
                                Dealer.hand[1].visible = true;
                                cout << "Hand: " << a << "Surrenders: " << SecondPlayer.hands[a].surrenders << endl;
                                cout << "SecondPlayer surrenders ";
                            }
                            Dealer.revealHoleCard = true;
                            SecondPlayer.bankroll -= (.5 * SecondPlayer.betSize);
                        }
                        if (gameDeck.returnDisplay())
                        {
                            cout << "hand " << a << endl;
                        }
                    }
                    if (gameDeck.returnDisplay())
                    {
                        Dealer.show();
                        show(SecondPlayer);
                    }
                }
                if (gameDeck.returnDisplay())
                {
                    cout << "\t\t$" << SecondPlayer.bankroll << endl;
                }
                if (SecondPlayer.bankroll < low2)
                {
                    low2 = SecondPlayer.bankroll;
                }
                if (SecondPlayer.bankroll > high2)
                {
                    high2 = SecondPlayer.bankroll;
                }
                numberOfHands++;
                if (gameDeck.returnDisplay())
                {
                    cin.get();
                }
            }
        }
    }
    cout << strat1.name << endl;
    cout << "$" << Player.bankroll << endl;
    cout << "Hands played: " << numberOfHands << endl;
    cout << "Average hand value: " << Player.bankroll / numberOfHands << endl;
    cout << "Highest: " << high1 << endl;
    cout << "Lowest: " << low1 << endl << endl;

    if (compare == true)
    {
        cout << strat2.name << endl;
        cout << "$" << SecondPlayer.bankroll << endl;
        cout << "Hands played: " << numberOfHands << endl;
        cout << "Average hand value: " << SecondPlayer.bankroll / numberOfHands << endl;
        cout << "Highest: " << high2 << endl;
        cout << "Lowest: " << low2 << endl;

        double average = ((Player.bankroll + SecondPlayer.bankroll) / 2);
        double difference = SecondPlayer.bankroll - Player.bankroll;
        cout << endl << "Percent Difference: " << ((difference/average) * 100) << endl;
    }

    endTime = time(0);
    totalTime = endTime - startTime;
    cout << endl << "Seconds Required: " << totalTime << endl;

    data(strat1, numberOfHands);
    return 0;
}

void startingHand(PLAYER &player, DECK &deck)
{
    player.activeHands = 1;
    player.hands[0].stands = false;
    player.hands[0].goesBust = false;
    player.hands[0].gotBlackJack = false;
    player.hands[0].softHand = false;
    player.hands[0].firstTwoCards = true;
    player.hands[0].canSplit = false;
    player.hands[0].surrenders = false;
    player.hands[0].push = false;
    player.hands[0].wins = false;
    player.hands[0].total = 0;

    for (int i = 0; i < 20; i++)
    {
        player.hands[0].cards[i].value = 0;
        player.hands[0].cards[i].inDeck = false;
    }

    // Draw first two cards
    player.hands[0].cards[0] = deck.draw();
    player.hands[0].cards[0].inDeck = true;
    player.hands[0].cards[1] = deck.draw();
    player.hands[0].cards[1].inDeck = true;
    player.hands[0].total += player.hands[0].cards[0].value;
    player.hands[0].total += player.hands[0].cards[1].value;
/*
    player.hands[0].cards[0].inDeck = true;
    player.hands[0].cards[1].inDeck = true;
    player.hands[0].cards[0].value = 10;
    player.hands[0].cards[1].value = 10;
    player.hands[0].cards[0].name = 'J';
    player.hands[0].cards[1].name = 'K';
    player.hands[0].total += 20;*/

    // Check for player blackjack
    if (player.hands[0].total == 21)
        {
            player.hands[0].stands = true;
            player.hands[0].gotBlackJack = true;
        }
    if ((player.hands[0].cards[0].value == 11) || (player.hands[0].cards[1].value == 11))
    {
        if (player.hands[0].total < 21)
        {
            player.hands[0].softHand = true;
        }
    }

    // Check for AA
    aceChanger(player.hands[0]);

    if (player.hands[0].total <= 11)
        {
            player.hands[0].doubleDown = true;
        }
    if ((player.hands[0].cards[0].value == 11) ||
        (player.hands[0].cards[1].value == 11))
        {
            if (player.hands[0].total < 21)
            {
                player.hands[0].doubleDown = true;
                player.hands[0].softHand = true;
            }
        }
    if (player.hands[0].cards[0].value == player.hands[0].cards[1].value)
    {
        player.hands[0].canSplit = true;
    }
}

void moves(PLAYER &player, DEALER dealer, DECK &deck, Strategy &strat)
{
    // Player moves
    char choice;

    for (int numberOfSplits = 0; numberOfSplits < player.activeHands; numberOfSplits++)
    {
        while (player.hands[numberOfSplits].firstTwoCards == true)
        {
          if (deck.returnDisplay())
          {
              menu(player, numberOfSplits);
          }
          //  cin >> choice;
          if (player.activeHands > 3)
          {
              for (int i = 0; i < 3; i++)
              {
                  player.hands[i].canSplit = false;
              }
              player.hands[numberOfSplits].firstTwoCards = false;
          }

          choice = strategy(player.hands[numberOfSplits], dealer, deck, strat);
            switch (choice)
            {
                case 's':
                    {
                        player.hands[numberOfSplits].stands = true;
                        player.hands[numberOfSplits].firstTwoCards = false;
                    }
                break;
                case 'h':
                    {
                        player.hands[numberOfSplits].cards[2] = deck.draw();
                        player.hands[numberOfSplits].cards[2].inDeck = true;
                        player.hands[numberOfSplits].total += player.hands[numberOfSplits].cards[2].value;
                        aceChanger(player.hands[numberOfSplits]);
                        player.hands[numberOfSplits].firstTwoCards = false;
                        player.hands[numberOfSplits].canSplit = false;
                        if (deck.returnDisplay())
                        {
                            if (!(player.hands[numberOfSplits].goesBust))
                            {
                                dealer.show();
                                show(player);
                            }
                        }
                        if (player.hands[numberOfSplits].total == 21)
                            player.hands[numberOfSplits].stands = true;
                    }
                break;
                case 'd':
                    {
                        player.betSize *= 2;
                        player.hands[numberOfSplits].cards[2] = deck.draw();
                        player.hands[numberOfSplits].cards[2].inDeck = true;
                        player.hands[numberOfSplits].total += player.hands[numberOfSplits].cards[2].value;
                        player.hands[numberOfSplits].stands = true;
                        player.hands[numberOfSplits].firstTwoCards = false;
                    }
                break;
                case 'p':
                    {
                        player.activeHands++;
                        splitter(player, deck, numberOfSplits);
                        if (deck.returnDisplay())
                        {
                            dealer.show();
                            show(player);
                        }
                    }
                break;
                case 'r':
                    {
                        player.hands[numberOfSplits].surrenders = true;
                        player.hands[numberOfSplits].firstTwoCards = false;
                    }
                break;
                default:
                    {
                        cout << "Enter a valid selection\n";
                        cin.get();
                    }
            }
        }
        int cardNumber = 3;
        while ((!(player.hands[numberOfSplits].goesBust))
            && (!(player.hands[numberOfSplits].stands))
            && (!(player.hands[numberOfSplits].surrenders)))
        {
            if (deck.returnDisplay())
            {
                menu(player, numberOfSplits);
            }
          //  cin >> choice;
            choice = strategy(player.hands[numberOfSplits], dealer, deck, strat);
            switch (choice)
            {
                case 's':
                    {
                        player.hands[numberOfSplits].stands = true;
                    }
                break;
                case 'h':
                    {
                        player.hands[numberOfSplits].cards[cardNumber] = deck.draw();
                        player.hands[numberOfSplits].cards[cardNumber].inDeck = true;
                        player.hands[numberOfSplits].total += player.hands[numberOfSplits].cards[cardNumber].value;
                        aceChanger(player.hands[numberOfSplits]);
                        if (deck.returnDisplay())
                        {
                            if (!(player.hands[numberOfSplits].goesBust))
                            {
                                dealer.show();
                                show(player);
                            }
                        }
                        if (player.hands[numberOfSplits].total == 21)
                            player.hands[numberOfSplits].stands = true;
                    }
                break;
                default:
                    {
                        cout << "Enter a valid choice\n";
                        cout << "Choice: " << choice << endl;
                        cin.get();
                    }
            }
            cardNumber++;
        }
    }
}

void show(PLAYER player)
{
    char picture1[28] =  {' ','_','_','_','_','_',' ',
                          '|',' ','O',' ',' ',' ','|',
                          '|',' ',' ',' ',' ',' ','|',
                          '|','_','_','_','X','_','|'};

    char picture2[55] = {' ','_','_','_','_','_',' ',' ',' ',' ',' ',
                         '|',' ','X',' ',' ','_','|','_','_','_',' ',
                         '|',' ',' ',' ','|',' ','O',' ',' ',' ','|',
                         '|','_','_','_','|',' ',' ',' ',' ',' ','|',
                         ' ',' ',' ',' ','|','_','_','_','X','_','|',};

    char picture3[90] = {' ','_','_','_','_','_',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                         '|',' ','X',' ',' ','_','|','_','_','_',' ',' ',' ',' ',' ',
                         '|',' ',' ',' ','|',' ','X',' ',' ','_','|','_','_','_',' ',
                         '|','_','_','_','|',' ',' ',' ','|',' ','O',' ',' ',' ','|',
                         ' ',' ',' ',' ','|','_','_','_','|',' ',' ',' ',' ',' ','|',
                         ' ',' ',' ',' ',' ',' ',' ',' ','|','_','_','_','X','_','|'};

    for (int h = 0; h < player.activeHands; h++)
    {
        pixel display[7][105];
        for (int r = 0; r < 7; r++)
        {
            for (int c = 0; c < 105; c++)
            {
                display[r][c].ink = ' ';
                display[r][c].num = 50;
            }
        }

        int cards = 0;
        for (int a = 0; a < 20; a++)
        {
            if (player.hands[h].cards[a].inDeck)
            {
                cards++;
            }
        }
        int trips = cards / 3;
        int type = cards % 3;
        int cardIndex = 0;
        for (int t = 0; t < trips; t++)
        {
            for (int i = 0; i < 90; i++)
            {
                int r = (i/15);
                int c = ((i%15) + (15*t));
                if (picture3[i] == 'X')
                {
                    display[r][c].num = cardIndex;
                    cardIndex++;
                }
                else if (picture3[i] == 'O')
                {
                    display[r][c].num = cardIndex;
                }
                else
                    display[r][c].ink = picture3[i];
            }
        }
        if (type == 2)
        {
            for (int i = 0; i < 55; i++)
            {
                int r = i / 11;
                int c = ((i%11) + (15*trips));
                if (picture2[i] == 'X')
                {
                    display[r][c].num = cardIndex;
                    cardIndex++;
                }
                else if (picture2[i] == 'O')
                {
                    display[r][c].num = cardIndex;
                }
                else
                    display[r][c].ink = picture2[i];
            }
        }
        if (type == 1)
        {
            for (int i = 0; i < 28; i++)
            {
                int r = i/7;
                int c = ((i%7) + (15*trips));
                if (picture1[i] == 'X')
                {
                    display[r][c].num = cardIndex;
                    cardIndex++;
                }
                else if (picture1[i] == 'O')
                {
                    display[r][c].num = cardIndex;
                }
                else
                    display[r][c].ink = picture1[i];
            }
        }
        if (player.activeHands > 1)
        {
            char hands[4] = {'H','a','n','d'};
            for (int c = 0; c < 4; c++)
            {
                display[5][c].ink = hands[c];
                display[5][4].ink = static_cast<char>(h - 1);
            }
        }
        if (player.hands[h].wins)
        {
            char win[6] = {'W','i','n','s','!','!'};
            for (int c = 8; c < 14; c++)
            {
                display[1][c].ink = win[c-8];
            }
        }
        else if (player.hands[h].goesBust)
        {
            char win[6] = {'B','u','s','t','e','d'};
            for (int c = 8; c < 14; c++)
            {
                display[1][c].ink = win[c-8];
            }
        }
        else if (player.hands[h].gotBlackJack)
        {
            char win[10] = {'B','l','a','c','k','J','a','c','k','!'};
            for (int c = 8; c < 18; c++)
            {
                display[1][c].ink = win[c-8];
            }
        }
        else if (player.hands[h].push)
        {
            char win[4] = {'P','u','s','h'};
            for (int c = 8; c < 12; c++)
            {
                display[1][c].ink = win[c-8];
            }
        }
        else if ((player.hands[h].stands) && (!(player.hands[h].wins)))
        {
            char win[5] = {'L','o','s','e','s'};
            for (int c = 8; c < 13; c++)
            {
                display[1][c].ink = win[c-8];
            }
        }
        for (int r = 0; r < 7; r++)
        {
            for (int c = 0; c < 105; c++)
            {
                if (display[r][c].num < 50)
                {
                    cout << player.hands[h].cards[display[r][c].num].name;
                }
                else
                    cout << display[r][c].ink;
            }
            cout << endl;
        }
        cin.get();
    }
   // cin.get();
}

void menu(PLAYER player, int numberOfSplits)
{
    if (player.activeHands > 1)
        cout << "Moves for hand " << numberOfSplits << ": ";
    if (player.hands[numberOfSplits].firstTwoCards)
    {
        cout << "s, h";
        if (player.hands[numberOfSplits].doubleDown)
            cout << ", d";
        if (player.hands[numberOfSplits].canSplit)
            cout << ", p";
        cout << ", r" << endl;
    }
    else
    {
        cout << "s, h\n";
    }
}

void aceChanger(Hand &hand)
{
    int ace;
    int numberOfAces = 0;
    bool hasAce = false;
    if (hand.total > 21)
    {
        for (int i = 0; i < 20; i++)
        {
            if (hand.cards[i].value == 11)
            {
                numberOfAces++;
                hasAce = true;
                ace = i;
            }
        }
        if (hasAce == true)
        {
            hand.cards[ace].value = 1;
            hand.total -= 10;
            if (numberOfAces == 1)
                hand.softHand = false;
            hasAce = false;
        }
        else
        {
            hand.goesBust = true;
        }
    }
    else if (hand.total < 12)
    {
        for (int i = 0; i < 20; i++)
        {
            if (hand.cards[i].value == 1)
            {
                numberOfAces++;
                hasAce = true;
                ace = i;
            }
        }
        if (hasAce)
        {
            hand.cards[ace].value = 11;
            hand.total += 10;
            hand.softHand = true;
            hasAce = false;
        }
    }
}

void splitter(PLAYER &player, DECK &deck, int numberOfSplits)
{
    int splitHand = numberOfSplits;
    int newHand = player.activeHands - 1;
    player.hands[newHand].stands = false;
    player.hands[newHand].goesBust = false;
    player.hands[newHand].gotBlackJack = false;
    player.hands[newHand].softHand = false;
    player.hands[newHand].canSplit = false;
    player.hands[newHand].firstTwoCards = true;
    player.hands[newHand].push = false;
    player.hands[newHand].wins = false;
    player.hands[newHand].total = 0;

    player.hands[splitHand].canSplit = false;
    player.hands[splitHand].firstTwoCards = true;

    for (int i = 0; i < 20; i++)
    {
        player.hands[newHand].cards[i].value = 0;
        player.hands[newHand].cards[i].inDeck = false;
    }
    // Pull the card from first hand
    player.hands[splitHand].cards[1].inDeck = false;
    player.hands[splitHand].total -= player.hands[splitHand].cards[1].value;

    // Put the card in the next hand
    player.hands[newHand].cards[0].inDeck = true;

    // Copy the name and value.
    player.hands[newHand].cards[0].name = player.hands[splitHand].cards[1].name;
    player.hands[newHand].cards[0].value = player.hands[splitHand].cards[1].value;
    player.hands[newHand].total += player.hands[splitHand].cards[1].value;

    // Draw one new card for each hand.
    player.hands[splitHand].cards[1] = deck.draw();
    player.hands[splitHand].cards[1].inDeck = true;
    player.hands[splitHand].total += player.hands[splitHand].cards[1].value;
/*
    player.hands[splitHand].cards[1].value = 10;
    player.hands[splitHand].cards[1].name = 'Q';
    player.hands[splitHand].cards[1].inDeck = true;
    player.hands[splitHand].total += 10;*/

    player.hands[newHand].cards[1] = deck.draw();
    player.hands[newHand].cards[1].inDeck = true;
    player.hands[newHand].total += player.hands[newHand].cards[1].value;

    for (int b = 0; b < player.activeHands; b++)
    {
        if (player.hands[b].total == 21)
        {
            player.hands[b].stands = true;
            player.hands[b].gotBlackJack = true;
            player.hands[b].firstTwoCards = false;
        }
        if ((player.hands[b].cards[0].value == 11) || (player.hands[b].cards[1].value == 11))
        {
            if (player.hands[b].total < 21)
            {
                player.hands[b].softHand = true;
            }
        }
        if (player.hands[b].cards[0].value == player.hands[b].cards[1].value)
        {
            player.hands[b].canSplit = true;
        }
        aceChanger(player.hands[b]);
    }
}

void HiLo(Strategy &strat)
{
    // Dealer card         2   3   4   5   6   7   8   9   T   A
char twoCardHardU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          'h','h','h','h','h','h','h','h','h','h', // 5
                          'h','h','h','h','h','h','h','h','h','h', // 6
                          'h','h','h','h','h','h','h','h','h','h', // 7
                          'h','h','h','d','d','h','h','h','h','h', // 8
                          'd','d','d','d','d','d','h','h','h','h', // 9
                          'd','d','d','d','d','d','d','d','d','d', // 10
                          'd','d','d','d','d','d','d','d','d','d', // 11
                          's','s','s','s','s','h','h','h','h','h', // 12
                          's','s','s','s','s','h','h','h','h','h', // 13
                          's','s','s','s','s','h','h','h','r','h', // 14
                          's','s','s','s','s','h','h','r','r','r', // 15
                          's','s','s','s','s','h','h','s','s','s', // 16
                          's','s','s','s','s','s','s','s','s','s', // 17
                          's','s','s','s','s','s','s','s','s','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
char twoCardSoftU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          '*','*','*','*','*','*','*','*','*','*', // 5
                          '*','*','*','*','*','*','*','*','*','*', // 6
                          '*','*','*','*','*','*','*','*','*','*', // 7
                          '*','*','*','*','*','*','*','*','*','*', // 8
                          '*','*','*','*','*','*','*','*','*','*', // 9
                          '*','*','*','*','*','*','*','*','*','*', // 10
                          '*','*','*','*','*','*','*','*','*','*', // 11
                          'p','p','p','p','p','p','p','p','p','p', // 12
                          'h','h','d','d','d','h','h','h','h','h', // 13
                          'h','h','d','d','d','h','h','h','h','h', // 14
                          'h','h','d','d','d','h','h','h','h','h', // 15
                          'h','h','d','d','d','h','h','h','h','h', // 16
                          'd','d','d','d','d','h','h','h','h','h', // 17
                          's','d','d','d','d','s','s','h','h','s', // 18
                          's','s','s','s','d','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21

     // Dealer card         2   3   4   5   6   7   8   9   T   A
 char twoCardPairU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           'p','p','p','p','p','p','h','h','h','h', // 2,2
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           'p','p','p','p','p','p','p','h','h','h', // 3,3
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           'h','h','p','p','p','h','h','h','h','h', // 4,4
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           'd','d','d','d','d','d','d','d','d','d', // 5,5
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'p','p','p','p','p','p','h','h','h','h', // 6,6
                           '*','*','*','*','*','*','*','*','*','*', // 13
                           'p','p','p','p','p','p','p','h','r','h', // 7,7
                           '*','*','*','*','*','*','*','*','*','*', // 15
                           'p','p','p','p','p','p','p','p','p','p', // 8,8
                           '*','*','*','*','*','*','*','*','*','*', // 17
                           'p','p','p','p','p','s','p','p','s','s', // 9,9
                           '*','*','*','*','*','*','*','*','*','*', // 19
                           's','s','s','p','p','s','s','s','s','s', // T,T
                           '*','*','*','*','*','*','*','*','*','*'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
       char hardU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          'h','h','h','h','h','h','h','h','h','h', // 4
                          'h','h','h','h','h','h','h','h','h','h', // 5
                          'h','h','h','h','h','h','h','h','h','h', // 6
                          'h','h','h','h','h','h','h','h','h','h', // 7
                          'h','h','h','h','h','h','h','h','h','h', // 8
                          'h','h','h','h','h','h','h','h','h','h', // 9
                          'h','h','h','h','h','h','h','h','h','h', // 10
                          'h','h','h','h','h','h','h','h','h','h', // 11
                          's','s','s','s','s','h','h','h','h','h', // 12
                          's','s','s','s','s','h','h','h','h','h', // 13
                          's','s','s','s','s','h','h','h','h','h', // 14
                          's','s','s','s','s','h','h','h','s','h', // 15
                          's','s','s','s','s','h','h','s','s','h', // 16
                          's','s','s','s','s','s','s','s','s','s', // 17
                          's','s','s','s','s','s','s','s','s','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
        char softU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           '*','*','*','*','*','*','*','*','*','*', // 4
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           '*','*','*','*','*','*','*','*','*','*', // 6
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           '*','*','*','*','*','*','*','*','*','*', // 8
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           '*','*','*','*','*','*','*','*','*','*', // 10
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'h','h','h','d','d','h','h','h','h','h', // 12
                           'h','h','h','h','h','h','h','h','h','h', // 13
                           'h','h','h','h','h','h','h','h','h','h', // 14
                           'h','h','h','h','h','h','h','h','h','h', // 15
                           'h','h','h','h','h','h','h','h','h','h', // 16
                           'h','h','h','h','h','h','h','h','h','h', // 17
                           's','s','s','s','s','s','s','h','h','s', // 18
                           's','s','s','s','s','s','s','s','s','s', // 19
                           's','s','s','s','s','s','s','s','s','s', // 20
                           's','s','s','s','s','s','s','s','s','s'};// 21

        // Dealer card         2   3   4   5   6   7   8   9   T   A
        char PairU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           'h','h','h','h','h','h','h','h','h','h', // 2,2
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           'h','h','h','h','h','h','h','h','h','h', // 3,3
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           'h','h','h','h','h','h','h','h','h','h', // 4,4
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           'd','d','d','d','d','d','d','d','h','h', // 5,5
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'h','h','h','h','h','h','h','h','h','h', // 6,6
                           '*','*','*','*','*','*','*','*','*','*', // 13
                           's','s','s','s','s','s','h','h','r','h', // 7,7
                           '*','*','*','*','*','*','*','*','*','*', // 15
                           's','s','s','s','s','s','h','h','r','h', // 8,8
                           '*','*','*','*','*','*','*','*','*','*', // 17
                           's','s','s','s','s','s','s','s','s','s', // 9,9
                           '*','*','*','*','*','*','*','*','*','*', // 19
                           's','s','s','s','s','s','s','s','s','s', // T,T
                           '*','*','*','*','*','*','*','*','*','*'};// 21

    // Dealer Card             2    3    4    5    6    7    8    9    T    A
       int hardIndex[210] = {-100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 1
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 2
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 3
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 4
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 5
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 6
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 7
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 8
                               1 ,-100,-100,-100,-100,  3 ,-100,-100,-100,-100, // 9
                             -100,-100,-100,-100,-100,-100,-100,-100,  4 ,  4 , // 10
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,  1 , // 11
                               3 ,  2 ,  0 , -2 , -1 ,-100,-100,-100,-100,-100, // 12
                              -1 , -2 ,-100,-100,-100,-100,-100,-100,-100,-100, // 13
                             -100,-100,-100,-100,-100,-100,-100,-100,  3 ,-100, // 14
                             -100,-100,-100,-100,-100,-100,-100,  2 ,  0 ,  1 , // 15
                             -100,-100,-100,-100,-100,-100,-100,  5 ,  0 ,-100, // 16
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 17
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 18
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 19
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 20
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100};// 21

       // Dealer Card          2    3    4    5    6    7    8    9    T    A
       int softIndex[210] = {-100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 1
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 2
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 3
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 4
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 5
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 6
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 7
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 8
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 9
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 10
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 11
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 12
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 13
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 14
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 15
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 16
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 17
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 18
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 19
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 20
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100};// 21

    // Dealer Card             2    3    4    5    6    7    8    9    T    A
       int pairIndex[210] = {-100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 1
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 2
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 3
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 2,2
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 5
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 3,3
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 7
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 4,4
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 9
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 5,5
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 11
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 6,6
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 13
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 7,7
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 15
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 8,8
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 17
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 9,9
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 19
                             -100,-100,-100,  5 ,  4 ,-100,-100,-100,-100,-100, // T,T
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100};// A,A

char twoCardHardL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          'h','h','h','h','h','h','h','h','h','h', // 5
                          'h','h','h','h','h','h','h','h','h','h', // 6
                          'h','h','h','h','h','h','h','h','h','h', // 7
                          'h','h','h','d','d','h','h','h','h','h', // 8
                          'h','d','d','d','d','h','h','h','h','h', // 9
                          'd','d','d','d','d','d','d','d','h','h', // 10
                          'd','d','d','d','d','d','d','d','d','h', // 11
                          'h','h','h','h','h','h','h','h','h','h', // 12
                          'h','h','s','s','s','h','h','h','h','h', // 13
                          's','s','s','s','s','h','h','h','h','h', // 14
                          's','s','s','s','s','h','h','h','h','h', // 15
                          's','s','s','s','s','h','h','h','h','h', // 16
                          's','s','s','s','s','s','s','s','s','s', // 17
                          's','s','s','s','s','s','s','s','s','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21
    // Dealer card         2   3   4   5   6   7   8   9   T   A
char twoCardSoftL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          '*','*','*','*','*','*','*','*','*','*', // 5
                          '*','*','*','*','*','*','*','*','*','*', // 6
                          '*','*','*','*','*','*','*','*','*','*', // 7
                          '*','*','*','*','*','*','*','*','*','*', // 8
                          '*','*','*','*','*','*','*','*','*','*', // 9
                          '*','*','*','*','*','*','*','*','*','*', // 10
                          '*','*','*','*','*','*','*','*','*','*', // 11
                          'p','p','p','p','p','p','p','p','p','p', // 12
                          'h','h','d','d','d','h','h','h','h','h', // 13
                          'h','h','d','d','d','h','h','h','h','h', // 14
                          'h','h','d','d','d','h','h','h','h','h', // 15
                          'h','h','d','d','d','h','h','h','h','h', // 16
                          'd','d','d','d','d','h','h','h','h','h', // 17
                          's','d','d','d','d','s','s','h','h','s', // 18
                          's','s','s','s','d','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21
     // Dealer card         2   3   4   5   6   7   8   9   T   A
 char twoCardPairL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           'p','p','p','p','p','p','h','h','h','h', // 2,2
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           'p','p','p','p','p','p','p','h','h','h', // 3,3
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           'h','h','p','p','p','h','h','h','h','h', // 4,4
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           'd','d','d','d','d','d','d','d','h','h', // 5,5
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'p','p','p','p','p','p','h','h','h','h', // 6,6
                           '*','*','*','*','*','*','*','*','*','*', // 13
                           'p','p','p','p','p','p','p','h','r','h', // 7,7
                           '*','*','*','*','*','*','*','*','*','*', // 15
                           'p','p','p','p','p','p','p','p','p','p', // 8,8
                           '*','*','*','*','*','*','*','*','*','*', // 17
                           'p','p','p','p','p','s','p','p','s','s', // 9,9
                           '*','*','*','*','*','*','*','*','*','*', // 19
                           's','s','s','s','s','s','s','s','s','s', // T,T
                           '*','*','*','*','*','*','*','*','*','*'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
       char hardL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          'h','h','h','h','h','h','h','h','h','h', // 5
                          'h','h','h','h','h','h','h','h','h','h', // 6
                          'h','h','h','h','h','h','h','h','h','h', // 7
                          'h','h','h','h','h','h','h','h','h','h', // 8
                          'h','h','h','h','h','h','h','h','h','h', // 9
                          'h','h','h','h','h','h','h','h','h','h', // 10
                          'h','h','h','h','h','h','h','h','h','h', // 11
                          'h','h','h','h','h','h','h','h','h','h', // 12
                          'h','h','s','s','s','h','h','h','h','h', // 13
                          's','s','s','s','s','h','h','h','h','h', // 14
                          's','s','s','s','s','h','h','h','h','h', // 15
                          's','s','s','s','s','h','h','h','h','h', // 16
                          's','s','s','s','s','s','s','s','s','s', // 17
                          's','s','s','s','s','s','s','s','s','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21
    // Dealer card         2   3   4   5   6   7   8   9   T   A
        char softL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           '*','*','*','*','*','*','*','*','*','*', // 4
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           '*','*','*','*','*','*','*','*','*','*', // 6
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           '*','*','*','*','*','*','*','*','*','*', // 8
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           '*','*','*','*','*','*','*','*','*','*', // 10
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'h','h','h','d','d','h','h','h','h','h', // 12
                           'h','h','h','h','h','h','h','h','h','h', // 13
                           'h','h','h','h','h','h','h','h','h','h', // 14
                           'h','h','h','h','h','h','h','h','h','h', // 15
                           'h','h','h','h','h','h','h','h','h','h', // 16
                           'h','h','h','h','h','h','h','h','h','h', // 17
                           's','s','s','s','s','s','s','h','h','s', // 18
                           's','s','s','s','s','s','s','s','s','s', // 19
                           's','s','s','s','s','s','s','s','s','s', // 20
                           's','s','s','s','s','s','s','s','s','s'};// 21

        // Dealer card         2   3   4   5   6   7   8   9   T   A
        char PairL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           'h','h','h','h','h','h','h','h','h','h', // 2,2
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           'h','h','h','h','h','h','h','h','h','h', // 3,3
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           'h','h','h','h','h','h','h','h','h','h', // 4,4
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           'd','d','d','d','d','d','d','d','h','h', // 5,5
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'h','h','h','h','h','h','h','h','h','h', // 6,6
                           '*','*','*','*','*','*','*','*','*','*', // 13
                           's','s','s','s','s','s','h','h','r','h', // 7,7
                           '*','*','*','*','*','*','*','*','*','*', // 15
                           's','s','s','s','s','s','h','h','r','h', // 8,8
                           '*','*','*','*','*','*','*','*','*','*', // 17
                           's','s','s','s','s','s','s','s','s','s', // 9,9
                           '*','*','*','*','*','*','*','*','*','*', // 19
                           's','s','s','s','s','s','s','s','s','s', // T,T
                           '*','*','*','*','*','*','*','*','*','*'};// 21


    // Initiate
    int play, deal, type, firstTwo, upper;
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 0; // 0,1,2
        firstTwo = 1; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardHardU[i];
        strat.index[play][deal][type] = hardIndex[i];
        strat.data[play][deal][type] = 0;
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 1; // 0,1,2
        firstTwo = 1; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardSoftU[i];
        strat.index[play][deal][type] = softIndex[i];
        strat.data[play][deal][type] = 0;
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 2; // 0,1,2
        firstTwo = 1; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardPairU[i];
        strat.index[play][deal][type] = pairIndex[i];
        strat.data[play][deal][type] = 0;
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 0; // 0,1,2
        firstTwo = 0; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = hardU[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 1; // 0,1,2
        firstTwo = 0; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = softU[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 2; // 0,1,2
        firstTwo = 0; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = PairU[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 0; // 0,1,2
        firstTwo = 1; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardHardL[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 1; // 0,1,2
        firstTwo = 1; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardSoftL[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 2; // 0,1,2
        firstTwo = 1; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardPairL[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 0; // 0,1,2
        firstTwo = 0; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = hardL[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 1; // 0,1,2
        firstTwo = 0; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = softL[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 2; // 0,1,2
        firstTwo = 0; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = PairL[i];
    }
    strat.name = "HiLo";
}

char strategy(Hand hand, DEALER Dealer, DECK deck, Strategy &strat)
{
    int c;
    string name;
    int play, deal, type, firstTwo, upper;
    char advice;
    play = hand.total - 1;
    deal = Dealer.hand[0].value - 2;
    type = 0;
    if (hand.softHand)
        type = 1;
    else if (hand.canSplit)
        type = 2;
    else if ((hand.canSplit) && (hand.softHand))
        type = 1;

    firstTwo = hand.firstTwoCards;

    if ((strat.name == "HiLo")
     || (strat.name == "HiLo lite"))
    {
        c = deck.trueHiLo();
        name = "HiLo";

        if (c < strat.index[play][deal][type])
            upper = 0;
        else
            upper = 1;
    }
    else if ((strat.name == "Advanced Omega II")
          || (strat.name == "Experimental")
          || (strat.name == "Omega Lite"))
    {
        name = "Omega";
        c = deck.trueOmega();
        if (c < strat.index[play][deal][type])
            upper = 0;
        else
            upper = 1;
    }
    else if (strat.name == "Basic")
    {
        upper = 1;
    }
    else if (strat.name == "Basic Omega")
    {
        name = "Omega";
        c = deck.trueOmega();
        if (c < -6)
            upper = 0;
        else if ((c >= -6) && (c <= 6))
            upper = 1;
        else
            upper = 2;
    }

    strat.data[play][deal][type]++;
    advice = strat.moves[play][deal][type][firstTwo][upper];

    if (deck.returnDisplay())
    {
        char guess;
        do
        {
            cin >> guess;
            if (guess != advice)
            {
                cout << "Wrong, the " << name << " is " << c << endl;
            }
        }
        while (guess != advice);

    }

    switch(advice)
    {
        case 's':
            {
                if (deck.returnDisplay())
                {
                   // cout << "stand\n";
                }
                return 's';
            }
        break;
        case 'h':
            {
                if (deck.returnDisplay())
                {
                  //  cout << "hit\n";
                }
                return 'h';
            }
        break;
        case 'd':
            {
                if (deck.returnDisplay())
                {
                 //   cout << "double\n";
                }
                return 'd';
            }
        break;
        case 'p':
            {
                if (deck.returnDisplay())
                {
                  //  cout << "split\n";
                }
                return 'p';
            }
        break;
        case 'r':
            {
                if (deck.returnDisplay())
                {
                  //  cout << "surrender\n";
                }
                return 'r';
            }
        break;
        case '*':
            {
                Dealer.show();
                for (int i = 0; i < 20; i++)
                {
                    if (hand.cards[i].inDeck)
                    {
                        cout << hand.cards[i].name << " ";
                    }
                }
                cout << endl;
                cout << "**************************";
                cout << strat.name << endl;
                cout << "True count: " << c << endl;
                cout << "Error in strategy\n";
                cout << "Player - 1: " << play << endl;
                cout << "Dealer - 2: " << deal << endl;
                cout << "0hard, 1soft, 2pair: " << type << endl;
                cout << "First two cards? " << firstTwo << endl;
                cout << "Above index count? " << upper << endl;
                cin.get();
            }
        default:
            {
                Dealer.show();
                for (int i = 0; i < 20; i++)
                {
                    if (hand.cards[i].inDeck)
                    {
                        cout << hand.cards[i].name << " ";
                    }
                }
                cout << endl;
                cout << "Error in strategy\n";
                cout << "Player - 1: " << play << endl;
                cout << "Dealer - 2: " << deal << endl;
                cout << "0hard, 1soft, 2pair: " << type << endl;
                cout << "First two cards? " << firstTwo << endl;
                cout << "Above index count? " << upper << endl;
                cin.get();
            }
    }
    cout << "What the fuck???\n";
    cin.get();
    return 'n';
}

void insurance(PLAYER &player, DEALER dealer, DECK deck, int p)
{
    int c;
    char choice;
    int decisionPoint;
    if (p == 1)
    {
        c = deck.trueHiLo();
        decisionPoint = 1;
    }
    else if (p == 2)
    {
        c = deck.trueOmega();
        decisionPoint = 6;
    }
    else if (p == 3)
    {
        c = deck.trueHiLo();
        decisionPoint = 20;
    }
    else
    {
        cout << "Error in insurance\n";
        cin.get();
    }

    if (dealer.hand[0].value == 11)
    {
        if (deck.returnDisplay())
        {
            cout << "Would you like to buy insurance? y/n \n";
        }
        // cin >> choice;
       // if (c >= 3)
        if (c > decisionPoint)
        {
            if (deck.returnDisplay())
            {
                cout << "You should take insurance\n";
            }
            choice = 'y';
        }
        //else if (c < 3)
        else if (c <= decisionPoint)
        {
            if (deck.returnDisplay())
            {
                cout << "Fuck insurance\n";
            }
            choice = 'n';
        }
        if ((choice == 'y') && (dealer.gotBlackjack))
        {
            if (deck.returnDisplay())
            {
                cout << "Your insurance paid off!!!\n";
            }
            player.bankroll += player.betSize;
        }
        else if ((choice == 'y') && (!(dealer.gotBlackjack)))
        {
            if (deck.returnDisplay())
            {
                cout << "You lost your insurance.\n";
            }
            player.bankroll -= (.5 * player.betSize);
        }
    }
}

bool initialBlackJack(PLAYER Player, DEALER Dealer, DECK deck)
{
    if (Player.hands[0].gotBlackJack && (Dealer.total < 21))
    {
        Dealer.revealHoleCard = true;
        if (deck.returnDisplay())
        {
            Dealer.show();
            show(Player);
            cout << "Player Blackjack!!!\n";
        }
        Player.bankroll += (1.5 * Player.betSize);
        return true;
    }
    else if (Dealer.gotBlackjack && (Player.hands[0].total < 21))
    {
        Dealer.revealHoleCard = true;
        if (deck.returnDisplay())
        {
            Dealer.show();
            show(Player);
            cout << "Dealer Blackjack...\n";
        }
        Player.bankroll -= Player.betSize;
        return true;
    }
    else if (Player.hands[0].gotBlackJack && Dealer.gotBlackjack)
    {
        Dealer.revealHoleCard = true;
        if (deck.returnDisplay())
        {
            Dealer.show();
            show(Player);
            cout << "Blackjack Push\n";
            Player.hands[0].push = true;
        }
        return true;
    }
    else
        return false;
}

void betSize(PLAYER &player, DECK deck, int p)
{
    int c;
    string name;
    if (p == 1)
    {
        c = deck.trueHiLo();
        name = "HiLo";
    }
    else if (p == 2)
    {
        c = deck.trueOmega();
        name = "Omega";
    }
    else if (p == 3)
    {
        c = deck.aceAdjusted();
        name = "Ace Omega";
    }

    if ((c >= 1) && (c <= (player.betSpread)))
        player.betSize = (c*player.betIncrement);
    else if (c < 1)
        player.betSize = player.betIncrement;
    else if (c > player.betSpread)
        player.betSize = (player.betSpread * player.betIncrement);
    else
        player.betSize = player.betIncrement;

    if (deck.returnDisplay())
    {
        int guess;
        do
        {
            deck.shoeSize();
            cout << "Enter bet: ";
            cin >> guess;
            if (guess != player.betSize)
            {
                cout << "Wrong, what is the true count?";
                cin >> guess;
                if (guess == c)
                {
                    cout << "Correct\n";
                }
                else
                {
                    cout << "The " << name << " is " << c;
                    cout << " so the correct bet is " << player.betSize << endl;
                }
            }
        }
        while (guess != player.betSize);
    }
}

void realBetSize(PLAYER &player, DECK deck, int p)
{
    int c;
    string name;
    if (p == 1)
    {
        c = deck.trueHiLo();
        name = "HiLo";
    }
    else if (p == 2)
    {
        c = deck.trueOmega();
        name = "Omega";
    }
    else if (p == 3)
    {
        c = deck.aceAdjusted();
        name = "Ace Omega";
    }
    // 5, 10, 20
    if (!(player.hands[0].push))
    {
        if ((c >= 1) && (c <= 2))
            player.betSize = (2*player.betIncrement);
        else if (c < 1)
            player.betSize = player.betIncrement;
        else if (c > 2)
            player.betSize = (player.betSpread * player.betIncrement);
        else
            player.betSize = player.betIncrement;
    }

    if (deck.returnDisplay())
    {
        cout << "Bet: " << player.betSize << endl;
        cout << name << ": " << c << endl;
    }
}


void HiLoLite(Strategy &strat)
{
    // Dealer card         2   3   4   5   6   7   8   9   T   A
char twoCardHardU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          'h','h','h','h','h','h','h','h','h','h', // 5
                          'h','h','h','h','h','h','h','h','h','h', // 6
                          'h','h','h','h','h','h','h','h','h','h', // 7
                          'h','h','h','d','d','h','h','h','h','h', // 8
                          'd','d','d','d','d','d','h','h','h','h', // 9
                          'd','d','d','d','d','d','d','d','d','d', // 10
                          'd','d','d','d','d','d','d','d','d','d', // 11
                          's','s','s','s','s','h','h','h','h','h', // 12
                          's','s','s','s','s','h','h','h','h','h', // 13
                          's','s','s','s','s','h','h','h','h','r', // 14
                          's','s','s','s','s','h','h','r','r','r', // 15
                          's','s','s','s','s','h','r','r','r','r', // 16
                          's','s','s','s','s','s','s','s','s','s', // 17
                          's','s','s','s','s','s','s','s','s','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
char twoCardSoftU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          '*','*','*','*','*','*','*','*','*','*', // 5
                          '*','*','*','*','*','*','*','*','*','*', // 6
                          '*','*','*','*','*','*','*','*','*','*', // 7
                          '*','*','*','*','*','*','*','*','*','*', // 8
                          '*','*','*','*','*','*','*','*','*','*', // 9
                          '*','*','*','*','*','*','*','*','*','*', // 10
                          '*','*','*','*','*','*','*','*','*','*', // 11
                          'p','p','p','p','p','p','p','p','p','p', // 12
                          'h','h','d','d','d','h','h','h','h','h', // 13
                          'h','h','d','d','d','h','h','h','h','h', // 14
                          'h','h','d','d','d','h','h','h','h','h', // 15
                          'h','h','d','d','d','h','h','h','h','h', // 16
                          'd','d','d','d','d','h','h','h','h','h', // 17
                          's','d','d','d','d','s','s','h','h','s', // 18
                          's','s','s','s','d','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21

     // Dealer card         2   3   4   5   6   7   8   9   T   A
 char twoCardPairU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           'p','p','p','p','p','p','h','h','h','h', // 2,2
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           'p','p','p','p','p','p','p','h','h','h', // 3,3
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           'h','h','p','p','p','h','h','h','h','h', // 4,4
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           'd','d','d','d','d','d','d','d','h','h', // 5,5
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'p','p','p','p','p','p','h','h','h','h', // 6,6
                           '*','*','*','*','*','*','*','*','*','*', // 13
                           'p','p','p','p','p','p','p','h','r','h', // 7,7
                           '*','*','*','*','*','*','*','*','*','*', // 15
                           'p','p','p','p','p','p','p','p','p','p', // 8,8
                           '*','*','*','*','*','*','*','*','*','*', // 17
                           'p','p','p','p','p','s','p','p','s','s', // 9,9
                           '*','*','*','*','*','*','*','*','*','*', // 19
                           's','p','p','p','p','s','s','s','s','s', // T,T
                           '*','*','*','*','*','*','*','*','*','*'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
       char hardU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          'h','h','h','h','h','h','h','h','h','h', // 4
                          'h','h','h','h','h','h','h','h','h','h', // 5
                          'h','h','h','h','h','h','h','h','h','h', // 6
                          'h','h','h','h','h','h','h','h','h','h', // 7
                          'h','h','h','h','h','h','h','h','h','h', // 8
                          'h','h','h','h','h','h','h','h','h','h', // 9
                          'h','h','h','h','h','h','h','h','h','h', // 10
                          'h','h','h','h','h','h','h','h','h','h', // 11
                          's','s','s','s','s','h','h','h','h','h', // 12
                          's','s','s','s','s','h','h','h','h','h', // 13
                          's','s','s','s','s','h','h','h','h','s', // 14
                          's','s','s','s','s','h','h','s','s','s', // 15
                          's','s','s','s','s','h','s','s','s','s', // 16
                          's','s','s','s','s','s','s','s','s','s', // 17
                          's','s','s','s','s','s','s','s','s','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
        char softU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           '*','*','*','*','*','*','*','*','*','*', // 4
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           '*','*','*','*','*','*','*','*','*','*', // 6
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           '*','*','*','*','*','*','*','*','*','*', // 8
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           '*','*','*','*','*','*','*','*','*','*', // 10
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'h','h','h','d','d','h','h','h','h','h', // 12
                           'h','h','h','h','h','h','h','h','h','h', // 13
                           'h','h','h','h','h','h','h','h','h','h', // 14
                           'h','h','h','h','h','h','h','h','h','h', // 15
                           'h','h','h','h','h','h','h','h','h','h', // 16
                           'h','h','h','h','h','h','h','h','h','h', // 17
                           's','s','s','s','s','s','s','h','h','s', // 18
                           's','s','s','s','s','s','s','s','s','s', // 19
                           's','s','s','s','s','s','s','s','s','s', // 20
                           's','s','s','s','s','s','s','s','s','s'};// 21

        // Dealer card         2   3   4   5   6   7   8   9   T   A
        char PairU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           'h','h','h','h','h','h','h','h','h','h', // 2,2
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           'h','h','h','h','h','h','h','h','h','h', // 3,3
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           'h','h','h','h','h','h','h','h','h','h', // 4,4
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           'd','d','d','d','d','d','d','d','d','d', // 5,5
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'h','h','h','h','h','h','h','h','h','h', // 6,6
                           '*','*','*','*','*','*','*','*','*','*', // 13
                           's','s','s','s','s','s','h','h','r','h', // 7,7
                           '*','*','*','*','*','*','*','*','*','*', // 15
                           's','s','s','s','s','s','h','h','r','h', // 8,8
                           '*','*','*','*','*','*','*','*','*','*', // 17
                           's','s','s','s','s','s','s','s','s','s', // 9,9
                           '*','*','*','*','*','*','*','*','*','*', // 19
                           's','s','s','s','s','s','s','s','s','s', // T,T
                           '*','*','*','*','*','*','*','*','*','*'};// 21

    // Dealer Card             2    3    4    5    6    7    8    9    T    A
       int hardIndex[210] = {-100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 1
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 2
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 3
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 4
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 5
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 6
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 7
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 8
                               1 ,-100,-100,-100,-100,  5 ,-100,-100,-100,-100, // 9
                             -100,-100,-100,-100,-100,-100, -5 , -1 ,  5 ,  5 , // 10
                             -100,-100,-100,-100,-100,-100, -5 , -5 , -5 , -1 , // 11
                               5 ,  1 ,  1 , -1 , -5 ,-100,-100,-100,-100,-100, // 12
                              -1 , -1 , -5 , -5 ,-100,-100,-100,-100,-100,-100, // 13
                              -5 , -5 ,-100,-100,-100,-100,-100,-100,-100, 10 , // 14
                              -2 ,-100,-100,-100,-100,-100,-100, 10 ,  5 ,  5 , // 15
                             -100,-100,-100,-100,-100,-100, 10 ,  5 ,  1 ,  5 , // 16
                             -100,-100,-100,-100,-100,-100,-100,-100,-100, -5 , // 17
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 18
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 19
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 20
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100};// 21

       // Dealer Card          2    3    4    5    6    7    8    9    T    A
       int softIndex[210] = {-100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 1
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 2
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 3
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 4
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 5
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 6
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 7
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 8
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 9
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 10
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 11
                             -100,-100,-100,-100,-100,-100,-100,-100,-100, -5 , // 12
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 13
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 14
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 15
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 16
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 17
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 18
                             -100,-100,-100,-100,  1 ,-100,-100,-100,-100,-100, // 19
                              10 , 10 , 10 ,  5 ,  5 ,-100,-100,-100,-100,-100, // 20
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100};// 21

    // Dealer Card             2    3    4    5    6    7    8    9    T    A
       int pairIndex[210] = {-100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 1
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 2
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 3
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 2,2
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 5
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 3,3
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 7
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 4,4
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 9
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 5,5
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 11
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 6,6
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 13
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 7,7
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 15
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 8,8
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 17
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 9,9
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 19
                             -100,  10,  10,  5 ,  5 ,-100,-100,-100,-100,-100, // T,T
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100};// A,A
// Dealer card             2   3   4   5   6   7   8   9   T   A
char twoCardHardL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          'h','h','h','h','h','h','h','h','h','h', // 5
                          'h','h','h','h','h','h','h','h','h','h', // 6
                          'h','h','h','h','h','h','h','h','h','h', // 7
                          'h','h','h','d','d','h','h','h','h','h', // 8
                          'h','d','d','d','d','h','h','h','h','h', // 9
                          'd','d','d','d','d','d','h','h','h','h', // 10
                          'd','d','d','d','d','d','h','h','h','h', // 11
                          'h','h','h','h','h','h','h','h','h','h', // 12
                          'h','h','h','h','s','h','h','h','h','h', // 13
                          'h','h','s','s','s','h','h','h','h','h', // 14
                          'h','s','s','s','s','h','h','h','h','h', // 15
                          's','s','s','s','s','h','h','h','h','h', // 16
                          's','s','s','s','s','s','s','s','s','h', // 17
                          's','s','s','s','s','s','s','s','s','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21
    // Dealer card         2   3   4   5   6   7   8   9   T   A
char twoCardSoftL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          '*','*','*','*','*','*','*','*','*','*', // 5
                          '*','*','*','*','*','*','*','*','*','*', // 6
                          '*','*','*','*','*','*','*','*','*','*', // 7
                          '*','*','*','*','*','*','*','*','*','*', // 8
                          '*','*','*','*','*','*','*','*','*','*', // 9
                          '*','*','*','*','*','*','*','*','*','*', // 10
                          '*','*','*','*','*','*','*','*','*','*', // 11
                          'p','p','p','p','p','p','p','p','p','h', // 12
                          'h','h','d','d','d','h','h','h','h','h', // 13
                          'h','h','d','d','d','h','h','h','h','h', // 14
                          'h','h','d','d','d','h','h','h','h','h', // 15
                          'h','h','d','d','d','h','h','h','h','h', // 16
                          'd','d','d','d','d','h','h','h','h','h', // 17
                          's','d','d','d','d','s','s','h','h','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21
     // Dealer card         2   3   4   5   6   7   8   9   T   A
 char twoCardPairL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           'p','p','p','p','p','p','h','h','h','h', // 2,2
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           'p','p','p','p','p','p','p','h','h','h', // 3,3
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           'h','h','p','p','p','h','h','h','h','h', // 4,4
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           'd','d','d','d','d','d','d','d','h','h', // 5,5
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'p','p','p','p','p','p','h','h','h','h', // 6,6
                           '*','*','*','*','*','*','*','*','*','*', // 13
                           'p','p','p','p','p','p','p','h','r','h', // 7,7
                           '*','*','*','*','*','*','*','*','*','*', // 15
                           'p','p','p','p','p','p','p','p','p','p', // 8,8
                           '*','*','*','*','*','*','*','*','*','*', // 17
                           'p','p','p','p','p','s','p','p','s','s', // 9,9
                           '*','*','*','*','*','*','*','*','*','*', // 19
                           's','s','s','s','s','s','s','s','s','s', // T,T
                           '*','*','*','*','*','*','*','*','*','*'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
       char hardL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          'h','h','h','h','h','h','h','h','h','h', // 5
                          'h','h','h','h','h','h','h','h','h','h', // 6
                          'h','h','h','h','h','h','h','h','h','h', // 7
                          'h','h','h','h','h','h','h','h','h','h', // 8
                          'h','h','h','h','h','h','h','h','h','h', // 9
                          'h','h','h','h','h','h','h','h','h','h', // 10
                          'h','h','h','h','h','h','h','h','h','h', // 11
                          'h','h','h','h','h','h','h','h','h','h', // 12
                          'h','h','h','h','s','h','h','h','h','h', // 13
                          'h','h','s','s','s','h','h','h','h','h', // 14
                          'h','s','s','s','s','h','h','h','h','h', // 15
                          's','s','s','s','s','h','h','h','h','h', // 16
                          's','s','s','s','s','s','s','s','s','h', // 17
                          's','s','s','s','s','s','s','s','s','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21
    // Dealer card         2   3   4   5   6   7   8   9   T   A
        char softL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           '*','*','*','*','*','*','*','*','*','*', // 4
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           '*','*','*','*','*','*','*','*','*','*', // 6
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           '*','*','*','*','*','*','*','*','*','*', // 8
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           '*','*','*','*','*','*','*','*','*','*', // 10
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'h','h','h','d','d','h','h','h','h','h', // 12
                           'h','h','h','h','h','h','h','h','h','h', // 13
                           'h','h','h','h','h','h','h','h','h','h', // 14
                           'h','h','h','h','h','h','h','h','h','h', // 15
                           'h','h','h','h','h','h','h','h','h','h', // 16
                           'h','h','h','h','h','h','h','h','h','h', // 17
                           's','s','s','s','s','s','s','h','h','s', // 18
                           's','s','s','s','s','s','s','s','s','s', // 19
                           's','s','s','s','s','s','s','s','s','s', // 20
                           's','s','s','s','s','s','s','s','s','s'};// 21

        // Dealer card         2   3   4   5   6   7   8   9   T   A
        char PairL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           'h','h','h','h','h','h','h','h','h','h', // 2,2
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           'h','h','h','h','h','h','h','h','h','h', // 3,3
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           'h','h','h','h','h','h','h','h','h','h', // 4,4
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           'd','d','d','d','d','d','d','d','h','h', // 5,5
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'h','h','h','h','h','h','h','h','h','h', // 6,6
                           '*','*','*','*','*','*','*','*','*','*', // 13
                           's','s','s','s','s','s','h','h','r','h', // 7,7
                           '*','*','*','*','*','*','*','*','*','*', // 15
                           's','s','s','s','s','s','h','h','r','h', // 8,8
                           '*','*','*','*','*','*','*','*','*','*', // 17
                           's','s','s','s','s','s','s','s','s','s', // 9,9
                           '*','*','*','*','*','*','*','*','*','*', // 19
                           's','s','s','s','s','s','s','s','s','s', // T,T
                           '*','*','*','*','*','*','*','*','*','*'};// 21


                           // maybe add and 8 vs 5,6 index???
    // Initiate
    int play, deal, type, firstTwo, upper;
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 0; // 0,1,2
        firstTwo = 1; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardHardU[i];
        strat.index[play][deal][type] = hardIndex[i];
        strat.data[play][deal][type] = 0;
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 1; // 0,1,2
        firstTwo = 1; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardSoftU[i];
        strat.index[play][deal][type] = softIndex[i];
        strat.data[play][deal][type] = 0;
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 2; // 0,1,2
        firstTwo = 1; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardPairU[i];
        strat.index[play][deal][type] = pairIndex[i];
        strat.data[play][deal][type] = 0;
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 0; // 0,1,2
        firstTwo = 0; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = hardU[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 1; // 0,1,2
        firstTwo = 0; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = softU[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 2; // 0,1,2
        firstTwo = 0; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = PairU[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 0; // 0,1,2
        firstTwo = 1; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardHardL[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 1; // 0,1,2
        firstTwo = 1; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardSoftL[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 2; // 0,1,2
        firstTwo = 1; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardPairL[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 0; // 0,1,2
        firstTwo = 0; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = hardL[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 1; // 0,1,2
        firstTwo = 0; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = softL[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 2; // 0,1,2
        firstTwo = 0; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = PairL[i];
    }
    strat.name = "HiLo lite";
}

void Experimental(Strategy &strat)
{
    // Dealer card         2   3   4   5   6   7   8   9   T   A
char twoCardHardU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          'h','h','h','h','h','h','h','h','h','h', // 5
                          'h','h','h','h','h','h','h','h','h','h', // 6
                          'h','h','h','h','h','h','h','h','h','h', // 7
                          'h','d','d','d','d','h','h','h','h','h', // 8
                          'd','d','d','d','d','d','d','h','h','h', // 9
                          'd','d','d','d','d','d','d','d','d','d', // 10
                          'd','d','d','d','d','d','d','d','d','d', // 11
                          's','s','s','s','s','h','h','h','h','h', // 12
                          's','s','s','s','s','h','h','h','h','h', // 13
                          's','s','s','s','s','h','h','h','s','s', // 14
                          's','s','s','s','s','s','s','s','s','s', // 15
                          's','s','s','s','s','s','s','s','s','s', // 16
                          's','s','s','s','s','s','s','s','s','s', // 17
                          's','s','s','s','s','s','s','s','s','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
char twoCardSoftU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          '*','*','*','*','*','*','*','*','*','*', // 5
                          '*','*','*','*','*','*','*','*','*','*', // 6
                          '*','*','*','*','*','*','*','*','*','*', // 7
                          '*','*','*','*','*','*','*','*','*','*', // 8
                          '*','*','*','*','*','*','*','*','*','*', // 9
                          '*','*','*','*','*','*','*','*','*','*', // 10
                          '*','*','*','*','*','*','*','*','*','*', // 11
                          'p','p','p','p','p','p','p','p','p','p', // 12
                          'h','d','d','d','d','h','h','h','h','h', // 13
                          'h','d','d','d','d','h','h','h','h','h', // 14
                          'h','d','d','d','d','h','h','h','h','h', // 15
                          'h','d','d','d','d','h','h','h','h','h', // 16
                          'd','d','d','d','d','d','h','h','h','h', // 17
                          'd','d','d','d','d','s','s','h','h','s', // 18
                          's','d','d','d','d','s','s','s','s','s', // 19
                          's','d','d','d','d','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21

     // Dealer card         2   3   4   5   6   7   8   9   T   A
 char twoCardPairU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           'p','p','p','p','p','p','p','h','h','h', // 2,2
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           'p','p','p','p','p','h','h','h','h','h', // 3,3
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           'h','p','p','p','p','h','h','h','h','h', // 4,4
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           'd','d','d','d','d','d','d','d','d','d', // 5,5
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'p','p','p','p','p','h','h','h','h','h', // 6,6
                           '*','*','*','*','*','*','*','*','*','*', // 13
                           'p','p','p','p','p','p','p','h','s','s', // 7,7
                           '*','*','*','*','*','*','*','*','*','*', // 15
                           'p','p','p','p','p','p','p','p','s','p', // 8,8
                           '*','*','*','*','*','*','*','*','*','*', // 17
                           'p','p','p','p','p','p','p','p','s','p', // 9,9
                           '*','*','*','*','*','*','*','*','*','*', // 19
                           's','p','p','p','p','s','s','s','s','s', // T,T
                           '*','*','*','*','*','*','*','*','*','*'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
       char hardU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          'h','h','h','h','h','h','h','h','h','h', // 4
                          'h','h','h','h','h','h','h','h','h','h', // 5
                          'h','h','h','h','h','h','h','h','h','h', // 6
                          'h','h','h','h','h','h','h','h','h','h', // 7
                          'h','h','h','h','h','h','h','h','h','h', // 8
                          'h','h','h','h','h','h','h','h','h','h', // 9
                          'h','h','h','h','h','h','h','h','h','h', // 10
                          'h','h','h','h','h','h','h','h','h','h', // 11
                          's','s','s','s','s','h','h','h','h','h', // 12
                          's','s','s','s','s','h','h','h','h','h', // 13
                          's','s','s','s','s','h','h','h','s','s', // 14
                          's','s','s','s','s','s','s','s','s','s', // 15
                          's','s','s','s','s','s','s','s','s','s', // 16
                          's','s','s','s','s','s','s','s','s','s', // 17
                          's','s','s','s','s','s','s','s','s','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
        char softU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           '*','*','*','*','*','*','*','*','*','*', // 4
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           '*','*','*','*','*','*','*','*','*','*', // 6
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           '*','*','*','*','*','*','*','*','*','*', // 8
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           '*','*','*','*','*','*','*','*','*','*', // 10
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'h','h','h','h','h','h','h','h','h','h', // 12
                           'h','h','h','h','h','h','h','h','h','h', // 13
                           'h','h','h','h','h','h','h','h','h','h', // 14
                           'h','h','h','h','h','h','h','h','h','h', // 15
                           'h','h','h','h','h','h','h','h','h','h', // 16
                           'h','h','h','h','h','h','h','h','h','h', // 17
                           's','s','s','s','s','s','s','h','h','s', // 18
                           's','s','s','s','s','s','s','s','s','s', // 19
                           's','s','s','s','s','s','s','s','s','s', // 20
                           's','s','s','s','s','s','s','s','s','s'};// 21

     // Dealer card         2   3   4   5   6   7   8   9   T   A
        char PairU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           'h','h','h','h','h','h','h','h','h','h', // 2,2
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           'h','h','h','h','h','h','h','h','h','h', // 3,3
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           'h','d','d','d','d','h','h','h','h','h', // 4,4
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           'd','d','d','d','d','d','d','d','d','d', // 5,5
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           's','s','s','s','s','h','h','h','h','h', // 6,6
                           '*','*','*','*','*','*','*','*','*','*', // 13
                           's','s','s','s','s','s','h','h','s','h', // 7,7
                           '*','*','*','*','*','*','*','*','*','*', // 15
                           's','s','s','s','s','s','s','s','s','s', // 8,8
                           '*','*','*','*','*','*','*','*','*','*', // 17
                           's','s','s','s','s','s','s','s','s','s', // 9,9
                           '*','*','*','*','*','*','*','*','*','*', // 19
                           's','s','s','s','s','s','s','s','s','s', // T,T
                           '*','*','*','*','*','*','*','*','*','*'};// 21

    // Dealer Card             2    3    4    5    6    7    8    9    T    A
       int hardIndex[210] = {-100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 1
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 2
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 3
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 4
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 5
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 6
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 7
                             -100, 15 , 10 ,  5 ,  5 ,-100,-100,-100,-100,-100, // 8
                               5 ,  0 , -5 , -5 , -10,  5 , 15 ,-100,-100,-100, // 9
                              -15, -15, -15, -20, -20, -10,  -5,  -1, 10 , 10 , // 10
                              -15, -20, -20, -20, -25, -10, -10,  -5, -5 ,  1 , // 11
                               5 ,  1 ,  0 , -1 , -1 ,-100,-100,-100,-100,-100, // 12
                              -1 , -1 , -5 , -10, -10,-100,-100,-100,-100,-100, // 13
                              -5 , -5 , -10, -10, -10,-100,-100,-100, 15 ,  20, // 14
                              -10, -10, -15, -15, -15,  20,  20,  15,  5 ,  20, // 15
                              -15, -15, -20, -20, -20,  15,  15,  5 ,  0 ,  15, // 16
                             -100,-100,-100,-100,-100,-100,-100,-100,-100, -10, // 17
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 18
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 19
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 20
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100};// 21

       // Dealer Card          2    3    4    5    6    7    8    9    T    A
       int softIndex[210] = {-100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 1
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 2
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 3
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 4
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 5
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 6
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 7
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 8
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 9
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 10
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 11
                              -20, -20, -20, -20, -20, -15, -15, -15, -15, -5 , // 12
                             -100, 10 ,  5 ,  0 , -5 ,-100,-100,-100,-100,-100, // 13
                             -100, 10 ,  1 , -5 , -5 ,-100,-100,-100,-100,-100, // 14
                             -100,  10,  0 , -5 , -10,-100,-100,-100,-100,-100, // 15
                             -100,  5 , -5 , -10, -15,-100,-100,-100,-100,-100, // 16
                               1 , -5 , -10, -15, -20, 20 ,-100,-100,-100,-100, // 17
                               1 , -1 , -10, -10, -10,-100, -22,-100,-100,  0 , // 18
                             -100, 10 ,  5 ,  5 ,  1 ,-100,-100,-100,-100,-100, // 19
                             -100, 15 , 15 , 10 , 10 ,-100,-100,-100,-100,-100, // 20
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100};// 21

    // Dealer Card             2    3    4    5    6    7    8    9    T    A
       int pairIndex[210] = {-100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 1
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 2
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 3
                              -5 , -10, -15, -20,-100,-100, 15 ,-100,-100,-100, // 2,2
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 5
                              -5 , -10, -15, -20,-100, 15 ,-100,-100,-100,-100, // 3,3
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 7
                             -100, 15 ,  5 ,  5 ,  0 ,-100,-100,-100,-100,-100, // 4,4
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 9
                             -15 , -15, -15, -20, -20, -10,  -5,  -1, 10 , 10 , // 5,5
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 11
                              -1 , -5 , -10, -10, -20,-100,-100,-100,-100,-100, // 6,6
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 13
                              -15, -20, -20,-100,-100,-100, -1 ,-100,  1 , 20 , // 7,7
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 15
                             -100,-100,-100,-100,-100,-100,-100,-100, 10 ,-100, // 8,8
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 17
                              -5 , -5 , -10, -10, -15,-100,-100,-100,-100, 20 , // 9,9
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 19
                             -100, 15 , 10 , 10 , 10 ,-100,-100,-100,-100,-100, // T,T
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100};// A,A

char twoCardHardL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          'h','h','h','h','h','h','h','h','h','h', // 5
                          'h','h','h','h','h','h','h','h','h','h', // 6
                          'h','h','h','h','h','h','h','h','h','h', // 7
                          'h','h','h','h','h','h','h','h','h','h', // 8
                          'h','h','h','h','h','h','h','h','h','h', // 9
                          'h','h','h','h','h','h','h','h','h','h', // 10
                          'h','h','h','h','h','h','h','h','h','h', // 11
                          'h','h','h','h','h','h','h','h','h','h', // 12
                          'h','h','h','h','h','h','h','h','h','h', // 13
                          'h','h','h','h','h','h','h','h','h','h', // 14
                          'h','h','h','h','h','h','h','h','h','h', // 15
                          'h','h','h','h','h','h','h','h','h','h', // 16
                          's','s','s','s','s','s','s','s','s','h', // 17
                          's','s','s','s','s','s','s','s','s','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21
    // Dealer card         2   3   4   5   6   7   8   9   T   A
char twoCardSoftL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          '*','*','*','*','*','*','*','*','*','*', // 5
                          '*','*','*','*','*','*','*','*','*','*', // 6
                          '*','*','*','*','*','*','*','*','*','*', // 7
                          '*','*','*','*','*','*','*','*','*','*', // 8
                          '*','*','*','*','*','*','*','*','*','*', // 9
                          '*','*','*','*','*','*','*','*','*','*', // 10
                          '*','*','*','*','*','*','*','*','*','*', // 11
                          'h','h','h','h','h','h','h','h','h','h', // 12
                          'h','h','h','h','h','h','h','h','h','h', // 13
                          'h','h','h','h','h','h','h','h','h','h', // 14
                          'h','h','h','h','h','h','h','h','h','h', // 15
                          'h','h','h','h','h','h','h','h','h','h', // 16
                          'h','h','h','h','h','h','h','h','h','h', // 17
                          's','s','s','s','s','s','h','h','h','h', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21
     // Dealer card         2   3   4   5   6   7   8   9   T   A
 char twoCardPairL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           'h','h','h','h','p','p','h','h','h','h', // 2,2
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           'h','h','h','h','p','p','h','h','h','h', // 3,3
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           'h','h','h','h','h','h','h','h','h','h', // 4,4
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           'h','h','h','h','h','h','h','h','h','h', // 5,5
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'h','h','h','h','h','h','h','h','h','h', // 6,6
                           '*','*','*','*','*','*','*','*','*','*', // 13
                           'h','h','h','p','p','p','h','h','h','h', // 7,7
                           '*','*','*','*','*','*','*','*','*','*', // 15
                           'p','p','p','p','p','p','p','p','s','p', // 8,8
                           '*','*','*','*','*','*','*','*','*','*', // 17
                           's','s','s','s','s','s','p','p','s','s', // 9,9
                           '*','*','*','*','*','*','*','*','*','*', // 19
                           's','s','s','s','s','s','s','s','s','s', // T,T
                           '*','*','*','*','*','*','*','*','*','*'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
       char hardL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          'h','h','h','h','h','h','h','h','h','h', // 5
                          'h','h','h','h','h','h','h','h','h','h', // 6
                          'h','h','h','h','h','h','h','h','h','h', // 7
                          'h','h','h','h','h','h','h','h','h','h', // 8
                          'h','h','h','h','h','h','h','h','h','h', // 9
                          'h','h','h','h','h','h','h','h','h','h', // 10
                          'h','h','h','h','h','h','h','h','h','h', // 11
                          'h','h','h','h','h','h','h','h','h','h', // 12
                          'h','h','h','h','h','h','h','h','h','h', // 13
                          'h','h','h','h','h','h','h','h','h','h', // 14
                          'h','h','h','h','h','h','h','h','h','h', // 15
                          'h','h','h','h','h','h','h','h','h','h', // 16
                          's','s','s','s','s','s','s','s','s','h', // 17
                          's','s','s','s','s','s','s','s','s','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21
    // Dealer card         2   3   4   5   6   7   8   9   T   A
        char softL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           '*','*','*','*','*','*','*','*','*','*', // 4
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           '*','*','*','*','*','*','*','*','*','*', // 6
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           '*','*','*','*','*','*','*','*','*','*', // 8
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           '*','*','*','*','*','*','*','*','*','*', // 10
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'h','h','h','h','h','h','h','h','h','h', // 12
                           'h','h','h','h','h','h','h','h','h','h', // 13
                           'h','h','h','h','h','h','h','h','h','h', // 14
                           'h','h','h','h','h','h','h','h','h','h', // 15
                           'h','h','h','h','h','h','h','h','h','h', // 16
                           'h','h','h','h','h','h','h','h','h','h', // 17
                           's','s','s','s','s','s','h','h','h','h', // 18
                           's','s','s','s','s','s','s','s','s','s', // 19
                           's','s','s','s','s','s','s','s','s','s', // 20
                           's','s','s','s','s','s','s','s','s','s'};// 21

        // Dealer card         2   3   4   5   6   7   8   9   T   A
        char PairL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           'h','h','h','h','h','h','h','h','h','h', // 2,2
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           'h','h','h','h','h','h','h','h','h','h', // 3,3
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           'h','h','h','h','h','h','h','h','h','h', // 4,4
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           'h','h','h','h','h','h','h','h','h','h', // 5,5
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'h','h','h','h','h','h','h','h','h','h', // 6,6
                           '*','*','*','*','*','*','*','*','*','*', // 13
                           'h','h','h','h','h','h','h','h','h','h', // 7,7
                           '*','*','*','*','*','*','*','*','*','*', // 15
                           'h','h','h','h','h','h','h','h','s','h', // 8,8
                           '*','*','*','*','*','*','*','*','*','*', // 17
                           's','s','s','s','s','s','s','s','s','s', // 9,9
                           '*','*','*','*','*','*','*','*','*','*', // 19
                           's','s','s','s','s','s','s','s','s','s', // T,T
                           '*','*','*','*','*','*','*','*','*','*'};// 21


    // Initiate
    int play, deal, type, firstTwo, upper;
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 0; // 0,1,2
        firstTwo = 1; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardHardU[i];
        strat.index[play][deal][type] = hardIndex[i];
        strat.data[play][deal][type] = 0;
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 1; // 0,1,2
        firstTwo = 1; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardSoftU[i];
        strat.index[play][deal][type] = softIndex[i];
        strat.data[play][deal][type] = 0;
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 2; // 0,1,2
        firstTwo = 1; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardPairU[i];
        strat.index[play][deal][type] = pairIndex[i];
        strat.data[play][deal][type] = 0;
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 0; // 0,1,2
        firstTwo = 0; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = hardU[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 1; // 0,1,2
        firstTwo = 0; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = softU[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 2; // 0,1,2
        firstTwo = 0; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = PairU[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 0; // 0,1,2
        firstTwo = 1; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardHardL[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 1; // 0,1,2
        firstTwo = 1; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardSoftL[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 2; // 0,1,2
        firstTwo = 1; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardPairL[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 0; // 0,1,2
        firstTwo = 0; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = hardL[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 1; // 0,1,2
        firstTwo = 0; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = softL[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 2; // 0,1,2
        firstTwo = 0; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = PairL[i];
    }
    strat.name = "Experimental";
}

void Basic(Strategy &strat)
{
    // Dealer card         2   3   4   5   6   7   8   9   T   A
char twoCardHardU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          'h','h','h','h','h','h','h','h','h','h', // 5
                          'h','h','h','h','h','h','h','h','h','h', // 6
                          'h','h','h','h','h','h','h','h','h','h', // 7
                          'h','h','h','d','d','h','h','h','h','h', // 8
                          'h','d','d','d','d','h','h','h','h','h', // 9
                          'd','d','d','d','d','d','d','d','h','h', // 10
                          'd','d','d','d','d','d','d','d','d','d', // 11
                          'h','h','s','s','s','h','h','h','h','h', // 12
                          's','s','s','s','s','h','h','h','h','h', // 13
                          's','s','s','s','s','h','h','h','s','h', // 14
                          's','s','s','s','s','h','h','h','h','h', // 15
                          's','s','s','s','s','h','h','h','r','r', // 16
                          's','s','s','s','s','s','s','s','s','s', // 17
                          's','s','s','s','s','s','s','s','s','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
char twoCardSoftU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          '*','*','*','*','*','*','*','*','*','*', // 5
                          '*','*','*','*','*','*','*','*','*','*', // 6
                          '*','*','*','*','*','*','*','*','*','*', // 7
                          '*','*','*','*','*','*','*','*','*','*', // 8
                          '*','*','*','*','*','*','*','*','*','*', // 9
                          '*','*','*','*','*','*','*','*','*','*', // 10
                          '*','*','*','*','*','*','*','*','*','*', // 11
                          'p','p','p','p','p','p','p','p','p','p', // 12
                          'h','h','d','d','d','h','h','h','h','h', // 13
                          'h','h','d','d','d','h','h','h','h','h', // 14
                          'h','h','d','d','d','h','h','h','h','h', // 15
                          'h','h','d','d','d','h','h','h','h','h', // 16
                          'd','d','d','d','d','h','h','h','h','h', // 17
                          's','d','d','d','d','s','s','h','h','s', // 18
                          's','s','s','s','d','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21

     // Dealer card         2   3   4   5   6   7   8   9   T   A
 char twoCardPairU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           'p','p','p','p','p','p','h','h','h','h', // 2,2
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           'p','p','p','p','p','p','p','h','h','h', // 3,3
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           'h','h','p','p','p','h','h','h','h','h', // 4,4
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           'd','d','d','d','d','d','d','d','d','d', // 5,5
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'p','p','p','p','p','p','h','h','h','h', // 6,6
                           '*','*','*','*','*','*','*','*','*','*', // 13
                           'p','p','p','p','p','p','p','h','r','h', // 7,7
                           '*','*','*','*','*','*','*','*','*','*', // 15
                           'p','p','p','p','p','p','p','p','p','p', // 8,8
                           '*','*','*','*','*','*','*','*','*','*', // 17
                           'p','p','p','p','p','s','p','p','s','s', // 9,9
                           '*','*','*','*','*','*','*','*','*','*', // 19
                           's','s','s','p','p','s','s','s','s','s', // T,T
                           '*','*','*','*','*','*','*','*','*','*'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
       char hardU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          'h','h','h','h','h','h','h','h','h','h', // 4
                          'h','h','h','h','h','h','h','h','h','h', // 5
                          'h','h','h','h','h','h','h','h','h','h', // 6
                          'h','h','h','h','h','h','h','h','h','h', // 7
                          'h','h','h','h','h','h','h','h','h','h', // 8
                          'h','h','h','h','h','h','h','h','h','h', // 9
                          'h','h','h','h','h','h','h','h','h','h', // 10
                          'h','h','h','h','h','h','h','h','h','h', // 11
                          'h','h','s','s','s','h','h','h','h','h', // 12
                          's','s','s','s','s','h','h','h','h','h', // 13
                          's','s','s','s','s','h','h','h','h','h', // 14
                          's','s','s','s','s','h','h','h','s','h', // 15
                          's','s','s','s','s','h','h','h','h','h', // 16
                          's','s','s','s','s','s','s','s','s','s', // 17
                          's','s','s','s','s','s','s','s','s','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
        char softU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           '*','*','*','*','*','*','*','*','*','*', // 4
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           '*','*','*','*','*','*','*','*','*','*', // 6
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           '*','*','*','*','*','*','*','*','*','*', // 8
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           '*','*','*','*','*','*','*','*','*','*', // 10
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'h','h','h','h','h','h','h','h','h','h', // 12
                           'h','h','h','h','h','h','h','h','h','h', // 13
                           'h','h','h','h','h','h','h','h','h','h', // 14
                           'h','h','h','h','h','h','h','h','h','h', // 15
                           'h','h','h','h','h','h','h','h','h','h', // 16
                           'h','h','h','h','h','h','h','h','h','h', // 17
                           's','s','s','s','s','s','s','h','h','s', // 18
                           's','s','s','s','s','s','s','s','s','s', // 19
                           's','s','s','s','s','s','s','s','s','s', // 20
                           's','s','s','s','s','s','s','s','s','s'};// 21

        // Dealer card         2   3   4   5   6   7   8   9   T   A
        char PairU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           'h','h','h','h','h','h','h','h','h','h', // 2,2
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           'h','h','h','h','h','h','h','h','h','h', // 3,3
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           'h','h','h','h','h','h','h','h','h','h', // 4,4
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           'd','d','d','d','d','d','d','d','h','h', // 5,5
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'h','h','h','h','h','h','h','h','h','h', // 6,6
                           '*','*','*','*','*','*','*','*','*','*', // 13
                           's','s','s','s','s','s','h','h','r','h', // 7,7
                           '*','*','*','*','*','*','*','*','*','*', // 15
                           's','s','s','s','s','s','h','h','r','h', // 8,8
                           '*','*','*','*','*','*','*','*','*','*', // 17
                           's','s','s','s','s','s','s','s','s','s', // 9,9
                           '*','*','*','*','*','*','*','*','*','*', // 19
                           's','s','s','s','s','s','s','s','s','s', // T,T
                           '*','*','*','*','*','*','*','*','*','*'};// 21


    // Initiate
    int play, deal, type, firstTwo, upper;
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 0; // 0,1,2
        firstTwo = 1; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardHardU[i];
        strat.data[play][deal][type] = 0;
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 1; // 0,1,2
        firstTwo = 1; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardSoftU[i];
        strat.data[play][deal][type] = 0;
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 2; // 0,1,2
        firstTwo = 1; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardPairU[i];
        strat.data[play][deal][type] = 0;
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 0; // 0,1,2
        firstTwo = 0; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = hardU[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 1; // 0,1,2
        firstTwo = 0; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = softU[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 2; // 0,1,2
        firstTwo = 0; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = PairU[i];
    }
    strat.name = "Basic";
}


void data(Strategy &strat, int hands)
{
    int tops = 20;
    int topTen[tops];
    double value[tops];
    int type[tops];
    for (int i = 0; i < tops; i++)
    {
        value[i] = 0;
    }
    for (int index = 0; index < tops; index++)
    {
        for (int i = 0; i < 210; i++)
        {
            int r = i/10;
            int c = i%10;
            for (int t = 0; t < 3; t++)
            {
                if (index > 0)
                {
                    bool check = false;
                    if (value[index] < strat.data[r][c][t])
                    {
                        for (int s = 0; s <= index; s++)
                        {
                            if (value[s] == strat.data[r][c][t])
                            {
                                check = true;
                            }
                        }
                        if (check == false)
                        {
                            topTen[index] = i;
                            value[index] = strat.data[r][c][t];
                            type[index] = t;
                        }
                    }
                }
                else
                {
                    if (value[index] < strat.data[r][c][t])
                    {
                        topTen[index] = i;
                        value[index] = strat.data[r][c][t];
                        type[index] = t;
                    }
                }
            }
        }
    }
    //double mostPlayed = 0;
    for (int t = 0; t < 3; t++)
    {
        cout << "       2,    3,    4,    5,    6,    7,    8,    9,    T,    A\n";
        for (int p = 0; p < 21; p++)
        {
            cout << setw(2) << p + 1;
            for (int d = 0; d < 10; d++)
            {
                if (((strat.data[p][d][t]/hands)*100) > 1)
                {
                    cout << "[" << setw(4) << setprecision(2) << (strat.data[p][d][t]/hands)*100 << "]";
                }
                else
                    cout << setw(6) << setprecision(2) << (strat.data[p][d][t]/hands)*100;
            }
            cout << "  " << setw(2) << p + 1;
            cout << endl;
        }
    }

    cout << "Most Valuable indexes: \n";
    for (int i = 0; i < tops; i++)
    {
        int v = topTen[i];
        int r = v/10;
        int c = v%10;
        int t = type[i];
        string name;
        if (t == 0)
            name = "hard";
        else if (t == 1)
            name = "soft";
        else
            name = "pair";
        cout << i + 1 << ". " << name << " ";
        cout << r + 1 << " vs " << c + 2 << " ";
        cout << setprecision(6) << (value[i]/hands)*100 << endl;
    }
    cout << endl;
}

void EasyPeasy(Strategy &strat)
{
    // Dealer card         2   3   4   5   6   7   8   9   T   A
char twoCardHardU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          'h','h','h','h','h','h','h','h','h','h', // 5
                          'h','h','h','h','h','h','h','h','h','h', // 6
                          'h','h','h','h','h','h','h','h','h','h', // 7
                          'h','h','h','d','d','h','h','h','h','h', // 8
                          'h','d','d','d','d','h','h','h','h','h', // 9
                          'd','d','d','d','d','d','d','d','d','h', // 10
                          'd','d','d','d','d','d','d','d','d','d', // 11
                          's','s','s','s','s','h','h','h','h','h', // 12
                          's','s','s','s','s','h','h','h','h','h', // 13
                          's','s','s','s','s','h','h','h','s','h', // 14
                          's','s','s','s','s','s','s','s','s','h', // 15
                          's','s','s','s','s','s','s','s','s','s', // 16
                          's','s','s','s','s','s','s','s','s','s', // 17
                          's','s','s','s','s','s','s','s','s','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
char twoCardSoftU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          '*','*','*','*','*','*','*','*','*','*', // 5
                          '*','*','*','*','*','*','*','*','*','*', // 6
                          '*','*','*','*','*','*','*','*','*','*', // 7
                          '*','*','*','*','*','*','*','*','*','*', // 8
                          '*','*','*','*','*','*','*','*','*','*', // 9
                          '*','*','*','*','*','*','*','*','*','*', // 10
                          '*','*','*','*','*','*','*','*','*','*', // 11
                          'p','p','p','p','p','p','p','p','p','p', // 12
                          'h','h','d','d','d','h','h','h','h','h', // 13
                          'h','h','d','d','d','h','h','h','h','h', // 14
                          'h','h','d','d','d','h','h','h','h','h', // 15
                          'h','h','d','d','d','h','h','h','h','h', // 16
                          'd','d','d','d','d','h','h','h','h','h', // 17
                          's','d','d','d','d','s','s','h','h','s', // 18
                          's','s','s','s','d','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21

     // Dealer card         2   3   4   5   6   7   8   9   T   A
 char twoCardPairU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           'p','p','p','p','p','p','h','h','h','h', // 2,2
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           'p','p','p','p','p','p','p','h','h','h', // 3,3
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           'h','h','p','p','p','h','h','h','h','h', // 4,4
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           'd','d','d','d','d','d','d','d','d','d', // 5,5
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'p','p','p','p','p','p','h','h','h','h', // 6,6
                           '*','*','*','*','*','*','*','*','*','*', // 13
                           'p','p','p','p','p','p','p','h','r','h', // 7,7
                           '*','*','*','*','*','*','*','*','*','*', // 15
                           'p','p','p','p','p','p','p','p','p','p', // 8,8
                           '*','*','*','*','*','*','*','*','*','*', // 17
                           'p','p','p','p','p','s','p','p','s','s', // 9,9
                           '*','*','*','*','*','*','*','*','*','*', // 19
                           's','s','s','p','p','s','s','s','s','s', // T,T
                           '*','*','*','*','*','*','*','*','*','*'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
       char hardU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          'h','h','h','h','h','h','h','h','h','h', // 4
                          'h','h','h','h','h','h','h','h','h','h', // 5
                          'h','h','h','h','h','h','h','h','h','h', // 6
                          'h','h','h','h','h','h','h','h','h','h', // 7
                          'h','h','h','h','h','h','h','h','h','h', // 8
                          'h','h','h','h','h','h','h','h','h','h', // 9
                          'h','h','h','h','h','h','h','h','h','h', // 10
                          'h','h','h','h','h','h','h','h','h','h', // 11
                          's','s','s','s','s','h','h','h','h','h', // 12
                          's','s','s','s','s','h','h','h','h','h', // 13
                          's','s','s','s','s','h','h','h','s','h', // 14
                          's','s','s','s','s','s','s','s','s','h', // 15
                          's','s','s','s','s','s','s','s','s','s', // 16
                          's','s','s','s','s','s','s','s','s','s', // 17
                          's','s','s','s','s','s','s','s','s','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
        char softU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           '*','*','*','*','*','*','*','*','*','*', // 4
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           '*','*','*','*','*','*','*','*','*','*', // 6
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           '*','*','*','*','*','*','*','*','*','*', // 8
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           '*','*','*','*','*','*','*','*','*','*', // 10
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'h','h','h','h','h','h','h','h','h','h', // 12
                           'h','h','h','h','h','h','h','h','h','h', // 13
                           'h','h','h','h','h','h','h','h','h','h', // 14
                           'h','h','h','h','h','h','h','h','h','h', // 15
                           'h','h','h','h','h','h','h','h','h','h', // 16
                           'h','h','h','h','h','h','h','h','h','h', // 17
                           's','s','s','s','s','s','s','h','h','s', // 18
                           's','s','s','s','s','s','s','s','s','s', // 19
                           's','s','s','s','s','s','s','s','s','s', // 20
                           's','s','s','s','s','s','s','s','s','s'};// 21

        // Dealer card         2   3   4   5   6   7   8   9   T   A
        char PairU[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           'h','h','h','h','h','h','h','h','h','h', // 2,2
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           'h','h','h','h','h','h','h','h','h','h', // 3,3
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           'h','h','h','h','h','h','h','h','h','h', // 4,4
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           'd','d','d','d','d','d','d','d','h','h', // 5,5
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'h','h','h','h','h','h','h','h','h','h', // 6,6
                           '*','*','*','*','*','*','*','*','*','*', // 13
                           's','s','s','s','s','s','h','h','r','h', // 7,7
                           '*','*','*','*','*','*','*','*','*','*', // 15
                           's','s','s','s','s','s','h','h','r','h', // 8,8
                           '*','*','*','*','*','*','*','*','*','*', // 17
                           's','s','s','s','s','s','s','s','s','s', // 9,9
                           '*','*','*','*','*','*','*','*','*','*', // 19
                           's','s','s','s','s','s','s','s','s','s', // T,T
                           '*','*','*','*','*','*','*','*','*','*'};// 21

        // Dealer Card          2    3    4    5    6    7    8    9    T    A
       int hardIndex[210] = {-100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 1
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 2
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 3
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 4
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 5
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 6
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 7
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 8
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 9
                             -100,-100,-100,-100,-100,-100,-100,-100, 10 ,-100, // 10
                             -100,-100,-100,-100,-100,-100,-100,-100, -5 ,-100, // 11
                               5 ,  2 ,  0 , -2 , -2 ,-100,-100,-100,-100,-100, // 12
                              -1 , -3 ,-100,-100,-100,-100,-100,-100,-100,-100, // 13
                             -100,-100,-100,-100,-100,-100,-100,-100, 15 ,-100, // 14
                             -100,-100,-100,-100,-100,  20,  20,  15,  5 ,  20, // 15
                             -100,-100,-100,-100,-100,  15,  15,  5 ,  0 ,  15, // 16
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 17
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 18
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 19
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 20
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100};// 21

          // Dealer Card          2    3    4    5    6    7    8    9    T    A
       int softIndex[210] = {-100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 1
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 2
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 3
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 4
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 5
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 6
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 7
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 8
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 9
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 10
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 11
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 12
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 13
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 14
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 15
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 16
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 17
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 18
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 19
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 20
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100};// 21

    // Dealer Card             2    3    4    5    6    7    8    9    T    A
       int pairIndex[210] = {-100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 1
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 2
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 3
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 2,2
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 5
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 3,3
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 7
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 4,4
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 9
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 5,5
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 11
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 6,6
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 13
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 7,7
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 15
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 8,8
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 17
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 9,9
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100, // 19
                             -100,-100,-100,  5 ,  5 ,-100,-100,-100,-100,-100, // T,T
                             -100,-100,-100,-100,-100,-100,-100,-100,-100,-100};// A,A

    // Dealer card         2   3   4   5   6   7   8   9   T   A
char twoCardHardL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          'h','h','h','h','h','h','h','h','h','h', // 5
                          'h','h','h','h','h','h','h','h','h','h', // 6
                          'h','h','h','h','h','h','h','h','h','h', // 7
                          'h','h','h','d','d','h','h','h','h','h', // 8
                          'h','d','d','d','d','h','h','h','h','h', // 9
                          'd','d','d','d','d','d','d','d','h','h', // 10
                          'd','d','d','d','d','d','d','d','d','d', // 11
                          'h','h','h','h','h','h','h','h','h','h', // 12
                          'h','h','s','s','s','h','h','h','h','h', // 13
                          's','s','s','s','s','h','h','h','h','h', // 14
                          's','s','s','s','s','h','h','h','h','h', // 15
                          's','s','s','s','s','h','h','h','h','h', // 16
                          's','s','s','s','s','s','s','s','s','s', // 17
                          's','s','s','s','s','s','s','s','s','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
char twoCardSoftL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          '*','*','*','*','*','*','*','*','*','*', // 4
                          '*','*','*','*','*','*','*','*','*','*', // 5
                          '*','*','*','*','*','*','*','*','*','*', // 6
                          '*','*','*','*','*','*','*','*','*','*', // 7
                          '*','*','*','*','*','*','*','*','*','*', // 8
                          '*','*','*','*','*','*','*','*','*','*', // 9
                          '*','*','*','*','*','*','*','*','*','*', // 10
                          '*','*','*','*','*','*','*','*','*','*', // 11
                          'p','p','p','p','p','p','p','p','p','p', // 12
                          'h','h','d','d','d','h','h','h','h','h', // 13
                          'h','h','d','d','d','h','h','h','h','h', // 14
                          'h','h','d','d','d','h','h','h','h','h', // 15
                          'h','h','d','d','d','h','h','h','h','h', // 16
                          'd','d','d','d','d','h','h','h','h','h', // 17
                          's','d','d','d','d','s','s','h','h','s', // 18
                          's','s','s','s','d','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21

     // Dealer card         2   3   4   5   6   7   8   9   T   A
 char twoCardPairL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           'p','p','p','p','p','p','h','h','h','h', // 2,2
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           'p','p','p','p','p','p','p','h','h','h', // 3,3
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           'h','h','p','p','p','h','h','h','h','h', // 4,4
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           'd','d','d','d','d','d','d','d','d','d', // 5,5
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'p','p','p','p','p','p','h','h','h','h', // 6,6
                           '*','*','*','*','*','*','*','*','*','*', // 13
                           'p','p','p','p','p','p','p','h','r','h', // 7,7
                           '*','*','*','*','*','*','*','*','*','*', // 15
                           'p','p','p','p','p','p','p','p','p','p', // 8,8
                           '*','*','*','*','*','*','*','*','*','*', // 17
                           'p','p','p','p','p','s','p','p','s','s', // 9,9
                           '*','*','*','*','*','*','*','*','*','*', // 19
                           's','s','s','s','s','s','s','s','s','s', // T,T
                           '*','*','*','*','*','*','*','*','*','*'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
       char hardL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                          '*','*','*','*','*','*','*','*','*','*', // 2
                          '*','*','*','*','*','*','*','*','*','*', // 3
                          'h','h','h','h','h','h','h','h','h','h', // 4
                          'h','h','h','h','h','h','h','h','h','h', // 5
                          'h','h','h','h','h','h','h','h','h','h', // 6
                          'h','h','h','h','h','h','h','h','h','h', // 7
                          'h','h','h','h','h','h','h','h','h','h', // 8
                          'h','h','h','h','h','h','h','h','h','h', // 9
                          'h','h','h','h','h','h','h','h','h','h', // 10
                          'h','h','h','h','h','h','h','h','h','h', // 11
                          'h','h','h','h','h','h','h','h','h','h', // 12
                          'h','h','s','s','s','h','h','h','h','h', // 13
                          's','s','s','s','s','h','h','h','h','h', // 14
                          's','s','s','s','s','h','h','h','h','h', // 15
                          's','s','s','s','s','h','h','h','h','h', // 16
                          's','s','s','s','s','s','s','s','s','s', // 17
                          's','s','s','s','s','s','s','s','s','s', // 18
                          's','s','s','s','s','s','s','s','s','s', // 19
                          's','s','s','s','s','s','s','s','s','s', // 20
                          's','s','s','s','s','s','s','s','s','s'};// 21

    // Dealer card         2   3   4   5   6   7   8   9   T   A
        char softL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           '*','*','*','*','*','*','*','*','*','*', // 4
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           '*','*','*','*','*','*','*','*','*','*', // 6
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           '*','*','*','*','*','*','*','*','*','*', // 8
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           '*','*','*','*','*','*','*','*','*','*', // 10
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'h','h','h','h','h','h','h','h','h','h', // 12
                           'h','h','h','h','h','h','h','h','h','h', // 13
                           'h','h','h','h','h','h','h','h','h','h', // 14
                           'h','h','h','h','h','h','h','h','h','h', // 15
                           'h','h','h','h','h','h','h','h','h','h', // 16
                           'h','h','h','h','h','h','h','h','h','h', // 17
                           's','s','s','s','s','s','s','h','h','s', // 18
                           's','s','s','s','s','s','s','s','s','s', // 19
                           's','s','s','s','s','s','s','s','s','s', // 20
                           's','s','s','s','s','s','s','s','s','s'};// 21

        // Dealer card         2   3   4   5   6   7   8   9   T   A
        char PairL[210] = {'*','*','*','*','*','*','*','*','*','*', // 1
                           '*','*','*','*','*','*','*','*','*','*', // 2
                           '*','*','*','*','*','*','*','*','*','*', // 3
                           'h','h','h','h','h','h','h','h','h','h', // 2,2
                           '*','*','*','*','*','*','*','*','*','*', // 5
                           'h','h','h','h','h','h','h','h','h','h', // 3,3
                           '*','*','*','*','*','*','*','*','*','*', // 7
                           'h','h','h','h','h','h','h','h','h','h', // 4,4
                           '*','*','*','*','*','*','*','*','*','*', // 9
                           'd','d','d','d','d','d','d','d','h','h', // 5,5
                           '*','*','*','*','*','*','*','*','*','*', // 11
                           'h','h','h','h','h','h','h','h','h','h', // 6,6
                           '*','*','*','*','*','*','*','*','*','*', // 13
                           's','s','s','s','s','s','h','h','r','h', // 7,7
                           '*','*','*','*','*','*','*','*','*','*', // 15
                           's','s','s','s','s','s','h','h','r','h', // 8,8
                           '*','*','*','*','*','*','*','*','*','*', // 17
                           's','s','s','s','s','s','s','s','s','s', // 9,9
                           '*','*','*','*','*','*','*','*','*','*', // 19
                           's','s','s','s','s','s','s','s','s','s', // T,T
                           '*','*','*','*','*','*','*','*','*','*'};// 21
    // Initiate
    int play, deal, type, firstTwo, upper;
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 0; // 0,1,2
        firstTwo = 1; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardHardU[i];
        strat.index[play][deal][type] = hardIndex[i];
        strat.data[play][deal][type] = 0;
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 1; // 0,1,2
        firstTwo = 1; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardSoftU[i];
        strat.index[play][deal][type] = softIndex[i];
        strat.data[play][deal][type] = 0;
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 2; // 0,1,2
        firstTwo = 1; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardPairU[i];
        strat.index[play][deal][type] = pairIndex[i];
        strat.data[play][deal][type] = 0;
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 0; // 0,1,2
        firstTwo = 0; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = hardU[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 1; // 0,1,2
        firstTwo = 0; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = softU[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 2; // 0,1,2
        firstTwo = 0; // true or false
        upper = 1; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = PairU[i];
    }

    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 0; // 0,1,2
        firstTwo = 1; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardHardL[i];
        strat.data[play][deal][type] = 0;
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 1; // 0,1,2
        firstTwo = 1; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardSoftL[i];
        strat.data[play][deal][type] = 0;
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 2; // 0,1,2
        firstTwo = 1; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = twoCardPairL[i];
        strat.data[play][deal][type] = 0;
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 0; // 0,1,2
        firstTwo = 0; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = hardL[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 1; // 0,1,2
        firstTwo = 0; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = softL[i];
    }
    for (int i = 0; i < 210; i++)
    {
        play = i / 10;
        deal = i % 10;
        type = 2; // 0,1,2
        firstTwo = 0; // true or false
        upper = 0; // true of false
        strat.moves[play][deal][type][firstTwo][upper] = PairL[i];
    }
    strat.name = "Omega Lite";
}

