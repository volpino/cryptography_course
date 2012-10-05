#!/bin/sh

make -k &&

# Start server
./server serv-cli.fifo cli-serv.fifo whatever&

# Let server start before starting client
sleep .5

# Start client
./client serv-cli.fifo cli-serv.fifo whatever username

# Start client with different params if needed
# ...

# Kill server
kill %1

# clean up
unlink serv-cli.fifo 2>/dev/null 1>/dev/null
unlink cli-serv.fifo 2>/dev/null 1>/dev/null

exit 0
