include ../../Makefile.inc

LIB=../libbunny24.a
TEST=bunny24_test

SOURCES=constants.c key_schedule.c mixing_layer.c cipher.c
OBJS=$(SOURCES:.c=.o)
DEPS=../libfield.a $(OBJS)

all: $(SOURCES) $(LIB)
test: $(TEST)
	./$(TEST) && echo "Tests passed! :D"

../libbunny24.a: $(DEPS)
	$(AR) $(ARFLAGS) rv $@ $?
	$(RANLIB) $@

bunny24_test: ../bunny24.o test.c
	$(CC) $(CFLAGS) $^ -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(LIB) $(TEST)
