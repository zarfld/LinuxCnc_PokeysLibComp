#!/bin/bash

# Get the directory of the current script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Execute the compile.sh script
echo "Executing compile.sh script..."
/bin/bash "${SCRIPT_DIR}/compile.sh"

# Ensure that the folder for example configs is existing
if [ ! -d "/usr/share/doc/linuxcnc/examples/sample-configs/by_interface/pokeys" ]; then
    echo "Creating folder for example configs..."
    mkdir -p /usr/share/doc/linuxcnc/examples/sample-configs/by_interface/pokeys
fi

# Copy the example configs to the correct folder
echo "Copying example configs to the correct folder..."
cp -r "./DM542_XXYZ_mill" /usr/share/doc/linuxcnc/examples/sample-configs/by_interface/pokeys"

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
