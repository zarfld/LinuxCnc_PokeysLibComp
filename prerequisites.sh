#!/bin/bash

# Update package lists
echo "Updating package lists..."
apt-get update


if ! dpkg -s linuxcnc-dev >/dev/null 2>&1; then
    # Installing Development Packages needed to build COMP files
    echo "Installing development packages..."
    apt-get install -y linuxcnc-dev
fi

# Install or update required packages
echo "Installing or updating required packages..."

# Check if git is installed
if ! dpkg -s git >/dev/null 2>&1; then
    echo "Installing git..."
    apt-get install -y git
else
    echo "Updating git..."
    apt-get install --only-upgrade -y git
fi

# Check if build-essential is installed
if ! dpkg -s build-essential >/dev/null 2>&1; then
    echo "Installing build-essential..."
    apt-get install -y build-essential
else
    echo "Updating build-essential..."
    apt-get install --only-upgrade -y build-essential
fi

# Check if libusb-1.0-0 is installed
if ! dpkg -s libusb-1.0-0 >/dev/null 2>&1; then
    echo "Installing libusb-1.0-0..."
    apt-get install -y libusb-1.0-0
else
    echo "Updating libusb-1.0-0..."
    apt-get install --only-upgrade -y libusb-1.0-0
fi

# Check if libusb-1.0-0-dev is installed
if ! dpkg -s libusb-1.0-0-dev >/dev/null 2>&1; then
    echo "Installing libusb-1.0-0-dev..."
    apt-get install -y libusb-1.0-0-dev
else
    echo "Updating libusb-1.0-0-dev..."
    apt-get install --only-upgrade -y libusb-1.0-0-dev
fi

# Change directory to parent directory
echo "Changing directory to parent directory..."
cd ..

# Check if pokeyslib folder exists
if [ -d "pokeyslib" ]; then
    # Fetch latest changes
    echo "Fetching latest changes..."
    cd pokeyslib
    git pull
    cd ..
else
    # Clone the repository
    echo "Cloning the repository..."
    git clone https://bitbucket.org/mbosnak/pokeyslib.git
fi

# Change directory to pokeyslib
echo "Changing directory to pokeyslib..."
cd pokeyslib

# Build and install
echo "Building and installing..."
make -f Makefile.noqmake install

# Add udev rules if not already existing
echo "Adding udev rules if not already existing..."
if [ ! -f "/etc/udev/rules.d/90-usb-pokeys.rules" ]; then
    echo 'SUBSYSTEM=="usb", ATTRS{idVendor}=="1dc3", ATTRS{idProduct}=="1001", GROUP="plugdev", MODE="664"' > /etc/udev/rules.d/90-usb-pokeys.rules

    # Reload udev rules
    echo "Reloading udev rules..."
    udevadm control --reload-rules
fi