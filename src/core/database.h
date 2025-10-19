#ifndef DATABASE_H
#define DATABASE_H
#include "card.h"

typedef struct Database{
    Card **cards;
    int capacity;
    int count;   
}Database;

Database *create_database();
void add_card_in_database(Database*db);
void checkcapacity(Database *db);





#endif