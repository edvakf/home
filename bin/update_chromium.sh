#!/bin/sh

if [[ $OSTYPE = "darwin10.0" ]]; then
  base="http://build.chromium.org/f/chromium/snapshots/Mac"
  version=`curl $base/LATEST`
  name="chrome-mac"
  zip="$base/$version/$name.zip"
  tmpdir=/tmp

  cd $tmpdir
  echo downloading varsion $version
  if [ -d $name ]; then
    rm -r $name
  fi
  curl $zip -o $name.zip
  unzip $name.zip

  if [ "$(ps -ax | grep Chromium.app | grep -v grep | wc -l | tr -d ' ')" == "0" ]; then
    echo "updating to build $version"
    rm -r /Applications/Chromium.app
    mv $name/Chromium.app /Applications/Chromium.app
  fi
elif [[ $OSTYPE = "cygwin" ]]; then
  base="http://build.chromium.org/f/chromium/snapshots/Win"
  version=`curl $base/LATEST`
  name="chrome-win32"
  zip="$base/$version/$name.zip"
  tmpdir="/cygdrive/c/Users/atsushi/Downloads"

  cd $tmpdir
  echo downloading varsion $version
  if [ -d $name ]; then
    rm -r $name
  fi
  curl $zip -o $name.zip
  unzip $name.zip
fi
