#!/bin/bash

# Update package lists
echo "Updating package lists..."
apt-get update

# Install required packages
echo "Installing required packages..."
apt-get install -y libreadline-dev python-dev-is-python3 python3-gi python3-lxml
