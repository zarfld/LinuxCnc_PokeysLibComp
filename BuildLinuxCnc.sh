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

# Add caching mechanism for dependencies
cache_dir="$HOME/.cache/linuxcnc"
mkdir -p "$cache_dir"
export CCACHE_DIR="$cache_dir"
export CCACHE_COMPRESS=1
export CCACHE_COMPRESSLEVEL=6
export CCACHE_MAXSIZE=5G
export PATH="/usr/lib/ccache:$PATH"

# Optimize the installation process for faster execution
make -j$(nproc) all

# Add logging for installation failures
log_file="$HOME/linuxcnc_build.log"
make -j$(nproc) all 2>&1 | tee "$log_file"
if [ "${PIPESTATUS[0]}" -ne 0 ]; then
    echo "Build failed. Check the log file for details: $log_file"
    exit 1
fi

# Integrate pokeys_rt and pokeys_py components
echo "Integrating pokeys_rt and pokeys_py components..."
cd ../../pokeys_rt
make -f Makefile.noqmake.rt install
cd ../pokeys_py
python3 setup.py install

# Ensure compatibility with LinuxCNC versions 2.8.x and 2.9.x
echo "Ensuring compatibility with LinuxCNC versions 2.8.x and 2.9.x..."
if [ -d "/usr/lib/linuxcnc/modules" ]; then
    echo "LinuxCNC modules directory exists."
else
    echo "LinuxCNC modules directory does not exist. Creating directory..."
    sudo mkdir -p /usr/lib/linuxcnc/modules
fi

if [ -d "/usr/include/linuxcnc" ]; then
    echo "LinuxCNC include directory exists."
else
    echo "LinuxCNC include directory does not exist. Creating directory..."
    sudo mkdir -p /usr/include/linuxcnc
fi

# Copy necessary files to LinuxCNC directories
echo "Copying necessary files to LinuxCNC directories..."
sudo cp ../pokeys_rt/libPoKeysRt.so /usr/lib/linuxcnc/modules
sudo cp ../pokeys_rt/PoKeysLibRt.h /usr/include/linuxcnc
