include ../../Makefile.inc

LIB=../librsa.a

SOURCES=rsa.c prime.c constants.c
OBJS=$(SOURCES:.c=.o)

TEST=rsa_test

all: $(SOURCES) $(LIB)
test: $(LIB) $(TEST)
	./$(TEST) && echo "Tests passed! :D"

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) rv $@ $?
	$(RANLIB) $@

$(TEST): $(OBJS) test.c
	$(CC) $(CFLAGS) $^ -o $@ -L .. -lrsa -lbunny24_prng -lbunny24 -lfield -lcrypto

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

gen_key: $(OBJS) gen_key.c
	$(CC) $(CFLAGS) $^ -o $@ -L .. -lrsa -lbunny24_prng -lbunny24 -lfield -lcrypto

clean:
	$(RM) $(OBJS) $(LIB) $(TEST)
