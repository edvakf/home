#!/bin/sh

base="http://build.chromium.org/f/chromium/snapshots/chromium-rel-xp"
version=`curl $base/LATEST`
name="chrome-win32"
zip="$base/$version/$name.zip"

cd /cygdrive/c/Users/atsushi/Downloads
if [ -d $name ]; then
  rm -r $name
fi
curl $zip -o $name.zip
unzip $name.zip
