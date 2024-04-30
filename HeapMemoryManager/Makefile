# Compiler
CC = gcc
SRC = hmm.c
OBJ = hmm.o

# Static library name 
STATIC_LIB = lib.a

# Dynamic library name 
DYNAMIC_LIB = lib.so


all: static dynamic

static: $(STATIC_LIB)

dynamic: $(DYNAMIC_LIB)

debug: 
	$(CC) -shared -fPIC -g $(SRC) -o $(DYNAMIC_LIB) 

$(STATIC_LIB): $(OBJ)
	ar rcs $(STATIC_LIB) $(OBJ)

$(DYNAMIC_LIB): $(OBJ)
	$(CC) -shared -o $(DYNAMIC_LIB) $(OBJ)
	

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -fPIC -c $(SRC)

clean:
	rm -f $(OBJ) $(STATIC_LIB) $(DYNAMIC_LIB)

