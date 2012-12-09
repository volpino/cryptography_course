include Makefile.inc

SERVER_OBJS=server.o common.o
CLIENT_OBJS=client.o common.o
EVE_OBJS=eve.o common.o

LIB_DIRS=field bunny24 bunny24_prng lfsr A5_1 RSA sponge_hash

all: libs server client eve

client: $(CLIENT_OBJS)
	$(CC) $(CLIENT_OBJS) -o $@ -L lib -lcrypto -lrsa -lbunny24_prng -lsponge_hash -lbunny24 -lfield -lstream -llfsr

server: $(SERVER_OBJS)
	$(CC) $(SERVER_OBJS) -o $@ -L lib -lcrypto -lrsa -lbunny24_prng -lsponge_hash -lbunny24 -lfield -lstream -llfsr

eve: $(EVE_OBJS)
	$(CC) $(EVE_OBJS) -o $@ -L lib -lcrypto -lrsa -lbunny24_prng -lsponge_hash -lbunny24 -lfield -lstream -llfsr

libs:
	for d in $(LIB_DIRS); do (cd lib/$$d; $(MAKE) $(MFLAGS) ); done


test:
	for d in $(LIB_DIRS); do (cd lib/$$d; $(MAKE) $(MFLAGS) test ); done

clean:
	$(RM) -f $(CLIENT_OBJS) $(SERVER_OBJS) server client
	for d in $(LIB_DIRS); do (cd lib/$$d; $(MAKE) $(MFLAGS) clean ); done
