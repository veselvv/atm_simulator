#ifndef DATABASE_H
#define DATABASE_H
#include "card.h"

typedef struct Database{
    Card **cards;
    int capacity;
    int count;   
}Database;

Database *create_database();
void add_new_card_in_database(Database*db);
void add_card_in_db(Database *db, Card * card);
void checkcapacity(Database *db);
Card *find_card_by_CardNumber(Database *db, const char *CardNumber);
Card *find_card_by_PhoneNumber(Database *db, const char *PhoneNumber);
Card *find_card_by_Login(Database *db,const char *login);
Database *find_card_by_name(Database *db, const char *name);
void remove_card_from_db_by_number(Database *db, const char *CardNumber);
int is_cardNumber_unique(Database * db, const char *CardNumber);
void update_card_balance(Database * db, const char *CardNumber, const double new_balance);
void update_card_pin(Database *db, const char *new_pin, const char *CardNumber);
void update_card_blocked_status(Database *db, int new_is_blocked, const char *CardNumber);
void printDatabase(Database *db);
void free_Database(Database *db);

#endif