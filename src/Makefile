include Makefile.inc

SERVER_OBJS=server.o common.o
CLIENT_OBJS=client.o common.o

LIB_DIRS=field bunny24 lfsr

all: libs server client

client: $(CLIENT_OBJS)
	$(CC) $(CLIENT_OBJS) -o $@

server: $(SERVER_OBJS)
	$(CC) $(SERVER_OBJS) -o $@

libs:
	for d in $(LIB_DIRS); do (cd lib/$$d; $(MAKE) $(MFLAGS) ); done


test:
	for d in $(LIB_DIRS); do (cd lib/$$d; $(MAKE) $(MFLAGS) test ); done

clean:
	$(RM) -f $(CLIENT_OBJS) $(SERVER_OBJS) server client
	for d in $(LIB_DIRS); do (cd lib/$$d; $(MAKE) $(MFLAGS) clean ); done
