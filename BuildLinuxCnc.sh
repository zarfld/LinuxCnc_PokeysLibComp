#!/bin/bash

# Install necessary packages
cd ..
sudo apt update
sudo apt autoremove
sudo apt install build-essential git devscripts dpkg-dev python
sudo apt install -y automake
sudo apt install -y libmodbus-dev
sudo apt install -y libgpiod
sudo apt install -y libgpiod-dev
sudo apt install -y libgtk2.0-dev
sudo apt install -y libglade2-dev
sudo apt install -y libxmu-dev
sudo apt install -y libglu1-mesa-dev
sudo apt install -y libgl1-mesa-dev
sudo apt install -y libreadline-dev
sudo apt install -y libjansson-dev
sudo apt install -y libboost-python-dev
sudo apt install -y libboost-thread-dev
sudo apt install -y libboost-system-dev
sudo apt install -y debhelper dh-python libudev-dev libxenomai-dev \
   tcl8.6-dev tk8.6-dev libreadline-gplv2-dev asciidoc \
   dvipng graphviz groff imagemagick inkscape python-lxml \
   source-highlight texlive-font-utils texlive-lang-cyrillic \
   texlive-lang-french texlive-lang-german texlive-lang-polish \
   texlive-lang-spanish w3c-linkchecker python-dev python-tk \
   libxmu-dev libgtk2.0-dev intltool libboost-python-dev \
   libusb-1.0-0-dev yapps2 libglu1-mesa-dev libgl1-mesa-dev \
   dblatex docbook-xsl texlive-extra-utils texlive-fonts-recommended \
   texlive-latex-recommended xsltproc gettext autoconf libmodbus-dev \
   asciidoc-dblatex libxaw7-dev bwidget libtk-img tclx python-gtk2

#git config --global http.postBuffer 4294967296
git config --global http.postBuffer 1073741824



# Clone LinuxCNC source code

#git clone --depth 1 https://github.com/linuxcnc/linuxcnc.git linuxcnc-dev
    if [ ! -d "linuxcnc-dev" ]; then
        if ! git clone --depth 1 https://github.com/linuxcnc/linuxcnc.git linuxcnc-dev; then
            echo "Failed to clone repository. Please check your internet connection."
            exit 1
        fi
    fi


cd linuxcnc-dev || exit

# Choose version to compile (2.7 or 2.8 pre1)
if [ -z "$1" ]; then
    git checkout master
else
    git checkout "$1"
fi

# Update source code
git pull

# Configure and compile LinuxCNC
if [ -d "src" ]; then
    cd src
else
    echo "Directory 'src' not found. Please check the repository."
    exit 1
fi

include /usr/include

if [ -f "./autogen.sh" ]; then
  #   ./autogen.sh
  sh autogen.sh
else
    echo "File 'autogen.sh' not found. Please check the repository."
    exit 1
fi

# Choose RealTime option (uspace for PREEMPT_RT)
#echo "Choose RealTime option (uspace for PREEMPT_RT):"
#select realtime_option in uspace preempt_rt xenomai; do
#    break;
#done
realtime_option="uspace"

# Choose build mode (RIP or Debian Packages)
build_mode="RIP"  # Change this to your desired build mode

if [ "$build_mode" = "RIP" ]; then
#    ./configure --with-realtime=$realtime_option --enable-build-documentation
    sh configure --with-realtime=$realtime_option --enable-build-documentation
    make -j$(nproc)
    sudo make setuid
    source ~/linuxcnc-dev/scripts/rip-environment
elif [ "$build_mode" = "Debian Packages" ]; then
    cd ~/linuxcnc-dev/debian
#    ./configure $realtime_option
    sh configure $realtime_option
    #The output of this command will list all the software packages that need to be installed to successfully compile the LinuxCNC software. 
    dpkg-checkbuilddeps

    # Install dependencies based on output of dpkg-checkbuilddeps command
    #sudo apt-get install -y <dependencies>
    sudo apt-get install -y $(dpkg-checkbuilddeps 2>&1 | grep -Po 'Depends: \K.*' | tr -d '<>' | tr ',' '\n' | awk '{print $1}' | tr '\n' ' ')

else
    echo "Invalid build mode choice. Please choose between 'RIP' or 'Debian Packages'."
fi