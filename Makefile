CC = gcc 
CFLAGS = -Wall -Wextra -g

TARGET = syz

SRC = main.c distro.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean: 
	rm -rf $(TARGET)
