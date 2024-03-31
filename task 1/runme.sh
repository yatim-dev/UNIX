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

echo -e $(stat -c "Filename: %n \t Size: %s. Blocks: %b. Bytes in block: %B" run/A) >> result.txt
echo -e $(stat -c "Filename: %n \t Size: %s. Blocks: %b. Bytes in block: %B" run/A.gz) >> result.txt
echo -e $(stat -c "Filename: %n \t Size: %s. Blocks: %b. Bytes in block: %B" run/B) >> result.txt
echo -e $(stat -c "Filename: %n \t Size: %s. Blocks: %b. Bytes in block: %B" run/B.gz) >> result.txt
echo -e $(stat -c "Filename: %n \t Size: %s. Blocks: %b. Bytes in block: %B" run/C) >> result.txt
echo -e $(stat -c "Filename: %n \t Size: %s. Blocks: %b. Bytes in block: %B" run/D) >> result.txt