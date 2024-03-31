#!/bin/bash

if [ ! -d build ]; then
    mkdir run
fi
if [ ! -f build/A ]; then
    python3 create_file.py
fi
make