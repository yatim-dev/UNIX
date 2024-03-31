#!/bin/bash

if [ ! -d run ]; then
    mkdir run
fi
if [ ! -f run/A ]; then
    python3 create_file.py
fi
make