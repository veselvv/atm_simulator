#ifndef FILE_IO_H
#define FILE_IO_H

#include "database.h"
#include "libs/cJSON.h"

// JSON операции
int save_db_to_jsoon(Database *db, const char *filename);
int load_Database_from_json(const char *filename, Database *db);

// Вспомогательные функции
char *read_file_to_string(const char *filename);
cJSON *card_to_json(const Card *card);
Card *json_to_card(const cJSON *json_card);

#endif