include ../../Makefile.inc

LIB=../libbunny24_prng.a

SOURCES=bunny24_prng.c
OBJS=$(SOURCES:.c=.o)

TEST=bunny24_prng_test

all: $(SOURCES) $(LIB)
test: $(LIB) $(TEST)
	./$(TEST) && echo "Tests passed! :D"

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) rv $@ $?
	$(RANLIB) $@

$(TEST): $(OBJS) test.c
	$(CC) $(CFLAGS) $^ -o $@ -L .. -lbunny24 -lfield

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(LIB) $(TEST)
