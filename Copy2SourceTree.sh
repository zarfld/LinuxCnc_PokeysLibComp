# copies content of subfolder pokeys_rt to linuxcnc source tree in ../linuxcnc-dev/src/hal/user_comps/pokeys_rt

# check if linuxcnc-dev is present
if [ ! -d ../linuxcnc-dev ]; then
	echo "linuxcnc-dev not found in parent directory"
	exit 1
fi

# check if pokeys_rt is present
if [ ! -d pokeys_rt ]; then
	echo "pokeys_rt not found in current directory"
	exit 1
fi

# check if pokeys_rt is up to date
if [ -d ../linuxcnc-dev/src/hal/user_comps/pokeys_rt ]; then
	if [ ! -z "$(diff -r pokeys_rt ../linuxcnc-dev/src/hal/user_comps/pokeys_rt)" ]; then
		echo "pokeys_rt is not up to date"
		exit 1
	fi
fi

# copy pokeys_rt to linuxcnc-dev
cp -r pokeys_rt ../linuxcnc-dev/src/hal/user_comps/

echo "pokeys_rt copied to linuxcnc-dev"