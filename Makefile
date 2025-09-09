CFLAGS = -Wall -Iinclude
SRC = $(wildcard src/*.c)

app: $(SRC)
	@echo "Compiling..."
	gcc $(CFLAGS) $(SRC) -lncurses -lm -o app

run: app
	@echo "Running..."
	./app

clean:
	@echo "Removing compiled files"
	rm -f app
