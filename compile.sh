#!/bin/bash

echo "Compiling pokeys.comp..."
halcompile --install pokeys.comp
echo "pokeys.comp compiled successfully."

echo "Compiling kbd48CNC.comp..."
halcompile --install kbd48CNC.comp
echo "kbd48CNC.comp compiled successfully."

echo "Compiling start_delay.comp..."
halcompile --install start_delay_3000.comp
halcompile --install start_delay_5000.comp
echo "start_delay.comp compiled successfully."

