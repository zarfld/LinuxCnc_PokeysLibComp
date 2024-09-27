#!/bin/bash

# Update package lists
echo "Updating package lists..."
apt-get update

# Install required packages for CI pipeline
echo "Installing required packages for CI pipeline..."
apt-get install -y cmake python3 linuxcnc-uspace linuxcnc-dev libmodbus-dev libgpiod-dev libgtk2.0-dev libglade2-dev libxmu-dev libglu1-mesa-dev libgl1-mesa-dev libreadline-dev libjansson-dev libboost-python-dev libboost-thread-dev libboost-system-dev python3-lxml python3-gtk2 debhelper dh-python libudev-dev libxenomai-dev tcl8.6-dev tk8.6-dev libreadline-gplv2-dev asciidoc dvipng graphviz groff imagemagick inkscape python-lxml source-highlight texlive-font-utils texlive-lang-cyrillic texlive-lang-french texlive-lang-german texlive-lang-polish texlive-lang-spanish w3c-linkchecker python-dev python-tk intltool libusb-1.0-0-dev yapps2 dblatex docbook-xsl texlive-extra-utils texlive-fonts-recommended texlive-latex-recommended xsltproc gettext autoconf asciidoc-dblatex libxaw7-dev bwidget libtk-img tclx python-gtk2
