CC = gcc
CFLAGS = -I.
DEPS = # .h
SOURCE = central_difference.c
OBJ = central_difference.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)
	$(CC) -o $@ $^ 
	@echo "Build complete!"
# Same as:
# bitmap: main.o bitmap.o
	# gcc -o bitmap central_difference.c
	# @echo "Build complete!"

.PHONY: clean

# Delete all .o files on cleanup
clean:
	@rm -f *.o
