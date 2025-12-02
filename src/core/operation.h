#ifndef OPERATION_H
#define OPERATION_H
#include "card.h"
#include "database.h"
void transform_balance_by_CardNumber(Card *card1,  Card *card2, const double balance);
void transform_balance_by_PhoneNumber(Card *card1,  Card *card2, const double balance);


void replenish_balance(Card *card, const double add_money);
void withdraw_balance(Card *card, double witdraw_sum);




#endif