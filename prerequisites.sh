#!/bin/bash

# Update package lists
sudo apt-get update

# Install required packages
sudo apt-get install -y git build-essential libusb-1.0-0 libusb-1.0-0-dev

# Change directory to parent directory
cd ..

# Clone the repository
git clone https://bitbucket.org/mbosnak/pokeyslib.git

# Change directory to pokeyslib
cd pokeyslib

# Build and install
sudo make -f Makefile.noqmake install

# Installing Development Packages needed to build COMP files
sudo apt-get install linuxcnc-dev