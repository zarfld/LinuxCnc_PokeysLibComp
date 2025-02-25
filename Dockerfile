# Use Debian 12 as the base image
FROM debian:12

# Install necessary dependencies for LinuxCNC and PoKeysLib
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    linuxcnc-uspace-dev \
    git \
    python3 \
    python3-pip \
    curl \
    sudo \
    libmodbus-dev \
    libgpiod-dev \
    libgtk2.0-dev \
    libglade2-dev \
    libxmu-dev \
    libglu1-mesa-dev \
    libgl1-mesa-dev \
    libreadline-dev \
    libjansson-dev \
    libboost-python-dev \
    libboost-thread-dev \
    libboost-system-dev \
    libboost-all-dev \
    python3-lxml \
    debhelper \
    dh-python \
    libudev-dev \
    tcl8.6-dev \
    tk8.6-dev \
    asciidoc \
    dvipng \
    graphviz \
    groff \
    imagemagick \
    inkscape \
    source-highlight \
    texlive-font-utils \
    texlive-lang-cyrillic \
    texlive-lang-french \
    texlive-lang-german \
    texlive-lang-polish \
    texlive-lang-spanish \
    w3c-linkchecker \
    python-dev-is-python3 \
    python-tk \
    intltool \
    libusb-1.0-0-dev \
    yapps2 \
    dblatex \
    docbook-xsl \
    texlive-extra-utils \
    texlive-fonts-recommended \
    texlive-latex-recommended \
    xsltproc \
    gettext \
    autoconf \
    asciidoc-dblatex \
    libxaw7-dev \
    bwidget \
    libtk-img \
    tclx

# Set up non-root user 'codespace' with sudo privileges
RUN useradd -m -s /bin/bash codespace && \
    echo 'codespace ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

# Copy the project files into the Docker image
COPY . /LinuxCnc_PokeysLibComp

# Set the working directory
WORKDIR /LinuxCnc_PokeysLibComp

# Set the entrypoint to run tests inside the Docker container
ENTRYPOINT ["sh", "install.sh"]
