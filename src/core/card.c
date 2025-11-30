#include "card.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h> 
#define True 1
#define False 0



Card *create_card(const char *pin,
    const char *name,  const char *login, const char *phone_number, const char *gender){
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
    generate_expiry_date(new_card->expiry_date);
    strcpy(new_card->gender,gender);



    
    return new_card;
}


void generate_random_cvv(char *cvv_buffer){
    int new_cvv = (rand()%999)+100;
    sprintf(cvv_buffer, "%03d",new_cvv);
}

void generate_expiry_date(char *expiry_buffer) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    tm_info->tm_year += 4;  
    tm_info->tm_mon = 11;   
    tm_info->tm_mday = 31;  
    strftime(expiry_buffer, 9, "%m/%Y", tm_info);   
}

void create_card_number(char *card_number_buffer){
    card_number_buffer[0]='4';
    for (int i =1;i<16;i++){
        card_number_buffer[i] = '0' + rand()%10;
    }
    card_number_buffer[16] = '\0';
}

void print_card_info(Card *card){
    printf("Владелец: %s\n", card->holder_name);
    printf("Номер карты: %s\n", card->card_number);
    printf("login: %s\n", card->login);
    printf("Номер телефона: %s\n", card->phone_number);
    printf("CVV: %s\n",card->cvv);
    printf("Баланс: %f\n",card->balance);
    printf("Срок действия %s\n",card->expiry_date);
    printf("Статус блокировки: %d\n",card->is_blocked);
    printf("Осталось количество попыток ввода PIN: %d\n",card->pin_atempts);
    printf("PIN-код: %s\n",card->pin);
    printf("Пол: %s\n",card->gender);
}


