#!/bin/bash

# Function to build and create images for Raspberry Pi 4 Bullseye
build_rpi4_bullseye() {
    echo "Building Raspberry Pi 4 Bullseye image..."
    # Cross-compile for ARM
    sudo apt-get update
    sudo apt-get install -y qemu qemu-user-static debootstrap
    sudo debootstrap --arch=arm64 --foreign bullseye rpi4_bullseye http://deb.debian.org/debian
    sudo cp /usr/bin/qemu-aarch64-static rpi4_bullseye/usr/bin/
    sudo chroot rpi4_bullseye /debootstrap/debootstrap --second-stage
    sudo chroot rpi4_bullseye apt-get update
    sudo chroot rpi4_bullseye apt-get install -y linux-image-arm64 linux-headers-arm64
    sudo chroot rpi4_bullseye apt-get install -y linuxcnc-uspace linuxcnc-dev

    # Ensure the image follows the structure and format of existing LinuxCNC releases
    sudo chroot rpi4_bullseye mkdir -p /usr/share/doc/linuxcnc/examples/sample-configs/by_interface/pokeys
    sudo chroot rpi4_bullseye cp -r /path/to/DM542_XXYZ_mill /usr/share/doc/linuxcnc/examples/sample-configs/by_interface/pokeys

    # Create the image
    sudo dd if=/dev/zero of=rpi4_bullseye.img bs=1M count=2048
    sudo mkfs.ext4 rpi4_bullseye.img
    sudo mount -o loop rpi4_bullseye.img /mnt
    sudo cp -r rpi4_bullseye/* /mnt/
    sudo umount /mnt

    # Test and validate the generated image
    sudo qemu-system-aarch64 -M raspi3 -kernel rpi4_bullseye/boot/vmlinuz-*-arm64 -initrd rpi4_bullseye/boot/initrd.img-*-arm64 -append "root=/dev/sda2" -hda rpi4_bullseye.img -nographic
}

# Function to build and create images for Raspberry Pi 4 Bookworm
build_rpi4_bookworm() {
    echo "Building Raspberry Pi 4 Bookworm image..."
    # Cross-compile for ARM
    sudo apt-get update
    sudo apt-get install -y qemu qemu-user-static debootstrap
    sudo debootstrap --arch=arm64 --foreign bookworm rpi4_bookworm http://deb.debian.org/debian
    sudo cp /usr/bin/qemu-aarch64-static rpi4_bookworm/usr/bin/
    sudo chroot rpi4_bookworm /debootstrap/debootstrap --second-stage
    sudo chroot rpi4_bookworm apt-get update
    sudo chroot rpi4_bookworm apt-get install -y linux-image-arm64 linux-headers-arm64
    sudo chroot rpi4_bookworm apt-get install -y linuxcnc-uspace linuxcnc-dev

    # Ensure the image follows the structure and format of existing LinuxCNC releases
    sudo chroot rpi4_bookworm mkdir -p /usr/share/doc/linuxcnc/examples/sample-configs/by_interface/pokeys
    sudo chroot rpi4_bookworm cp -r /path/to/DM542_XXYZ_mill /usr/share/doc/linuxcnc/examples/sample-configs/by_interface/pokeys

    # Create the image
    sudo dd if=/dev/zero of=rpi4_bookworm.img bs=1M count=2048
    sudo mkfs.ext4 rpi4_bookworm.img
    sudo mount -o loop rpi4_bookworm.img /mnt
    sudo cp -r rpi4_bookworm/* /mnt/
    sudo umount /mnt

    # Test and validate the generated image
    sudo qemu-system-aarch64 -M raspi3 -kernel rpi4_bookworm/boot/vmlinuz-*-arm64 -initrd rpi4_bookworm/boot/initrd.img-*-arm64 -append "root=/dev/sda2" -hda rpi4_bookworm.img -nographic
}

# Function to build and create images for amd64 Hybrid
build_amd64_hybrid() {
    echo "Building amd64 Hybrid image..."
    # Natively compile for amd64
    sudo apt-get update
    sudo apt-get install -y debootstrap
    sudo debootstrap --arch=amd64 bullseye amd64_hybrid http://deb.debian.org/debian
    sudo chroot amd64_hybrid apt-get update
    sudo chroot amd64_hybrid apt-get install -y linux-image-amd64 linux-headers-amd64
    sudo chroot amd64_hybrid apt-get install -y linuxcnc-uspace linuxcnc-dev

    # Ensure the image follows the structure and format of existing LinuxCNC releases
    sudo chroot amd64_hybrid mkdir -p /usr/share/doc/linuxcnc/examples/sample-configs/by_interface/pokeys
    sudo chroot amd64_hybrid cp -r /path/to/DM542_XXYZ_mill /usr/share/doc/linuxcnc/examples/sample-configs/by_interface/pokeys

    # Create the image
    sudo dd if=/dev/zero of=amd64_hybrid.img bs=1M count=2048
    sudo mkfs.ext4 amd64_hybrid.img
    sudo mount -o loop amd64_hybrid.img /mnt
    sudo cp -r amd64_hybrid/* /mnt/
    sudo umount /mnt

    # Test and validate the generated image
    sudo qemu-system-x86_64 -kernel amd64_hybrid/boot/vmlinuz-*-amd64 -initrd amd64_hybrid/boot/initrd.img-*-amd64 -append "root=/dev/sda2" -hda amd64_hybrid.img -nographic
}

# Function to build and create images for Raspbian Bullseye with LinuxCNC 2.8-latest
build_raspbian_bullseye_linuxcnc_2_8_latest() {
    echo "Building Raspbian Bullseye with LinuxCNC 2.8-latest image..."
    # Cross-compile for ARM
    sudo apt-get update
    sudo apt-get install -y qemu qemu-user-static debootstrap
    sudo debootstrap --arch=arm64 --foreign bullseye raspbian_bullseye_linuxcnc_2_8_latest http://deb.debian.org/debian
    sudo cp /usr/bin/qemu-aarch64-static raspbian_bullseye_linuxcnc_2_8_latest/usr/bin/
    sudo chroot raspbian_bullseye_linuxcnc_2_8_latest /debootstrap/debootstrap --second-stage
    sudo chroot raspbian_bullseye_linuxcnc_2_8_latest apt-get update
    sudo chroot raspbian_bullseye_linuxcnc_2_8_latest apt-get install -y linux-image-arm64 linux-headers-arm64
    sudo chroot raspbian_bullseye_linuxcnc_2_8_latest apt-get install -y linuxcnc-uspace=2.8-latest linuxcnc-dev=2.8-latest

    # Ensure the image follows the structure and format of existing LinuxCNC releases
    sudo chroot raspbian_bullseye_linuxcnc_2_8_latest mkdir -p /usr/share/doc/linuxcnc/examples/sample-configs/by_interface/pokeys
    sudo chroot raspbian_bullseye_linuxcnc_2_8_latest cp -r /path/to/DM542_XXYZ_mill /usr/share/doc/linuxcnc/examples/sample-configs/by_interface/pokeys

    # Create the image
    sudo dd if=/dev/zero of=raspbian_bullseye_linuxcnc_2_8_latest.img bs=1M count=2048
    sudo mkfs.ext4 raspbian_bullseye_linuxcnc_2_8_latest.img
    sudo mount -o loop raspbian_bullseye_linuxcnc_2_8_latest.img /mnt
    sudo cp -r raspbian_bullseye_linuxcnc_2_8_latest/* /mnt/
    sudo umount /mnt

    # Test and validate the generated image
    sudo qemu-system-aarch64 -M raspi3 -kernel raspbian_bullseye_linuxcnc_2_8_latest/boot/vmlinuz-*-arm64 -initrd raspbian_bullseye_linuxcnc_2_8_latest/boot/initrd.img-*-arm64 -append "root=/dev/sda2" -hda raspbian_bullseye_linuxcnc_2_8_latest.img -nographic
}

# Function to upload the generated images to the repository's releases or a suitable cloud storage platform
upload_images() {
    echo "Uploading generated images..."
    # Use GitHub Actions to upload the generated images
}

# Main script execution
build_rpi4_bullseye
build_rpi4_bookworm
build_amd64_hybrid
build_raspbian_bullseye_linuxcnc_2_8_latest
upload_images
