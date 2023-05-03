//
//  File: main.c
//  ECE Final Project: A Game of Dos
//  Created by Carson Brofft
//  May 3 2023
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>

#define MAX 108
#define HANDSIZE 7
#define MAX_NUM_PLAYERS 6

//given struct for card variables
typedef struct card_s {
    char color[10];
    int value;
    char action[15];
    struct card_s *pt;
} card;

//created struct for player hands
typedef struct hand_s {
    int numCards;
    int playerNum;
    int points;
    card *playerHand;
} hand;

//EDIT LINKED LIST FUNCTIONS
//adds card to the front of the linked list, takes linked list and a card as input, no output necessary
void addNewNodeToBeginning(card ** head, card temp) {
    card * new_node;   //temp card
    new_node = (card *)malloc(sizeof(card));   //allocates memory for new card node
    new_node->value = temp.value;
    strcpy(new_node->action, temp.action);   //copy data
    strcpy(new_node->color, temp.color);
    new_node->pt = *head;
    *head = new_node;
}

//adds card to the end of the linked list; takes linked list and a card as input, no output necessary
void addNewNodeToEnd(card * head, card temp) {
    card * current = head;   //temp card
    while (current->pt != NULL)
        current = current->pt;
    current->pt = (card *) malloc(sizeof(card));   //allocates memory for new card node
    current->pt->value = temp.value;
    strcpy(current->pt->action, temp.action);   //copy data
    strcpy(current->pt->color, temp.color);
    current->pt->pt = NULL;
}

//deletes first card from linked list; receives linked list as input; returns head pointer to that list without the first node
card deleteFirstNode(card ** head) {
    card ret;
    card * next_node = NULL;
    next_node = (*head)->pt;
    ret.value = (*head)->value;
    strcpy(ret.action, (*head)->action);
    strcpy(ret.color, (*head)->color);
    ret.pt = NULL;
    *head = next_node;
    return ret;
}

//deletes card from any location in linked list; receives linked list and the specific position of a card to be deleted; no output necessary
void deleteNode(card **head, int pos) {
    card* temp = *head;   //temp pointer to head node
    card* prev = NULL;
    if(pos == 0) {
        *head = temp->pt;
        temp->pt = NULL;
        free(temp); //frees memory from deck
        return;
    }
    for(int i = 0 ; i < pos ; i++) {
        prev = temp;
        temp = temp->pt;
    }
    prev->pt = temp->pt;
    free(temp);  //frees memory
}

//DECK FUNCTIONS
//function initializes the 108-card deck from scratch; no input; returns head pointer to the linked list of the deck
card* initializeDeck(void) {
    card *temp = (card*)malloc(sizeof(card)); //amount of memory necessary for one card
    temp->pt = NULL;
    card *head = NULL;
    
    //adds card information to each card
    for(int i = 0 ; i < 4 ; i++) {
        for(int j = 0 ; j < 24 ; j++) {
            switch(i) {
                case 0:
                    strcpy(temp->color, "Red");
                    break;
                case 1:
                    strcpy(temp->color, "Yellow");
                    break;
                case 2:
                    strcpy(temp->color, "Green");
                    break;
                case 3:
                    strcpy(temp->color, "Blue");
                    break;
            }
            
            if (j < 3)
                temp->value = 1;
            else if (j < 6)
                temp->value = 3;
            else if (j < 9)
                temp->value = 4;
            else if (j < 12)
                temp->value = 5;
            else if (j < 14)
                temp->value = 6;
            else if (j < 16)
                temp->value = 7;
            else if (j < 18)
                temp->value = 8;
            else if (j < 20)
                temp->value = 9;
            else if (j < 22)
                temp->value = 10;
            else
                temp->value = 11;
            
            if (temp->value == 11)
                strcpy(temp->action, "AnyNumber");
            else
                strcpy(temp->action, "none");

            if(head == NULL)
                head = temp;
            else
                addNewNodeToBeginning(&head, *temp);
            temp = (card*)malloc(sizeof(card));   //allocates memory for next card in deck
        }
    }
    
    //dos
    for(int i = 0 ; i < 12 ; i++) {
        strcpy(temp->color, "Black");
        temp->value = 2;
        strcpy(temp->action, "AnyColor");
        addNewNodeToBeginning(&head, *temp);
        temp = (card*)malloc(sizeof(card));   //allocates memory for next card in deck
    }
    return head;   //pointer to first card in the deck
}

