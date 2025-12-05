#include <sys/stat.h>
#include "logging_operations.h"
#include <stdlib.h>
#include<stdio.h>
#include<string.h>
#include "file_io.h"
#include "./libs/cJSON.h"


cJSON *operation_to_json(const Operation *op){
    if(op==NULL){
        return NULL;
    }
    cJSON *op_json = cJSON_CreateObject();
    if(op_json==NULL){
        return NULL;
    }
    cJSON_AddStringToObject(op_json,"targetCard", op->targetCard);
    cJSON_AddNumberToObject(op_json,"balance_before",op->balance_before);
    cJSON_AddNumberToObject(op_json, "balance_after",op->balance_after);
    cJSON_AddNumberToObject(op_json,"amount",op->amount);
    cJSON_AddStringToObject(op_json,"output_opType",op->output_opType);
    cJSON_AddNumberToObject(op_json,"op_type",op->op_type);
    char date_str[20];
    struct tm* timeinfo = localtime(&op->created_date);
    strftime(date_str, sizeof(date_str), "%Y-%m-%d %H:%M:%S", timeinfo);
    cJSON_AddStringToObject(op_json, "created_date", date_str);
    return op_json;
}

Operation *json_to_op(const cJSON *op_json){
    if(op_json==NULL){
        return NULL;
    }

    Operation *op=malloc(sizeof(Operation));
    if(op==NULL){
        return NULL;
    }
    memset(op,0,sizeof(Operation));

    cJSON *item;

    item = cJSON_GetObjectItemCaseSensitive(op_json, "targetCard");
    if (cJSON_IsString(item)&& item->valuestring!=NULL){
        strncpy(op->targetCard,item->valuestring,sizeof(op->targetCard)-1);
    }
    item=cJSON_GetObjectItemCaseSensitive(op_json, "output_opType");
    if(cJSON_IsString(item)&&item->valuestring!=NULL){
        strncpy(op->output_opType, item->valuestring,sizeof(op->targetCard)-1);
    }
    item=cJSON_GetObjectItemCaseSensitive(op_json,"op_type");
    if(cJSON_IsNumber(item)){
        op->op_type=item->valueint;
    }
    item=cJSON_GetObjectItemCaseSensitive(op_json,"balance_before");
    if(cJSON_IsNumber(item)){
        op->balance_before=item->valuedouble;
    }
    item=cJSON_GetObjectItemCaseSensitive(op_json,"balance_after");
    if(cJSON_IsNumber(item)){
        op->balance_after=item->valuedouble;
    }
    item=cJSON_GetObjectItemCaseSensitive(op_json,"amount");
    if(cJSON_IsNumber(item)){
        op->amount=item->valuedouble;
    }
    item = cJSON_GetObjectItemCaseSensitive(op_json, "created_date");
    if (cJSON_IsString(item) && item->valuestring != NULL) {
        struct tm tm = {0};
        int year, month, day, hour, min, sec;
        
        if (sscanf(item->valuestring, "%d-%d-%d %d:%d:%d", 
                &year, &month, &day, &hour, &min, &sec) == 6) {
            tm.tm_year = year - 1900;  // Год с 1900
            tm.tm_mon = month - 1;     // Месяц 0-11
            tm.tm_mday = day;
            tm.tm_hour = hour;
            tm.tm_min = min;
            tm.tm_sec = sec;
            tm.tm_isdst = -1;
            op->created_date = mktime(&tm);
        } else {
            // Если не удалось распарсить
            op->created_date = time(NULL);
        }
    } else {
        // Если даты нет в JSON
        op->created_date = time(NULL);
    }

    return op;
}


int append_operation_to_log(Card *card, Operation *op) {
    if (!card || !op) return 0;
    
    // Создаем папки с проверкой
    struct stat st = {0};
    
    if (stat("../../data", &st) == -1) {
        mkdir("../../data", 0755);
    }
    
    if (stat("../../data/logs", &st) == -1) {
        mkdir("../../data/logs", 0755);
    }
    
    // Имя файла
    char filename[256];
    snprintf(filename, sizeof(filename), 
             "../../data/logs/operations_%s.json", card->card_number);
    
    // Читаем старые данные
    cJSON *root = NULL;
    char *old = read_file_to_string(filename);
    
    if (old) {
        root = cJSON_Parse(old);
        free(old);
    }
    
    // Если не массив, создаем новый
    if (!root || !cJSON_IsArray(root)) {
        if (root) cJSON_Delete(root);
        root = cJSON_CreateArray();
    }
    
    // Добавляем новую операцию
    cJSON *new_op = operation_to_json(op);
    if (!new_op) {
        cJSON_Delete(root);
        return 0;
    }
    
    cJSON_AddItemToArray(root, new_op);
    
    // Сохраняем
    char *json = cJSON_Print(root);
    int ok = write_file(filename, json);
    
    if (json) free(json);
    cJSON_Delete(root);
    
    return ok;
}

// int load_Database_from_json(const char *filename, Database *db){
//     if(db==NULL || filename==NULL){
//         return 0;
//     }

//     char *json_string = read_file_to_string(filename);

//     if (json_string==NULL){
//         return 0;
//     }

//     cJSON *root = cJSON_Parse(json_string);

//     if (root==NULL){
//         free(json_string);
//         return 0;
//     }


//     if(!cJSON_IsArray(root)){
//         cJSON_Delete(root);
//         free(json_string);
//         return 0;
//     }

//     int loaded_card = 0;
//     cJSON *item;
//     cJSON_ArrayForEach(item,root){
//         Operation *op=json_to_op(item);
//         if (op!=NULL){
//             add_card_in_db(db, op);
//             loaded_card++;
//         }
//     }
    
//     free(json_string);
//     cJSON_Delete(root);
//     return 1;
// }