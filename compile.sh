#!/bin/bash

echo "Compiling pokeys_uspace/pokeys.comp..."
sudo halcompile --install --userspace --extra-link-args="-L/usr/lib -lPoKeys" pokeys_uspace/pokeys.c
echo "pokeys_uspace/pokeys.comp compiled successfully."

echo "Compiling kbd48CNC.comp..."
halcompile --install kbd48CNC.comp
echo "kbd48CNC.comp compiled successfully."

# Check LinuxCNC version

# Prompt the user for confirmation before compilation of pokeys_homecomp.comp
read -p "Do you want to compile of pokeys_homecomp.comp (needs LinuxCNC version 2.9. or higher? (y/n): " answer
if [[ $answer == "y" ]]; then
    echo "Compiling pokeys_homecomp.comp..."
    halcompile --install pokeys_homecomp.comp
    echo "pokeys_homecomp.comp compiled successfully."
else
    echo "Skipping compilation of pokeys_homecomp.comp. LinuxCNC version is not higher than 2.9."
fi

echo "Compiling start_delay.comp..."
halcompile --install start_delay_3000.comp
halcompile --install start_delay_5000.comp
echo "start_delay.comp compiled successfully."

echo "Compiling pokeys_rt.comp..."
halcompile --install pokeys_rt/pokeys_rt.comp
echo "pokeys_rt.comp compiled successfully."

echo "Compiling pokeys_py components..."
python3 setup.py install
echo "pokeys_py components compiled successfully."
