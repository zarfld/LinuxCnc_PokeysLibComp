#!/bin/bash


# Install necessary packages
cd ..
sudo apt update
sudo apt install build-essential git devscripts dpkg-dev python


git config --global http.postBuffer 524288000

# Clone LinuxCNC source code
git clone git://github.com/linuxcnc/linuxcnc.git linuxcnc-dev
cd linuxcnc-dev

# Choose version to compile (2.7 or 2.8 pre1)
if [ -z "$1" ]; then
    git checkout master
else
    git checkout "$1"
fi

# Update source code
git pull

# Configure and compile LinuxCNC
cd src
./autogen.sh

# Choose RealTime option (uspace for PREEMPT_RT)
echo "Choose RealTime option (uspace for PREEMPT_RT):"
select realtime_option in uspace preempt_rt xenomai do
    break;
done

# Compile LinuxCNC
./configure --with-realtime="$realtime_option"
make -j$(nproc)
sudo make setuid

# For Run In Place (RIP) environment
source ~/linuxcnc-dev/scripts/rip-environment