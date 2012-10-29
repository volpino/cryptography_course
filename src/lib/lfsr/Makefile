include ../../Makefile.inc

OBJ=lfsr.o
LIB=../liblfsr.a
TEST=lfsr_test

all: $(LIB)

test: $(TEST)
	./$(TEST) && echo "Test passed :D"

lfsr.o: lfsr.c
	$(CC) $(CFLAGS) -c $^ -o $@

../liblfsr.a : lfsr.o
	$(AR) $(ARFLAGS) rv $@ $?
	$(RANLIB) $@

lfsr_test: lfsr.c test.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(OBJ) $(TEST) $(LIB)
