CC = gcc
CFLAGS = -Wall -g `pkg-config --cflags gtk4` -I../core -I../core/libs
LIBS = `pkg-config --libs gtk4`

# Все нужные исходные файлы
SRCS = app.c \
       ../core/database.c \
       ../core/card.c \
       ../core/file_io.c \
       ../core/operation.c \
       ../core/libs/cJSON.c

OBJS = $(SRCS:.c=.o)

TARGET = myapp

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
