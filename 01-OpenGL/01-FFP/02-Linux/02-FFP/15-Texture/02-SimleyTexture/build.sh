#!/bin/sh

rm -rf XWindow
gcc -o xwindow XWindow.c -lX11 -lGL -lGLU -lSOIL
./xwindow

