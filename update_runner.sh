#!/bin/bash

# GitHub Actions Runner Update Script
# This script updates the self-hosted runner to the latest version

set -e

echo "=== GitHub Actions Runner Update Script ==="
echo ""

# Configuration
RUNNER_DIR="/home/cnc/actions-runner"
RUNNER_USER="cnc"

# Check current version
echo "Current runner version:"
if [ -f "$RUNNER_DIR/bin/Runner.Listener" ]; then
    CURRENT_VERSION=$($RUNNER_DIR/bin/Runner.Listener --version 2>/dev/null || echo "unknown")
    echo "  $CURRENT_VERSION"
else
    echo "  Runner not found at $RUNNER_DIR"
    exit 1
fi

echo ""
echo "Required minimum version for checkout@v6: v2.329.0"
echo ""

# Get latest runner version
echo "Fetching latest runner version from GitHub..."
LATEST_VERSION=$(curl -s https://api.github.com/repos/actions/runner/releases/latest | grep '"tag_name":' | sed -E 's/.*"v([^"]+)".*/\1/')
echo "Latest available version: v$LATEST_VERSION"
echo ""

# Ask for confirmation
read -p "Do you want to update the runner? (y/n): " -n 1 -r
echo ""
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "Update cancelled."
    exit 0
fi

echo ""
echo "=== Starting Runner Update ==="
echo ""

# Stop the runner service
echo "Stopping runner service..."
cd "$RUNNER_DIR"
sudo ./svc.sh stop || true

# Download the latest runner
echo "Downloading runner v$LATEST_VERSION..."
RUNNER_ARCH="x64"  # Change to arm64 if running on ARM
RUNNER_FILE="actions-runner-linux-${RUNNER_ARCH}-${LATEST_VERSION}.tar.gz"
DOWNLOAD_URL="https://github.com/actions/runner/releases/download/v${LATEST_VERSION}/${RUNNER_FILE}"

curl -o "${RUNNER_FILE}" -L "${DOWNLOAD_URL}"

# Extract the new version
echo "Extracting runner..."
tar xzf "${RUNNER_FILE}"

# Clean up
rm "${RUNNER_FILE}"

# Fix permissions
echo "Fixing permissions..."
sudo chown -R ${RUNNER_USER}:${RUNNER_USER} "$RUNNER_DIR"

# Start the runner service
echo "Starting runner service..."
sudo ./svc.sh start

echo ""
echo "=== Update Complete ==="
echo ""
echo "New runner version:"
$RUNNER_DIR/bin/Runner.Listener --version

echo ""
echo "Runner service status:"
sudo ./svc.sh status

echo ""
echo "Update completed successfully!"
