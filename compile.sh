#!/bin/bash

echo "Compiling pokeys.comp..."
halcompile --install pokeys.comp
echo "pokeys.comp compiled successfully."

echo "Compiling kbd48CNC.comp..."
halcompile --install kbd48CNC.comp
echo "kbd48CNC.comp compiled successfully."

echo "Compiling pokeys_homecomp.comp..."
halcompile --install pokeys_homecomp.comp
echo "pokeys_homecomp.comp compiled successfully."

echo "Compiling pokeys_rt.comp..."
halcompile --install pokeys_rt.comp
echo "pokeys_rt.comp compiled successfully."