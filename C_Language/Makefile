exec = mathematics.out
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g


$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

install:
	make
	copy mathematics.out C:\Users\jorge\Desktop\


clean:
	-del *.out
	-del *.o
	-del src\*.o
