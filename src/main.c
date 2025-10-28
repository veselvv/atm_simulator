#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "core/card.h"
#include "core/database.h"
#include "core/file_io.h"

int main() {
    srand(time(NULL));
    printf("=== ТЕСТ БАНКОМАТА ===\n");
    // Тест создания базы данных
    Database *db = create_database();
    printf("Загружаем базу данных...\n");
    if (load_Database_from_json("test_database.json", db)){
        printf("База данных успешно загруженна!\n");
    }
    else{
        printf("Не удалось загрузить базу данных\n");
    }
    int running =1;
    int choice; 
    while (running){
        printf("Введите что вы хотите сделать?\n");
        printf("1 - Добавить карту в базу данных\n"
               "2 - Записать текущее состояние бд в файл\n"
               "3 - Добавить новвую карту в бд\n"
               "4 - Найти карту по номеру карты\n"
               "5 - Найти карту по номеру телефона\n"
               "6 - Найти карту по логину\n"
               "7 - Найти карты по имени\n"
               "8 - Удалить карту по номеру карты\n"
               "9 - Обновить баланс карты по номеру\n"
               "10 - Обновить пинкод по номеру карты\n"
               "11 - Обновить статус карты\n"
               "12 - Вывести базу данных\n"
            );
        scanf("%d", &choice);
        switch(choice){
            case 0:{
                running=0;
                break;
            }
            case 1:{
                char pin[5];
                char name[51];
                char login[31];
                char phone_number[12];
                printf("Введите Pin: ");
                scanf("%s",pin);
                printf("\n");
                printf("Введите имя: ");
                scanf("%s", name);
                printf("\n");
                printf("Введите логин: ");
                scanf("%s", login);
                printf("\n");
                printf("Введит свой номер телефона в формате 8XXX-XXX-XX-XX: ");
                scanf("%s",phone_number);
                Card *card = create_card(pin,name,login,phone_number);
                add_card_in_db(db,card);
                break;
            }
            case 2:{
                printf("Сохраняем базу данных...\n");
                if (save_db_to_jsoon(db, "test_database.json")){
                    printf("База данных успешно сохранена!");
                }
                else{
                    printf("Ошибка сохранения данных :(");
                }
                break;
            }
            case 3:{
                add_new_card_in_database(db);
                printf("Карта успешно добавлена\n");
                break;
            }
            case 4:{
                char number[17];
                printf("Введите номер карты: \n");
                scanf("%s", number);
                number[16]='\0';
                Card *card = find_card_by_CardNumber(db, number);
                if(card!=NULL){
                    print_card_info(card);
                }
                break;
            }
            case 5:{
                char phone_number[12];
                printf("Введите номер телефона для поиска: \n");
                scanf("%s",phone_number);
                phone_number[11]='\0';
                Card *card = find_card_by_PhoneNumber(db,phone_number);
                if(card!=NULL){
                    print_card_info(card);
                }
                break;         
            }
            case 6:{
                char login[31];
                printf("Введите логин: \n");
                scanf("%s", login);
                Card *card = find_card_by_Login(db,login);
                if (card!=NULL){
                    print_card_info(card);
                }
                break;
            }
            case 7:{
                char holder_name[51];
                printf("Введите ФИО: \n");
                scanf("%s", holder_name);
                Database *data = find_card_by_name(db, holder_name);
                if (data!=NULL){
                    printDatabase(data);
                    free_Database(data);
                }
                else{
                    printf("Ошибка создания бд\n");
                }
                break;
            }
            case 8:{
                char number[17];
                printf("Введите номер карты для удаления: \n");
                scanf("%s", number);
                remove_card_from_db_by_number(db,number);
                printf("Карта %s удалена!\n", number);
                break;
            }
            case 9:{
                char number[17];
                double balance;
                printf("Введите номер карты: \n");
                scanf("%s", number);
                number[16]='\0';
                printf("Введите новый баланс: \n");
                scanf("%lf", &balance);
                update_card_balance(db,number,balance);
                Card *card = find_card_by_CardNumber(db,number);
                if (card!=NULL){
                    print_card_info(card);
                }
                break;
            }
            case 10:{
                char number[17];
                char pin[5];
                printf("Введите номер карты: \n");
                scanf("%s", number);
                printf("Введите новый PIN код: \n");
                scanf("%s",pin);
                update_card_pin(db,pin,number);
                Card *card = find_card_by_CardNumber(db,number);
                if(card!=NULL){
                    print_card_info(card);
                }
                break;
            }
            case 11:{
                char number[17];
                int status;
                printf("Введите номер карты: \n");
                scanf("%s", number);
                printf("Введите новый статус: 1 - заблокировать, 0 - разблокировать\n");
                scanf("%d", &status);
                update_card_blocked_status(db,status, number);
                Card *card = find_card_by_CardNumber(db,number);
                if (card!=NULL){
                    print_card_info(card);
                }
                break;

            }
            case 12:{
                printDatabase(db);
            }

        }
    }
    save_db_to_jsoon(db,"test_database.json");
    free_Database(db);

    
    return 0;
}