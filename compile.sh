#!/bin/bash

echo "Compiling pokeys.comp..."
halcompile --install pokeys.comp
echo "pokeys.comp compiled successfully."

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

echo "Compiling pokeys_digital_io.comp..."
halcompile --install pokeys_userspace/pokeys_digital_io.comp
echo "pokeys_digital_io.comp compiled successfully."

echo "Compiling pokeys_analog_io.comp..."
halcompile --install pokeys_userspace/pokeys_analog_io.comp
echo "pokeys_analog_io.comp compiled successfully."

echo "Compiling pokeys_pev2.comp..."
halcompile --install pokeys_userspace/pokeys_pev2.comp
echo "pokeys_pev2.comp compiled successfully."

echo "Compiling pokeys_counters.comp..."
halcompile --install pokeys_userspace/pokeys_counters.comp
echo "pokeys_counters.comp compiled successfully."
