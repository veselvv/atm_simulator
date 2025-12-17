# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -g
GTK_CFLAGS = `pkg-config --cflags gtk4`
GTK_LIBS = `pkg-config --libs gtk4`
INCLUDES = -I../core -I../core/libs

# Директории
BUILD_DIR = ../../build
UI_BUILD_DIR = $(BUILD_DIR)/ui
CORE_BUILD_DIR = $(BUILD_DIR)/core
LIBS_BUILD_DIR = $(BUILD_DIR)/core/libs

# Исходные файлы
APP_SRC = app.c
CORE_SRCS = ../core/database.c ../core/card.c ../core/file_io.c ../core/operation.c ../core/logging_operation.c ../core/libs/cJSON.c

# Объектные файлы (с путями в build)
APP_OBJ = $(UI_BUILD_DIR)/app.o
CORE_OBJS = $(CORE_BUILD_DIR)/database.o $(CORE_BUILD_DIR)/card.o $(CORE_BUILD_DIR)/file_io.o \
            $(CORE_BUILD_DIR)/operation.o $(CORE_BUILD_DIR)/logging_operation.o \
            $(LIBS_BUILD_DIR)/cJSON.o

# Исполняемый файл
TARGET = $(BUILD_DIR)/myapp

# Создание директорий
CREATE_DIRS = $(BUILD_DIR) $(UI_BUILD_DIR) $(CORE_BUILD_DIR) $(LIBS_BUILD_DIR)

# Основная цель
all: $(CREATE_DIRS) $(TARGET)

# Создание директорий
$(BUILD_DIR) $(UI_BUILD_DIR) $(CORE_BUILD_DIR) $(LIBS_BUILD_DIR):
	mkdir -p $@

# Компиляция объектных файлов
$(CORE_BUILD_DIR)/database.o: ../core/database.c ../core/database.h ../core/card.h
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(INCLUDES) -c $< -o $@

$(CORE_BUILD_DIR)/card.o: ../core/card.c ../core/card.h
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(INCLUDES) -c $< -o $@

$(CORE_BUILD_DIR)/file_io.o: ../core/file_io.c ../core/file_io.h ../core/database.h
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(INCLUDES) -c $< -o $@

$(CORE_BUILD_DIR)/operation.o: ../core/operation.c ../core/operation.h ../core/logging_operations.h
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(INCLUDES) -c $< -o $@

$(CORE_BUILD_DIR)/logging_operation.o: ../core/logging_operation.c ../core/logging_operations.h ../core/file_io.h
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBS_BUILD_DIR)/cJSON.o: ../core/libs/cJSON.c ../core/libs/cJSON.h
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(INCLUDES) -c $< -o $@

$(UI_BUILD_DIR)/app.o: app.c
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(INCLUDES) -c $< -o $@

# Линковка
$(TARGET): $(APP_OBJ) $(CORE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(GTK_LIBS)
	@echo "Исполняемый файл создан: $(TARGET)"

# Очистка
clean:
	rm -rf $(BUILD_DIR)/ui $(BUILD_DIR)/core $(BUILD_DIR)/myapp
	@echo "Очистка завершена"

# Глубокая очистка (весь build)
distclean: clean
	rm -rf $(BUILD_DIR)
	@echo "Полная очистка build директории"

# Перекомпиляция
rebuild: clean all

# Запуск
run: all
	@echo "Запуск приложения..."
	cd $(BUILD_DIR) && ./myapp

# Просмотр структуры build
tree:
	@echo "Структура build директории:"
	@tree $(BUILD_DIR) 2>/dev/null || echo "Установите 'tree' для просмотра структуры"

.PHONY: all clean distclean rebuild run tree