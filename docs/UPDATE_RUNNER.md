# Updating Self-Hosted GitHub Actions Runner

## Why Update?

The repository now uses `actions/checkout@v6.0.1` which requires **Actions Runner v2.329.0 or newer**.

## Check Current Version

Run the next CI workflow - it will show the current runner version in the logs under "Check Actions Runner version".

Or SSH to your runner machine and run:
```bash
/home/cnc/actions-runner/bin/Runner.Listener --version
```

## Update Methods

### Method 1: Automated Script (Recommended)

1. Copy `update_runner.sh` to your runner machine:
   ```bash
   scp update_runner.sh cnc@<runner-ip>:/home/cnc/
   ```

2. SSH to the runner:
   ```bash
   ssh cnc@<runner-ip>
   ```

3. Run the update script:
   ```bash
   chmod +x update_runner.sh
   ./update_runner.sh
   ```

The script will:
- Show current and latest versions
- Stop the runner service
- Download and extract the latest runner
- Restart the service

### Method 2: Manual Update

1. SSH to your runner machine:
   ```bash
   ssh cnc@<runner-ip>
   ```

2. Stop the runner service:
   ```bash
   cd /home/cnc/actions-runner
   sudo ./svc.sh stop
   ```

3. Download the latest runner (check https://github.com/actions/runner/releases for latest version):
   ```bash
   # For x64 architecture:
   curl -o actions-runner-linux-x64-2.321.0.tar.gz -L \
     https://github.com/actions/runner/releases/download/v2.321.0/actions-runner-linux-x64-2.321.0.tar.gz
   
   # For ARM64 architecture (Raspberry Pi):
   curl -o actions-runner-linux-arm64-2.321.0.tar.gz -L \
     https://github.com/actions/runner/releases/download/v2.321.0/actions-runner-linux-arm64-2.321.0.tar.gz
   ```

4. Extract the new version:
   ```bash
   tar xzf actions-runner-linux-*.tar.gz
   ```

5. Fix permissions:
   ```bash
   sudo chown -R cnc:cnc /home/cnc/actions-runner
   ```

6. Start the runner service:
   ```bash
   sudo ./svc.sh start
   ```

7. Verify the update:
   ```bash
   ./bin/Runner.Listener --version
   sudo ./svc.sh status
   ```

## Fallback Option

If you can't update the runner immediately, you can temporarily use `actions/checkout@v4.3.1` instead:

In `.github/workflows/ci.yml`, change:
```yaml
uses: actions/checkout@v6.0.1
```
to:
```yaml
uses: actions/checkout@v4.3.1
```

## Troubleshooting

### Runner won't start after update
```bash
cd /home/cnc/actions-runner
sudo ./svc.sh uninstall
sudo ./svc.sh install
sudo ./svc.sh start
```

### Permission errors
```bash
sudo chown -R cnc:cnc /home/cnc/actions-runner
sudo chmod -R u+rwx /home/cnc/actions-runner
```

### Check runner logs
```bash
journalctl -u actions.runner.* -f
```

## Post-Update

After updating, trigger a new CI run by pushing a commit to verify everything works correctly.
