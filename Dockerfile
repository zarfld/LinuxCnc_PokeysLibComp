# Use the base image from jeffersonjhunt/linuxcnc-docker
FROM jeffersonjhunt/linuxcnc-docker:latest

# Install necessary dependencies for LinuxCNC and PoKeysLib
RUN apt-get update && apt-get install -y \
    cmake \
    python3 \
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

# Copy the project files into the Docker image
COPY . /LinuxCnc_PokeysLibComp

# Set the working directory
WORKDIR /LinuxCnc_PokeysLibComp

# Set the entrypoint to run tests inside the Docker container
ENTRYPOINT ["sh", "install.sh"]
