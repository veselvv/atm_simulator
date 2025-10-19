#ifndef CARD_H
#define CARD_H
#include <time.h> 
typedef struct Card{
    //Идентификация пользователя
    char card_number[17];
    char holder_name[51];

    //Безопасность
    char pin[5];
    int is_blocked;
    int pin_atempts;

    //Баланс
    double balance;

    //Доп информация
    char expiry_date[6];
    char cvv[4];
    char phone_number[12];
    char login[31];
    time_t created_date;

}Card;

Card *create_card(const char *pin, const char *name,  const char *login, const char *phone_number);
void generate_random_cvv(char *cvv_buffer);
void create_card_number(char *card_number_buffer);

#endif
