CC = gcc
CFLAGS = -Iheaders -Wall -Wextra -std=c11

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = zagmate.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q src\*.o
	if exist $(TARGET) del /Q $(TARGET)