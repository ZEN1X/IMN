#!/bin/bash

mkdir build
(
  cd build
  cmake ..
  make
)
./lab1
python3 plot.py
echo '! done'
echo '! plots have been placed in the "plots" subdirectory'
echo '! data for the plots has been placed in the "output" subdirectory'
