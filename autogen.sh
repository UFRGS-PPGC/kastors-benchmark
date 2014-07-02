#!/bin/sh

if ! libtool --version > /dev/null
then
	# Perhaps we are on a Mac
	if ! glibtool --version > /dev/null
	then
	echo "GNU Libtool is missing, please install it."
	exit 1
	fi
fi
autoreconf -ivf -I m4

