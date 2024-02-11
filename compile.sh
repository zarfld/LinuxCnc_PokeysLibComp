#!/bin/bash

echo "Compiling pokeys.comp..."
halcompile --install pokeys.comp
echo "pokeys.comp compiled successfully."

echo "Compiling kbd48CNC.comp..."
halcompile --install kbd48CNC.comp
echo "kbd48CNC.comp compiled successfully."

echo "Compiling pokeys_homecomp.comp..."
halcompile --install pokeys_homecomp.comp
echo "pokeys_homecomp.comp compiled successfully."

echo "preparing prerequisites of pokeys_rt.comp..."
cp ../pokeyslib/Makefile.noqmake ../pokeyslib/Makefile.noqmake.rt
#modify the Makefile.noqmake.rt to exclude libusb-1.0
sed -i 's/ -lusb-1.0//g' ../pokeyslib/Makefile.noqmake.rt
#modify the Makefile.noqmake.rt to change line 'cp libPoKeys.so /usr/lib' to 'cp libPoKeys.so /usr/lib/linuxcnc/modules'
sed -i 's/cp libPoKeys.so \/usr\/lib/cp libPoKeys.so \/usr\/lib\/linuxcnc\/modules/g' ../pokeyslib/Makefile.noqmake.rt

sed -i 's/hid-libusb.c //g' ../pokeyslib/Makefile.noqmake.rt

sed -i 's/PoKeysLibFastUSB.c //g' ../pokeyslib/Makefile.noqmake.rt

#modify the Makefile.noqmake.rt to change line 'cp PoKeysLib.h /usr/include' to 'cp PoKeysLib.h  /usr/include/linuxcnc'
sed -i 's/cp PoKeysLib.h \/usr\/include/cp PoKeysLib.h  \/usr\/include\/linuxcnc/g' ../pokeyslib/Makefile.noqmake.rt

#add lines folloing line in the begining of ../pokeyslib/pokeyslib.h
#  #ifdef POKEYSLIB_USE_LIBUSB
#    #undef POKEYSLIB_USE_LIBUSB
#  #endif
sed -i 's/#define POKEYSLIB_USE_LIBUSB/#ifdef POKEYSLIB_USE_LIBUSB\n#undef POKEYSLIB_USE_LIBUSB\n#endif\n#define POKEYSLIB_USE_LIBUSB/g' ../pokeyslib/pokeyslib.h

# copy pokeys_rt.comp to ../pokeyslib
cp pokeys_rt.comp ../pokeyslib

# copy ../pokeyslib/Makefile.noqmake.rt and ../pokeyslib/PoKeysLib.h to current folder
cp ../pokeyslib/Makefile.noqmake.rt .
cp ../pokeyslib/PoKeysLib.h .
cp ../libPoKeys.so .

# stage, commit and push  Makefile.noqmake.rt and PoKeysLib.h
#git add Makefile.noqmake.rt
#git add PoKeysLib.h
#git commit -m "Makefile.noqmake.rt and PoKeysLib.h updated for pokeys_rt.comp"
#git push

# compile pokeyslib and install it to /usr/lib/linuxcnc/modules
cd ../pokeyslib
make -f Makefile.noqmake.rt install
# register the pokeyslib to be included in halcompile for pokeys_rt.comp

echo "Compiling pokeys_rt.comp..."
halcompile --install pokeys_rt.comp
echo "pokeys_rt.comp compiled successfully."