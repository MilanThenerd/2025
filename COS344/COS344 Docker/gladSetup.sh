#!/bin/bash

echo "Starting glad setup"
cd /usr/local/lib/
git clone https://github.com/glfw/glfw.git
cd glfw
ls
cmake .
make
make install

echo "Finished glad setup"
echo "test" >> text.txt
