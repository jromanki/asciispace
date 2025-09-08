CFLAGS = -Wall

app: main.c
	@echo "Compaling..."
	gcc $(CFLAGS) main.c -lncurses -lm -o app 

run: app
	@echo "Running..."
	./app

clean:
	@echo "Removing compiled files"
	rm app