CC = gcc 
CFLAGS = -Wall -Wextra -g

TARGET = syz

SRC = syz.c distro.c

$(TARGET): $(SRC) colors.h
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean: 
	rm $(TARGET)
