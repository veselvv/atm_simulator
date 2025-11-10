#ifndef OPERATION_H
#define OPERATION_H
#include "card.h"
#include "database.h"


void transform_balance_by_CardNumber(Database *db, const char *card1, const char *card2, const float balance);
void transform_balance_by_PhoneNumber(Database *db, const char *number1, const char *number2, const float balance);
void replenish_balance(Database *db, const char *number, const float add_money);
void withdraw_balance(Database *db, const char *number, float witdraw_sum);




#endif