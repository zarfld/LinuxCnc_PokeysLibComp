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

#if ! dpkg -s linuxcnc-dev >/dev/null 2>&1; then
#    # Ask user if linuxcnc-dev should be installed
#    read -p "Do you want to install linuxcnc-dev? (y/n): " install_linuxcnc_dev
#    if [ "$install_linuxcnc_dev" = "y" ]; then
#        # Installing Development Packages needed to build COMP files
#        echo "Installing linuxcnc-dev..."
#        retry sudo apt-get install -y linuxcnc-dev
#    fi
#else
#    echo "Updating linuxcnc-dev..."
#    retry sudo apt-get install --only-upgrade -y linuxcnc-dev
#fi

# Determine if we're running a PREEMPT_RT kernel
if uname -v | grep -q "PREEMPT_RT"; then
    PKG_NAME="linuxcnc-uspace-dev"
else
    PKG_NAME="linuxcnc-dev"
fi

echo "Detected package to install/update: $PKG_NAME"

# Check if the package is already installed
if ! dpkg -s "$PKG_NAME" >/dev/null 2>&1; then
    # Ask user if the package should be installed
    read -rp "Do you want to install $PKG_NAME? (y/n): " install_choice
    if [[ "$install_choice" =~ ^[Yy]$ ]]; then
        echo "Installing $PKG_NAME..."
        retry sudo apt-get install -y "$PKG_NAME"
    else
        echo "Skipping installation of $PKG_NAME."
    fi
else
    echo "Updating $PKG_NAME..."
    retry sudo apt-get install --only-upgrade -y "$PKG_NAME"
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
dir
#echo "Changing directory to parent directory..."
#cd ..
#echo "Changed directory to parent directory..."
#dir

# Ensure external/pokeyslib submodule is initialized and up to date
POKEYSLIB_DIR="external/pokeyslib"

if [ -d "$POKEYSLIB_DIR" ]; then
    echo "Updating submodule $POKEYSLIB_DIR..."
    git submodule update --init --recursive "$POKEYSLIB_DIR"
    cd "$POKEYSLIB_DIR" || exit 1
    git config pull.ff only
    git pull
    cd - || exit 1
else
    echo "Submodule directory missing, initializing..."
    git submodule update --init --recursive "$POKEYSLIB_DIR"
fi

# Change directory to pokeyslib
echo "Changing directory to $POKEYSLIB_DIR..."
cd "$POKEYSLIB_DIR" || exit 1

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
