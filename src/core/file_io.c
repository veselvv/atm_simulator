#include "libs/cJSON.h"
#include "database.h"
#include "card.h"
#include "file_io.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char * read_file_to_string(const char *filename){
    FILE *fp = fopen(filename, "r");
    if(fp==NULL){
        printf("Не удалось прочитать файл\n");
        return NULL;
    }
    fseek(fp,0,SEEK_END);
    size_t file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *content = malloc(file_size+1);

    fread(content, 1,file_size,fp);
    content[file_size]='\0';
    fclose(fp);
    return content;
}

cJSON *card_to_json(const Card *card){
    if (card == NULL){
        return NULL;
    }
    cJSON *card_json = cJSON_CreateObject();
    cJSON_AddStringToObject(card_json, "card_number",card->card_number);
    cJSON_AddStringToObject(card_json, "cvv",card->cvv);
    cJSON_AddStringToObject(card_json, "expiry_date",card->expiry_date);
    cJSON_AddStringToObject(card_json, "holder_name",card->holder_name);
    cJSON_AddStringToObject(card_json, "login", card->login);
    cJSON_AddStringToObject(card_json, "phone_number", card->phone_number);
    cJSON_AddStringToObject(card_json, "pin", card->pin);
    cJSON_AddNumberToObject(card_json, "balance", card->balance);
    cJSON_AddNumberToObject(card_json, "is_blocked", card->is_blocked);
    cJSON_AddNumberToObject(card_json, "pin_atempts",card->pin_atempts);
    cJSON_AddStringToObject(card_json, "gender",card->gender);
    char date_str[20];
     struct tm* timeinfo = localtime(&card->created_date);
    strftime(date_str, sizeof(date_str), "%Y-%m-%d %H:%M:%S", timeinfo);
    cJSON_AddStringToObject(card_json, "created_date", date_str);
    return card_json;
}



Card *json_to_card(const cJSON *json_card){
    if (json_card==NULL){
        return NULL;
    }
    Card *card = malloc(sizeof(Card));
    if (card == NULL){
        return NULL;
    }
    memset(card, 0, sizeof(Card));
    cJSON *item;


    item = cJSON_GetObjectItemCaseSensitive(json_card, "gender");
    if (cJSON_IsString(item) && item->valuestring!=NULL){
        strncpy(card->gender, item->valuestring, sizeof(card->gender)-1);
    }
    item = cJSON_GetObjectItemCaseSensitive(json_card, "card_number");
    if (cJSON_IsString(item) && item->valuestring!=NULL){
        strncpy(card->card_number, item->valuestring, sizeof(card->card_number)-1);
    }
    item = cJSON_GetObjectItemCaseSensitive(json_card, "holder_name");
    if (cJSON_IsString(item) && item->valuestring!=NULL){
        strncpy(card->holder_name, item->valuestring, sizeof(card->holder_name)-1);
    }
    item = cJSON_GetObjectItemCaseSensitive(json_card, "pin");
    if (cJSON_IsString(item) && item->valuestring!=NULL){
        strncpy(card->pin, item->valuestring, sizeof(card->pin)-1);
    }
    item = cJSON_GetObjectItemCaseSensitive(json_card, "cvv");
    if (cJSON_IsString(item) && item->valuestring!=NULL){
        strncpy(card->cvv, item->valuestring, sizeof(card->cvv)-1);
    }
    item = cJSON_GetObjectItemCaseSensitive(json_card, "expiry_date");
    if (cJSON_IsString(item) && item->valuestring!=NULL){
        strncpy(card->expiry_date, item->valuestring, sizeof(card->expiry_date)-1);
    }
    item = cJSON_GetObjectItemCaseSensitive(json_card, "login");
    if (cJSON_IsString(item) && item->valuestring!=NULL){
        strncpy(card->login, item->valuestring, sizeof(card->login)-1);
    }
    item = cJSON_GetObjectItemCaseSensitive(json_card, "phone_number");
    if (cJSON_IsString(item) && item->valuestring!=NULL){
        strncpy(card->phone_number, item->valuestring, sizeof(card->phone_number)-1);
    }
    item = cJSON_GetObjectItemCaseSensitive(json_card, "balance");
    if(cJSON_IsNumber(item)){
        card->balance = item->valuedouble;
    }
    item = cJSON_GetObjectItemCaseSensitive(json_card, "pin_atempts");
    if(cJSON_IsNumber(item)){
        card->pin_atempts = item->valueint;
    }
    item = cJSON_GetObjectItemCaseSensitive(json_card, "is_blocked");
    if (cJSON_IsNumber(item)){
        card->is_blocked = item->valueint;
    }
    item = cJSON_GetObjectItemCaseSensitive(json_card, "created_date");
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
            card->created_date = mktime(&tm);
        } else {
            // Если не удалось распарсить
            card->created_date = time(NULL);
        }
    } else {
    // Если даты нет в JSON
    card->created_date = time(NULL);
}
    return card;
}



int save_db_to_jsoon(Database *db, const char *filename){
    if (db==NULL||filename==NULL){
        return 0;
    }
    cJSON *cards_array = cJSON_CreateArray();
    if (cards_array==NULL){
        return 0;
    }

    int saved_cards = 0;
    for(int i =0;i<db->capacity; i++){
        if(db->cards[i]!=NULL){
            cJSON *card_json= card_to_json(db->cards[i]);
            cJSON_AddItemToArray(cards_array, card_json);
            saved_cards++;
        }
    }

    char *json_string = cJSON_Print(cards_array);
    if (json_string==NULL){
        cJSON_Delete(cards_array);
        return 0;
    }

    FILE *file = fopen(filename, "w");
    if (file==NULL){
        cJSON_Delete(cards_array);
        free(json_string);
        return 0;
    }
    fprintf(file, "%s", json_string);
    fclose(file);

    cJSON_Delete(cards_array);
    free(json_string);

    return 1;
}


int load_Database_from_json(const char *filename, Database *db){
    if(db==NULL || filename==NULL){
        return 0;
    }

    char *json_string = read_file_to_string(filename);

    if (json_string==NULL){
        return 0;
    }

    cJSON *root = cJSON_Parse(json_string);

    if (root==NULL){
        free(json_string);
        return 0;
    }


    if(!cJSON_IsArray(root)){
        cJSON_Delete(root);
        free(json_string);
        return 0;
    }

    int loaded_card = 0;
    cJSON *item;
    cJSON_ArrayForEach(item,root){
        Card *card=json_to_card(item);
        if (card!=NULL){
            add_card_in_db(db, card);
            loaded_card++;
        }
    }
    
    free(json_string);
    cJSON_Delete(root);
    return 1;
}

int write_file(const char *filename, const char *content) {
    if (filename == NULL || content == NULL) {
        return 0;
    }
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return 0;
    }
    
    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, file);
    fclose(file);
    
    return (written == len) ? 1 : 0;
}