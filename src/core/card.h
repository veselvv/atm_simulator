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
    char expiry_date[9];
    char cvv[4];
    char phone_number[12];
    char login[31];
    char gender[2];
    time_t created_date;

}Card;

Card *create_card(const char *pin, const char *name,  const char *login, const char *phone_number, const char *gender);
void generate_random_cvv(char *cvv_buffer);
void generate_expiry_date(char *expiry_buffer);
void create_card_number(char *card_number_buffer);
void print_card_info(Card *card);

#endif
