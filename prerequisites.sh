#!/bin/bash

# Update package lists
echo "Updating package lists..."
sudo apt-get update

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

# Check runner availability
echo "Checking runner availability..."
if [ -z "$RUNNER_NAME" ]; then
    echo "No runner available. Exiting..."
    exit 1
fi
echo "Runner $RUNNER_NAME is available."

# Log runner details
echo "Logging runner details..."
echo "Runner Name: $RUNNER_NAME"
echo "Runner OS: $RUNNER_OS"
echo "Runner Architecture: $RUNNER_ARCH"
echo "Runner Name: $RUNNER_NAME" >> runner_details.log
echo "Runner OS: $RUNNER_OS" >> runner_details.log
echo "Runner Architecture: $RUNNER_ARCH" >> runner_details.log

if ! dpkg -s linuxcnc-dev >/dev/null 2>&1; then
    # Ask user if linuxcnc-dev should be installed
    read -p "Do you want to install linuxcnc-dev? (y/n): " install_linuxcnc_dev
    if [ "$install_linuxcnc_dev" = "y" ]; then
        # Installing Development Packages needed to build COMP files
        echo "Installing linuxcnc-dev..."
        retry sudo apt-get install -y linuxcnc-dev
    fi
else
    echo "Updating linuxcnc-dev..."
    retry sudo apt-get install --only-upgrade -y linuxcnc-dev
fi

# Install or update required packages
echo "Installing or updating required packages..."

# Check if git is installed
if ! dpkg -s git >/dev/null 2>&1; then
    echo "Installing git..."
    retry sudo apt-get install -y git
else
    echo "Updating git..."
    retry sudo apt-get install --only-upgrade -y git
fi

# Check if build-essential is installed
if ! dpkg -s build-essential >/dev/null 2>&1; then
    echo "Installing build-essential..."
    retry sudo apt-get install -y build-essential
else
    echo "Updating build-essential..."
    retry sudo apt-get install --only-upgrade -y build-essential
fi

# Check if libusb-1.0-0 is installed
if ! dpkg -s libusb-1.0-0 >/dev/null 2>&1; then
    echo "Installing libusb-1.0-0..."
    retry sudo apt-get install -y libusb-1.0-0
else
    echo "Updating libusb-1.0-0..."
    retry sudo apt-get install --only-upgrade -y libusb-1.0-0
fi

# Check if libusb-1.0-0-dev is installed
if ! dpkg -s libusb-1.0-0-dev >/dev/null 2>&1; then
    echo "Installing libusb-1.0-0-dev..."
    retry sudo apt-get install -y libusb-1.0-0-dev
else
    echo "Updating libusb-1.0-0-dev..."
    retry sudo apt-get install --only-upgrade -y libusb-1.0-0-dev
fi

# Add caching mechanism for dependencies
cache_dir="$HOME/.cache/linuxcnc"
mkdir -p "$cache_dir"
export CCACHE_DIR="$cache_dir"
export CCACHE_COMPRESS=1
export CCACHE_COMPRESSLEVEL=6
export CCACHE_MAXSIZE=5G
export PATH="/usr/lib/ccache:$PATH"

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
make -j$(nproc) -f Makefile.noqmake install 2>&1 | tee build.log
if [ "${PIPESTATUS[0]}" -ne 0 ]; then
    echo "Build failed. Check the log file for details: build.log"
    exit 1
fi

# Add udev rules if not already existing
echo "Adding udev rules if not already existing..."
if [ ! -f "/etc/udev/rules.d/90-usb-pokeys.rules" ]; then
    echo 'SUBSYSTEM=="usb", ATTRS{idVendor}=="1dc3", ATTRS{idProduct}=="1001", GROUP="plugdev", MODE="664"' > /etc/udev/rules.d/90-usb-pokeys.rules

    # Reload udev rules
    echo "Reloading udev rules..."
    udevadm control --reload-rules
fi

# Verify QEMU installation
echo "Verifying QEMU installation..."
qemu_version=$(qemu-system-x86_64 --version)
echo "QEMU version: $qemu_version"
