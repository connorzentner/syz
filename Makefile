CC = gcc 
CFLAGS = -Wall -Wextra -g

TARGET = syz

SRC = syz.c distro.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean: 
	rm -rf $(TARGET)
