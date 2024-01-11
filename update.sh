#!/bin/bash

# Update package lists
echo "Updating package lists..."
apt-get update

# Upgrade packages
echo "Upgrading packages..."
apt-get upgrade -y

echo "updating Git repository..."
git pull

# Get the directory of the current script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Execute the compile.sh script
echo "Executing compile.sh script..."
/bin/bash "${SCRIPT_DIR}/compile.sh"