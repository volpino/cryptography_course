# this is not a script, use it to copy commands from here

# mkfifo /tmp/ce.fifo /tmp/ec.fifo /tmp/se.fifo /tmp/es.fifo
# useless, server e eve create FIFOs where they listen

./server /tmp/se.fifo /tmp/es.fifo
./eve /tmp/se.fifo /tmp/es.fifo /tmp/ec.fifo /tmp/ce.fifo
./client /tmp/ec.fifo /tmp/ce.fifo f Pippo
rm /tmp/ce.fifo /tmp/ec.fifo /tmp/se.fifo /tmp/es.fifo
