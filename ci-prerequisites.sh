#!/bin/bash

# Update package lists
echo "Updating package lists..."
apt-get update

retry() {
    local n=1
    local max=3
    local delay=5
    while true; do
        "$@" && break || {
            if [[ $n -lt $max ]]; then
                ((n++))
                echo "Command failed. Attempt $n/$max:"
                sleep $delay;
            else
                echo "The command has failed after $n attempts."
                return 1
            fi
        }
    done
}

if ! dpkg -s linuxcnc-dev >/dev/null 2>&1; then
    # Ask user if linuxcnc-dev should be installed
    read -p "Do you want to install linuxcnc-dev? (y/n): " install_linuxcnc_dev
    if [ "$install_linuxcnc_dev" = "y" ]; then
        # Installing Development Packages needed to build COMP files
        echo "Installing linuxcnc-dev..."
        retry apt-get install -y linuxcnc-dev
    fi
else
    echo "Updating linuxcnc-dev..."
    retry apt-get install --only-upgrade -y linuxcnc-dev
fi

# Install or update required packages
echo "Installing or updating required packages..."

# Check if git is installed
if ! dpkg -s git >/dev/null 2>&1; then
    echo "Installing git..."
    retry apt-get install -y git
else
    echo "Updating git..."
    retry apt-get install --only-upgrade -y git
fi

# Check if build-essential is installed
if ! dpkg -s build-essential >/dev/null 2>&1; then
    echo "Installing build-essential..."
    retry apt-get install -y build-essential
else
    echo "Updating build-essential..."
    retry apt-get install --only-upgrade -y build-essential
fi

# Check if libusb-1.0-0 is installed
if ! dpkg -s libusb-1.0-0 >/dev/null 2>&1; then
    echo "Installing libusb-1.0-0..."
    retry apt-get install -y libusb-1.0-0
else
    echo "Updating libusb-1.0-0..."
    retry apt-get install --only-upgrade -y libusb-1.0-0
fi

# Check if libusb-1.0-0-dev is installed
if ! dpkg -s libusb-1.0-0-dev >/dev/null 2>&1; then
    echo "Installing libusb-1.0-0-dev..."
    retry apt-get install -y libusb-1.0-0-dev
else
    echo "Updating libusb-1.0-0-dev..."
    retry apt-get install --only-upgrade -y libusb-1.0-0-dev
fi

# Change directory to parent directory
echo "Changing directory to parent directory..."
cd ..

# Check if pokeyslib folder exists
if [ -d "pokeyslib" ]; then
    # Fetch latest changes
    echo "Fetching latest changes..."
    cd pokeyslib
    git config pull.ff only
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
