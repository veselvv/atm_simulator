# ============================================================================
# ATM Simulator Makefile
# ============================================================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
INCLUDES = -I./src -I./src/core -I./src/core/libs -I./src/ui

# GTK4 флаги
GTK_CFLAGS = $(shell pkg-config --cflags gtk4 2>/dev/null || echo "")
GTK_LIBS = $(shell pkg-config --libs gtk4 2>/dev/null || echo "")
ifneq ($(GTK_CFLAGS),)
    HAS_GTK = yes
endif

# ============================================================================
# Пути
# ============================================================================

SRC_DIR = src
BUILD_DIR = build

CORE_DIR = $(SRC_DIR)/core
LIBS_DIR = $(CORE_DIR)/libs
UI_DIR = $(SRC_DIR)/ui

# ============================================================================
# Исходные файлы
# ============================================================================

# Core
CORE_SRCS = $(CORE_DIR)/card.c \
            $(CORE_DIR)/database.c \
            $(CORE_DIR)/file_io.c \
            $(CORE_DIR)/operation.c \
            $(CORE_DIR)/logging_operation.c

LIBS_SRCS = $(LIBS_DIR)/cJSON.c
UI_SRCS = $(UI_DIR)/app.c
ADMIN_SRC = $(SRC_DIR)/admin.c

# ============================================================================
# Объектные файлы (все в корень build/)
# ============================================================================

OBJS = $(BUILD_DIR)/card.o \
       $(BUILD_DIR)/database.o \
       $(BUILD_DIR)/file_io.o \
       $(BUILD_DIR)/operation.o \
       $(BUILD_DIR)/logging_operation.o \
       $(BUILD_DIR)/cJSON.o \
       $(BUILD_DIR)/app.o \
       $(BUILD_DIR)/admin.o

# ============================================================================
# Исполняемые файлы (в корень build/)
# ============================================================================

TARGET_APP = $(BUILD_DIR)/myapp
TARGET_ADMIN = $(BUILD_DIR)/admin

# ============================================================================
# Цели
# ============================================================================

.PHONY: all clean distclean rebuild run run-admin help

all: dirs $(TARGET_APP) $(TARGET_ADMIN)
	@echo ""
	@echo "✅ Сборка завершена!"
	@echo "   ➜ GUI:  $(TARGET_APP)"
	@echo "   ➜ Админка: $(TARGET_ADMIN)"
	@echo ""
	@echo "📁 Запуск:"
	@echo "   cd build && ./myapp"
	@echo "   cd build && ./admin"
	@echo ""

# Создание директорий
dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p data logs

# ============================================================================
# Сборка GUI (myapp)
# ============================================================================

ifdef HAS_GTK
$(TARGET_APP): $(BUILD_DIR)/app.o $(BUILD_DIR)/card.o $(BUILD_DIR)/database.o \
               $(BUILD_DIR)/file_io.o $(BUILD_DIR)/operation.o \
               $(BUILD_DIR)/logging_operation.o $(BUILD_DIR)/cJSON.o
	$(CC) $(CFLAGS) -o $@ $^ $(GTK_LIBS)
else
$(TARGET_APP):
	@echo "❌ GTK4 не найден! Установите:"
	@echo "   Ubuntu/Debian: sudo apt install libgtk-4-dev"
	@echo "   Fedora:        sudo dnf install gtk4-devel"
	@echo "   Arch:          sudo pacman -S gtk4"
	@exit 1
endif

# ============================================================================
# Сборка Админки (admin)
# ============================================================================

$(TARGET_ADMIN): $(BUILD_DIR)/admin.o $(BUILD_DIR)/card.o $(BUILD_DIR)/database.o \
                 $(BUILD_DIR)/file_io.o $(BUILD_DIR)/operation.o \
                 $(BUILD_DIR)/logging_operation.o $(BUILD_DIR)/cJSON.o
	$(CC) $(CFLAGS) -o $@ $^

# ============================================================================
# Компиляция .c → .o (все в корень build/)
# ============================================================================

$(BUILD_DIR)/card.o: $(CORE_DIR)/card.c $(CORE_DIR)/card.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/database.o: $(CORE_DIR)/database.c $(CORE_DIR)/database.h $(CORE_DIR)/card.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/file_io.o: $(CORE_DIR)/file_io.c $(CORE_DIR)/file_io.h $(CORE_DIR)/database.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/operation.o: $(CORE_DIR)/operation.c $(CORE_DIR)/operation.h $(CORE_DIR)/logging_operations.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/logging_operation.o: $(CORE_DIR)/logging_operation.c $(CORE_DIR)/logging_operations.h $(CORE_DIR)/file_io.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/cJSON.o: $(LIBS_DIR)/cJSON.c $(LIBS_DIR)/cJSON.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/app.o: $(UI_DIR)/app.c
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/admin.o: $(SRC_DIR)/admin.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# ============================================================================
# Запуск
# ============================================================================

run: $(TARGET_APP)
	cd $(BUILD_DIR) && ./myapp

run-admin: $(TARGET_ADMIN)
	cd $(BUILD_DIR) && ./admin

# ============================================================================
# Очистка
# ============================================================================

clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/myapp $(BUILD_DIR)/admin
	@echo "🧹 Очищено: build/*.o, build/myapp, build/admin"

distclean:
	rm -rf $(BUILD_DIR)
	@echo "🧹 Удалено: $(BUILD_DIR)/"

clean-data:
	rm -rf data logs
	@echo "🧹 Удалено: data/ logs/"

# ============================================================================
# Полная пересборка
# ============================================================================

rebuild: distclean all

# ============================================================================
# Справка
# ============================================================================

help:
	@echo ""
	@echo "📚 ATM Simulator — Makefile"
	@echo ""
	@echo "  make              - собрать всё (myapp + admin)"
	@echo "  make run          - запустить GUI"
	@echo "  make run-admin    - запустить админку"
	@echo "  make clean        - очистить объектные файлы и бинарники"
	@echo "  make distclean    - удалить build/"
	@echo "  make clean-data   - удалить data/ и logs/"
	@echo "  make rebuild      - полная пересборка"
	@echo "  make help         - показать эту справку"
	@echo ""