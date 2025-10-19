#include "card.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h> 
#define True 1
#define False 0



Card *create_card(const char *pin,
    const char *name,  const char *login, const char *phone_number){
    //memmory for NEw card
    Card *new_card = malloc(sizeof(Card));

    //Identify Person data
    create_card_number(new_card->card_number);
    strcpy(new_card->holder_name, name);
    strcpy(new_card->login, login);
    strcpy(new_card->phone_number, phone_number);

    //safety
    strcpy(new_card->pin, pin);
    new_card->is_blocked=False;
    new_card->pin_atempts=3;

    //Balance
    new_card->balance = 0;

    //OTHER
    generate_random_cvv(new_card->cvv);
    new_card->created_date = time(NULL);
    
    return new_card;
}


void generate_random_cvv(char *cvv_buffer){
    int new_cvv = (rand()%999)+100;
    sprintf(cvv_buffer, "%03d",new_cvv);
}

void create_card_number(char *card_number_buffer){
    card_number_buffer[0]='4';
    for (int i =1;i<16;i++){
        card_number_buffer[i] = '0' + rand()%10;
    }
    card_number_buffer[16] = '\0';
}
