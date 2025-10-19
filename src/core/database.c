#include "database.h"
#include "card.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>



typedef struct Database{
    Card **cards;
    int capacity;
    int count;   
}Database;


Database *create_database(){
    Database *db = malloc(sizeof(Database));
    db->count=0;
    db->capacity=10;
    db->cards=malloc(sizeof(Card*)*db->capacity);
    for (int i = 0; i<db->capacity; i++){
        db->cards[i] = NULL;
    }
    return db;
}

void add_card_in_database(Database*db){
    char pin[5];
    char name[51];
    double balance;
    char login[31];
    char phone_number[12];

    printf("Введите Pin: ");
    scanf("%s",pin);
    printf("\n");

    printf("Введите имя: ");
    scanf("%s", name);
    printf("\n");

    printf("Введите логин: ");
    scanf("%s", login);
    printf("\n");

    printf("Введит свой номер телефона в формате 8XXX-XXX-XX-XX: ");
    scanf("%s",phone_number);

    checkcapacity(db);

    for(int i = 0; i<db->capacity; i++){
        if(db->cards[i]==NULL){
            db->cards[i] = create_card(pin,name,balance,login,phone_number);
            db->count++;
            break;
        }
    }
}

void checkcapacity(Database *db){
    if ((db)->count>=(db)->capacity){
        Card **new_cards = realloc(db->cards, sizeof(Card*)*(db->capacity*2));
        if (new_cards==NULL){
            printf("Не удалось расширить базу данных");
            exit(1);
        }
        for(int i = db->capacity; i<db->capacity*2;i++){
            new_cards[i] = NULL;
        }
        db->cards = new_cards;
        db->capacity*=2;
    }
}