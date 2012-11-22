include ../../Makefile.inc

LIB=../libstream.a

SOURCES=A5_1.c
OBJS=$(SOURCES:.c=.o)

TEST=test

all: $(SOURCES) $(LIB)
#test: $(LIB) $(TEST)
#	./$(TEST) && echo "Tests passed! :D"

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) rv $@ $?
	$(RANLIB) $@

#$(TEST): $(OBJS) test.c
#	$(CC) $(CFLAGS) $^ -o $@ -L .. -lfield

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(LIB) $(TEST)
