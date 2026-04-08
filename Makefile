CC = gcc 
CFLAGS = -Wall -Wextra -g

TARGET = syz

PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

SRC = main.c distro.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

install: $(TARGET)
	install -D -m 775 $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)

uninstall: 
	rm -f $(DESTDIR)$(BINDIR)/$(TARGET)

clean:
	rm -rf $(TARGET)

.PHONY: install uninstall clean
