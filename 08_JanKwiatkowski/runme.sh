#!/bin/bash

RED='\033[0;91m'
NC='\033[0m' # No Color
BLUE='\033[0;96m'

function build_and_run() {
  mkdir build
  (
    cd build || exit
    cmake .. -DCMAKE_BUILD_TYPE=Release -G Ninja
    ninja
  )

  rm -r build
  echo -e "${RED}! running the binary, please wait${NC}"
  ./lab8
  # gnuplot plot_lab7.plt
  gnuplot plot_u_D0_D01.plt
  gnuplot plot_x_c_v.plt
  mkdir output
  mkdir plots
  mv -- *.dat output/
  mv -- *.png plots/
  echo -e "${RED}! done${NC}"
  echo -e "${RED}! plots have been placed in the \"plots\" subdirectory${NC}"
  echo -e "${RED}! data for the plots has been placed in the \"output\" subdirectory${NC}"
  echo -e "${BLUE}! you can suffix the command with \"clean\" to delete the build & run artifacts${NC}"
}

function clean() {
  rm ./lab8
  rm -r plots/
  rm -r output/
  echo -e "${RED}! executable, \"plots\" subdirectory and \"output\" subdirectory all removed${NC}"
}

if [ "${1}" = "clean" ]; then
  clean
else
  build_and_run
fi
