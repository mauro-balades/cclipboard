#!/usr/bin/bash

cd ..

# Rebuild MakeFile
cmake .

# Rebuild library
make
sudo make install

# compile and run tests
cd test

make
make test
