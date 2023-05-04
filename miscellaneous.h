//
//  miscellaneous.h
//  Final Project
//
//  Created by Dominick D’Emilio on 4/22/22.
//

#ifndef miscellaneous_h
#define miscellaneous_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>

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

#endif /* miscellaneous_h */
