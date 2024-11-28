#!/bin/bash

mkdir build
(
  cd build || exit
  cmake ..
  make
)
./lab4
python3 plot.py
echo '! done'
echo '! plots have been placed in the "plots" subdirectory'
echo '! data for the plots has been placed in the "output" subdirectory'
