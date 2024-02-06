#!/bin/bash

# Get the directory of the current script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Execute the prerequisites.sh script
echo "Executing prerequisites.sh script..."
/bin/bash "${SCRIPT_DIR}/prerequisites.sh"

# Execute the compile.sh script
echo "Executing compile.sh script..."
/bin/bash "${SCRIPT_DIR}/compile.sh"

# Ensure that the folder for example configs is existing
if [ ! -d "/usr/share/doc/linuxcnc/examples/sample-configs/by_interface/pokeys" ]; then
    mkdir -p /usr/share/doc/linuxcnc/examples/sample-configs/by_interface/pokeys
fi

# Copy the example configs to the correct folder
echo "Copying example configs to the correct folder..."
cp -r "./DM542_XXYZ_mill" /usr/share/doc/linuxcnc/examples/sample-configs/by_interface/pokeys