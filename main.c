#define _CRT_SECURE_NO_WARNINGS
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

bool validPlay(card centerCard, int input1, int input2, hand* currentHand) {
    card card1, card2;
    hand tempHand = *currentHand;

    for (int i = 1; i < currentHand->numCards + 1; i++) {
        if (i == input1)
            card1 = *tempHand.playerHand;
        if (i == input2)
            card2 = *tempHand.playerHand;
        tempHand.playerHand = tempHand.playerHand->pt;
    }

    // 1 input
    if (input2 < 0) {
        if (card1.value == centerCard.value || strcmp(card1.action, "AnyNumber") == 1 || strcmp(centerCard.action, "AnyNumber") == 1) {
            deleteNode(&currentHand->playerHand, input1);
            return true;
        }
        else {
            print(&card1);
            printf("Invalid play.\n");
            return false;
        }
    }

    // 2 inputs
    else {
        if (card1.value + card2.value == centerCard.value || strcmp(card1.action, "AnyNumber") == 1 || strcmp(card2.action, "AnyNumber") == 1) {
            deleteNode(&currentHand->playerHand, input1);
            deleteNode(&currentHand->playerHand, input2);
            return true;
        }
        else {
            print(&card1);
            printf("Invalid play.\n");
            return false;
        }
    }

    // THIS FUNCTION NEEDS TO BE POLISHED
}

void playerTurn(hand* currentHand, hand* centerRow, card* deck) {
    int userInput;
    int userInput2;
    card activeCenterCard;

    //Prints discard card and player's hand
    printf("Player %d's hand: ", currentHand->playerNum);
    print(currentHand->playerHand);
    printf("\nCenter Row: ");
    print(centerRow->playerHand);

    //Player decides which card to play
    for (int i = 0; i < centerRow->numCards; i++) {
        activeCenterCard = *centerRow->playerHand;
        printf("How many cards do you want to play on %d %s: ", activeCenterCard.value, activeCenterCard.color);
        scanf("%d", &userInput);

        // chooses what card
        if (userInput == 1) {
            printf("Select a card from 1-%d: ", currentHand->numCards);
            scanf("%d", &userInput);
            validPlay(*centerRow->playerHand, userInput, -1, currentHand);
        }
        else if (userInput == 2) {
            printf("Select 2 cards from 1-%d: ", currentHand->numCards);
            scanf("%d, %d", &userInput, &userInput2);
            validPlay(*centerRow->playerHand, userInput, userInput2, currentHand);
        }
        else if (i == centerRow->numCards - 1) {
            break;
        }
        
        // NEEDS LOGIC HERE

        activeCenterCard = *activeCenterCard.pt;
    }
    
    if(centerRow->numCards == 0) {
        return;
    }

    // AND LOGIC HERE

    // THIS STUFF IS FOR WHEN THE USER DOES NOT PLAY A CARD, AND HAS TO DRAW
    drawCard(currentHand, &deck);
    print(currentHand->playerHand);
    printf("You drew a card. Press 1 to make a match or 0 to discard: ");
    scanf("%d", &userInput);

}

//THE MAIN FUNCTION
int main(void) {
    bool winner = false;
    int playerToAct = 0;
    hand centerRow;

    centerRow.playerHand = NULL;
    centerRow.playerHand = 0;

    //set up players and the deck
    int numPlayers = init_players();
    card* deck = init_deck();

    //initialize hands
    hand PlayersHands[numPlayers];
    for (int i = 0; i < numPlayers; i++) {
        PlayersHands[i].points = 0;
        PlayersHands[i].playerNum = i + 1;
        PlayersHands[i].playerHand = NULL;
        PlayersHands[i].numCards = 0;
    }
    for (int i = 0; i < HANDSIZE; i++)
        for (int j = 0; j < numPlayers; j++)
            drawCard(&PlayersHands[j], &deck); //function "deals" cards to each player in alternating order by taking top card off the deck and adding it to their hand

    //Turns over first 2 cards
    for (int i = 0; i < 2; i++)
        drawCard(&centerRow, &deck);

    //Start of Gameplay
    while (!winner) {
        playerTurn(&PlayersHands[playerToAct], &centerRow, deck);
        
        while (centerRow.numCards < 2) {
            drawCard(&centerRow, &deck);
        }
        
        // NEED MORE GAMEPLAY LOGIC HERE
        
        
        // next player
        playerToAct++;
        playerToAct = playerToAct % numPlayers;
    }

    //frees all dynamically allocated memory and ends function
    for (int i = 0; i < numPlayers; i++)
        free(PlayersHands[i].playerHand);
    free(deck);
    return 0;
}
