#ifndef LOGGING_OPERATIONS_H
#define LOGGING_OPERATIONS_H
#include "card.h"
#include "./libs/cJSON.h"
#include <time.h> 




typedef enum{
    OP_WITHDRAW,
    OP_REPLENISH,
    OP_TRANSFER_IN,
    OP_TRANSFER_OUT
}OperationType;


typedef struct Operation{
    OperationType op_type;
    double balance_before;
    double balance_after;
    double amount;
    char targetCard[17];
    char output_opType[20];
    time_t created_date;
}Operation;


cJSON *operation_to_json(const Operation *op);
Operation *json_to_op(const cJSON *op_json);
int append_operation_to_log(Card *card, Operation *op);

#endif