//function reads the 108-card deck from a file given an input of string (UnoDeck.txt); returns head pointer to the linked list for the deck
card* readDeck(char str[]) {
    card *head = NULL, *tail = NULL, *temp = (card*)malloc(sizeof(card));
    FILE *inp = fopen(str, "r");
    if(inp == NULL)
        printf("Error finding file\n");
    while(fscanf(inp, " %s %d %s", temp->color, &temp->value, temp->action) != EOF) {   //scans data from file named by str[] ("UnoDeck.txt" usually)
        if(head == NULL)
            head = temp;
        else
            tail->pt = temp;
        tail = temp;
        tail->pt = NULL;
        temp = (card*)malloc(sizeof(card));   //allocates memory for next card in deck
    }
    fclose(inp);
    return head;
}

//shuffles the deck of cards to a random order, receives head pointer and number of cards to be shuffled as input, no output necessary
void shuffleDeck(card *head, int remaining) {
    card *memory[remaining];   //creates temp array to help copy data in deck
    int i = 0;
    while(head != NULL) {
        memory[i] = head;
        i++;
        head = head->pt;
    }
    card t;
    srand((int)time(0));
    for(int i = 0 ; i < remaining ; i++) {
        int j = rand() % remaining;   //picks random card position in deck
        strcpy(t.color, memory[i]->color);
        t.value = memory[i]->value;
        strcpy(t.action, memory[i]->action);
        if(i != j) {
            strcpy(memory[i]->color, memory[j]->color);
            strcpy(memory[i]->action, memory[j]->action);
        }
        memory[i]->value = memory[j]->value;
        strcpy(memory[j]->color, t.color);
        memory[j]->value = t.value;
        strcpy(memory[j]->action, t.action);
    }
}

//prints the cards with graphics; receives pointer to a card and continues to print the cards after the selected card until the pointer is NULL
void print(card *head){
    while(head != NULL) {
        printf("\n┌─────────┐\n");
        if(head->value > 12)
            printf("│%9s│\n│         │\n", head->action);
        else if(head->value > 9)
            printf("│%9s│\n│%9s│\n", head->color, head->action);
        else
            printf("│%9s│\n│ %d       │\n", head->color, head->value);
        head = head->pt;
        printf("└─────────┘");
    }
    printf("\n");
}

//draws a card from the deck, receives deck, player's hand, number of cards in draw pile, and number of cards in discard pile as input. No output necessary
void drawCard(hand* array, card **deck, int *numCardsRemaining, int *numCardsDiscarded) {
    array->numCards++;   //number of cards in player's hand increases by 1
    addNewNodeToBeginning(&array->playerHand, **deck);   //copy the top card in the deck to player's hand
    deleteFirstNode(deck);   //removes top card from the deck
    *numCardsRemaining = *numCardsRemaining - 1;
    //reshuffles the deck once all cards in draw pile are used
    if(*numCardsRemaining == 0) {
        printf("\nNo more cards remaining in the draw pile. Shuffling the discarded cards and using that for the new draw pile...\n");
        shuffleDeck(*deck, *numCardsRemaining + *numCardsDiscarded);
        *numCardsRemaining = *numCardsDiscarded;
        *numCardsDiscarded = 0;
    }
}

//PLAY FUNCTIONS
//For Draw-Four challenge: function checks if the player had a playable card other than a Draw-Four. returns true if card can be played, false otherwise
bool checkPlayability(card *head, card temp) {
    bool flag = false;
    while(head != NULL) {
        if(head->value == temp.value || (strcmp(head->color, temp.color) == 0)) {
            flag = true;
            break;
        }
        head = head->pt;
    }
    return flag;
}

//For Progressive UNO: function checks if the player has a playable +2 or +4 card, returns true if card can be played, false otherwise
bool checkPlayabilityOnPenalty(card *head, card temp) {
    bool flag = false;
    while(head != NULL) {
        if(strcmp(head->action, temp.action) == 0) {
            flag = true;
            break;
        }
        head = head->pt;
    }
    return flag;
}

