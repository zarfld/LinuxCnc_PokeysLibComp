#!/bin/bash

# Get the directory of the current script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Execute the prerequisites.sh script
echo "Executing prerequisites.sh script..."
/bin/bash "${SCRIPT_DIR}/prerequisites.sh"

# Execute the compile.sh script
echo "Executing compile.sh script..."
/bin/bash "${SCRIPT_DIR}/compile.sh"