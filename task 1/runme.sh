#!/bin/bash

if [ -d run ]; then
    make clean
fi

mkdir run
python3 create_file.py
make

./run/task1 run/A run/B
gzip -k run/A
gzip -k run/B
gzip -cd run/B.gz | ./run/task1 run/C
./run/task1 run/A run/D -b 100

echo -e $(stat -c "Filename: %n \t Size: %s. Blocks: %b. Bytes in block: %B" run/A)
echo -e $(stat -c "Filename: %n \t Size: %s. Blocks: %b. Bytes in block: %B" run/A.gz)
echo -e $(stat -c "Filename: %n \t Size: %s. Blocks: %b. Bytes in block: %B" run/B)
echo -e $(stat -c "Filename: %n \t Size: %s. Blocks: %b. Bytes in block: %B" run/B.gz)
echo -e $(stat -c "Filename: %n \t Size: %s. Blocks: %b. Bytes in block: %B" run/C)
echo -e $(stat -c "Filename: %n \t Size: %s. Blocks: %b. Bytes in block: %B" run/D)