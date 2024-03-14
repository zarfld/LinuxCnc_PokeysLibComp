#!/bin/bash

# Install necessary packages
cd ..
sudo apt update
sudo apt install build-essential git devscripts dpkg-dev python

git config --global http.postBuffer 2147483648


# Clone LinuxCNC source code

#git clone --depth 1 https://github.com/linuxcnc/linuxcnc.git linuxcnc-dev
    if [ ! -d "linuxcnc-dev" ]; then
        if ! git clone https://github.com/linuxcnc/linuxcnc.git linuxcnc-dev; then
            echo "Failed to clone repository. Please check your internet connection."
            exit 1
        fi
    fi


cd linuxcnc-dev || exit

# Choose version to compile (2.7 or 2.8 pre1)
if [ -z "$1" ]; then
    git checkout master
else
    git checkout "$1"
fi

# Update source code
git pull

# Configure and compile LinuxCNC
if [ -d "src" ]; then
    cd src
else
    echo "Directory 'src' not found. Please check the repository."
    exit 1
fi

if [ -f "./autogen.sh" ]; then
    ./autogen.sh
else
    echo "File 'autogen.sh' not found. Please check the repository."
    exit 1
fi

# Choose RealTime option (uspace for PREEMPT_RT)
#echo "Choose RealTime option (uspace for PREEMPT_RT):"
#select realtime_option in uspace preempt_rt xenomai; do
#    break;
#done
realtime_option="PREEMPT_RT"

# Choose build mode (RIP or Debian Packages)
build_mode="RIP"  # Change this to your desired build mode

if [ "$build_mode" == "RIP" ]; then
    ./configure --with-realtime=$realtime_option --enable-build-documentation
    make -j$(nproc)
    sudo make setuid
    source ~/linuxcnc-dev/scripts/rip-environment
elif [ "$build_mode" == "Debian Packages" ]; then
    cd ~/linuxcnc-dev/debian
    ./configure $realtime_option
    dpkg-checkbuilddeps
    # Install dependencies based on output of dpkg-checkbuilddeps command
else
    echo "Invalid build mode choice. Please choose between 'RIP' or 'Debian Packages'."
fi