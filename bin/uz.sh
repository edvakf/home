#!/bin/sh

if [[ $# -lt 1 ]]; then
  echo "Usage: $0 some.zip"
  exit 1
fi

zip=$1
path=/tmp/unzip$RANDOM$RANDOM

echo "unzip in $path"
unzip $zip -d $path

dir=${zip%.*}
mkdir $dir

if [[ $? -ne 0 ]]; then
  exit 1
fi

if [[ `ls -1 $path | wc -l` -eq 1 ]]; then
  for file in `ls $path`; do # only one file anyway
    if [[ -d $file ]]; then
      echo "mv $path/$file/* $dir"
      mv $path/$file/* $dir
    else
      echo "mv $path/$file $dir"
      mv $path/$file $dir
    fi
  done
else
  echo "mv $path/* $dir"
  mv $path/* $dir
fi