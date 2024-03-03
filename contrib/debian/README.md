
Debian
====================
This directory contains files used to package aidpd/aidp-qt
for Debian-based Linux systems. If you compile aidpd/aidp-qt yourself, there are some useful files here.

## aidp: URI support ##


aidp-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install aidp-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your aidp-qt binary to `/usr/bin`
and the `../../share/pixmaps/aidp128.png` to `/usr/share/pixmaps`

aidp-qt.protocol (KDE)

