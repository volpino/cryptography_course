include ../../Makefile.inc

LIB=../libbunny24.a

SOURCES=constants.c key_schedule.c mixing_layer.c cipher.c
OBJS=$(SOURCES:.c=.o)

TEST=bunny24_test

all: $(SOURCES) $(LIB)
test: $(LIB) $(TEST)
	./$(TEST) && echo "Tests passed! :D"

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) rv $@ $?
	$(RANLIB) $@

$(TEST): $(OBJS) test.c
	$(CC) $(CFLAGS) $^ -o $@ -L .. -lfield

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(LIB) $(TEST)
