#include <stdio.h>
#include <stdlib.h>
#include "core/card.h"
#include "core/database.h"

int main() {
    printf("=== ТЕСТ БАНКОМАТА ===\n");
    
    // Тест создания базы данных
    Database *db = create_database();
    if (db) {
        printf("✅ База данных создана\n");
        printf("   Вместимость: %d\n", db->capacity);
        printf("   Карт в базе: %d\n", db->count);
    } else {
        printf("❌ Ошибка создания базы данных\n");
        return 1;
    }
    
    // Тест создания карты
    Card *card = create_card("1234", "Иван Тестов", "test_user", "89001234567");
    if (card) {
        printf("✅ Тестовая карта создана\n");
        printf("   Номер: %s\n", card->card_number);
        printf("   Держатель: %s\n", card->holder_name);
        printf("   PIN: %s\n", card->pin);
        printf("   Логин: %s\n", card->login);
        
        // Добавляем карту в базу
        add_card_in_db(db, card);
        printf("✅ Карта добавлена в базу\n");
        printf("   Теперь карт в базе: %d\n", db->count);
        
        free(card);
    }
    if (save_db_to_jsoon(db, "test_database.json")) {
        printf("✅ База данных сохранена в test_database.json\n");
    } else {
        printf("❌ Ошибка сохранения базы данных!\n");
    }
    
    
    printf("\n🎯 Тест завершен успешно!\n");
    return 0;
}