#ifndef OPERATION_H
#define OPERATION_H
#include "card.h"
#include "database.h"
int transform_balance_by_CardNumber(Card *card1,  Card *card2, const double balance);
int transform_balance_by_PhoneNumber(Card *card1,  Card *card2, const double balance);


int replenish_balance(Card *card, const double add_money);
int withdraw_balance(Card *card, double witdraw_sum);


#endif