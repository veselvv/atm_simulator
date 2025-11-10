#include "card.h"
#include "database.h"
#include "operation.h"
#include<stdio.h>
#include <stdlib.h>



void transform_balance_by_CardNumber(Database *db, const char *card1, const char *card2, const float balance){
    if (db==NULL|| card1==NULL||card2==NULL){
        printf("Err: Unvalid data\n");
        return;
    }
    Card *card_from = find_card_by_CardNumber(db, card1);
    Card *card_to = find_card_by_CardNumber(db,card2);


    if (card_from==NULL||card_to==NULL){    
        return;
    }

    if (card_from->is_blocked==1){
        printf("Нельзя выполнить перевод с заблокированной карты.\n");
        return;
    }
    if (card_to->is_blocked==1){
        printf("Нельзя выполнить перевод на заблокированную карту.\n");
        return;
    }
    if (balance<=0){
        printf("Баланс должен быть больше 0.\n");
        return;
    }
    if (card_from->balance<balance){
        printf("Неудалось выполнить перевод, недостаточно средств.\n");
        return;
    }
    card_from->balance = card_from->balance-balance;
    card_to->balance = card_to->balance+balance;
    printf("Перевод успешно выполнен\n");
}



void transform_balance_by_PhoneNumber(Database *db, const char *number1, const char *number2, const float balance){
    if (db==NULL|| number1==NULL||number2==NULL){
        printf("Err: Unvalid data\n");
        return;
    }
    Card *card_from = find_card_by_PhoneNumber(db, number1);
    Card *card_to = find_card_by_PhoneNumber(db,number2);


    if (card_from==NULL||card_to==NULL){    
        return;
    }

    if (card_from->is_blocked==1){
        printf("Нельзя выполнить перевод с заблокированной карты.\n");
        return;
    }
    if (card_to->is_blocked==1){
        printf("Нельзя выполнить перевод на заблокированную карту.\n");
        return;
    }
    if (balance<=0){
        printf("Баланс для перевода должен быть больше 0.\n");
        return;
    }
    if (card_from->balance<balance){
        printf("Неудалось выполнить перевод, недостаточно средств.\n");
        return;
    }
    card_from->balance = card_from->balance-balance;
    card_to->balance = card_to->balance+balance;
    printf("Перевод успешно выполнен!\n");
}


void replenish_balance(Database *db, const char *number, const float add_money){
    if(db==NULL||number==NULL){
        printf("Err:invalid data\n");
        return;
    }
    
    Card *card = find_card_by_CardNumber(db,number);

    if (card==NULL){
        printf("Err: не удалось найти карту!\n");
        return;
    }

    if(card->is_blocked==1){
        printf("Err: Нельзя пополнить баланс заблокированной карты\n");
        return ;
    }
    if(add_money<=0){
        printf("Err: Сумма для пополнения должна быть положительной!\n");
        return;
    }
    card->balance+=add_money;
    printf("Пополнение баланса выполнено успешно!\n");
    return;
}



void withdraw_balance(Database *db, const char *number, float witdraw_sum){
    if(db == NULL|| number == NULL){
        printf("Err:invalid data\n");
        return;
    }
    Card *card = find_card_by_CardNumber(db, number);
    
    if(card==NULL){
        printf("Err: не удалось найти карту\n");
        return;
    }

    if(card->is_blocked==1){
        printf("Err: Нельзя снять деньги с заблокированной карты\n");
        return;
    }

    if(witdraw_sum<=0){
        printf("Err: сумма для снятия должна быть положительная!\n");
        return ;
    }

    if(card->balance<witdraw_sum){
        printf("Err: не удалось снять деньги, на счете недостаточно средст, ддя осуществления данной операции!\n");
        return;
    }

    card->balance-=witdraw_sum;
    printf("Снятие денег с баланса выполнено успешно!\n");
}