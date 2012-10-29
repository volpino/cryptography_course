include ../../Makefile.inc

OBJ=field.o
LIB=../libfield.a
TEST=field_test

all: $(LIB)

test: $(TEST)
	./$(TEST) && echo "Test passed :D"

field.o: field.c
	$(CC) $(CFLAGS) -c $^ -o $@

../libfield.a : field.o
	$(AR) $(ARFLAGS) rv $@ $?
	$(RANLIB) $@

field_test: field.c test.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(OBJ) $(TEST) $(LIB)
