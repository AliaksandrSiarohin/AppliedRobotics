#! /bin/bash

make -f Makelib clean
make -f Makelib1 clean
make -f Makelib2 clean
make -f Makelib all
make -f Makelib1 all
make -f Makelib2 all

