#include "database.h"
#include "card.h"
#include "libs/cJSON.h"
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

void add_new_card_in_database(Database*db){
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
            Card *new_card = create_card(pin,name,login,phone_number);
            while (is_cardNumber_unique(db,new_card->card_number)!=1){
                free(new_card);
                new_card=NULL;
                new_card = create_card(pin,name,login,phone_number);
            }
            db->count++;
            break;
        }
    }
}

void add_card_in_db(Database *db, Card * card){
    if (db==NULL||card==NULL){
        return;
    }
    checkcapacity(db);
    for(int i = 0; i<db->capacity; i++){
        if(db->cards[i]==NULL){
            db->cards[i] = malloc(sizeof(Card));
            db->cards[i]->balance = card->balance;
            db->cards[i]->pin_atempts = card->pin_atempts;
            db->cards[i]->is_blocked = card->is_blocked;
            db->cards[i]->created_date = card->created_date;
            strcpy(db->cards[i]->card_number,card->card_number);
            strcpy(db->cards[i]->cvv, card->cvv);
            strcpy(db->cards[i]->expiry_date, card->expiry_date);
            strcpy(db->cards[i]->holder_name, card->holder_name);
            strcpy(db->cards[i]->login, card->login);
            strcpy(db->cards[i]->phone_number,card->phone_number);
            db->count++;
            break;
        }
    }
}

void checkcapacity(Database *db){
    if (db==NULL){
        return;
    }
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


Card *find_card_by_CardNumber(Database *db, const char *CardNumber){
    if(db==NULL || CardNumber==NULL){
        return NULL;
    }
    for (int i = 0; i<db->capacity; i++){
        if (db->cards[i]!=NULL){
            if (strcmp(db->cards[i]->card_number,CardNumber)==0){
                return db->cards[i];
            }
        }
    }
    return NULL;
}
Card *find_card_by_PhoneNumber(Database *db, const char *PhoneNumber){
    if (db == NULL||PhoneNumber==NULL){
        return NULL;
    }
    for (int i = 0; i<db->capacity; i++){
        if (db->cards[i]!=NULL){
            if (strcmp(db->cards[i]->phone_number, PhoneNumber)==0){
                return db->cards[i];
            }
        }
    }
    return NULL;
}
Card *find_card_by_Login(Database *db,const char *login){
    if(db == NULL|| login==NULL){
        return NULL;
    }
    for (int i =0; i<db->capacity;i++){
        if (db->cards[i]!=NULL){
            if (strcmp(db->cards[i]->login, login)==0){
                return db->cards[i];
            }
        }
    }
    return NULL;
}


Database *find_card_by_name(Database *db, const char *name){
    if (db == NULL||name==NULL){
        return NULL;
    }
    Database *new_db_with_holders = create_database();
    if (new_db_with_holders==NULL){
        return NULL;
    }
    for(int i =0; i<db->capacity;i++){
        if (db->cards[i]!=NULL){
            if (strcmp(db->cards[i]->holder_name, name)==0){
                add_card_in_db(new_db_with_holders, db->cards[i]);
            }
        }
    }
    return new_db_with_holders;
    
}

void remove_card_from_db_by_number(Database *db, const char *CardNumber){
    if(db==NULL||CardNumber==NULL){
        return;
    }
    for (int i =0; i<db->capacity; i++){
        if (db->cards[i]!=NULL){
            if (strcmp(db->cards[i]->card_number, CardNumber)==0){
                free(db->cards[i]); 
                db->cards[i]=NULL;
                db->count--;
                return;
            }
        }
    }
    printf("карта %s не найдена", CardNumber);
}



int is_cardNumber_unique(Database * db, const char *CardNumber){
    if(db==NULL){
        return 0;
    }
    for(int i =0; i <db->capacity; i++){
        if (db->cards[i]!=NULL){
            if(strcmp(db->cards[i]->card_number, CardNumber)==0){
                return 0;
            }
        }
    }
    return 1;
}


void update_card_balance(Database * db, const char *CardNumber, const double new_balance){
    if (db==NULL||CardNumber==NULL){
        return;
    }
    for (int i =0; i<db->capacity; i++){
        if (db->cards[i]!=NULL){
            if (strcmp(db->cards[i]->card_number, CardNumber)==0){
                db->cards[i]->balance = new_balance;
                return;
            }
        }
    }
    printf("Карта %s не найдена\n",CardNumber);
}


void update_card_pin(Database *db, const char *new_pin, const char *CardNumber){
    if (db==NULL||new_pin==NULL){
        return;
    }
    for (int i =0;i<db->capacity; i++){
        if (db->cards[i]!=NULL){
            if(strcmp(db->cards[i]->card_number,CardNumber)==0){
                strcpy(db->cards[i]->pin, new_pin);
                return;
            }
        }
    }
    printf("Карта %s не найдена\n", CardNumber);
}


void update_card_blocked_status(Database *db, int new_is_blocked, const char *CardNumber){
    if (db==NULL||CardNumber==NULL){
        return;
    }
    for (int i = 0; i<db->capacity; i++){
        if (db->cards[i]!=NULL){
            if(strcmp(db->cards[i]->card_number, CardNumber)==0){
                db->cards[i]->is_blocked = new_is_blocked;
                return;
            }
        }
    }
    printf("Карта %s не найдена",CardNumber);
}