CC := mpicc
CFLAGS := -03 -std=gnu11
LLIBS := -lcrypt
BIN := crack
SRC := src

HEADER_FILES := $(shell find $(SRC) -name '*.h')
SRC_FILES := $(shell find $(SRC) -name '*.c')
OBJ_FILES := $(patsubst %.c,%.o,$(SRC_FILES))

.PHONY: build

build: $(BINARY)

# Finds all files that is of type .o and afterwards removes binary
clean:
	find . -type f -name "*.o" -delete
	rm -f $(BINARY)

# Link
$(BINARY): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LLIBS)

# Compile
%.o: %.c $(HEADER_FILES)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	find . -type f -name "*.o" -delete
	rm -f $(BINARY)
