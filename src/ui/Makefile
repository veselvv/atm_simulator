# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -g
GTK_CFLAGS = `pkg-config --cflags gtk4`
GTK_LIBS = `pkg-config --libs gtk4`
INCLUDES = -I../core -I../core/libs

# Исходные файлы
APP_SRC = app.c
CORE_SRCS = ../core/database.c ../core/card.c ../core/file_io.c ../core/operation.c ../core/logging_operation.c ../core/libs/cJSON.c

# Объектные файлы
APP_OBJ = app.o
CORE_OBJS = ../core/database.o ../core/card.o ../core/file_io.o ../core/operation.o ../core/logging_operation.o ../core/libs/cJSON.o

# Исполняемый файл
TARGET = myapp

# Основная цель
all: $(TARGET)

# Компиляция объектных файлов
../core/database.o: ../core/database.c ../core/database.h ../core/card.h
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(INCLUDES) -c $< -o $@

../core/card.o: ../core/card.c ../core/card.h
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(INCLUDES) -c $< -o $@

../core/file_io.o: ../core/file_io.c ../core/file_io.h ../core/database.h
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(INCLUDES) -c $< -o $@

../core/operation.o: ../core/operation.c ../core/operation.h ../core/logging_operations.h
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(INCLUDES) -c $< -o $@

../core/logging_operation.o: ../core/logging_operation.c ../core/logging_operations.h ../core/file_io.h
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(INCLUDES) -c $< -o $@

../core/libs/cJSON.o: ../core/libs/cJSON.c ../core/libs/cJSON.h
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(INCLUDES) -c $< -o $@

app.o: app.c
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $(INCLUDES) -c $< -o $@

# Линковка
$(TARGET): $(APP_OBJ) $(CORE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(GTK_LIBS)

# Очистка
clean:
	rm -f $(TARGET) $(APP_OBJ) ../core/*.o ../core/libs/*.o

# Перекомпиляция
rebuild: clean all

# Запуск
run: all
	./$(TARGET)

.PHONY: all clean rebuild run
