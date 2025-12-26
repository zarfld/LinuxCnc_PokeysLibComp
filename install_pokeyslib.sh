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
if [ ! -d "external/pokeyslib" ]; then
    echo "Cloning PoKeysLib repository..."
    git clone https://bitbucket.org/mbosnak/pokeyslib.git external/pokeyslib
else
    echo "PoKeysLib repository already cloned."
fi

# Initialize the submodule for pokeyslib only (not recursive to avoid other submodules)
echo "Initializing the pokeyslib submodule..."
git submodule update --init external/pokeyslib

# Überprüfen, ob der Treiber bereits kompiliert ist
cd external/pokeyslib || exit 1
if [ ! -f "pokeyslib/libpokeyslib.so" ]; then
    echo "Compiling and installing the PoKeys driver..."
    sudo make -f Makefile.noqmake install
else
    echo "PoKeys driver already compiled and installed."
fi
