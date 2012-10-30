include ../../Makefile.inc

OBJ=bunny24_prng.o
LIB=../libbunny24_prng.a
TEST=

all: $(LIB)

test: $(TEST)
	./$(TEST) && echo "Test passed :D"

bunny24_prng.o: bunny24_prng.c
	$(CC) $(CFLAGS) -c $^ -o $@

../libbunny24_prng.a : bunny24_prng.o
	$(AR) $(ARFLAGS) rv $@ $?
	$(RANLIB) $@

clean:
	$(RM) $(OBJ) $(TEST) $(LIB)
