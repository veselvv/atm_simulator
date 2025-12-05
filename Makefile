# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
INCLUDES = -Isrc/core -Isrc/core/libs

# Директории
SRC_DIR = src
CORE_DIR = $(SRC_DIR)/core
LIBS_DIR = $(CORE_DIR)/libs
BUILD_DIR = build
CORE_BUILD_DIR = $(BUILD_DIR)/core

# Исходные файлы
MAIN_SRC = $(SRC_DIR)/main.c
CORE_SRCS = $(CORE_DIR)/card.c $(CORE_DIR)/database.c $(CORE_DIR)/file_io.c \
            $(CORE_DIR)/operation.c $(CORE_DIR)/logging_operation.c \
            $(LIBS_DIR)/cJSON.c

CORE_HEADERS = $(CORE_DIR)/card.h $(CORE_DIR)/database.h $(CORE_DIR)/file_io.h \
               $(CORE_DIR)/operation.h $(CORE_DIR)/logging_operations.h \
               $(LIBS_DIR)/cJSON.h

# Объектные файлы
MAIN_OBJ = $(BUILD_DIR)/main.o
CARD_OBJ = $(CORE_BUILD_DIR)/card.o
DATABASE_OBJ = $(CORE_BUILD_DIR)/database.o
FILE_IO_OBJ = $(CORE_BUILD_DIR)/file_io.o
OPERATION_OBJ = $(CORE_BUILD_DIR)/operation.o
LOGGING_OBJ = $(CORE_BUILD_DIR)/logging_operation.o
CJSON_OBJ = $(CORE_BUILD_DIR)/cJSON.o

CORE_OBJS = $(CARD_OBJ) $(DATABASE_OBJ) $(FILE_IO_OBJ) $(OPERATION_OBJ) $(LOGGING_OBJ) $(CJSON_OBJ)

# Исполняемый файл
TARGET = $(BUILD_DIR)/atm_admin

# Основная цель
all: $(BUILD_DIR) $(CORE_BUILD_DIR) $(TARGET)

# Создание директорий
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(CORE_BUILD_DIR):
	mkdir -p $(CORE_BUILD_DIR)

# Компиляция исполняемого файла
$(TARGET): $(MAIN_OBJ) $(CORE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Компиляция main.o
$(MAIN_OBJ): $(MAIN_SRC) $(CORE_HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Компиляция объектных файлов core
$(CARD_OBJ): $(CORE_DIR)/card.c $(CORE_DIR)/card.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(DATABASE_OBJ): $(CORE_DIR)/database.c $(CORE_DIR)/database.h $(CORE_DIR)/card.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(FILE_IO_OBJ): $(CORE_DIR)/file_io.c $(CORE_DIR)/file_io.h $(CORE_DIR)/database.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OPERATION_OBJ): $(CORE_DIR)/operation.c $(CORE_DIR)/operation.h $(CORE_DIR)/logging_operations.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LOGGING_OBJ): $(CORE_DIR)/logging_operation.c $(CORE_DIR)/logging_operations.h $(CORE_DIR)/file_io.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(CJSON_OBJ): $(LIBS_DIR)/cJSON.c $(LIBS_DIR)/cJSON.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Очистка
clean:
	rm -rf $(BUILD_DIR)/*.o $(CORE_BUILD_DIR)/*.o $(TARGET)

# Перекомпиляция
rebuild: clean all

# Запуск
run: all
	./$(TARGET)

# Тестовая команда для проверки
test: all
	./$(TARGET) --test

.PHONY: all clean rebuild run test
