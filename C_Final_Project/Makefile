# Executable name
exec = mathlang.out

# Source and object files
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g

# Main target
$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

# Rule for object files
%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

# Install the program (may require sudo)
install:
	make
	cp ./$(exec) /usr/local/bin/mathlang

# Clean up build artifacts
clean:
	-rm -f $(exec)
	-rm -f *.o
	-rm -f src/*.o