//For automated player, Bob the Bot, to Play: functions finds a playable card for Bob, otherwise returns 0; receives computer's hand and discarded card as input, returns position in hand that is possible to be played
int compPlay(card *head, card temp, char nextColor[]) {
    int count = 1;
    int flag = 0;
    while(head != NULL) {
        //If card can be legally played
        if(head->value == temp.value || (strcmp(head->color, temp.color) == 0) || strcmp(head->action, "Wild") == 0 || strcmp(head->action, "Draw-Four") == 0 || (((strcmp(temp.action, "Wild") == 0) || (strcmp(temp.action, "Draw-Four") == 0)) && (strcmp(head->color, nextColor) == 0))) {
            flag = 1;
            break;
        }
        count++;
        head = head->pt;
    }
    if(flag)
        return count;
    else
        return 0;
}

//THE MAIN FUNCTION
int main(void) {
    int numCardsRemaining = MAX;
    int numCardsDiscarded = 0;
    int numPlayers = 0;
    int shuffle = 0;
    char bot = 'n';
    card topDiscard, selectedCardByUser; //temporary variables to store one card at a time
    card *deck = (card*)malloc(numCardsRemaining * sizeof(card)); // allocates memory for the entire deck of 108 cards
    
    //Game Play Set Up
    while(numPlayers < 1 || numPlayers > MAX_NUM_PLAYERS) {
        printf("Let's play a game of UNO\nEnter number of human players: ");
        scanf("%d", &numPlayers);
    }
    printf("Would you like to play against a computer bot as well (y/n)? ");
    scanf(" %c", &bot);
    //initializes deck of cards
    while((shuffle != 1) && (shuffle != 2)) {
        printf("Press 1 to shuffle the UNO deck or 2 to load a deck from a file: ");
        scanf(" %d", &shuffle);
        if(shuffle == 1) {
            deck = initializeDeck(); //Creates deck from scratch
            shuffleDeck(deck, numCardsRemaining); //Shuffles deck
            printf("The deck has been shuffled and dealt\n");
        }
        if(shuffle == 2) {
            char filename[100];
            printf("Enter file name (UnoDeck.txt): ");
            scanf(" %[^\n]s", filename);
            deck = readDeck(filename); //Reads from file
            printf("The deck has been loaded and dealt\n");
        }
    }
    //adds a computer to the game if the player wanted
    if(bot == 'y')
        numPlayers++;
    
    //initializes points and player numbers
    hand PlayersHands[numPlayers];
    for(int i = 0 ; i < numPlayers ; i++) {
        PlayersHands[i].points = 0;
        PlayersHands[i].playerNum = i + 1;
    }

    bool winner = false;
    while(!winner) {
        //Deals the cards to the players
        for(int i = 0 ; i < numPlayers ; i++) {
            PlayersHands[i].playerHand = NULL;
            PlayersHands[i].numCards = 0;
        }
        for(int i = 0 ; i < HANDSIZE; i++)
            for(int j = 0 ; j < numPlayers ; j++)
                drawCard(&PlayersHands[j], &deck, &numCardsRemaining, &numCardsDiscarded); //function "deals" cards to each player in alternating order by taking top card off the deck and adding it to their hand
        
        //Turns over first card for discard pile
        bool contToShuffle = true;
        card discard2;
        int playerToAct = 0;
        bool reverse = false;
        while(contToShuffle) {
            addNewNodeToEnd(deck, deck[0]);   //puts the first card in the deck also at the end of the deck
            deleteFirstNode(&deck);   //removes the first card in the deck and also gives the newest card value
            numCardsRemaining--;   //removes one from number of cards remaining in draw pile
            discard2 = *deck;   //temporary variable to get top discard card
            for(int i = 0 ; i < numCardsRemaining + numCardsDiscarded ; i++)
                discard2 = *discard2.pt;   //advances temporary variable until discard is found, at the end of the deck
            playerToAct = 0;   //actually means player 1, just cast as a 0 in code
            reverse = false;
            
            //if first card is Draw-Four, reshuffles the deck
            if(strcmp(discard2.action, "AnyColor") == 0) {
                numCardsRemaining++;
                shuffleDeck(deck, numCardsRemaining);
            }
            //if card is normal, allows play to begin, breaking while loop
            else
                contToShuffle = false;
        }
        
        //Asks user what color they want first if the first card turned over is a wild
        char nextColor[7];
        int nextColorNum;
        if(strcmp(discard2.action, "Wild") == 0) {
            printf("Player 1's hand:");
            print(PlayersHands[0].playerHand);
            printf("A Wild Card was turned up at the beginning of play! What should the next color be? Type 1 for Red, 2 for Yellow, 3 for Green, or 4 for Blue: ");
            
            //if computer is playing to act right now
            if(playerToAct == numPlayers - 1 && bot == 'y') {
                srand((int)time(0));
                nextColorNum = (rand() % 4) + 1;
                printf("Bob the Bot chose next color: ");
            }
            
            //if human player is making decision
            else
                scanf(" %d", &nextColorNum);
            
            //uses input from player to determine next color
            switch(nextColorNum) {
                case 1:
                    strcpy(nextColor, "Red");
                    break;
                case 2:
                    strcpy(nextColor, "Yellow");
                    break;
                case 3:
                    strcpy(nextColor, "Green");
                    break;
                case 4:
                    strcpy(nextColor, "Blue");
                    break;
                default:
                    printf("Error reading correct card\n");
                    return -1;
            }
            
            //Says next color if Bob the Bot chose it
            if(playerToAct == numPlayers - 1 && bot == 'y')
                printf("%s\n", nextColor);
        }
        
        //Start of Gameplay
        bool roundOver = false;
        bool cardWasPlayed = true;
        int cardNumToPlay;
        int penalty = 0;
        while(!roundOver) {
            bool turnActive = true;
            
            //Get data for discard pile
            topDiscard = *deck;
            for(int j = 0 ; j < numCardsRemaining + numCardsDiscarded ; j++)
                topDiscard = *topDiscard.pt;
            
            //Skips player if skip was played
            if(((strcmp(topDiscard.action, "Skip") == 0) && cardWasPlayed) || ((numPlayers == 2 && (strcmp(topDiscard.action, "Reverse") == 0)) && cardWasPlayed)) {
                printf("\nPlayer %d was skipped\n", playerToAct + 1);
                turnActive = false;
                cardWasPlayed = false;
            }
            
            //Makes player draw two if draw two was played, unless they also have draw 2 in their hand
            if((strcmp(topDiscard.action, "Draw-Two") == 0) && cardWasPlayed) {
                //checks for progressive UNO
                if((checkPlayabilityOnPenalty(PlayersHands[playerToAct].playerHand, topDiscard)) && ((playerToAct < numPlayers - 1) || bot != 'y')) {
                    penalty++;
                    printf("\nProgressive UNO rules: Player %d, you must play your Draw-Two card, or type 0 to accept the current penalty and draw. Please do not select any other card.\n", playerToAct + 1);
                }
                else {
                    printf("\nPlayer %d must draw %d cards and forfeit their turn\n", playerToAct + 1, 2 + 2 * penalty);
                    for(int i = 0 ; i < 2 + 2 * penalty; i++)
                        drawCard(&PlayersHands[playerToAct], &deck, &numCardsRemaining, &numCardsDiscarded);
                    penalty = 0;
                    cardWasPlayed = false;
                    turnActive = false;
                }
            }
            
            //Makes player draw four, unless they also have draw 4 in their hand
            if((strcmp(topDiscard.action, "Draw-Four") == 0) && cardWasPlayed) {
               //Challenge Draw-Four rules
                printf("\nPlayer %d: Would you like to challenge the Draw-Four played by the previous player (y/n)? ", playerToAct + 1);
                char challenge;
                
                //if Bob the Bot is player
                if(playerToAct == numPlayers - 1 && bot == 'y') {
                    printf("Bob the Bot is challenging your play");
                    challenge = 'y';
                }
                else
                    scanf(" %c", &challenge);
                if(challenge == 'y') {   //Player chose to challenge
                    //determines last player to act
                    int lastPlayer;
                    if(reverse)
                        lastPlayer = playerToAct + 1;
                    else
                        lastPlayer = playerToAct + (numPlayers - 1);
                    lastPlayer = lastPlayer % numPlayers;
                    
                    //Gets temp card for top discarded card
                    card discard3 = *deck;
                    for(int i = 0 ; i < numCardsRemaining + numCardsDiscarded - 1 ; i++)
                        discard3 = *discard3.pt;
                    
                    //Challenge was successful
                    if(checkPlayability(PlayersHands[lastPlayer].playerHand, discard3)) {
                        printf("\nPlayer %d won the challenge. Player %d must now draw four cards\n", playerToAct + 1, lastPlayer + 1);
                        for(int i = 0 ; i < 4 ; i++)
                            drawCard(&PlayersHands[lastPlayer], &deck, &numCardsRemaining, &numCardsDiscarded);
                    }
                    
                    //challenge was not successful
                    else {
                        printf("\nPlayer %d lost the challenge, must draw six cards, and forfeit their turn\n", playerToAct + 1);
                        for(int i = 0 ; i < 6 ; i++)
                            drawCard(&PlayersHands[playerToAct], &deck, &numCardsRemaining, &numCardsDiscarded);
                        cardWasPlayed = false;
                        turnActive = false;
                    }
                }
                else {   //if player does not challenge, Progressive UNO executes if applicable
                    if(checkPlayabilityOnPenalty(PlayersHands[playerToAct].playerHand, topDiscard)) {
                        printf("\nProgressive UNO rules: Player %d, you must play your Draw-Four card, or type 0 to accept the current penalty and draw. Please do not select any other card.\n", playerToAct + 1);
                        penalty++;
                    }
                    else {   //if player does not have a Draw-Two, forces them to draw two cards
                        printf("\nPlayer %d must draw %d cards and forfeit their turn\n", playerToAct + 1, 4 + 4 * penalty);
                        for(int i = 0 ; i < 4 + 4 * penalty; i++)
                            drawCard(&PlayersHands[playerToAct], &deck, &numCardsRemaining, &numCardsDiscarded);
                        penalty = 0;
                        cardWasPlayed = false;
                        turnActive = false;
                    }
                }
            }
            
            while(turnActive) {
                //Prints discard card and player's hand
                printf("\nDiscard pile: ");
                print(&topDiscard);
                if((strcmp(topDiscard.action, "Wild") == 0) || (strcmp(topDiscard.action, "Draw-Four") == 0))
                    if(!penalty)
                        printf("(But you must play a %s card)\n", nextColor);
                if(playerToAct == numPlayers - 1 && bot == 'y')
                    printf("Bob the Bot's hand: ");
                else
                    printf("Player %d's hand: ", PlayersHands[playerToAct].playerNum);
                print(PlayersHands[playerToAct].playerHand);
                
                //Player decides which card to play
                if(playerToAct == numPlayers - 1 && bot == 'y') {
                    cardNumToPlay = compPlay(PlayersHands[playerToAct].playerHand, topDiscard, nextColor);
                    if(cardNumToPlay)
                        printf("\nBob the Bot played a card\n");
                    else
                        printf("\nBob the Bot drew a card\n");
                }
                else {
                    if(PlayersHands[playerToAct].numCards == 1)
                        printf("Press 1 to play the card from your hand, or press 0 to draw a card from the deck: ");
                    else
                        printf("Type 1-%d to play any card from your hand, or type 0 to draw from the deck: ", PlayersHands[playerToAct].numCards);
                    scanf(" %d", &cardNumToPlay);
                }
                
                //Player choses to draw a card
                if(cardNumToPlay == 0) {
                    //If progressive UNO is in play
                    if(penalty != 0) {
                        if(strcmp(topDiscard.action, "Draw-Four") == 0)
                            for(int i = 0 ; i < 4 * penalty ; i++)
                                drawCard(&PlayersHands[playerToAct], &deck, &numCardsRemaining, &numCardsDiscarded);
                        else
                            for(int i = 0 ; i < 2 * penalty ; i++)
                                drawCard(&PlayersHands[playerToAct], &deck, &numCardsRemaining, &numCardsDiscarded);
                        penalty = 0;
                    }
                    else //just draw one card otherwise
                        drawCard(&PlayersHands[playerToAct], &deck, &numCardsRemaining, &numCardsDiscarded);
                    turnActive = false;
                    cardWasPlayed = false;
                    break;
                }
                
                //Player choses to select a card
                if(cardNumToPlay >= 1 && cardNumToPlay <= PlayersHands[playerToAct].numCards) {
                    //Temp variable to hold selected card
                    selectedCardByUser = *PlayersHands[playerToAct].playerHand;
                    for(int j = 0 ; j < cardNumToPlay - 1 ; j++)
                        selectedCardByUser = *selectedCardByUser.pt;
                    
                    //if Player chose wild
                    if((strcmp(selectedCardByUser.action, "Wild") == 0) || (strcmp(selectedCardByUser.action, "Draw-Four") == 0)) {
                        //Plays card
                        PlayersHands[playerToAct].numCards--;   //number of cards in player's hand decreases by 1
                        numCardsDiscarded++;
                        addNewNodeToEnd(deck, selectedCardByUser);   //card user selected is now the most recently discarded card
                        deleteNode(&PlayersHands[playerToAct].playerHand, cardNumToPlay - 1);   //deletes card player selected from their hand
                        if(PlayersHands[playerToAct].numCards == 1)
                            printf("\nPlayer %d has UNO\n", PlayersHands[playerToAct].playerNum);
                        
                        //Let's player pick next color
                        printf("What should the next color be? Type 1 for Red, 2 for Yellow, 3 for Green, or 4 for Blue: ");
                        //if player is Bob the Bot
                        if(playerToAct == numPlayers - 1 && bot == 'y') {
                            srand((int)time(0));
                            nextColorNum = (rand() % 4) + 1;
                            printf("Bob the Bot chose next color: ");
                        }
                        
                        //if player to act is a human player
                        else
                            scanf(" %d", &nextColorNum);
                        
                        //determines next color based on input from player
                        switch(nextColorNum) {
                            case 1:
                                strcpy(nextColor, "Red");
                                break;
                            case 2:
                                strcpy(nextColor, "Yellow");
                                break;
                            case 3:
                                strcpy(nextColor, "Green");
                                break;
                            case 4:
                                strcpy(nextColor, "Blue");
                                break;
                            default:
                                printf("Error reading correct card\n");
                                return -1;
                        }
                        
                        if(playerToAct == numPlayers - 1 && bot == 'y')
                            printf("%s\n", nextColor);
                        
                        cardWasPlayed = true;
                        turnActive = false;
                        break;
                    }
                    
                    //Plays card if possible
                    if((((strcmp(topDiscard.action, "Wild") == 0) || (strcmp(topDiscard.action, "Draw-Four") == 0)) && (strcmp(selectedCardByUser.color, nextColor) == 0)) || ((selectedCardByUser.value == topDiscard.value) || (strcmp(selectedCardByUser.color, topDiscard.color) == 0))){
                        //if reverse, reverses direction of play
                        if(strcmp(selectedCardByUser.action, "Reverse") == 0)
                            reverse = !reverse;
                        //Plays card
                        PlayersHands[playerToAct].numCards--;   //number of cards in player's hand decreases by 1
                        numCardsDiscarded++;
                        addNewNodeToEnd(deck, selectedCardByUser);   //card user selected is now the most recently discarded card
                        deleteNode(&PlayersHands[playerToAct].playerHand, cardNumToPlay - 1);   //deletes card player selected from their hand
                        
                        //7-0 Game Play rules
                        if(selectedCardByUser.value == 7) {
                            printf("Which player number would you like to swap hands with (1-%d)? You may not select yourself ", numPlayers);
                            int switchHand;
                            
                            //Computer's Turn to Act
                            if(playerToAct == numPlayers - 1 && bot == 'y') {
                                printf("Bob the Bot chose to swap hands with Player 1\n");
                                switchHand = 1;
                            }
                            
                            //Human chooses which other player to swap hands with
                            else
                                scanf("%d", &switchHand);
                            switchHand--;
                            
                            //The next 6 lines all swap hands with another player using two temporary variables, switchNumCards and switchPlayerHands
                            int switchNumCards = PlayersHands[playerToAct].numCards;
                            PlayersHands[playerToAct].numCards = PlayersHands[switchHand].numCards;
                            PlayersHands[switchHand].numCards = switchNumCards;
                            card *switchPlayerHands = PlayersHands[playerToAct].playerHand;
                            PlayersHands[playerToAct].playerHand = PlayersHands[switchHand].playerHand;
                            PlayersHands[switchHand].playerHand = switchPlayerHands;
                        }
                        if(selectedCardByUser.value == 0) {
                            printf("\nAll players rotate hands in the direction of play\n");
                            if(reverse) {
                                //Every player passes their hand in the direction of play, using two temporary variables
                                int switchNumCards = PlayersHands[0].numCards;
                                card *switchHands = PlayersHands[0].playerHand;
                                for(int i = 0 ; i < numPlayers - 1 ; i++) {
                                    PlayersHands[i].numCards = PlayersHands[i + 1].numCards;
                                    PlayersHands[i].playerHand = PlayersHands[i + 1].playerHand;
                                }
                                PlayersHands[numPlayers - 1].numCards = switchNumCards;
                                PlayersHands[numPlayers - 1].playerHand = switchHands;
                            }
                            else {
                                //Every player passes their hand in the direction of play, using two temporary variables
                                int switchNumCards = PlayersHands[numPlayers - 1].numCards;
                                card *switchHands = PlayersHands[numPlayers - 1].playerHand;
                                for(int i = numPlayers - 1 ; i > 0 ; i--) {
                                    PlayersHands[i].numCards = PlayersHands[i - 1].numCards;
                                    PlayersHands[i].playerHand = PlayersHands[i - 1].playerHand;
                                }
                                PlayersHands[0].numCards = switchNumCards;
                                PlayersHands[0].playerHand = switchHands;
                            }
                        }
                        
                        //Detects if at the end of their turn, a player has UNO or not
                        if(PlayersHands[playerToAct].numCards == 1)
                            printf("Player %d has UNO\n\n", PlayersHands[playerToAct].playerNum);
                        cardWasPlayed = true;
                        turnActive = false;
                        break;
                    }
                    
                    //if card is illegal and cannot be played, sends a message to the player
                    if(selectedCardByUser.value < 10)
                        printf("\nThe %s %d could not be played on top of the ", selectedCardByUser.color, selectedCardByUser.value);
                    else
                        printf("\nThe %s %s could not be played on top of the ", selectedCardByUser.color, selectedCardByUser.action);
                    if(topDiscard.value < 10)
                        printf("%s %d\n", topDiscard.color, topDiscard.value);
                    else if(topDiscard.value < 13)
                        printf("%s %s\n", topDiscard.color, topDiscard.action);
                    else
                        printf("%s (Remember you must play a %s card)\n", topDiscard.action, nextColor);
                }
            }
            
            //Points: win detection for round
            if(PlayersHands[playerToAct].numCards == 0) {
                printf("Player %d wins this round! The point totals are now as follows:\n", PlayersHands[playerToAct].playerNum);
                for(int i = 0 ; i < numPlayers ; i++) {
                    //Counts the values of each card in every player's hand and adds that value to the winning player's point total
                    for(int j = 0 ; j < PlayersHands[i].numCards ; j++) {
                        if(PlayersHands[i].playerHand->value < 10)
                            PlayersHands[playerToAct].points += PlayersHands[i].playerHand->value;
                        else if(PlayersHands[i].playerHand->value < 13)
                            PlayersHands[playerToAct].points += 20;
                        else
                            PlayersHands[playerToAct].points += 50;
                        PlayersHands[i].playerHand = PlayersHands[i].playerHand->pt;
                    }
                }
                for(int i = 0 ; i < numPlayers ; i++)
                    printf("Player %d: %d\n", i + 1, PlayersHands[i].points);   //Prints number of points each player has
                roundOver = true;
            }
            
            //Determines next player, depending on direction determined by reverse
            if(reverse)
                playerToAct = playerToAct + (numPlayers - 1);
            else
                playerToAct++;
            playerToAct = playerToAct % numPlayers;
        }
        
        //Points: win detection for entire game
        for(int i = 0 ; i < numPlayers ; i++) {
            if(PlayersHands[i].points >= 500) {
                printf("Player %d wins the game!\n", i + 1);
                winner = true;
            }
        }
        //Prepares another round of play if no player has 500 points yet
        if(!winner) {
            printf("\nShuffling and dealing for the next round of play...\n");
            for(int i = 0 ; i < numPlayers ; i++)
                free(PlayersHands[i].playerHand);   //frees memory in player hands
            numCardsRemaining = MAX;
            numCardsDiscarded = 0;
            deck = initializeDeck();   //resets deck
            shuffleDeck(deck, numCardsRemaining);   //shuffles deck
        }
    }
    
    //frees all dynamically allocated memory and ends function
    for(int i = 0 ; i < numPlayers ; i++)
        free(PlayersHands[i].playerHand);
    free(deck);
    return 0;
}
