#!/bin/fish

cmake -B build
cd build
make
./day-3-2 #> ../out.txt
