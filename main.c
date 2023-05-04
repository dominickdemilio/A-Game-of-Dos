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
void drawCard(hand* array, card *deck) {
    array->numCards++;   //number of cards in player's hand increases by 1
    addNewNodeToBeginning(&array->playerHand, *deck);   //copy the top card in the deck to player's hand
    deleteFirstNode(&deck);   //removes top card from the deck
}

card* init_deck(void) {
    int shuffle = 0;
    card *deck = (card*)malloc(MAX * sizeof(card)); // allocates memory for the entire deck of 108 cards

    while((shuffle != 1) && (shuffle != 2)) {
        printf("Press 1 to shuffle the DOS deck or 2 to load a deck from a file: ");
        scanf(" %d", &shuffle);
        if(shuffle == 1) {
            deck = createDeck(); //Creates deck from scratch
            shuffleDeck(deck); //Shuffles deck
            printf("The deck has been shuffled and dealt\n");
        }
        if(shuffle == 2) {
            char filename[100];
            printf("Enter file name (DosDeck.txt): ");
            scanf(" %[^\n]s", filename);
            deck = readDeck(filename); //Reads from file
            printf("The deck has been loaded and dealt\n");
        }
    }
    return deck;
}

int init_players(void) {
    int numPlayers = 0;
    while(numPlayers < 1 || numPlayers > MAX_NUM_PLAYERS) {
        printf("Let's play a game of DOS\nEnter number of players: ");
        scanf("%d", &numPlayers);
    }
    return numPlayers;
}

void playerTurn(hand* currentHand, hand* centerRow, card* deck) {
    int cardNumToPlay;
    card activeCenterCard;
    
    //Prints discard card and player's hand
    printf("Player %d's hand: ", currentHand->playerNum);
    print(currentHand->playerHand);
    printf("\nCenter Row: ");
    print(centerRow->playerHand);

    //Player decides which card to play
    for (int i = 0 ; i < centerRow->numCards ; i++) {
        activeCenterCard = *centerRow->playerHand;
        printf("How many cards do you want to play on %d %s: ", activeCenterCard.value, activeCenterCard.color);
    }
}



//THE MAIN FUNCTION
int main(void) {
    int numCardsRemaining = MAX;
    int numCardsDiscarded = 0;
    card cardSelectedByUser;
    bool winner = false;
    int playerToAct = 0;
    
    //set up players and the deck
    int numPlayers = init_players();
    card* deck = init_deck();
        
    //initialize hands
    hand PlayersHands[numPlayers];
    for(int i = 0 ; i < numPlayers ; i++) {
        PlayersHands[i].points = 0;
        PlayersHands[i].playerNum = i + 1;
    }
    
    //Deals the cards to the players
    for(int i = 0 ; i < numPlayers ; i++) {
        PlayersHands[i].playerHand = NULL;
        PlayersHands[i].numCards = 0;
    }
    for(int i = 0 ; i < HANDSIZE; i++)
        for(int j = 0 ; j < numPlayers ; j++)
            drawCard(&PlayersHands[j], deck); //function "deals" cards to each player in alternating order by taking top card off the deck and adding it to their hand
    
    //Turns over first 2 cards
    hand centerRow;
    for(int i = 0 ; i < 2 ; i++)
        drawCard(&centerRow, deck);
    
    //Start of Gameplay
    while(!winner) {
        playerTurn(&PlayersHands[playerToAct], &centerRow, deck);
        
        
        
    }

    //frees all dynamically allocated memory and ends function
    for(int i = 0 ; i < numPlayers ; i++)
        free(PlayersHands[i].playerHand);
    free(deck);
    return 0;
}
