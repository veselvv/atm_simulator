#ifndef OPERATION_H
#define OPERATION_H
#include "card.h"
#include "database.h"


void transform_balance_by_CardNumber(Database *db, const char *card1, const char *card2, const double balance);
void transform_balance_by_PhoneNumber(Database *db, const char *number1, const char *number2, const double balance);
void replenish_balance(Card *card, const double add_money);
void withdraw_balance(Card *card, double witdraw_sum);




#endif