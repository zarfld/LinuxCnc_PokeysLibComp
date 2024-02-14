#!/bin/bash

echo "Compiling pokeys.comp..."
halcompile -install pokeys.comp
echo "pokeys.comp compiled successfully."

echo "Compiling kbd48CNC.comp..."
halcompile -install kbd48CNC.comp
echo "kbd48CNC.comp compiled successfully."

# Check LinuxCNC version
linuxcnc_version=$(linuxcnc --version | awk '{print $2}')
required_version="2.9"

if [[ $(echo "$linuxcnc_version >= $required_version" | bc -l) -eq 1 ]]; then
    echo "Compiling pokeys_homecomp.comp..."
    halcompile -install pokeys_homecomp.comp
    echo "pokeys_homecomp.comp compiled successfully."
else
    echo "Skipping compilation of pokeys_homecomp.comp. LinuxCNC version is not higher than 2.9."
fi

echo "preparing prerequisites of pokeys_rt.comp..."
cp ../pokeyslib/Makefile.noqmake ../pokeyslib/Makefile.noqmake.rt
cp ../pokeyslib/PoKeysLib.h ../pokeyslib/PoKeysLibRt.h

#modify the Makefile.noqmake.rt to exclude libusb-1.0
sed -i 's/ -lusb-1.0//g' ../pokeyslib/Makefile.noqmake.rt

#remove -lusb-1.0 from LDFLAGS in Makefile.noqmake.rt
sed -i 's/LDFLAGS = -lusb-1.0/LDFLAGS = /g' ../pokeyslib/Makefile.noqmake.rt


#modify the Makefile.noqmake.rt to change line 'cp libPoKeys.so /usr/lib' to 'cp libPoKeys.so /usr/lib/linuxcnc/modules'
sed -i 's/cp libPoKeys.so \/usr\/lib/cp libPoKeys.so \/usr\/lib\/linuxcnc\/modules/g' ../pokeyslib/Makefile.noqmake.rt

sed -i 's/hid-libusb.c //g' ../pokeyslib/Makefile.noqmake.rt

sed -i 's/PoKeysLibFastUSB.c //g' ../pokeyslib/Makefile.noqmake.rt

#modify the Makefile.noqmake.rt to change line 'cp PoKeysLib.h /usr/include' to 'cp PoKeysLib.h  /usr/include/linuxcnc'
sed -i 's/cp PoKeysLib.h \/usr\/include/cp PoKeysLibRt.h  \/usr\/include\/linuxcnc/g' ../pokeyslib/Makefile.noqmake.rt

#add line '#undef POKEYSLIB_USE_LIBUSB' in the begining of ../pokeyslib/PoKeysLibRt.h
sed -i '1s/^/#undef POKEYSLIB_USE_LIBUSB\n/' ../pokeyslib/PoKeysLibRt.h

# copy pokeys_rt.comp to ../pokeyslib
cp pokeys_rt.comp ../pokeyslib

# copy ../pokeyslib/Makefile.noqmake.rt and ../pokeyslib/PoKeysLibRt.h to current folder
cp ../pokeyslib/Makefile.noqmake.rt .
cp ../pokeyslib/PoKeysLibRt.h .
cp ../libPoKeys.so .

# stage, commit and push  Makefile.noqmake.rt and PoKeysLibRt.h
#git add Makefile.noqmake.rt
#git add PoKeysLibRt.h
#git commit -m "Makefile.noqmake.rt and PoKeysLibRt.h updated for pokeys_rt.comp"
#git push

# compile pokeyslib and install it to /usr/lib/linuxcnc/modules
cd ../pokeyslib
make -f Makefile.noqmake.rt install
# register the pokeyslib to be included in halcompile for pokeys_rt.comp

echo "Compiling pokeys_rt.comp..."
halcompile --install pokeys_rt.comp
echo "pokeys_rt.comp compiled successfully."