#!/bin/bash

# Check if the user has rights to see the PoKeys57CNC_ID secret
if [ -n "${POKEYS57CNC_ID}" ]; then
  export POKEYS57CNC_ID="${POKEYS57CNC_ID}"
else
  export POKEYS57CNC_ID="0"
fi

# Check if the user has rights to see the PoKeys57E_ID secret
if [ -n "${POKEYS57E_ID}" ]; then
  export POKEYS57E_ID="${POKEYS57E_ID}"
else
  export POKEYS57E_ID="0"
fi
