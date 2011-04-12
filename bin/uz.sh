#!/bin/sh

if [[ $# -lt 1 ]]; then
  echo "Usage: $0 some.zip"
  exit 1
fi

zip=$1
dir=${zip%.*}

unzip $zip -d $dir

if [[ `ls -1 $dir | wc -l` -eq 1 ]]; then
  for file in `ls $dir`; do # only one file anyway
    if [[ -d $dir/$file ]]; then
      echo "move up directory"
      tmp=$dir/temporary_unzip_directory
      mv $dir/$file $tmp
      mv $tmp/* $dir
      rm -r $tmp
    fi
  done
fi