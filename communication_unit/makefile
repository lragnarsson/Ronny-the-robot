CC = gcc
FILES =
FLAGS = -Wpedantic
OUT = communication_module

build: $(FILES)
	$(CC) $(FLAGS) -o $(OUT) $(FILES)

clean:
	-rm -f *.o core

rebuild: clean build
