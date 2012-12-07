#!/bin/sh

make --quiet clean all || exit 0

# Start server
./server serv-cli.fifo cli-serv.fifo whatever 2> /tmp/server_output &

# Let server start before starting client
sleep .5

# Start client
echo "================ CLIENT ================"
./client serv-cli.fifo cli-serv.fifo whatever username
echo

# Start client with different params if needed
# ...

# Kill server
kill %1

# print server output
echo "================ SERVER ================"
cat /tmp/server_output

# clean up
rm -f serv-cli.fifo
rm -f cli-serv.fifo
rm /tmp/server_output

exit 0
