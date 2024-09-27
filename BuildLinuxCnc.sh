#!/bin/bash

# Install necessary packages
cd ..
sudo apt update

sudo apt autoremove
#git config --global http.postBuffer 4294967296
git config --global http.postBuffer 1073741824

# Clone LinuxCNC source code

dir
cd linuxcnc-dev || exit

# Choose version to compile (2.7 or 2.8 pre1)
if [ -z "$1" ]; then
    git checkout master
else
    git checkout "$1"
fi

# Update source code
git pull --ff-only

# Configure and compile LinuxCNC
if [ -d "src" ]; then
    dir
    cd src
else
    echo "Directory 'src' not found. Please check the repository."
    exit 1
fi

source /usr/include
#include /usr/include

if [ -f "./autogen.sh" ]; then
  #   ./autogen.sh
  sh autogen.sh
else
    echo "File 'autogen.sh' not found. Please check the repository."
    exit 1
fi

# Choose RealTime option (uspace for PREEMPT_RT)
#echo "Choose RealTime option (uspace for PREEMPT_RT):"
#select realtime_option in uspace preempt_rt xenomai; do
#    break;
#done
realtime_option="uspace"

# Choose build mode (RIP or Debian Packages)
build_mode="Debian Packages"  # Change this to your desired build mode

if [ "$build_mode" = "RIP" ]; then
#    ./configure --with-realtime=$realtime_option --enable-build-documentation
    sh configure --with-realtime=$realtime_option --enable-build-documentation
    make -j$(nproc)
    sudo make setuid
    dir
    source ../scripts/rip-environment
elif [ "$build_mode" = "Debian Packages" ]; then
    dir
    cd ../linuxcnc-dev/debian
#    ./configure $realtime_option
    sh configure $realtime_option
    #The output of this command will list all the software packages that need to be installed to successfully compile the LinuxCNC software. 
    dpkg-checkbuilddeps

    # Install dependencies based on output of dpkg-checkbuilddeps command
    #sudo apt-get install -y <dependencies>
    sudo apt-get install -y $(dpkg-checkbuilddeps 2>&1 | grep -Po 'Depends: \K.*' | tr -d '<>' | tr ',' '\n' | awk '{print $1}' | tr '\n' ' ')

else
    echo "Invalid build mode choice. Please choose between 'RIP' or 'Debian Packages'."
fi
