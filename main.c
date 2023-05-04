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

#include "miscellaneous.h"

#define MAX 108
#define HANDSIZE 7
#define MAX_NUM_PLAYERS 6

//draws a card from the deck, receives deck, player's hand, & number of cards in draw pile as input. No output necessary
void drawCard(hand* array, card **deck, int *numCardsRemaining) {
    array->numCards++;   //number of cards in player's hand increases by 1
    addNewNodeToBeginning(&array->playerHand, **deck);   //copy the top card in the deck to player's hand
    deleteFirstNode(deck);   //removes top card from the deck
    *numCardsRemaining = *numCardsRemaining - 1;
}

//THE MAIN FUNCTION
int main(void) {
    int numCardsRemaining = MAX;
    int numCardsDiscarded = 0;
    int numPlayers = 0;
    int shuffle = 0;
    card topDiscard, selectedCardByUser; //temporary variables to store one card at a time
    card *deck = (card*)malloc(numCardsRemaining * sizeof(card)); // allocates memory for the entire deck of 108 cards
    
    //Game Play Set Up
    while(numPlayers < 1 || numPlayers > MAX_NUM_PLAYERS) {
        printf("Let's play a game of UNO\nEnter number of human players: ");
        scanf("%d", &numPlayers);
    }
    
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
    
    //initializes points and player numbers
    hand PlayersHands[numPlayers];
    for(int i = 0 ; i < numPlayers ; i++) {
        PlayersHands[i].points = 0;
        PlayersHands[i].playerNum = i + 1;
    }
    
    bool winner = false;
    while(!winner) {
        int playerToAct = 0;
        
        //Deals the cards to the players
        for(int i = 0 ; i < numPlayers ; i++) {
            PlayersHands[i].playerHand = NULL;
            PlayersHands[i].numCards = 0;
        }
        for(int i = 0 ; i < HANDSIZE; i++)
            for(int j = 0 ; j < numPlayers ; j++)
                drawCard(&PlayersHands[j], &deck, &numCardsRemaining); //function "deals" cards to each player in alternating order by taking top card off the deck and adding it to their hand
        
        //Turns over first 2 cards
        hand centerRow;
        for(int i = 0 ; i < 2 ; i++)
            drawCard(&centerRow, &deck, &numCardsRemaining);
        
        //Start of Gameplay
        bool roundOver = false;
        int cardNumToPlay;
        while(!roundOver) {
            bool turnActive = true;

            while(turnActive) {
                //Prints discard card and player's hand
                printf("Player %d's hand: ", PlayersHands[playerToAct].playerNum);
                print(PlayersHands[playerToAct].playerHand);
                printf("\nCenter Row: ");
                print(centerRow.playerHand);

                //Player decides which card to play
                if(PlayersHands[playerToAct].numCards == 1)
                    printf("Press 1 to play the card from your hand, or press 0 to draw a card from the deck: ");
                else
                    printf("Type 1-%d to play any card from your hand, or type 0 to draw from the deck: ", PlayersHands[playerToAct].numCards);
                scanf(" %d", &cardNumToPlay);

                //Player chooses to draw a card
                if(cardNumToPlay == 0) {
                    drawCard(&PlayersHands[playerToAct], &deck, &numCardsRemaining);
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
                        //if player is Computer
                        if(playerToAct == numPlayers - 1 && bot == 'y') {
                            srand((int)time(0));
                            int nextColorNum = (rand() % 4) + 1;
                            printf("Computer chose next color: ");
                        }
                        
            


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
                        printf("%s (Remember you must play a card)\n", topDiscard.action);
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

            playerToAct = (playerToAct + 1) % numPlayers;
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
