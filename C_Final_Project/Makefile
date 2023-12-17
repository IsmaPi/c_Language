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
%.o: %.c
	gcc -c $(flags) $< -o $@

# Install the program (may require admin privileges)
install:
	make
	copy $(exec) C:\Path\To\Installation\Directory\

# Clean up build artifacts
clean:
	del /f /q $(exec)
	del /f /q src\*.o