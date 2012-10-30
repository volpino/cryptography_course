include ../../Makefile.inc

LIB=../libfield.a

SOURCES=field.c
OBJS=$(SOURCES:.c=.o)

TEST=field_test

all: $(SOURCES) $(LIB)
test: $(LIB) $(TEST)
	./$(TEST) && echo "Tests passed! :D"

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) rv $@ $?
	$(RANLIB) $@

$(TEST): $(OBJS) test.c
	$(CC) $(CFLAGS) $^ -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(LIB) $(TEST)
