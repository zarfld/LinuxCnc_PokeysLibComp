#!/bin/bash

# Überprüfen, ob die Abhängigkeiten installiert sind
if ! dpkg -s libusb-1.0-0 libusb-1.0-0-dev >/dev/null 2>&1; then
    echo "Installing dependencies..."
    sudo apt-get update
    sudo apt-get install -y git build-essential libusb-1.0-0 libusb-1.0-0-dev
else
    echo "Dependencies are already installed."
fi

# Überprüfen, ob das Repository geklont wurde
if [ ! -d "pokeyslib" ]; then
    echo "Cloning PoKeysLib repository..."
    git clone https://bitbucket.org/mbosnak/pokeyslib.git
else
    echo "PoKeysLib repository already cloned."
fi

# Überprüfen, ob der Treiber bereits kompiliert ist
if [ ! -f "pokeyslib/libpokeyslib.so" ]; then
    echo "Compiling and installing the PoKeys driver..."
    cd pokeyslib
    sudo make -f Makefile.noqmake install
else
    echo "PoKeys driver already compiled and installed."
fi
