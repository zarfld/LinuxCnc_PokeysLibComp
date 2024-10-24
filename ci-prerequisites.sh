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

# Install eatmydata, git, python3, devscripts, and build-essential
echo "Installing dependencies..."
retry sudo apt-get install -y eatmydata git python3 devscripts build-essential

# Add caching mechanism for dependencies
cache_dir="$HOME/.cache/linuxcnc"
mkdir -p "$cache_dir"
export CCACHE_DIR="$cache_dir"
export CCACHE_COMPRESS=1
export CCACHE_COMPRESSLEVEL=6
export CCACHE_MAXSIZE=5G
export PATH="/usr/lib/ccache:$PATH"

# Optimize the installation process for faster execution
#make -j$(nproc) all

# Add logging for installation failures
log_file="$HOME/linuxcnc_build.log"
make -j$(nproc) all 2>&1 | tee "$log_file"
if [ "${PIPESTATUS[0]}" -ne 0 ]; then
    echo "Build failed. Check the log file for details: $log_file"
    exit 1
fi
