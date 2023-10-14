#!/usr/bin/env bash

#
# Script to generate DejaVuSansMono.png from DejaVuSansMono.ttf
#
# Requires msdfgen tool from https://github.com/Chlumsky/msdfgen
# and the montage tool from imagemagick
#

FONT_DIR=./assets/fonts/open-sans
FONT_NAME=OpenSans-Regular.ttf
mkdir -p tmp
for x in {{2..7},{A..F}}; do
  for y in {{0..9},{A..F}}; do
    echo 0x$x$y
    msdfgen msdf -font "$FONT_DIR/$FONT_NAME" 0x$x$y -size 40 40 -translate 10 8 -o tmp/$x$y.png
  done
done

montage tmp/*.png -geometry +0+0 -tile 16x DejaVuSansMono.png
convert DejaVuSansMono.png -define h:format=rgba -depth 8 DejaVuSansMono.h
