#!/bin/bash



# Prompt the user for confirmation before upgrading packages
read -p "Do you want to update packages lists? (y/n): " answer
if [[ $answer == "y" ]]; then
    # Update package lists
    echo "Updating package lists..."
    apt-get update

    # Prompt the user for confirmation before upgrading packages
    read -p "Do you want to upgrade packages? (y/n): " answer2
    if [[ $answer2 == "y" ]]; then
        # Upgrade packages
        echo "Upgrading packages..."
        apt-get upgrade -y
    else
        echo "Skipping package upgrade."
    fi
else
    echo "Skipping package list udatee."
fi




echo "Updating Git repository..."
git config pull.ff only
git pull


# Prompt the user for confirmation before updating pokeyslib
read -p "Do you want to update pokeyslib also? (y/n): " answer3
if [[ $answer3 == "y" ]]; then
    # Update pokeyslib
    echo "Updating pokeyslib..."
    cd ../pokeyslib
    git config pull.ff only
    git pull
    # Build and install
    echo "Building and installing pokeyslib"
    make -f Makefile.noqmake install
    cd -
else
    echo "Skipping pokeyslib update."
fi

# Get the directory of the current script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

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