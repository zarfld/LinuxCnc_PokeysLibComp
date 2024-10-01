#!/bin/bash

echo "Updating Git repository..."
git config pull.ff only
git pull

# Update pokeyslib
echo "Updating pokeyslib..."
cd ../pokeyslib
git config pull.ff only
git pull
# Build and install
echo "Building and installing pokeyslib"
make -f Makefile.noqmake install
cd -

# Add caching mechanism for dependencies
cache_dir="$HOME/.cache/linuxcnc"
mkdir -p "$cache_dir"
export CCACHE_DIR="$cache_dir"
export CCACHE_COMPRESS=1
export CCACHE_COMPRESSLEVEL=6
export CCACHE_MAXSIZE=5G
export PATH="/usr/lib/ccache:$PATH"

# Get the directory of the current script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Execute the compile.sh script
echo "Executing compile.sh script..."
/bin/bash "${SCRIPT_DIR}/compile.sh" 2>&1 | tee compile.log
if [ "${PIPESTATUS[0]}" -ne 0 ]; then
    echo "Compile failed. Check the log file for details: compile.log"
    exit 1
fi

# Ensure that the folder for example configs is existing
if [ ! -d "/usr/share/doc/linuxcnc/examples/sample-configs/by_interface/pokeys" ]; then
    echo "Creating folder for example configs..."
    mkdir -p /usr/share/doc/linuxcnc/examples/sample-configs/by_interface/pokeys
fi

# Copy the example configs to the correct folder
echo "Copying example configs to the correct folder..."
cp -r "./DM542_XXYZ_mill" /usr/share/doc/linuxcnc/examples/sample-configs/by_interface/pokeys

# Add Sentry setup and configuration
echo "Setting up Sentry for telemetry..."
SENTRY_DSN="your_sentry_dsn_here"
echo "SENTRY_DSN=${SENTRY_DSN}" >> /etc/environment

# Add opt-in/opt-out mechanism for telemetry
echo "Adding opt-in/opt-out mechanism for telemetry..."
read -p "Do you want to enable telemetry? (y/n): " enable_telemetry
if [[ $enable_telemetry == "y" ]]; then
    echo "Telemetry enabled."
    echo "TELEMETRY_ENABLED=true" >> /etc/environment
else
    echo "Telemetry disabled."
    echo "TELEMETRY_ENABLED=false" >> /etc/environment
fi
