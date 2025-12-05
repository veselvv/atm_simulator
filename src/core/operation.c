#include "card.h"
#include "database.h"
#include "operation.h"
#include "logging_operations.h"
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>



void transform_balance_by_CardNumber(Card *card1,  Card *card2, const double balance){
    
    if (card1==NULL||card2==NULL){
        printf("Err: Unvalid data\n");
        return;
    }
    if (card1==NULL||card2==NULL){    
        return;
    }

    if (card1->is_blocked==1){
        printf("Нельзя выполнить перевод с заблокированной карты.\n");
        return;
    }
    if (card2->is_blocked==1){
        printf("Нельзя выполнить перевод на заблокированную карту.\n");
        return;
    }
    if (balance<=0){
        printf("Баланс должен быть больше 0.\n");
        return;
    }
    if (card1->balance<balance){
        printf("Неудалось выполнить перевод, недостаточно средств.\n");
        return;
    }
    Operation *op1 = malloc(sizeof(Operation));
    Operation *op2 = malloc(sizeof(Operation));
    if (op1 == NULL || op2 == NULL) {
        printf("Ошибка выделения памяти для операций\n");
        if (op1) free(op1);
        if (op2) free(op2);
        return;
    }

    // Инициализируем память нулями
    memset(op1, 0, sizeof(Operation));
    memset(op2, 0, sizeof(Operation));

    op1->balance_before=card1->balance;
    op2->balance_before=card2->balance;
    op1->created_date=time(NULL);
    op2->created_date=time(NULL);
    op1->op_type=OP_TRANSFER_OUT;
    op2->op_type=OP_TRANSFER_IN;
    op1->amount=balance;
    op2->amount=balance;
    strncpy(op1->targetCard, card2->card_number, sizeof(op1->targetCard) - 1);
    strncpy(op2->targetCard, card1->card_number, sizeof(op2->targetCard) - 1);
    strncpy(op1->output_opType, "TRANSFER_OUT", sizeof(op1->output_opType) - 1);
    strncpy(op2->output_opType, "TRANSFER_IN", sizeof(op2->output_opType) - 1);
    card1->balance = card1->balance-balance;
    card2->balance = card2->balance+balance;
    op1->balance_after=card1->balance;
    op2->balance_after=card2->balance;
    printf("Перевод успешно выполнен\n");
    append_operation_to_log(card1,op1);
    append_operation_to_log(card2,op2);

    free(op1);
    free(op2);
}


void transform_balance_by_PhoneNumber(Card *card1,  Card *card2, const double balance){
    if (card1==NULL||card2==NULL){    
        printf("Не удалось найти карту\n");
        return;
    }

    if (card1->is_blocked==1){
        printf("Нельзя выполнить перевод с заблокированной карты.\n");
        return;
    }
    if (card2->is_blocked==1){
        printf("Нельзя выполнить перевод на заблокированную карту.\n");
        return;
    }
    if (balance<=0){
        printf("Баланс для перевода должен быть больше 0.\n");
        return;
    }
    if (card1->balance<balance){
        printf("Неудалось выполнить перевод, недостаточно средств.\n");
        return;
    }
        Operation *op1 = malloc(sizeof(Operation));
    Operation *op2 = malloc(sizeof(Operation));
    if (op1 == NULL || op2 == NULL) {
        printf("Ошибка выделения памяти для операций\n");
        if (op1) free(op1);
        if (op2) free(op2);
        return;
    }

    // Инициализируем память нулями
    memset(op1, 0, sizeof(Operation));
    memset(op2, 0, sizeof(Operation));

    op1->balance_before=card1->balance;
    op2->balance_before=card2->balance;
    op1->created_date=time(NULL);
    op2->created_date=time(NULL);
    op1->op_type=OP_TRANSFER_OUT;
    op2->op_type=OP_TRANSFER_IN;
    op1->amount=balance;
    op2->amount=balance;
    strncpy(op1->targetCard, card2->phone_number, sizeof(op1->targetCard) - 1);
    strncpy(op2->targetCard, card1->phone_number, sizeof(op2->targetCard) - 1);
    strncpy(op1->output_opType, "TRANSFER_OUT", sizeof(op1->output_opType) - 1);
    strncpy(op2->output_opType, "TRANSFER_IN", sizeof(op2->output_opType) - 1);
    card1->balance = card1->balance-balance;
    card2->balance = card2->balance+balance;
    op1->balance_after=card1->balance;
    op2->balance_after=card2->balance;
    printf("Перевод успешно выполнен\n");
    append_operation_to_log(card1,op1);
    append_operation_to_log(card2,op2);
    free(op1);
    free(op2);
}


void replenish_balance(Card *card, const double add_money){
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
    Operation *op=malloc(sizeof(Operation));
    if (op==NULL){
        printf("Не удалось выделить память под операцию\n");
        return;
    }
    memset(op,0,sizeof(Operation));
    op->balance_before=card->balance;
    op->created_date=time(NULL);
    op->amount = add_money;
    op->op_type=OP_REPLENISH;
    strncpy(op->targetCard, card->card_number, sizeof(op->targetCard) - 1);
    strncpy(op->output_opType, "OP_REPLENISH", sizeof(op->output_opType) - 1);
    card->balance+=add_money;
    op->balance_after=card->balance;
    append_operation_to_log(card,op);
    printf("Пополнение баланса выполнено успешно!\n");
    free(op);
    return;
}



void withdraw_balance(Card *card, double witdraw_sum){    
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
    Operation *op = malloc(sizeof(Operation));
    if(op==NULL){
        printf("Ошибка выделения памяти под операцию\n");
        return;
    }
    memset(op,0,sizeof(Operation));
    op->balance_before=card->balance;
    op->created_date=time(NULL);
    op->amount = witdraw_sum;
    op->op_type=OP_WITHDRAW;
    strncpy(op->targetCard, card->card_number, sizeof(op->targetCard) - 1);
    strncpy(op->output_opType, "OP_WITHDRAW", sizeof(op->output_opType) - 1);
    card->balance-=witdraw_sum;
    op->balance_after=card->balance;
    append_operation_to_log(card,op);
    free(op);
    printf("Снятие денег с баланса выполнено успешно!\n");

}